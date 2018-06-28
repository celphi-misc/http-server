CC = gcc
OPTIONS = -c -Wall -g
LIBS = -lpthread
HEADERS = *.h
RECEIPE = server.o server_util.o http.o serve_file.o

all: server
server: $(RECEIPE)
	$(CC) -o server $(RECEIPE) $(LIBS)

%.o: %.c
	$(CC) $(OPTIONS) $<

.PHONY: clean
clean:
	rm -f *.o server
