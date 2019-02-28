#include "matcher.h"


std::vector<cv::Point3d> doMatch(std::vector<cv::Point> left, std::vector<cv::Point> right)
{
	std::vector<cv::Point3d> points3D;
	//vector<cv::Point> epiline;
	EpiGeo EpipolarGeo;
	//epiline = EpipolarGeo.findEpipolarLine(pnt);
	HungarianAlgorithm hungAlgo;
	vector<vector<double>> CorresMatrix;
	vector<vector<Track*>> matchedPair;
	double corresthresh = 0.003;
	double targetDis = 3000;
	int imageWidth = EpipolarGeo.getImageWidth();
	int imageHeight = EpipolarGeo.getImageHeight();
	double scalar = EpipolarGeo.getAlpha();
	scalar = 1 / scalar;
	double focal = EpipolarGeo.getFocalLength();

	for (size_t i = 0; i < left.size(); i++)
	{
		vector<double> rowVec;
		for (size_t j = 0; j < right.size(); j++)
		{
			double corres = EpipolarGeo.pntCorrespondence(left[i], right[j]);
			rowVec.push_back(abs(corres));
		}
		CorresMatrix.push_back(rowVec);
	}

	vector<int> assignment;
	double cost = hungAlgo.Solve(CorresMatrix, assignment);

	for (int i = 0; i < assignment.size(); i++)
	{
		if (assignment[i] != -1)
		{
			if (CorresMatrix[i][assignment[i]] > corresthresh)
			{
				assignment[i] = -1;
			}
		}
	}

	for (size_t i = 0; i < assignment.size(); i++)
	{
		if (i < right.size())
		{
			if (assignment[i] != -1)
			{
				left[i].x = -left[i].x + imageWidth / 2;
				left[i].y = -left[i].y + imageHeight / 2;
				right[assignment[i]].x = right[assignment[i]].x - imageWidth / 2;
				right[assignment[i]].y = -right[assignment[i]].y + imageHeight / 2;
				double x = (targetDis*scalar*right[assignment[i]].x*(focal + scalar * left[i].x)) / (focal*focal - scalar * scalar*left[i].x*right[assignment[i]].x);
				double y = (targetDis*scalar*left[i].x*(focal + scalar * right[assignment[i]].x)) / (focal*focal - scalar * scalar*left[i].x*right[assignment[i]].x);
				double z = (targetDis*scalar*left[i].y*(focal + scalar * right[assignment[i]].x)) / (focal*focal - scalar * scalar*left[i].x*right[assignment[i]].x);
				points3D.push_back(cv::Point3d(x, y, z));
			}
		}
		else
			break;
	}
	return points3D;
}