
#include "SBD.h"
#include <opencv2\opencv.hpp>
#include <iostream>
#include "xiApiPlusOcv.hpp"

using namespace std;
using namespace cv;

int BIIMAGE_SENSITIVITY = 252;
const int THRESHOLD_TYPE = 1;

void SimpleBlobDetection()
{
	//open ximea camera
	xiAPIplusCameraOcv cam;
	// Retrieving a handle to the camera device
	printf("Opening first camera...\n");
	cam.OpenFirst();

	//Set exposure
	cam.SetExposureTime(4000); //4000 us = 4 ms
	// Note: The default parameters of each camera might be different in different API versions

	//cam.SetHeight(480);
	//cam.SetWidth(640);

	printf("Starting acquisition...\n");
	cam.StartAcquisition();
	Mat keyPointImage;
	int j = 0;
	int k = 0;
	string str;
	Point2d p0, p1, p2;

	namedWindow("Original Image", WINDOW_AUTOSIZE);

	//VideoCapture cap(1);
	vector<KeyPoint> detectKeyPoint;						

	//--------------------------------------------------------------------------------------------------
	//setting detector parameters

	SimpleBlobDetector::Params params;

	//filter by area
	params.filterByArea = true;
	params.minArea = 20;
	params.maxArea = 2000;

	//filter by circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.1;

	//filter by convexity
	params.filterByConvexity = true;
	params.minConvexity = 0.87;

	//filter by inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0.01;

	//--------------------------------------------------------------------------------------------------

	//instantiate a SBD pointer
	Ptr<SimpleBlobDetector> sbd = SimpleBlobDetector::create(params);

	createTrackbar("Threshold Level", "Original Image", &BIIMAGE_SENSITIVITY, 255, 0);
	 
	for (;;)
	{
		k++;
		Mat frame, gray, bi_image;
		//read every frame of the camera input
		//cap >> frame;
		frame = cam.GetNextImageOcvMat();
		if (frame.channels() == 3)
		{
			cvtColor(frame, gray, CV_BGR2GRAY);
		}
		else
		{
			frame.copyTo(gray);
		}
		
		if (k < 10)
			continue;

		//GaussianBlur(gray, gray, Size(31, 31), 0, 0);

		threshold(gray, bi_image, BIIMAGE_SENSITIVITY, 255, THRESHOLD_TYPE);

		sbd->detect(bi_image, detectKeyPoint);
		drawKeypoints(gray, detectKeyPoint, keyPointImage, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

		//stringstream ss;
		//ss << k;	 
		//putText(keyPointImage, ss.str(), Point(10, 20), 1, 1.0, Scalar(0, 0, 255), 1, CV_AA);

		imshow("Original Image", keyPointImage);
		imshow("Binary Image", bi_image);
		//imshow("Original Image", gray);

		if (waitKey(1) == 27)
			break;
	}
	printf("Done\n");
}