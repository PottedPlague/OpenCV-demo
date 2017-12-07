#include "connectedComp.h"
#include <opencv2\opencv.hpp>
#include "xiApiPlusOcv.hpp"
#include <stdio.h>
#include <ctime>

using namespace std;
using namespace cv;

void RTcontours()
{
	xiAPIplusCameraOcv cam;
	Mat frame, mask;
	clock_t startTime;
	int framerate;

	int max_lut_index, min_lut_index, max_lut_value, min_lut_value;
	int k = 0;	//frame counter

	printf("Opening first camera...\n");
	cam.OpenFirst();

	max_lut_index = cam.GetLookUpTableIndex_Maximum();
	min_lut_index = cam.GetLookUpTableIndex_Minimum();
	max_lut_value = cam.GetLookUpTableValue_Maximum();
	min_lut_value = cam.GetLookUpTableValue_Minimum();

	cout << "Max LUT index is:" << max_lut_index << endl;
	cout << "Min LUT index is:" << min_lut_index << endl;
	cout << "Max LUT value is:" << max_lut_value << endl;
	cout << "Min LUT value is:" << min_lut_value << endl;

	int operational_point = 200;	//(int)(max_lut_index / 2);
	for (int i = 0; i < max_lut_index; i++)
	{
		cam.SetLookUpTableIndex(i);
		if (i < operational_point)
			cam.SetLookUpTableValue(min_lut_value);
		else
			cam.SetLookUpTableValue(max_lut_value);
	}

	cam.EnableLookUpTable();

	//Set exposure
	cam.SetExposureTime(3000); //3000 us = 3 ms

	printf("Starting acquisition...\n");
	cam.StartAcquisition();

	namedWindow("Output", WINDOW_AUTOSIZE);
	startTime = clock();

	for (;;)
	{
		frame = cam.GetNextImageOcvMat();

		cvtColor(frame, mask, CV_GRAY2BGR);
		vector<Vec4i> hierarchy;
		vector<vector<Point> > contours;
		findContours(frame, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

		vector<Moments> M(contours.size());
		vector<Point2f> MC(contours.size());
		for (size_t i = 0; i < contours.size(); i++)
		{
			M[i] = moments(contours[i]);
		}

		for (size_t i = 0; i < contours.size(); i++)
		{
			MC[i] = Point2f(M[i].m10 / M[i].m00, M[i].m01 / M[i].m00);
		}

		for (size_t i = 0; i < contours.size(); ++i)
		{
			// Calculate the area of each contour
			double area = contourArea(contours[i]);
			// Ignore contours that are too small or too large
			if (area < 1e2 || 1e5 < area) continue;
			// Draw each contour only for visualisation purposes
			drawContours(mask, contours, static_cast<int>(i), Scalar(0, 0, 255), 2, 8, hierarchy, 0);
			circle(mask, MC[i], 4, Scalar(0, 0, 255), -1);
		}

		framerate = CLOCKS_PER_SEC / (clock() - startTime);
		startTime = clock();
		cout << "Current framerate: " << framerate << endl;

		imshow("Output", mask);
		if (waitKey(1) == 27)
			break;
		frame.release();
	}


cam.StopAcquisition();
cam.Close();
cout << "Acquisition ended." << endl;

}