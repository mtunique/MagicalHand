#include "hand_functions.h"
#include "base_functions.h"
#include <cmath>
#include <iostream>

double m_tan(HandPoint & l, HandPoint & r)
{
	if (l.x == r.x)
		return MAXTAN;
	else
		return (l.y - r.y) / (r.x - l.x);
}

double distance(HandPoint & l, HandPoint & r)
{
	return sqrt((l.x - r.x)*(l.x - r.x) + (l.y - r.y)*(l.y - r.y));
}

void pressDown(BYTE key)
{
	if (!GetAsyncKeyState(key))
		keybd_event(key, 0, 0, 0);
}

void pressUp(BYTE key)
{
	if (GetAsyncKeyState(key))
		keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
}

void nitrogen()
{
	pressUp(VK_LEFT);
	pressUp(VK_RIGHT);

	pressDown(VK_RSHIFT);
	pressUp(VK_RSHIFT);
	std::cout << "µªÆø";
}

void control(HandPoint & blue, HandPoint & red, HandPoint & green, HandPoint & yellow)
{
	//É²³µ
	if (red.color && yellow.color)
	{
		if (!GetAsyncKeyState(VK_DOWN))
			//before press
			pressUp(VK_UP);
		pressUp(VK_LEFT);
		pressUp(VK_RIGHT);

		pressDown(VK_DOWN);
		std::cout << "É²³µ";
		return;
	}
	else
		pressUp(VK_DOWN);

	//ÊÍ·ÅµªÆø
	if (distance(blue, green) < THR_NRT)
	{
		//before press
		nitrogen();
		return;
	}

	//Æ¯ÒÆ
	if (green.x < blue.x)
	{
		//×óÆ¯ÒÆ
		if (green.y < blue.y)
		{
			pressUp(VK_DOWN);
			pressUp(VK_RIGHT);

			pressDown(VK_UP);
			pressDown(VK_LEFT);
			pressDown(VK_RSHIFT);
			std::cout << "×óÆ¯ÒÆ";
		}
		else
			//ÓÒÆ¯ÒÆ
		{
			pressUp(VK_DOWN);
			pressUp(VK_LEFT);

			pressDown(VK_UP);
			pressDown(VK_RIGHT);
			pressDown(VK_RSHIFT);
			std::cout << "ÓÒÆ¯ÒÆ";
		}
		return;
	}

	double tempTan = m_tan(blue, green);
	if (tempTan > 0.57)
	{
		pressUp(VK_DOWN);
		pressUp(VK_RSHIFT);
		pressUp(VK_RIGHT);

		pressDown(VK_UP);
		pressDown(VK_LEFT);
		std::cout << "×ó×ª";
		return;
	}
	else
	if (tempTan < -0.57)
	{
		pressUp(VK_DOWN);
		pressUp(VK_RSHIFT);
		pressUp(VK_LEFT);

		pressDown(VK_UP);
		pressDown(VK_RIGHT);
		std::cout << "ÓÒ×ª";
		return;
	}

	pressUp(VK_DOWN);
	pressUp(VK_LEFT);
	pressUp(VK_RIGHT);
	pressUp(VK_SHIFT);

	pressDown(VK_UP);
	std::cout << "Ç°ÐÐ";
}

int main()
{
	int x;
	std::cin >> x;
	while (1)
	{
		nitrogen();
		Sleep(1000);
	}
	return 0;
}