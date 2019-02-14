#pragma once
#include "KalmanFilter.h"

class Track
{
public:
	Track(cv::Point3d prediction, int track_id);
	~Track();
	cv::Point3d prediction_;
	int skipped_frames;
	KF kalman;
	int real;
	std::vector<cv::Point3d> trace;
	int track_id_;
	int clr_id;

private:

};
