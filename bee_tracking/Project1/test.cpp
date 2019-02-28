#include "test.h"

int test()
{
	std::vector<std::vector<std::vector<double>>> coor3d;
	cv::Point3d p1, p2, p1p, p2p;
	std::vector<double> gaps;
	std::vector<int> gapIDs;
	coor3d = vectorReader("F:/renderoutput/two/ball002p004/coor3d.xml");
	for (size_t i = 0; i < coor3d.size(); i++)
	{
		if (i > 0)
		{
			gaps.clear();
			p1p = cv::Point3d(coor3d[i - 1][0][0], coor3d[i - 1][0][1], coor3d[i - 1][0][2]);
			p2p = cv::Point3d(coor3d[i - 1][1][0], coor3d[i - 1][1][1], coor3d[i - 1][1][2]);
			p1 = cv::Point3d(coor3d[i][0][0], coor3d[i][0][1], coor3d[i][0][2]);
			p2 = cv::Point3d(coor3d[i][1][0], coor3d[i][1][1], coor3d[i][1][2]);

			double gap = cv::norm(p1 - p1p);
			gaps.push_back(gap);
			gap = cv::norm(p1 - p2p);
			gaps.push_back(gap);
			gap = cv::norm(p2 - p1p);
			gaps.push_back(gap);
			gap = cv::norm(p2 - p2p);
			gaps.push_back(gap);

			std::sort(gaps.begin(), gaps.end());
			if (gaps[0] > 15 || gaps[1] > 15)
				gapIDs.push_back(i);
			std::cout << i << std::endl;
			std::cout << "First: " << gaps[0] << ". Second: " << gaps[1] << std::endl;
		}
	}
	cv::waitKey(10);
	return 0;
}