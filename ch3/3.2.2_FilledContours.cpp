// 给图像添加充满边缘

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
#include <vtkAppendPolyData.h>
#include <vtkCleanPolyData.h>
#include <vtkClipPolyData.h>
#include <vtkContourFilter.h>
#include <vector>
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

    vtkNew<vtkAppendPolyData> appendFilledContours;

    int numberOfContours = 9;
    double delta = (scalarRange[1] - scalarRange[0]) / static_cast<double>(numberOfContours - 1);    //设置间距

    //保存修剪过滤器的标量值
    std::vector<vtkSmartPointer<vtkClipPolyData>> clippersLow;
    std::vector<vtkSmartPointer<vtkClipPolyData>> clippersHigh;
    for(int i = 0; i < numberOfContours; i++)
    {
        double valueLow  = scalarRange[0] + static_cast<double>(i) * delta;
        double valueHigh = scalarRange[0] + static_cast<double>(i+1) * delta;
        clippersLow.push_back(vtkSmartPointer<vtkClipPolyData>::New());
        clippersLow[i]->SetValue(valueLow);
        if(i == 0)
        {
            clippersLow[i]->SetInputConnection(reader->GetOutputPort());
        }
        else
        {
            clippersLow[i]->SetInputConnection(clippersHigh[i-1]->GetOutputPort(1));
        }
        clippersLow[i]->InsideOutOff();    //将边界点包括进去
        clippersLow[i]->Update();

        clippersHigh.push_back(vtkSmartPointer<vtkClipPolyData>::New());
        clippersHigh[i]->SetValue(valueHigh);
        clippersHigh[i]->SetInputConnection(clippersLow[i]->GetOutputPort());
        clippersHigh[i]->GenerateClippedOutputOn();
        clippersHigh[i]->InsideOutOn();    //不包括边界点
        clippersHigh[i]->Update();
        
        if(clippersHigh[i]->GetOutput()->GetNumberOfCells() == 0) continue;

        vtkNew<vtkFloatArray> cd;
        cd->SetNumberOfComponents(1);
        cd->SetNumberOfTuples(clippersHigh[i]->GetOutput()->GetNumberOfCells());
        cd->FillComponent(0, valueLow);

        clippersHigh[i]->GetOutput()->GetCellData()->SetScalars(cd);
        appendFilledContours->AddInputConnection(clippersHigh[i]->GetOutputPort());
    }

    vtkNew<vtkCleanPolyData> filledContours;
    filledContours->SetInputConnection(appendFilledContours->GetOutputPort());

    //创建映射器用于映射颜色的查询表
    vtkNew<vtkLookupTable> lut;
    lut->SetNumberOfTableValues(numberOfContours+1);    //指定查询表的颜色数量
    lut->Build();    //从色调、饱和度、数字及字母的大小值中生成查询表

    vtkNew<vtkPolyDataMapper> contourMapper;
    contourMapper->SetInputConnection(filledContours->GetOutputPort());
    //设置标量的范围
    contourMapper->SetScalarRange(scalarRange[0], scalarRange[1]);
    contourMapper->SetScalarModeToUseCellData();
    contourMapper->SetLookupTable(lut);    //设置查询表

    vtkNew<vtkActor> contourActor;
    contourActor->SetMapper(contourMapper);
    contourActor->GetProperty()->SetInterpolationToFlat();    //设置角色填写阴影

    vtkNew<vtkContourFilter> contours;
    contours->SetInputConnection(filledContours->GetOutputPort());
    contours->GenerateValues(numberOfContours, scalarRange[0], scalarRange[1]);
    
    vtkNew<vtkPolyDataMapper> contourLineMapper;
    contourLineMapper->SetInputConnection(contours->GetOutputPort());
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