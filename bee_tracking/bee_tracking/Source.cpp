/*
Light spot tracking programme 
******************************
Author: Cong Sun
Date: 15/12/2016

Version: low_frame_rate
*/

#include <opencv2\opencv.hpp>   
#include <math.h>

using namespace cv;
using namespace std;


int main()
{
	//open the default camera   
	VideoCapture cap(0);
	//check if the camera is opened successfully   
	if (!cap.isOpened())
	{
		return -1;
	}

	vector<KeyPoint> detectKeyPoint;					//vector to store the keypoints

	Mat keyPointImage;									//for displaying video with marked keypoints
	Mat trajectory = Mat::zeros(500, 700, CV_8UC3);		//create black empty image

	double x0 = 0.0, y0 = 0.0, x1 = 0.0, y1 = 0.0, x2 = 0.0, y2 = 0.0;		//coordinates of light spots
	int j = 0;								//line drawing counter
	int k = 0;								//used to skip the first several loops, letting the camera ready
	int distance = 0.0;

	//![SBD]  
	//set detector parameters  
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

	//instantiate a SBD pointer  
	Ptr<SimpleBlobDetector> sbd = SimpleBlobDetector::create(params);

	//create two windows showing..   
	namedWindow("Original");				//the original video captured   
	moveWindow("Original", 150, 300);		//setting window position

	namedWindow("VideoCapture");			//the video after setting threshold   
	moveWindow("VideoCapture", 830, 300);	//setting window position

	//![Main loop]
	for (;;)
	{
		k++;
		Mat frame, gray;

		//read every frame of the camera input   
		cap >> frame;

		//convert the original image to grayscale one
		if (frame.channels() == 3)
		{
			cvtColor(frame, gray, CV_BGR2GRAY);
		}
		else
		{
			frame.copyTo(gray);
		}

		//skip the first several frames
		if (k < 10)
		{
			continue;
		}

		//setting threshold to create binary image, using THRESH_BINARY_INVERTED   
		threshold(gray, gray, 252, 255, 1);

		//detect lightspots in the frame and store them in detectKeyPoint  
		sbd->detect(gray, detectKeyPoint);

		//mark the key points with red circles  
		drawKeypoints(frame, detectKeyPoint, keyPointImage, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	

		//![trajactory drawing] & [coordinates displaying]
		if (detectKeyPoint.size() > 0)		//check if there are keypoints in the frame
		{
			//skip the first two points to avoid connection with the origin
			if (j < 2)
			{
				//read current keypoint coordinates 
				x0 = detectKeyPoint[0].pt.x;
				y0 = detectKeyPoint[0].pt.y;
				x1 = x0;
				y1 = y0;

				//do nothing but display coordinates
				//cout << x0 << ", " << y0 << endl;
				//cout << x1 << ", " << y1 << endl;
			}
			else
			{
				//read current keypoint coordinates
				x0 = detectKeyPoint[0].pt.x;
				y0 = detectKeyPoint[0].pt.y;

				distance = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
				cout << distance << endl;
				//line drawing and coordiantes displaying
				if (distance >= 0 && distance < 52)
					line(trajectory, Point(x1, y1), Point(x0, y0), Scalar(0, 255, distance * 5), 2, 8);
				else if (distance >= 52 && distance < 103)
					line(trajectory, Point(x1, y1), Point(x0, y0), Scalar(0, 255 - (distance - 51) * 5, 255), 2, 8);
				else
					line(trajectory, Point(x1, y1), Point(x0, y0), Scalar(0, 0, 255), 2, 8);
				//cout << x0 << ", " << y0 << endl;
				//cout << x1 << "," << y1 << endl;
				
				//pass current point to x1 and y1, acting as the 'previous' point in the next loop
				x1 = x0;
				y1 = y0;
			}
			j++;
		}

		//display videos   
		imshow("Original", gray);
		imshow("Trajactory", trajectory);
		imshow("VideoCapture", keyPointImage);

		//small delay before next loop   
		waitKey(5);
	}
	return 0;
}