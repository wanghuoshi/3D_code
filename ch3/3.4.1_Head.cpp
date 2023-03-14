#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVolume.h>
#include <vtkVolume16Reader.h>
#include <vtkVolumeProperty.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCamera.h>
#include <vtkFixedPointVolumeRayCastMapper.h>

void main()
{
    vtkNew<vtkRenderer> ren;
    vtkNew<vtkRenderWindow> renWin;
    vtkNew<vtkRenderWindowInteractor> iren;
    iren->SetRenderWindow(renWin);

    vtkNew<vtkInteractorStyleTrackballCamera> style;
    iren->SetInteractorStyle(style);

    //读取维序列片，并设置其尺寸、范围及在三维坐标轴上的间隔
    vtkNew<vtkVolume16Reader> v16;
    v16->SetDataDimensions(64,64);
    v16->SetImageRange(1,93);
    v16->SetDataByteOrderToLittleEndian();
    v16->SetFilePrefix("D:/VTK/VTK-9.2.2-data/Testing/Data/headsq/quarter");
    v16->SetDataSpacing(3.2,3.2,1.5);

    //创建用于vtkVolumeRayCastMapper的光线函数vtkVolumeRayCompositeFunction
    vtkNew<vtkFixedPointVolumeRayCastMapper> volumeMapper;
    volumeMapper->SetInputData(v16->GetOutput());
    
    //创建颜色转化函数vtkColorTransferFunction，用于将像素的强烈程度转化成不同的颜色
    vtkNew<vtkColorTransferFunction> volumeColor;
    volumeColor->AddRGBPoint(0,   0.0,0.0,0.0);    //其他颜色
    volumeColor->AddRGBPoint(500, 1.0,0.5,0.3);    //设置皮肤的颜色
    volumeColor->AddRGBPoint(1000,1.0,0.5,0.3);    //设置转化的肉体颜色
    volumeColor->AddRGBPoint(1150,1.0,1.0,0.9);    //设置转化的骨头颜色

    //创建分段函数，设置不同组织的透明度
    vtkNew<vtkPiecewiseFunction> volumeScalarOpacity;
    volumeScalarOpacity->AddPoint(0   ,0.00);
    volumeScalarOpacity->AddPoint(500 ,0.15);
    volumeScalarOpacity->AddPoint(1000,0.15);
    volumeScalarOpacity->AddPoint(1150,0.85);

    //同上，设置不同坡度的透明度
}