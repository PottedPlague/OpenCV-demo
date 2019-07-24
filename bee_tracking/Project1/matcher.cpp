#include "matcher.h"
#define DEBUG_MODE 0

std::vector<cv::Point3d> doMatch(std::vector<cv::Point> left, std::vector<cv::Point> right, cv::Mat lScene, cv::Mat rScene)
{
	std::vector<cv::Point3d> points3D;
	//vector<cv::Point> epiline;
	EpiGeo EpipolarGeo;
	//epiline = EpipolarGeo.findEpipolarLine(pnt);
	HungarianAlgorithm hungAlgo;
	vector<vector<double>> CorresMatrix;
	vector<vector<Track*>> matchedPair;
	double corresthresh = 0.01;
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

	///////////////////////////////////////////////begin
#if DEBUG_MODE == 1
	for (int i = 0; i < left.size(); i++)
		cv::circle(lScene, left[i], 4, cv::Scalar(0, 0, 255), 2);
	for (int i = 0; i < right.size(); i++)
		cv::circle(rScene, right[i], 4, cv::Scalar(0, 0, 255), 2);
#endif
	///////////////////////////////////////////////end

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

		if (assignment[i] != -1)
		{
			///////////////////////////////////////////////begin
#if DEBUG_MODE == 1
			cv::circle(lScene, left[i], 4, cv::Scalar(0, 255, 0), 2);
			cv::circle(rScene, right[assignment[i]], 4, cv::Scalar(0, 255, 0), 2);
			cv::line(rScene, EpipolarGeo.findEpipolarLine(left[i])[0], EpipolarGeo.findEpipolarLine(left[i])[1], cv::Scalar(255, 0, 0), 2);
#endif
			///////////////////////////////////////////////end

			left[i].x = -left[i].x + imageWidth / 2;
			left[i].y = -left[i].y + imageHeight / 2;
			right[assignment[i]].x = right[assignment[i]].x - imageWidth / 2;
			right[assignment[i]].y = -right[assignment[i]].y + imageHeight / 2;
			double x = (targetDis*scalar*right[assignment[i]].x*(focal + scalar * left[i].x)) / (focal*focal - scalar * scalar*left[i].x*right[assignment[i]].x);
			double y = (targetDis*scalar*left[i].x*(focal + scalar * right[assignment[i]].x)) / (focal*focal - scalar * scalar*left[i].x*right[assignment[i]].x);
			double z = (targetDis*scalar*left[i].y*(focal + scalar * right[assignment[i]].x)) / (focal*focal - scalar * scalar*left[i].x*right[assignment[i]].x);
			/*if (x > 5000 && y > 5000 && z > 5000)
				continue;*/
			points3D.push_back(cv::Point3d(x, y, z));
		}
	}

	///////////////////////////////////////////////begin
#if DEBUG_MODE == 1
	cv::resize(lScene, lScene, cv::Size(1920, 1080));
	cv::resize(rScene, rScene, cv::Size(1920, 1080));
	cv::imshow("Left Scene", lScene);
	cv::imshow("Right Scene", rScene);
	cv::waitKey(0);
#endif
	///////////////////////////////////////////////end

	return points3D;
}