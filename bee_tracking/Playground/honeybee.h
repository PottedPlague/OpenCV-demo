#pragma once
#include <opencv2\opencv.hpp>

namespace beeproject {
	class honeybee
	{
	public:
		honeybee();
		~honeybee();

		/* @Returns current position of honeybee model. 
		The coordinate system uses OpenCV 2D double point type - Point2d. */
		cv::Point2d getPosition();
		/* @Returns velocity of honeybee in current position. (m/s)*/
		double getVelocity();
		/* @Returns current linear acceleration of honeybee in current position. (m/s2)*/
		double getAcceleration();

		/* @Function creates a binding box same size as the selected ROI. 
		Then the boneybee appears on the background plane as a filled circle with a diameter of 4 within the binding box. 
		The appearing position is randomly calculated with certain velocity and acceleration constraints. 
		The appearing duration and framerate is configured by user in advance or by default. */
		void startMoving();
		/* @Adds a honeybee to the moving plane. 
		@param dst - the Mat type plane to put bee on. The plane is usually a pure black plane. 
		@param range - */
		void add(cv::Mat dst,
				 cv::Rect2d range,
				 cv::Point2d initPosition = cv::Point2d(0, 0),
				 int duration = 10000,
				 cv::Scalar beeColour = cv::Scalar(0, 255, 255));	

		void setFlightDuration(int msec);
		void setFramerate(int frate);
		void setMovementRange(cv::Rect2d range);

	private:
		double velocity_;
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
	};

}	//namespace beeproject