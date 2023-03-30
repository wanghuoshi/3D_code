#include "itkThresholdImageFilter.h"

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

int main()
{
    // 定义对象类型
    using PixelType = unsigned char;
    using ImageType = itk::Image<PixelType, 2>;
    using FilterType = itk::ThresholdImageFilter<ImageType>;
    using ReaderType = itk::ImageFileReader<ImageType>;
    using WriterType = itk::ImageFileWriter<ImageType>;

    // 创建读取、写入图像文件滤波器及ThresholdImageFilter
    ReaderType::Pointer reader = ReaderType::New();
    FilterType::Pointer filter = FilterType::New();
    WriterType::Pointer writer = WriterType::New();
    writer->SetInput(filter->GetOutput());

    // 设置图像文件的读取路径
    reader->SetFileName("D:/ITK/ITK-data/Wrapping/images/BrainProtonDensitySlice.png");
    filter->SetInput(reader->GetOutput());
    filter->SetOutsideValue(0); // 设置外部值

    // 采用方式ThresholdBelow
    filter->ThresholdBelow(180);
    filter->Update();
    writer->SetFileName(argv[2]);
    writer->Update();

    //  Software Guide : BeginLatex
    //
    //  The output of this example is shown in
    //  Figure~\ref{fig:ThresholdTransferFunctionBelow}.  The second operating
    //  mode of the filter is now enabled by calling the method
    //  \code{ThresholdAbove()}.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    filter->ThresholdAbove(180);
    filter->Update();
    // Software Guide : EndCodeSnippet

    writer->SetFileName(argv[3]);
    writer->Update();

    //  Software Guide : BeginLatex
    //
    //  Updating the filter with this new setting produces the output shown in
    //  Figure~\ref{fig:ThresholdTransferFunctionAbove}.  The third operating
    //  mode of the filter is enabled by calling \code{ThresholdOutside()}.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    filter->ThresholdOutside(170, 190);
    filter->Update();
    // Software Guide : EndCodeSnippet

    writer->SetFileName(argv[4]);
    writer->Update();

    //  Software Guide : BeginLatex
    //
    //  The output of this third, ``band-pass'' thresholding mode is shown in
    //  Figure~\ref{fig:ThresholdTransferFunctionOutside}.
    //
    //  The examples in this
    //  section also illustrate the limitations of the thresholding filter for
    //  performing segmentation by itself. These limitations are particularly
    //  noticeable in noisy images and in images lacking spatial uniformity, as
    //  is the case with MRI due to field bias.
    //
    //  \relatedClasses
    //  \begin{itemize}
    //  \item \doxygen{BinaryThresholdImageFilter}
    //  \end{itemize}
    //
    //  Software Guide : EndLatex

    return EXIT_SUCCESS;
}
