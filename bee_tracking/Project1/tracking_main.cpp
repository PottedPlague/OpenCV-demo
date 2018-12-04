#include "tracking_main.h"

int trackingMain()
{
	cv::VideoCapture cap;
	cap.open("F:/renderoutput/multi/morph/right_long.avi");
	Detectors detector;
	Tracker tracker(50, 20, 40, 100);						//thresholds of: max separation, max frameloss, max trace length; and ID counter; default(50, 10, 40, 100)
	int skip_frame_count = 0;

	std::vector<cv::Scalar> track_colours = { 
		cv::Scalar(255, 0, 0), cv::Scalar(0, 255, 0), cv::Scalar(0, 0, 255), 
		cv::Scalar(255, 255, 0), cv::Scalar(0, 255, 255), cv::Scalar(255, 0, 255), 
		cv::Scalar(255, 127, 255), cv::Scalar(127, 0, 255), cv::Scalar(127, 0, 127) };

	bool pause = 0;
	cv::Mat frame, orig_frame;

	for (;;)
	{
		cap >> frame;
		if (frame.empty())
		{
			cv::destroyAllWindows();
			return -1;
		}

		orig_frame = frame.clone();
		if (skip_frame_count < 2)
		{
			skip_frame_count++;
			continue;
		}

		std::vector<cv::Point> centers = detector.detect(frame, 'l');

		if (centers.size() > 0)
		{
			tracker.update(centers);
			for (int i = 0; i < tracker.tracks.size(); i++)
			{
				if (tracker.tracks[i].trace.size() > 1)
				{
					for (int j = 0; j < tracker.tracks[i].trace.size() - 1; j++)
					{
						//int x1 = tracker.tracks[i].trace[j].x;
						//int y1 = tracker.tracks[i].trace[j].y;
						//int x2 = tracker.tracks[i].trace[j + 1].x;
						//int y2 = tracker.tracks[i].trace[j + 1].y;
						int clr = tracker.tracks[i].track_id_ % 9;
						cv::line(frame, tracker.tracks[i].trace[j], tracker.tracks[i].trace[j + 1], track_colours[clr], 2);
					}
				}
			}
			cv::imshow("Tracking", frame);
		}

		int k = cv::waitKey(16);
		if (k == 27)
			break;
		if (k == 112)
		{
			pause = !pause;
			if (pause == 1)
			{
				std::cout << "Code is paused. Press 'p' to resume. " << std::endl;
				while (pause = 1)
				{
					int key = cv::waitKey(30);
					if (key == 112)
					{
						pause = 0;
						break;
					}
				}
			}
		}
	}
	cv::destroyAllWindows();
	return 0;
}
