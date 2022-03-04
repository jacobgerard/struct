CC=gcc
CFLAGS=-g -Wall -O2
BIN=test
OBJ=test.o alist.o llist.o bst.o hashtable.o avl.o

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm *.o $(BIN)
