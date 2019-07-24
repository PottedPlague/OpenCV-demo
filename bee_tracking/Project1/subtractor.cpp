#include "subtractor.h"
#include <opencv2\opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/bgsegm.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudacodec.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;


int Subtractor()
{
	int strat = 0;		//0 - avi, 1 - image sequence
	if (strat == 0)
	{
		VideoCapture capL, capR;
		VideoWriter outL, outR;
		int k = 1, percent, totalFrames;
		Ptr<bgsegm::BackgroundSubtractorMOG> pMOG = bgsegm::createBackgroundSubtractorMOG();

		capL.open("D:/gopro_stream/06-06-2019/Session_2/Left/GH010010_select.avi");
		//capR.open("F:/renderoutput/multi/right_long.avi");
		outL.open("D:/gopro_stream/06-06-2019/Session_2/Left/BGS/GH010010_MOG_.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 60, Size((int)capL.get(CAP_PROP_FRAME_WIDTH), (int)capL.get(CAP_PROP_FRAME_HEIGHT)));
		//outR.open("F:/renderoutput/multi/morph/right_long.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 60, Size((int)capR.get(CAP_PROP_FRAME_WIDTH), (int)capR.get(CAP_PROP_FRAME_HEIGHT)));
		totalFrames = capL.get(cv::CAP_PROP_FRAME_COUNT);

		pMOG->setHistory(200);
		pMOG->setNMixtures(5);
		pMOG->setBackgroundRatio(0.85);

		Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
		Mat frameL, frameR, fgmaskL, fgmaskR, outputL, outputR;

		for (;;)
		{
			capL >> frameL;
			//cout << frameL.size() << endl;
			if (frameL.empty())
				break;

			pMOG->apply(frameL, fgmaskL);
			morphologyEx(fgmaskL, fgmaskL, MORPH_OPEN, kernel);
			cvtColor(fgmaskL, outputL, COLOR_GRAY2BGR);
			outL.write(outputL);

			/*capR >> frameR;
			pMOG->apply(frameR, fgmaskR);
			morphologyEx(fgmaskR, fgmaskR, MORPH_OPEN, kernel);
			cvtColor(fgmaskR, outputR, COLOR_GRAY2BGR);
			outR.write(outputR);*/
			percent = k * 100 / totalFrames;
			std::cout << "Processing..." << percent  << "%" << endl;
			k++;
		}
		//destroyAllWindows();
		outL.release();
		//outR.release();
	}
	else
	{
		String path("F:/renderoutput/multi/two_obj/left/*.tif");
		vector<String> fn;
		Mat frame, fgmask, output;
		VideoWriter out;
		Ptr<cuda::BackgroundSubtractorMOG> pMOG = cuda::createBackgroundSubtractorMOG();
		out.open("F:/renderoutput/multi/morph/new/test.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(1920, 1080));
		
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
			cvtColor(fgmask, output, COLOR_GRAY2BGR);
			out.write(output);
		}
		out.release();
	}
	waitKey(200);
	return 0;
}

int Subtractor_gpu()
{
	/*const string fname = "D:/gopro_stream/06-06-2019/Session_2/Left/GH010010_select.avi";
	cuda::GpuMat d_frame;
	Ptr<cudacodec::VideoReader> d_reader = cudacodec::createVideoReader(fname);

	for (;;)
	{
		if (!d_reader->nextFrame(d_frame))
			break;

		Mat frame;
		d_frame.download(frame);
		imshow("GPU", frame);

		if (waitKey(16) > 0)
			break;

	}*/

	VideoCapture cap;
	VideoWriter out;
	int k = 1, percent, totalFrames;
	Ptr<cuda::BackgroundSubtractorMOG> pMOG = cuda::createBackgroundSubtractorMOG();
	string fname = "D:/gopro_stream/06-06-2019/Session_2/Right/GX010011_select.avi";
	cap.open(fname);
	out.open("D:/gopro_stream/06-06-2019/Session_2/Right/BGS/GX010011_MOG_03_close5_hist20.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 60, Size((int)cap.get(CAP_PROP_FRAME_WIDTH), (int)cap.get(CAP_PROP_FRAME_HEIGHT)));
	totalFrames = cap.get(cv::CAP_PROP_FRAME_COUNT);

	pMOG->setHistory(20);
	pMOG->setNMixtures(5);
	pMOG->setBackgroundRatio(0.3);

	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	Ptr<cuda::Filter> pMorph = cuda::createMorphologyFilter(MORPH_CLOSE, CV_8UC1, kernel);
	Mat frame, fgmask, output;
	cuda::GpuMat frame_g, fgmask_g, output_g;

	for (;;)
	{
		cap >> frame;
		//cout << frame.size() << endl;
		if (frame.empty())
			break;

		frame_g.upload(frame);
		pMOG->apply(frame_g, fgmask_g);
		pMorph->apply(fgmask_g, fgmask_g);
		cuda::cvtColor(fgmask_g, output_g, COLOR_GRAY2BGR);
		output_g.download(output);
		out.write(output);

		percent = k * 100 / totalFrames;
		std::cout << "Processing..." << percent << "%" << endl;
		k++;
	}
	out.release();
	waitKey(200);
	return 0;
}

int Subtractor_MOG2()
{
	VideoCapture cap;
	VideoWriter out;
	int k = 1, percent, totalFrames;
	Ptr<cuda::BackgroundSubtractorMOG2> pMOG = cuda::createBackgroundSubtractorMOG2(400, 25, false);
	string fname = "D:/gopro_stream/11-07-2019/Session2/Right/GX010017_synched.mp4";
	cap.open(fname);
	out.open("D:/gopro_stream/11-07-2019/Session2/Right/BGS/GH010017_MOG2_25_dilate5_hist400_Gau5.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 60, Size((int)cap.get(CAP_PROP_FRAME_WIDTH), (int)cap.get(CAP_PROP_FRAME_HEIGHT)));
	totalFrames = cap.get(cv::CAP_PROP_FRAME_COUNT);

	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	Ptr<cuda::Filter> pMorph = cuda::createMorphologyFilter(MORPH_DILATE, CV_8UC1, kernel);
	Ptr<cuda::Filter> pGaussian = cuda::createGaussianFilter(CV_8UC3, CV_8UC1, Size(5, 5), 0);
	Mat frame, fgmask, output;
	cuda::GpuMat frame_g, fgmask_g, output_g;

	for (;;)
	{
		cap >> frame;
		//cout << frame.size() << endl;
		if (frame.empty())
			break;

		frame_g.upload(frame);
		
		pGaussian->apply(frame_g, frame_g);
		pMOG->apply(frame_g, fgmask_g);
		pMorph->apply(fgmask_g, fgmask_g);
		cuda::cvtColor(fgmask_g, output_g, COLOR_GRAY2BGR);
		output_g.download(output);
		out.write(output);

		percent = k * 100 / totalFrames;
		std::cout << "Processing..." << percent << "%" << endl;
		k++;
	}
	out.release();

	waitKey(200);
	return 0;
}
