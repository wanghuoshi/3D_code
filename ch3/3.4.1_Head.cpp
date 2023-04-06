// �����-����һ����ͷ

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

    //��ȡά����Ƭ����������ߴ硢��Χ������ά�������ϵļ��
    vtkNew<vtkVolume16Reader> v16;
    v16->SetDataDimensions(64,64);
    v16->SetImageRange(1,93);
    v16->SetDataByteOrderToLittleEndian();
    v16->SetFilePrefix("D:/VTK/VTK-9.2.2-data/Testing/Data/headsq/quarter");
    v16->SetDataSpacing(3.2,3.2,1.5);

    //��������vtkVolumeRayCastMapper�Ĺ��ߺ���vtkVolumeRayCompositeFunction
    vtkNew<vtkFixedPointVolumeRayCastMapper> volumeMapper;
    volumeMapper->SetInputConnection(v16->GetOutputPort());
    
    //������ɫת������vtkColorTransferFunction�����ڽ����ص�ǿ�ҳ̶�ת���ɲ�ͬ����ɫ
    vtkNew<vtkColorTransferFunction> volumeColor;
    volumeColor->AddRGBPoint(0,   0.0,0.0,0.0);    //������ɫ
    volumeColor->AddRGBPoint(500, 1.0,0.5,0.3);    //����Ƥ������ɫ
    volumeColor->AddRGBPoint(1000,1.0,0.5,0.3);    //����ת����������ɫ
    volumeColor->AddRGBPoint(1150,1.0,1.0,0.9);    //����ת���Ĺ�ͷ��ɫ

    //�����ֶκ��������ò�ͬ��֯��͸����
    vtkNew<vtkPiecewiseFunction> volumeScalarOpacity;
    volumeScalarOpacity->AddPoint(0   ,0.00);
    volumeScalarOpacity->AddPoint(500 ,0.15);
    volumeScalarOpacity->AddPoint(1000,0.15);
    volumeScalarOpacity->AddPoint(1150,0.85);

    //ͬ�ϣ����ò�ͬ�¶ȵ�͸����
    vtkNew<vtkPiecewiseFunction> volumeGradientOpacity;
    volumeGradientOpacity->AddPoint(0  ,0.0);
    volumeGradientOpacity->AddPoint(90 ,0.5);
    volumeGradientOpacity->AddPoint(100,1.0);

    //����vtkVolumeProperty������ɫ��͸������ϵ����
    vtkNew<vtkVolumeProperty> volumeProperty;
    volumeProperty->SetColor(volumeColor);
    volumeProperty->SetScalarOpacity(volumeScalarOpacity);
    volumeProperty->SetGradientOpacity(volumeGradientOpacity);
    //���ò������ݵĲ�������
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->ShadeOn();    //���������ֽ��м���
    volumeProperty->SetAmbient(0.4);
    volumeProperty->SetDiffuse(0.6);
    volumeProperty->SetSpecular(0.2);

    vtkNew<vtkVolume> volume;
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);

    //�������ݼ��ŵ�����Ⱦ����
    ren->AddViewProp(volume);

    //�������
    vtkCamera *camera = ren->GetActiveCamera();
    double *c = volume->GetCenter();
    camera->SetFocalPoint(c[0], c[1], c[2]);
    camera->SetPosition(c[0] + 400, c[1], c[2]);
    camera->SetViewUp(0,0,-1);

    renWin->SetSize(640, 480);

    //������Ⱦ����
    iren->Initialize();
    iren->Start();

    return EXIT_SUCCESS;
}