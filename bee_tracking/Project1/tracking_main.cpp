#include "tracking_main.h"

int trackingMain(int maxSeparation, int maxFrameLoss, int maxTraceLength, int idCounter)
{
	vector<vector<cv::Point3d>> coor3d;
	vector<cv::Point3d> detection;
	cv::VideoCapture capL, capR;
	capL.open("F:/renderoutput/ball20/left.avi");
	capR.open("F:/renderoutput/ball20/right.avi");
	Detectors detectorL, detectorR;
	Tracker tracker(maxSeparation, maxFrameLoss, maxTraceLength, idCounter);						
	//thresholds of: max separation, max frameloss, max trace length; 
	//and ID counter; default(50, 10, 40, 100)
	
	int frameCounter = 0;
	std::vector<std::vector<Track*>> successfulMatches;
	bool pause = 0;
	cv::Mat frameL, frameR;

	///////////////////////////////////////////////
	/*std::vector<int> gapIDs;
	double gap;
	std::vector<std::vector<cv::Point>> leftScene;
	std::vector<std::vector<cv::Point>> rightScene;
	cv::Mat lScene = cv::Mat::zeros(1080, 1920, CV_8UC3);
	cv::Mat rScene = cv::Mat::zeros(1080, 1920, CV_8UC3);*/
	///////////////////////////////////////////////

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

		///////////////////////////////////////////////
		/*leftScene.push_back(centersL);
		rightScene.push_back(centersR);*/
		///////////////////////////////////////////////

		/*if (frameCounter == 201)
			cv::waitKey(10);*/
		if (centersL.size() > 0 && centersR.size() > 0)
		{
			detection = doMatch(centersL, centersR);
			coor3d.push_back(detection);

			///////////////////////////////////////////////
			/*if (frameCounter > 1)
			{
				if (!coor3d[frameCounter].empty())
				{
					gap = cv::norm(coor3d[frameCounter][0] - coor3d[frameCounter - 1][0]);
					std::cout << gap << std::endl;
					if (gap > 10)
						gapIDs.push_back(frameCounter);
				}
				else
				{
					std::cout << "No detection in current frame." << std::endl;
				}
			}*/

			///////////////////////////////////////////////

			tracker.update(detection);
		}
		frameCounter++;
		cout << frameCounter << endl;
		/*int k = cv::waitKey(2);
		if (k == 27)
			break;*/
	}

	///////////////////////////////////////////////
	/*for (size_t i = 0; i < leftScene.size(); i++)
	{
		for (size_t j = 0; j < leftScene[i].size(); j++)
		{
			cv::circle(lScene, leftScene[i][j], 2, cv::Scalar(0, 0, 255), -1);
			cv::imshow("Left Scene", lScene);
			cv::waitKey(16);
		}
	}
	for (size_t i = 0; i < rightScene.size(); i++)
	{
		for (size_t j = 0; j < rightScene[ie].size(); j++)
		{
			cv::circle(rScene, rightScene[i][j], 2, cv::Scalar(0, 255, 0), -1);
			cv::imshow("Right Scene", rScene);
			cv::waitKey(16);
		}
	}
	
	cv::waitKey(300);*/
	///////////////////////////////////////////////

	std::vector<Track> allTracks;
	allTracks.reserve(tracker.tracks.size() + tracker.completeTracks.size());
	allTracks.insert(allTracks.end(), tracker.tracks.begin(), tracker.tracks.end());
	allTracks.insert(allTracks.end(), tracker.completeTracks.begin(), tracker.completeTracks.end());
	saveVecOfVecOfPoints(coor3d, "F:/renderoutput/ball20/coor3d.xml");
	saveVecOfVecOfPoints(allTracks, "F:/renderoutput/ball20/allTracks.xml");
	
	visualisation3d(allTracks);
	cv::destroyAllWindows();
	return 0;
}
