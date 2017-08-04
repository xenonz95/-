.PHONY = all clean

CXXFLAGS = -std=c++11

all : main.o  extractor.o
	g++ $^ -o main

main.o : extractor.h extractor.o

extractor.o : extractor.h

clean : 
	rm *.o main