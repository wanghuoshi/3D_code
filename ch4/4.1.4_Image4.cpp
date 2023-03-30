// 定义原点和间距

#include "itkImage.h"

int main()
{
    constexpr unsigned int Dimension = 3;
    using ImageType = itk::Image<unsigned short, Dimension>;
    ImageType::Pointer image = ImageType::New();

    const ImageType::SizeType size = {
        {200, 200, 200}}; // Size along {X,Y,Z}
    const ImageType::IndexType start = {
        {0, 0, 0}}; // First index on {X,Y,Z}

    ImageType::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);

    image->SetRegions(region);
    image->Allocate(true); // initialize buffer to zero

    ImageType::SpacingType spacing;

    spacing[0] = 0.33;          // 沿X轴方向的间隔
    spacing[1] = 0.33;          // 沿Y轴方向的间隔
    spacing[2] = 1.20;          // 沿Z轴方向的间隔
    image->SetSpacing(spacing); // 初始化图像间隔

    // 查看图像间隔的代码
    const ImageType::SpacingType &sp = image->GetSpacing();
    std::cout << "Spacing = ";
    std::cout << sp[0] << ", " << sp[1] << ", " << sp[2] << std::endl;

    // 定义图像中心点
    ImageType::PointType newOrigin;
    newOrigin.Fill(0.0);
    image->SetOrigin(newOrigin);
 
    // 查看图像中心点
    const ImageType::PointType &origin = image->GetOrigin();

    std::cout << "Origin = ";
    std::cout << origin[0] << ", " << origin[1] << ", " << origin[2] << std::endl;

    // Point是用泛型来表示坐标类型及空间大小点的
    using PointType = itk::Point<double, ImageType::ImageDimension>;
    PointType point;
    point[0] = 1.45; // x coordinate
    point[1] = 7.21; // y coordinate
    point[2] = 9.28; // z coordinate

    ImageType::IndexType pixelIndex;

    // 将物理空点的点映射到图像上的索引
    const bool isInside = image->TransformPhysicalPointToIndex(point, pixelIndex);
    if (isInside)
    {
        ImageType::PixelType pixelValue = image->GetPixel(pixelIndex);
        pixelValue += 5;
        std::cout<<"pixelIndex处的像素大小为:";
        std::cout<<pixelValue<<std::endl;
        image->SetPixel(pixelIndex, pixelValue);
        std::cout<<"修改后pixelIndex处像素大小为:";
        std::cout<<pixelValue<<std::endl;
    }

    return EXIT_SUCCESS;
}
