#include "allClass.h"

#define box_height 1080
#define box_width 1920

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

//tool function end--------------------------------------


coordinate::coordinate(char x, char y)
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
//----------------------------------

display::display(string str)
{
	//init screen buffer
	struct fb_fix_screeninfo fb_fix;
	struct fb_var_screeninfo fb_var;
	char *buf;
	if ((fd = open(str.c_str(), O_RDWR)) < 0)
	{
		printf("Unable to open framebuffer %s\n", str.c_str());
		return;
	}
	if (ioctl(fd, FBIOGET_VSCREENINFO, &fb_var) < 0)
	{
		printf("Unable to get  FBIOGET_VSCREENINFO %s\n", str.c_str());
		close(fd);
		return;
	}
	if (ioctl(fd, FBIOGET_FSCREENINFO, &fb_fix) < 0)
	{
		printf("Unable to get  FBIOGET_FSCREENINFO %s\n", str.c_str());
		close(fd);
		return;
	}

	width = fb_var.xres;
	height = fb_var.yres;
	printf("framebuffer info: bits=%d, w=%d, h=%d, mem_line=%d, mem_size=%d\n",
		   fb_var.bits_per_pixel, fb_var.xres, fb_var.yres, fb_fix.line_length, fb_fix.smem_len);
	buf = (char *)mmap((void *)NULL, fb_fix.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (*(int *)buf == -1)
	{
		printf("Error: failed to map fb buffer to memory.|%d\n", errno);
		close(fd);
		return;
	}

	fb = (int *)buf;
	//init layer
	vector<item> emptyLayer;
	layer.push_back(emptyLayer);
	layer.push_back(emptyLayer);
	layer.push_back(emptyLayer);
	layer.shrink_to_fit();
	//init buffer
	buffer = new int[height * width];
	memset(buffer, 0, width * height * sizeof(int));
}
void display::show()
{
	for (auto i = layer.begin(); i != layer.end(); i++)
	{
		for (auto j = (*i).begin(); j != (*i).end(); j++)
		{
			(*j).draw(buffer);
		}
	}
	memcpy(fb, buffer, height * width * sizeof(int));
}
void display::addItem(item work, int layerNum)
{
	layer[layerNum].push_back(work);
}
void display::clearLayer(int layerNum)
{
	layer[layerNum].clear();
}

display::~display()
{
	delete buffer;
	fb = nullptr;
	close(fd);
}

//----------------------------------

void item::full_xy_area(int x1, int y1, int x2, int y2, int color, int *buffer)
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

void item::full_xy_line(int x1, int y1, int x2, int y2, int color, int *buffer)
{
	int dx, dy;
	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	if (dx >= dy)
		full_xy_area(x1, y1, x2, y1, color, buffer);
	else
		full_xy_area(x1, y1, x1, y2, color, buffer);
}

int *item::get_font(int no, int color)
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

void item::print_a_word(char x, char y, int charno, int color, int *buffer)
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

point::point(coordinate _point, int _color)
{
	pointCo = _point;
	color = _color;
}
void point::draw(int *buffer)
{
	full_xy_area(pointCo.x, pointCo.y, pointCo.x + 1, pointCo.y + 1, color, buffer);
}
line::line(coordinate point1, coordinate point2, int _color)
{
	point1 = point1;
	point2 = point2;
	color = _color;
}
void line::draw(int *buffer)
{
	full_xy_line(point1.x, point1.y, point2.x, point2.y, color, buffer);
}
rectangle::rectangle(coordinate point1, coordinate point2, int _color)
{
	point1 = point1;
	point2 = point2;
	color = _color;
}
void rectangle::draw(int *buffer)
{
	full_xy_area(point1.x, point1.y, point2.x, point2.y, color, buffer);
}
void word::setFont(char word)
{
	wordNum = word;
}
void word::draw(int *buffer)
{
	int *temp;
	temp = get_font(wordNum, 0x00ffffff);
	print_a_word(point.x, point.y, 0x00ffffff, buffer);
}
//---------------------------------
input::input(string PATH)
{
	fileKey = open(PATH , O_RDONLY);
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
	if(select(fileKey + 1,&read_set,NULL,NULL,tv) > 0)
	{
		if(read(fileKey, &event, sizeof(event)) > 0)
		{
			if(event.type == EV_KEY)
			{
				if (event.code > 1 && event.code < 11)
				{
					if (event.value == 1)
					{
						function = code;
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
char input::getFun()
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