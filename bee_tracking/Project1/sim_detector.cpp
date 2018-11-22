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
	vector<vector<Point>> contoursL, contoursR;								//collection of the largest contours in each scene
	vector<Point> centroidL, centroidR;										//centres of mass
	vector<int> numConL, numConR;
	double area;


	//Mat outputL = Mat::zeros(Size((int)capL.get(3), (int)capL.get(4)), CV_8UC3);
	//Mat outputR = Mat::zeros(Size((int)capR.get(3), (int)capR.get(4)), CV_8UC3);

	for (;;)
	{
		vector<vector<Point>> cntL, cntR;									//contours in a single scene
		capL >> frameL;
		capR >> frameR;
		if (frameL.empty()|frameR.empty())
			break;

		cvtColor(frameL, threshL, CV_BGR2GRAY);
		cvtColor(frameR, threshR, CV_BGR2GRAY);
		
		findContours(threshL, cntL, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
		findContours(threshR, cntR, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
		numConL.push_back(cntL.size());
		numConR.push_back(cntR.size());

		cout << cntL.size() << " contours are detected. " << endl;

		if (cntL.size() != 0 && cntR.size() != 0)
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
			Moments ML = moments(cntL[largestCnt]);
			int cenx = (int)(ML.m10 / ML.m00);
			int ceny = (int)(ML.m01 / ML.m00);
			cenx = -cenx + capL.get(3) / 2;
			ceny = -ceny + capL.get(4) / 2;
			centroidL.push_back(Point(cenx, ceny));
			//circle(outputL, Point(cenx, ceny), 2, Scalar(0, 255, 255), -1);

			maxArea = 0.0;
			largestCnt;
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
			Moments MR = moments(cntR[largestCnt]);
			cenx = (int)(MR.m10 / MR.m00);
			ceny = (int)(MR.m01 / MR.m00);
			cenx = cenx - capR.get(3) / 2;
			ceny = -ceny + capR.get(4) / 2;
			centroidR.push_back(Point(cenx, ceny));
			//circle(outputR, Point(cenx, ceny), 2, Scalar(0, 0, 255), -1);
		}
		cout << "All done. Now display the outputs..." << endl;
		//imshow("OutputL", outputL);
		//imshow("OutputR", outputR);
		//waitKey(100);
	}
	//destroyAllWindows();

	Mat coorL = Mat::zeros(Size(2, centroidL.size()), CV_16S);
	for (int i = 0; i < centroidL.size(); i++)
	{
		coorL.at<short>(i, 0) = centroidL[i].x;
		coorL.at<short>(i, 1) = centroidL[i].y;
	}

	Mat coorR = Mat::zeros(Size(2, centroidR.size()), CV_16S);
	for (int i = 0; i < centroidR.size(); i++)
	{
		coorR.at<short>(i, 0) = centroidR[i].x;
		coorR.at<short>(i, 1) = centroidR[i].y;
	}
	
	FileStorage fs("dualChannelCoors.yml", FileStorage::WRITE);
	fs << "coorL" << coorL;
	fs << "coorR" << coorR;
	fs.release();
	waitKey(500);
	return 0;
}
