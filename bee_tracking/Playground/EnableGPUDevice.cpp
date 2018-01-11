#include "EnableGPUDevice.h"
#include <opencv2\opencv.hpp>
#include <opencv2\core\cuda.hpp>
#include <iostream>

using namespace std;
using namespace cv;
using namespace cv::cuda;

void EnableGPUDevice()
{
	/*Mat src = imread("horizon.bmp", 0);
	if (!src.data) exit(1);
	GpuMat d_src(src);
	GpuMat d_dst;*/

	cout << getCudaEnabledDeviceCount() << endl;
	for (;;)
		;
}