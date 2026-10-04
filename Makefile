# FileGuard Makefile
CC      = gcc
CFLAGS  = -Wall -Wextra -g -Iinclude
SRC     = src/main.c src/disk.c
OBJ     = $(SRC:src/%.c=build/%.o)
HEADERS = $(wildcard include/*.h)
TARGET  = fileguard

.RECIPEPREFIX = >

all: $(TARGET)

$(TARGET): $(OBJ)
> $(CC) $(CFLAGS) -o $@ $(OBJ)

build/%.o: src/%.c $(HEADERS) | build
> $(CC) $(CFLAGS) -c $< -o $@

build:
> mkdir -p build

clean:
> rm -rf build $(TARGET)
> rm -f data/*.img

.PHONY: all clean
