#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataReader.h>
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
#include <vector>

int main()
{
    double scalarRange[2];    //用于保存标量数据

    //读取文件并获得该文件的标量范围
}