#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkRGBPixel.h"
#include "itkBMPImageIOFactory.h"

int main()
{
    // ����RGB��������
    using PixelType = itk::RGBPixel<unsigned char>;

    // ����������ص�ͼ������
    using ImageType = itk::Image<PixelType, 3>;
    
    // ����ͼ���ļ���ȡ��
    itk::ObjectFactoryBase::RegisterFactory(itk::BMPImageIOFactory::New());
    using ReaderType = itk::ImageFileReader<ImageType>;
    ReaderType::Pointer reader = ReaderType::New();
    const char *const filename = "D:/VTK/VTK-9.2.2-data/Testing/Data/masonry.bmp";
    // ����ͼ���ļ���ȡ·��
    reader->SetFileName(filename);
    reader->Update();

    ImageType::Pointer image = reader->GetOutput();
    // ���������������ϵķ�Χ
    const ImageType::IndexType pixelIndex = {{25, 35, 0}};

    PixelType onePixel = image->GetPixel(pixelIndex); // ��ȡ��ͼ�������

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

    // ��ȡ��Ӧ����ɫ����
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
