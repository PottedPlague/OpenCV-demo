#include "stdafx.h" 

#ifdef WIN32
#include "xiApi.h"       // Windows
#else
#include <m3api/xiApi.h> // Linux, OSX
#endif
#include <memory.h>

#define HandleResult(res,place) if (res!=XI_OK) {printf("Error after %s (%d)\n",place,res);goto finish;}

int _tmain(int argc, _TCHAR* argv[])
{
	// image buffer
	XI_IMG image;
	memset(&image, 0, sizeof(image));
	image.size = sizeof(XI_IMG);

	HANDLE xiH = NULL;
	XI_RETURN stat = XI_OK;

	// Retrieving a handle to the camera device 
	printf("Opening first camera...\n");
	stat = xiOpenDevice(0, &xiH);
	HandleResult(stat, "xiOpenDevice");

	int maxIndex, minIndex;
	int maxValue, minValue;

	stat = xiGetParamInt(xiH, XI_PRM_LUT_INDEX XI_PRM_INFO_MIN, &minIndex);
	HandleResult(stat, "xiGetParam 1");
	stat = xiGetParamInt(xiH, XI_PRM_LUT_INDEX XI_PRM_INFO_MAX, &maxIndex);
	HandleResult(stat, "xiGetParam 2");
	stat = xiGetParamInt(xiH, XI_PRM_LUT_VALUE XI_PRM_INFO_MIN, &minValue);
	HandleResult(stat, "xiGetParam 3");
	stat = xiGetParamInt(xiH, XI_PRM_LUT_VALUE XI_PRM_INFO_MAX, &maxValue);
	HandleResult(stat, "xiGetParam 4");

	// LUT is 10bit as sensor bit depth
	printf("max index %d \n", maxIndex);
	printf("max value %d \n", maxValue);								// max of 10bits
	

	int operational_point = (int)(maxIndex/2);							// threshold from value grey value of 128 (8 bit) resp. 512 (10 bit)
	for (int i = 0; i < maxIndex; i++)
	{
		stat = xiSetParamInt(xiH, XI_PRM_LUT_INDEX, i);
		HandleResult(stat, "xiSetParam");
		if (i < operational_point)
			stat = xiSetParamInt(xiH, XI_PRM_LUT_VALUE, minValue);		// minValue - black
		else stat = xiSetParamInt(xiH, XI_PRM_LUT_VALUE, maxValue);		// maxValue - white
		HandleResult(stat, "xiSetParam");
	}

	/*
	// invert image
	for (int i = 0; i <= maxIndex; i++){
		stat = xiSetParamInt(xiH, XI_PRM_LUT_INDEX, i);
		HandleResult(stat, "xiSetParam");
		stat = xiSetParamInt(xiH, XI_PRM_LUT_VALUE, (maxValue - i));
		HandleResult(stat, "xiSetParam");
		printf("i=%d\tLUT value=%d\n", i, maxValue - i);
	}
	*/

	stat = xiSetParamInt(xiH, XI_PRM_LUT_EN, 1);
	HandleResult(stat, "xiSetParamLUT");

	// Setting "exposure" parameter (10ms=10000us)
	stat = xiSetParamInt(xiH, XI_PRM_EXPOSURE, 2000);
	HandleResult(stat, "xiSetParam (exposure set)");

	// Note:
	// The default parameters of each camera might be different in different API versions
	// In order to ensure that your application will have camera in expected state,
	// please set all parameters expected by your application to required value.

	printf("Starting acquisition...\n");
	stat = xiStartAcquisition(xiH);
	HandleResult(stat, "xiStartAcquisition");

#define EXPECTED_IMAGES 5000
	for (int images = 0; images < EXPECTED_IMAGES; images++)
	{
		// getting image from camera
		stat = xiGetImage(xiH, 5000, &image);
		HandleResult(stat, "xiGetImage");
		unsigned char pixel = *(unsigned char*)image.bp;
		printf("Image %d (%dx%d) received from camera. First pixel value: %d\n", images, (int)image.width, (int)image.height, pixel);
	}

	printf("Stopping acquisition...\n");
	xiStopAcquisition(xiH);
	xiCloseDevice(xiH);
finish:
	printf("Done\n");

	{
		printf("Press enter\n");
		char s[100];
		fgets(s, sizeof(s), stdin);
	}
	return 0;
}