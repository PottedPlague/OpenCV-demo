#include "Kalman.h"

using namespace beeproject;

KF::KF(double deltaTime)
{
	dt_ = deltaTime;
}

KF::~KF()
{
}

void KF::create(cv::Point2d p0, cv::Point2d v0)
{
	// Initialise Kalman filter
	// Create Kalman filter object with 4-d state, 2-d measurement and no control vector
	myKalman_ = cv::KalmanFilter(4, 2, 0);

	// state is (pos_x, pos_y, v_x, v_y) - position and velocity
	cv::Mat x_k(4, 1, CV_32F);

	// Transition matrix 'F' describes relationship between model parameters
	// at step k and step k+1 (this is the "dynamics" in our model.
	float F[] = { 1, 0, dt_, 0,
				  0, 1, 0, dt_,
				  0, 0, 1, 0,
				  0, 0, 0, 1 };

	cv::randn(x_k, cv::Scalar::all(0), cv::Scalar::all(0.1));
	myKalman_.transitionMatrix = cv::Mat(4, 4, CV_32F, F).clone();

	// Initialise other Kalman filter parameters. 
	cv::setIdentity(myKalman_.measurementMatrix);
	cv::setIdentity(myKalman_.processNoiseCov, cv::Scalar::all(1e-1));
	cv::setIdentity(myKalman_.measurementNoiseCov, cv::Scalar::all(1e-1));
	cv::setIdentity(myKalman_.errorCovPost, cv::Scalar::all(.1));

	// choose random initial state
	//cv::randn(KF.statePost, cv::Scalar::all(0), cv::Scalar::all(0.1));
	myKalman_.statePre.at<float>(0) = p0.x;
	myKalman_.statePre.at<float>(1) = p0.y;
	myKalman_.statePre.at<float>(2) = v0.x;
	myKalman_.statePre.at<float>(3) = v0.y;
	myKalman_.statePost.at<float>(0) = p0.x;
	myKalman_.statePost.at<float>(1) = p0.y;
}

cv::Point2d KF::predict()
{
	cv::Mat y_k;	//prediction
	y_k = myKalman_.predict();

	//prediction point
	lastPointResult_ = cv::Point2d(y_k.at<float>(0), y_k.at<float>(1));

	return lastPointResult_;
}

void KF::update(cv::Point2d measurement)
{
	// measurement, positions on x and y axis
	cv::Mat z_k = cv::Mat::zeros(2, 1, CV_32F);

	//measurement point
	z_k.at<float>(0) = measurement.x;
	z_k.at<float>(1) = measurement.y;

	myKalman_.correct(z_k);
}
