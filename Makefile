CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11
TARGET  = city_manager
MONITOR = monitor_reports
SRCS    = main.c operations.c permissions.c filter.c utils.c
OBJS    = $(SRCS:.c=.o)

all: $(TARGET) $(MONITOR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(MONITOR): monitor_reports.c city_manager.h
	$(CC) $(CFLAGS) -o $@ monitor_reports.c

%.o: %.c city_manager.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGET) $(MONITOR) $(OBJS)
