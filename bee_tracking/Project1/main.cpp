#include <functional>

#include "3dRecording.h"
#include "camCalib.h"
#include "dualCam.h"
#include "disparity.h"
#include "birdsEye.h"
#include "subtractor.h"
#include "sim_detector.h"
#include "sim_coorCalc.h"
#include "KalmanFilter.h"
#include "Hungarian.h"
#include "tracking_main.h"
#include "sim_tracker.h"
#include "findEpiLine.h"
#include "tools.h"
#include "matcher.h"
#include "visualise3d.h"
#include "test.h"
#include "parallelAlign.h"
#include "getChessboard.h"
#include "videoTrim.h"
#include <opencv2/core/utility.hpp>


using namespace cv;
using namespace std;

int main()
{

	//return locate("D:/gopro_stream/11-07-2019/Session2/Left/chessboard_left.mp4");
	//return trim("D:/gopro_stream/11-07-2019/Session2/Right/BGS/GH010017_MOG2_25_dilate5_hist400_Gau5.avi", "D:/gopro_stream/11-07-2019/Session2/Contours/right.avi");

	//return addedMorph();
	//return masking();
	return test();
	//return getChessboard();
	//return camCalib();
	//return findParallel();
	//return imageUndistort();
	//return Subtractor_gpu();
	//return Subtractor_MOG2();
	//return simDetector();
	//return simCoorCalc();
	//return simTracker();
	//return dualCam();
	//return trackingMain(50,20);
	//return visualisation3d("D:/gopro_stream/06-06-2019/Session_2/Contours/allTracks.xml", 0);
	//return visualisation3d("D:/gopro_stream/11-07-2019/Session2/Contours/allTracks.xml", 0);
	//return imgSeqToAVI();


}