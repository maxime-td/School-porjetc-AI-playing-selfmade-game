CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = $(sdl2-config --cflags --libs)

DIRCOMM = 

#liste des fichiers source
SRC = $(wildcard $(DIRCOMM)*.c)

HEADERS = $(wildcard $(DIRCOMM)*.h) 

#liste des fichiers objets
OBJ = $(SRC:%.c=%.o)

main: $(OBJ) $(HEADERS)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)
	
main.o:$(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@
	

.PHONY: clean reclean

clean:
	rm -f $(OBJ) 

reclean:
	rm -f $(OBJ) 
