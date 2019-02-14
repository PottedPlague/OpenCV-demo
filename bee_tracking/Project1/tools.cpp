#include "tools.h"

int imgSeqToAVI()
{
	cv::String path("F:/renderoutput/ball20/right/*.tif");
	std::vector<cv::String> fn;
	cv::glob(path, fn, true);
	cv::VideoWriter out;
	out.open("F:/renderoutput/ball20/video/right20.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, cv::Size(1920, 1080));
	for (size_t k = 0; k < fn.size(); ++k)
	{
		cv::Mat img = cv::imread(fn[k]);
		if (img.empty())
			continue;
		out.write(img);
		int perc = 100 * k / fn.size();
		std::cout << "Processing..." << perc << "%" << std::endl;
	}
	out.release();
	return 0;
}

std::string intToStr(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}