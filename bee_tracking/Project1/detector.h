#pragma once
#include <opencv2\opencv.hpp>

class Detectors
{
public:
	Detectors();
	~Detectors();

	std::vector<cv::Point> detect(cv::Mat frame, char cam);

private:
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Point> centroid;
	std::vector<int> numCon;
	double area;
	bool found = 0;
	char cam_;
};