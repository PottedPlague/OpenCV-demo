
//#include "average.h"

#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//string intToString(int number)
//{
//	//this function has a number input and string output
//	std::stringstream ss;
//	ss << number;
//	return ss.str();
//}

void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step, const Scalar& color);

void simple_Farneback(String prev_img, String next_img)
{
	Mat prev, next;
	Mat output;

	prev = imread(prev_img);
	next = imread(next_img);
	cvtColor(prev, prev, CV_BGR2GRAY, 1);
	cvtColor(next, next, CV_BGR2GRAY, 1);
	
	calcOpticalFlowFarneback(prev, next, output, 0.5, 1, 13, 10, 5, 1.1, 0);

	imshow("Previous", prev);
	imshow("Next", next);
	drawOptFlowMap(output, prev, 10, (0, 0, 255));

	imshow("Farneback", prev);
}

void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step, const Scalar& color)
{
	for (int y = 0; y < cflowmap.rows; y += step)
	{
		for (int x = 0; x < cflowmap.cols; x += step)
		{
			const Point2f& fxy = flow.at< Point2f>(y, x);
			line(cflowmap, Point(x, y), Point(cvRound(x + fxy.x), cvRound(y + fxy.y)),
				color);
			circle(cflowmap, Point(cvRound(x + fxy.x), cvRound(y + fxy.y)), 1, color, -1);

		}
	}
}