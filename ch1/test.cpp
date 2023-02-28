#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageToVTKImageFilter.h"
#include "vtkImageViewer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkImageActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
	typedef itk::Image<unsigned short, 2> ImageType;
	typedef itk::ImageFileReader<ImageType> ReaderType;
	typedef itk::ImageToVTKImageFilter<ImageType> ConnectorType;

	ReaderType::Pointer reader = ReaderType::New();
	ConnectorType::Pointer connector = ConnectorType::New();
	reader->SetFileName("D:\\Study\\Projects\\vtk_itk_test\\vtk_itk_test\\ThresholdSegmentationLevelSetWhiteMatterTest.png");
	connector->SetInput(reader->GetOutput());

	vtkImageActor* actor = vtkImageActor::New();
	actor->SetInputData(connector->GetOutput());
	vtkRenderer* ren = vtkRenderer::New();
	ren->AddActor(actor);
	vtkRenderWindow* renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren);

	vtkImageViewer* viewer = vtkImageViewer::New();
	vtkRenderWindowInteractor* renderWindowInteractor = vtkRenderWindowInteractor::New();
	viewer->SetInputData(connector->GetOutput());
	viewer->SetupInteractor(renderWindowInteractor);
	viewer->SetColorWindow(255);
	viewer->SetColorLevel(128);
	viewer->Render();
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();

	return 0;
}