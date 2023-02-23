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

int main()
{
    int i;

    //创建一个用于存放点集的VTK浮点型数组
    vtkFloatArray *pcoords = vtkFloatArray::New();

    //指定该数组用于存放三维的点
    pcoords->SetNumberOfComponents(3);
    //设置数组元组个数
    pcoords->SetNumberOfTuples(4);

    float pts[4][3] = {{0.0,0.0,0.0},{0.0,1.0,0.0},
                       {1.0,0.0,0.0},{1.0,1.0,0.0}};
    //用上述二维数组初始化点集数组
    for(i = 0; i < 4; i++)
    {
        pcoords->SetTuple(i,pts[i]);
    }

    //创建点集并初始化
    vtkPoints *points = vtkPoints::New();
    points->SetData(pcoords);

    //创建构成多边形数据集的“积木”
    vtkCellArray *strips = vtkCellArray::New();
    strips->InsertNextCell(4);                   //设置数组存放点的个数
    //设置点与点之间的连接关系
    strips->InsertCellPoint(0);
    strips->InsertCellPoint(1);
    strips->InsertCellPoint(2);
    strips->InsertCellPoint(3);
    
    //该数组用于后面的标量信息
    vtkIntArray *temperature = vtkIntArray::New();
    temperature->SetName("Temperature");          //设置数组名
    //在数组的后面添加数据
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

    //创建多边形数据集，并将“积木”组成多边形数据
    vtkPolyData *polydata = vtkPolyData::New();
    polydata->SetPoints(points);
    polydata->SetStrips(strips);
    //设置多边形数据集的标量值
    polydata->GetPointData()->SetScalars(temperature);
    polydata->GetPointData()->AddArray(vorticity);

    //设置映射器和标量范围
    vtkPolyDataMapper *mapper =vtkPolyDataMapper::New();
    mapper->SetInputData(polydata);
    mapper->SetScalarRange(0,40);                 //设置映射范围

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

    //释放对象内存
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