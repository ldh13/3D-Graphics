# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g `sdl2-config --cflags`
LDLFLAGS = `sdl2-config --libs` -lm

#Source files and target
SRCS = main.c graphics.c linalg.c
OBJS = $(SRCS:.c=.o)
TARGET = cube

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLFLAGS)
	
%.o: %.c graphics.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)