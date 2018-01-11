#include "OLcontours.h"
#include "RTcontours.h"
#include "connectedComp.h"
#include "xiApiPlusOcv.hpp"

using namespace std;
using namespace cv;

int main()
{
	vector<Mat> frame;
	xiAPIplusCameraOcv cam;

	cam.OpenFirst();

	//cam.SetTriggerDelay(1000);	// not supported by xiQ model
	cam.SetExposureTime(20);

	cam.StartAcquisition();

	frame.push_back(cam.GetNextImageOcvMat());
	frame.push_back(cam.GetNextImageOcvMat());
	frame.push_back(cam.GetNextImageOcvMat());

	imshow("Output", frame[0]);
	waitKey(0);

	cam.StopAcquisition();
	cam.Close();

	waitKey(500);

	return 0;
}