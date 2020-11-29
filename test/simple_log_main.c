#include <stdio.h>
#include <assert.h>

#include "simple_log.h"

int main(int argc, char **argv)
{
	int simple_log_type = 0;
	for (simple_log_type = 0; simple_log_type < SIMPLE_LOG_LEVEL_NUM; simple_log_type++) {
		simple_log_all(simple_log_type, "hello world! %d", simple_log_type);
		simple_log_err(simple_log_type, "hello world! %d", simple_log_type);
		simple_log_warn(simple_log_type, "hello world! %d", simple_log_type);
		simple_log_msg(simple_log_type, "hello world! %d", simple_log_type);
		simple_log_dbg(simple_log_type, "hello world! %d", simple_log_type);
	}


}
