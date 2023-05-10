INCLUDE_DIR := ./inc
CFLAG = -I${INCLUDE_DIR} -Wall -O2
CC = /usr/bin/gcc
LDLIBS = -lm -lpthread -lxenctrl

vpath %.c ${SRC_PATH}

.PHONY: all clean
ALL = print_exit_distr

all: ${ALL}

clean:
	rm -rf ${ALL} *.o 
	clear

%.o: %.c
	${CC} ${CFLAG} -c $^ -o $@

xen_ver: print_exit_distr.o
	${CC} ${CFLAG} $^ ${LDLIBS} -o $@


