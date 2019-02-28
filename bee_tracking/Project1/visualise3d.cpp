#include "visualise3d.h"

int visualisation3d(std::vector<Track> trackData)
{
	std::vector<std::vector<std::vector<double>>> tracks;
	std::vector<std::vector<double>> track;
	for (int i = 0; i < trackData.size(); i++)
	{
		track.clear();
		for (int j = 0; j < trackData[i].trace.size(); j++)
		{
			std::vector<double> pnt = { 
				trackData[i].trace[j].x,
				trackData[i].trace[j].y,
				trackData[i].trace[j].z };
			track.push_back(pnt);
		}
		tracks.push_back(track);
	}
	return display(tracks);
}

int visualisation3d(cv::String filename, bool plotDot)
{
	if (plotDot)
		return displayDot(vectorReader(filename));
	else
		return display(vectorReader(filename));
}

int display(std::vector<std::vector<std::vector<double>>> tracks)
{
	int lenDispThresh = 100;
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	//renderWindow->SetSize(300, 300);
	renderWindow->AddRenderer(renderer);
	renderWindow->SetWindowName("Lines");
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	unsigned char clr[10][3] =
	{
		{ 255, 0, 0 },{ 0, 255, 0 },{ 0, 0, 255 },
	{ 255, 255, 0 },{ 0, 255, 255 },{ 255, 0, 255 },
	{ 255, 127, 255 },{ 127, 0, 255 },{ 127, 0, 127 }
	};

	vtkSmartPointer<vtkPolyData> polyData =
		vtkSmartPointer<vtkPolyData>::New();

	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();

	vtkSmartPointer<vtkPolyLine> polyLine =
		vtkSmartPointer<vtkPolyLine>::New();

	vtkSmartPointer<vtkCellArray> lines =
		vtkSmartPointer<vtkCellArray>::New();

	vtkSmartPointer<vtkUnsignedCharArray> colors =
		vtkSmartPointer<vtkUnsignedCharArray>::New();

	vtkSmartPointer<vtkPolyDataMapper> lineMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();

	vtkSmartPointer<vtkActor> lineActor =
		vtkSmartPointer<vtkActor>::New();

	for (int i = 0; i < tracks.size(); i++)
	//for (int i = 2; i < 4; i++)
	{
		if (i % 2 == 0)
		{
			if (tracks[i].size() > lenDispThresh)
			{
				polyData = vtkSmartPointer<vtkPolyData>::New();

				points = vtkSmartPointer<vtkPoints>::New();
				for (int j = 0; j < tracks[i].size(); j++)
				{
					double pnt[3] = { tracks[i][j][0], tracks[i][j][1], tracks[i][j][2] };
					points->InsertNextPoint(pnt);
				}

				polyData->SetPoints(points);

				polyLine = vtkSmartPointer<vtkPolyLine>::New();
				polyLine->GetPointIds()->SetNumberOfIds(tracks[i].size());
				for (unsigned int j = 0; j < tracks[i].size(); j++)
				{
					polyLine->GetPointIds()->SetId(j, j);
				}

				lines = vtkSmartPointer<vtkCellArray>::New();
				lines->InsertNextCell(polyLine);

				polyData->SetLines(lines);

				colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
				colors->SetNumberOfComponents(3);
				colors->InsertNextTypedTuple(clr[i % 9]);

				polyData->GetCellData()->SetScalars(colors);

				lineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
				lineMapper->SetInputData(polyData);

				lineActor = vtkSmartPointer<vtkActor>::New();

				lineActor->SetMapper(lineMapper);
				renderer->AddActor(lineActor);
			}
		}
	}

	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}

int displayDot(std::vector<std::vector<std::vector<double>>> coor3d) 
{
	vtkSmartPointer<vtkNamedColors> colors =
		vtkSmartPointer<vtkNamedColors>::New();
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> vertices =
		vtkSmartPointer<vtkCellArray>::New();

	for (int i = 0; i < coor3d.size(); i++)
	{
		for (int j = 0; j < coor3d[i].size(); j++)
		{
			double pnt[3] = { coor3d[i][j][0], coor3d[i][j][1], coor3d[i][j][2] };
			points->InsertNextPoint(pnt);
			vtkIdType pid[1];
			pid[0] = points->InsertNextPoint(pnt);
			vertices->InsertNextCell(1, pid);
		}
	}

	vtkSmartPointer<vtkPolyData> pointCloud =
		vtkSmartPointer<vtkPolyData>::New();

	pointCloud->SetPoints(points);
	pointCloud->SetVerts(vertices);

	// Visualize
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(pointCloud);

	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());
	actor->GetProperty()->SetPointSize(2);

	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetWindowName("Points");
	renderWindow->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	renderer->AddActor(actor);
	//renderer->SetBackground(colors->GetColor3d("DarkOliveGreen").GetData());

	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}