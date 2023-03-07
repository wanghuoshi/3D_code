#include "vtkSmartPointer.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper2D.h"
#include "vtkActor2D.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkIdFilter.h"
#include "vtkRenderer.h"
#include "vtkSelectVisiblePoints.h"
#include "vtkLabeledDataMapper.h"
#include "vtkCellCenters.h"
#include "vtkLabeledDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkTextProperty.h"

int xLength, yLength;
vtkSmartPointer<vtkSelectVisiblePoints> visPts, visCells;
vtkSmartPointer<vtkPoints> pts;
vtkSmartPointer<vtkRenderWindow> renWin;

//创建一个用于画窗口的函数
void PlaceWindow(int xmin, int ymin)
{
    int xmax = xmin + xLength;
    int ymax = ymin + yLength;

    visPts->SetSelection(xmin, xmax, ymin, ymax);    //指定要显示的矩形区域
    visCells->SetSelection(xmin, xmax, ymin, ymax); 

    pts->InsertPoint(0, xmin, ymin, 0);    //Id、X、Y、Z
    pts->InsertPoint(1, xmin, ymin, 0);
    pts->InsertPoint(2, xmin, ymin, 0);
    pts->InsertPoint(3, xmin, ymin, 0);

    pts->Modified();
    renWin->Render();
}

//创建一个函数用于移动被选择的窗体
void MoveWindow()
{
    for(int y=0; y<300; y+=25)
    {
        for(int x=100; x<300; x+=25)
        {
            PlaceWindow(x,y);
        }
    }
}

int main()
{
    //创建显示区域的矩形窗口
    int xmin = 200;
    xLength = 100；
    int xmax = xmin + xLength;
    int ymin = 200;
    yLength = 100;
    int ymax = ymin + yLength;

    pts = vtkSmartPointer<vtkPoints>::New();
    pts->InsertPoint(0, xmin, ymin, 0);
    pts->InsertPoint(1, xmin, ymin, 0);
    pts->InsertPoint(2, xmin, ymin, 0);
    pts->InsertPoint(3, xmin, ymin, 0);
    
    vtkSmartPointer<vtkCellArray> rect = vtkSmartPointer<vtkCellArray>::New();
    rect->InsertNextCell(5);    //指定创建的单元数
    rect->InsertCellPoint(0);   //设置矩形四个点的链接结构
    rect->InsertCellPoint(1);
    rect->InsertCellPoint(2);
    rect->InsertCellPoint(3);
    rect->InsertCellPoint(0);

    vtkSmartPointer<vtkPolyData> selectRect = vtkSmartPointer<vtkPolyData>::New();
    selectRect->SetPoints(pts);
    selectRect->SetLines(rect);

    vtkSmartPointer<vtkPolyDataMapper2D> rectMapper = vtkSmartPointer<vtkPolyDataMapper2D>::New();
    rectMapper->SetInputData(selectRect);

    vtkSmartPointer<vtkActor2D> rectActor = vtkSmartPointer<vtkActor2D>::New();
    rectActor->SetMapper(rectMapper);

    //创建一个球及与它相关的映射器和角色
    vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
    vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    sphereMapper->SetInputConnection(sphere->GetOutputPort());
    sphereMapper->global
}