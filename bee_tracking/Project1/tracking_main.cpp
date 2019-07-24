#include "tracking_main.h"

int trackingMain(int maxSeparation, int maxFrameLoss, int maxTraceLength, int idCounter)
{
	vector<vector<cv::Point3d>> coor3d;
	vector<cv::Point3d> detection;
	cv::VideoCapture capL, capR;
	capL.open("D:/gopro_stream/11-07-2019/Session2/Contours/subtract_left10_open5.avi");
	capR.open("D:/gopro_stream/11-07-2019/Session2/Contours/subtract_right4_open3_m1.avi");
	Detectors detectorL, detectorR;
	Tracker tracker(maxSeparation, maxFrameLoss, maxTraceLength, idCounter);						
	//thresholds of: max separation, max frameloss, max trace length; 
	//and ID counter; default(50, 10, 40, 100)
	
	int frameCounter = 0;
	std::vector<std::vector<Track*>> successfulMatches;
	bool pause = 0;
	cv::Mat frameL, frameR;

	for (int p = 0; p < 2300; p++)	///////////////////////////////////////////////added p
	{
		capL >> frameL;
		capR >> frameR;
		if (frameL.empty()||frameR.empty())
		{
			cv::destroyAllWindows();
			break;
		}

		///////////////////////////////////////////////begin
		/*if (p < 420)
			continue;*/
		///////////////////////////////////////////////end

		std::vector<cv::Point> centersL = detectorL.detect(frameL);
		std::vector<cv::Point> centersR = detectorR.detect(frameR);

		/*if (frameCounter == 201)
			cv::waitKey(10);*/
		frameCounter++;
		cout << frameCounter << endl;
		if (centersL.size() > 0 && centersR.size() > 0)
		{
			detection = doMatch(centersL, centersR, frameL, frameR);	///////////////////////////////////////////////added two more arguments
			if (detection.size() == 0)
			{
				cout << "Frame skipped!"  << "--->" << frameCounter << endl;
				continue;
			}
			coor3d.push_back(detection);
			tracker.update(detection);
		}
	}

	std::vector<Track> allTracks;
	allTracks.reserve(tracker.tracks.size() + tracker.completeTracks.size());
	allTracks.insert(allTracks.end(), tracker.tracks.begin(), tracker.tracks.end());
	allTracks.insert(allTracks.end(), tracker.completeTracks.begin(), tracker.completeTracks.end());
	saveVecOfVecOfPoints(coor3d, "D:/gopro_stream/11-07-2019/Session2/Contours/coor3d.xml");
	saveVecOfVecOfPoints(allTracks, "D:/gopro_stream/11-07-2019/Session2/Contours/allTracks.xml");
	
	visualisation3d("D:/gopro_stream/11-07-2019/Session2/Contours/coor3d.xml", 1);
	cv::destroyAllWindows();
	return 0;
}
