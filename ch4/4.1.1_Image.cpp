#include "itkImage.h"

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

    return EXIT_SUCCESS;
}
