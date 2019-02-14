#include "KalmanFilter.h"

KF::KF(double deltaTime)
{
	dt_ = deltaTime;
}

KF::~KF()
{
}

void KF::setDeltaTime(double deltaTime)
{
	dt_ = deltaTime;
}

void KF::create(cv::Point3d p0, cv::Point3d v0)
{
	// Initialise Kalman filter
	// Create Kalman filter object with 4-d state, 2-d measurement and no control vector
	myKalman_ = cv::KalmanFilter(6, 3, 0);

	// state is (pos_x, pos_y, pos_z, v_x, v_y, v_z) - position and velocity
	cv::Mat x_k(6, 1, CV_32F);

	// Transition matrix 'F' describes relationship between model parameters
	// at step k and step k+1 (this is the "dynamics" in our model.
	float F[] = { 1, 0, 0, dt_, 0, 0,
		0, 1, 0, 0, dt_, 0,
		0, 0, 1, 0, 0, dt_,
		0, 0, 0, 1, 0, 0,
		0, 0, 0, 0, 1, 0,
		0, 0, 0, 0, 0, 1};

	cv::randn(x_k, cv::Scalar::all(0), cv::Scalar::all(0.1));
	myKalman_.transitionMatrix = cv::Mat(6, 6, CV_32F, F).clone();

	// Initialise other Kalman filter parameters. 
	cv::setIdentity(myKalman_.measurementMatrix);
	cv::setIdentity(myKalman_.processNoiseCov, cv::Scalar::all(1e-2));					//Q_k
	cv::setIdentity(myKalman_.measurementNoiseCov, cv::Scalar::all(1e-1));				//R_k
	cv::setIdentity(myKalman_.errorCovPost, cv::Scalar::all(.1));						//P_k

	// choose random initial state
	//cv::randn(KF.statePost, cv::Scalar::all(0), cv::Scalar::all(0.1));
	myKalman_.statePre.at<float>(0) = p0.x;
	myKalman_.statePre.at<float>(1) = p0.y;
	myKalman_.statePre.at<float>(2) = p0.z;
	myKalman_.statePre.at<float>(3) = v0.x;
	myKalman_.statePre.at<float>(4) = v0.y;
	myKalman_.statePre.at<float>(5) = v0.z;
	myKalman_.statePost.at<float>(0) = p0.x;
	myKalman_.statePost.at<float>(1) = p0.y;
	myKalman_.statePost.at<float>(2) = p0.z;
}

cv::Point3d KF::predict()
{
	cv::Mat y_k;	//prediction
	y_k = myKalman_.predict();

	//prediction point
	lastPointResult_ = cv::Point3d(y_k.at<float>(0), y_k.at<float>(1), y_k.at<float>(2));

	return lastPointResult_;
}

cv::Point3d KF::update(cv::Point3d measurement, bool flag)
{
	// measurement, positions on x and y axis
	cv::Mat z_k = cv::Mat::zeros(3, 1, CV_32F);

	if (!flag)
	{
		z_k.at<float>(0) = lastPointResult_.x;
		z_k.at<float>(1) = lastPointResult_.y;
		z_k.at<float>(2) = lastPointResult_.z;
	}
	else
	{
		//measurement point
		z_k.at<float>(0) = measurement.x;
		z_k.at<float>(1) = measurement.y;
		z_k.at<float>(2) = measurement.z;
	}
	
	myKalman_.correct(z_k);
	lastPointResult_ = cv::Point3d(myKalman_.statePost.at<float>(0), myKalman_.statePost.at<float>(1), myKalman_.statePost.at<float>(2));
	return lastPointResult_;
}
