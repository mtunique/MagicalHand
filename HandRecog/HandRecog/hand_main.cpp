/*
 *A Hand_Recognization program to simulate the keyboard & 
 *mouse events on Windows OS. 2 Alternative modes that are 
 *normal_mode & game_mode. Game_mode is espacially for "NFS"
 */

#include <cv.h>
#include <highgui.h>
#include <iostream>

#include "hand_functions.h"
#include "CameraDS.h"


using namespace std;
using namespace cv;


int main()
{
	//get ready for the camera and initialize some basic structures
	CCameraDS cameDS;
	int h_camera_count = CCameraDS::CameraCount();
	int h_camera_num = 0;

	IplImage *p_frame = 0;//used to receive from the camera
	Mat source_image;//take over from the "IplImage"

	//search for an available camera and initialize
	while (h_camera_num < h_camera_count)
	{
		if ((!cameDS.OpenCamera(h_camera_num, true)) || ((p_frame = cameDS.QueryFrame()) == NULL))
		{
			h_camera_num++;
		}
		else
		{
			break;
		}
		cameDS.CloseCamera();
	}

	//if the camera doesn't work, tell me
	if (h_camera_num == h_camera_count)
	{
		cout << "Cannot open camera" << endl;
		return -1;
	}

	namedWindow(window_name, CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	Mat smooth_test, hsv_test, channel_image[3];//--------------------------------

	while (p_frame == cameDS.QueryFrame())
	{
		source_image = p_frame;

		GaussianBlur(source_image, smooth_test, Size(17, 17), 0, 0);
		cvtColor(smooth_test, hsv_test, CV_BGR2HSV);
		split(hsv_test, channel_image);

		imshow(window_name, smooth_test);

		imshow("H", channel_image[0]);
		imshow("S", channel_image[1]);
		imshow("V", channel_image[2]);

		int c = waitKey(10);

		if (c == ' ' || c == 27)
		{
			break;
		}
	}
	destroyWindow(window_name);

	cameDS.CloseCamera();
	
	return 0;
}