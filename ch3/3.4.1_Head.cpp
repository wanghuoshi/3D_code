// 体绘制-绘制一个人头

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

#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2)

int main()
{
    vtkNew<vtkRenderer> ren;
    vtkNew<vtkRenderWindow> renWin;
    renWin->AddRenderer(ren);
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
    volumeMapper->SetInputConnection(v16->GetOutputPort());
    
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
    vtkNew<vtkPiecewiseFunction> volumeGradientOpacity;
    volumeGradientOpacity->AddPoint(0  ,0.0);
    volumeGradientOpacity->AddPoint(90 ,0.5);
    volumeGradientOpacity->AddPoint(100,1.0);

    //创建vtkVolumeProperty，将颜色及透明度联系起来
    vtkNew<vtkVolumeProperty> volumeProperty;
    volumeProperty->SetColor(volumeColor);
    volumeProperty->SetScalarOpacity(volumeScalarOpacity);
    volumeProperty->SetGradientOpacity(volumeGradientOpacity);
    //设置插入数据的插入类型
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->ShadeOn();    //对阴暗部分进行计算
    volumeProperty->SetAmbient(0.4);
    volumeProperty->SetDiffuse(0.6);
    volumeProperty->SetSpecular(0.2);

    vtkNew<vtkVolume> volume;
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);

    //将体数据集放到体渲染器中
    ren->AddViewProp(volume);

    //设置相机
    vtkCamera *camera = ren->GetActiveCamera();
    double *c = volume->GetCenter();
    camera->SetFocalPoint(c[0], c[1], c[2]);
    camera->SetPosition(c[0] + 400, c[1], c[2]);
    camera->SetViewUp(0,0,-1);

    renWin->SetSize(640, 480);

    //启动渲染交互
    iren->Initialize();
    iren->Start();

    return EXIT_SUCCESS;
}