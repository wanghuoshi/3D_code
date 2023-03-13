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

    //��ȡ��ά��Ƭ
    vtkNew<vtkVolume16Reader> v16;
    v16->SetDataDimensions(64,64);    //���ö�ȡ��Ƭ�ĳߴ�
    v16->SetDataByteOrderToLittleEndian();
    v16->SetFilePrefix("D:/VTK/VTK-9.2.2-data/Testing/Data/headsq/quarter");
    v16->SetImageRange(1,93);    //���ö�ȡ����Ƭ�ķ�Χ
    v16->SetDataSpacing(3.2,3.2,1.5);    //����ά�������ϵļ��

    //��Ƥ���ϳ�ȡֵΪ500�ĵ�ֵ��
    vtkNew<vtkContourFilter> skinExtractor;
    skinExtractor->SetInputConnection(v16->GetOutputPort());
    skinExtractor->SetValue(0,500);

    //�ڵ�ֵ���ϲ���������
    vtkNew<vtkPolyDataNormals> skinNormals;
    skinNormals->SetInputConnection(skinExtractor->GetOutputPort());
    skinNormals->SetFeatureAngle(60.0);

    //���˵�����Ҫ�����ݵ�Ԫ����ֹ��Ч�ľɵ�Ԫ����
    vtkNew<vtkStripper> skinStripper;
    skinStripper->SetInputConnection(skinNormals->GetOutputPort());

    vtkNew<vtkPolyDataMapper> skinMapper;
    skinMapper->SetInputConnection(skinNormals->GetOutputPort());
    skinMapper->ScalarVisibilityOff();

    vtkNew<vtkActor> skin;
    skin->SetMapper(skinMapper);
    skin->GetProperty()->SetDiffuseColor(1,.49,.25);    //�����������ϵ��
    skin->GetProperty()->SetSpecular(.3);    //���þ������ϵ��
    skin->GetProperty()->SetSpecularPower(20);    //���þ���ָ��
    skin->GetProperty()->SetOpacity(.5);    //����͸����

    //��ȡ��ֵ��Ĺ�ͷ��ע��ͬ��
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

    //Ϊ���������ṩһ����Χ�߿�
    vtkNew<vtkOutlineFilter> outlineData;
    outlineData->SetInputConnection(v16->GetOutputPort());

    vtkNew<vtkPolyDataMapper> mapOutline;
    mapOutline->SetInputConnection(outlineData->GetOutputPort());

    vtkNew<vtkActor> outline;
    outline->SetMapper(mapOutline);
    outline->GetProperty()->SetColor(0,0,0);

    //����һ����������������ӽǡ������λ�õ�
    vtkNew<vtkCamera> aCamera;
    aCamera->SetViewUp(0,0,-1);
    aCamera->SetPosition(0,1,0);
    aCamera->SetFocalPoint(0,0,0);
    aCamera->ComputeViewPlaneNormal();
    //��������ͷ��ת��������ʱ�ĽǶ�
    aCamera->Azimuth(30.0);
    aCamera->Elevation(30.0);

    aRenderer->AddActor(outline);
    aRenderer->AddActor(skin);
    aRenderer->AddActor(bone);
    aRenderer->SetActiveCamera(aCamera);
    aRenderer->ResetCamera();
    aRenderer->SetBackground(.2,.3,.4);
    aCamera->Dolly(1.5);    //������������㣬�ԷŴ�ͼ��

    aRenderer->ResetCameraClippingRange();

    vtkNew<vtkInteractorStyleTrackballCamera> style;
    iren->SetInteractorStyle(style);

    //��������
    iren->Initialize();
    iren->Start();
}
