#include "offlineContourFinding.h"
#include <opencv2\opencv.hpp>
#include <iostream>

int OLcontours()
{
	std::vector<cv::Mat> video;
	cv::Mat frame;
	std::string filename = "D:/pic/vrbee.avi";
	int perc = 101;

	cv::VideoCapture cap;
	cap.open(filename);
	if (!cap.isOpened())
	{
		std::cerr << "Failed to read video file." << std::endl;
		return -1;
	}

	for (;;)
	{
		cap >> frame;
		if (frame.rows == 0 || frame.cols == 0)
			break;
		video.push_back(frame.clone());
	}
	
	std::vector<cv::Mat> grays(video.size());
	for (int i = 0; i < video.size(); i++)
	{
		cv::cvtColor(video[i], grays[i], CV_BGR2GRAY);
		cv::threshold(grays[i], grays[i], 200, 255, 0);
		std::vector<std::vector<cv::Point>> contours;
		cv::findContours(grays[i], contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

		std::vector<cv::Moments> M(contours.size());
		std::vector<cv::Point2f> MC(contours.size());

		for (size_t s = 0; s < contours.size(); s++)
			M[s] = cv::moments(contours[s]);
		for (size_t s = 0; s < contours.size(); s++)
			MC[s] = cv::Point2f(M[s].m10 / M[s].m00, M[s].m01 / M[s].m00);

		for (size_t s = 0; s < contours.size(); s++)
		{
			double area = cv::contourArea(contours[s]);
			/*if (area < 1e2 || 1e5 < area)
				continue;*/
			cv::drawContours(video[i], contours, s, cv::Scalar(0, 0, 255), 1, 8, cv::noArray(), 0);
			cv::circle(video[i], MC[s], 2, cv::Scalar(0, 0, 255), -1);
			cv::putText(video[i], "bee", MC[s], 1, 3.0, cv::Scalar(0, 0, 255), 2, CV_AA);
		}

		if (perc != 100 * i / video.size())
		{
			perc = 100 * i / video.size();
			std::cout << "Processing...(" << perc << "%)" << std::endl;
		}
	}
	std::cout << "All contours have been found, now display the video result." << std::endl;

	cv::namedWindow("Output", cv::WINDOW_AUTOSIZE);
	for (int s = 0; s < video.size(); s++)
	{
		cv::imshow("Output", video[s]);
		cv::waitKey(20); //50 fps
	}

	for (;;)
	{
		switch (cv::waitKey(10))
		{
		case 114:	//if 'r' pressed, replay video
			for (int s = 0; s < video.size(); s++)
			{
				cv::imshow("Output", video[s]);
				cv::waitKey(20); //50 fps
			}

		case 27:
			cv::waitKey(500);
			return 0;
		}
	}

	return 0;
}