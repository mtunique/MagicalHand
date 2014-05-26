#include <WinSock2.h>
#include "hand_functions.h"
#include "base_functions.h"
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>

#pragma  comment(lib,"ws2_32.lib")

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


void dan(double  ddd){
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return;
	}

	SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{
		printf("invalid socket !");
		return;
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8899);
	serAddr.sin_addr.S_un.S_addr = inet_addr("192.168.121.183");
	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		printf("connect error !");
		closesocket(sclient);
		return;
	}
	
	char * sendData = "pwm 11 1000 10";
	std::cout << ddd;
	send(sclient, sendData, strlen(sendData), 0);

	char recData[255];
	int ret = recv(sclient, recData, 255, 0);
	if (ret > 0)
	{
		recData[ret] = 0x00;
		printf(recData);
	}
	closesocket(sclient);
	WSACleanup();
	return ;
}

double m_tan(HandPoint &l, HandPoint &r)
{
	return ((l.y * r.y) + (r.x * l.x) )/ sqrt(r.x*r.x + r.y * r.y) * sqrt(l.x*r.x + l.y * l.y);
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
	std::cout << "µªÆø" ;
}

void streight()
{
	pressUp(VK_RSHIFT);
	pressUp(VK_SPACE);
	pressUp(VK_DOWN);
	pressUp(VK_LEFT);
	pressUp(VK_RIGHT);

	pressDown(0x41);
	pressUp(0x41);
	std::cout << "Ç°ÐÐ" /*<< GetAsyncKeyState(VK_UP) << ' ' << std::endl*/;
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
	//if (blue.x > green.x && blue.x > MAX_X / 2 && green.x < MAX_X / 2)
	//{
	//	HandPoint tmp;
	//	tmp = blue; blue = green; green = tmp;
	//	tmp = red; red = yellow; yellow = tmp;
	//}

	////Æ¯ÒÆ

	////×óÆ¯ÒÆ
	//if (green.x < MAX_X / 2 && blue.x < MAX_X / 2)
	//{
	//	pressUp(VK_SPACE);
	//	pressUp(VK_DOWN);
	//	pressUp(VK_RIGHT);
	//	pressUp(VK_RSHIFT);

	//	pressDown(VK_UP);
	//	pressDown(VK_LEFT);
	//	pressDown(VK_RSHIFT);
	//	pressUp(VK_UP);
	//	pressUp(VK_LEFT);
	//	pressUp(VK_RSHIFT);
	//	std::cout << "×óÆ¯ÒÆ" << blue.x << ' ' << blue.y << '\t' << green.x << ' ' << green.y << ' ' << std::endl;
	//	return;
	//}

	////ÓÒÆ¯ÒÆ
	//if (green.x > MAX_X / 2 && blue.x > MAX_X / 2)
	//{
	//	pressUp(VK_SPACE);
	//	pressUp(VK_DOWN);
	//	pressUp(VK_LEFT);
	//	pressUp(VK_RSHIFT);

	//	pressDown(VK_UP);
	//	pressDown(VK_RIGHT);
	//	pressDown(VK_RSHIFT);
	//	pressUp(VK_UP);
	//	pressUp(VK_RIGHT);
	//	pressUp(VK_RSHIFT);
	//	std::cout << "ÓÒÆ¯ÒÆ" << blue.x << ' ' << blue.y << '\t' << green.x << ' ' << green.y << ' ' << std::endl;
	//	return;
	//}

	////É²³µ
	////if (red.color &&yellow.color)
	////{
	////	pressUp(VK_SPACE);
	////	pressUp(VK_UP);
	////	pressUp(VK_LEFT);
	////	pressUp(VK_RIGHT);
	////	pressUp(VK_RSHIFT);

	////	pressDown(VK_DOWN);
	////	std::cout << "É²³µ" << blue.x << ' ' << blue.y << '\t' << green.x << ' ' << green.y << ' ' << std::endl;
	////	return;
	////}
	////else
	////{
	////	pressUp(VK_DOWN);
	////	pressDown(VK_UP);
	////}

	////ÊÍ·ÅµªÆø
	//if (distance(blue, green) < THR_NRT)
	//{
	//	//before press
	//	nitrogen();
	//	std::cout << blue.x << ' ' << blue.y << '\t' << green.x << ' ' << green.y << ' ' << std::endl;
	//	return;
	//}


	double tempTan = abs(m_tan(blue, green));
	dan(tempTan*100);
	
}

void init()
{
	
}

//int main()
//{
//	int x;
//	std::cin >> x;
//	while (1)
//	{
//		streight();
//		Sleep(10);
//	}
//	return 0;
//}