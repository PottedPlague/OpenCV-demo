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

KF Track::callKalman()
{
	return kalman_;
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

	for (int frameCount = 0; frameCount < detections_.size(); frameCount++)
	{
		if (tracks.size() == 0 && detections_[frameCount].size() != 0)
			for (int i = 0; i < detections_[frameCount].size(); i++)
			{
				countID_++;															//ID starts from 1, not 0
				tracks.push_back(Track(countID_, detections_[0][i]));
			}
		
		int N = tracks.size();
		int M = detections_[frameCount].size();
		for (int i = 0; i < N; i++)
			tracks[i].callKalman().predict();

		cv::Mat cost = cv::Mat::zeros(cv::Size(N, M), CV_32F);
	}

	// calculate cost matrix

}
