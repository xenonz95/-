VPATH = ./lib

.PHONY: all clean
all : test lib

test : main.o liblist.so
	gcc main.o -L./lib -o $@ -llist

main.o : api.h

lib : liblist.so

liblist.so : list.o
	gcc -fPIC -shared -o $@ $^
#	@-mkdir ./lib
	@mv $@ ./lib/$@
list.o : list.h api.h

clean :
	@-rm *.o test ./lib/*.so