#include "xiApiPlusOcv.hpp"
#include <opencv2\opencv.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;

const int OP_POINT = 200;	//operational point(threshold level), usually half of max_lut_index;

void adjustLUT()
{
	xiAPIplusCameraOcv cam;
	Mat frame;

	int max_lut_index, min_lut_index;
	int max_lut_value, min_lut_value;

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

	for (int i = 0; i < max_lut_index; i++)
	{
		cam.SetLookUpTableIndex(i);
		if (i < OP_POINT)
			cam.SetLookUpTableValue(min_lut_value);
		else
			cam.SetLookUpTableValue(max_lut_value);
	}

	cam.EnableLookUpTable();

	//Set exposure
	cam.SetExposureTime(3000); //3000 us = 3 ms

	printf("Starting acquisition...\n");
	cam.StartAcquisition();

	namedWindow("Camera Feed", WINDOW_AUTOSIZE);
	for (;;)
	{
		frame = cam.GetNextImageOcvMat();
		imshow("Camera Feed", frame);

		if (waitKey(1) == 27)
			break;
	}

	waitKey(500);
}