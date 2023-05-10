NCLUDE_DIR := ./inc
CFLAG = -I${INCLUDE_DIR} -Wall -O2
CC = /usr/bin/gcc
LDLIBS = -lm -lpthread -lxenctrl

vpath %.c ${SRC_PATH}

.PHONY: all clean
ALL = main

all: ${ALL}

clean:
	rm -rf ${ALL} *.o
	clear

%.o: %.c
	${CC} ${CFLAG} -c $^
main: ${ALL} *.o
	${CC} ${CFLAG} $^ ${LDLIBS} -o $@
