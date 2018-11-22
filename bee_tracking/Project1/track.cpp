#include "track.h"

Track::Track(cv::Point prediction, int track_id)
{
	track_id_ = track_id;
	prediction_ = prediction;
	kalman.create(prediction_);
	skipped_frames = 0;
	real = 0;
}

Track::~Track()
{
}