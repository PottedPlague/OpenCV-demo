#include "tracking_main.h"

int trackingMain()
{
	vector<vector<cv::Point3d>> coor3d;
	vector<cv::Point3d> detection;
	cv::VideoCapture capL, capR;
	capL.open("F:/renderoutput/ball20/video/left20.avi");
	capR.open("F:/renderoutput/ball20/video/right20.avi");
	Detectors detectorL, detectorR;


	Tracker tracker(50, 20, 1700, 100);						
	//thresholds of: max separation, max frameloss, max trace length; 
	//and ID counter; default(50, 10, 40, 100)
	
	int frameCounter = 0;
	std::vector<std::vector<Track*>> successfulMatches;
	bool pause = 0;
	cv::Mat frameL, frameR;

	for (;;)
	{
		capL >> frameL;
		capR >> frameR;
		if (frameL.empty()||frameR.empty())
		{
			cv::destroyAllWindows();
			break;
		}
		std::vector<cv::Point> centersL = detectorL.detect(frameL);
		std::vector<cv::Point> centersR = detectorR.detect(frameR);

		if (centersL.size() > 0 && centersR.size() > 0)
		{
			detection = doMatch(centersL, centersR);
			coor3d.push_back(detection);
			tracker.update(detection);
			//trackerL.update(centersL);
			//trackerR.update(centersR);
			/*for (int i = 0; i < trackerL.tracks.size(); i++)
			{
				if (trackerL.tracks[i].trace.size() > 1)
				{
					for (int j = 0; j < trackerL.tracks[i].trace.size() - 1; j++)
					{
						int clr = trackerL.tracks[i].clr_id % 9;
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
						int clr = trackerR.tracks[i].clr_id % 9;
						cv::line(frameR, trackerR.tracks[i].trace[j], trackerR.tracks[i].trace[j + 1], track_colours[clr], 2);
					}
				}
			}
			cv::imshow("Left scene", frameL);
			cv::imshow("Right scene", frameR);*/
		}
		frameCounter++;
		cout << frameCounter << endl;
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

	visualisation3d(tracker.tracks);
	cv::destroyAllWindows();
	return 0;
}
