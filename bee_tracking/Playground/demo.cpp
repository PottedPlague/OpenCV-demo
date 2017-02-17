
#include <opencv2\opencv.hpp>
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat previous, next;
	previous = imread("D:\\pic\\previous.bmp");
	next = imread("D:\\pic\\next.bmp");
	Mat output;
	calcOpticalFlowFarneback(previous, next, output, );
	waitKey(50);
	return 0;
}