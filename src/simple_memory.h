#ifndef __MEMORY_H__
#define __MEMORY_H__

void memory_init();
void memory_uninit();
void *memory_malloc(unsigned int buffer_size);
void memory_free(void *buffer_addr);

#endif // __MEMORY_H__

