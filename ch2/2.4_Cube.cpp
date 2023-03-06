#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkAutoInit.h" 

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

int main()
{
    int i;
    static float x[8][3] = { {0,0,0},{1,0,0},{1,1,0},{0,1,0},
                            {0,0,1},{1,0,1},{1,1,1},{0,1,1} };    //�˸�����
    static vtkIdType pts[6][4] = { {0,1,2,3},{4,5,6,7},{0,1,5,4},
                                  {1,2,6,5},{2,3,7,6},{3,0,4,7} };    //������˸�����ı�Ź�������

    //���幹��������ġ���ľ��                    
    vtkSmartPointer<vtkPolyData> cube = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkFloatArray> scalars = vtkSmartPointer<vtkFloatArray>::New();
    for (i = 0; i < 8; i++) points->InsertPoint(i, x[i]);        //������İ˸���
    for (i = 0; i < 6; i++) polys->InsertNextCell(4, pts[i]);    //�������������
    for (i = 0; i < 8; i++) scalars->InsertTuple1(i, i);         //����ö���εı���

    //�齨��ľ
    cube->SetPoints(points);
    cube->SetPolys(polys);
    cube->GetPointData()->SetScalars(scalars);

    vtkSmartPointer<vtkPolyDataMapper> cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cubeMapper->SetInputData(cube);
    cubeMapper->SetScalarRange(0, 7);    //ָ�������ķ�Χ

    vtkSmartPointer<vtkActor> cubeActor = vtkSmartPointer<vtkActor>::New();
    cubeActor->SetMapper(cubeMapper);

    vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();    //ָ�������
    camera->SetPosition(1, 1, 1);
    camera->SetFocalPoint(0, 0, 0);

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();    //��Ⱦ
    renderer->AddActor(cubeActor);
    renderer->SetActiveCamera(camera);
    renderer->ResetCamera();
    renderer->SetBackground(1, 1, 1);

    vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();    //ָ����Ⱦ����
    renWin->AddRenderer(renderer);
    renWin->SetSize(300, 300);

    vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();    //���ý�������
    iren->SetRenderWindow(renWin);
    iren->Initialize();    //��������
    iren->Start();

    return 0;
}