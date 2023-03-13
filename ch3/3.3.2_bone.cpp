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

void main()
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
    skin->GetProperty()->SetOpacity(.5);    //设置透明度

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

    //创建一个照相机，设置其视角、焦点和位置等
    vtkNew<vtkCamera> aCamera;
    aCamera->SetViewUp(0,0,-1);
    aCamera->SetPosition(0,1,0);
    aCamera->SetFocalPoint(0,0,0);
    aCamera->ComputeViewPlaneNormal();
    //设置摄像头旋转穿过物体时的角度
    aCamera->Azimuth(30.0);
    aCamera->Elevation(30.0);

    aRenderer->AddActor(outline);
    aRenderer->AddActor(skin);
    aRenderer->AddActor(bone);
    aRenderer->SetActiveCamera(aCamera);
    aRenderer->ResetCamera();
    aRenderer->SetBackground(.2,.3,.4);
    aCamera->Dolly(1.5);    //将相机靠近焦点，以放大图像

    aRenderer->ResetCameraClippingRange();

    vtkNew<vtkInteractorStyleTrackballCamera> style;
    iren->SetInteractorStyle(style);

    //启动交互
    iren->Initialize();
    iren->Start();
}
