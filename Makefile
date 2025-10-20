HELL := /bin/bash
CC = g++
TARGET = dmalloc
CFLAGS = -Wall -Wextra -ggdb -O0
OBJS = dmalloc.o
all: $(TARGET)

$(TARGET) : $(OBJS)
		ar rcs libdmalloc.a $(OBJS)
%.o: src/%.c
		$(CC) $(CFLAGS) -c $<
clean:
		@rm -f $(OBJS) $(TARGET)
run:
		./$(TARGET)