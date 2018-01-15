#include "honeybee.h"

using namespace beeproject;

int main()
{
	cv::Mat frame = cv::Mat::zeros(cv::Size(800, 600), CV_8UC3);
	cv::Rect2d beebox;
	beebox = cv::selectROI(frame);
	std::string filename = "D:/pic/virtualbee.avi";

	cv::VideoWriter writer(filename, CV_FOURCC('M', 'J', 'P', 'G'), 50, cv::Size(frame.cols, frame.rows));
	honeybee bee;
	
	bee.add(frame, beebox);

	bee.startMoving();
	for (size_t i = 0; i < bee.getFrames().size(); i++)
		writer.write(bee.getFrames()[i]);
	
	return 0;
}










//#include <opencv2/core/utility.hpp>
//#include <opencv2/tracking.hpp>
//#include <opencv2/videoio.hpp>
//#include <opencv2/highgui.hpp>
//#include <iostream>
//#include <cstring>
//#include <ctime>
//#include "samples_utility.hpp"
//#include "multiTrackingh.h"
//
//using namespace std;
//using namespace cv;
//
//int main() {
//
//	//return multiTracking();
//	
//	// set the default tracking algorithm
//	string trackingAlg = "KCF";
//	
//	// create the tracker
//	MultiTracker trackers;
//	
//	// container of the tracked objects
//	vector<Rect2d> objects;
//	
//	VideoCapture cap(0);
//	Mat frame;
//	
//	// get bounding box
//	cap >> frame;
//
//	vector<Rect> ROIs;
//	selectROIs("tracker", frame, ROIs);
//	
//	//quit when the tracked object(s) is not provided
//	if (ROIs.size()<1)
//		return 0;
//	
//	// initialize the tracker
//	vector<Ptr<Tracker> > algorithms;
//	for (size_t i = 0; i < ROIs.size(); i++)
//	{
//		 algorithms.push_back(createTrackerByName(trackingAlg));
//		 objects.push_back(ROIs[i]);
//	}
//	
//	trackers.add(algorithms, frame, objects);
//	
//	// do the tracking
//	printf("Start the tracking process, press ESC to quit.\n");
//	for (;;) 
//	{
//	// get frame from the video
//		cap >> frame;
//		
//		// stop the program if no more images
//		if (frame.rows == 0 || frame.cols == 0)
//			break;
//		
//		// update the tracking result
//		trackers.update(frame);
//		
//		// draw the tracked object
//		for (unsigned i = 0; i<trackers.getObjects().size(); i++)
//			rectangle(frame, trackers.getObjects()[i], Scalar(255, 0, 0), 2, 1);
//		
//		// show image with the tracked object
//		imshow("tracker", frame);
//		
//		// quit on ESC button
//		if (waitKey(1) == 27)break;
//	}
//	
//}
