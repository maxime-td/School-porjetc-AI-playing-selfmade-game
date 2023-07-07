CC = gcc
CFLAGS =-Wall -Wextra -MMD -O3 $(sdl2-config --cflags) #-g -fsanitize=address,undefined
LDFLAGS =-lm -lSDL2 -lSDL2_ttf -lSDL2_image -pthread#-fsanitize=address,undefined

DIRCOMM = src/

#liste des fichiers source
SRC = $(wildcard $(DIRCOMM)*.c)
HEADERS = $(wildcard $(DIRCOMM)*.h)

#liste des fichiers objets
OBJ=$(addprefix build/,$(SRC:.c=.o))
DEP=$(addprefix build/,$(SRC:.c=.d))

main: $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

build/%.o: %.c $(HEADERS)
	@mkdir -p build
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean reclean

clean:
	rm -f $(OBJ) main

reclean:
	rm -f $(SRC)