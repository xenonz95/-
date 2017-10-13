CXXFLAGS = -std=c++11

main :_font.h main.cpp allClass.h allClass.cpp


test : _font.h test.cpp allClass.h allClass.cpp
	g++ test.cpp _font.h allClass.h allClass.cpp -o test

.PHONY : clean test

clean :
	-rm  main *.o test