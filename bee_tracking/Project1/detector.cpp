#include "detector.h"

Detectors::Detectors()
{
}

Detectors::~Detectors()
{
}

std::vector<cv::Point> Detectors::detect(cv::Mat frame)
{
	centroid.clear();
	cv::Mat thresh, frame_;
	frame_ = frame.clone();

	//cv::GaussianBlur(frame_, frame_, cv::Size(5, 5), 0);
	cv::cvtColor(frame_, thresh, CV_BGR2GRAY);
	cv::threshold(thresh, thresh, 10, 255, 0);
	findContours(thresh, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	if (contours.size() != 0)
	{
		for (size_t i = 0; i < contours.size(); i++)
		{
			cv::Moments M = moments(contours[i]);
			int x = (int)(M.m10 / M.m00);
			int y = (int)(M.m01 / M.m00);
			cv::Point pnt = cv::Point(x, y);
			centroid.push_back(pnt);
		}
	}

	return centroid;
}