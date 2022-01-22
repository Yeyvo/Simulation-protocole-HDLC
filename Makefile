CC = gcc
OBJ = obj
SRC = src
FLAG = -Wall

output: $(OBJ)/main.o $(OBJ)/crc.o
	$(CC) $(FLAG) $(DEBUG) $(OBJ)/main.o $(OBJ)/crc.o -o output -lm

$(OBJ)/main.o: $(SRC)/main.c
	$(CC) $(FLAG) $(DEBUG) -c $(SRC)/main.c -o $(OBJ)/main.o -lm

$(OBJ)/crc.o: $(SRC)/crc/crc.c $(SRC)/crc/crc.h
	$(CC) $(FLAG) $(DEBUG) -c $(SRC)/crc/crc.c -o $(OBJ)/crc.o -lm


# debug: DEBUG = -DDEBUG
debug: DEBUG = -g

debug: output

clean: 
	rm -f output $(OBJ)/*.o
init:
	mkdir $(OBJ)