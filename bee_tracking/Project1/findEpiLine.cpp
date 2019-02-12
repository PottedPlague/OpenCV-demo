#include "findEpiLine.h"

EpiGeo::EpiGeo()
{
	R = (cv::Mat_<double>(3, 3) << 0, 0, 1, 0, 1, 0, -1, 0, 0);
	T = (cv::Mat_<double>(3, 1) << -1, 0, 1);
	C = (cv::Mat_<double>(4, 1) << 0, 0, 0, 1);
	a = (cv::Mat_<double>(3, 4) << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0);
	focalLength = 19.561;
	horizontalFovAngle = 0.950017618;
}

EpiGeo::~EpiGeo()
{
}

void EpiGeo::setRotationMat(cv::Mat rotation)
{
	R = rotation.clone();
}

void EpiGeo::setTranslationVec(cv::Mat translation)
{
	T = translation.clone();
}

void EpiGeo::setLeftCameraPos(cv::Mat leftCameraPos)
{
	C = leftCameraPos.clone();
}

void EpiGeo::setFocalLength(double focalLen)
{
	focalLength = focalLen;
}

void EpiGeo::setHorizontalFovAngle(double fovAngleH)
{
	horizontalFovAngle = fovAngleH;
}

std::vector<cv::Point> EpiGeo::findEpipolarLine(int imageHeight, int imageWidth, cv::Point imagePnt)
{
	double alpha = (imageWidth / 2) / (focalLength*tan(0.5*horizontalFovAngle));
	K = (cv::Mat_<double>(3, 3) << focalLength * alpha, 0.0, imageWidth / 2.0, 0.0, focalLength*alpha, imageHeight / 2.0, 0.0, 0.0, 1.0);
	K_dash = K.clone();

	cv::Mat b;
	cv::hconcat(R, T, b);

	P = K * a;
	P_dash = K_dash * b;
	cv::Mat P_transpose;
	cv::Mat P_dash_transpose;
	cv::Mat R_transpose;
	cv::transpose(P, P_transpose);
	cv::transpose(P_dash, P_dash_transpose);
	cv::transpose(R, R_transpose);
	P_plus = P_transpose * (P*P_transpose).inv();
	e = -K * R_transpose * T;
	e_dash = P_dash * C;
	e_dash_skew = (cv::Mat_<double>(3, 3) << 0.0, -e_dash.at<double>(2), e_dash.at<double>(1), e_dash.at<double>(2), 0.0, -e_dash.at<double>(0), -e_dash.at<double>(1), e_dash.at<double>(0), 0.0);
	F = e_dash_skew * P_dash * P_plus;
	F = F / F.at<double>(2, 2);
	cv::Mat iPnt = (cv::Mat_<double>(3, 1) << imagePnt.x, imagePnt.y, 1);
	l_dash = F * iPnt;

	std::vector<cv::Point> epiline_2dots;
	epiline_2dots.push_back(cv::Point(0, -l_dash.at<double>(2) / l_dash.at<double>(1)));
	epiline_2dots.push_back(cv::Point(imageWidth, -(l_dash.at<double>(2) + imageWidth * l_dash.at<double>(0)) / l_dash.at<double>(1)));
	return epiline_2dots;
}

double EpiGeo::pntCorrespondence(cv::Point leftPnt, cv::Point rightPnt)
{
	cv::Mat lpnt = (cv::Mat_<double>(3, 1) << leftPnt.x, leftPnt.y, 1);
	cv::Mat rpnt = (cv::Mat_<double>(3, 1) << rightPnt.x, rightPnt.y, 1);
	cv::Mat rpnt_transpose;
	cv::transpose(rpnt, rpnt_transpose);
	cv::Mat corres = (rpnt_transpose * F * lpnt);
	return corres.at<double>(0, 0);
}