
#ifndef BASE_FUNCTIONS_H
#define BASE_FUNCTIONS_H

#include <Windows.h>
struct HandPoint;

//type 
const double MAXTAN = 1000000000;
const double THR_NRT = 20;


//computation geometry
double m_tan(HandPoint &, HandPoint &);
double distance(HandPoint &, HandPoint &);
HandPoint getCenter(HandPoint &, HandPoint &);


void control(HandPoint &, HandPoint &, HandPoint &, HandPoint &);
void control2(HandPoint &blue, HandPoint &red, HandPoint &green, HandPoint &yellow);
void init();



//press key
void pressDown(BYTE);
void pressUp(BYTE);


#endif 