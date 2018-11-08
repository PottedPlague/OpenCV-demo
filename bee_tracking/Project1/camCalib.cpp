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

	cv::String path = "D:/pic/calib/right/right_";

	Mat img, gray;

	bool found = false;

	vector<Point3f> obj;										//in "chessboard corners" unit
	for (int j = 0; j < board_n; j++)
		obj.push_back(Point3f(j / board_w, j%board_w, 0.0f));	//(0,0,0), (0,1,0),...,(5,8,0)

	for (int k = 1; k<50; k++)
	{
		img = imread(path + to_string(k) + ".tif");
		cvtColor(img, gray, CV_BGR2GRAY);
		found = findChessboardCorners(gray, board_sz, corners); //CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS

		if (found)
		{
			imagePoints.push_back(corners);
			objectPoints.push_back(obj);
			cout << "Corners stored: " << endl;
			cout << k << endl;
			cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
			drawChessboardCorners(gray, board_sz, corners, found);
			imshow("corners1", gray);
		}
		waitKey(500);
	}


	cout << "Starting Calibration" << endl;
	Mat intrinsic_matrix, distortion_coeffs;
	//Mat CM = Mat(3, 3, CV_64FC1);
	//Mat D;
	vector<Mat> rvecs, tvecs;

	calibrateCamera(objectPoints, imagePoints, img.size(), intrinsic_matrix, distortion_coeffs, rvecs, tvecs);
	Mat imgU;
	undistort(img, imgU, intrinsic_matrix, distortion_coeffs);
	FileStorage fs("monocam.yml", FileStorage::WRITE);
	fs << "camera_matrix" << intrinsic_matrix;
	fs << "distortion_coefficients" << distortion_coeffs;
	cout << "Done Calibration\n" << endl;

	cout << "Starting Rectification\n" << endl;
	fs.release();
	destroyAllWindows();
	return 0;
}
