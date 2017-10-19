
#ifndef __TRACKING_H
#define __TRACKING_H

#include <opencv2\opencv.hpp>

void drawOptFlowMap(const cv::Mat& flow, cv::Mat& cflowmap, int step, const cv::Scalar& color);
void simple_Farneback(cv::String prev_img, cv::String next_img);

#endif //!__TRACKING_H