VPATH = ../src:../head:../

.PHONY : all clean


all: main libfifo.so *.o

main: libfifo.so main.o
	g++ main.o -L. -o main -lfifo

BufferManager.o:CircleBuffer.h BufferManager.cpp
	g++ -c ../src/BufferManager.cpp -fPIC 

CircleBuffer.o:CircleBuffer.cpp
	g++ -c ../src/CircleBuffer.cpp -fPIC

fifo.o:BufferManager.h CircleBuffer.h fifo.cpp
	g++ -c ../src/fifo.cpp -fPIC

main.o: fifo.h

libfifo.so:fifo.o CircleBuffer.o BufferManager.o
	g++ -fPIC -shared -o libfifo.so fifo.o CircleBuffer.o BufferManager.o

clean:
	rm *.o *.so


#由于编写时遇到了很多问题，咨询了一些人，所以现在的makefile比较混乱，下一步再修改，目前优先把功能实现。