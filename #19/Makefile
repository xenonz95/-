.PHONY:all clean work
work = Zflock try Zfcntl


all : $(work) file.txt

$(work) : 

file.txt :
	touch file.txt
	echo "some text" > file.txt

clean :
	rm $(work) *.txt *.o

refresh :
	rm *.txt
	touch file.txt
	echo "some text" > file.txt