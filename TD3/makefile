all: compil
compil: main.o
	gcc main.o -o ex -lm
	@echo Compilation finie.
%.o: %.c
	gcc -c -Wall -ansi $<