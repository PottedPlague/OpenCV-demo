/*
Reads in recorded video clip and selects proper images for calibration. 

Date modified: 09/05/2019
*/

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include "getChessboard.h"
#include "tools.h"

int getChessboard()
{
	cv::VideoCapture cap;
	cap.open("D:/PhD/camCalibration/1080p/GoPro_Left/GH010007.mp4");

	cv::Mat frame, rshape;
	std::string filepath = "D:/PhD/camCalibration/1080p/GoPro_Left/left_";
	int i=0, j, k;
	cap >> frame;
	cv::resize(frame, rshape, cv::Size(1280, 720));
	cv::imshow("GoPro Left", rshape);

	for (;;)
	{
		j = cv::waitKey(5);
		if (j == 32)
		{
			cap >> frame;
			if (frame.empty())
				continue;
			cv::resize(frame, rshape, cv::Size(1280, 720));
			cv::imshow("GoPro Left", rshape);
		}
		if (j == 27)
			break;
		if (j == 115)
		{
			cv::imwrite(filepath + intToStr(i + 1) + ".tif", frame);
			std::cout << "No. of saved images: " << i << std::endl;
			i++;
		}
	}
	cv::destroyAllWindows();
	cv::waitKey(200);
	return 0;
}