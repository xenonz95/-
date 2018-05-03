.PHONY: clean all

obj = client server

all: $(obj)

client: Client.c
	gcc $^ -o $@

server: Server.c
	gcc $^ -o $@
clean:
	rm $(obj)
