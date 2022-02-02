.SSP: all

all: server client

server: main.cc
	g++ main.cc -o $@ -D DEBUG

.SSP: clean

clean: 
	rm -f server client