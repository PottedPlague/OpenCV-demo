#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>

class KF
{
public:
	KF(double deltaTime = 0.5);
	~KF();

	void setDeltaTime(double deltaTime);
	void create(cv::Point p0, cv::Point v0 = cv::Point(0, 0));
	cv::Point predict();
	cv::Point update(cv::Point measurement, bool flag);
	cv::Point lastPointResult_;

private:
	std::vector<cv::Point> initPoints_;
	double dt_;	//delta time
	double acceNoiseCov_;
	cv::KalmanFilter myKalman_;
};
