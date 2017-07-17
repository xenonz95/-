#ifndef FIFO_H
#define FIFO_H

class fifo {
	public:
		void init(int length);
		int in(const void* p,int len);
		int out(void* p,int len);
		int len(void);
		int avail(void);
		~fifo();
	private:
		char* space;
		int head,end;
		int mask;
};

#endif
