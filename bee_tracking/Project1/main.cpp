#include "3dRecording.h"
#include "camCalib.h"
#include "dualCam.h"
#include "disparity.h"
#include "birdsEye.h"
#include "sim_subtractor.h"
#include "sim_detector.h"
#include "sim_coorCalc.h"
#include "KalmanFilter.h"
#include "Hungarian.h"
#include "tracking_main.h"
#include "sim_tracker.h"
#include "findEpiLine.h"
#include "tools.h"
#include "matcher.h"
#include <array>

#include <vtkAutoInit.h>
#define vtkRenderingCore_AUTOINIT 2(vtkRenderingOpenGL2, vtkInteractionStyle)
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCylinderSource.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>




using namespace cv;
using namespace std;

int main()
{
	vtkSmartPointer<vtkNamedColors> colors =
		vtkSmartPointer<vtkNamedColors>::New();

	// Set the background color.
	std::array<unsigned char, 4> bkg{ { 26, 51, 102, 255 } };
	colors->SetColor("BkgColor", bkg.data());

	// This creates a polygonal cylinder model with eight circumferential facets
	// (i.e, in practice an octagonal prism).
	vtkSmartPointer<vtkCylinderSource> cylinder =
		vtkSmartPointer<vtkCylinderSource>::New();
	cylinder->SetResolution(8);

	// The mapper is responsible for pushing the geometry into the graphics
	// library. It may also do color mapping, if scalars or other attributes are
	// defined.
	vtkSmartPointer<vtkPolyDataMapper> cylinderMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

	// The actor is a grouping mechanism: besides the geometry (mapper), it
	// also has a property, transformation matrix, and/or texture map.
	// Here we set its color and rotate it around the X and Y axes.
	vtkSmartPointer<vtkActor> cylinderActor = vtkSmartPointer<vtkActor>::New();
	cylinderActor->SetMapper(cylinderMapper);
	cylinderActor->GetProperty()->SetColor(
		colors->GetColor4d("Tomato").GetData());
	cylinderActor->RotateX(30.0);
	cylinderActor->RotateY(-45.0);

	// The renderer generates the image
	// which is then displayed on the render window.
	// It can be thought of as a scene to which the actor is added
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(cylinderActor);
	renderer->SetBackground(colors->GetColor3d("BkgColor").GetData());
	// Zoom in a little by accessing the camera and invoking its "Zoom" method.
	renderer->ResetCamera();
	renderer->GetActiveCamera()->Zoom(1.5);

	// The render window is the actual GUI window
	// that appears on the computer screen
	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetSize(300, 300);
	renderWindow->AddRenderer(renderer);
	renderWindow->SetWindowName("Cylinder");

	// The render window interactor captures mouse events
	// and will perform appropriate camera or actor manipulation
	// depending on the nature of the events.
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	// This starts the event loop and as a side effect causes an initial render.
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;

	/*VideoCapture cap(1);
	Mat frame;
	namedWindow("Camera");
	moveWindow("Camera", 10, 10);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
	for (;;)
	{
		cap >> frame;
		imshow("Camera", frame);
		if (waitKey(16) == 27)
			break;
	}
	destroyAllWindows();
	waitKey(200);
	return 0;*/

	

	/*Mat frame = imread("D:/pic/epipolar_right.tif");
	line(frame, Point(0, 656), Point(1920, 903), Scalar(0, 0, 255), 3);
	line(frame, Point(0, 469), Point(1920, 320), Scalar(0, 255, 255), 3);
	imshow("Epipolar line", frame);
	waitKey(0);
	destroyAllWindows();
	return 0;*/

	/*Mat img = imread("Picture12.bmp", 1);
	namedWindow("output", WINDOW_KEEPRATIO);
	imshow("output", img);
	cout << img.size() << endl;
	waitKey(0);
	destroyAllWindows();
	waitKey(300);
	return 0;*/

	////disparityGen();
	//Ptr<StereoSGBM> stereo = StereoSGBM::create();
	//stereo->setBlockSize(11);				//odd number >= 1
	//stereo->setDisp12MaxDiff(1);
	//stereo->setMinDisparity(-39);
	//stereo->setNumDisparities(112);			//(maxDisp - minDisp). must be divisible by 16
	//stereo->setP1(216);
	//stereo->setP2(864);
	//stereo->setPreFilterCap(63);
	//stereo->setSpeckleRange(32);
	//stereo->setSpeckleWindowSize(100);
	//stereo->setUniquenessRatio(10);			//usually 5-15
	//stereo->setMode(stereo->MODE_SGBM);

	//Mat img_L, img_R, disp, vdisp;

	//img_L = imread("D:/pic/stereo/left.png");
	//img_R = imread("D:/pic/stereo/right.png");

	//cout << "computing disparity map..." << endl;
	//stereo->compute(img_R, img_L, disp);
	//normalize(disp, vdisp, 0, 256, NORM_MINMAX, CV_8U);

	//cout << "now displaying the results..." << endl;
	//imshow("Disparity", vdisp);
	//imshow("image1", img_L);
	//imshow("image2", img_R);
	//while (1)
	//{
	//	if (waitKey(5) == 27)
	//		break;
	//}

	//return birdsEyeView();
	//return camCalib();
	//return simSubtractor();
	//return simDetector();
	//return simCoorCalc();
	//return simTracker();
	//return dualCam();
	//return trackingMain();
}