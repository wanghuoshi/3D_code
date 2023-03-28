// 创建一幅图像

#include "itkImage.h"
#include "itkImageToVTKImageFilter.h"

#include "vtkImageViewer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkImageActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

int main()
{
    // ����һ������unsigned short���ص�3Dͼ������ 
    using ImageType = itk::Image<unsigned short, 3>;
    // ����ͼ�����͵�ָ��
    ImageType::Pointer image = ImageType::New();

    // ����ͼ�����͵�����
    ImageType::IndexType start;
    start[0] = 0; // �ڣ��᷽�������
    start[1] = 0; // �ڣ��᷽�������
    start[2] = 0; // �ڣ��᷽�������

    // ����ͼ��ĳߴ�
    ImageType::SizeType size;
    size[0] = 200; // size along X
    size[1] = 200; // size along Y
    size[2] = 200; // size along Z

    // ����ͼ���������������ʼ��������
    ImageType::RegionType region;

    region.SetSize(size);
    region.SetIndex(start);
    
    // ��ʼ��ͼ��
    image->SetRegions(region);
    image->Allocate();    // ��ͼ������ڴ�

    using ConnectorType = itk::ImageToVTKImageFilter<ImageType>;
    ConnectorType::Pointer connector = ConnectorType::New();
    connector->SetInput(image);
    connector->Update();

    vtkImageActor* actor = vtkImageActor::New();
    vtkRenderer* ren = vtkRenderer::New();
    ren->AddActor(actor);
    vtkRenderWindow* renWin = vtkRenderWindow::New();
    renWin->AddRenderer(ren);

    vtkImageViewer* viewer = vtkImageViewer::New();
    vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
    iren->SetRenderWindow(renWin);

    viewer->SetInputData(connector->GetOutput());
    viewer->SetupInteractor(iren);
    viewer->SetColorWindow(255);
    viewer->SetColorLevel(128);
    viewer->Render();
    iren->Initialize();
    iren->Start();

    image->Delete();
    connector->Delete();
    actor->Delete();
    ren->Delete();
    renWin->Delete();
    viewer->Delete();
    iren->Delete();

    return EXIT_SUCCESS;
}
