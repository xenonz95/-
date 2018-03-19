.PHONY : all clean test

all : out

out : main.c
	gcc main.c -o out

test : test.c
	gcc test.c -o test

clean : 
	-@rm *.o out test
