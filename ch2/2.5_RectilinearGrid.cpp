#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkFloatArray.h"
#include "vtkRectilinearGrid.h"
#include "vtkRectilinearGridGeometryFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkAutoInit.h" 

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

int main()
{
    int i;

    //三个数组指定了网格在三个坐标轴的稀疏程度（间隔）
    static float x[47] = {
        -1.22396, -1.17188, -1.11979, -1.06771, -1.01562, -0.963542,
        -0.911458, -0.859375, -0.807282, -0.755208, -0.703125, -0.651042,
        -0.598958, -0.546875, -0.494792, -0.442708, -0.390625, -0.338542,
        -0.286458, -0.234375, -0.182292, -0.130209, -0.078125, -0.026042,
        0.0260415, 0.078125, 0.130208, 0.182291, 0.234375, 0.286458,
        0.338542, 0.390625, 0.442708, 0.494792, 0.546875, 0.598958,
        0.651042, 0.703125, 0.755208, 0.807292, 0.859375, 0.911458,
        0.963542, 1.01562, 1.06771, 1.11979, 1.17188
    };
    static float y[33] = {
        -1.25, -1.17188, -1.09375, -1.01562, -0.9375, -0.859375,
        -0.78125, -0.703125, -0.625, -0.546875, -0.46875, -0.390625,
        -0.3125, -0.234375, -0.15625, -0.078125, 0, 0.078125,
        0.15625, 0.234375, 0.3125, 0.390625, 0.46875, 0.546875,
        0.625, 0.703125, 0.78125, 0.859375, 0.9375, 1.01562,
        1.09375, 1.17188, 1.25
    };
    static float z[44] = {
        0, 0.1, 0.2, 0.3, 0.4, 0.5,
        0.6, 0.7, 0.75, 0.8, 0.9, 1,
        1.1, 1.2, 1.3, 1.4, 1.5, 1.6,
        1.7, 1.75, 1.8, 1.9, 2, 2.1,
        2.2, 2.3, 2.4, 2.5, 2.6, 2.7,
        2.75, 2.8, 2.9, 3, 3.1, 3.2,
        3.3, 3.4, 3.5, 3.6, 3.7, 3.75,
        3.8, 3.9
    };

    //定义三个数组作为X、Y、Z轴坐标系
    vtkSmartPointer<vtkFloatArray> xCoords = vtkSmartPointer<vtkFloatArray>::New();
    for (i = 0; i < 47; i++) xCoords->InsertNextValue(x[i]);

    vtkSmartPointer<vtkFloatArray> yCoords = vtkSmartPointer<vtkFloatArray>::New();
    for (i = 0; i < 33; i++) yCoords->InsertNextValue(y[i]);

    vtkSmartPointer<vtkFloatArray> zCoords = vtkSmartPointer<vtkFloatArray>::New();
    for (i = 0; i < 44; i++) zCoords->InsertNextValue(z[i]);

    //创建一个基于矩形结构的几何对象，该结构在X、Y、Z轴方向上变化的间隔遵循拓扑规则（等距）
    vtkSmartPointer<vtkRectilinearGrid> rgrid = vtkSmartPointer<vtkRectilinearGrid>::New();
    rgrid->SetDimensions(47, 33, 44);
    rgrid->SetXCoordinates(xCoords);
    rgrid->SetYCoordinates(yCoords);
    rgrid->SetZCoordinates(zCoords);

    //从矩形网格中提取一个面，作为显示和分析的对象
    vtkSmartPointer<vtkRectilinearGridGeometryFilter> plane =
        vtkSmartPointer<vtkRectilinearGridGeometryFilter>::New();
    plane->SetInputData(rgrid);
    plane->SetExtent(0, 46, 16, 16, 0, 43);    //设置提取范围

    vtkSmartPointer<vtkPolyDataMapper> rgridMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    rgridMapper->SetInputConnection(plane->GetOutputPort());

    vtkSmartPointer<vtkActor> wireActor = vtkSmartPointer<vtkActor>::New();
    wireActor->SetMapper(rgridMapper);
    wireActor->GetProperty()->SetRepresentationToWireframe();
    wireActor->GetProperty()->SetColor(0, 1, 0);    //设置网格颜色

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(wireActor);
    renderer->SetBackground(0, 0, 0);    //设置背景色
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Elevation(60.0);
    renderer->GetActiveCamera()->Azimuth(30.0);
    renderer->GetActiveCamera()->Zoom(1.0);

    vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer(renderer);
    renWin->SetSize(300, 300);

    vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    iren->SetRenderWindow(renWin);

    iren->Initialize();
    iren->Start();

    return 0;
}