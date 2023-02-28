#include "vtkActor.h"
#include "vtkCellArray.h"
#include "vtkDoubleArray.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkAutoInit.h" 

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

int main()
{
    int i;

    //����һ�����ڴ�ŵ㼯��VTK����������
    vtkFloatArray *pcoords = vtkFloatArray::New();

    //ָ�����������ڴ����ά�ĵ�
    pcoords->SetNumberOfComponents(3);
    //��������Ԫ�����
    pcoords->SetNumberOfTuples(4);

    float pts[4][3] = {{0.0,0.0,0.0},{0.0,1.0,0.0},
                       {1.0,0.0,0.0},{1.0,1.0,0.0}};
    //��������ά�����ʼ���㼯����
    for(i = 0; i < 4; i++)
    {
        pcoords->SetTuple(i,pts[i]);
    }

    //�����㼯����ʼ��
    vtkPoints *points = vtkPoints::New();
    points->SetData(pcoords);

    //�������ɶ�������ݼ��ġ���ľ��
    vtkCellArray *strips = vtkCellArray::New();
    strips->InsertNextCell(4);                   //���������ŵ�ĸ���
    //���õ����֮������ӹ�ϵ
    strips->InsertCellPoint(0);
    strips->InsertCellPoint(1);
    strips->InsertCellPoint(2);
    strips->InsertCellPoint(3);
    
    //���������ں���ı�����Ϣ
    vtkIntArray *temperature = vtkIntArray::New();
    temperature->SetName("Temperature");          //����������
    //������ĺ����������
    temperature->InsertNextValue(10);
    temperature->InsertNextValue(20);
    temperature->InsertNextValue(30);
    temperature->InsertNextValue(40);

    vtkDoubleArray* vorticity = vtkDoubleArray::New();
    vorticity->SetName("Vorticity");
    vorticity->InsertNextValue(2.7);
    vorticity->InsertNextValue(4.1);
    vorticity->InsertNextValue(5.3);
    vorticity->InsertNextValue(3.4);

    //������������ݼ�����������ľ����ɶ��������
    vtkPolyData *polydata = vtkPolyData::New();
    polydata->SetPoints(points);
    polydata->SetStrips(strips);
    //���ö�������ݼ��ı���ֵ
    polydata->GetPointData()->SetScalars(temperature);
    polydata->GetPointData()->AddArray(vorticity);

    //����ӳ�����ͱ�����Χ
    vtkPolyDataMapper *mapper =vtkPolyDataMapper::New();
    mapper->SetInputData(polydata);
    mapper->SetScalarRange(0,40);                 //����ӳ�䷶Χ

    vtkActor *actor = vtkActor::New();
    actor->SetMapper(mapper);

    vtkRenderer *ren = vtkRenderer::New();
    ren->AddActor(actor);

    vtkRenderWindow *renWin = vtkRenderWindow::New();
    renWin->AddRenderer(ren);

    vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
    iren->SetRenderWindow(renWin);
    iren->Initialize();
    iren->Start();

    //�ͷŶ����ڴ�
    pcoords->Delete();
    points->Delete();
    strips->Delete();
    temperature->Delete();
    vorticity->Delete();
    polydata->Delete();
    mapper->Delete();
    actor->Delete();
    ren->Delete();
    renWin->Delete();
    iren->Delete();

    return 0;
}