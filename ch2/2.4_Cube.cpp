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

int main()
{
    int i;
    static float x[8][3] = {{0,0,0},{1,0,0},{1,1,0},{0,1,0},
                            {0,0,1},{1,0,1},{1,1,1},{0,1,1}};    //八个顶点
    static vtkIdType pts[6][4] = {{0,1,2,3},{4,5,6,7},{0,1,5,4},
                                  {1,2,6,5},{2,3,7,6},{3,0,4,7}};    //用上面八个顶点的标号构建六面

    //定义构成立方体的“积木”                    
    vtkSmartPointer<vtkPolyData> cube = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkFloatArray> scalars = vtkSmartPointer<vtkFloatArray>::New();
    for(i=0;i<8;i++) points->InsertPoint(i,x[i]);        //立方体的八个点
    for(i=0;i<8;i++) polys->InsertNextCell(4,pts[i]);    //立方体的六个面
    for(i=0;i<8;i++) scalars->InsertTuple(i,i);          //定义该多边形的标量

    //组建积木
    cube->SetPoints(points);
    cube->SetPolys(polys);
    cube->GetPointData()->SetScalars(scalars);

    vtkSmartPointer<vtkPolyDataMapper> cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cubeMapper->SetInputData(cube);
    cubeMapper->SetScalarRange(0,7);                     //指定标量的范围
    
    vtkSmartPointer<vtkActor> cubeActor = vtkSmartPointer<vtkActor>::New();
    cubeActor->SetMapper(cubeMapper);

    vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();    //指定照相机
    camera->SetPosition(1,1,1);
    camera->SetFocalPoint(0,0,0);

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();    //渲染
    renderer->AddActor(cubeActor);
    renderer->SetActiveCamera(camera);
    renderer->ResetCamera();
    renderer->SetBackground(1,1,1);

    vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer
}