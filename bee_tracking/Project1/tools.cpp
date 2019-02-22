#include "tools.h"

int imgSeqToAVI()
{
	cv::String path("F:/renderoutput/four/ball2468/left/*.tif");
	std::vector<cv::String> fn;
	cv::glob(path, fn, true);
	cv::VideoWriter out;
	out.open("F:/renderoutput/four/ball2468/left.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, cv::Size(1920, 1080));
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

int saveVecOfVecOfPoints(std::vector<Track> tracks, cv::String filename)
{
	cv::FileStorage fs(filename, cv::FileStorage::WRITE);
	fs << "data" << "[";
	for (int i = 0; i < tracks.size(); i++)
	{
		fs << "[:";
		for (int j = 0; j < tracks[i].trace.size(); j++)
		{
			fs << "[:" << tracks[i].trace[j].x << tracks[i].trace[j].y << tracks[i].trace[j].z << "]";
		}
		fs << "]";
	}
	fs << "]";
	fs.release();
	return 0;
}

int saveVecOfVecOfPoints(std::vector<std::vector<cv::Point3d>> pnt3d, cv::String filename)
{
	cv::FileStorage fs(filename, cv::FileStorage::WRITE);
	fs << "data" << "[";
	for (int i = 0; i < pnt3d.size(); i++)
	{
		fs << "[:";
		for (int j = 0; j < pnt3d[i].size(); j++)
		{
			fs << "[:" << pnt3d[i][j].x << pnt3d[i][j].y << pnt3d[i][j].z << "]";
		}
		fs << "]";
	}
	fs << "]";
	fs.release();
	return 0;
}

std::vector<std::vector<std::vector<double>>> vectorReader(cv::String filename)
{
	std::vector<std::vector<std::vector<double>>> tracks;

	cv::FileStorage fs(filename, cv::FileStorage::READ);
	cv::FileNode data = fs["data"];
	for (cv::FileNodeIterator itData = data.begin(); itData != data.end(); ++itData)
	{
		std::vector<std::vector<double>> track;
		cv::FileNode pts = *itData;
		for (cv::FileNodeIterator itPts = pts.begin(); itPts != pts.end(); ++itPts)
		{
			cv::FileNode pt = *itPts;
			cv::FileNodeIterator itPt = pt.begin();
			cv::Point3d point;
			point.x = *itPt;
			++itPt;
			point.y = *itPt;
			++itPt;
			point.z = *itPt;
			std::vector<double> pnt = {
				point.x,
				point.y,
				point.z };
			track.push_back(pnt);
			/*std::vector<double> point;
			double value = *itPt;
			++itPt;
			point.push_back(value);
			value = *itPt;
			++itPt;
			point.push_back(value);
			value = *itPt;
			point.push_back(value);
			track.push_back(point);*/
		}
		tracks.push_back(track);
	}
	
	return tracks;
}
