CC = gcc
CFLAGS = -Wall -Wextra -g

DIRCOMM = src/

#liste des fichiers source
SRC = $(wildcard $(DIRCOMM)*.c)

HEADERS = $(wildcard $(DIRCOMM)*.h) 

#liste des fichiers objets
OBJ = $(SRC:%.c=%.o)

main: $(SRC) $(HEADERS)
<<<<<<< HEAD
	$(CC) $(SRC) -o $@ -lSDL2_ttf $(shell sdl2-config --cflags --libs)
=======
	$(CC) $(SRC) -o $@ -lSDL2_ttf -lm $(shell sdl2-config --cflags --libs)
>>>>>>> cce7c8075007bd3a24d018cc7e574d919806dcac

.PHONY: clean reclean

clean:
	rm -f $(OBJ) 

reclean:
	rm -f $(OBJ)