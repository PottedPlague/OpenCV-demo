#include <opencv2\opencv.hpp>
#include <iostream>
#include "tyt.h"

using namespace std;
using namespace cv;

const int edgeLowThresh = 10;

bool compareContourAreas(vector<Point> contour1, vector<Point> contour2)
{
	double i = fabs(contourArea(contour1));
	double j = fabs(contourArea(contour2));
	return (i < j);
}

void tyt()
{
	Mat img = imread("D:/pic/tyt/img_0.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat edge;
	vector<vector<Point>> contours;

	namedWindow("Output", WINDOW_AUTOSIZE);
	namedWindow("Edge", WINDOW_AUTOSIZE);

	Canny(img, edge, edgeLowThresh, 3 * edgeLowThresh);

	findContours(edge, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);


	imshow("Output", img);
	imshow("Edge", edge);
	waitKey(0);

}