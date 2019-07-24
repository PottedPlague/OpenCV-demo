/*
Reads from pre-saved calibration images (x50)
and calculates camera intrinsics etc.

Date modified: 09/05/2019
*/

#include "camCalib.h"
#include <opencv2\opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int camCalib()
{
	int board_w = 9;
	int board_h = 6;

	Size board_sz = Size(board_w, board_h);
	int board_n = board_h * board_w;							//54 inner corners

	vector<vector<Point3f>> objectPoints;			
	vector<vector<Point2f>> imagePoints;
	vector<Point2f> corners;

	cv::String path = "D:/PhD/camCalibration/1080p/GoPro_Left/left_";

	Mat img, gray;

	bool found = false;

	vector<Point3f> obj;										//in "chessboard corners" unit
	for (int j = 0; j < board_n; j++)
		obj.push_back(Point3f(j / board_w, j%board_w, 0.0f));	//(0,0,0), (0,1,0),...,(5,8,0)

	for (int k = 1; k<61; k++)
	{
		img = imread(path + to_string(k) + ".tif");
		cvtColor(img, gray, COLOR_BGR2GRAY);
		found = findChessboardCorners(gray, board_sz, corners); //CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS

		if (found)
		{
			imagePoints.push_back(corners);
			objectPoints.push_back(obj);
			cout << "Corners stored: " << endl;
			cout << k << endl;
			cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1), TermCriteria(TermCriteria::EPS | TermCriteria::MAX_ITER, 30, 0.1));
			drawChessboardCorners(gray, board_sz, corners, found);
			imshow("corners1", gray);
		}
		waitKey(200);
	}


	cout << "Starting Calibration" << endl;
	Mat intrinsic_matrix, distortion_coeffs;
	vector<Mat> rvecs, tvecs;

	calibrateCamera(objectPoints, imagePoints, img.size(), intrinsic_matrix, distortion_coeffs, rvecs, tvecs);
	Mat imgU;
	undistort(img, imgU, intrinsic_matrix, distortion_coeffs);
	FileStorage fs("GoPro_Left.yml", FileStorage::WRITE);
	fs << "camera_matrix" << intrinsic_matrix;
	fs << "distortion_coefficients" << distortion_coeffs;
	cout << "Done Calibration\n" << endl;

	fs.release();
	destroyAllWindows();
	return 0;
}

int imageUndistort()
{
	Mat intrinsic_matrix, distortion_coeffs;

	FileStorage fs;
	fs.open("GoPro_Left.yml", FileStorage::READ);
	fs["camera_matrix"] >> intrinsic_matrix;
	fs["distortion_coefficients"] >> distortion_coeffs;
	fs.release();

	Mat map1, map2;
	initUndistortRectifyMap(intrinsic_matrix, distortion_coeffs, Mat(), intrinsic_matrix, Size(2704, 1520), CV_16SC2, map1, map2);

	Mat frame, frame0, letmesee;
	frame0 = imread("D:/PhD/camCalibration/GoPro_Left/left_15.tif");
	remap(frame0, frame, map1, map2, INTER_LINEAR, BORDER_CONSTANT, Scalar());

	resize(frame, letmesee, Size(1280, 720));
	imshow("Undistorted", letmesee);
	waitKey(0);

	destroyAllWindows();
	return 0;
}
