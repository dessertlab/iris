INCLUDE_DIR := ./inc
CFLAG = -I${INCLUDE_DIR} -Wall -O2
CC = /usr/bin/gcc
LDLIBS = -lm -lxenctrl

vpath %.c ${SRC_PATH}

.PHONY: all clean
ALL = IRIS_cli

all: ${ALL}

clean:
	rm -rf ${ALL} *.o 
	clear

%.o: %.c
	${CC} ${CFLAG} -c $^ -o $@




