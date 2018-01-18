
#include <opencv2\opencv.hpp>
#include <stdio.h>
#include "xiApiPlusOcv.hpp"
#include <ctime>
#include "OLcontours.h"

using namespace std;
using namespace cv;

void _delayCheck(clock_t start_t)
{
	double timePassed;
	timePassed = 1000 * (clock() - start_t) / CLOCKS_PER_SEC;
	cout << "Time passed (ms): " << timePassed << endl;
}

void OLcontours()
{
	xiAPIplusCameraOcv cam;
	Mat frame;
	vector<Mat> frames;

	clock_t startTime;
	String filename;

	int max_lut_index, min_lut_index, max_lut_value, min_lut_value;
	int k = 0;	//frame counter
	int perc = 101;
	double timePassed;
	int framerate;


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
	startTime = clock();

	for (int j = 0; j < 600; j++)
	{

		//k++;

		frame = cam.GetNextImageOcvMat();
		frames.push_back(frame.clone());
		//imwrite("D:/pic/frame.png", frame);

		//stringstream ss;
		//ss << k;
		framerate = CLOCKS_PER_SEC / (clock() - startTime);
		startTime = clock();
		cout << "Current framerate: " << framerate << endl;
		//putText(frame, ss.str(), Point(20, 30), 1, 2.0, Scalar(255, 255, 255), 2, CV_AA);

		//imshow("camera feed", frame);
		/*if (waitKey(1) == 27)
		break;*/

		frame.release();
	}

	cam.StopAcquisition();
	cam.Close();
	cout << "Acquisition ended." << endl;

	cout << "Starting contours finding..." << endl;

	vector<Mat> masks(frames.size());
	for (int s = 0; s < frames.size(); s++)
	{
		cvtColor(frames[s], masks[s], CV_GRAY2BGR);
		vector<Vec4i> hierarchy;
		vector<vector<Point> > contours;
		findContours(frames[s], contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

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
			drawContours(masks[s], contours, static_cast<int>(i), Scalar(0, 0, 255), 2, 8, hierarchy, 0);
			circle(masks[s], MC[i], 4, Scalar(0, 0, 255), -1);
		}
		//sbd->detect(frames[s], detectKeyPoint);
		//drawKeypoints(frames[s], detectKeyPoint, frames[s], Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
		if (perc != 100 * s / frames.size())
		{
			perc = 100 * s / frames.size();
			cout << "Processing...(" << perc << "%)" << endl;
		}
	}

	cout << "All contours have been found." << endl;

	namedWindow("Output", WINDOW_AUTOSIZE);
	for (int s = 0; s < masks.size(); s++)
	{
		imshow("Output", masks[s]);
		waitKey(16); //60 fps
	}

	for (;;)
	{
		switch (waitKey(10))
		{
		case 114:	//if 'r' pressed, replay video
			for (int s = 0; s < masks.size(); s++)
			{
				imshow("Output", masks[s]);
				waitKey(16); //60 fps
			}

		case 27:
			waitKey(500);
			break;
		}
	}
}
