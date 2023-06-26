CC = gcc
CFLAGS = -Wall -Wextra -g

DIRCOMM = src/

#liste des fichiers source
SRC = $(wildcard $(DIRCOMM)*.c)

HEADERS = $(wildcard $(DIRCOMM)*.h) 

#liste des fichiers objets
OBJ = $(SRC:%.c=%.o)

main: $(OBJ) $(HEADERS)
	$(CC) $(OBJ) -o $@ $(shell sdl2-config --cflags --libs)
	
main.o:$(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean reclean

clean:
	rm -f $(OBJ) 

reclean:
	rm -f $(OBJ)
