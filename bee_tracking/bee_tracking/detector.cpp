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
	cv::GaussianBlur(frame_, frame_, cv::Size(5, 5), 0);
	cv::cvtColor(frame_, thresh, CV_BGR2GRAY);
	findContours(thresh, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	numCon.push_back(contours.size());

	if (contours.size() != 0)
	{
		found = 1;
		for (int i = 0; i < contours.size(); i++)
		{
			//cv::Mat drawingboard = frame_.clone();
			double area = contourArea(contours[i]);
			//cv::drawContours(drawingboard, contours, i, cv::Scalar(0, 0, 255), 2);
			//cv::imshow("Current contour", drawingboard);
			//cv::waitKey(100);
			if (area > 30)
			{
				cv::Moments ML = moments(contours[i]);
				int cenx = (int)(ML.m10 / ML.m00);
				int ceny = (int)(ML.m01 / ML.m00);
				//cenx = -cenx + frame_.cols / 2;
				//ceny = -ceny + frame_.rows / 2;
				centroid.push_back(cv::Point(cenx, ceny));
			}
			//cv::destroyAllWindows();
		}
	}

	return centroid;
}