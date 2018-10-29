#include "birdsEye.h"
#include <opencv2\opencv.hpp>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

int birdsEyeView()
{
	int board_w = 9;
	int board_h = 6;

	Size board_sz = Size(board_w, board_h);
	int board_n = board_h * board_w;

	Mat CM1, D1;

	FileStorage fs;
	fs.open("monocam.yml", FileStorage::READ);
	fs["CM"] >> CM1;
	fs["D"] >> D1;
	if (!fs.isOpened() || CM1.empty() || D1.empty())
	{
		cout << "Error: Couldn't load intrinsic parameters." << endl;
		return 1;
	}
	fs.release();

	Mat gray_image, image, image0 = imread("Picture14.jpg", 1);
	if (image0.empty())
	{
		cout << "Couldn't load image." << endl;
		return 2;
	}
	undistort(image0, image, CM1, D1);
	cvtColor(image, gray_image, CV_BGR2GRAY);
	imshow("image0", image0);
	imshow("image", image);
	imshow("gray", gray_image);
	waitKey(300);

	vector<Point2f> corners;
	bool found = findChessboardCorners(
		gray_image,
		board_sz,
		corners,
		CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FILTER_QUADS
	);
	if (!found)
	{
		cout << "Couldn't acquire checkerboard." << endl;
		destroyAllWindows();
		return 3;
	}

	cornerSubPix(
		gray_image,
		corners,
		Size(11, 11),
		Size(-1, -1),
		TermCriteria(
			TermCriteria::EPS | TermCriteria::COUNT,
			30, 0.1
		)
	);

	Point2f objPts[4], imgPts[4];
	objPts[0].x = 0;			objPts[0].y = 0;
	objPts[1].x = board_w-1;	objPts[1].y = 0;
	objPts[2].x = 0;			objPts[2].y = board_h-1;
	objPts[3].x = board_w-1;	objPts[3].y = board_h-1;
	imgPts[0] = corners[0];
	imgPts[1] = corners[board_w-1];
	imgPts[2] = corners[(board_h-1)*board_w];
	imgPts[3] = corners[(board_h-1)*board_w + board_w-1];

	circle(image, imgPts[0], 9, Scalar(255, 0, 0), 3);
	circle(image, imgPts[1], 9, Scalar(0, 255, 0), 3);
	circle(image, imgPts[2], 9, Scalar(0, 0, 255), 3);
	circle(image, imgPts[3], 9, Scalar(0, 255, 255), 3);

	drawChessboardCorners(image, board_sz, corners, found);
	imshow("Checkers", image);

	Mat H = getPerspectiveTransform(objPts, imgPts);
	double Z = 25; 
	Mat birds_image;
	for (;;)
	{
		H.at<double>(2, 2) = Z;
		warpPerspective(
			image,
			birds_image,
			H,
			image.size(),
			WARP_INVERSE_MAP | INTER_LINEAR,
			BORDER_CONSTANT,
			Scalar::all(0)
		);
		imshow("Birds_Eye", birds_image);
		int key = waitKey() & 255;
		if (key == 'u')
			Z += 0.5;
		if (key == 'd')
			Z -= 0.5;
		if (key == 27)
			break;
	}

	destroyAllWindows();
	vector<Point2f> image_points;
	vector<Point3f> object_points;
	for (int i = 0; i < 4; ++i)
	{
		image_points.push_back(imgPts[i]);
		object_points.push_back(Point3f(objPts[i].x, objPts[i].y, 0));
	}

	Mat rvec, tvec, rmat;
	solvePnP(
		object_points,
		image_points,
		CM1,
		Mat(),
		rvec,
		tvec
	);
	Rodrigues(rvec, rmat);

	//print
	cout << "rotation matrix: " << rmat << endl;
	cout << "transition vector: " << tvec << endl;
	cout << "homography matrix: " << H << endl;
	cout << "inverted homography matrix: " << H.inv() << endl;

	return 0;
}
