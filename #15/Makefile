.PHONY : all clean opttest

all : out

out : main.c
	gcc main.c -o out

opttest : opttest.c
	gcc opttest.c -o opttest

clean : 
	-@rm *.o
	-@rm out opttest
