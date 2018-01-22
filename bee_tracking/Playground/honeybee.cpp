#include "honeybee.h"

using namespace beeproject;

cv::Mat background;
cv::Mat drawingboard;

cv::Rect2d bindingbox;
int count;
int resetPoint = 100;

std::vector<Honeybee> bees;
std::vector<cv::Mat> frames;

std::complex<double> result;

Honeybee::Honeybee() 
{
	initialposition_ = cv::Point2d(0, 0);
	beecolour_ = cv::Scalar(0, 255, 255);
	rands[0] = rand() % 50 / 10.0;
	rands[1] = rand() % 50 / 10.0;
	rands[2] = rand() % 100 / 10.0;
	rands[3] = rand() % 50 / 10.0;
	rands[4] = rand() % 50 / 10.0;
	rands[5] = rand() % 100 / 10.0;
	angvelocity_ = (500 + rand() % 1000) / 30000.0;
}

Honeybee::~Honeybee() {}

void Honeybee::setMovementRange(cv::Rect2d range)
{
	bindingbox_ = range;
}

void Honeybee::setFlightDuration(int msec)
{
	duration_ = msec;
}

void Honeybee::setFramerate(int frate)
{
	framerate_ = frate;
}



void Honeybee::nextPosition()
{
	if (count % resetPoint == 0)
	{
		rands[3] = rands[0];
		rands[4] = rands[1];
		rands[5] = rands[2];

		rands[0] = rand() % 50 / 10.0;
		rands[1] = rand() % 50 / 10.0;
		rands[2] = rand() % 100 / 10.0;

	}
	theta_ = angvelocity_ * count;
	rho_ = (count % resetPoint) * (7 - 3 * cos(rands[0] * theta_) * sin(rands[1] * theta_) + cos(rands[2] * theta_))
			+ (resetPoint - 1 - count % resetPoint) * (7 - 3 * cos(rands[3] * theta_) * sin(rands[4] * theta_) + cos(rands[5] * theta_));

	result = std::polar(rho_, theta_);
	currentposition_.x = 30.0/resetPoint * result.real() + bindingbox.x + bindingbox.width / 2;
	currentposition_.y = 30.0/resetPoint * result.imag() + bindingbox.y + bindingbox.height / 2;
	positions_.push_back(currentposition_);
}

double Honeybee::getAcceleration()
{
	return acceleration_;
}

double Honeybee::getVelocity()
{
	return velocity_;
}

cv::Point2d Honeybee::getPosition()
{
	return positions_.back();
}

cv::Scalar Honeybee::getColour()
{
	return beecolour_;
}

void beeproject::add(cv::Mat dst, int amount, cv::Rect2d range)
{
	for (int i = 0; i < amount; i++)
	{
		Honeybee bee;
		bees.push_back(bee);
	}
		
	background = dst.clone();
	bindingbox = range;
}

std::vector<cv::Mat> beeproject::startMoving(int frate, int msec)
{
	int framerate = frate;
	int durationT = msec;

	count = 0;

	for (; count < durationT * framerate / 1000; count++)
	{
		drawingboard = background.clone();
		for (size_t i = 0; i < bees.size(); i++)
		{
			bees[i].nextPosition();
			cv::circle(drawingboard, bees[i].getPosition(), 4, bees[i].getColour(), -1);
		}
		
		/*bees[0].nextPosition();
		cv::circle(drawingboard, bees[0].getPosition(), 4, bees[0].getColour(), -1);*/
		
		frames.push_back(drawingboard);

		cv::imshow("Output", drawingboard);
		cv::waitKey(20);	//framerate control
	}
	return frames;
}
