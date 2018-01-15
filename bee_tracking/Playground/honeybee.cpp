#include "honeybee.h"

using namespace beeproject;

honeybee::honeybee() 
{
	framerate_ = 30;
}

honeybee::~honeybee() {}

void honeybee::setMovementRange(cv::Rect2d range)
{
	bindingbox_ = range;
}

void honeybee::setFlightDuration(int msec)
{
	duration_ = msec;
}

void honeybee::setFramerate(int frate)
{
	framerate_ = frate;
}

void honeybee::add(cv::Mat dst, cv::Rect2d range, cv::Point2d initPosition, cv::Scalar beeColour)
{
	background_ = dst.clone();
	initialposition_ = initPosition;
	beecolour_ = beeColour;
	bindingbox_ = range;
	positions_.push_back(initPosition);
}

void honeybee::startMoving(int msec, int frate)
{
	duration_ = msec;
	framerate_ = frate;
	double rand_a = rand() % 50 / 10.0; 
	double rand_b = rand() % 50 / 10.0; 
	double rand_c = rand() % 100 / 10.0; 
	double rand_x = rand() % 50 / 10.0; 
	double rand_y = rand() % 50 / 10.0; 
	double rand_z = rand() % 100 / 10.0;
	
	std::complex<double> result;
	for (int i = 0; i < duration_*framerate_ / 500; i++)
	{
		if (i % 20 == 0)
		{
			rand_x = rand_a;
			rand_y = rand_b;
			rand_z = rand_c;

			rand_a = rand() % 50 / 10.0;
			rand_b = rand() % 50 / 10.0;
			rand_c = rand() % 100 / 10.0;


		}
		theta_ = 0.05 * i;
		rho_ = (i % 20) * (7 - 3 * cos(rand_a * theta_) * sin(rand_b * theta_) + cos(rand_c * theta_))
				+ (19 - i % 20) * (7 - 3 * cos(rand_x * theta_) * sin(rand_y * theta_) + cos(rand_z * theta_));

		result = std::polar(rho_, theta_);
		currentposition_.x = 1.4 * result.real() + bindingbox_.x + bindingbox_.width / 2;
		currentposition_.y = 1.4 * result.imag() + bindingbox_.y + bindingbox_.height / 2;
		positions_.push_back(currentposition_);

		drawingboard_ = background_.clone();
		cv::circle(drawingboard_, positions_.back(), 4, beecolour_, -1);

		cv::imshow("Output", drawingboard_);
		cv::waitKey(10);
	}
	
}

double honeybee::getAcceleration()
{
	return acceleration_;
}

double honeybee::getVelocity()
{
	return velocity_;
}

cv::Point2d honeybee::getPosition()
{
	return positions_.back();
}