all:
	gcc -g -Wall -Werror src/*.c -lm -o exec

run:
	./exec

zip:
	zip exercicio.zip src/*.c src/*.h Makefile