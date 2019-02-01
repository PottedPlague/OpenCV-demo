#pragma once
#include "tracker.h"
int simCoorCalc(std::vector<std::vector<Track*>>& matchedPairs, int frameWidth, int frameHeight);
cv::Mat simCoorCalc(cv::Mat pntL, cv::Mat pntR, int frameWidth, int frameHeight);
