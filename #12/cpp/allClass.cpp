#include "allClass.h"

coordinate::coordinate(char x = 0, char y = 0)
{
	this.x = x;
	this.y = y;
}

coordinate coordinate::operator+(&A)
{
	return coordinate(x + A.x, y + A.y);
}

coordinate coordinate::operator-(&A)
{
	return coordinate(x - A.x, y - A.y);
}
//----------------------------------

display::display(string)
{
	//init screen buffer
	struct fb_fix_screeninfo fb_fix;
	struct fb_var_screeninfo fb_var;
	char *buf;
	if ((fd = open(dev, O_RDWR)) < 0)
	{
		printf("Unable to open framebuffer %s\n", dev);
		return;
	}
	if (ioctl(fd, FBIOGET_VSCREENINFO, &fb_var) < 0)
	{
		printf("Unable to get  FBIOGET_VSCREENINFO %s\n", dev);
		close(fd);
		return;
	}
	if (ioctl(fd, FBIOGET_FSCREENINFO, &fb_fix) < 0)
	{
		printf("Unable to get  FBIOGET_FSCREENINFO %s\n", dev);
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
}
void display::show()
{
	for (auto i = layer.begin(); i != layer.end(); i++)
	{
		for (auto j = i.begin(); j != i.end(); j++)
		{
			j.draw(buffer);
		}
	}
	memcpy(fd, buffer, height * width * sizeof(int));
}
void display::addItem(item work, int layerNum)
{
	layer[layerNum].push_back(work);
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
			if (i < 0 || i > box.height || j < 0 || j > box.width)
				continue;
			buffer[i * (box.width) + j] = color;
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

char item::out_code(int code)
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

void item::print_a_word(int x, int y, int charno, int color, int *buffer)
{

	int *font, tempx = x - 8 * multiplySize, tempy = y - 16, i, j;

	font = get_font(charno, color);
	for (i = 0; i < CHAR_HEIGHT * multiplySize; i++)
	{
		for (j = 0; j < helvB12_width[charno] * multiplySize; j++)
		{
			if (i < 0 || i > box.height || j < 0 || j > box.width)
				continue;
			if (font[i / multiplySize * helvB12_width[charno] + j / multiplySize])
			{
				buffer[(i + tempy) * (box.width) + (j + tempx)] = color;
			}
		}
	}
	lastWidth = multiplySize * helvB12_width[charno];
	// *x += helvB12_width[charno];
}

int item::codeToNo(int code)
{
	if (code >= 'a' && code <= 'z')
	{
		return code - 'a' + 65;
	}
	return 0;
}
point::point(coordinate _point, int _color = 0x00008888)
{
	point = _point;
	color = _color;
}
virtual point::draw(int *buffer)
{
	full_xy_area(point.x, point.y, point.x + 1, point.y + 1, color, buffer);
}
line::line(coordinate point1, coordinate point2, , int _color = 0x00888800)
{
	point1 = point1;
	point2 = point2;
	color = _color;
}
virtual line::draw(int *buffer)
{
	full_xy_line(point1.x, point1.y, point2.x, point2.y, color, buffer);
}
rectangle::rectangle(coordinate point1, coordinate point2, , int _color = 0x00008888)
{
	point1 = point1;
	point2 = point2;
	color = _color;
}
virtual rectangle::draw(int *buffer)
{
	full_xy_area(point1.x, point1.y, point2.x, point2.y, color, buffer);
}
virtual word::draw(int *buffer)
{
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
	FD_ZERO(&read_set);
	FD_SET(fileKey, &read_set);

}
char input::getWord()
{
}
char input::getDirection()
{
}
char input::getFun()
{
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