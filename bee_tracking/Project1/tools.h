#pragma once
#include <opencv2\opencv.hpp>
#include "track.h"

int imgSeqToAVI();
std::string intToStr(int number);
std::string dblToStr(double number);
int saveVecOfVecOfPoints(std::vector<Track> tracks, cv::String filename);
int saveVecOfVecOfPoints(std::vector<std::vector<cv::Point3d>> pnt3d, cv::String filename);
std::vector<std::vector<std::vector<double>>> vectorReader(cv::String filename);
void mouse_callback(int event, int x, int y, int flag, void *param);