// 面绘制-三正交面

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkVolume16Reader.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkOutlineFilter.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkPolyDataNormals.h>
#include <vtkContourFilter.h>
#include <vtkSmartPointer.h>
#include <vtkStripper.h>
#include <vtkContourFilter.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkImageActor.h>

#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

int main()
{
    vtkNew<vtkRenderer> aRenderer;
    vtkNew<vtkRenderWindow> renWin;
    renWin->AddRenderer(aRenderer);

    vtkNew<vtkRenderWindowInteractor> iren;
    iren->SetRenderWindow(renWin);

    //读取三维切片
    vtkNew<vtkVolume16Reader> v16;
    v16->SetDataDimensions(64,64);    //设置读取切片的尺寸
    v16->SetDataByteOrderToLittleEndian();
    v16->SetFilePrefix("D:/VTK/VTK-9.2.2-data/Testing/Data/headsq/quarter"); 
    v16->SetImageRange(1,93);    //设置读取的切片的范围
    v16->SetDataSpacing(3.2,3.2,1.5);    //在三维坐标轴上的间隔

    //在皮肤上抽取值为500的等值面
    vtkNew<vtkContourFilter> skinExtractor;
    skinExtractor->SetInputConnection(v16->GetOutputPort());
    skinExtractor->SetValue(0,500);

    //在等值面上产生法向量
    vtkNew<vtkPolyDataNormals> skinNormals;
    skinNormals->SetInputConnection(skinExtractor->GetOutputPort());
    skinNormals->SetFeatureAngle(60.0);

    //过滤掉不需要的数据单元，防止无效的旧单元存在
    vtkNew<vtkStripper> skinStripper;
    skinStripper->SetInputConnection(skinNormals->GetOutputPort());
    vtkNew<vtkPolyDataMapper> skinMapper;
    skinMapper->SetInputConnection(skinNormals->GetOutputPort());
    skinMapper->ScalarVisibilityOff();
    vtkNew<vtkActor> skin;
    skin->SetMapper(skinMapper);
    skin->GetProperty()->SetDiffuseColor(1,.49,.25);    //设置漫反射光系数
    skin->GetProperty()->SetSpecular(.3);    //设置镜反射广系数
    skin->GetProperty()->SetSpecularPower(20);    //设置镜面指数

    //抽取等值面的骨头，注释同上
    vtkNew<vtkContourFilter> boneExtractor;
    boneExtractor->SetInputConnection(v16->GetOutputPort());
    boneExtractor->SetValue(0,1150);
    vtkNew<vtkPolyDataNormals> boneNormals;
    boneNormals->SetInputConnection(boneExtractor->GetOutputPort());
    boneNormals->SetFeatureAngle(60.0);
    vtkNew<vtkStripper> boneStripper;
    boneStripper->SetInputConnection(boneNormals->GetOutputPort());
    vtkNew<vtkPolyDataMapper> boneMapper;
    boneMapper->SetInputConnection(boneStripper->GetOutputPort());
    boneMapper->ScalarVisibilityOff();
    vtkNew<vtkActor> bone;
    bone->SetMapper(boneMapper);
    bone->GetProperty()->SetDiffuseColor(1,1,.9412);

    //为上述内容提供一个外围边框
    vtkNew<vtkOutlineFilter> outlineData;
    outlineData->SetInputConnection(v16->GetOutputPort());
    vtkNew<vtkPolyDataMapper> mapOutline;
    mapOutline->SetInputConnection(outlineData->GetOutputPort());
    vtkNew<vtkActor> outline;
    outline->SetMapper(mapOutline);
    outline->GetProperty()->SetColor(0,0,0);

    //下面创建三个穿过之前读取的立体的正交面，每个面使用的纹理贴图不同，因而着色不同

    //首先创建一个灰度查询表
    vtkNew<vtkLookupTable> bwLut;
    bwLut->SetTableRange(0,2000);
    bwLut->SetSaturationRange(0,0);
    bwLut->SetHueRange(0,0);
    bwLut->SetValueRange(0,1);
    bwLut->Build();

    //接着创建一个包含所有色调的查询表
    vtkNew<vtkLookupTable> hueLut;
    hueLut->SetTableRange(0,2000);
    hueLut->SetHueRange(0,1);
    hueLut->SetSaturationRange(1,1);
    hueLut->SetValueRange(1,1);
    hueLut->Build();

    //最后创建一个具有单一色调，但饱和度不同的查询表
    vtkNew<vtkLookupTable> satLut;
    satLut->SetTableRange(0,2000);
    satLut->SetHueRange(.6,.6);
    satLut->SetSaturationRange(0,1);
    satLut->SetValueRange(1,1);
    satLut->Build();

    //创建第一个平面。过滤器vtkImageMapToColors通过上面创建的查询表对数据进行映射
    //vtkImageActor是vtkProp类型，它使用纹理映射显示图像，结果很快
    //（注意：输入数据的值类型必须是unsigned char，这正好是vtkImageMapToColors
    //产生的值类型），同时注意指定显示范围，流水线会请求范围内的数据，
    //并且vtkImageMapToColors只处理一个数据切片
    vtkNew<vtkImageMapToColors> saggitalColors;
    saggitalColors->SetInputConnection(v16->GetOutputPort());
    saggitalColors->SetLookupTable(bwLut);
    vtkNew<vtkImageActor> saggital;
    saggital->SetInputData(saggitalColors->GetOutput());
    //设置显示的冠状画面
    saggital->SetDisplayExtent(32,32,0,63,0,92);

    //创建第二个平面，与之前的操作一样，除了显示范围不同外
    vtkNew<vtkImageMapToColors> axialColors;
    axialColors->SetInputConnection(v16->GetOutputPort());
    axialColors->SetLookupTable(hueLut);
    vtkNew<vtkImageActor> axial;
    axial->SetInputData(axialColors->GetOutput());
    //设置显示的横断面
    axial->SetDisplayExtent(0,63,0,63,46,46);

    //创建第三个平面。与之前的操作一样，除了显示范围不同外
    vtkNew<vtkImageMapToColors> coronalColors;
    coronalColors->SetInputConnection(v16->GetOutputPort());
    coronalColors->SetLookupTable(satLut);
    vtkNew<vtkImageActor> coronal;
    coronal->SetInputData(coronalColors->GetOutput());
    //设置显示的矢状面
    coronal->SetDisplayExtent(0,63,32,32,0,92);

    //创建一个照相机，设置其视角、焦点和位置等
    vtkNew<vtkCamera> aCamera;
    aCamera->SetViewUp(0,0,-1);
    aCamera->SetPosition(0,1,0);
    aCamera->SetFocalPoint(0,0,0);
    aCamera->ComputeViewPlaneNormal();

    aRenderer->AddActor(outline);
    aRenderer->AddActor(saggital);
    // aRenderer->AddActor(axial);
    // aRenderer->AddActor(coronal);
    // aRenderer->AddActor(axial);
    // aRenderer->AddActor(coronal);
    aRenderer->AddActor(skin);
    aRenderer->AddActor(bone);

    //关闭骨头的演示
    bone->VisibilityOff();

    //皮肤设为半透明状态，以便观察三截面效果
    skin->GetProperty()->SetOpacity(0.5);

    aRenderer->SetActiveCamera(aCamera);
    //aRenderer->Render();
    aRenderer->ResetCamera();
    aCamera->Dolly(1.5);    //将相机靠近焦点，以放大图像
    aRenderer->SetBackground(1,1,1);
    renWin->SetSize(640,480);
    aRenderer->ResetCameraClippingRange();    //重设相机的剪切范围

    //启动交互
    iren->Initialize();
    iren->Start();

    return EXIT_SUCCESS;
}
