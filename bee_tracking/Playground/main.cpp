//
//#include "offlineContourFinding.h"
//
//int main()
//{
//	return OLcontours();
//}

#define ENABLE_TESTS 3	// 1 - bee creation
						// 2 - KCF tracker
						// 3 - Kalman filter



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
#include "Kalman.h"
#include "detector.h"

using namespace beeproject;

int main()
{
	std::vector<cv::Point2d> Pts;
	Pts = detectContours();
	KF myKalman;
	myKalman.create(cv::Point(642, 288), cv::Point2d(0, 0));

	for (int i = 0; i < Pts.size(); i++)
	{
		myKalman.predict();
		myKalman.update(Pts[i], 1);
	}
	cv::waitKey(0);

	return 0;
}
#endif