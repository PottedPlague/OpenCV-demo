#include "test.h"

int test()
{
	//cv::VideoCapture cap;
	//cap.open("D:/gopro_stream/06-06-2019/Session_2/Contours/subtract_left3_open5.avi");
	//cv::Mat frame, thresh;
	//std::vector<std::vector<cv::Point>> contours;
	//std::vector<cv::Point> centroid;
	//std::vector<int> numCon;

	//for (;;)
	//{
	//	cap >> frame;
	//	if (frame.empty())
	//		break;
	//	//cv::GaussianBlur(frame_, frame_, cv::Size(5, 5), 0);
	//	cv::cvtColor(frame, thresh, cv::COLOR_BGR2GRAY);
	//	cv::threshold(thresh, thresh, 10, 255, 0);
	//	cv::findContours(thresh, contours, cv::RetrievalModes::RETR_LIST, cv::ContourApproximationModes::CHAIN_APPROX_NONE);

	//	if (contours.size() != 0)
	//	{
	//		for (size_t i = 0; i < contours.size(); i++)
	//		{
	//			double area = cv::contourArea(contours[i]);
	//			if (area > 15 && area < 180)
	//			{
	//				cv::Moments M = moments(contours[i]);
	//				int x = (int)(M.m10 / M.m00);
	//				int y = (int)(M.m01 / M.m00);
	//				cv::Point pnt = cv::Point(x, y);
	//				centroid.push_back(pnt);
	//				cv::circle(frame, pnt, 2, cv::Scalar(0, 0, 255), -1);
	//			}
	//		}
	//		cv::resize(frame, frame, cv::Size(1920, 1080));
	//		cv::imshow("Contours", frame);
	//		cv::waitKey(30);
	//	}
	//	else
	//	{
	//		std::cout << "No contours found!" << std::endl;
	//	}
	//}
	//cv::destroyAllWindows();
	//cv::waitKey(200);
	//return 0;

	////////////////////////////////////////////////////////////////////////////////
	/*cv::Mat img, gray, output;
	img = cv::imread("D:/gopro_stream/11-07-2019/Session2/Contours/left_thresh_norm_m1.tif");
	cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, gray, 4, 255, 0);
	cv::cvtColor(gray, output, cv::COLOR_GRAY2BGR);
	cv::imwrite("D:/gopro_stream/11-07-2019/Session2/Contours/left_thresh_norm4_m1.tif", output);
	cv::waitKey(200);
	return 0;*/
	////////////////////////////////////////////////////////////////////////////////
	cv::Mat frame, mask, output;
	cv::cuda::GpuMat frame_g, mask_g, output_g;
	cv::VideoCapture cap;
	cap.open("D:/gopro_stream/11-07-2019/Session2/Contours/right.avi");
	mask = cv::imread("D:/gopro_stream/11-07-2019/Session2/Contours/right_thresh_norm4_m1.tif");
	mask_g.upload(mask);
	cv::String fname = "D:/gopro_stream/11-07-2019/Session2/Contours/subtract_right4_m1.avi";
	cv::VideoWriter writer;
	writer.open(fname, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 60, mask.size());
	int k = 0;

	for (int p = 0; p < 3600; p++)
	{
		cap >> frame;
		if (frame.empty())
			break;

		frame_g.upload(frame);
		cv::cuda::subtract(frame_g, mask_g, output_g);
		output_g.download(output);
		writer.write(output);
		k++;
		std::cout << "frame " << k << std::endl;
	}

	cv::waitKey(200);
	return 0;
	////////////////////////////////////////////////////////////////////////////////
	/*cv::VideoCapture left, right;
	left.open("D:/gopro_stream/11-07-2019/Session2/Calibration/chessboard_left.mp4");
	right.open("D:/gopro_stream/11-07-2019/Session2/Calibration/chessboard_right.mp4");
	cv::Mat imgleft, imgright, imgleft_r, imgright_r;
	left >> imgleft;
	right >> imgright;
	cv::resize(imgleft, imgleft_r, cv::Size(1920, 1080));
	cv::resize(imgright, imgright_r, cv::Size(1920, 1080));
	cv::imshow("Left channel", imgleft_r);
	cv::imshow("Right channel", imgright_r);
	int k = 0;
	cv::String fname_l = "D:/gopro_stream/11-07-2019/Session2/Calibration/left/calib";
	cv::String fname_r = "D:/gopro_stream/11-07-2019/Session2/Calibration/right/calib";

	for (;;)
	{
		int j = cv::waitKey(5);
		if (j == 32)
		{
			left >> imgleft;
			right >> imgright;
			if (imgleft.empty() && imgright.empty())
				break;

			cv::resize(imgleft, imgleft_r, cv::Size(1920, 1080));
			cv::resize(imgright, imgright_r, cv::Size(1920, 1080));
			cv::imshow("Left channel", imgleft_r);
			cv::imshow("Right channel", imgright_r);
			cv::waitKey(10);
		}
		if (j == 27)
			break;
		if (j == 115)
		{
			cv::imwrite(fname_l + intToStr(k) + ".tif", imgleft);
			cv::imwrite(fname_r + intToStr(k) + ".tif", imgright);
			k++;
		}
	}
	cv::destroyAllWindows();
	cv::waitKey(200);
	return 0;*/
	////////////////////////////////////////////////////////////////////////////////
	/*cv::VideoCapture cap(0 + cv::CAP_DSHOW);
	cv::Mat frame;

	for (;;)
	{
		cap >> frame;

		cv::line(frame, cv::Point(0, cap.get(4) / 2), cv::Point(cap.get(3), cap.get(4) / 2), cv::Scalar(0, 0, 255));
		cv::line(frame, cv::Point(cap.get(3) / 2, 0), cv::Point(cap.get(3) / 2, cap.get(4)), cv::Scalar(255, 0, 0));
		cv::imshow("Corners", frame);
		if (cv::waitKey(16) == 27)
			break;
	}
	cv::destroyAllWindows();
	cv::waitKey(0);
	return 0;*/
	////////////////////////////////////////////////////////////////////////////////

	/*cv::Mat frame;
	cv::VideoCapture cap1, cap2;
	cap1.open("D:/gopro_stream/06-06-2019/Session_2/Left/GH010010_synced.avi");
	cap2.open("D:/gopro_stream/06-06-2019/Session_2/Left/GH020010.mp4");
	int k = 0;
	int total1 = cap1.get(cv::CAP_PROP_FRAME_COUNT);
	int total2 = cap2.get(cv::CAP_PROP_FRAME_COUNT);

	cv::VideoWriter out("D:/gopro_stream/06-06-2019/Session_2/Left/complete.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 60, cv::Size(cap1.get(cv::CAP_PROP_FRAME_WIDTH), cap1.get(cv::CAP_PROP_FRAME_HEIGHT)));

	for (;;)
	{
		cap1 >> frame;
		if (frame.empty())
		{
			k = 0;
			break;
		}
		out.write(frame);
		int perc = (100 * k) / total1;
		std::cout << "Processing clip 1..." << perc << "%" << std::endl;
		k++;
	}

	for (;;)
	{
		cap2 >> frame;
		if (frame.empty())
		{
			k = 0;
			break;
		}
		out.write(frame);
		int perc = (100 * k) / total2;
		std::cout << "Processing clip 2..." << perc << "%" << std::endl;
		k++;
	}
	out.release();
	cv::waitKey(200);
	return 0;*/
}

int masking()
{
	cv::VideoCapture cap;
	cap.open("D:/gopro_stream/11-07-2019/Session2/Contours/left.avi");
	cv::Mat board = cv::Mat::zeros(cv::Size(2704, 1520), CV_16UC3);
	cv::Mat frame, frame16, board8;
	cv::cuda::GpuMat frame_g, frame_g16, board_g, board_g8;
	board_g.upload(board);
	double alpha = 1.0 / 2340;
	double beta = 1.0;
	int k = 0;
	int pxval;

	for (int p = 0;p < 3600; p++)
	{
		cap >> frame;
		if (frame.empty())
			break;

		frame_g.upload(frame);
		frame_g.convertTo(frame_g16, CV_16UC3);
		cv::cuda::threshold(frame_g16, frame_g16, 100, 65535, 0);
		cv::cuda::addWeighted(frame_g16, alpha, board_g, beta, 0.0, board_g, CV_16UC3);
		std::cout << "Frame " << k << std::endl;
		k++;
	}
	board_g.download(board);
	cv::imshow("Output", board);
	cv::waitKey(0);
	board.convertTo(board8, CV_8UC3, 1.0/256);
	cv::imwrite("D:/gopro_stream/11-07-2019/Session2/Contours/left_thresh_norm_m1.tif", board8);
	cv::destroyAllWindows();
	cv::waitKey(200);
	return 0;
}

int addedMorph()
{
	cv::Mat frame, output;
	cv::cuda::GpuMat frame_g, output_g;
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
	cv::Ptr<cv::cuda::Filter> pMorph = cv::cuda::createMorphologyFilter(cv::MORPH_OPEN, CV_8UC1, kernel);
	cv::VideoCapture cap;
	cap.open("D:/gopro_stream/11-07-2019/Session2/Contours/subtract_right4_m1.avi");
	cv::VideoWriter out;
	out.open("D:/gopro_stream/11-07-2019/Session2/Contours/subtract_right4_open3_m1.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 60, cv::Size(2704, 1520));
	int k = 0;

	for (;;)
	{
		cap >> frame;
		if (frame.empty())
			break;

		frame_g.upload(frame);
		cv::cuda::cvtColor(frame_g, frame_g, cv::COLOR_BGR2GRAY);
		pMorph->apply(frame_g, output_g);
		cv::cuda::cvtColor(output_g, output_g, cv::COLOR_GRAY2BGR);
		output_g.download(output);
		out.write(output);
		std::cout << "Frame " << k << std::endl;
		k++;
	}
	out.release();
	return 0;
}