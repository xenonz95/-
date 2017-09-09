#include "allClass.h"

#define PATH "/dev/input/event4"

int main()
{
	int i = 0, j = 0;
	
	coordinate cursor,point1,point2,speed;
	display one("/dev/fb0");
	input in(PATH);
	funMode runState;
	cursor.x = one.width / 2;
	cursor.y = one.height / 2;

	while (1)
	{
		//runing process
		if(runState.mode > 1)
		{
			point2 = cursor;
		}
		if(runState.mode > 0)
		{
			
			item *temp;
			switch (runState.function)
			{
			case 1:temp = new point(point1);break;
			case 2:temp = new line(point1, point2);break;
			case 3:temp = new rectangle(point1, point2);break;
			case 4:temp = new point(point1);break;
			default:temp = new point(point1);break;
			}
			if(in.getConfirm())
			{
				runState.mode--;
			}
			switch(runState.mode)
			{
				case 0: runState.function = 0; break;
				case 1: one.addItem(*temp, 0);one.clearLayer(1);runState.mode--;break;
				case 2: one.clearLayer(1);one.addItem(*temp, 1);break;
				case 3: break;
				default : runState.mode = 0; break;
			}
			if (runState.function == 5)
			{
				one.clearLayer(0);
			}
			delete temp;
		}
		in.update();
		//for cursor
		switch(in.getDirection())
		{
			case 0: speed = {0,0};break;
			case 1: speed = {1,0};break;
			case 2: speed = {1,1};break;
			case 3: speed = {0,1};break;
			case 4: speed = {-1,1};break;
			case 5: speed = {-1,0};break;
			case 6: speed = {-1,-1};break;
			case 7: speed = {0,-1};break;
			case 8: speed = {1,-1};break;
			default : speed = {0,0};break;
		}
		//for funMode
		switch(in.getFun())
		{
			case 0: break;
			case 1: runState.function = 1;runState.mode = 1; point1 =  cursor;break;
			case 2: runState.function = 2;runState.mode = 2; point1 = point2 = cursor;break;
			case 3: runState.function = 3;runState.mode = 2; point1 = point2 = cursor;break;
			case 4: runState.function = 4;runState.mode = 2; point1 = cursor;break;
			case 5: runState.function = 5;runState.mode = 2; point1 = cursor;break;
			default:break;
		}
	}
	return 0;
}