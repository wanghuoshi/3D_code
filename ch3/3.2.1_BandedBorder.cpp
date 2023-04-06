// 给图像添加带状边缘范例

#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkBandedPolyDataContourFilter.h>
#include <vtkFloatArray.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkScalarsToColors.h>
#include <vtkLookupTable.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include "vtkAutoInit.h" 

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

int main()
{
    double scalarRange[2];    //用于保存标量数据

    //读取文件并获得该文件的标量范围
    vtkNew<vtkXMLPolyDataReader> reader;
    reader->SetFileName("D:/VTK/VTK-9.2.2-data/Testing/Data/filledContours.vtp");
    reader->Update();    //立即分配内存，以便获取标量范围
    reader->GetOutput()->GetPointData()->GetScalars()->GetRange(scalarRange);

    int numberOfContours = 9;

    //创建过滤器，将以数据为标量且将相同的部分设为条状轮廓
    vtkNew<vtkBandedPolyDataContourFilter> bandedContours;
    bandedContours->SetInputConnection(reader->GetOutputPort());
    bandedContours->SetScalarModeToValue();    //设置提取格式
    bandedContours->GenerateContourEdgesOn();    //产生轮廓边缘
    //设置轮廓的数量及取值范围
    bandedContours->GenerateValues(numberOfContours, scalarRange[0], scalarRange[1]);

    //创建映射器用于映射颜色的查询表
    vtkNew<vtkLookupTable> lut;
    lut->SetNumberOfTableValues(numberOfContours+1);    //指定查询表的颜色数量
    lut->Build();    //从色调、饱和度、数字及字母的大小值中生成查询表

    vtkNew<vtkPolyDataMapper> contourMapper;
    contourMapper->SetInputConnection(bandedContours->GetOutputPort());
    //设置标量的范围
    contourMapper->SetScalarRange(scalarRange[0], scalarRange[1]);
    contourMapper->SetScalarModeToUseCellData();
    contourMapper->SetLookupTable(lut);    //设置查询表

    vtkNew<vtkActor> contourActor;
    contourActor->SetMapper(contourMapper);
    contourActor->GetProperty()->SetInterpolationToFlat();    //设置角色填写阴影
    
    vtkNew<vtkPolyDataMapper> contourLineMapper;
    contourLineMapper->SetInputData(bandedContours->GetContourEdgesOutput());
    contourLineMapper->SetScalarRange(scalarRange[0], scalarRange[1]);
    contourLineMapper->ScalarVisibilityOff();    //设置标量不用于产生颜色

    vtkNew<vtkActor> contourLineActor;
    contourLineActor->SetMapper(contourLineMapper);
    contourLineActor->GetProperty()->SetLineWidth(2);    //设置色彩轮廓的宽度

    //创建渲染器、渲染窗口、渲染交互窗口
    vtkNew<vtkRenderer> renl;
    vtkNew<vtkRenderWindow> renWin;
    vtkNew<vtkRenderWindowInteractor> iren;

    renl->SetBackground(.1,.2,.3);
    renWin->AddRenderer(renl);
    iren->SetRenderWindow(renWin);

    //给渲染器赋值
    renl->AddActor(contourActor);
    renl->AddActor(contourLineActor);

    //启动交互
    renWin->Render();
    iren->Start();

    return EXIT_SUCCESS;
}