#include "3dRecording.h"
#include "camCalib.h"
#include "dualCam.h"
#include "disparity.h"
#include "birdsEye.h"
#include "sim_subtractor.h"
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

using namespace cv;
using namespace std;

int main()
{
	/*VideoCapture cap(1);
	Mat frame;
	namedWindow("Camera");
	moveWindow("Camera", 10, 10);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
	for (;;)
	{
		cap >> frame;
		imshow("Camera", frame);
		if (waitKey(16) == 27)
			break;
	}
	destroyAllWindows();
	waitKey(200);
	return 0;*/



	/*Mat frame = imread("D:/pic/epipolar_right.tif");
	line(frame, Point(0, 656), Point(1920, 903), Scalar(0, 0, 255), 3);
	line(frame, Point(0, 469), Point(1920, 320), Scalar(0, 255, 255), 3);
	imshow("Epipolar line", frame);
	waitKey(0);
	destroyAllWindows();
	return 0;*/

	/*Mat img = imread("Picture12.bmp", 1);
	namedWindow("output", WINDOW_KEEPRATIO);
	imshow("output", img);
	cout << img.size() << endl;
	waitKey(0);
	destroyAllWindows();
	waitKey(300);
	return 0;*/

	////disparityGen();
	//Ptr<StereoSGBM> stereo = StereoSGBM::create();
	//stereo->setBlockSize(11);				//odd number >= 1
	//stereo->setDisp12MaxDiff(1);
	//stereo->setMinDisparity(-39);
	//stereo->setNumDisparities(112);			//(maxDisp - minDisp). must be divisible by 16
	//stereo->setP1(216);
	//stereo->setP2(864);
	//stereo->setPreFilterCap(63);
	//stereo->setSpeckleRange(32);
	//stereo->setSpeckleWindowSize(100);
	//stereo->setUniquenessRatio(10);			//usually 5-15
	//stereo->setMode(stereo->MODE_SGBM);

	//Mat img_L, img_R, disp, vdisp;

	//img_L = imread("D:/pic/stereo/left.png");
	//img_R = imread("D:/pic/stereo/right.png");

	//cout << "computing disparity map..." << endl;
	//stereo->compute(img_R, img_L, disp);
	//normalize(disp, vdisp, 0, 256, NORM_MINMAX, CV_8U);

	//cout << "now displaying the results..." << endl;
	//imshow("Disparity", vdisp);
	//imshow("image1", img_L);
	//imshow("image2", img_R);
	//while (1)
	//{
	//	if (waitKey(5) == 27)
	//		break;
	//}

	//return birdsEyeView();
	//return camCalib();
	//return simSubtractor();
	//return simDetector();
	//return simCoorCalc();
	//return simTracker();
	//return dualCam();
	//return trackingMain(50,20);
	return visualisation3d("F:/renderoutput/twenty/coor3d.xml", 1);
	//return imgSeqToAVI();
	//return test();

}