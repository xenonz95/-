#include "head.h"

#define PATH "/dev/input/event4"

struct information
{
	int width;
	int height;
} box;

int *DISPLAY_MEM_BASE_fb = 0;
int *screen_buffer, *draw_buffer, *new_buffer;
int fd;
int lastWidth = 0, multiplySize = 3;
void fb_init(char *dev)
{
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
	box.width = fb_var.xres;
	box.height = fb_var.yres;
	printf("framebuffer info: bits=%d, w=%d, h=%d, mem_line=%d, mem_size=%d\n", fb_var.bits_per_pixel, fb_var.xres, fb_var.yres, fb_fix.line_length, fb_fix.smem_len);
	buf = (char *)mmap((void *)NULL, fb_fix.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (*(int *)buf == -1)
	{
		printf("Error: failed to map fb buffer to memory.|%d\n", errno);
		close(fd);
		box.width = 0;
		box.height = 0;
		return;
	}

	DISPLAY_MEM_BASE_fb = (int *)buf;
	return;
}

void clear_buffer(int *buffer)
{
	memset(buffer, 0, box.width * box.height * sizeof(int));
}

void full_xy_area(int x1, int y1, int x2, int y2, int color, int *buffer)
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

void full_xy_line(int x1, int y1, int x2, int y2, int color, int *buffer)
{
	int dx, dy;
	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	if (dx >= dy)
		full_xy_area(x1, y1, x2, y1, color, buffer);
	else
		full_xy_area(x1, y1, x1, y2, color, buffer);
}

char out_code(int code)
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

int *get_font(int no, int color)
{
	int *font, i, j;
	int temp;
	font = (int *)malloc(helvB12_width[no] * CHAR_HEIGHT * sizeof(int));
	for (i = 0; i < CHAR_HEIGHT; i++)
	{
		temp = helvB12_bits[no * CHAR_HEIGHT + (int)i];
		for (j = 0; j < helvB12_width[no]; j++)
		{
			if (temp & (1 << (16-j)))
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

void print_a_word(int x, int y, int charno, int color, int *buffer)
{
	
	int *font,tempx = x-8*multiplySize,tempy = y-16,i,j;
	
	font = get_font(charno, color);
	for(i=0;i<CHAR_HEIGHT*multiplySize;i++)
	{
		for(j=0;j<helvB12_width[charno]*multiplySize;j++)
		{
			if (i < 0 || i > box.height || j < 0 || j > box.width)
				continue;
			if(font[i/multiplySize * helvB12_width[charno] + j/multiplySize])
			{
				buffer[(i + tempy) * (box.width) + (j + tempx)] = color;
			}
		}
	}
	lastWidth = multiplySize*helvB12_width[charno];
	// *x += helvB12_width[charno];
}

int codeToNo(int code)
{
	if (code >= 'a' && code <= 'z')
	{
		return code - 'a' + 65;
	}
	return 0;
}






















int main()
{
	int i = 0, j = 0, file_key, pressed = 0, function = 0, mode = 0, charno = 83, code = -1;
	struct input_event input_ev_one;
	struct information pos, action, point1, point2, zero = {0, 0};

	file_key = open(PATH, O_NONBLOCK);
	fb_init("/dev/fb0");
	if (file_key <= 0)
	{
		printf("open keyboard(event4) filed\n");
		return -1;
	}
	action = zero;
	putchar('\n');
	screen_buffer = (int *)malloc(sizeof(int) * (box.width) * (box.height));
	draw_buffer = (int *)malloc(sizeof(int) * (box.width) * (box.height));
	new_buffer = (int *)malloc(sizeof(int) * (box.width) * (box.height));
	pos.width = box.width / 2;
	pos.height = box.height / 2;

	clear_buffer(DISPLAY_MEM_BASE_fb);
	while (1)
	{
		memcpy(draw_buffer, screen_buffer, sizeof(int) * (box.width) * (box.height));

		//full_xy_area(pos.width-5,pos.height-5,pos.width+5,pos.height+5,0x00000000,screen_buffer);
		if (mode > 0)
		{
			if (mode > 1)
			{
				//clear_buffer(screen_buffer);
				point2 = pos;
			}
			else
			{
				memcpy(new_buffer, draw_buffer, sizeof(int) * (box.width) * (box.height));
			}

			switch (function)
			{
			case 1:
				full_xy_area(point1.width, point1.height, point1.width + 1, point1.height + 1, 0x00008888, mode == 1 ? screen_buffer : draw_buffer);
				break;
			case 2:
				full_xy_line(point1.width, point1.height, point2.width, point2.height, 0x00880088, mode == 1 ? screen_buffer : draw_buffer);
				break;
			case 3:
				full_xy_area(point1.width, point1.height, point2.width, point2.height, 0x00008888, mode == 1 ? screen_buffer : draw_buffer);
				break;
			case 4:
				print_a_word(point2.width, point2.height, charno, 0x00ffffff, mode == 1 ? screen_buffer : draw_buffer);
				switch(mode)
				{
					case 3:print_a_word(point2.width, point2.height, charno, 0x00ffffff, screen_buffer);pos.width+=lastWidth+2;--mode;code = -1;break;
					case 2:print_a_word(point2.width, point2.height, charno, 0x00ffffff, draw_buffer);break;
					case 1:print_a_word(point2.width, point2.height, charno, 0x00ffffff, screen_buffer);break;
					default : break;
				}
				break;
			case 5:
				clear_buffer(screen_buffer);
				--mode;
				break;
			default:
				break;
			}
			if(mode == 1)
				--mode;
		}
		else
		{
			mode = 0;
		}
		if (read(file_key, &input_ev_one, sizeof input_ev_one) == sizeof input_ev_one)
		{
			if (input_ev_one.type == EV_KEY)
			{
				if (input_ev_one.value == 1)
				{
					switch (input_ev_one.code)
					{
					case KEY_UP:
						action.height = -1;
						++pressed;
						break;
					case KEY_LEFT:
						action.width = -1;
						++pressed;
						break;
					case KEY_DOWN:
						action.height = 1;
						++pressed;
						break;
					case KEY_RIGHT:
						action.width = 1;
						++pressed;
						break;
					case KEY_ESC:
						mode-=2;
						break;
					case KEY_1:
						function = 1, mode = 1, point1 = pos;
						break;
					case KEY_2:
						function = 2, mode = 2, point2 = point1 = pos;
						break;
					case KEY_3:
						function = 3, mode = 2, point2 = point1 = pos;
						break;
					case KEY_4:
						function = 4, mode = 2, point2 = pos;
						break;
					case KEY_5:
						function = 5, mode = 2, point1 = pos;
					case KEY_ENTER:
						--mode;
						break;
					default: code = out_code(input_ev_one.code);
						break;
					}
					//     if(input_ev_one.code == KEY_ESC)
					//         break;
					
				}
				else if (input_ev_one.value == 0)
				{
					switch (input_ev_one.code)
					{
					case KEY_UP:
						--pressed;
						break;
					case KEY_LEFT:
						--pressed;
						break;
					case KEY_DOWN:
						--pressed;
						break;
					case KEY_RIGHT:
						--pressed;
						break;
					default:
						break;
					}
					if (!pressed)
					{
						action.height = 0;
						action.width = 0;
					}
				}
			}
		}
		if(code > 0)
		{
			if(function == 4)
				++mode;
			charno = codeToNo(code);
		}
		pos.width = (pos.width + action.width + box.width) % box.width;
		pos.height = (pos.height + action.height + box.height) % box.height;
		if (mode != 1)
		{
			full_xy_area(pos.width - 5, pos.height - 5, pos.width + 5, pos.height + 5, 0x00ffffff, draw_buffer);
			memcpy(DISPLAY_MEM_BASE_fb, draw_buffer, sizeof(int) * (box.width) * (box.height));
		}
		else
		{
			full_xy_area(pos.width - 5, pos.height - 5, pos.width + 5, pos.height + 5, 0x00ffffff, new_buffer);
			memcpy(DISPLAY_MEM_BASE_fb, new_buffer, sizeof(int) * (box.width) * (box.height));
		}
	}

	clear_buffer(DISPLAY_MEM_BASE_fb);
	close(file_key);
	close(fd);
	return 0;
}