#pragma once
#include <opencv2\opencv.hpp>
#include <complex>

namespace beeproject {

	class Honeybee
	{
	public:
		Honeybee();
		~Honeybee();

		/* @Returns current position of honeybee model. 
		The coordinate system uses OpenCV 2D double point type - Point2d. */
		cv::Point2d getPosition();
		/* @Returns velocity of honeybee in current position. (m/s)*/
		double getVelocity();
		/* @Returns current linear acceleration of honeybee in current position. (m/s2)*/
		double getAcceleration();
		/* @Returns the colour of bee in Scalar type. */
		cv::Scalar getColour();

		/* @Function creates a binding box same size as the selected ROI.
		Then the boneybee appears on the background plane as a filled circle with a diameter of 4 within the binding box.
		The appearing position is randomly calculated with certain velocity and acceleration constraints.
		The appearing duration and framerate is configured by user in advance or by default. */
		void nextPosition();

		/* @Sets the flight duration of bee. 
		@param msec - duration in millisecond*/
		void setFlightDuration(int msec);
		/* @Sets the framerate of bee movement. 30fps by default. */
		void setFramerate(int frate);
		/* @Sets the binding range of bee's movement. 
		@param range - Rect2d type of a rectangular box. Defined by the left-top coordinates, width and height of the rectangle in an order of (x, y, width, height).*/
		void setMovementRange(cv::Rect2d range);

	private:
		double rho_;
		double theta_;
		double velocity_;
		double angvelocity_;
		double acceleration_;
		int framerate_;
		int duration_;
		cv::Scalar beecolour_;
		cv::Rect2d bindingbox_;
		cv::Point2d initialposition_;
		cv::Point2d currentposition_;
		std::vector<cv::Point2d> positions_;
		cv::Mat background_;
		cv::Mat drawingboard_;
		double rands[6];
	};

	/* @Adds a set of honeybees to the moving plane.
	@param dst - the Mat type plane to put bee on. The plane colour is pure black by default.
	@param range - rectangular boundaries of honeybee's range of movement.
	@param initPosition - initial position of bee if not defined by user.
	@param duration - flight duration of bee.
	@param beeColour - the colour of bee model shown in the background defined by BGR values. Yellow by default. */
	void add(cv::Mat dst, int amount, cv::Rect2d range);

	void startMoving(int frate = 50, int msec = 10000);
}	//namespace beeproject