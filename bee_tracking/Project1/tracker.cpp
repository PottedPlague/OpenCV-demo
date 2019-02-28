#include "tracker.h"
#include "Hungarian.h"


Tracker::Tracker(int dist_thresh, int max_frame_to_skip, int max_trace_length, int trackIdCount)
{
	dist_thresh_ = dist_thresh;
	max_frame_to_skip_ = max_frame_to_skip;
	max_trace_length_ = max_trace_length;
	trackIdCount_ = trackIdCount;
}

Tracker::~Tracker()
{
}

void Tracker::update(vector<cv::Point3d> detections)
{
	if (tracks.size() == 0)
	{
		for (int i = 0; i < detections.size(); i++)
		{
			Track track = Track(detections[i], trackIdCount_);
			trackIdCount_++;
			tracks.push_back(track);
		}
	}

	int N = tracks.size();
	int M = detections.size();
	vector<vector<double>> costMatrix;
	for (int i = 0; i < N; i++)
	{
		vector<double> rowCost;
		for (int j = 0; j < M; j++)
		{
			double diff = cv::norm(tracks[i].prediction_ - detections[j]);
			rowCost.push_back(diff);
		}
		costMatrix.push_back(rowCost);
	}

	HungarianAlgorithm HungAlgo;
	vector<int> assignment;
	double cost = HungAlgo.Solve(costMatrix, assignment);

	vector<int> unassigned_tracks;
	for (int i = 0; i < assignment.size(); i++)
	{
		if (assignment[i] != -1)
		{
			if (costMatrix[i][assignment[i]] > dist_thresh_)
			{
				assignment[i] = -1;
				unassigned_tracks.push_back(i);
			}
		}
		else
		{
			tracks[i].skipped_frames++;
		}
	}

	vector<int> del_tracks;
	for (int i = 0; i < tracks.size(); i++)
	{
		if (tracks[i].skipped_frames > max_frame_to_skip_)
			del_tracks.push_back(i);
	}
	if (del_tracks.size() > 0)
	{
		for (int i = del_tracks.size() - 1; i >= 0; i--)
		{
			if (del_tracks[i] < tracks.size())
			{
				completeTracks.push_back(tracks[del_tracks[i]]);
				tracks.erase(tracks.begin() + del_tracks[i]);
				assignment.erase(assignment.begin() + del_tracks[i]);
			}
			else
			{
				printf("ERROR: id is greater than size of tracks.");
			}
		}
	}

	vector<int> unassigned_detects;
	for (int i = 0; i < detections.size(); i++)
	{
		unassigned_detects.push_back(i);
	}
	for (int i = 0; i < assignment.size(); i++)
	{
		unassigned_detects.erase(remove(unassigned_detects.begin(), unassigned_detects.end(), assignment[i]), unassigned_detects.end());
	}

	if (unassigned_detects.size() != 0)
	{
		for (int i = 0; i < unassigned_detects.size(); i++)
		{
			Track track = Track(detections[unassigned_detects[i]], trackIdCount_);
			trackIdCount_++;
			tracks.push_back(track);
		}
	}

	for (int i = 0; i < assignment.size(); i++)
	{
		tracks[i].kalman.predict();

		if (assignment[i] != -1)
		{
			tracks[i].skipped_frames = 0;
			tracks[i].prediction_ = tracks[i].kalman.update(detections[assignment[i]], 1);
			tracks[i].real++;
		}
		else
		{
			tracks[i].prediction_ = tracks[i].kalman.update(cv::Point3d(0, 0, 0), 0);
		}

		/*if (tracks[i].trace.size() > max_trace_length_)
		{
			for (int j = 0; j < tracks[i].trace.size() - max_trace_length_; j++)
			{
				tracks[i].trace.erase(tracks[i].trace.begin());
			}
		}*/
		
		tracks[i].trace.push_back(tracks[i].prediction_);
		tracks[i].kalman.lastPointResult_ = tracks[i].prediction_;
	}
}
