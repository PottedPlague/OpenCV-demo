#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>

namespace beeproject
{
	class KF
	{
	public:
		KF(double deltaTime = 0.5);
		~KF();

		void create(cv::Point2d p0, cv::Point2d v0 = cv::Point2d(0, 0));
		cv::Point2d predict();
		void update(cv::Point2d measurement, bool isReceived);

	private:
		std::vector<cv::Point2d> initPoints_;
		cv::Point2d lastPointResult_;

		double dt_;	//delta time
		double acceNoiseCov_;

		cv::KalmanFilter myKalman_;

	};
}	//namespace beeproject