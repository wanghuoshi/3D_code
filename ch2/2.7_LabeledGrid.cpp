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
    sphereMapper->GlobalImmediateModelRenderingOn();

    vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
    sphereActor->SetMapper(sphereMapper);

    //通过vtkIdFilter产生点、单元的Id集
    vtkSmartPointer<vtkIdFilter> ids = vtkSmartPointer<vtkIdFilter>::New();
    ids->SetInputConnection(sphere->GetOutputPort());
    ids->PointIdsOn();
    ids->CellIdsOn();
    ids->FieldDataOn();

    //为vtkSelectVisiblePoints创建一个渲染器
    vtkSmartPointer<vtkRenderer> renl = vtkSmartPointer<vtkRenderer>::New();

    //为点创建标记
    visPts = vtkSmartPointer<vtkSelectVisiblePoints>::New();
    visPts->SetInputConnection(ids->GetOutputPort());
    visPts->SetRenderer(renl);
    visPts->SelectionWindowOn();
    visPts->SelectionWindowOn();
    visPts->SetSelection(xmin, xmin+xLength, ymin, ymin+yLength);

    //创建一个映射器用于显示点的Id集，并指定标记的格式
    vtkSmartPointer<vtkLabeledDataMapper> ldm = vtkSmartPointer<vtkLabeledDataMapper>::New();
    ldm->SetInputConnection(visPts->GetOutputPort());
    ldm->SetLabelModeToLabelFieldData();

    vtkSmartPointer<vtkActor2D> pointLabels = vtkSmartPointer<vtkActor2D>::New();
    pointLabels->SetMapper(ldm);

    //创建单元标记
    vtkSmartPointer<vtkCellCenters> cc = vtkSmartPointer<vtkCellCenters>::New();
    cc->SetInputConnection(ids->GetOutputPort());

    visCells = vtkSmartPointer<vtkSelectVisiblePoints>::New();
    visCells->SetInputConnection(cc->GetOutputPort());
    visCells->SetRenderer(renl);
    visCells->SelectionWindowOn();
    visCells->SetSelection(xmin, xmin+xLength, ymin, ymin+yLength);

    //创建一个映射器用于显示单元的Id集，并指定其标记的格式
    vtkSmartPointer<vtkLabeledDataMapper> cellMapper = vtkSmartPointer<vtkLabeledDataMapper>::New();
    cellMapper->SetInputConnection(visCells->GetOutputPort());
    //设置用于标记的数据域
    cellMapper->SetLabelModeToLabelFieldData();
    cellMapper->GetLabelTextProperty()->SetColor(0,1,0);

    vtkSmartPointer<vtkActor2D> cellLabels = vtkSmartPointer<vtkActor2D>::New();
    cellLabels->SetMapper(cellMapper);
    
    //创建一个渲染窗口及交换渲染窗口
    renWin = vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer(renl);

    vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();

    //为渲染器添加角色，设置渲染背景色、渲染窗口的尺寸
    renl->AddActor(sphereActor);
    renl->AddActor2D(rectActor);
    renl->AddActor2D(pointLabels);
    renl->AddActor2D(cellLabels);

    renl->SetBackground(1,1,1);
    renWin->SetSize(500,500);
    renWin->Render();

    //通过数据集移动选择的窗口
    MoveWindow();

    //／输入被选择渲染窗口的中心，即xmin=ymin=200, xLength=yLength=100
    PlaceWindow(xmin, ymin);

    iren->Initialize();
    iren->Start();

    return 0;
}