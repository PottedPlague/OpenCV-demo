#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>

class KF
{
public:
	KF(double deltaTime = 0.5);
	~KF();

	void setDeltaTime(double deltaTime);
	void create(cv::Point3d p0, cv::Point3d v0 = cv::Point3d(0, 0, 0));
	cv::Point3d predict();
	cv::Point3d update(cv::Point3d measurement, bool flag);
	cv::Point3d lastPointResult_;

private:
	std::vector<cv::Point3d> initPoints_;
	double dt_;	//delta time
	double acceNoiseCov_;
	cv::KalmanFilter myKalman_;
};
