#include "dualCam.h"
#include <opencv2\opencv.hpp>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

int dualCam()
{
	//int numBoards = 49;
	int board_w = 9;
	int board_h = 6;

	Size board_sz = Size(board_w, board_h);
	int board_n = board_h * board_w;

	vector<vector<Point3f>> objectPoints;
	vector<vector<Point2f>> imagePoints1, imagePoints2;
	vector<Point2f> corners1, corners2;

	cv::String path2 = "D:/pic/calib/left/left_";
	cv::String path1 = "D:/pic/calib/right/right_";
	
	Mat img1, gray1, img2, gray2;

	bool found1 = false;
	bool found2 = false;

	vector<Point3f> obj;
	for (int j = 0; j < board_n; j++)
		obj.push_back(Point3f(j / board_w, j%board_w, 0.0f));

	for(int k=1;k<50;k++)
	{
		img1 = imread(path1 + to_string(k) + ".tif");
		img2 = imread(path2 + to_string(k) + ".tif");
		cvtColor(img1, gray1, CV_BGR2GRAY);
		cvtColor(img2, gray2, CV_BGR2GRAY);
		found1 = findChessboardCorners(gray1, board_sz, corners1, 0); //CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS
		found2 = findChessboardCorners(gray2, board_sz, corners2, 0); //CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS

		if (found1)
		{
			cornerSubPix(gray1, corners1, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
			drawChessboardCorners(gray1, board_sz, corners1, found1);
			imagePoints1.push_back(corners1);
		}

		if (found2)
		{
			cornerSubPix(gray2, corners2, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
			drawChessboardCorners(gray2, board_sz, corners2, found2);
			imagePoints2.push_back(corners2);
		}

		imshow("corners1", gray1);
		imshow("corners2", gray2);
		objectPoints.push_back(obj);
		printf("Corners stored: ");
		cout << k << endl;
		waitKey(500);
	}

	destroyAllWindows();

	printf("Starting Calibration\n");
	Mat CM1 = Mat(3, 3, CV_64FC1);
	Mat CM2 = Mat(3, 3, CV_64FC1);
	Mat D1, D2;
	Mat R, T, E, F;

	stereoCalibrate(objectPoints, imagePoints1, imagePoints2,
		CM1, D1, CM2, D2, img1.size(), R, T, E, F, CALIB_FIX_ASPECT_RATIO | CALIB_ZERO_TANGENT_DIST | CALIB_SAME_FOCAL_LENGTH, TermCriteria(TermCriteria::COUNT | TermCriteria::EPS, 100, 1e-5));

	FileStorage fs1("mystereocalib.yml", FileStorage::WRITE);
	fs1 << "CM1" << CM1;
	fs1 << "CM2" << CM2;
	fs1 << "D1" << D1;
	fs1 << "D2" << D2;
	fs1 << "R" << R;
	fs1 << "T" << T;
	fs1 << "E" << E;
	fs1 << "F" << F;
	printf("Done Calibration\n");

	printf("Starting Rectification\n");
	Mat R1, R2, P1, P2, Q;
	stereoRectify(CM1, D1, CM2, D2, img1.size(), R, T, R1, R2, P1, P2, Q);
	fs1 << "R1" << R1;
	fs1 << "R2" << R2;
	fs1 << "P1" << P1;
	fs1 << "P2" << P2;
	fs1 << "Q" << Q;
	printf("Done Rectification\n");
	fs1.release();

	printf("Applying Undistort\n");

	Mat map1x, map1y, map2x, map2y;
	Mat imgU1, imgU2, disp, vdisp;

	initUndistortRectifyMap(CM1, D1, R1, P1, img1.size(), CV_32FC1, map1x, map1y);
	initUndistortRectifyMap(CM2, D2, R2, P2, img1.size(), CV_32FC1, map2x, map2y);

	printf("Undistort complete\n");

	//Setup for finding stereo correspondences
	Ptr<StereoSGBM> stereo = StereoSGBM::create(-64, 128, 11, 100, 1000, 32, 0, 15, 1000, 16, StereoSGBM::MODE_HH);

	VideoCapture cap1(1);					//left
	cap1.set(CAP_PROP_FRAME_HEIGHT, 720);
	cap1.set(CAP_PROP_FRAME_WIDTH, 1280);
	VideoCapture cap2(2);					//right
	cap2.set(CAP_PROP_FRAME_HEIGHT, 720);
	cap2.set(CAP_PROP_FRAME_WIDTH, 1280);

	while (1)
	{
		cap1 >> img1;
		cap2 >> img2;

		remap(img1, imgU1, map1x, map1y, INTER_LINEAR, BORDER_CONSTANT, Scalar());
		remap(img2, imgU2, map2x, map2y, INTER_LINEAR, BORDER_CONSTANT, Scalar());
		stereo->compute(imgU1, imgU2, disp);
		normalize(disp, vdisp, 0, 256, NORM_MINMAX, CV_8U);
		imshow("Disparity", vdisp);
		imshow("image1", imgU1);
		imshow("image2", imgU2);

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