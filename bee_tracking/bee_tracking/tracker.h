#pragma once
#include "track.h"
#include <algorithm>

class Tracker
{
public:
	Tracker(int dist_thresh, int max_frame_to_skip, int max_trace_length, int trackIdCount);
	~Tracker();
	void update(std::vector<cv::Point> detections);
	std::vector<Track> tracks;
	
private:
	int dist_thresh_;
	int max_frame_to_skip_;
	int max_trace_length_;
	int trackIdCount_;
};

