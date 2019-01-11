#include "parallelAlign.h"

int findParallel()
{
	double i = 1;
	double green = 0;
	double red = 0;
	double avg_green = 0;
	double avg_red = 0;
	double ori_x = 0;
	double ori_y = 0;
	cv::VideoCapture cap(1);
	cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

	int chess_w = 9;
	int chess_h = 6;
	int chess_n = chess_w * chess_h;
	cv::Size patternSize = cv::Size(chess_w, chess_h);
	cv::Mat frame;
	bool found = false;


	cv::String filename = "D:/pic/monocam.yml";
	cv::FileStorage fs(filename, cv::FileStorage::READ);
	cv::Mat intrinsics, distortion, rvec, tvec;
	fs["CM"] >> intrinsics;
	fs["D"] >> distortion;
	fs.release();

	std::vector<cv::Point3f> object_points;
	std::vector<cv::Point2f> image_points;
	std::vector<cv::Point2f> corners;
	std::vector<cv::Point3f> axis;

	axis.push_back(cv::Point3f(5.0, 4.0, 0.0));
	axis.push_back(cv::Point3f(-1.0, 4.0, 0.0));
	axis.push_back(cv::Point3f(2.0, 7.0, 0.0));
	axis.push_back(cv::Point3f(2.0, 1.0, 0.0));
	axis.push_back(cv::Point3f(2.0, 4.0, 3.0));
	axis.push_back(cv::Point3f(2.0, 4.0, -3.0));

	for (int j = 0; j < chess_n; j++)
		object_points.push_back(cv::Point3f(j / chess_w, j%chess_w, 0.0f));	//(0,0,0), (0,1,0),...,(5,8,0)

	for (;;)
	{
		cap >> frame;
		if (frame.empty())
			return -1;
		found = cv::findChessboardCorners(frame, patternSize, corners, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);
		if (found)
		{
			//cv::drawChessboardCorners(frame, patternSize, corners, found);
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
				avg_red = avg_red / i;
				ori_x = corners[0].x - cap.get(3) / 2;
				ori_y = corners[0].y - cap.get(4) / 2;
				std::cout << "Green is: " << avg_green << "\n" << "Red is: " << avg_red << "\n";
				std::cout << "Origin is: (" << ori_x << ", " << ori_y << ")" << std::endl;
				avg_green = 0;
				avg_red = 0;
				i = 1;
			}

			i++;


				
		}
		cv::imshow("Corners", frame);
		if (cv::waitKey(16) == 27)
			break;
	}

	cv::destroyAllWindows();
	cv::waitKey(300);
	return 0;
}