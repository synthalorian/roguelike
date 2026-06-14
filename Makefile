CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
LDFLAGS = -lncurses

SRC = src/main.c src/map.c src/entity.c src/game.c src/render.c src/input.c
OBJ = $(SRC:.c=.o)
TARGET = roguelike

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
