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

//����һ�����ڻ����ڵĺ���
void PlaceWindow(int xmin, int ymin)
{
    int xmax = xmin + xLength;
    int ymax = ymin + yLength;

    visPts->SetSelection(xmin, xmax, ymin, ymax);    //ָ��Ҫ��ʾ�ľ�������
    visCells->SetSelection(xmin, xmax, ymin, ymax); 

    pts->InsertPoint(0, xmin, ymin, 0);    //Id��X��Y��Z
    pts->InsertPoint(1, xmin, ymin, 0);
    pts->InsertPoint(2, xmin, ymin, 0);
    pts->InsertPoint(3, xmin, ymin, 0);

    pts->Modified();
    renWin->Render();
}

//����һ�����������ƶ���ѡ��Ĵ���
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
    //������ʾ����ľ��δ���
    int xmin = 200;
    xLength = 100��
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
    rect->InsertNextCell(5);    //ָ�������ĵ�Ԫ��
    rect->InsertCellPoint(0);   //���þ����ĸ�������ӽṹ
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

    //����һ����������ص�ӳ�����ͽ�ɫ
    vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
    vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    sphereMapper->SetInputConnection(sphere->GetOutputPort());
    sphereMapper->GlobalImmediateModelRenderingOn();

    vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
    sphereActor->SetMapper(sphereMapper);

    //ͨ��vtkIdFilter�����㡢��Ԫ��Id��
    vtkSmartPointer<vtkIdFilter> ids = vtkSmartPointer<vtkIdFilter>::New();
    ids->SetInputConnection(sphere->GetOutputPort());
    ids->PointIdsOn();
    ids->CellIdsOn();
    ids->FieldDataOn();

    //ΪvtkSelectVisiblePoints����һ����Ⱦ��
    vtkSmartPointer<vtkRenderer> renl = vtkSmartPointer<vtkRenderer>::New();

    //Ϊ�㴴�����
    visPts = vtkSmartPointer<vtkSelectVisiblePoints>::New();
    visPts->SetInputConnection(ids->GetOutputPort());
    visPts->SetRenderer(renl);
    visPts->SelectionWindowOn();
    visPts->SelectionWindowOn();
    visPts->SetSelection(xmin, xmin+xLength, ymin, ymin+yLength);

    //����һ��ӳ����������ʾ���Id������ָ����ǵĸ�ʽ
    vtkSmartPointer<vtkLabeledDataMapper> ldm = vtkSmartPointer<vtkLabeledDataMapper>::New();
    ldm->SetInputConnection(visPts->GetOutputPort());
    ldm->SetLabelModeToLabelFieldData();

    vtkSmartPointer<vtkActor2D> pointLabels = vtkSmartPointer<vtkActor2D>::New();
    pointLabels->SetMapper(ldm);

    //������Ԫ���
    vtkSmartPointer<vtkCellCenters> cc = vtkSmartPointer<vtkCellCenters>::New();
    cc->SetInputConnection(ids->GetOutputPort());

    visCells = vtkSmartPointer<vtkSelectVisiblePoints>::New();
    visCells->SetInputConnection(cc->GetOutputPort());
    visCells->SetRenderer(renl);
    visCells->SelectionWindowOn();
    visCells->SetSelection(xmin, xmin+xLength, ymin, ymin+yLength);

    //����һ��ӳ����������ʾ��Ԫ��Id������ָ�����ǵĸ�ʽ
    vtkSmartPointer<vtkLabeledDataMapper> cellMapper = vtkSmartPointer<vtkLabeledDataMapper>::New();
    cellMapper->SetInputConnection(visCells->GetOutputPort());
    //�������ڱ�ǵ�������
    cellMapper->SetLabelModeToLabelFieldData();
    cellMapper->GetLabelTextProperty()->SetColor(0,1,0);

    vtkSmartPointer<vtkActor2D> cellLabels = vtkSmartPointer<vtkActor2D>::New();
    cellLabels->SetMapper(cellMapper);
    
    //����һ����Ⱦ���ڼ�������Ⱦ����
    renWin = vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer(renl);

    vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();

    //Ϊ��Ⱦ����ӽ�ɫ��������Ⱦ����ɫ����Ⱦ���ڵĳߴ�
    renl->AddActor(sphereActor);
    renl->AddActor2D(rectActor);
    renl->AddActor2D(pointLabels);
    renl->AddActor2D(cellLabels);

    renl->SetBackground(1,1,1);
    renWin->SetSize(500,500);
    renWin->Render();

    //ͨ�����ݼ��ƶ�ѡ��Ĵ���
    MoveWindow();

    //�����뱻ѡ����Ⱦ���ڵ����ģ���xmin=ymin=200, xLength=yLength=100
    PlaceWindow(xmin, ymin);

    iren->Initialize();
    iren->Start();

    return 0;
}