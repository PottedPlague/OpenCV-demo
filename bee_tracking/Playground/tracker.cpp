#include "tracker.h"

using namespace beeproject;

Track::Track(int ID, cv::Point2d initPt)
{
	trackID_ = ID;
	coorPre_ = initPt;
	skippedFrames_ = 0;
	kalman_.create(initPt);
}

Track::~Track()
{
}

void Track::makePrediction()
{
	coorPre_ = kalman_.predict();
}

void Track::updateKalman(cv::Point2d measurement)
{
	kalman_.update(measurement, 1);
}

cv::Point2d Track::getPrediction()
{
	return coorPre_;
}

Tracker::Tracker(int maxFramesToSkip, double distThresh, int maxTraceLength, int trackIDCount)
{
	maxFramesToSkip_ = maxFramesToSkip;
	distThresh_ = distThresh;
	maxTraceLength_ = maxTraceLength;
	countID_ = trackIDCount;
}

Tracker::~Tracker()
{
}

void Tracker::solve(std::vector<std::vector<cv::Point2d>> detections) 
{
	std::vector<Track> tracks;
	detections_ = detections;
	cv::Mat drawingboard = cv::Mat::zeros(cv::Size(800, 800), CV_8UC3);

	for (int frameCount = 5; frameCount < detections_.size(); frameCount++)			//frame starts from 5
	{
		std::vector<int> assignment;
		if (tracks.size() == 0 && detections_[frameCount].size() != 0)
			for (int i = 0; i < detections_[frameCount].size(); i++)
			{
				tracks.push_back(Track(countID_, detections_[frameCount][i]));
				countID_++;															//ID starts from 0
			}

		int N = tracks.size();														//N - predictions; M - detections
		for (int i = 0; i < N; i++)
			assignment.push_back(-1);
		int M = detections_[frameCount].size();
		cv::Mat cost = cv::Mat::zeros(cv::Size(N, M), CV_32F);						//cost matrix
		
		for (int p = 0; p < N; p++)
		{
			tracks[p].makePrediction();												//make prediction of each track in this individual frame
			for (int q = 0; q < M; q++)
			{
				cost.at<float>(p, q) = cv::norm(tracks[p].getPrediction() - detections_[frameCount][q]);
			}
		}

		//start assignment
		for (int p = 0; p < N; p++)
		{
			double min = std::numeric_limits<double>::max();
			for (int q = 0; q < M; q++)
			{
				if (cost.at<float>(p, q) < min)
				{
					min = cost.at<float>(p, q);
					assignment[p] = q;
				}	
			}
		}
			
		for (int i = 0; i < N; i++)
			tracks[i].updateKalman(detections_[frameCount][assignment[i]]);

		cv::circle(drawingboard, tracks[0].getPrediction(), 2, cv::Scalar(0, 255, 0), -1);
		cv::circle(drawingboard, detections_[frameCount][assignment[0]], 2, cv::Scalar(0, 130, 0), -1);
		cv::circle(drawingboard, tracks[1].getPrediction(), 2, cv::Scalar(0, 0, 255), -1);
		cv::circle(drawingboard, detections_[frameCount][assignment[1]], 2, cv::Scalar(0, 0, 130), -1);
		cv::circle(drawingboard, tracks[2].getPrediction(), 2, cv::Scalar(255, 0, 0), -1);
		cv::circle(drawingboard, detections_[frameCount][assignment[2]], 2, cv::Scalar(130, 0, 0), -1);
		
		cv::imshow("Output", drawingboard);
		cv::waitKey(30);
	}

	// calculate cost matrix

}
