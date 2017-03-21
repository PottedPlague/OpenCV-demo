#include <opencv2\opencv.hpp>
#include <iostream>
#include <ctime>
#include "xiApiPlusOcv.hpp"

using namespace std;
using namespace cv;

string intToString(int number)
{
	//this function has a number input and string output
	std::stringstream ss;
	ss << number;
	return ss.str();
}

int main()
{
	Mat frame;
	//open ximea camera
	xiAPIplusCameraOcv cam;
	// Retrieving a handle to the camera device
	printf("Opening first camera...\n");
	cam.OpenFirst();

	//Set exposure
	cam.SetExposureTime(10000); //10000 us = 10 ms
	// Note: The default parameters of each camera might be different in different API versions

	printf("Starting acquisition...\n");
	cam.StartAcquisition();

	string help[3] = { "First image captured!", "Second image captured!", "Third image captured!" };

	namedWindow("Example", WINDOW_AUTOSIZE);			
	moveWindow("Example", 800, 10);

	for (int i = 0; i < 3; i++)
	{
		frame = cam.GetNextImageOcvMat();
		frame = cam.GetNextImageOcvMat();
		frame = cam.GetNextImageOcvMat();
		frame = cam.GetNextImageOcvMat();

		//cvtColor(frame, frame, CV_BGR2GRAY);
		imshow("Example", frame);
		
		time_t t = time(0);
		struct tm* now = localtime(&t);
		string cap_time;

		cap_time = intToString(now->tm_mday) + "-" + intToString(now->tm_mon + 1) + "-" + intToString(now->tm_year + 1900) + " " + intToString(now->tm_hour) + "." + intToString(now->tm_min);
		imwrite("D:\\pic\\" + cap_time + ".bmp", frame);
		cout << cap_time << endl;
		cout << help[i] << endl;
		cout << "D:\\pic\\" + cap_time + ".bmp" << endl;
		
		waitKey(60100);
	}

	cout << "ALL DONE!" << endl;

	for (;;)
	{
		if (waitKey(30) == 27)
			break;
	}

	cam.StopAcquisition();
	cam.Close();
	printf("Camera closed.\n");

	waitKey(500);
	return 0;
}