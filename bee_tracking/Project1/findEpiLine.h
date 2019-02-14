#pragma once
#include <opencv2\opencv.hpp>

class EpiGeo
{
public:
	EpiGeo();
	~EpiGeo();
	void setRotationMat(cv::Mat rotation);
	void setTranslationVec(cv::Mat translation);
	void setLeftCameraPos(cv::Mat leftCameraPos);
	void setFocalLength(double focalLen);
	double getFocalLength();
	void setHorizontalFovAngle(double fovAngleH);
	void setImageWidth(int imageWidth);
	int getImageWidth();
	void setImageHeight(int imageHeight);
	int getImageHeight();
	double getAlpha();
	std::vector<cv::Point> findEpipolarLine(cv::Point imagePnt);

	double pntCorrespondence(
		cv::Point leftPnt,
		cv::Point rightPnt);
private:
	cv::Mat R;
	cv::Mat T;
	cv::Mat C;
	cv::Mat e;
	cv::Mat e_dash;
	cv::Mat e_dash_skew;
	cv::Mat P;
	cv::Mat P_dash;
	cv::Mat P_plus;
	cv::Mat K;
	cv::Mat K_dash;
	cv::Mat l_dash;
	cv::Mat F;
	cv::Mat a;

	double focalLength;
	double horizontalFovAngle;
	double alpha;
	int imgWidth;
	int imgHeight;

};