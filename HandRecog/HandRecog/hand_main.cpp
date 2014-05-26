/*
*A Hand_Recognization program to simulate the keyboard &
*mouse events on Windows OS. 2 Alternative modes that are
*normal_mode & game_mode. Game_mode is espacially for "NFS"
*/

#include <cv.h>
#include <highgui.h>
#include <iostream>

#include "hand_functions.h"
#include "base_functions.h"
#include "CameraDS.h"


using namespace std;
using namespace cv;

struct DetectPoint
{
	Point2f rp[4];
	Point2f center;
	double area;
	int id;
};

bool cmp(DetectPoint a, DetectPoint b)
{
	return a.area > b.area;
}

void cvSkinSegment(IplImage* img, IplImage* mask)
{
	CvSize imageSize = cvSize(img->width, img->height);
	IplImage *imgY = cvCreateImage(imageSize, IPL_DEPTH_8U, 1);
	IplImage *imgCr = cvCreateImage(imageSize, IPL_DEPTH_8U, 1);
	IplImage *imgCb = cvCreateImage(imageSize, IPL_DEPTH_8U, 1);


	IplImage *imgYCrCb = cvCreateImage(imageSize, img->depth, img->nChannels);
	cvCvtColor(img, imgYCrCb, CV_BGR2YCrCb);
	cvSplit(imgYCrCb, imgY, imgCr, imgCb, 0);
	int y, cr, cb, l, x1, y1, value;
	unsigned char *pY, *pCr, *pCb, *pMask;

	pY = (unsigned char *)imgY->imageData;
	pCr = (unsigned char *)imgCr->imageData;
	pCb = (unsigned char *)imgCb->imageData;
	pMask = (unsigned char *)mask->imageData;
	cvSetZero(mask);
	l = img->height * img->width;
	for (int i = 0; i < l; i++)
	{
		y = *pY;
		cr = *pCr;
		cb = *pCb;
		cb -= 109;
		cr -= 152;
		x1 = (819 * cr - 614 * cb) / 32 + 51;
		y1 = (819 * cr + 614 * cb) / 32 + 77;
		x1 = x1 * 41 / 1024;
		y1 = y1 * 73 / 1024;
		value = x1 * x1 + y1 * y1;
		if (y < 100)
		{
			(*pMask) = (value < 700) ? 255 : 0;
		}
		else
		{
			(*pMask) = (value < 850) ? 255 : 0;
		}
		pY++;
		pCr++;
		pCb++;
		pMask++;
	}
	cvReleaseImage(&imgY);
	cvReleaseImage(&imgCr);
	cvReleaseImage(&imgCb);
	cvReleaseImage(&imgYCrCb);
}

void ProcessContours(Mat source_img, Mat &edges_img, vector<vector<Point>> &contours_out)
{
	vector<vector<Point>> contours_tmp, contours_tmp_smooth;
	findContours(source_img, contours_tmp, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));
	contours_tmp_smooth = contours_tmp;
	//smooth filter the contours
	for (unsigned int i = 0; i < contours_tmp.size(); i++)
	{
		approxPolyDP(contours_tmp[i], contours_tmp_smooth[i], 2, true);
	}
	Mat out_line_img(source_img.size(), CV_8UC1, Scalar(255));
	drawContours(out_line_img, contours_tmp_smooth, -1, Scalar(0), 1);

	edges_img = out_line_img;
	contours_out = contours_tmp_smooth;
}

int main()
{
	//get ready for the camera and initialize some basic structures
	CCameraDS cameDS;
	int h_camera_count = CCameraDS::CameraCount();
	int h_camera_num = 0;

	IplImage *p_frame = 0;//used to receive from the camera
	IplImage * test_img_mask = cvCreateImage(Size(640, 480), IPL_DEPTH_8U, 1);//receive from the skin_detect
	Mat source_image, output_image, final_image;//take over from the "IplImage"||after filter output image||final output image
	Mat edge_image;

	vector<vector<Point>> contours_smooth;
	DetectPoint detect_point[MAX_OUTLINES];
	RotatedRect returnRect[MAX_OUTLINES];
	int outline_num = 0;

	double last_area[2];
	last_area[0] = last_area[1] = 1;//monitor the area change by threshold
	double area_now[2];
	int mx_size;//judge size for now

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

	//namedWindow(window_name, CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	Mat source;
	while (p_frame == cameDS.QueryFrame())
	{
		source = p_frame;
		cvSkinSegment(p_frame, test_img_mask);
		//filtering the image by 4
		cvErode(test_img_mask, test_img_mask, NULL, 1);
		cvDilate(test_img_mask, test_img_mask, NULL, 1);
		cvSmooth(test_img_mask, test_img_mask, CV_GAUSSIAN, 21, 0, 0);
		cvThreshold(test_img_mask, test_img_mask, 130, 255, CV_THRESH_BINARY);

		output_image = test_img_mask;
		imshow("skin detect", output_image);

		for (int row = 0; row < 100; row++)
		{
			for (int col = 0; col < output_image.cols; col++)
			{
				*(output_image.data + output_image.step[0] * row + output_image.step[1] * col) = 0;//B channel set to black
				*(output_image.data + output_image.step[0] * row + output_image.step[1] * col + output_image.elemSize1()) = 0;//G channel set to black
				*(output_image.data + output_image.step[0] * row + output_image.step[1] * col + output_image.elemSize1() * 2) = 0;//R channei set to black
			}
		}

		ProcessContours(output_image, edge_image, contours_smooth);

		outline_num = contours_smooth.size();
		if (outline_num > MAX_OUTLINES)
		{
			return -1;
		}
		vector<DetectPoint> vec_detect_point;
		for (int i = 0; i < outline_num; i++)
		{
			detect_point[i].id = i;
			returnRect[i] = minAreaRect(contours_smooth[i]);
			returnRect[i].points(detect_point[i].rp);
			detect_point[i].area = returnRect[i].size.area();
			detect_point[i].center = returnRect[i].center;

			vec_detect_point.push_back(detect_point[i]);
		}

		//sort and put them back to find out the biggest
		sort(vec_detect_point.begin(), vec_detect_point.end(), cmp);
		for (int i = 0; i < vec_detect_point.size(); i++)
		{
			detect_point[i] = vec_detect_point[i];
		}

		Mat rect_image(Size(640, 480), CV_8UC1, Scalar(255));
		for (int i = 0; i < 1; i++)
		{
			line(source, detect_point[i].rp[0], detect_point[i].rp[1], Scalar(0));
			line(source, detect_point[i].rp[1], detect_point[i].rp[2], Scalar(0));
			line(source, detect_point[i].rp[2], detect_point[i].rp[3], Scalar(0));
			line(source, detect_point[i].rp[3], detect_point[i].rp[0], Scalar(0));
		}

		//show out the rect image

		imshow("source", source);

		int c = waitKey(10);
		if (c == ' ' || c == 27)
		{
			break;
		}
	}

	//destroyWindow(window_name);

	cameDS.CloseCamera();

	return 0;
}
