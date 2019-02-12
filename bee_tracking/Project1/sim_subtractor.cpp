#include "sim_subtractor.h"
#include <opencv2\opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <opencv2\bgsegm.hpp>

using namespace std;
using namespace cv;

int simSubtractor()
{
	int strat = 1;		//0 - avi, 1 - image sequence
	if (strat == 0)
	{
		VideoCapture capL, capR;
		VideoWriter outL, outR;
		Ptr<bgsegm::BackgroundSubtractorMOG> pMOG = bgsegm::createBackgroundSubtractorMOG();

		capL.open("F:/renderoutput/multi/left_long.avi");
		capR.open("F:/renderoutput/multi/right_long.avi");
		outL.open("F:/renderoutput/multi/morph/left_long.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, Size((int)capL.get(CAP_PROP_FRAME_WIDTH), (int)capL.get(CAP_PROP_FRAME_HEIGHT)));
		outR.open("F:/renderoutput/multi/morph/right_long.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, Size((int)capR.get(CAP_PROP_FRAME_WIDTH), (int)capR.get(CAP_PROP_FRAME_HEIGHT)));

		pMOG->setHistory(200);
		pMOG->setNMixtures(5);
		pMOG->setBackgroundRatio(0.85);

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
	}
	else
	{
		String path("F:/renderoutput/multi/two_obj/left/*.tif");
		vector<String> fn;
		Mat frame, fgmask, output;
		VideoWriter out;
		Ptr<bgsegm::BackgroundSubtractorMOG> pMOG = bgsegm::createBackgroundSubtractorMOG();
		out.open("F:/renderoutput/multi/morph/new/test.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, Size(1920, 1080));
		
		pMOG->setHistory(200);
		pMOG->setNMixtures(5);
		pMOG->setBackgroundRatio(0.9); 
		Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));

		glob(path, fn, true);
		for (size_t k = 0; k < 400; k++)
		{
			cout << k << endl;
			frame = imread(fn[k]);
			if (frame.empty())
				return -1;
			pMOG->apply(frame, fgmask);
			morphologyEx(fgmask, fgmask, MORPH_OPEN, kernel);
			cvtColor(fgmask, output, CV_GRAY2BGR);
			out.write(output);
		}
		out.release();
	}
	waitKey(200);
	return 0;
}
