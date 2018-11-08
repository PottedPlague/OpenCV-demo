#include "sim_detector.h"
#include <opencv2\opencv.hpp>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

int simDetector()
{
	VideoCapture capL, capR;
	capL.open("F:/renderoutput/helical_line/morph/left.avi");
	capR.open("F:/renderoutput/helical_line/morph/right.avi");

	Mat frameL, frameR, threshL, threshR;
	vector<vector<Point>> contoursL, contoursR;
	vector<Point> centroidL, centroidR;
	vector<int> numConL, numConR;
	double area;


	Mat outputL = Mat::zeros(Size((int)capL.get(3), (int)capL.get(4)), CV_8UC3);
	Mat outputR = Mat::zeros(Size((int)capR.get(3), (int)capR.get(4)), CV_8UC3);

	for (;;)
	{
		vector<vector<Point>> cntL, cntR;
		capL >> frameL;
		capR >> frameR;
		if (frameL.empty()|frameR.empty())
			break;

		cvtColor(frameL, threshL, CV_BGR2GRAY);
		cvtColor(frameR, threshR, CV_BGR2GRAY);
		
		findContours(threshL, cntL, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
		numConL.push_back(cntL.size());
		cout << cntL.size() << " contours are detected. " << endl;
		if (cntL.size() != 0)
		{
			double maxArea = 0.0;
			int largestCnt;
			for (int s = 0; s < cntL.size(); s++)
			{
				area = contourArea(cntL[s]);
				if (area > maxArea)
				{
					largestCnt = s;
					maxArea = area;
				}
			}
			contoursL.push_back(cntL[largestCnt]);
			Moments M = moments(cntL[largestCnt]);
			int cenx = (int)(M.m10 / M.m00);
			int ceny = (int)(M.m01 / M.m00);
			centroidL.push_back(Point(cenx, ceny));
			circle(outputL, Point(cenx, ceny), 2, Scalar(0, 255, 255), -1);
		}
		cout << "Lefe channel finished. " << endl;

		findContours(threshR, cntR, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
		numConR.push_back(cntR.size());
		cout << cntR.size() << " contours are detected. " << endl;
		if (cntR.size() != 0)
		{
			double maxArea = 0.0;
			int largestCnt;
			for (int s = 0; s < cntR.size(); s++)
			{
				area = contourArea(cntR[s]);
				if (area > maxArea)
				{
					largestCnt = s;
					maxArea = area;
				}
			}
			contoursR.push_back(cntR[largestCnt]);
			Moments M = moments(cntR[largestCnt]);
			int cenx = (int)(M.m10 / M.m00);
			int ceny = (int)(M.m01 / M.m00);
			centroidR.push_back(Point(cenx, ceny));
			circle(outputR, Point(cenx, ceny), 2, Scalar(0, 0, 255), -1);
		}
		cout << "All done. Now display the outputs..." << endl;
		imshow("OutputL", outputL);
		imshow("OutputR", outputR);
		waitKey(100);
	}
	destroyAllWindows();
	waitKey(500);
	return 0;
}
