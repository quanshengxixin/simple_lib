CC := gcc
##################################################################
# simple_log_main
##################################################################
SIMPLE_LOG_MAIN_SRCS = ./src/simple_log.c \
					./test/simple_log_main.c

SIMPLE_LOG_MAIN_INC = \
					-I ./include

SIMPLE_LOG_MAIN_CFLAGS := -g -Wall -O3

SIMPLE_LOG_MAIN_LDFLAGS :=

SIMPLE_LOG_MAIN_LIBS :=

SIMPLE_LOG_MAIN_OBJS := $(patsubst %.c,%.o,$(SIMPLE_LOG_MAIN_SRCS))

SIMPLE_LOG_MAIN_TARGET := simple_log_main


##################################################################
# build setting
##################################################################
all: $(SIMPLE_LOG_MAIN_TARGET)

clean:
	find . -name "*.o" | xargs rm -r
	rm -rf $(SIMPLE_LOG_MAIN_TARGET)

$(SIMPLE_LOG_MAIN_OBJS):%.o:%.c 
	$(CC) $(SIMPLE_LOG_MAIN_CFLAGS) $(SIMPLE_LOG_MAIN_INC) -c $< -o $@
$(SIMPLE_LOG_MAIN_TARGET):$(SIMPLE_LOG_MAIN_OBJS)
	$(CC) -o $@ $(SIMPLE_LOG_MAIN_OBJS) $(SIMPLE_LOG_MAIN_LDFLAGS) $(SIMPLE_LOG_MAIN_LIBS)
