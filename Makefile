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
# simple_kernel_main
##################################################################
SIMPLE_KERNEL_MAIN_SRCS = ./test/simple_kernel_main.c \
						./src/simple_log.c

SIMPLE_KERNEL_MAIN_INC = \
					-I ./include

SIMPLE_KERNEL_MAIN_CFLAGS := -g -Wall -O3

SIMPLE_KERNEL_MAIN_LDFLAGS :=

SIMPLE_KERNEL_MAIN_LIBS :=

SIMPLE_KERNEL_MAIN_OBJS := $(patsubst %.c,%.o,$(SIMPLE_KERNEL_MAIN_SRCS))

SIMPLE_KERNEL_MAIN_TARGET := simple_kernel_main


##################################################################
# simple_memory_main
##################################################################
SIMPLE_MEMORY_MAIN_SRCS = ./test/simple_memory_main.c \
						./src/simple_log.c \
						./src/simple_memory.c

SIMPLE_MEMORY_MAIN_INC = \
					-I ./include

SIMPLE_MEMORY_MAIN_CFLAGS := -g -Wall -O3

SIMPLE_MEMORY_MAIN_LDFLAGS :=

SIMPLE_MEMORY_MAIN_LIBS :=

SIMPLE_MEMORY_MAIN_OBJS := $(patsubst %.c,%.o,$(SIMPLE_MEMORY_MAIN_SRCS))

SIMPLE_MEMORY_MAIN_TARGET := simple_memory_main


##################################################################
# simple_pcm_main
##################################################################
SIMPLE_PCM_MAIN_SRCS = ./test/simple_pcm_main.c \
						./src/simple_log.c \
						./src/simple_pcm.c

SIMPLE_PCM_MAIN_INC = \
					-I ./include

SIMPLE_PCM_MAIN_CFLAGS := -g -Wall -O3

SIMPLE_PCM_MAIN_LDFLAGS :=

SIMPLE_PCM_MAIN_LIBS :=

SIMPLE_PCM_MAIN_OBJS := $(patsubst %.c,%.o,$(SIMPLE_PCM_MAIN_SRCS))

SIMPLE_PCM_MAIN_TARGET := simple_pcm_main

##################################################################
# build setting
##################################################################
all: $(SIMPLE_LOG_MAIN_TARGET) $(SIMPLE_KERNEL_MAIN_TARGET) $(SIMPLE_MEMORY_MAIN_TARGET) \
	$(SIMPLE_PCM_MAIN_TARGET)

clean:
	find . -name "*.o" | xargs rm -r
	rm -rf $(SIMPLE_LOG_MAIN_TARGET) $(SIMPLE_KERNEL_MAIN_TARGET) $(SIMPLE_MEMORY_MAIN_TARGET) \
		$(SIMPLE_PCM_MAIN_TARGET)

$(SIMPLE_LOG_MAIN_OBJS):%.o:%.c
	$(CC) $(SIMPLE_LOG_MAIN_CFLAGS) $(SIMPLE_LOG_MAIN_INC) -c $< -o $@
$(SIMPLE_LOG_MAIN_TARGET):$(SIMPLE_LOG_MAIN_OBJS)
	$(CC) -o $@ $(SIMPLE_LOG_MAIN_OBJS) $(SIMPLE_LOG_MAIN_LDFLAGS) $(SIMPLE_LOG_MAIN_LIBS)

$(SIMPLE_KERNEL_MAIN_OBJS):%.o:%.c
	$(CC) $(SIMPLE_KERNEL_MAIN_CFLAGS) $(SIMPLE_KERNEL_MAIN_INC) -c $< -o $@
$(SIMPLE_KERNEL_MAIN_TARGET):$(SIMPLE_KERNEL_MAIN_OBJS)
	$(CC) -o $@ $(SIMPLE_KERNEL_MAIN_OBJS) $(SIMPLE_KERNEL_MAIN_LDFLAGS) $(SIMPLE_KERNEL_MAIN_LIBS)

$(SIMPLE_MEMORY_MAIN_OBJS):%.o:%.c
	$(CC) $(SIMPLE_MEMORY_MAIN_CFLAGS) $(SIMPLE_MEMORY_MAIN_INC) -c $< -o $@
$(SIMPLE_MEMORY_MAIN_TARGET):$(SIMPLE_MEMORY_MAIN_OBJS)
	$(CC) -o $@ $(SIMPLE_MEMORY_MAIN_OBJS) $(SIMPLE_MEMORY_MAIN_LDFLAGS) $(SIMPLE_MEMORY_MAIN_LIBS)

$(SIMPLE_PCM_MAIN_OBJS):%.o:%.c
	$(CC) $(SIMPLE_PCM_MAIN_CFLAGS) $(SIMPLE_PCM_MAIN_INC) -c $< -o $@
$(SIMPLE_PCM_MAIN_TARGET):$(SIMPLE_PCM_MAIN_OBJS)
	$(CC) -o $@ $(SIMPLE_PCM_MAIN_OBJS) $(SIMPLE_PCM_MAIN_LDFLAGS) $(SIMPLE_PCM_MAIN_LIBS)
