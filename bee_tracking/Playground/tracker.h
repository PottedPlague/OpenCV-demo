#pragma once
#include <opencv2\opencv.hpp>

namespace beeproject
{
	class track
	{
	public:
		track();
		~track();

	private:
		int trackID_;
		cv::Point2d coorPre_;

	};

	class tracker
	{
	public:
		tracker();
		~tracker();

	private:

	};
}	//namespace beeproject