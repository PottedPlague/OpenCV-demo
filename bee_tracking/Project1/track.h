#pragma once
#include "KalmanFilter.h"

class Track
{
public:
	Track(cv::Point prediction, int track_id);
	~Track();
	cv::Point prediction_;
	int skipped_frames;
	KF kalman;
	int real;
	std::vector<cv::Point> trace;
	int track_id_;
private:
	
};
