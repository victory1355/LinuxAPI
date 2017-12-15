all:client server

CPPFLAGS += -I ../lib

LDFLAGS += -L ../lib
LDFLAGS += -lvnet
LDFLAGS += -Wl,-rpath=../lib

client:client.c
	gcc $^ -o $@ $(CPPFLAGS) $(LDFLAGS)

server:server.c
	gcc $^ -o $@ $(CPPFLAGS) $(LDFLAGS)

clean:
	rm -rf client server
