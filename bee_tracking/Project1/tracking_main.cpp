#include "tracking_main.h"

int trackingMain()
{
	vector<vector<cv::Point3d>> coor3d;
	vector<cv::Point3d> detection;
	cv::VideoCapture capL, capR;
	capL.open("F:/renderoutput/four/ball2468/left.avi");
	capR.open("F:/renderoutput/four/ball2468/right.avi");
	Detectors detectorL, detectorR;


	Tracker tracker(50, 20, 1800, 100);						
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
		}
		frameCounter++;
		cout << frameCounter << endl;
		/*int k = cv::waitKey(2);
		if (k == 27)
			break;*/
	}
	//saveVecOfVecOfPoints(coor3d, "F:/renderoutput/four/ball2468/coor3d.xml");
	//saveVecOfVecOfPoints(tracker.tracks, "F:/renderoutput/four/ball2468/tracks.xml");
	
	visualisation3d(tracker.tracks);
	cv::destroyAllWindows();
	return 0;
}
