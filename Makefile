CC = gcc
CFLAGS = -Wall -Wextra -Og -g #-fsanitize=address,undefined

DIRCOMM = src/

#liste des fichiers source
SRC = $(wildcard $(DIRCOMM)*.c)

HEADERS = $(wildcard $(DIRCOMM)*.h) 

#liste des fichiers objets
OBJ = $(SRC:%.c=%.o)

main: $(SRC) $(HEADERS)
	$(CC) $(SRC) -o $@ -lm -lSDL2_ttf -lSDL2_image -pthread $(CFLAGS) $(shell sdl2-config --cflags --libs)

.PHONY: clean reclean

clean:
	rm -f main

reclean:
	rm -f main
