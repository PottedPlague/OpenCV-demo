/*
Aligns the chessboard pattern with the camera plane, 
so that they are parrallel to each other.

Date modified: 14/05/2019
*/

#include "parallelAlign.h"
#include "tools.h"

int findParallel()
{
	double i = 1;
	double green = 0;
	double red = 0;
	double avg_green = 0;
	double avg_red = 0;
	double green_disp = 0;
	double ori_x = 0;
	double ori_y = 0;
	cv::VideoCapture cap(0 + cv::CAP_DSHOW);
	//cap.set(cv::CAP_PROP_FRAME_WIDTH, 2704);
	//cap.set(cv::CAP_PROP_FRAME_HEIGHT, 1520);

	bool fastMode = true;
	int chess_w = 9;
	int chess_h = 6;
	int chess_n = chess_w * chess_h;
	cv::Size patternSize = cv::Size(chess_w, chess_h);
	cv::Mat frame, gray, thresh, pyrd1, pyrd2, pyrd3;
	bool found = false;
	cv::Point3i origin = cv::Point3i((chess_h - 1) / 2, (chess_w - 1) / 2, 0);
	char cam;
	printf("Which camera do you want to calibrate: ");
	cam = getchar();

	cv::String filename = "D:/Projects/bee_tracking/Project1/GoPro_";
	cv::FileStorage fs(filename + cam + ".yml", cv::FileStorage::READ);
	cv::Mat intrinsics, distortion, rvec, tvec;
	fs["camera_matrix"] >> intrinsics;
	fs["distortion_coefficients"] >> distortion;
	fs.release();

	std::cout << cap.get(3) << ", " << cap.get(4) << std::endl;
	std::vector<cv::Point3f> object_points;
	std::vector<cv::Point2f> image_points;
	std::vector<cv::Point2f> corners;
	std::vector<cv::Point3f> axis;

	axis.push_back(cv::Point3f(origin.x + 3.0, origin.y, origin.z));				//x
	axis.push_back(cv::Point3f(origin.x - 3.0, origin.y, origin.z));
	axis.push_back(cv::Point3f(origin.x, origin.y + 3, origin.z));				//y
	axis.push_back(cv::Point3f(origin.x, origin.y - 3, origin.z));
	axis.push_back(cv::Point3f(origin.x, origin.y, origin.z + 6));				//z
	axis.push_back(cv::Point3f(origin.x, origin.y, origin.z - 6));

	for (int j = 0; j < chess_n; j++)
		object_points.push_back(cv::Point3f(j / chess_w, j%chess_w, 0.0f));	//(0,0,0), (0,1,0),...,(5,8,0)

	for (;;)
	{
		cap >> frame;
		if (frame.empty())
			return -1;

		if (fastMode)
		{
			cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
			pyrDown(gray, pyrd1, cv::Size(cap.get(3)/2, cap.get(4)/2));
			//pyrDown(pyrd1, pyrd2, cv::Size(cap.get(3)/4, cap.get(4)/4));
			found = cv::findChessboardCorners(pyrd1, patternSize, corners, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE); //cv::CALIB_CB_NORMALIZE_IMAGE
			if (found)
			{
				for (int i = 0; i < corners.size(); i++)
				{ 
					corners[i] = corners[i] * 2;
				}
				cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.1));
			}
		}
		else
		{
			found = cv::findChessboardCorners(frame, patternSize, corners, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);	//cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE
		}
		if (found)
		{
			cv::drawChessboardCorners(frame, patternSize, corners, found);
			cv::solvePnP(cv::Mat(object_points), cv::Mat(corners), intrinsics, distortion, rvec, tvec);
			cv::projectPoints(cv::Mat(axis), rvec, tvec, intrinsics, distortion, image_points);
			cv::line(frame, image_points[0], image_points[1], cv::Scalar(0, 0, 255), 4);
			cv::line(frame, image_points[2], image_points[3], cv::Scalar(255, 0, 0), 4);
			cv::line(frame, image_points[4], image_points[5], cv::Scalar(0, 255, 0), 4);

			green = cv::norm(image_points[4] - image_points[5]);
			red = cv::norm(image_points[0] - image_points[1]);
			avg_green += green;
			avg_red += red;

			if (i == 10)
			{
				avg_green = avg_green / i;
				green_disp = avg_green;
				avg_red = avg_red / i;
				ori_x = corners[22].x - cap.get(3) / 2;
				ori_y = corners[22].y - cap.get(4) / 2;
				std::cout << "Green is: " << avg_green << "\n" << "Red is: " << avg_red << "\n";
				std::cout << "Origin is: (" << ori_x << ", " << ori_y << ")" << std::endl;
				avg_green = 0;
				avg_red = 0;
				i = 1;
				
			}
			cv::putText(frame, "length of green line: " + dblToStr(green_disp), cv::Point(10, 40), 1, 2, cv::Scalar(0, 255, 0), 2);
			i++;
		}
		cv::line(frame, cv::Point(0, cap.get(4) / 2), cv::Point(cap.get(3), cap.get(4) / 2), cv::Scalar(0, 0, 255));
		cv::line(frame, cv::Point(cap.get(3)/2, 0), cv::Point(cap.get(3) / 2, cap.get(4)), cv::Scalar(255, 0, 0));
		cv::imshow("Corners", frame);
		if (cv::waitKey(16) == 27)
			break;
	}
	cv::destroyAllWindows();
	cv::waitKey(300);
	return 0;
}