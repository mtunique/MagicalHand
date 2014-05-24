#ifndef __HAND_FUNCTIONS__
#define __HAND_FUNCTIONS__


static char *window_name = "Demo Window";


//lights color
const int LIGHTS_OUT = 0;
const int LIGHTS_ON = 1;

const int MAX_X = 640;
const int MAX_Y = 480;
const int MID_X = MAX_X / 2;
const int MID_Y = MAX_Y / 2;

const int MAX_OUTLINES = 2000;
const int DISPLAY_OUTLINE_NUM = 2;
const int AREA_CHANGE_THRESH = 0;

const double CHANGE_AREA_THRESH = 1.5;


struct HandPoint
{
	double x;
	double y;
	int color;
};


#endif