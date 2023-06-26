CC = gcc
CFLAGS = -Wall -Wextra -g
DIRCOMM = 

#liste des fichiers source
SRC = $(wildcard $(DIRCOMM)*.c)

HEADERS = $(wildcard $(DIRCOMM)*.h) 

#liste des fichiers objets
OBJ = $(SRC:%.c=%)

goutte: $(SRC) $(HEADERS)
	$(CC) $(SRC) -o $@ $(shell sdl2-config --cflags --libs)

.PHONY: clean reclean

clean:
	rm -f $(OBJ) 

reclean:
	rm -f $(OBJ)
