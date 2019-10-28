#include "findEpiLine.h"

EpiGeo::EpiGeo()
{
	R = (cv::Mat_<double>(3, 3) << 0, 0, 1, 0, 0.95, 0, -0.93, 0, 0);
	T = (cv::Mat_<double>(3, 1) << -54.67, 0, 53.47);
	//R = (cv::Mat_<double>(3, 3) << 1.1324584933210915e-02, -3.6707809869289858e-03, 9.9992913706074027e-01, -7.2941631488052565e-03, 9.9996635254407018e-01, 3.7535268305266423e-03, -9.9990927036409116e-01, -7.3361533963564752e-03, 1.1297428614336257e-02);
	//T = (cv::Mat_<double>(3, 1) << -5.1470698727199213e+01, 2.2122760773804612e-02, 5.3477628856229380e+01);
	C = (cv::Mat_<double>(4, 1) << 0, 0, 0, 1);
	a = (cv::Mat_<double>(3, 4) << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0);
	imgWidth = 2704;
	imgHeight = 1520;
	focalLength = 17;
	horizontalFovAngle = 1.5;
	alpha = (imgWidth / 2) / (focalLength*tan(0.5*horizontalFovAngle));

	//K = (cv::Mat_<double>(3, 3) << focalLength * alpha, 0.0, imgWidth / 2.0, 0.0, focalLength*alpha, imgHeight / 2.0, 0.0, 0.0, 1.0);
	K = (cv::Mat_<double>(3, 3) << 1.3889719780811658e+03, 0., 1.3082529520864737e+03, 0., 1.3889719780811658e+03, 7.5327594486217765e+02, 0., 0., 1.);
	
	
	K_dash = (cv::Mat_<double>(3, 3) << 1.3889719780811658e+03, 0., 1.3090883005307123e+03, 0., 1.3889719780811658e+03, 7.5516380970505634e+02, 0., 0., 1.);
	//K_dash = K.clone();

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
	
	/*std::cout << F.at<double>(0, 0) << ", " << F.at<double>(0, 1) << ", " << F.at<double>(0, 2) << std::endl;
	std::cout << F.at<double>(1, 0) << ", " << F.at<double>(1, 1) << ", " << F.at<double>(1, 2) << std::endl;
	std::cout << F.at<double>(2, 0) << ", " << F.at<double>(2, 1) << ", " << F.at<double>(2, 2) << std::endl;*/

}

EpiGeo::~EpiGeo()
{
}

void EpiGeo::update()
{
	alpha = (imgWidth / 2) / (focalLength*tan(0.5*horizontalFovAngle));
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
}

void EpiGeo::setRotationMat(cv::Mat rotation)
{
	R = rotation.clone();
	update();
}

void EpiGeo::setTranslationVec(cv::Mat translation)
{
	T = translation.clone();
	update();
}

void EpiGeo::setLeftCameraPos(cv::Mat leftCameraPos)
{
	C = leftCameraPos.clone();
	update();
}

void EpiGeo::setFocalLength(double focalLen)
{
	focalLength = focalLen;
	update();
}

double EpiGeo::getFocalLength()
{
	return focalLength;
}

void EpiGeo::setHorizontalFovAngle(double fovAngleH)
{
	horizontalFovAngle = fovAngleH;
	update();
}

void EpiGeo::setImageWidth(int imageWidth)
{
	imgWidth = imageWidth;
	update();
}

int EpiGeo::getImageWidth()
{
	return imgWidth;
}

void EpiGeo::setImageHeight(int imageHeight)
{
	imgHeight = imageHeight;
	update();
}

int EpiGeo::getImageHeight()
{
	return imgHeight;
}

double EpiGeo::getAlpha()
{
	return alpha;
}

std::vector<cv::Point> EpiGeo::findEpipolarLine(cv::Point imagePnt)
{
	cv::Mat iPnt = (cv::Mat_<double>(3, 1) << imagePnt.x, imagePnt.y, 1);
	l_dash = F * iPnt;

	std::vector<cv::Point> epiline_2dots;
	epiline_2dots.push_back(cv::Point(0, -l_dash.at<double>(2) / l_dash.at<double>(1)));
	epiline_2dots.push_back(cv::Point(imgWidth, -(l_dash.at<double>(2) + imgWidth * l_dash.at<double>(0)) / l_dash.at<double>(1)));
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