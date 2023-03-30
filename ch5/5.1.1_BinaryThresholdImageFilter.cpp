#include "itkBinaryThresholdImageFilter.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageToVTKImageFilter.h"
#include "vtkImageViewer.h"
#include "vtkRenderWindowInteractor.h"
#include "itkPNGImageIOFactory.h"

#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2)

int main()
{
    // 定义对象类型
    using InputPixelType = unsigned char;     // 输入图像像素
    using OutputPixelType = unsigned char;    // 输出图像像素

    using InputImageType = itk::Image<InputPixelType, 2>;      // 输入图像类型
    using OutputImageType = itk::Image<OutputPixelType, 2>;    // 输出图像类型

    using FilterType = itk::BinaryThresholdImageFilter<InputImageType, OutputImageType>;    // 实例化滤波器类型
    using ReaderType = itk::ImageFileReader<InputImageType>;     // 实例化读取图像文件滤波器
    using WriterType = itk::ImageFileWriter<OutputImageType>;    // 实例化图像输出器
    using ConnectorType = itk::ImageToVTKImageFilter<InputImageType>;

    ReaderType::Pointer reader = ReaderType::New();    // 创建读取图像文件滤波器

    itk::ObjectFactoryBase::RegisterFactory(itk::PNGImageIOFactory::New());

    reader->SetFileName("D:/ITK/ITK-data/Wrapping/images/BrainProtonDensitySlice.png");

    // **************************通过VTK显示原始图像**************************************
    vtkRenderWindowInteractor *interactor0 = vtkRenderWindowInteractor::New();
    ConnectorType::Pointer connector0 = ConnectorType::New();    // ITK到VTK的转换器
    connector0->SetInput(reader->GetOutput());
    
    vtkImageViewer *viewer0 = vtkImageViewer::New();
    viewer0->SetupInteractor(interactor0);
    viewer0->SetInputData(connector0->GetOutput());
    viewer0->SetPosition(150,150);
    viewer0->Render();    // 渲染图像
    viewer0->SetColorWindow(255);
    viewer0->SetColorLevel(128);
    // *********************************************************************************

    // 创建二值门限阈值滤波器，并设置其相关属性
    FilterType::Pointer filter = FilterType::New();    // 创建图像过滤器
    filter->SetInput(reader->GetOutput());             // 设置过滤的图像

    const OutputPixelType outsideValue = 0;     // 设置门限值之外的图像亮度值
    const OutputPixelType insideValue = 180;    // 设置门限值之内的图像亮度值
    
    filter->SetOutsideValue(outsideValue);      // 初始化滤波器门限值之外的图像亮度值
    filter->SetInsideValue(insideValue);        // 初始化滤波器门限值之内的图像亮度值

    const InputPixelType lowerThreshold = 80;     // 设置下门限值大小
    const InputPixelType upperThreshold = 160;    // 设置上门限值大小

    filter->SetLowerThreshold(lowerThreshold);    // 初始化滤波器下门限值
    filter->SetUpperThreshold(upperThreshold);    // 初始化滤波器上门限值

    filter->Update();    // 执行二值门限阈值滤波器操作

    // **************************通过VTK显示过滤后的图像**********************************
    vtkRenderWindowInteractor *interactor = vtkRenderWindowInteractor::New();
    ConnectorType::Pointer connector = ConnectorType::New();    // ITK到VTK的转换器
    connector->SetInput(reader->GetOutput());
    
    vtkImageViewer *viewer = vtkImageViewer::New();
    viewer->SetupInteractor(interactor);
    viewer->SetInputData(connector0->GetOutput());
    viewer->SetPosition(350,150);
    viewer->Render();    // 渲染图像
    viewer->SetColorWindow(255);
    viewer->SetColorLevel(128);

    interactor->Start();     // 启动循环交互
    interactor0->Start();    //最后启动原始图像交互，否则无法进入处理后的图像渲染
    // *********************************************************************************

    WriterType::Pointer writer = WriterType::New();    // 创建图像输出器
    writer->SetInput(filter->GetOutput());             // 设置输出的图像

    writer->SetFileName("D:/Projects/3D_code/ch5/5.1.1.png");    // 设置输出文件路径
    writer->Update();    // 执行写入图像文件滤波器操作

    return EXIT_SUCCESS;
}
