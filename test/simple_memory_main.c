
#include "simple_log.h"
#include "simple_memory.h"

int main(int argc, char **argv)
{
	int ret = 0;
	set_loglevel(SIMPLE_LOG_TYPE_MEMORY, SIMPLE_LOG_DBG);

	ret = simple_memory_init((void *)0xA0268000, 0xF0000);
	if (ret) {
		simple_log_err(SIMPLE_LOG_TYPE_MEMORY, "simple_memory_init error: %d", ret);
		return ret;
	}

	void *a = simple_memory_malloc(0xFFFF);
	void *b = simple_memory_malloc(0x10000);

	simple_memory_free(a);
	simple_memory_free(b);

	simple_memory_uninit();

	return 0;
}