CXXFLAGS = -std=c++11

main : main.cpp allhead.h allclass.h  _font.h allclass.cpp
	g++ $(CXXFLAGS) $^ -o $@


.PHONY : clean test

clean : 
	-rm main *.o test