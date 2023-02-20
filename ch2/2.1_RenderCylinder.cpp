#include "vtkCylinderSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkAutoInit.h" 

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

int main()
{
	vtkCylinderSource* cylinder = vtkCylinderSource::New();
	cylinder->SetResolution(8);
	vtkPolyDataMapper* cylinderMapper = vtkPolyDataMapper::New();
	cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

	vtkActor* cylinderActor = vtkActor::New();
	cylinderActor->SetMapper(cylinderMapper);
	cylinderActor->GetProperty()->SetColor(1.0000, 0.3882, 0.2784);
	cylinderActor->RotateX(30.0);
	cylinderActor->RotateY(-45.0);

	vtkRenderer* renl = vtkRenderer::New();
	vtkRenderWindow* renWin = vtkRenderWindow::New();
	renWin->AddRenderer(renl);
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	renl->AddActor(cylinderActor);
	renl->SetBackground(0.1, 0.2, 0.4);
	renWin->SetSize(200, 200);

	renl->ResetCamera();
	renl->GetActiveCamera()->Zoom(1.5);

	renWin->Render();

	iren->Start();
	cylinder->Delete();
	cylinderMapper->Delete();
	cylinderActor->Delete();
	renl->Delete();
	renWin->Delete();
	iren->Delete();

	return 0;
}