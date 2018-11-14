#include "3dRecording.h"
#include "camCalib.h"
#include "dualCam.h"
#include "disparity.h"
#include "birdsEye.h"
#include "sim_subtractor.h"
#include "sim_detector.h"
#include "sim_coorCalc.h"

int main()
{
	//Mat img = imread("Picture12.bmp", 1);
	//namedWindow("output", WINDOW_KEEPRATIO);
	//imshow("output", img);
	//cout << img.size() << endl;
	//waitKey(0);
	//destroyAllWindows();
	//waitKey(300);
	//return 0;

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
	return simCoorCalc();
}