#include "sim_subtractor.h"
#include <opencv2\opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <opencv2\bgsegm.hpp>

using namespace std;
using namespace cv;

int simSubtractor()
{
	VideoCapture capL, capR;
	VideoWriter outL, outR;
	Ptr<bgsegm::BackgroundSubtractorMOG> pMOG = bgsegm::createBackgroundSubtractorMOG();

	int sce = 1;						// 0 - cube, 1 - helical line
	if (sce == 0)
	{
		capL.open("F:/renderoutput/cube/left_uc.avi");
		capR.open("F:/renderoutput/cube/right_uc.avi");

		outL.open("F:/renderoutput/cube/morph/left.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, Size((int)capL.get(CAP_PROP_FRAME_WIDTH), (int)capL.get(CAP_PROP_FRAME_HEIGHT)));
		outR.open("F:/renderoutput/cube/morph/right.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, Size((int)capR.get(CAP_PROP_FRAME_WIDTH), (int)capR.get(CAP_PROP_FRAME_HEIGHT)));

		pMOG->setHistory(200);
		pMOG->setNMixtures(5);
		pMOG->setBackgroundRatio(0.85);
	}

	if (sce == 1)
	{
		capL.open("F:/renderoutput/multi/left_long.avi");
		capR.open("F:/renderoutput/multi/right_long.avi");

		outL.open("F:/renderoutput/multi/morph/left_long.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, Size((int)capL.get(CAP_PROP_FRAME_WIDTH), (int)capL.get(CAP_PROP_FRAME_HEIGHT)));
		outR.open("F:/renderoutput/multi/morph/right_long.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, Size((int)capR.get(CAP_PROP_FRAME_WIDTH), (int)capR.get(CAP_PROP_FRAME_HEIGHT)));

		pMOG->setHistory(200);
		pMOG->setNMixtures(5);
		pMOG->setBackgroundRatio(0.85);
	}
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	Mat frameL, frameR, fgmaskL, fgmaskR, outputL, outputR;

	for (;;)
	{
		capL >> frameL;
		cout << frameL.size() << endl;
		if (frameL.empty())
			break;

		pMOG->apply(frameL, fgmaskL);
		morphologyEx(fgmaskL, fgmaskL, MORPH_OPEN, kernel);
		cvtColor(fgmaskL, outputL, CV_GRAY2BGR);
		outL.write(outputL);

		capR >> frameR;
		pMOG->apply(frameR, fgmaskR);
		morphologyEx(fgmaskR, fgmaskR, MORPH_OPEN, kernel);
		cvtColor(fgmaskR, outputR, CV_GRAY2BGR);
		outR.write(outputR);

	}
	//destroyAllWindows();
	outL.release();
	outR.release();

	return 0;
}
