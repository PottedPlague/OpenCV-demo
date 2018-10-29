#include "3dRecording.h"
#include <opencv2\opencv.hpp>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

void tdRecording()
{
	VideoCapture cap1(1);
	cap1.set(CAP_PROP_FRAME_HEIGHT, 720);
	cap1.set(CAP_PROP_FRAME_WIDTH, 1280);
	VideoCapture cap2(2);
	cap2.set(CAP_PROP_FRAME_HEIGHT, 720);
	cap2.set(CAP_PROP_FRAME_WIDTH, 1280);

	Mat frame1, frame2;
	string filename1 = "D:/pic/dual1.avi";
	string filename2 = "D:/pic/dual2.avi";
	int numFrame = 0;

	VideoWriter writer1(filename1, CV_FOURCC('M', 'J', 'P', 'G'), 30, cv::Size(cap1.get(CAP_PROP_FRAME_WIDTH), cap1.get(CAP_PROP_FRAME_HEIGHT)));
	VideoWriter writer2(filename2, CV_FOURCC('M', 'J', 'P', 'G'), 30, cv::Size(cap2.get(CAP_PROP_FRAME_WIDTH), cap2.get(CAP_PROP_FRAME_HEIGHT)));

	while (numFrame < 600)
	{
		cap1 >> frame1;
		cap2 >> frame2;
		writer1.write(frame1);
		writer2.write(frame2);
		cout << "Writing..." << endl;
	}

	cout << "Finish!" << endl;
}
