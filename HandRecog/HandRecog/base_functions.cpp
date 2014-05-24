#include "hand_functions.h"
#include "base_functions.h"
#include <cmath>
#include <iostream>

class Line
{
public:
	double maxX, maxY, minX, minY, stX, stY, enX, enY;
	int pre;
	void init()
	{
		maxX = 0; maxY = 0;
		minX = 100000; minY = 100000;		
		pre = 0;
	}
} l;

double m_tan(HandPoint &l, HandPoint &r)
{
	if (l.x == r.x)
		return MAXTAN;
	else
		return (l.y - r.y) / (r.x - l.x);
}

double distance(HandPoint &l, HandPoint &r)
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
	pressUp(VK_RSHIFT);

	pressDown(VK_SPACE);
	std::cout << "����" ;
}

void streight()
{
	pressUp(VK_RSHIFT);
	pressUp(VK_SPACE);
	pressUp(VK_DOWN);
	pressUp(VK_LEFT);
	pressUp(VK_RIGHT);

	pressDown(VK_UP);
	pressUp(VK_UP);
	std::cout << "ǰ��" /*<< GetAsyncKeyState(VK_UP) << ' ' << std::endl*/;
}

void control2(HandPoint &green, HandPoint &red, HandPoint &blue, HandPoint &yellow)
{
	if (! green.color)
	{
		if (l.pre)
		{
			if (l.maxX - l.minX < l.maxY - l.minY)
			{
				if (l.stX < l.enX)
				{
					//right
					pressDown(VK_ESCAPE);
					pressUp(VK_ESCAPE);
					std::cout << "ESC" << std::endl;
				}
				else
				{
					//left
					pressDown(VK_RETURN);
					pressUp(VK_RETURN);
					std::cout << "ENTERN" << std::endl;
				}
			}
			else
			{
				if (l.stY < l.enY)
				{
					//down
					pressDown(VK_DOWN);
					pressUp(VK_DOWN);
					std::cout << "DOWN" << std::endl;
				}
				else
				{
					//up
					pressDown(VK_UP);
					pressUp(VK_UP);
					std::cout << "UP" << std::endl;
				}

			}
		}
		l.pre = 0;
		return;
	}
	else
	{
		if (!l.pre)
		{
			l.stX = green.x;
			l.stY = green.y;
		}
		l.minX = min(l.minX, green.x);
		l.minY = min(l.minY, green.y); 
		l.maxX = max(l.maxX, green.x);
		l.maxY = max(l.maxY, green.y);
		l.enX = green.x;
		l.enY = green.y;		
	}
}

void control(HandPoint &blue, HandPoint &red, HandPoint &green, HandPoint &yellow)
{
	if (blue.x > green.x && blue.x > MAX_X / 2 && green.x < MAX_X / 2)
	{
		HandPoint tmp;
		tmp = blue; blue = green; green = tmp;
		tmp = red; red = yellow; yellow = tmp;
	}

	//Ư��

	//��Ư��
	if (green.x < MAX_X / 2 && blue.x < MAX_X / 2)
	{
		pressUp(VK_SPACE);
		pressUp(VK_DOWN);
		pressUp(VK_RIGHT);
		pressUp(VK_RSHIFT);

		pressDown(VK_UP);
		pressDown(VK_LEFT);
		pressDown(VK_RSHIFT);
		pressUp(VK_UP);
		pressUp(VK_LEFT);
		pressUp(VK_RSHIFT);
		std::cout << "��Ư��" << blue.x << ' ' << blue.y << '\t' << green.x << ' ' << green.y << ' ' << std::endl;
		return;
	}

	//��Ư��
	if (green.x > MAX_X / 2 && blue.x > MAX_X / 2)
	{
		pressUp(VK_SPACE);
		pressUp(VK_DOWN);
		pressUp(VK_LEFT);
		pressUp(VK_RSHIFT);

		pressDown(VK_UP);
		pressDown(VK_RIGHT);
		pressDown(VK_RSHIFT);
		pressUp(VK_UP);
		pressUp(VK_RIGHT);
		pressUp(VK_RSHIFT);
		std::cout << "��Ư��" << blue.x << ' ' << blue.y << '\t' << green.x << ' ' << green.y << ' ' << std::endl;
		return;
	}

	//ɲ��
	//if (red.color &&yellow.color)
	//{
	//	pressUp(VK_SPACE);
	//	pressUp(VK_UP);
	//	pressUp(VK_LEFT);
	//	pressUp(VK_RIGHT);
	//	pressUp(VK_RSHIFT);

	//	pressDown(VK_DOWN);
	//	std::cout << "ɲ��" << blue.x << ' ' << blue.y << '\t' << green.x << ' ' << green.y << ' ' << std::endl;
	//	return;
	//}
	//else
	//{
	//	pressUp(VK_DOWN);
	//	pressDown(VK_UP);
	//}

	//�ͷŵ���
	if (distance(blue, green) < THR_NRT)
	{
		//before press
		nitrogen();
		std::cout << blue.x << ' ' << blue.y << '\t' << green.x << ' ' << green.y << ' ' << std::endl;
		return;
	}


	double tempTan = m_tan(blue, green);
	if (tempTan > 0.57)
	{
		pressUp(VK_SPACE);
		pressUp(VK_DOWN);
		pressUp(VK_RSHIFT);
		pressUp(VK_RIGHT);
		pressUp(VK_UP);
		pressUp(VK_LEFT);

		pressDown(VK_UP);
		pressDown(VK_LEFT);
		pressUp(VK_UP);
		pressUp(VK_LEFT);
		std::cout << "��ת" << blue.x << ' ' << blue.y << '\t' << green.x << ' ' << green.y << ' ' << std::endl;
		return;
	}
	else
	if (tempTan < -0.57)
	{
		pressUp(VK_SPACE);
		pressUp(VK_DOWN);
		pressUp(VK_RSHIFT);
		pressUp(VK_LEFT);
		pressUp(VK_UP);
		pressUp(VK_RIGHT);

		pressDown(VK_UP);
		pressDown(VK_RIGHT);
		pressUp(VK_UP);
		pressUp(VK_RIGHT);
		std::cout << "��ת" << blue.x << ' ' << blue.y << '\t' << green.x << ' ' << green.y << ' ' << std::endl;
		return;
	}

	streight();
	std::cout << blue.x << ' ' << blue.y << '\t' << green.x << ' ' << green.y << ' ' << std::endl;
}

void init()
{
	
}

int main()
{
	int x;
	std::cin >> x;
	while (1)
	{
		streight();
		Sleep(1000);
	}
	return 0;
}