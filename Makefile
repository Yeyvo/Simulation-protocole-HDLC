CC = gcc
OBJ = obj
SRC = src
FLAG = -Wall

output: $(OBJ)/main.o $(OBJ)/crc.o $(OBJ)/utils.o $(OBJ)/hdlc.o
	$(CC) $(FLAG) $(DEBUG) $(OBJ)/main.o $(OBJ)/crc.o $(OBJ)/utils.o $(OBJ)/hdlc.o -o output -lm

$(OBJ)/main.o: $(SRC)/main.c
	$(CC) $(FLAG) $(DEBUG) -c $(SRC)/main.c -o $(OBJ)/main.o -lm

$(OBJ)/crc.o: $(SRC)/crc/crc.c $(SRC)/crc/crc.h
	$(CC) $(FLAG) $(DEBUG) -c $(SRC)/crc/crc.c -o $(OBJ)/crc.o -lm

$(OBJ)/utils.o: $(SRC)/utils/utils.c $(SRC)/utils/utils.h
	$(CC) $(FLAG) $(DEBUG) -c $(SRC)/utils/utils.c -o $(OBJ)/utils.o -lm

$(OBJ)/hdlc.o: $(SRC)/hdlc/hdlc.c $(SRC)/hdlc/hdlc.h
	$(CC) $(FLAG) $(DEBUG) -c $(SRC)/hdlc/hdlc.c -o $(OBJ)/hdlc.o -lm

# debug: DEBUG = -DDEBUG
debug: DEBUG = -g

debug: output

clean: 
	rm -f output $(OBJ)/*.o
init:
	mkdir $(OBJ)