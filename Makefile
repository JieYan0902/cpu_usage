.PHONY: all clean

CC := /opt/buildroot-gcc342/bin/mipsel-linux-gcc
DIR := $(shell pwd)

OBJS := $(patsubst %.c,%.o,$(wildcard *.c))
SRC := $(patsubst %.o, %.c,$(OBJS))
CFLAGS := -O2
LDFLAGS := 

all : cpu_usage

cpu_usage : $(OBJS)
	$(CC) -o cpu_usage -O2 $(LDFLAGS) $^
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o;
	rm -rf cpu_usage;

