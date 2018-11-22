#include "birdsEye.h"
#include <opencv2\opencv.hpp>
#include <stdio.h>
#include <iostream>

int birdsEyeView()
{
	int board_w = 9;
	int board_h = 6;

	cv::Size board_sz = cv::Size(board_w, board_h);
	int board_n = board_h * board_w;

	cv::Mat CM1, D1;

	cv::FileStorage fs;
	fs.open("monocam.yml", cv::FileStorage::READ);
	fs["CM"] >> CM1;
	fs["D"] >> D1;
	if (!fs.isOpened() || CM1.empty() || D1.empty())
	{
		std::cout << "Error: Couldn't load intrinsic parameters." << std::endl;
		return 1;
	}
	fs.release();

	cv::Mat gray_image, image, image0 = cv::imread("Picture14.jpg", 1);
	if (image0.empty())
	{
		std::cout << "Couldn't load image." << std::endl;
		return 2;
	}
	undistort(image0, image, CM1, D1);
	cvtColor(image, gray_image, CV_BGR2GRAY);
	imshow("image0", image0);
	imshow("image", image);
	imshow("gray", gray_image);
	cv::waitKey(300);

	std::vector<cv::Point2f> corners;
	bool found = findChessboardCorners(
		gray_image,
		board_sz,
		corners,
		cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FILTER_QUADS
	);
	if (!found)
	{
		std::cout << "Couldn't acquire checkerboard." << std::endl;
		cv::destroyAllWindows();
		return 3;
	}

	cornerSubPix(
		gray_image,
		corners,
		cv::Size(11, 11),
		cv::Size(-1, -1),
		cv::TermCriteria(
			cv::TermCriteria::EPS | cv::TermCriteria::COUNT,
			30, 0.1
		)
	);

	cv::Point2f objPts[4], imgPts[4];
	objPts[0].x = 0;			objPts[0].y = 0;
	objPts[1].x = board_w-1;	objPts[1].y = 0;
	objPts[2].x = 0;			objPts[2].y = board_h-1;
	objPts[3].x = board_w-1;	objPts[3].y = board_h-1;
	imgPts[0] = corners[0];
	imgPts[1] = corners[board_w-1];
	imgPts[2] = corners[(board_h-1)*board_w];
	imgPts[3] = corners[(board_h-1)*board_w + board_w-1];

	circle(image, imgPts[0], 9, cv::Scalar(255, 0, 0), 3);
	circle(image, imgPts[1], 9, cv::Scalar(0, 255, 0), 3);
	circle(image, imgPts[2], 9, cv::Scalar(0, 0, 255), 3);
	circle(image, imgPts[3], 9, cv::Scalar(0, 255, 255), 3);

	drawChessboardCorners(image, board_sz, corners, found);
	imshow("Checkers", image);

	cv::Mat H = getPerspectiveTransform(objPts, imgPts);
	double Z = 25; 
	cv::Mat birds_image;
	for (;;)
	{
		H.at<double>(2, 2) = Z;
		warpPerspective(
			image,
			birds_image,
			H,
			image.size(),
			cv::WARP_INVERSE_MAP | cv::INTER_LINEAR,
			cv::BORDER_CONSTANT,
			cv::Scalar::all(0)
		);
		imshow("Birds_Eye", birds_image);
		int key = cv::waitKey() & 255;
		if (key == 'u')
			Z += 0.5;
		if (key == 'd')
			Z -= 0.5;
		if (key == 27)
			break;
	}

	cv::destroyAllWindows();
	std::vector<cv::Point2f> image_points;
	std::vector<cv::Point3f> object_points;
	for (int i = 0; i < 4; ++i)
	{
		image_points.push_back(imgPts[i]);
		object_points.push_back(cv::Point3f(objPts[i].x, objPts[i].y, 0));
	}

	cv::Mat rvec, tvec, rmat;
	solvePnP(
		object_points,
		image_points,
		CM1,
		cv::Mat(),
		rvec,
		tvec
	);
	Rodrigues(rvec, rmat);

	//print
	std::cout << "rotation matrix: " << rmat << std::endl;
	std::cout << "transition vector: " << tvec << std::endl;
	std::cout << "homography matrix: " << H << std::endl;
	std::cout << "inverted homography matrix: " << H.inv() << std::endl;

	return 0;
}
