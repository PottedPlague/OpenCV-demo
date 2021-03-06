#include "sim_tracker.h"

int simTracker()
{
	cv::VideoCapture capL, capR;
	capL.open("F:/renderoutput/multi/morph/two_left.avi");
	capR.open("F:/renderoutput/multi/morph/two_right.avi");
	Detectors detectorL, detectorR;
	Tracker trackerL(50, 20, 40, 100);						//thresholds of: max separation, max frameloss, max trace length; and ID counter; default(50, 10, 40, 100)
	Tracker trackerR(50, 20, 40, 100);
	int skip_frame_count = 0;
	std::vector<std::vector<Track*>> successfulMatches;

	std::vector<cv::Scalar> track_colours = {
		cv::Scalar(255, 0, 0), cv::Scalar(0, 255, 0), cv::Scalar(0, 0, 255),
		cv::Scalar(255, 255, 0), cv::Scalar(0, 255, 255), cv::Scalar(255, 0, 255),
		cv::Scalar(255, 127, 255), cv::Scalar(127, 0, 255), cv::Scalar(127, 0, 127) };

	bool pause = 0;
	cv::Mat frameL, frameR, orig_frameL, orig_frameR;

	for (;;)
	{
		capL >> frameL;
		capR >> frameR;
		if (frameL.empty() || frameR.empty())
		{
			cv::destroyAllWindows();
			break;
		}

		orig_frameL = frameL.clone();
		orig_frameR = frameR.clone();
		if (skip_frame_count < 5)
		{
			skip_frame_count++;
			continue;
		}

		std::vector<cv::Point> centersL = detectorL.detect(frameL);
		std::vector<cv::Point> centersR = detectorR.detect(frameR);

		if (centersL.size() > 0 && centersR.size() > 0)
		{
			trackerL.update(centersL);
			trackerR.update(centersR);
			std::cout << "No. tracks in cam L and R: " << trackerL.tracks.size() << ", " << trackerR.tracks.size() << std::endl;
			for (int i = 0; i < trackerL.tracks.size(); i++)
			{
				if (trackerL.tracks[i].trace.size() > 1)
				{
					for (int j = 0; j < trackerL.tracks[i].trace.size() - 1; j++)
					{
						int clr = trackerL.tracks[i].track_id_ % 9;
						cv::line(frameL, trackerL.tracks[i].trace[j], trackerL.tracks[i].trace[j + 1], track_colours[clr], 2);
					}
				}
			}

			for (int i = 0; i < trackerR.tracks.size(); i++)
			{
				if (trackerR.tracks[i].trace.size() > 1)
				{
					for (int j = 0; j < trackerR.tracks[i].trace.size() - 1; j++)
					{
						int clr = trackerR.tracks[i].track_id_ % 9;
						cv::line(frameR, trackerR.tracks[i].trace[j], trackerR.tracks[i].trace[j + 1], track_colours[clr], 2);
					}
				}
			}
			cv::imshow("Left scene", frameL);
			cv::imshow("Right scene", frameR);
		}

		int k = cv::waitKey(5);
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