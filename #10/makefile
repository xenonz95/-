VPATH = ./lib

.PHONY: all clean run
all : test lib

test : main.o libhlist.so
	gcc main.o -L./lib -o $@ -lhlist

main.o : api.h

lib : libhlist.so

libhlist.so : hlist.o
	gcc -fPIC -shared -o $@ $^
	@-mkdir ./lib
	@mv $@ ./lib/$@

hlist.o : list.h api.h hlist.c
	gcc -c -o $@ hlist.c -fPIC

clean :
	@-rm *.o test ./lib/*.so

run :
	@LD_LIBRARY_PATH=./lib ./main