#include "base_functions.h"
#include <cmath>

double m_tan(HandPoint & l, HandPoint & r)
{
	if (l.x == r.x)
		return MAXTAN;
	else
		return (r.y - l.y) / (r.x - l.x);
}

double distance(HandPoint & l, HandPoint & r)
{	
	return sqrt((l.x - r.x)*(l.x - r.x) + (l.y - r.y)*(l.y - r.y));
}