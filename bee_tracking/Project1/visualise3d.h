#pragma once
#include <vtkAutoInit.h>
#define vtkRenderingCore_AUTOINIT 2(vtkRenderingOpenGL2, vtkInteractionStyle)

#include <array>
#include <vtkActor.h>
#include <vtkCallbackCommand.h>
#include <vtkCamera.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkCylinderSource.h>
#include <vtkDoubleArray.h>
#include <vtkLineSource.h>
#include <vtkNamedColors.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyLine.h>
#include <vtkProgrammableFilter.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkUnsignedCharArray.h>
#include <vtkVertexGlyphFilter.h>
#include <opencv2\opencv.hpp>
#include "track.h"
#include "tracker.h"
#include "tools.h"

int visualisation3d(std::vector<Track> trackData);
int visualisation3d(cv::String filename, bool plotDot = false);
int display(std::vector<std::vector<std::vector<double>>> tracks);
int displayDot(std::vector<std::vector<std::vector<double>>> tracks);