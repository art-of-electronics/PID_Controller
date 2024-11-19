# Variables
CC = gcc
CFLAGS = -fPIC
LDFLAGS = -shared
TARGET = libpid_controller.so
SRC = pid_controller.c

# Default target
all: $(TARGET)

# Rule to build the shared library
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

# Clean up
clean:
	rm -f $(TARGET)

