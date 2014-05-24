#ifndef __HAND_FUNCTIONS__
#define __HAND_FUNCTIONS__


static char *window_name = "Demo Window";


//lights color
const int LIGHTS_OUT = 0;
const int BLUE_LED = 1;
const int RED_LED = 2;
const int GREEN_LED = 3;
const int YELLOW_LED = 4;

const int MAX_X = 320;
const int MAX_Y = 240;



struct HandPoint
{
	double x;
	double y;
	int color;
};


#endif