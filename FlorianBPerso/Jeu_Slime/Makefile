CC = gcc
CFLAGS = -Wall -Wextra -g
DIRCOMM = 

#liste des fichiers source
SRC = $(wildcard $(DIRCOMM)*.c)

HEADERS = $(wildcard $(DIRCOMM)*.h) 

#liste des fichiers objets
OBJ = $(SRC:%.c=%)

main: $(SRC) $(HEADERS)
	$(CC) $(SRC) -o $@ -lSDL2_image $(CFLAGS) $(shell sdl2-config --cflags --libs)


.PHONY: clean reclean

clean:
	rm -f $(OBJ) 

reclean:
	rm -f $(OBJ)
