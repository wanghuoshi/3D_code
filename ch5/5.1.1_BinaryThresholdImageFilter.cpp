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
    // �����������
    using InputPixelType = unsigned char;     // ����ͼ������
    using OutputPixelType = unsigned char;    // ���ͼ������

    using InputImageType = itk::Image<InputPixelType, 2>;      // ����ͼ������
    using OutputImageType = itk::Image<OutputPixelType, 2>;    // ���ͼ������

    using FilterType = itk::BinaryThresholdImageFilter<InputImageType, OutputImageType>;    // ʵ�����˲�������
    using ReaderType = itk::ImageFileReader<InputImageType>;     // ʵ������ȡͼ���ļ��˲���
    using WriterType = itk::ImageFileWriter<OutputImageType>;    // ʵ����ͼ�������
    using ConnectorType = itk::ImageToVTKImageFilter<InputImageType>;

    ReaderType::Pointer reader = ReaderType::New();    // ������ȡͼ���ļ��˲���

    itk::ObjectFactoryBase::RegisterFactory(itk::PNGImageIOFactory::New());

    reader->SetFileName("D:/ITK/ITK-data/Wrapping/images/BrainProtonDensitySlice.png");

    // **************************ͨ��VTK��ʾԭʼͼ��**************************************
    vtkRenderWindowInteractor *interactor0 = vtkRenderWindowInteractor::New();
    ConnectorType::Pointer connector0 = ConnectorType::New();    // ITK��VTK��ת����
    connector0->SetInput(reader->GetOutput());
    
    vtkImageViewer *viewer0 = vtkImageViewer::New();
    viewer0->SetupInteractor(interactor0);
    viewer0->SetInputData(connector0->GetOutput());
    viewer0->SetPosition(150,150);
    viewer0->Render();    // ��Ⱦͼ��
    viewer0->SetColorWindow(255);
    viewer0->SetColorLevel(128);
    // *********************************************************************************

    // ������ֵ������ֵ�˲��������������������
    FilterType::Pointer filter = FilterType::New();    // ����ͼ�������
    filter->SetInput(reader->GetOutput());             // ���ù��˵�ͼ��

    const OutputPixelType outsideValue = 0;     // ��������ֵ֮���ͼ������ֵ
    const OutputPixelType insideValue = 180;    // ��������ֵ֮�ڵ�ͼ������ֵ
    
    filter->SetOutsideValue(outsideValue);      // ��ʼ���˲�������ֵ֮���ͼ������ֵ
    filter->SetInsideValue(insideValue);        // ��ʼ���˲�������ֵ֮�ڵ�ͼ������ֵ

    const InputPixelType lowerThreshold = 80;     // ����������ֵ��С
    const InputPixelType upperThreshold = 160;    // ����������ֵ��С

    filter->SetLowerThreshold(lowerThreshold);    // ��ʼ���˲���������ֵ
    filter->SetUpperThreshold(upperThreshold);    // ��ʼ���˲���������ֵ

    filter->Update();    // ִ�ж�ֵ������ֵ�˲�������

    // **************************ͨ��VTK��ʾ���˺��ͼ��**********************************
    vtkRenderWindowInteractor *interactor = vtkRenderWindowInteractor::New();
    ConnectorType::Pointer connector = ConnectorType::New();    // ITK��VTK��ת����
    connector->SetInput(reader->GetOutput());
    
    vtkImageViewer *viewer = vtkImageViewer::New();
    viewer->SetupInteractor(interactor);
    viewer->SetInputData(connector0->GetOutput());
    viewer->SetPosition(350,150);
    viewer->Render();    // ��Ⱦͼ��
    viewer->SetColorWindow(255);
    viewer->SetColorLevel(128);

    interactor->Start();     // ����ѭ������
    interactor0->Start();    //�������ԭʼͼ�񽻻��������޷����봦����ͼ����Ⱦ
    // *********************************************************************************

    WriterType::Pointer writer = WriterType::New();    // ����ͼ�������
    writer->SetInput(filter->GetOutput());             // ���������ͼ��

    writer->SetFileName("D:/Projects/3D_code/ch5/5.1.1.png");    // ��������ļ�·��
    writer->Update();    // ִ��д��ͼ���ļ��˲�������

    return EXIT_SUCCESS;
}
