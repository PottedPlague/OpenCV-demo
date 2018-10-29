#include "tracker.h"

using namespace beeproject;

Track::Track(int ID, cv::Point2d initPt)
{
	trackID_ = ID;
	coorPre_ = initPt;
	skippedFrames_ = 0;
	kalman_.create(initPt);
	isReceived = 0;
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
	kalman_.update(measurement);
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
	std::vector<cv::Scalar> colours = { cv::Scalar(0, 254, 0), cv::Scalar(0, 0, 254), cv::Scalar(254, 0, 0) };

	std::vector<Track> tracks;
	detections_ = detections;
	cv::Mat drawingboard = cv::Mat::zeros(cv::Size(800, 800), CV_8UC3);

	//video writer
	std::string filename = "D:/pic/3points_encounter.avi";
	cv::VideoWriter writer(filename, CV_FOURCC('M', 'J', 'P', 'G'), 30, cv::Size(drawingboard.cols, drawingboard.rows));

	for (int frameCount = 5; frameCount < detections_.size(); frameCount++)			//frame starts from 5
	{
		std::vector<int> assignment;
		if (tracks.size() == 0 && detections_[frameCount].size() != 0)
			for (int i = 0; i < detections_[frameCount].size(); i++)
			{
				tracks.push_back(Track(countID_, detections_[frameCount][i]));
				countID_++;															//ID starts from 0
			}

		int N = tracks.size();														//N - tracks; M - new detections
		int M = detections_[frameCount].size();
		for (int i = 0; i < M; i++)
			assignment.push_back(-1);
		
		cv::Mat cost = cv::Mat::zeros(cv::Size(M, N), CV_32F);						//cost matrix
		
		for (int p = 0; p < N; p++)
		{
			tracks[p].makePrediction();												//make prediction of each track in this individual frame
			for (int q = 0; q < M; q++)
			{
				cost.at<float>(p, q) = cv::norm(tracks[p].getPrediction() - detections_[frameCount][q]);
				//std::cout << tracks[p].getPrediction().x << ", " << tracks[p].getPrediction().y << std::endl;
				//std::cout << detections_[frameCount][q].x << ", " << detections_[frameCount][q].y << std::endl;
				std::cout << cost.at<float>(p, q) << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;

		//start assignment
		for (int p = 0; p < N; p++)
		{
			double min = std::numeric_limits<double>::max();
			for (int q = 0; q < M; q++)
			{
				if (assignment[q] == -1)
				{
					if (cost.at<float>(p, q) < min && cost.at<float>(p, q) < 20.0)
					{
						min = cost.at<float>(p, q);
						assignment[q] = p;
						tracks[p].isReceived = 1;
					}
				}
			}
		}

		//check if all existing tracks have assignment, use previous predictions to update those have no assigned points.
		for (int i = 0; i < N; i++)
		{
			if (tracks[i].isReceived == 0)
			{
				tracks[i].updateKalman(tracks[i].getPrediction());
			}
			else
			{
				if (frameCount > 380)
					cv::circle(drawingboard, tracks[i].getPrediction(), 2, colours[i], -1);
			}
				
		}

		//check if all the detection points have assigned to tracks, create new tracks for points belong to no track
		for (int i = 0; i < M; i++)
		{
			if (assignment[i] == -1)
				continue;															//------talk about this later------
			else
			{
				tracks[assignment[i]].updateKalman(detections_[frameCount][i]);
				tracks[i].isReceived = 0;
			}
		}

		/*cv::circle(drawingboard, tracks[0].getPrediction(), 2, cv::Scalar(0, 255, 0), -1);
		cv::circle(drawingboard, detections_[frameCount][assignment[0]], 2, cv::Scalar(0, 130, 0), -1);
		cv::circle(drawingboard, tracks[1].getPrediction(), 2, cv::Scalar(0, 0, 255), -1);
		cv::circle(drawingboard, detections_[frameCount][assignment[1]], 2, cv::Scalar(0, 0, 130), -1);
		cv::circle(drawingboard, tracks[2].getPrediction(), 2, cv::Scalar(255, 0, 0), -1);
		cv::circle(drawingboard, detections_[frameCount][assignment[2]], 2, cv::Scalar(130, 0, 0), -1);*/
		
		cv::imshow("Output", drawingboard);
		cv::waitKey(30);				//30 or 80

		writer.write(drawingboard.clone());
	}

	// calculate cost matrix

}
