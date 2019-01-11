#include "sim_coorCalc.h"
#include <opencv2\opencv.hpp>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

string intToStr(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

int simCoorCalc(std::vector<std::vector<Track*>>& matchedPairs)
{
	vector<Point> coorL, coorR;
	double half_frame_width = 640.;								//640
	double S = 400.;											//target distance
	double f = 0.77011811;										//focal length
	double FOV_angle_h = 0.95;									//horizontal view angle of FOV - rad
	double alpha = 0.396 / 640.;

	/*FileStorage fs;
	fs.open("dualChannelCoors.yml", FileStorage::READ);
	fs["coorL"] >> coorL;
	fs["coorR"] >> coorR;
	fs.release();*/
	for (int i = 0; i < matchedPairs.size(); i++)
	{
		coorL.clear();
		coorL = matchedPairs[i][0]->trace;
		coorR.clear();
		coorR = matchedPairs[i][1]->trace;
		Mat coor3D = Mat::zeros(Size(3, (coorL.size() > coorR.size()) ? coorR.size() : coorL.size()), CV_16S);
		for (int i = 0; i < coor3D.rows; i++)
		{
			coor3D.at<short>(i, 0) = (S*alpha*coorR[i].x*(f + alpha * coorL[i].x)) / (f * f - alpha * alpha * coorL[i].x * coorR[i].x);			//x
			coor3D.at<short>(i, 1) = (S*alpha*coorL[i].x*(f + alpha * coorR[i].x)) / (f * f - alpha * alpha * coorL[i].x * coorR[i].x);			//y
			coor3D.at<short>(i, 2) = (S*alpha*coorL[i].y*(f + alpha * coorR[i].x)) / (f * f - alpha * alpha * coorL[i].x * coorR[i].x);			//z
		}
		FileStorage out("D:/pic/coor3D_" + intToStr(i + 1) + ".yml", FileStorage::WRITE);
		out << "coor3D" << coor3D;
		out.release();
	}
	
	/*for (int i = 0; i < coor3D.rows; i++)
	{
		coor3D.at<short>(i, 0) = (S*alpha*coorR.at<short>(i, 0)*(f + alpha * coorL.at<short>(i, 0))) / (f * f - alpha * alpha * coorL.at<short>(i, 0) * coorR.at<short>(i, 0));
		coor3D.at<short>(i, 1) = (S*alpha*coorL.at<short>(i, 0)*(f + alpha * coorR.at<short>(i, 0))) / (f * f - alpha * alpha * coorL.at<short>(i, 0) * coorR.at<short>(i, 0));
		coor3D.at<short>(i, 2) = (S*alpha*coorL.at<short>(i, 1)*(f + alpha * coorR.at<short>(i, 0))) / (f * f - alpha * alpha * coorL.at<short>(i, 0) * coorR.at<short>(i, 0));
	}

	FileStorage out("coor3D.yml", FileStorage::WRITE);
	out << "coor3D" << coor3D;
	out.release();*/
	cout << "All done." << endl;
	waitKey(300);

	return 0;
}
