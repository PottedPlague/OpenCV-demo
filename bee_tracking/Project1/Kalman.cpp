#include "Kalman.h"
#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

#define phi2xy(mat) Point(cvRound(img.cols/2 + img.cols/3*cos(mat.at<float>(0))), cvRound(img.rows/2 - img.cols/3*sin(mat.at<float>(0))))

void Kalman()
{
	//Initialise, create Kalman filter object, window, random number
	//generator etc.
	//
	Mat img(500, 500, CV_8UC3);
	KalmanFilter kalman(2, 1, 0);

	//state is (phi, delta_phi) - angle and angular velocity
	//Initialise with random guess.
	//
	Mat x_k(2, 1, CV_32F);
	randn(x_k, 0., 0.1);

	//process noise
	//
	Mat w_k(2, 1, CV_32F);

	//measurements, only one parameter for angle
	//
	Mat z_k = Mat::zeros(1, 1, CV_32F);

	//Transition matrix 'F' describes relationship between
	//model parameters at step k and at step k+1 (this is the "dynamics" in our model.
	//
	float F[] = { 1, 1, 0, 1 };
	kalman.transitionMatrix = Mat(2, 2, CV_32F, F).clone();

	//Initialise other Kalman filter parameters.
	//
	setIdentity(kalman.measurementMatrix, Scalar(1));
	setIdentity(kalman.processNoiseCov, Scalar(1e-5));
	setIdentity(kalman.measurementNoiseCov, Scalar(1e-1));
	setIdentity(kalman.errorCovPost, Scalar(1));

	//choose random initial state
	//
	randn(kalman.statePost, 0., 0.1);

	for (;;)
	{
		//predict point position
		//
		Mat y_k = kalman.predict();

		//generate measurement (z_k)
		//
		randn(z_k, 0., sqrt((double)kalman.measurementNoiseCov.at<float>(0, 0)));
		z_k = kalman.measurementMatrix*x_k + z_k;

		//plot points (e.g., convert to planar co-ordinates and draw)
		//
		img = Scalar::all(0);
		circle(img, phi2xy(z_k), 4, Scalar(128, 255, 255));		//observed
		circle(img, phi2xy(y_k), 4, Scalar(255, 255, 255), 2);	//predicted
		circle(img, phi2xy(x_k), 4, Scalar(0, 0, 255));			//actual

		imshow("Kalman", img);

		//adjust Kalman filter state
		//
		kalman.correct(z_k);

		//Apply the transition matrix 'F' (e.g., step time forward)
		//and also apply the "process" noise w_k
		//
		randn(w_k, 0., sqrt((double)kalman.processNoiseCov.at<float>(0, 0)));
		x_k = kalman.transitionMatrix*x_k + w_k;

		//exit if user hits 'Esc'
		if ((waitKey(100) & 255) == 27) break;
	}

}