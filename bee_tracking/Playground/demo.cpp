
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;
using namespace std;

int main()
{
	namedWindow("Example", WINDOW_AUTOSIZE);
	VideoCapture cap;
	cap.open("D://pic//example.mp4");
	Mat frame;
	for (;;)
	{
		cap >> frame;
		if (frame.empty())
			break;
		imshow("Example", frame);
		if (waitKey(16) >= 0)
			break;
	}
	return 0;
}