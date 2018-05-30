all: compil
compil: ternaire.o
	gcc ternaire.o -o ex -lm
	@echo Compilation finie.
%.o: %.c
	gcc -c -Wall -ansi $<