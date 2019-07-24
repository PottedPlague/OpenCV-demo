#include "videoTrim.h"


int locate(std::string filename)
{
	cv::VideoCapture camL, camR;
	camL.open(filename);

	cv::Mat frame, frame0;
	int k = 1;
	camL >> frame;
	cv::resize(frame, frame0, cv::Size(1920, 1080));
	cv::imshow("Left GoPro", frame0);
	std::cout << "Frame #" << k << std::endl;

	for (;;)
	{
		int j = cv::waitKey(5);
		if (j == 32)
		{
			camL >> frame;
			if (frame.empty())
			{
				std::cout << "Empty frame!" << std::endl;
				continue;
			}
				
			k++;
			cv::resize(frame, frame0, cv::Size(1920, 1080));
			cv::imshow("Left GoPro", frame0);
			std::cout << "Frame #" << k << std::endl;
			cv::waitKey(10);
		}
		if (j == 27)
			return 0;
	}
	cv::destroyAllWindows();
	cv::waitKey(200);
	return 0;
}

int trim(std::string fileIn, std::string fileOut)
{
	int frameStart, frameEnd, totalFrames, k = 1, percent;
	std::cout << "Select the #frame to start from: ";
	std::cin >> frameStart;
	std::cout << "Select the #frame to end with: ";
	std::cin >> frameEnd;

	cv::Mat frame;
	cv::VideoCapture vid;
	vid.open(fileIn);

	cv::VideoWriter writer(fileOut, cv::VideoWriter::fourcc('H', '2', '6', '4'), 60, cv::Size(vid.get(cv::CAP_PROP_FRAME_WIDTH), vid.get(cv::CAP_PROP_FRAME_HEIGHT)));
	totalFrames = frameEnd;
	if (frameEnd == 0)
	{
		frameEnd = INT_MAX;
		totalFrames = vid.get(cv::CAP_PROP_FRAME_COUNT);
	}	

	for (int i = 0; i < totalFrames; i++)
	{
		vid >> frame;
		if (frame.empty())
			continue;
		if (k < frameStart)
		{
			k++;
		}
		else if (k > frameEnd)
		{
			break;
		}
		else
		{
			writer.write(frame);
			percent = (k - frameStart) * 100 / (totalFrames - frameStart);
			std::cout << "Processing..." << percent << "%" << std::endl;
			k++;
		}
	}
	writer.release();
	return 0;
}