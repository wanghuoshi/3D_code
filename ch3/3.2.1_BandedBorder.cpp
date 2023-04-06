// ��ͼ����Ӵ�״��Ե����

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
    double scalarRange[2];    //���ڱ����������

    //��ȡ�ļ�����ø��ļ��ı�����Χ
    vtkNew<vtkXMLPolyDataReader> reader;
    reader->SetFileName("D:/VTK/VTK-9.2.2-data/Testing/Data/filledContours.vtp");
    reader->Update();    //���������ڴ棬�Ա��ȡ������Χ
    reader->GetOutput()->GetPointData()->GetScalars()->GetRange(scalarRange);

    int numberOfContours = 9;

    //��������������������Ϊ�����ҽ���ͬ�Ĳ�����Ϊ��״����
    vtkNew<vtkBandedPolyDataContourFilter> bandedContours;
    bandedContours->SetInputConnection(reader->GetOutputPort());
    bandedContours->SetScalarModeToValue();    //������ȡ��ʽ
    bandedContours->GenerateContourEdgesOn();    //����������Ե
    //����������������ȡֵ��Χ
    bandedContours->GenerateValues(numberOfContours, scalarRange[0], scalarRange[1]);

    //����ӳ��������ӳ����ɫ�Ĳ�ѯ��
    vtkNew<vtkLookupTable> lut;
    lut->SetNumberOfTableValues(numberOfContours+1);    //ָ����ѯ�����ɫ����
    lut->Build();    //��ɫ�������Ͷȡ����ּ���ĸ�Ĵ�Сֵ�����ɲ�ѯ��

    vtkNew<vtkPolyDataMapper> contourMapper;
    contourMapper->SetInputConnection(bandedContours->GetOutputPort());
    //���ñ����ķ�Χ
    contourMapper->SetScalarRange(scalarRange[0], scalarRange[1]);
    contourMapper->SetScalarModeToUseCellData();
    contourMapper->SetLookupTable(lut);    //���ò�ѯ��

    vtkNew<vtkActor> contourActor;
    contourActor->SetMapper(contourMapper);
    contourActor->GetProperty()->SetInterpolationToFlat();    //���ý�ɫ��д��Ӱ
    
    vtkNew<vtkPolyDataMapper> contourLineMapper;
    contourLineMapper->SetInputData(bandedContours->GetContourEdgesOutput());
    contourLineMapper->SetScalarRange(scalarRange[0], scalarRange[1]);
    contourLineMapper->ScalarVisibilityOff();    //���ñ��������ڲ�����ɫ

    vtkNew<vtkActor> contourLineActor;
    contourLineActor->SetMapper(contourLineMapper);
    contourLineActor->GetProperty()->SetLineWidth(2);    //����ɫ�������Ŀ��

    //������Ⱦ������Ⱦ���ڡ���Ⱦ��������
    vtkNew<vtkRenderer> renl;
    vtkNew<vtkRenderWindow> renWin;
    vtkNew<vtkRenderWindowInteractor> iren;

    renl->SetBackground(.1,.2,.3);
    renWin->AddRenderer(renl);
    iren->SetRenderWindow(renWin);

    //����Ⱦ����ֵ
    renl->AddActor(contourActor);
    renl->AddActor(contourLineActor);

    //��������
    renWin->Render();
    iren->Start();

    return EXIT_SUCCESS;
}