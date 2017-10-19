#include "noise_capture.h"
#include <opencv2\opencv.hpp>
#include <iostream>
#include <ctime>
#include "xiApiPlusOcv.hpp"

using namespace std;
using namespace cv;

string int2String(int number);


void noise_capture()
{
	Mat frame;
	//open xim  ea camera
	xiAPIplusCameraOcv cam;
	// Retrieving a handle to the camera device
	printf("Opening first camera...\n");
	cam.OpenFirst();

	//Set exposure
	cam.SetExposureTime(10000); //10000 us = 10 ms
	// Note: The default parameters of each camera might be different in different API versions

	printf("Starting acquisition...\n");
	cam.StartAcquisition();

	namedWindow("Example", WINDOW_AUTOSIZE);
	moveWindow("Example", 800, 10);

	frame = cam.GetNextImageOcvMat();
	frame = cam.GetNextImageOcvMat();
	frame = cam.GetNextImageOcvMat();
	frame = cam.GetNextImageOcvMat();

	//cvtColor(frame, frame, CV_BGR2GRAY);
	imshow("Example", frame);

	time_t t = time(0);
	struct tm* now = localtime(&t);
	string cap_time;

	cap_time = int2String(now->tm_mday) + "-" + int2String(now->tm_mon + 1) + "-" + int2String(now->tm_year + 1900) + " " + int2String(now->tm_hour) + "." + int2String(now->tm_min);
	imwrite("D:\\pic\\" + cap_time + ".bmp", frame);
	cout << cap_time << endl;
	cout << "Image captured!" << endl;
	cout << "D:\\pic\\" + cap_time + ".bmp" << endl;


	cout << "ALL DONE!" << endl;

	cam.StopAcquisition();
	cam.Close();
	printf("Camera closed.\n");
}

string int2String(int number)
{
	//this function has a number input and string output
	std::stringstream ss;
	ss << number;
	return ss.str();
}