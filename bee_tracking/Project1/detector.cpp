#include "detector.h"

Detectors::Detectors()
{
}

Detectors::~Detectors()
{
}

std::vector<cv::Point> Detectors::detect(cv::Mat frame, char cam)
{
	cv::Mat thresh, frame_;
	frame_ = frame.clone();
	cam_ = cam;
	cv::cvtColor(frame_, thresh, CV_BGR2GRAY);
	findContours(thresh, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	numCon.push_back(contours.size());

	if (contours.size() != 0)
	{
		found = 1;
		for (int i = 0; i < contours.size(); i++)
		{
			double area = contourArea(contours[i]);
			if (area > 30)
			{
				if (cam_ = 'l')
				{
					cv::Moments ML = moments(contours[i]);
					int cenx = (int)(ML.m10 / ML.m00);
					int ceny = (int)(ML.m01 / ML.m00);
					cenx = -cenx + frame_.cols / 2;
					ceny = -ceny + frame_.rows / 2;
					centroid.push_back(cv::Point(cenx, ceny));
				}
				if (cam_ = 'r')
				{
					cv::Moments ML = moments(contours[i]);
					int cenx = (int)(ML.m10 / ML.m00);
					int ceny = (int)(ML.m01 / ML.m00);
					cenx = cenx - frame_.cols / 2;
					ceny = -ceny + frame_.rows / 2;
					centroid.push_back(cv::Point(cenx, ceny));
				}
			}
		}
	}

	return centroid;
}