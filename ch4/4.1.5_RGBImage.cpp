#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkRGBPixel.h"
#include "itkBMPImageIOFactory.h"

int main()
{
    // 定义RGB像素类型
    using PixelType = itk::RGBPixel<unsigned char>;

    // 定义基于像素的图像类型
    using ImageType = itk::Image<PixelType, 3>;
    
    // 定义图像文件读取器
    itk::ObjectFactoryBase::RegisterFactory(itk::BMPImageIOFactory::New());
    using ReaderType = itk::ImageFileReader<ImageType>;
    ReaderType::Pointer reader = ReaderType::New();
    const char *const filename = "D:/VTK/VTK-9.2.2-data/Testing/Data/masonry.bmp";
    // 设置图像文件读取路径
    reader->SetFileName(filename);
    reader->Update();

    ImageType::Pointer image = reader->GetOutput();
    // 定义像素在坐标上的范围
    const ImageType::IndexType pixelIndex = {{25, 35, 0}};

    PixelType onePixel = image->GetPixel(pixelIndex); // 获取该图像的像素

    PixelType::ValueType red = onePixel.GetRed();
    PixelType::ValueType green = onePixel.GetGreen();
    PixelType::ValueType blue = onePixel.GetBlue();


    std::cout << "Pixel values from GetRed,GetGreen,GetBlue:" << std::endl;
    std::cout << "Red = "
              << itk::NumericTraits<PixelType::ValueType>::PrintType(red)
              << std::endl;
    std::cout << "Green = "
              << itk::NumericTraits<PixelType::ValueType>::PrintType(green)
              << std::endl;
    std::cout << "Blue = "
              << itk::NumericTraits<PixelType::ValueType>::PrintType(blue)
              << std::endl;

    // 提取相应的颜色像素
    red = onePixel[0];   // extract Red   component
    green = onePixel[1]; // extract Green component
    blue = onePixel[2];  // extract Blue  component

    std::cout << "Pixel values:" << std::endl;
    std::cout << "Red = "
              << itk::NumericTraits<PixelType::ValueType>::PrintType(red)
              << std::endl;
    std::cout << "Green = "
              << itk::NumericTraits<PixelType::ValueType>::PrintType(green)
              << std::endl;
    std::cout << "Blue = "
              << itk::NumericTraits<PixelType::ValueType>::PrintType(blue)
              << std::endl;

    return EXIT_SUCCESS;
}
