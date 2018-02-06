#pragma once
#include <opencv2\opencv.hpp>
#include "Kalman.h"

namespace beeproject
{
	class Track
	{
	public:
		Track(int ID, cv::Point2d initPt);
		~Track();

	private:
		int trackID_;
		std::vector<cv::Point2d> trace_;
		int skippedFrames_;
		cv::Point2d coorPre_;
		KF kalman_;

	};

	class Tracker
	{
	public:
		Tracker(int maxFramesToSkip, double distThresh, std::vector<std::vector<cv::Point2d>> centroids);
		~Tracker();

		void solve();

	private:
		size_t countID_;
		std::vector<Track> tracks_;
		int maxFramesToSkip_;
		double distThresh_;
		std::vector<std::vector<cv::Point2d>> detections_;

	};
}	//namespace beeproject