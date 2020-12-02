#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "simple_log.h"
#include "simple_errno.h"
#include "pcm_format_conversion.h"
#include "simple_list.h"

struct free_area {
	struct list_head list;
	int i;
};

int main()
{
	int ret = SIMPLE_OK;
	LIST_HEAD(my_list);
	int index;
	for (index = 0; index < 10; index++) {
		struct free_area *head = (struct free_area *)malloc(sizeof(struct free_area));
		head->i = index + 1;
		list_add_tail(&head->list, &my_list);
	}

	struct free_area *pos;
	/*for (pos = list_entry((&my_list)->next, struct free_area, list); &pos->list != (&my_list); pos = list_next_entry(pos, list)) {
		printf("i = %d\t", pos->i);
	}*/
	/*list_for_each_entry(pos, struct free_area, &my_list, list) {
		printf("%d\n", pos->i);
	}*/
	struct list_head* pos_list;
	list_for_each(pos_list, &my_list) {
		pos = container_of(pos_list, struct free_area, list);
		printf("%d", pos->i);
	}
	
	printf("hello world£¬ %d\n", list_empty(&my_list));
	return ret;
}