.PHONY:all clean work
work = test
OBJ = test.o
LIB = -lm
CC = gcc

all : $(work)

$(work) : $(OBJ)
	$(CC) $^ $(LIB) -o $@

copy :

clean :
	rm $(work) *.o copy
