#include "detector.h"

std::vector<std::vector<cv::Point2d>> detectContours()
{
	std::vector<cv::Mat> video;
	std::vector<std::vector<cv::Point2d>> coors;
	cv::Mat frame;
	std::string filename = "D:/pic/virtualbee_1.avi";
	cv::VideoCapture cap;
	cap.open(filename);

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
		std::vector<cv::Point2d> MC(contours.size());

		for (size_t s = 0; s < contours.size(); s++)
			M[s] = cv::moments(contours[s]);
		for (size_t s = 0; s < contours.size(); s++)
			MC[s] = cv::Point2d(M[s].m10 / M[s].m00, M[s].m01 / M[s].m00);
		coors.push_back(MC);
	}

	return coors;
}
