BIN=temp_alarm
CFLAGS=-Wall -Wextra -g
LIBS=
DSTD=/usr/bin
SRC=$(wildcard ./*.c)
OBJ=$(patsubst %.c, %.o, $(SRC))

all:$(BIN)

$@:$<
	$(CC) -o $@ $(CFLAGS) -c $<

$(BIN):$(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(LIBS)

clean:
	rm *o $(BIN)

install:
	install ./$(BIN) $(DSTD)

uninstall:
	rm -rf $(DSTD)/$(BIN)
