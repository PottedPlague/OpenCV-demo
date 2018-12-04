#include "tools.h"

int imgSeqToAVI()
{
	cv::String path("F:/renderoutput/multi/img_seq/right/*.tif");
	std::vector<cv::String> fn;
	cv::glob(path, fn, true);
	cv::VideoWriter out;
	out.open("F:/renderoutput/multi/right_long.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, cv::Size(1280, 720));
	for (size_t k = 0; k < fn.size(); ++k)
	{
		cv::Mat img = cv::imread(fn[k]);
		if (img.empty())
			continue;
		out.write(img);
	}
	out.release();
	return 0;
}
