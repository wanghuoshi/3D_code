#include "itkImage.h"

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

    return EXIT_SUCCESS;
}
