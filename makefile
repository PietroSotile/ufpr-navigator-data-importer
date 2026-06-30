CC = gcc
CFLAGS = -Wall -g

TARGET = navdi

HDR = \
	include/converter.h

OBJ = \
	src/main.o \
    src/converter.o

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o: src/main.c $(HDR)
converter.o: src/converter.c $(HDR)

clean:
	rm -rf $(OBJ) $(TARGET)
