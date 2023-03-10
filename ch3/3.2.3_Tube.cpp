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
#include <vtkDoubleArray.h>
#include <vtkTubeFilter.h>

#include <vtkMath.h>

int main()
{
    //���ô��������ܵ�����
    double vX, vY, vZ;
    unsigned int nV = 256;        //���������ܵĵ��������أ�
    unsigned int nCyc = 5;        //���������ܵ�����
    double rT1 = 0.1, rT2 = 0.5;  //���ùܵĿ�ʼ�������˵İ뾶
    double rS = 2;                //�ܵİ뾶
    double h = 10;                //�ܸ�
    unsigned int nTv = 8;         //����ÿ����ı���ɫ��

    unsigned int i;

    //������͵�Ԫ
    vtkNew<vtkPoints> points;
    for(i = 0; i < nV; i++)
    {
        //���ùܵ�����
        vX = rS * cos(2 * vtkMath::Pi() * nCyc * i / (nV - 1));
        vY = rS * cos(2 * vtkMath::Pi() * nCyc * i / (nV - 1));
        vZ = h * i / nV;
        points->InsertPoint(i, vX, vY, vZ);
    }

    vtkNew<vtkCellArray> lines;
    lines->InsertNextCell(nV);
    for(i = 0; i < nV; i++)
    {
        lines->InsertCellPoint(i);
    }

    //���ùܱ仯�İ뾶
    vtkNew<vtkDoubleArray> tubeRadius;
    tubeRadius->SetName("TubeRadius");
    tubeRadius->SetNumberOfTuples(nV);
    for(i = 0; i < nV; i++)
    {
        tubeRadius->SetTuple1(i, rT1 + (rT2 - rT1) * sin(vtkMath::Pi() * i / (nV - 1)));
    }

    //����ɫ�ʴ���ɫ����ɫ�仯�Ĺܱ�
    vtkNew<vtkPolyData> polyData;
    polyData->SetPoints(points);
    polyData->SetLines(lines);
    polyData->GetPointData()->AddArray(tubeRadius);
    polyData->GetPointData()->SetActiveScalars("TubeRadius");

    //����ɫ�ʴ���ɫ����ɫ�仯�Ĺܱ�
    vtkNew<vtkUnsignedCharArray> colors;
    colors->SetName("Colors");
    colors->SetNumberOfComponents(3);
    colors->SetNumberOfTuples(nV);
    for(i = 0; i < nV; i++)
    {
        colors->InsertTuple3(i,
                             int(255*i/(nV-1)),
                             0,
                             int(255*(nV-1-i)/(nV-1)));
    }

    //ͨ��vtkUnsignedCharArray����������
    polyData->GetPointData()->AddArray(colors);

    vtkNew<vtkTubeFilter> tube;
    tube->SetInputData(polyData);
    tube->SetNumberOfSides(nTv);
    tube->SetVaryRadiusToVaryRadiusByAbsoluteScalar();    //���ñ������ùܵı仯�뾶

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(tube->GetOutputPort());
    mapper->ScalarVisibilityOn();
    mapper->SetScalarModeToUsePointFieldData();
    mapper->SelectColorArray("Colors");

    vtkNew<vtkActor> actor;
    
}

