#include <stdio.h>
#include <assert.h>

#include "simple_log.h"

static simple_log_level_t simple_log_levels[SIMPLE_LOG_TYPE_NUM] = { SIMPLE_LOG_NON };

void set_loglevel(simple_log_type_t type, simple_log_level_t level)
{
	assert(type < SIMPLE_LOG_TYPE_NUM);
	assert(level < SIMPLE_LOG_LEVEL_NUM);

	simple_log_levels[type] = level;
}

simple_log_level_t get_loglevel(simple_log_type_t type)
{
	assert(type < SIMPLE_LOG_TYPE_NUM);

	return simple_log_levels[type];
}

void simple_log_print(const char *fcn, long line, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	fprintf(stdout, "[%s]: %ld: ", fcn, line);
	vfprintf(stdout, fmt, ap);
	putc('\n', stdout);

	va_end(ap);
}
