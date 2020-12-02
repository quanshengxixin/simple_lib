#ifndef __SIMPLE_MEMORY_H__
#define __SIMPLE_MEMORY_H__

void simple_memory_init(void *addr_base, unsigned int addr_size);
void simple_memory_uninit();
void *simple_memory_malloc(unsigned int buffer_size);
void simple_memory_free(void *buffer_addr);

#endif // __SIMPLE_MEMORY_H__
