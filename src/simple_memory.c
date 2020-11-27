#include <stdlib.h>
#include <pthread.h>

#include "simple_log.h"
#include "simple_list.h"

#include "simple_memory.h"

struct free_area {
    struct list_head list;
    void * addr;
    unsigned int order;
};

#define MAS_ORDER   11
#define PAGE_SHIFT  10
// #define MEM_SIZE_SHIFT  16

static unsigned int page_size = (1 << PAGE_SHIFT); // 1k in bytes
static void *total_addr = (void *)0xA0268000;
static unsigned int total_size = 0xF0000; // in bytes

struct free_area free_areas[MAS_ORDER] = {0};
struct free_area work_areas = {0};
pthread_mutex_t memory_mutex;

static inline int get_order(unsigned int size)
{
    int order;

    size = (size - 1) >> (PAGE_SHIFT - 1);
    order = -1;
    do {
        size >>= 1;
        order++;
    } while (size);
    return order;
}

int simple_memory_init(void *buffer_base, unsigned int buffer_size)
{
    int order = 0;
    struct free_area *pos;
    for (order = 0; order < MAS_ORDER; order++) {
        INIT_LIST_HEAD(&free_areas[order].list);
    }

    INIT_LIST_HEAD(&work_areas.list);

	total_addr = buffer_base;
	total_size = buffer_size;

    simple_log_dbg(SIMPLE_LOG_TYPE_MEMORY, "page_size = 0x%X, memory_size = 0x%X, memory_addr = 0x%X",
	               page_size, total_size, total_addr);

    order = get_order(total_size);
    simple_log_dbg(SIMPLE_LOG_TYPE_MEMORY, "total order = %d, 0x%x", order, page_size * (1 << order));
    struct free_area *area = (struct free_area *)malloc(sizeof(struct free_area));
    area->addr = total_addr;
    area->order = order;
    list_add(&area->list, &free_areas[order].list);

    for (order = 0; order < MAS_ORDER; order++) {
        if (!list_empty(&free_areas[order].list))
            list_for_each_entry(pos, &free_areas[order].list, list)
                simple_log_dbg(SIMPLE_LOG_TYPE_MEMORY, "order = %d, addr = 0x%x, size = %u",
				               pos->order, pos->addr, page_size * (1 << order));
    }

    pthread_mutex_init(&memory_mutex, NULL);
	return 0;
}

void simple_memory_uninit()
{
    struct free_area *pos;
    struct free_area *tmp;
    int i;
    pthread_mutex_lock(&memory_mutex);
    for (i = 0; i < MAS_ORDER; i++) {
        list_for_each_entry_safe(pos, tmp, &free_areas[i].list, list)
        {
            simple_log_dbg(SIMPLE_LOG_TYPE_MEMORY, "i = %d, addr =  %x\n", i, pos->addr);
            list_del(&(pos->list));
            free(pos);
        }
    }

    list_for_each_entry_safe(pos, tmp, &work_areas.list, list) {
        simple_log_dbg(SIMPLE_LOG_TYPE_MEMORY, "work order = %d, addr =  %x\n", pos->order, pos->addr);
        list_del(&(pos->list));
        free(pos);
    }

    pthread_mutex_unlock(&memory_mutex);
    pthread_mutex_destroy(&memory_mutex);
}

static inline void * expand(int order, int current_order, struct free_area *area)
{
    struct free_area *pos;
    if (order == current_order) {
        list_add(&area->list, &work_areas.list);
        return area->addr;
    }

    if (order > current_order) {
        return NULL;
    }

    current_order --;
    area->order --;
    pos = (struct free_area *)malloc(sizeof(struct free_area));
    pos->addr = area->addr + page_size * (1 << area->order);
    pos->order = area->order;
    list_add(&pos->list, &free_areas[pos->order].list);
    return expand(order, current_order, area);
}

void *simple_memory_malloc(unsigned int buffer_size)
{
    int current_order;
    struct free_area *area;
    struct free_area *pos;
    void *return_addr;
    int order = get_order(buffer_size);
    pthread_mutex_lock(&memory_mutex);
    for (current_order = order; current_order < MAS_ORDER; current_order++) {
        area = &free_areas[current_order];
        if (list_empty(&area->list))
            continue;
        pos = list_first_entry(&area->list, struct free_area, list);
        simple_log_dbg(SIMPLE_LOG_TYPE_MEMORY, "order = %d, current order = %d\n", order, current_order);
        simple_log_dbg(SIMPLE_LOG_TYPE_MEMORY, "addr = 0x%x, current order = %d\n", pos->addr, pos->order);
        list_del(&pos->list);
        return_addr = expand(order, current_order, pos);
        pthread_mutex_unlock(&memory_mutex);
        return return_addr;
    }
    pthread_mutex_unlock(&memory_mutex);
    return NULL;
}

static inline int area_is_buddy(struct free_area *pre, struct free_area *post)
{
    if (pre->order != post->order)
        return 0;

    if ((pre->addr + (page_size << pre->order)) != post->addr)
        return 0;

    if (((pre->addr - total_addr) % (page_size << (pre->order + 1))) != 0)
        return 0;
    return 1;
}

static inline void reduce(int order)
{
    struct free_area *tmp_pre;
    struct free_area *tmp_post;
    struct free_area *pre;
    struct free_area *post;
    if (order >= (MAS_ORDER - 1))
        return;
    list_for_each_entry_safe(pre, tmp_pre, &free_areas[order].list, list) {
        list_for_each_entry_safe(post, tmp_post, &free_areas[order].list, list) {
            if (area_is_buddy(pre, post)) {
                list_del(&(post->list));
                free(post);
                list_del(&(pre->list));
                pre->order++;
                list_add(&pre->list, &free_areas[pre->order].list);
                return reduce(pre->order);
            }
        }
    }
    return;
}

void simple_memory_free(void *buffer_addr)
{
    struct free_area *pos;

    pthread_mutex_lock(&memory_mutex);
    list_for_each_entry(pos, &work_areas.list, list) {
        if (pos->addr == buffer_addr) {
            simple_log_dbg(SIMPLE_LOG_TYPE_MEMORY, "order = %d, addr = 0x%x", pos->order, pos->addr);
            list_del(&(pos->list));
            list_add(&pos->list, &free_areas[pos->order].list);
            reduce(pos->order);
            pthread_mutex_unlock(&memory_mutex);
            return;
        }
    }
    pthread_mutex_unlock(&memory_mutex);
}
