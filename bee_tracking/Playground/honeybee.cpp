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

void honeybee::add(cv::Mat dst, cv::Rect2d range, cv::Point2d initPosition, int duration, cv::Scalar beeColour)
{
	background_ = dst.clone();
	duration_ = duration;
	initialposition_ = initPosition;
	beecolour_ = beeColour;
	bindingbox_ = range;
	positions_.push_back(initPosition);
}

void honeybee::startMoving()
{
	currentposition_.x = bindingbox_.x + rand() % 100 / 100.0 * bindingbox_.width;
	currentposition_.y = bindingbox_.y + rand() % 100 / 100.0 * bindingbox_.height;
	positions_.push_back(currentposition_);

	drawingboard_ = background_.clone();
	cv::circle(drawingboard_, positions_.back(), 4, beecolour_, -1);

	cv::imshow("Output", drawingboard_);
	cv::waitKey(30);
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