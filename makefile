CC = gcc
CFLAGS = -Wall -g

TARGET = navdi

HDR = \
	include/converter.h \
	include/csv_parser.h

OBJ = \
	src/main.o \
    src/converter.o \
	src/csv_parser.o

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o: src/main.c $(HDR)
converter.o: src/converter.c $(HDR)
csv_parser.o: src/csv_parser.c $(HDR)

clean:
	rm -rf $(OBJ) $(TARGET)
