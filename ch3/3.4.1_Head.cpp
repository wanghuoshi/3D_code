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

    //��ȡά����Ƭ����������ߴ硢��Χ������ά�������ϵļ��
    vtkNew<vtkVolume16Reader> v16;
    v16->SetDataDimensions(64,64);
    v16->SetImageRange(1,93);
    v16->SetDataByteOrderToLittleEndian();
    v16->SetFilePrefix("D:/VTK/VTK-9.2.2-data/Testing/Data/headsq/quarter");
    v16->SetDataSpacing(3.2,3.2,1.5);

    //��������vtkVolumeRayCastMapper�Ĺ��ߺ���vtkVolumeRayCompositeFunction
    vtkNew<vtkFixedPointVolumeRayCastMapper> volumeMapper;
    volumeMapper->SetInputData(v16->GetOutput());
    
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
}