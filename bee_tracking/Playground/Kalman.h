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

		void create(cv::Point2d p0, cv::Point2d v0);
		cv::Point2d predict();
		cv::Point2d update(cv::Point2d pt, bool isReceived);

	private:
		cv::Mat board_ = cv::Mat::zeros(cv::Size(800, 600), CV_8UC3);
		std::vector<cv::Point2d> initPoints_;
		cv::Point2d lastPointResult_;

		double dt_;	//delta time
		double acceNoiseCov_;

		cv::KalmanFilter myKalman_;

	};
}	//namespace beeproject