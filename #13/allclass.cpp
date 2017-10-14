//self defined
#include "allhead.h"
#include "allclass.h"
#include "_font.h"

//tool function------------------------------------------

static inline char _codeToASCII(int code)
{
	char c = -1;
	switch(code){
    	case 16:c = 'q';break;
        case 17:c = 'w';break;
        case 18:c = 'e';break;
        case 19:c = 'r';break;
        case 20:c = 't';break;
        case 21:c = 'y';break;
        case 22:c = 'u';break;
        case 23:c = 'i';break;
        case 24:c = 'o';break;
        case 25:c = 'p';break;
        case 30:c = 'a';break;
        case 31:c = 's';break;
        case 32:c = 'd';break;
        case 33:c = 'f';break;
        case 34:c = 'g';break;
        case 35:c = 'h';break;
        case 36:c = 'j';break;
        case 37:c = 'k';break;
        case 38:c = 'l';break;
        case 44:c = 'z';break;
        case 45:c = 'x';break;
        case 46:c = 'c';break;
        case 47:c = 'v';break;
        case 48:c = 'b';break;
        case 49:c = 'n';break;
        case 50:c = 'm';break;
		default: /*printf ("|isn't a normal key");*/
			break;
	}
return c;
}
static inline void full_xy_area(int x1, int y1, int x2, int y2, int color, int *buffer,int box_height,int box_width)
{
	int i, j;
	if (x1 > x2)
	{
		x2 = x1 ^ x2;
		x1 = x1 ^ x2;
		x2 = x1 ^ x2;
	}
	if (y1 > y2)
	{
		y2 = y1 ^ y2;
		y1 = y1 ^ y2;
		y2 = y1 ^ y2;
	}
	for (i = y1; i <= y2; i++)
	{
		for (j = x1; j <= x2; j++)
		{
			if (i < 0 || i > box_height || j < 0 || j > box_width)
				continue;
			buffer[i * (box_width) + j] = color;
		}
	}
}

static inline void full_xy_line(int x1, int y1, int x2, int y2, int color, int *buffer,int box_height,int box_width)
{
	int dx, dy;
	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	if (dx >= dy)
		full_xy_area(x1, y1, x2, y1, color, buffer,box_height, box_width);
	else
		full_xy_area(x1, y1, x1, y2, color, buffer, box_height, box_width);
}

static inline int *get_font(int no, int color)
{
	int *font, i, j;
	int temp;
	font = (int *)malloc(helvB12_width[no] * CHAR_HEIGHT * sizeof(int));
	for (i = 0; i < CHAR_HEIGHT; i++)
	{
		temp = helvB12_bits[no * CHAR_HEIGHT + (int)i];
		for (j = 0; j < helvB12_width[no]; j++)
		{
			if (temp & (1 << (16 - j)))
			{
				font[i * helvB12_width[no] + j] = color;
				//printf("%x\n",font[i * helvB12_width[no] + j]);
			}
			else
				font[i * helvB12_width[no] + j] = 0;
		}
	}
	return font;
}

static inline void print_a_word(char x, char y, int charno, int color, int *buffer,int box_height,int box_width,int multiplySize,int lastWidth)
{

	int *font, tempx = x - 8 * multiplySize, tempy = y - 16, i, j;

	font = get_font(charno, color);
	for (i = 0; i < CHAR_HEIGHT * multiplySize; i++)
	{
		for (j = 0; j < helvB12_width[charno] * multiplySize; j++)
		{
			if (i < 0 || i > box_height || j < 0 || j > box_width)
				continue;
			if (font[i / multiplySize * helvB12_width[charno] + j / multiplySize])
			{
				buffer[(i + tempy) * (box_width) + (j + tempx)] = color;
			}
		}
	}
	lastWidth = multiplySize * helvB12_width[charno];
	// *x += helvB12_width[charno];
}

//tool function end--------------------------------------


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

//------------------------------------

//------------------------------------

//------------------------------------
input::input(string PATH)
{
	fileKey = open(PATH.c_str() , O_RDONLY);
	if (fileKey <= 0)
	{
		printf("open keyboard filed(%s)\n",PATH.c_str());
		exit(-1);
	}
	word = 0;
	direction = 0;
	function = 0;
}
void input::update()
{
	fd_set read_set;
	timeval tv;
	FD_ZERO(&read_set);
	FD_SET(fileKey, &read_set);
	tv.tv_sec = tv.tv_usec = 8333;//1E+6/(60*2)us to insure get the keydown before the screen refresh
	if(select(fileKey + 1,&read_set,NULL,NULL,&tv) > 0)
	{
		if(read(fileKey, &event, sizeof(event)) > 0)
		{
			if(event.type == EV_KEY)
			{
				if (event.code > 1 && event.code < 11)
				{
					if (event.value == 1)
					{
						function = event.code;
					}
				}
				if (event.code == 11)
				{
					if (event.value == 1)
					{
						function = 10;
					}
				}
				if(event.code == 28)
				{
					if(event.value == 1)
					{
						confirm = 1;
					}
				}
				if (event.code >= 16 && event.code <= 50)
				{
					if (event.value == 1)
					{
						word = _codeToASCII(event.code);
					}
				}
				if (event.code >= 102 && event.code <= 108)
				{
					if(event.value == 1 || event.value == 2)
					{
						if(event.code == 106)
							direction &= 0x01;
						if(event.code == 103)
							direction &= 0x02;
						if(event.code == 105)
							direction &= 0x04;
						if(event.code == 108)
							direction &= 0x08;
					}
				}
			}
		}
	}

}
char input::getWord()
{
	if (word >= 'a' && word <= 'z')
	{
		return word - 'a' + 65;
	}
	return 0;
}
char input::getDirection()
{
	char temp = direction;
	direction = 0;
	switch(temp)
	{
		case 0:return 0;break;
		case 1:return 1;break;
		case 2:return 3;break;
		case 3:return 2;break;
		case 4:return 5;break;
		case 5:return 0;break;
		case 6:return 4;break;
		case 7:return 3;break;
		case 8:return 7;break;
		case 9:return 8;break;
		case 10:return 0;break;
		case 11:return 1;break;
		case 12:return 6;break;
		case 13:return 7;break;
		case 14:return 5;break;
		case 15:return 0;break;
		default:return 0;break;
	}
}
int input::getFun()
{
	return function;
}
 bool input::getConfirm()
 {
	 bool temp = confirm;
	 confirm = 0;
	 return temp;
 }
//----------------------------------
funMode::funMode()
{
	function = 0;
	mode = 0;
}
void funMode::set(int a, int b)
{
	function = a;
	mode = b;
}
void funMode::used()
{
	--mode;
}
//------------------------------------
