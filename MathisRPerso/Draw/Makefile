CC = gcc
CFLAGS = -Wall -Wextra -g
DIRCOMM = 

#liste des fichiers source
SRC = $(wildcard $(DIRCOMM)*.c)

HEADERS = $(wildcard $(DIRCOMM)*.h) 

#liste des fichiers objets
OBJ = $(SRC:%.c=%)

snake: $(SRC) $(HEADERS)
	$(CC) $(SRC) -o $@ -lm -lSDL2_ttf $(CFLAGS) $(shell sdl2-config --cflags --libs)

.PHONY: clean reclean

clean:
	rm -f $(OBJ) 

reclean:
	rm -f $(OBJ)
