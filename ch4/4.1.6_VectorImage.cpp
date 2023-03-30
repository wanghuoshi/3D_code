#include "itkVector.h"
#include "itkImage.h"

int main()
{
    using PixelType = itk::Vector<float, 3>;
    // 定义导入图像类型
    using ImageType = itk::Image<PixelType, 3>;
    ImageType::Pointer image = ImageType::New();  

    // 指定图像的大小
    const ImageType::SizeType size = {{200, 200, 200}}; // Size of {X,Y,Z}

    // 指定图像的起始位置
    const ImageType::IndexType start = {{0, 0, 0}};     // First index at {X,Y,Z}

    // 初始化图像区域
    ImageType::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);

    image->SetRegions(region);
    image->Allocate();

    // 设置图像原点
    ImageType::PixelType initialValue;
    initialValue.Fill(0.0);
    image->FillBuffer(initialValue);

    const ImageType::IndexType pixelIndex = {{27, 29, 37}}; // Position {X,Y,Z}

    // Software Guide : BeginLatex
    //
    // The Vector class inherits the operator \code{[]} from the
    // \doxygen{FixedArray} class. This makes it possible to access the
    // Vector's components using index notation.
    //
    // Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    ImageType::PixelType pixelValue;
    pixelValue[0] = 1.345; // x component
    pixelValue[1] = 6.841; // y component
    pixelValue[2] = 3.295; // x component
    // Software Guide : EndCodeSnippet

    // Software Guide : BeginLatex
    //
    // We can now store this vector in one of the image pixels by defining an
    // index and invoking the \code{SetPixel()} method.
    //
    // Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    image->SetPixel(pixelIndex, pixelValue);
    // Software Guide : EndCodeSnippet

    // The GetPixel method can also be used to read Vectors
    // pixels from the image
    ImageType::PixelType value = image->GetPixel(pixelIndex);

    std::cout << value << std::endl;

    // Lets repeat that both \code{SetPixel()} and \code{GetPixel()} are
    // inefficient and should only be used for debugging purposes or for
    // implementing interactions with a graphical user interface such as
    // querying pixel value by clicking with the mouse.

    return EXIT_SUCCESS;
}
