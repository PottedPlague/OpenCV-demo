#include "connectedComp.h"
#include <opencv2\opencv.hpp>
#include "xiApiPlusOcv.hpp"
#include <stdio.h>
#include <ctime>

using namespace std;
using namespace cv;

void connectedComp()
{
	xiAPIplusCameraOcv cam;
	Mat frame, label;
	clock_t startTime;
	int framerate;

	int max_lut_index, min_lut_index, max_lut_value, min_lut_value;
	int k = 0;	//frame counter

	printf("Opening first camera...\n");
	cam.OpenFirst();

	max_lut_index = cam.GetLookUpTableIndex_Maximum();
	min_lut_index = cam.GetLookUpTableIndex_Minimum();
	max_lut_value = cam.GetLookUpTableValue_Maximum();
	min_lut_value = cam.GetLookUpTableValue_Minimum();

	cout << "Max LUT index is:" << max_lut_index << endl;
	cout << "Min LUT index is:" << min_lut_index << endl;
	cout << "Max LUT value is:" << max_lut_value << endl;
	cout << "Min LUT value is:" << min_lut_value << endl;

	int operational_point = 200;	//(int)(max_lut_index / 2);
	for (int i = 0; i < max_lut_index; i++)
	{
		cam.SetLookUpTableIndex(i);
		if (i < operational_point)
			cam.SetLookUpTableValue(min_lut_value);
		else
			cam.SetLookUpTableValue(max_lut_value);
	}

	cam.EnableLookUpTable();

	//Set exposure
	cam.SetExposureTime(3000); //3000 us = 3 ms

	printf("Starting acquisition...\n");
	cam.StartAcquisition();

	namedWindow("Output", WINDOW_AUTOSIZE);
	startTime = clock();

	for (;;)
	{
		frame = cam.GetNextImageOcvMat();

		//compute the connected components
		connectedComponents(frame, label, 8, CV_16U);
		normalize(label, label, 0, 255, NORM_MINMAX, CV_8U);


		framerate = CLOCKS_PER_SEC / (clock() - startTime);
		startTime = clock();
		cout << "Current framerate: " << framerate << endl;

		imshow("Output", label);
		if (waitKey(1) == 27)
			break;
		frame.release();
	}

	cam.StopAcquisition();
	cam.Close();
	cout << "Acquisition ended." << endl;

}