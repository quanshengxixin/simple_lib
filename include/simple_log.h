#ifndef __SIMPLE_LOG_H__
#define __SIMPLE_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

#define SIMPLE_LOG_TYPE_NUM		8

typedef enum {
	SIMPLE_LOG_TYPE_MAIN,
	SIMPLE_LOG_TYPE_CONVERSION,
	SIMPLE_LOG_TYPE_MEMORY,
	SIMPLE_LOG_TYPE_KERNEL,
} simple_log_type_t;

typedef enum SIMPLE_LOG_LEVEL {
	SIMPLE_LOG_NON = 0,
	SIMPLE_LOG_ALL = 1,
	SIMPLE_LOG_ERR,
	SIMPLE_LOG_WARN,
	SIMPLE_LOG_MSG,
	SIMPLE_LOG_DBG,
	SIMPLE_LOG_LEVEL_NUM,
} simple_log_level_t;

void set_loglevel(simple_log_type_t type, simple_log_level_t level);
simple_log_level_t get_loglevel(simple_log_type_t type);
void simple_log_print(const char *fcn, long line, const char *fmt, ...);

#define simple_log_all(type, ...)												\
do {  if (get_loglevel(type) > SIMPLE_LOG_NON) {									\
        simple_log_print(__FUNCTION__, __LINE__, "all: "__VA_ARGS__);         \
    }																			\
} while(0)

#define simple_log_err(type, ...)												\
do {  if (get_loglevel(type) > SIMPLE_LOG_ALL) {									\
        simple_log_print(__FUNCTION__, __LINE__, "error: "__VA_ARGS__);         \
    }																			\
} while(0)

#define simple_log_warn(type, ...)												\
do {  if (get_loglevel(type) > SIMPLE_LOG_ERR) {									\
        simple_log_print(__FUNCTION__, __LINE__, "warn: "__VA_ARGS__);         \
    }																			\
} while(0)

#define simple_log_msg(type, ...)												\
do {  if (get_loglevel(type) > SIMPLE_LOG_WARN) {									\
        simple_log_print(__FUNCTION__, __LINE__, "msg: "__VA_ARGS__);         \
    }																			\
} while(0)

#define simple_log_dbg(type, ...)												\
do {  if (get_loglevel(type) > SIMPLE_LOG_MSG) {									\
        simple_log_print(__FUNCTION__, __LINE__, "dbg: "__VA_ARGS__);         \
    }																			\
} while(0)

#ifdef __cplusplus
}
#endif

#endif // __SIMPLE_LOG_H__
