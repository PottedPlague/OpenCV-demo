
#include <opencv2\opencv.hpp>
#include <stdio.h>

using namespace std;
using namespace cv;

void detectContours()
{
	Mat img = imread("D:/pic/sample_0.png", IMREAD_COLOR);

	//--------finding contours--------//
	Mat grey;
	cvtColor(img, grey, CV_BGR2GRAY);
	Mat bw;


	threshold(grey, bw, 50, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	vector<Vec4i> hierarchy;
	vector<vector<Point> > contours;
	findContours(bw, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	vector<Moments> M(contours.size());
	vector<Point2f> MC(contours.size());
	for (size_t i = 0; i < contours.size(); i++)
	{
		M[i] = moments(contours[i]);
	}

	for (size_t i = 0; i < contours.size(); i++)
	{
		MC[i] = Point2f(M[i].m10 / M[i].m00, M[i].m01 / M[i].m00);
	}

	for (size_t i = 0; i < contours.size(); ++i)
	{
		// Calculate the area of each contour
		double area = contourArea(contours[i]);
		// Ignore contours that are too small or too large
		if (area < 1e2 || 1e5 < area) continue;
		// Draw each contour only for visualisation purposes
		drawContours(img, contours, static_cast<int>(i), Scalar(0, 0, 255), 2, 8, hierarchy, 0);
		circle(img, MC[i], 4, Scalar(0, 0, 255), -1);
	}

	imshow("Output", img);
	waitKey(0);
}