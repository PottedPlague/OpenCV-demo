#include "sim_coorCalc.h"
#include <opencv2\opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <complex>

using namespace std;
using namespace cv;

static complex<double> complex_sqrt(complex<double> z)
{
	return pow(z, 1. / 2.);
}

static complex<double> complex_cbrt(complex<double> z)
{
	return pow(z, 1. / 3.);
}

int simCoorCalc()
{
	Mat coorL, coorR;
	double half_frame_width = 640.;								//640
	double S = 400.;											//target distance
	double f = 0.77011811;										//focal length
	double FOV_angle_h = 0.95;									//horizontal view angle of FOV - rad
	//double alpha = tan(FOV_angle_h / 2) / half_frame_width;;	//physical/pixel scaler
	double alpha = 0.396 / 640.;

	FileStorage fs;
	fs.open("dualChannelCoors.yml", FileStorage::READ);
	fs["coorL"] >> coorL;
	fs["coorR"] >> coorR;
	fs.release();

	Mat coor3D = Mat::zeros(Size(coorL.cols + 1, (coorL.rows > coorR.rows) ? coorR.rows : coorL.rows), CV_16S);
	
	for (int i = 0; i < coor3D.rows; i++)
	{
		/*double L = coorL.at<short>(i, 0);
		double R = coorR.at<short>(i, 0);
		complex<double> a = alpha * alpha * alpha * R * L * L;
		complex<double> b = (4. * alpha * alpha * alpha * R * L * L * S) / a;
		complex<double> c = (2. * alpha * alpha * R * L * f * S + 6 * alpha * alpha * alpha * R * L * L * S * S) / a;
		complex<double> d = (4. * alpha * alpha * R * L * f * S * S - f * f * f + 4. * alpha * alpha * alpha * R * L * L * S * S * S) / a;
		complex<double> e = (2. * alpha * alpha * R * L * f * S * S * S + alpha * alpha * alpha * R * L * L * S * S * S * S + alpha * R * f * f * S * S) / a;

		complex<double> Q1 = c * c - 3. * b * d + 12. * e;
		complex<double> Q2 = 2. * c * c * c - 9. * b * c * d + 27. * d * d + 27. * b * b * e - 72. * c * e;
		complex<double> Q3 = 8. * b * c - 16. * d - 2. * b * b * b;
		complex<double> Q4 = 3. * b * b - 8. * c;

		complex<double> Q5 = complex_cbrt(Q2 / 2. + complex_sqrt(Q2 * Q2 / 4. - Q1 * Q1 * Q1));
		complex<double> Q6 = (Q1 / Q5 + Q5) / 3.;
		complex<double> Q7 = 2. * complex_sqrt(Q4 / 12. + Q6);

		complex<double> roots[4];
		roots[0] = (-b - Q7 - complex_sqrt(4. * Q4 / 6. - 4. * Q6 - Q3 / Q7)) / 4.;
		roots[1] = (-b - Q7 + complex_sqrt(4. * Q4 / 6. - 4. * Q6 - Q3 / Q7)) / 4.;
		roots[2] = (-b + Q7 - complex_sqrt(4. * Q4 / 6. - 4. * Q6 + Q3 / Q7)) / 4.;
		roots[3] = (-b + Q7 + complex_sqrt(4. * Q4 / 6. - 4. * Q6 + Q3 / Q7)) / 4.;*/

		//coor3D.at<short>(i, 0) = 0.0;
		//coor3D.at<short>(i, 0) = 0.0;
		coor3D.at<short>(i, 0) = (S*alpha*coorR.at<short>(i, 0)*(f + alpha * coorL.at<short>(i, 0))) / (f * f - alpha * alpha * coorL.at<short>(i, 0) * coorR.at<short>(i, 0));
		coor3D.at<short>(i, 1) = (S*alpha*coorL.at<short>(i, 0)*(f + alpha * coorR.at<short>(i, 0))) / (f * f - alpha * alpha * coorL.at<short>(i, 0) * coorR.at<short>(i, 0));
		//coor3D.at<short>(i, 2) = i;
		coor3D.at<short>(i, 2) = (S*alpha*coorL.at<short>(i, 1)*(f + alpha * coorR.at<short>(i, 0))) / (f * f - alpha * alpha * coorL.at<short>(i, 0) * coorR.at<short>(i, 0));
		//coor3D.at<short>(i, 2) = (S*alpha*coorR.at<short>(i, 1)*(focal + alpha * coorL.at<short>(i, 0))) / (focal*focal - alpha * alpha*coorL.at<short>(i, 0)*coorR.at<short>(i, 0));
	}

	FileStorage out("coor3D.yml", FileStorage::WRITE);
	out << "coor3D" << coor3D;
	out.release();
	cout << "All done." << endl;

	/*Mat outputL = Mat::zeros(Size(1280, 720), CV_8UC3);
	Mat outputR = Mat::zeros(Size(1280, 720), CV_8UC3);

	for (int i = 0; i < coorL.rows; i++)
	{
		
		circle(outputL, Point(coorL.at<short>(i, 0), coorL.at<short>(i, 1)), 2, Scalar(0, 255, 255), -1);
		circle(outputR, Point(coorR.at<short>(i, 0), coorR.at<short>(i, 1)), 2, Scalar(0, 0, 255), -1);

		imshow("OutputL", outputL);
		imshow("OutputR", outputR);
		waitKey(100);
	}
	destroyAllWindows();*/
	waitKey(300);

	return 0;
}
