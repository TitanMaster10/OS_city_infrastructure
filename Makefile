CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11
TARGET  = city_manager
SRCS    = main.c operations.c permissions.c filter.c utils.c
OBJS    = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c city_manager.h
	$(CC) $(CFLAGS) -c $

clean:
	rm -f $(TARGET) $(OBJS)