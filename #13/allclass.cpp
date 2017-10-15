//self defined
//#include "allhead.h"
#include "allclass.h"
#include "_font.h"
//-----------------------
coordinate::coordinate(int x, int y)
{
	this->x = x;
	this->y = y;
}

coordinate coordinate::operator+(coordinate &A)
{
	return coordinate(x + A.x, y + A.y);
}

coordinate coordinate::operator-(coordinate &A)
{
	return coordinate(x - A.x, y - A.y);
}
coordinate &coordinate::debug()
{
	cout << '(' << (int)x << ',' << (int)y << ')' << endl;
	return *this;
}
int coordinate::index(int width)
{
	return y*width+x;
}
//-----------------------
int display::openfd()
{
	if ((fd = open(address.c_str(), O_RDWR)) < 0)
	{
		cout << "cant open feamebuffer\n";
		return -1;
	}
	struct fb_var_screeninfo fb_var;
	if (ioctl(fd, FBIOGET_VSCREENINFO, &fb_var) < 0)
	{
		printf("Unable to get  FBIOGET_VSCREENINFO %s\n", address.c_str());
		close(fd);
		return -1;
	}
	struct fb_fix_screeninfo fb_fix;
	if (ioctl(fd, FBIOGET_FSCREENINFO, &fb_fix) < 0)
	{
		printf("Unable to get  FBIOGET_FSCREENINFO %s\n", address.c_str());
		close(fd);
		return -1;
	}
	width = fb_var.xres;
	height = fb_var.yres;
	printf("framebuffer info: w=%d, h=%d\n", fb_var.xres, fb_var.yres);
	display_mem_fb = (int *)mmap((void *)NULL, fb_fix.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (*display_mem_fb < 0)
	{
		printf("Error: failed to map fb buffer to memory.|%d\n", errno);
		close(fd);
		width = 0;
		height = 0;
		return -1;
	}
	return 0;
}
display &display::debug()
{
	printf("info: w=%d, h=%d\n", width, height);
	for (int i = 0; i < 100000; i++)
	{
		display_mem_fb[i] = 0xFFFFFF;
	}
	return *this;
}
void display::draw(int *buffer, coordinate pos, coordinate size)
{
	coordinate p,tp;
	for (int i = 0; i < size.y; i++)
	{
		p = {0,i};
		tp = p+pos;
		//cout << buffer << endl;
		//cout << buffer+tp.index(width) << endl;
		memcpy(display_mem_fb+tp.index(width),buffer+i*size.x,size.x*sizeof(int));
	}
}
void display::draw(screen item)
{
	draw(item.buffer, item.pos, item.size);
}
//----------------------------------------

screen &screen::setPos(coordinate pos)
{
	screen::pos = pos;
	return *this;
}
screen &screen::setSize(coordinate size)
{
	screen::size = size;
	if (buffer != nullptr)
	{
		free(buffer);
	}
	buffer = new int[size.x * size.y];
	return *this;
}
screen &screen::fullColor(int color)
{
	for (int i = 0; i < size.y; i++)
	{
		for (int j = 0; j < size.x; j++)
		{
			buffer[i * size.x + j] = color;
		}
	}
	return *this;
}
screen &screen::debug()
{
	cout << "pos:";
	pos.debug();
	cout << "size:";
	size.debug();
}