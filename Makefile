SHELL := /bin/bash
CC = ibm-clang++64
TARGET = libdmalloc.a
CFLAGS = -Wall -Wextra -ggdb -O0
OBJS = dmalloc.o
all: $(TARGET)

$(TARGET) : $(OBJS)
	ar rcs $(TARGET) $(OBJS)
%.o: %.cpp
	$(CC) $(CFLAGS) -c $<
clean:
	@rm -f $(OBJS) $(TARGET)
run:
	./$(TARGET)
