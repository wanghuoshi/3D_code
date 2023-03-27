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
    // 创建一幅基于unsigned short像素的3D图像类型 
    using ImageType = itk::Image<unsigned short, 3>;
    // 定义图像类型的指针
    ImageType::Pointer image = ImageType::New();

    // 定义图像类型的索引
    ImageType::IndexType start;
    start[0] = 0; // 在Ｘ轴方向的索引
    start[1] = 0; // 在Ｙ轴方向的索引
    start[2] = 0; // 在Ｚ轴方向的索引

    // 定义图像的尺寸
    ImageType::SizeType size;
    size[0] = 200; // size along X
    size[1] = 200; // size along Y
    size[2] = 200; // size along Z

    // 定义图像区域变量、并初始化该区域
    ImageType::RegionType region;

    region.SetSize(size);
    region.SetIndex(start);
    
    // 初始化图像
    image->SetRegions(region);
    image->Allocate();    // 给图像分配内存

    using ConnectorType = itk::ImageToVTKImageFilter<ImageType>;
    ConnectorType::Pointer connector = ConnectorType::New();
    connector->SetInput(image);

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
