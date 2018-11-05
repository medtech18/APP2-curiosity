all: main

ALL_TESTS= simple mesures marques blocs echanges if clones rotations ifignore for for2 boucle-if langton labyrinthe surprise


CC=clang
CFLAGS=-g -Wall -Wextra #-Werror
LDFLAGS=


# Ici, on utilise l'"intelligence" de 'make' qui saura tout seul
# comment créer les .o à partir des .c
main: main.o curiosity.o listes.o interprete.o


clean:
	rm -f main *.o
