#include "disparity.h"
#include <opencv2\opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int disparityGen()
{
	Mat D1, D2, R, T, E, F, R1, R2, P1, P2, Q;
	Mat CM1 = Mat(3, 3, CV_64FC1);
	Mat CM2 = Mat(3, 3, CV_64FC1);
	FileStorage fs;
	fs.open("mystereocalib.yml", FileStorage::READ);
	fs["CM1"] >> CM1;
	fs["CM2"] >> CM2;
	fs["D1"] >> D1;
	fs["D2"] >> D2;
	fs["R"] >> R;
	fs["T"] >> T;
	fs["E"] >> E;
	fs["F"] >> F;
	fs["R1"] >> R1;
	fs["R2"] >> R2;
	fs["P1"] >> P1;
	fs["P2"] >> P2;
	fs["Q"] >> Q;
	fs.release();

	printf("Applying Undistort\n");

	Mat img1, img2;
	Mat map1x, map1y, map2x, map2y;
	Mat imgU1, imgU2, disp, vdisp;

	initUndistortRectifyMap(CM1, D1, R1, P1, Size(1280, 720), CV_32FC1, map1x, map1y);
	initUndistortRectifyMap(CM2, D2, R2, P2, Size(1280, 720), CV_32FC1, map2x, map2y);

	printf("Undistort complete\n");

	//Setup for finding stereo correspondences
	Ptr<StereoSGBM> stereo = StereoSGBM::create();
	stereo->setBlockSize(5);
	stereo->setDisp12MaxDiff(-1);
	stereo->setMinDisparity(0);
	stereo->setNumDisparities(64);
	stereo->setP1(0);
	stereo->setP2(0);
	stereo->setPreFilterCap(1);
	stereo->setSpeckleRange(0);
	stereo->setSpeckleWindowSize(0);
	stereo->setUniquenessRatio(0);
	stereo->setMode(stereo->MODE_SGBM);

	VideoCapture cap1(1);					//left
	cap1.set(CAP_PROP_FRAME_HEIGHT, 720);
	cap1.set(CAP_PROP_FRAME_WIDTH, 1280);
	VideoCapture cap2(2);					//right
	cap2.set(CAP_PROP_FRAME_HEIGHT, 720);
	cap2.set(CAP_PROP_FRAME_WIDTH, 1280);

	namedWindow("Disparity");
	namedWindow("image1");
	namedWindow("image2");

	while (1)
	{
		if (waitKey(5) == ' ')
		{
			cout << "Initializing..." << endl;
			cap1 >> img1;
			cap2 >> img2;
			cap1 >> img1;
			cap2 >> img2;
			cout << "reading images..." << endl;
			cap1 >> img1;
			cap2 >> img2;

			cout << "doing remapping..." << endl;
			remap(img1, imgU1, map1x, map1y, INTER_LINEAR, BORDER_CONSTANT, Scalar());
			remap(img2, imgU2, map2x, map2y, INTER_LINEAR, BORDER_CONSTANT, Scalar());

			cout << "computing disparity map..." << endl;
			stereo->compute(imgU1, imgU2, disp);
			normalize(disp, vdisp, 0, 256, NORM_MINMAX, CV_8U);

			cout << "now displaying the results..." << endl;
			imshow("Disparity", vdisp);
			imshow("image1", imgU1);
			imshow("image2", imgU2);
			imwrite("D:/pic/stereo/left.tif", imgU1);
			imwrite("D:/pic/stereo/right.tif", imgU2);

			cout << "one cycle finished!" << endl;
		}
		

		if (waitKey(5) == 27)
		{
			break;
		}
	}

	cap1.release();
	cap2.release();
	waitKey(200);
	return 0;
}