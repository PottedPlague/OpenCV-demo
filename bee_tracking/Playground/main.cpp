//
//#include "offlineContourFinding.h"
//
//int main()
//{
//	return OLcontours();
//}

#define ENABLE_TESTS 5	// 1 - bee creation
						// 2 - KCF tracker
						// 3 - Kalman filter
						// 4 - resize images in bulk
						// 5 - Museum demo


/******************************************************************************
Implementation of virtual bee 
******************************************************************************/

#if ENABLE_TESTS == 1
#include "honeybee.h"

using namespace beeproject;

int main()
{
	cv::Mat frame = cv::Mat::zeros(cv::Size(800, 600), CV_8UC3);
	cv::Rect2d beebox;
	beebox = cv::selectROI(frame);
	
	std::string filename = "D:/pic/virtualbee.avi";

	cv::VideoWriter writer(filename, CV_FOURCC('M', 'J', 'P', 'G'), 30, cv::Size(frame.cols, frame.rows));
	add(frame, 10, beebox);
	
	std::vector<cv::Mat> video(startMoving());

	for (size_t i = 0; i < video.size(); i++)
		writer.write(video[i]);
	
	return 0;
}
#endif

/******************************************************************************
KCF tracker
******************************************************************************/

#if ENABLE_TESTS == 2
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>
#include <ctime>
#include "samples_utility.hpp"
#include "multiTrackingh.h"

using namespace std;
using namespace cv;

int main() {

	//return multiTracking();
	
	// set the default tracking algorithm
	string trackingAlg = "KCF";
	
	// create the tracker
	MultiTracker trackers;
	
	// container of the tracked objects
	vector<Rect2d> objects;
	
	VideoCapture cap(0);
	Mat frame;
	
	// get bounding box
	cap >> frame;

	vector<Rect> ROIs;
	selectROIs("tracker", frame, ROIs);
	
	//quit when the tracked object(s) is not provided
	if (ROIs.size()<1)
		return 0;
	
	// initialize the tracker
	vector<Ptr<Tracker> > algorithms;
	for (size_t i = 0; i < ROIs.size(); i++)
	{
		 algorithms.push_back(createTrackerByName(trackingAlg));
		 objects.push_back(ROIs[i]);
	}
	
	trackers.add(algorithms, frame, objects);
	
	// do the tracking
	printf("Start the tracking process, press ESC to quit.\n");
	for (;;) 
	{
	// get frame from the video
		cap >> frame;
		
		// stop the program if no more images
		if (frame.rows == 0 || frame.cols == 0)
			break;
		
		// update the tracking result
		trackers.update(frame);
		
		// draw the tracked object
		for (unsigned i = 0; i<trackers.getObjects().size(); i++)
			rectangle(frame, trackers.getObjects()[i], Scalar(255, 0, 0), 2, 1);
		
		// show image with the tracked object
		imshow("tracker", frame);
		
		// quit on ESC button
		if (waitKey(1) == 27)break;
	}
	
}
#endif

#if ENABLE_TESTS == 3
#include "detector.h"
#include "tracker.h"

using namespace beeproject;

int main()
{
	std::vector<std::vector<cv::Point2d>> Pts;
	Pts = detectContours();

	Tracker myTracker = Tracker::Tracker(30, 160, 5, 0);
	myTracker.solve(Pts);


	/*for (int i = 0; i < Pts.size(); i++)
	{
		myKalman.predict();
		myKalman.update(Pts[i], 1);
	}*/
	cv::waitKey(0);

	return 0;
}
#endif

#if ENABLE_TESTS == 4
#include <opencv2\opencv.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

std::string intToStr(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

int main()
{
	/*Rect roi;
	roi.x = 0;
	roi.y = 0;
	roi.width = 2048;
	roi.height = 270;

	for (int i = 1; i < 101; i++)
	{
		string filename_i = "F:/corridor/15m_files/15m (" + intToStr(i) + ").tif";
		string filename_o = "F:/corridor/cropped/15m_files/15m (" + intToStr(i) + ").tif";
		Mat img = imread(filename_i, CV_LOAD_IMAGE_GRAYSCALE);
		Mat crop = img(roi).clone();
		imwrite(filename_o, crop);
	}*/
	for (int k = 1; k < 1040; k++)
	{
		string filename_i = "F:/corridor_redo/16m_files/16m (" + intToStr(k) + ").tif";
		string filename_o = "F:/corridor_redo/scaled/16m-" + intToStr(k) + ".tif";
		Mat img = imread(filename_i, CV_LOAD_IMAGE_GRAYSCALE);

		for (int i = 0; i < img.rows; i++)
			for (int j = 0; j < img.cols; j++)
			{
				if (img.at<uchar>(i, j) * 30 > 254)
					img.at<uchar>(i, j) = 255;
				else
					img.at<uchar>(i, j) = img.at<uchar>(i, j) * 30;
			}

		imwrite(filename_o, img);
	}

	waitKey(200);
	return 0;
}

#endif

#if ENABLE_TESTS == 5
#include <opencv2\opencv.hpp>
#include <stdio.h>
using namespace std;
using namespace cv;

int main()
{
	VideoCapture cam(1);
	cam.set(CAP_PROP_FRAME_WIDTH, 1280);
	cam.set(CAP_PROP_FRAME_HEIGHT, 720);
	cam.set(CAP_PROP_AUTOFOCUS, 0);
	Mat track_f = Mat::zeros(720, 1280, CV_8UC3);
	//vector<KeyPoint> detectKeyPoint;	
	vector<Point2d> track;
	Point2d pt;
	//Mat keyPointImage;
	int j = 0;

	for (;;)
	{
		Mat frame, flip, gray, binary, output;
		cam >> frame;
		cv::flip(frame, flip, 1);
		cvtColor(flip, gray, COLOR_BGR2GRAY);
		GaussianBlur(gray, gray, Size(11, 11), 1.0);
		threshold(gray, binary, 190, 255, 0);

		std::vector<std::vector<cv::Point>> contours;
		cv::findContours(binary, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
		Moments M;
		Point2d MC;
		if (contours.size() > 0)
		{
			M = moments(contours[0]);
			MC = Point2d(M.m10 / M.m00, M.m01 / M.m00);
			if (isnan(MC.x))
				continue;
			track.push_back(MC);
			circle(flip, MC, 4, Scalar(0, 0, 255), 4);
			j++;
			if (j >= 3)
			{
				line(track_f, track.end()[-1], track.end()[-2], Scalar(0, 255, 255), 2);
			}
		}
		addWeighted(track_f, 1.0, flip, 1.0, 0.0, output);
		//imshow("Track", track_f);
		imshow("Binary", binary);
		imshow("Camera", output);
		switch (waitKey(20))
		{
		case 27:
			return 0;
		case 114:
			{
				track_f = Mat::zeros(720, 1280, CV_8UC3);
				j = 0;
			}
		}
	}
	cam.release();
	destroyAllWindows();
	waitKey(200);
	return 0;
	/*
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

	int j = 0;
	double distance = 0.0;

	while (1)
	{
		Mat frame, gray, binary;
		cam >> frame;
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		GaussianBlur(gray, gray, Size(11, 11), 1.0);
		threshold(gray, binary, 215, 255, 1);
		sbd->detect(binary, detectKeyPoint);
		drawKeypoints(gray, detectKeyPoint, frame, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
		if (detectKeyPoint.size() > 0)
		{
			pt.x = detectKeyPoint[0].pt.x;
			pt.y = detectKeyPoint[0].pt.y;
			track.push_back(pt);
			j++;
			if (j >= 3)
			{
				line(track_f, track.end()[-1], track.end()[-2], Scalar(0, 255, 255), 2);
			}
		}
		
		imshow("Track", track_f);
		imshow("Detection", frame);
		imshow("Binary", binary);
		switch (waitKey(20))
		{
		case 27:
			return 0;
		case 114:
			track_f = Mat::zeros(720, 1280, CV_8UC3);
		}
		
	}

	cam.release();
	destroyAllWindows();
	return 0;*/
}

#endif