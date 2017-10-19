
#include <opencv2\opencv.hpp>
#include "disparity_map.h"

using namespace cv;
using namespace std;

void disparity_map()
{
	Mat imgL = imread("D:/pic/new_L.png", IMREAD_GRAYSCALE);
	Mat imgR = imread("D:/pic/new_R.png", IMREAD_GRAYSCALE);
	Mat disp = Mat(imgL.rows, imgL.cols, CV_16S);
	Mat disp8 = Mat(imgL.rows, imgL.cols, CV_8UC1);

	if (imgL.empty() || imgR.empty())
		cout << "Error reading images!" << endl;

	namedWindow("left image", WINDOW_AUTOSIZE);
	namedWindow("right image", WINDOW_AUTOSIZE);
	namedWindow("disparity map", WINDOW_AUTOSIZE);

	// call the instructor of StereoBM
	int ndisparities = 16 * 6; // Range of disparity
	int SADWindowSize = 21; // Size of the block window. Must be odd

	Ptr<StereoBM> sbm = StereoBM::create(ndisparities, SADWindowSize);
	/*sbm->setBlockSize(9);
	sbm->setDisp12MaxDiff(1);
	sbm->setMinDisparity(-39);
	sbm->setNumDisparities(112);
	sbm->setPreFilterCap(61);
	sbm->setPreFilterSize(5);
	sbm->setSpeckleRange(8);
	sbm->setSpeckleWindowSize(0);
	sbm->setTextureThreshold(507);
	sbm->setUniquenessRatio(0);*/

	// calculate the disparity image
	sbm->compute(imgL, imgR, disp);

	// check the extreme values
	double maxVal;
	double minVal;

	minMaxLoc(disp, &minVal, &maxVal);
	printf("Min disp: %f Max disp: %f", minVal, maxVal);

	//convert disp to 8UC1 type
	disp.convertTo(disp8, CV_8UC1, 255 / (maxVal - minVal));
	imshow("disparity map", disp8);
	//resizeWindow("disparity map", 600, 800);

	//imwrite("SBM_sample_16x6.png", disp8);

}