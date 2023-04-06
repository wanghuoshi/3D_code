// �����-��������

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

    //���洴����������֮ǰ��ȡ������������棬ÿ����ʹ�õ�������ͼ��ͬ�������ɫ��ͬ

    //���ȴ���һ���ҶȲ�ѯ��
    vtkNew<vtkLookupTable> bwLut;
    bwLut->SetTableRange(0,2000);
    bwLut->SetSaturationRange(0,0);
    bwLut->SetHueRange(0,0);
    bwLut->SetValueRange(0,1);
    bwLut->Build();

    //���Ŵ���һ����������ɫ���Ĳ�ѯ��
    vtkNew<vtkLookupTable> hueLut;
    hueLut->SetTableRange(0,2000);
    hueLut->SetHueRange(0,1);
    hueLut->SetSaturationRange(1,1);
    hueLut->SetValueRange(1,1);
    hueLut->Build();

    //��󴴽�һ�����е�һɫ���������ͶȲ�ͬ�Ĳ�ѯ��
    vtkNew<vtkLookupTable> satLut;
    satLut->SetTableRange(0,2000);
    satLut->SetHueRange(.6,.6);
    satLut->SetSaturationRange(0,1);
    satLut->SetValueRange(1,1);
    satLut->Build();

    //������һ��ƽ�档������vtkImageMapToColorsͨ�����洴���Ĳ�ѯ������ݽ���ӳ��
    //vtkImageActor��vtkProp���ͣ���ʹ������ӳ����ʾͼ�񣬽���ܿ�
    //��ע�⣺�������ݵ�ֵ���ͱ�����unsigned char����������vtkImageMapToColors
    //������ֵ���ͣ���ͬʱע��ָ����ʾ��Χ����ˮ�߻�����Χ�ڵ����ݣ�
    //����vtkImageMapToColorsֻ����һ��������Ƭ
    vtkNew<vtkImageMapToColors> saggitalColors;
    saggitalColors->SetInputConnection(v16->GetOutputPort());
    saggitalColors->SetLookupTable(bwLut);
    vtkNew<vtkImageActor> saggital;
    saggital->SetInputData(saggitalColors->GetOutput());
    //������ʾ�Ĺ�״����
    saggital->SetDisplayExtent(32,32,0,63,0,92);

    //�����ڶ���ƽ�棬��֮ǰ�Ĳ���һ����������ʾ��Χ��ͬ��
    vtkNew<vtkImageMapToColors> axialColors;
    axialColors->SetInputConnection(v16->GetOutputPort());
    axialColors->SetLookupTable(hueLut);
    vtkNew<vtkImageActor> axial;
    axial->SetInputData(axialColors->GetOutput());
    //������ʾ�ĺ����
    axial->SetDisplayExtent(0,63,0,63,46,46);

    //����������ƽ�档��֮ǰ�Ĳ���һ����������ʾ��Χ��ͬ��
    vtkNew<vtkImageMapToColors> coronalColors;
    coronalColors->SetInputConnection(v16->GetOutputPort());
    coronalColors->SetLookupTable(satLut);
    vtkNew<vtkImageActor> coronal;
    coronal->SetInputData(coronalColors->GetOutput());
    //������ʾ��ʸ״��
    coronal->SetDisplayExtent(0,63,32,32,0,92);

    //����һ����������������ӽǡ������λ�õ�
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

    //�رչ�ͷ����ʾ
    bone->VisibilityOff();

    //Ƥ����Ϊ��͸��״̬���Ա�۲�������Ч��
    skin->GetProperty()->SetOpacity(0.5);

    aRenderer->SetActiveCamera(aCamera);
    //aRenderer->Render();
    aRenderer->ResetCamera();
    aCamera->Dolly(1.5);    //������������㣬�ԷŴ�ͼ��
    aRenderer->SetBackground(1,1,1);
    renWin->SetSize(640,480);
    aRenderer->ResetCameraClippingRange();    //��������ļ��з�Χ

    //��������
    iren->Initialize();
    iren->Start();

    return EXIT_SUCCESS;
}
