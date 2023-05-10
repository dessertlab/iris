all: main
#-lm -lpthread -lxenctrl
main: main.o mylib.o #executor.o
	gcc -Wall -o main main.o mylib.o 
#executor.o
	
mylib.o: mylib.c
	gcc -c -Wall mylib.c

main.o: main.c
	gcc -c -Wall main.c

clean:
	rm main
	rm main.o
	rm mylib.o

	
