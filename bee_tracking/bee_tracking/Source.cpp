/*
Light spot tracking programme 
******************************
Author: Cong Sun
Date: 10/01/2017

Version: 3d tracking & coordinates saved in files
*/

#include <opencv2\opencv.hpp>   
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <Windows.h>
#include <fstream>
#include <iterator>
#include <vector>

//#include "portaudio.h"
//#include "pa_asio.h"


using namespace cv;
using namespace std;

const int BIIMAGE_SENSITIVITY = 252;
const int THRESHOLD_TYPE = 1;				//1: THRESH_BINARY; 2: THRESH_BINARY_INV; 3: THRESH_TRUNC; 4: TRESH_TOZERO; 5: THRESH_TOZERO_INV

const int METHOD_INDICATOR = 1;				//1: by distance; 2: by acceleration
const int LINE_THICKNESS = 1;
const double VELOCITY_SENSITIVITY = 25.5;

string doubleToString(double number){

	//this function has a number input and string output
	std::stringstream ss;
	ss << number;
	return ss.str();
}

int main()
{
	//open the default camera   
	VideoCapture cap(0);
	//check if the camera is opened successfully   
	if (!cap.isOpened())
	{
		return -1;
	}

	vector<double> coordinate_x;							//record horizontal coordinate
	vector<double> coordinate_y;							//record vertical coordinate

	vector<KeyPoint> detectKeyPoint;						//vector to store the keypoints

	Mat keyPointImage;										//for displaying video with marked keypoints
	Mat trajectory_line = Mat::zeros(480, 640, CV_8UC3);	//create black empty image for displaying line-type trajactory
	Mat trajectory_dot = Mat::zeros(480, 640, CV_8UC3);		//create black empty image for displaying dot-type trajactory

	bool BinaryImage = false;								//can be toggled by pressing 'b'
	bool pause = false;										//can be toggled by pressing 'p'
	bool lineMode = false;									//can be toggled by pressing 'l'
	bool dotMode = false;									//can be toggled by pressing 'd'

	double coor[3][2];										//coordinates of light spots
	int j = 0;												//line drawing counter
	int k = 0;												//used to skip the first several loops, letting the camera ready
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

	namedWindow("VideoCapture");			//create a window displaying the camera feed   
	moveWindow("VideoCapture", 805, 10);	//setting window position



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
		threshold(gray, gray, BIIMAGE_SENSITIVITY, 255, THRESHOLD_TYPE);

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
				switch (METHOD_INDICATOR)
				{
				case 1:
					//![option I: velocity]
					//read current keypoint coordinates
					coor[0][0] = detectKeyPoint[0].pt.x;
					coor[0][1] = detectKeyPoint[0].pt.y;

					coordinate_x.push_back(coor[0][0]);
					coordinate_y.push_back(coor[0][1]);

					//calculate the distance between to adjacent points
					distance = sqrt((coor[1][0] - coor[0][0]) * (coor[1][0] - coor[0][0]) + (coor[1][1] - coor[0][1]) * (coor[1][1] - coor[0][1]));

					//calculate distances between each two adjacent points
					distance_10 = sqrt((coor[1][0] - coor[0][0]) * (coor[1][0] - coor[0][0]) + (coor[1][1] - coor[0][1]) * (coor[1][1] - coor[0][1]));
					distance_21 = sqrt((coor[2][0] - coor[1][0]) * (coor[2][0] - coor[1][0]) + (coor[2][1] - coor[1][1]) * (coor[2][1] - coor[1][1]));

					//difference in distance, with fixed time interval, this can be treated as acceleration as well
					delta_dis = abs(distance_10 - distance_21);
					
					
					//line drawing
					if (distance >= 0.0 && distance <= VELOCITY_SENSITIVITY)
						line(trajectory_line, Point(coor[1][0], coor[1][1]), Point(coor[0][0], coor[0][1]), Scalar(0, 255, (distance * 10)), LINE_THICKNESS, 8);
					else if (distance > VELOCITY_SENSITIVITY && distance <= 2 * VELOCITY_SENSITIVITY)
						line(trajectory_line, Point(coor[1][0], coor[1][1]), Point(coor[0][0], coor[0][1]), Scalar(0, (255 - (distance - VELOCITY_SENSITIVITY) * 10), 255), LINE_THICKNESS, 8);
					else
						line(trajectory_line, Point(coor[1][0], coor[1][1]), Point(coor[0][0], coor[0][1]), Scalar(0, 0, 255), LINE_THICKNESS, 8);
						
					//dots drawing
					circle(trajectory_dot, Point(coor[0][0], coor[0][1]), 0.5, Scalar(255, 255, 255), -1);					
					
					//print the coordinates of current lightspot
					putText(keyPointImage, "Tracking object at (" + doubleToString(coor[0][0]) + "," + doubleToString(coor[0][1]) + ")", Point(10, 20), 1, 1.0, Scalar(0, 0, 255), 1, CV_AA);
					
					//print the distance between two adjacent spots
					putText(keyPointImage, "Object velocity is:" + doubleToString(distance), Point(10, 40), 1, 1.0, Scalar(0, 0, 255), 1, CV_AA);

					//print the two distances and acceleration
					putText(keyPointImage, "Object acceleration is:" + doubleToString(delta_dis), Point(10, 60), 1, 1.0, Scalar(0, 0, 255), 1, CV_AA);

					//cout << "Dis1: " << left << setw(10) << distance_10 << "Dis2: " << left << setw(10) << distance_21 << "Acc: " << left << setw(10) << delta_dis << endl;

					//pass current point to x1 and y1, acting as the 'previous' point in the next loop
					coor[2][0] = coor[1][0];
					coor[2][1] = coor[1][1];
					coor[1][0] = coor[0][0];
					coor[1][1] = coor[0][1];

					break;

				case 2:
					//![option II: acceleration]
					//read current keypoint coordinates
					coor[0][0] = detectKeyPoint[0].pt.x;
					coor[0][1] = detectKeyPoint[0].pt.y;

					coordinate_x.push_back(coor[0][0]);
					coordinate_y.push_back(coor[0][1]);

					//calculate distances between each two adjacent points
					distance_10 = sqrt((coor[1][0] - coor[0][0]) * (coor[1][0] - coor[0][0]) + (coor[1][1] - coor[0][1]) * (coor[1][1] - coor[0][1]));
					distance_21 = sqrt((coor[2][0] - coor[1][0]) * (coor[2][0] - coor[1][0]) + (coor[2][1] - coor[1][1]) * (coor[2][1] - coor[1][1]));

					//difference in distance, with fixed time interval, this can be treated as acceleration as well
					delta_dis = abs(distance_10 - distance_21);

					//line drawing
					if (delta_dis >= 0.0 && delta_dis <= VELOCITY_SENSITIVITY)
						line(trajectory_line, Point(coor[1][0], coor[1][1]), Point(coor[0][0], coor[0][1]), Scalar(0, 255, (delta_dis * 10)), LINE_THICKNESS, 8);
					else if (delta_dis > VELOCITY_SENSITIVITY && delta_dis <= 2 * VELOCITY_SENSITIVITY)
						line(trajectory_line, Point(coor[1][0], coor[1][1]), Point(coor[0][0], coor[0][1]), Scalar(0, (255 - (delta_dis - VELOCITY_SENSITIVITY) * 10), 255), LINE_THICKNESS, 8);
					else
						line(trajectory_line, Point(coor[1][0], coor[1][1]), Point(coor[0][0], coor[0][1]), Scalar(0, 0, 255), LINE_THICKNESS, 8);

					//dots drawing
					circle(trajectory_dot, Point(coor[0][0], coor[0][1]), 0.5, Scalar(255, 255, 255), -1);

					//print the coordinates of current lightspot
					cout << coor[0][0] << ", " << coor[0][1] << endl;

					//print the two distances and acceleration
					//cout << "Dis1: " << left << setw(10) << distance_10 << "Dis2: " << left << setw(10) << distance_21 << "Acc: " << left << setw(10) << delta_dis << endl;

					//transmitting values
					coor[2][0] = coor[1][0];
					coor[2][1] = coor[1][1];
					coor[1][0] = coor[0][0];
					coor[1][1] = coor[0][1];

					break;
				}				
			}
			j++;

			//save coordinates to text files
			if (j >= 100)
			{
				ofstream output_file1("D:\\pic\\coordinate_x.txt");
				ostream_iterator<double> output_iterator_x(output_file1, "\n");
				copy(coordinate_x.begin(), coordinate_x.end(), output_iterator_x);

				ofstream output_file2("D:\\pic\\coordinate_y.txt");
				ostream_iterator<double> output_iterator_y(output_file2, "\n");
				copy(coordinate_y.begin(), coordinate_y.end(), output_iterator_y);
			}
		}

		if (BinaryImage == true)
		{
			namedWindow("Binary Image");				//create a window displaying the binary image   
			moveWindow("Binary Image", 150, 10);		//setting window position
			imshow("Binary Image", gray);
		}
		else
			destroyWindow("Binary Image");

		if (lineMode == true)
		{
			namedWindow("Line Trajactory");				//create a window displaying the line-type trajactory of the moving light spot
			moveWindow("Line Trajactory", 150, 520);	//setting window position
			imshow("Line Trajactory", trajectory_line);
		}
		else
			destroyWindow("Line Trajactory");

		if (dotMode == true)
		{
			namedWindow("Dot Trajactory");				//create a window displaying the dot-type trajactory of the moving light spot
			moveWindow("Dot Trajactory", 150, 520);	//setting window position
			imshow("Dot Trajactory", trajectory_dot);
		}
		else
			destroyWindow("Dot Trajactory");
		
		imshow("VideoCapture", keyPointImage);

		//check to see if a button has been pressed.
		//this 10ms delay is necessary for proper operation of this program
		//if removed, frames will not have enough time to referesh and a blank 
		//image will appear.
		switch (waitKey(10))
		{

		case 27: //'esc' key has been pressed, exit program.
			return 0;

		case 98: //'b' has been pressed and this will open binary image window
			BinaryImage = !BinaryImage;
			if (BinaryImage == false) 
				cout << "Binary image window closed." << endl;
			else 
				cout << "Binary image window opened." << endl;
			break;

		case 100: //'d' has been pressed and this will open dot-type trajactory plotting window
			dotMode = !dotMode;
			if (dotMode == false)
				cout << "Dot-type trajactory window closed." << endl;
			else
				cout << "Dot-type trajactory window opened." << endl;
			break;

		case 108: //'l' has been pressed and this will open line-type trajactory plotting window
			lineMode = !lineMode;
			if (lineMode == false)
				cout << "Line-type trajactory window closed." << endl;
			else
				cout << "Line-type trajactory window opened." << endl;
			break;

		case 112: //'p' has been pressed. this will pause/resume the code.
			pause = !pause;
			if (pause == true)
			{
				cout << "Code paused, press 'p' again to resume" << endl;
				while (pause == true)
				{
					//stay in this loop until 
					switch (waitKey())
					{
					case 112:
						//change pause back to false
						pause = false;
						cout << "Code resumed." << endl;
						break;
					}
				}
			}
		}

		//cout << double(clock() - startTime) / (double)CLOCKS_PER_SEC << " seconds." << endl;
	}
	return 0;
}
