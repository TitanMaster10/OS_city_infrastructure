CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11
TARGET  = city_manager
MONITOR = monitor_reports
SCORER  = scorer
HUB     = city_hub
SRCS    = main.c operations.c permissions.c filter.c utils.c
OBJS    = $(SRCS:.c=.o)

all: $(TARGET) $(MONITOR) $(SCORER) $(HUB)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(MONITOR): monitor_reports.c city_manager.h
	$(CC) $(CFLAGS) -o $@ monitor_reports.c

$(SCORER): scorer.c
	$(CC) $(CFLAGS) -o $@ scorer.c

$(HUB): city_hub.c
	$(CC) $(CFLAGS) -o $@ city_hub.c

%.o: %.c city_manager.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGET) $(MONITOR) $(SCORER) $(HUB) $(OBJS)
