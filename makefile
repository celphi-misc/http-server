CC = gcc
OPTIONS = -c -Wall -g
LIBS = -lmagic -lpthread
HEADERS = *.h
RECEIPE = server.o server_util.o http.o serve_file.o
TEST_RECEIPE = test.o server_util.o http.o serve_file.o

all: server
server: $(RECEIPE)
	$(CC) -o server $(RECEIPE) $(LIBS)
test: $(TEST_RECEIPE)
	$(CC) -o test $(TEST_RECEIPE) $(LIBS)

%.o: %.c
	$(CC) $(OPTIONS) $<

.PHONY: clean
clean:
	rm -f *.o server
