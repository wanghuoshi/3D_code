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

#include <vtkMath.h>

int main()
{
    //设置创建螺旋管的属性
    double vX, vY, vZ;
    unsigned int nV = 256;        //设置螺旋管的点数（像素）
    unsigned int nCyc = 5;        //设置螺旋管的周数
    double rT1 = 0.1, rT2 = 0.5;  //设置管的开始、结束端的半径
    double rS = 2;                //管的半径
    double h = 10;                //管高
    unsigned int nTv = 8;         //设置每个点的表面色数

    unsigned int i;

    //创建点和单元
    vtkNew<vtkPoints> points;
    for(i = 0; i < nV; i++)
    {
        //设置管的坐标
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

    //设置管变化的半径
    vtkNew<vtkDoubleArray> tubeRadius;
    tubeRadius->SetName("TubeRadius");
    tubeRadius->SetNumberOfTuples(nV);
    for(i = 0; i < nV; i++)
    {
        tubeRadius->SetTuple1(i, rT1 + (rT2 - rT1) * sin(vtkMath::Pi() * i / (nV - 1)));
    }

    //设置色彩从蓝色到红色变化的管变
}

