// ��ͼ����ӳ�����Ե

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
    double scalarRange[2];    //���ڱ����������

    //��ȡ�ļ�����ø��ļ��ı�����Χ
    vtkNew<vtkXMLPolyDataReader> reader;
    reader->SetFileName("D:/VTK/VTK-9.2.2-data/Testing/Data/filledContours.vtp");
    reader->Update();    //���������ڴ棬�Ա��ȡ������Χ
    reader->GetOutput()->GetPointData()->GetScalars()->GetRange(scalarRange);

    vtkNew<vtkAppendPolyData> appendFilledContours;

    int numberOfContours = 9;
    double delta = (scalarRange[1] - scalarRange[0]) / static_cast<double>(numberOfContours - 1);    //���ü��

    //�����޼��������ı���ֵ
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
        clippersLow[i]->InsideOutOff();    //���߽�������ȥ
        clippersLow[i]->Update();

        clippersHigh.push_back(vtkSmartPointer<vtkClipPolyData>::New());
        clippersHigh[i]->SetValue(valueHigh);
        clippersHigh[i]->SetInputConnection(clippersLow[i]->GetOutputPort());
        clippersHigh[i]->GenerateClippedOutputOn();
        clippersHigh[i]->InsideOutOn();    //�������߽��
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

    //����ӳ��������ӳ����ɫ�Ĳ�ѯ��
    vtkNew<vtkLookupTable> lut;
    lut->SetNumberOfTableValues(numberOfContours+1);    //ָ����ѯ�����ɫ����
    lut->Build();    //��ɫ�������Ͷȡ����ּ���ĸ�Ĵ�Сֵ�����ɲ�ѯ��

    vtkNew<vtkPolyDataMapper> contourMapper;
    contourMapper->SetInputConnection(filledContours->GetOutputPort());
    //���ñ����ķ�Χ
    contourMapper->SetScalarRange(scalarRange[0], scalarRange[1]);
    contourMapper->SetScalarModeToUseCellData();
    contourMapper->SetLookupTable(lut);    //���ò�ѯ��

    vtkNew<vtkActor> contourActor;
    contourActor->SetMapper(contourMapper);
    contourActor->GetProperty()->SetInterpolationToFlat();    //���ý�ɫ��д��Ӱ

    vtkNew<vtkContourFilter> contours;
    contours->SetInputConnection(filledContours->GetOutputPort());
    contours->GenerateValues(numberOfContours, scalarRange[0], scalarRange[1]);
    
    vtkNew<vtkPolyDataMapper> contourLineMapper;
    contourLineMapper->SetInputConnection(contours->GetOutputPort());
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