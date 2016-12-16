/*
Light spot tracking programme 
******************************
Author: Cong Sun
Date: 15/12/2016

Version: low_frame_rate
*/

#include <opencv2\opencv.hpp>   
#include <math.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <Windows.h>

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
	Mat trajectory = Mat::zeros(480, 640, CV_8UC3);		//create black empty image

	double coor[3][2];									//coordinates of light spots
	int j = 0;											//line drawing counter
	int k = 0;											//used to skip the first several loops, letting the camera ready
	double distance = 0.0;									//distance between the current point and the previous one
	double distance_10 = 0.0, distance_21 = 0.0;			//distances between adjacent three points
	double delta_dis = 0.0;

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

	//set the display console position
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, 0, 880, 550, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	//create two windows showing..   
	namedWindow("Original");				//the original video captured   
	moveWindow("Original", 150, 10);		//setting window position

	namedWindow("VideoCapture");			//the video after setting threshold   
	moveWindow("VideoCapture", 805, 10);	//setting window position

	namedWindow("Trajactory", WINDOW_NORMAL);
	moveWindow("Trajactory", 150, 520);



	//![Main loop]
	for (;;)
	{
		//clock_t startTime = clock();
		
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

		//cout << "Width : " << frame.cols << endl;
		//cout << "Height: " << frame.rows << endl;

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
				coor[0][0] = detectKeyPoint[0].pt.x;
				coor[0][1] = detectKeyPoint[0].pt.y;
				coor[1][0] = coor[0][0];
				coor[1][1] = coor[0][1];
				coor[2][0] = coor[0][0];
				coor[2][1] = coor[0][1];

				//do nothing but display coordinates
				//cout << coor[0][0] << ", " << coor[0][1] << endl;
				//cout << coor[1][0] << ", " << coor[1][1] << endl;
			}
			else
			{
				
				//![option I: velocity]
				//read current keypoint coordinates
				coor[0][0] = detectKeyPoint[0].pt.x;
				coor[0][1] = detectKeyPoint[0].pt.y;

				//calculate the distance between to adjacent points
				distance = sqrt((coor[1][0] - coor[0][0]) * (coor[1][0] - coor[0][0]) + (coor[1][1] - coor[0][1]) * (coor[1][1] - coor[0][1]));
				cout << "Distance: " << distance << endl;

				//[1]:line drawing and coordiantes displaying
				
				if (distance >= 0.0 && distance <= 25.5)
					line(trajectory, Point(coor[1][0], coor[1][1]), Point(coor[0][0], coor[0][1]), Scalar(0, 255, (distance * 10)), 1, 8);
				else if (distance > 25.5 && distance <= 51.0)
					line(trajectory, Point(coor[1][0], coor[1][1]), Point(coor[0][0], coor[0][1]), Scalar(0, (255 - (distance - 25.5) * 10), 255), 1, 8);
				else
					line(trajectory, Point(coor[1][0], coor[1][1]), Point(coor[0][0], coor[0][1]), Scalar(0, 0, 255), 1, 8);
				
				//[2]: dots drawing
				//circle(trajectory, Point(coor[0][0], coor[0][1]), 0.5, Scalar(255, 255, 255), -1);



				//cout << coor[0][0] << ", " << coor[0][1] << endl;
				//cout << coor[1][0] << "," << coor[1][1] << endl;
				cout << "Distance: " << left << setw(10) << distance << endl;
				
				//pass current point to x1 and y1, acting as the 'previous' point in the next loop
				coor[1][0] = coor[0][0];
				coor[1][1] = coor[0][1];
				//![option I end]
								

				/*
				//![option II: acceleration]
				//read current keypoint coordinates
				coor[0][0] = detectKeyPoint[0].pt.x;
				coor[0][1] = detectKeyPoint[0].pt.y;

				//calculate distances between each two adjacent points
				distance_10 = sqrt((coor[1][0] - coor[0][0]) * (coor[1][0] - coor[0][0]) + (coor[1][1] - coor[0][1]) * (coor[1][1] - coor[0][1]));
				distance_21 = sqrt((coor[2][0] - coor[1][0]) * (coor[2][0] - coor[1][0]) + (coor[2][1] - coor[1][1]) * (coor[2][1] - coor[1][1]));

				//difference in distance, with fixed time interval, this can be treated as acceleration as well
				delta_dis = abs(distance_10 - distance_21);

				//line drawing
				if (delta_dis >= 0.0 && delta_dis <= 25.5)
					line(trajectory, Point(coor[1][0], coor[1][1]), Point(coor[0][0], coor[0][1]), Scalar(0, 255, (delta_dis * 10)), 2, 8);
				else if (delta_dis > 25.5 && delta_dis <= 51.0)
					line(trajectory, Point(coor[1][0], coor[1][1]), Point(coor[0][0], coor[0][1]), Scalar(0, (255 - (delta_dis - 25.5) * 10), 255), 2, 8);
				else
					line(trajectory, Point(coor[1][0], coor[1][1]), Point(coor[0][0], coor[0][1]), Scalar(0, 0, 255), 2, 8);

				//print the two distances and acceleration
				cout << "Dis_1: " << left << setw(10) << distance_10 << "Dis_2: " << left << setw(10) << distance_21 << "Acc: " << left << setw(10) << delta_dis << endl;

				//transmitting values
				coor[2][0] = coor[1][0];
				coor[2][1] = coor[1][1];
				coor[1][0] = coor[0][0];
				coor[1][1] = coor[0][1];
				//![option II end]
				*/
			}
			j++;
		}
		//display videos   
		imshow("Original", gray);
		imshow("Trajactory", trajectory);
		imshow("VideoCapture", keyPointImage);

		//small delay before next loop   
		waitKey(5);

		//cout << double(clock() - startTime) / (double)CLOCKS_PER_SEC << " seconds." << endl;
	}
	return 0;
}