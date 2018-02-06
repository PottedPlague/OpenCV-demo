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

Tracker::Tracker(int maxFramesToSkip, double distThresh, std::vector<std::vector<cv::Point2d>> centroids)
{
	maxFramesToSkip_ = maxFramesToSkip;
	distThresh_ = distThresh;
	detections_ = centroids;
	countID_ = 0;
}

Tracker::~Tracker()
{
}

void Tracker::solve()
{
	// create new tracks at the beginning besed on detection results
	if (tracks_.size() < detections_.size())
	{
		size_t lack = detections_.size() - tracks_.size();
		for (; countID_ < lack; countID_++)
		{
			Track track = Track(countID_, detections_[0][countID_]);
			tracks_.push_back(track);
		}
	}


	// calculate cost matrix

}
