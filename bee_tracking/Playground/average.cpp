
#include <opencv2\opencv.hpp>
#include <iostream>
#include <fstream>
#include "average.h"

using namespace cv;
using namespace std;

void calc_average()
{
	vector<double> Average;
	Mat smile = imread("D:/pic/smile.bmp");

	//[!input the folder directory here!]
	vector<String> filenames;
	String folder = "F:/PhD/noise measuring/07_03_2017/";

	glob(folder, filenames);

	cout << "Ready to start.." << endl;

	namedWindow("Pause");

	for (size_t i = 0; i < filenames.size(); ++i)
	{
		Mat img = imread(filenames[i]);

		if (!img.data)
			cerr << "Problem loading image!" << endl;

		cout << filenames[i] << endl;
		Scalar AvgGrayValue = mean(img);
		cout << AvgGrayValue.val[0] << endl;
		cout << endl;

		Average.push_back(AvgGrayValue.val[0]);

	}

	cout << "Storing results to file, please wait..." << endl;
	ofstream output_file("F:/PhD/noise measuring/average.txt");
	ostream_iterator<double> output_iterator(output_file, "\n");
	copy(Average.begin(), Average.end(), output_iterator);
	cout << "Results saved successfully!" << endl;

	imshow("Finish!", smile);
	cout << "All done! Press ESCAPE to quit." << endl;
	
	for (;;)
	{
		if (waitKey(10) == 27)
			break;
	}
}