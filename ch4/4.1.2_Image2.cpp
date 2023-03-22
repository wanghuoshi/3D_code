#include "itkImage.h"
#include "itkImageFileReader.h"
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
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2)

int main(int, char *argv[])
{
    // 定义像素的类型和图像的维数
    using PixelType = unsigned char;
    constexpr unsigned int Dimension = 3;

    // 定义图像变量
    using ImageType = itk::Image<PixelType, Dimension>;

    // 定义图像读取器及其指针
    using ReaderType = itk::ImageFileReader<ImageType>;
    ReaderType::Pointer reader = ReaderType::New();

    // 设置文件图像的路径
    const char *filename = "D:/VTK/VTK-9.2.2-data/Testing/Data/fullhead15.png";
    reader->SetFileName(filename);
    
    // 立即更新
    reader->Update();
   
    // 将图像指针指向读入的图像
    ImageType::Pointer image = reader->GetOutput();

    // ///为了帮助大家理解程序的功能，下面的代码用于将图像通过VTK显示出来/// //
    using ConnectorType = itk::ImageToVTKImageFilter<ImageType>;
    ConnectorType::Pointer connector = ConnectorType::New();
    connector->SetInput(image);

    vtkImageActor *actor = vtkImageActor::New();
    vtkRenderer *ren = vtkRenderer::New();
    ren->AddActor(actor);
    vtkRenderWindow *renWin = vtkRenderWindow::New();
    renWin->AddRenderer(ren);

    vtkImageViewer *viewer = vtkImageViewer::New();
    vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
    iren->SetRenderWindow(renWin);

    viewer->SetInputData(connector->GetOutput());
    viewer->SetupInteractor(iren);
    viewer->SetColorWindow(255);
    viewer->SetColorLevel(128);
    viewer->Render();
    iren->Initialize();
    iren->Start();

    reader->Delete();
    connector->Delete();
    actor->Delete();
    ren->Delete();
    renWin->Delete();
    viewer->Delete();
    iren->Delete();

    return EXIT_SUCCESS;
}
