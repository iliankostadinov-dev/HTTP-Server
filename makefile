CC = gcc
CFLAGS = -g -Wall -pthread -Iinclude
SRC = $(wildcard src/*.c)
TARGET = http_srv

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	-rm -f $(TARGET)
