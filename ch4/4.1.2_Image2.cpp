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
    // �������ص����ͺ�ͼ���ά��
    using PixelType = unsigned char;
    constexpr unsigned int Dimension = 3;

    // ����ͼ�����
    using ImageType = itk::Image<PixelType, Dimension>;

    // ����ͼ���ȡ������ָ��
    using ReaderType = itk::ImageFileReader<ImageType>;
    ReaderType::Pointer reader = ReaderType::New();

    // �����ļ�ͼ���·��
    const char *filename = "D:/VTK/VTK-9.2.2-data/Testing/Data/fullhead15.png";
    reader->SetFileName(filename);
    
    // ��������
    reader->Update();
   
    // ��ͼ��ָ��ָ������ͼ��
    ImageType::Pointer image = reader->GetOutput();

    // ///Ϊ�˰������������Ĺ��ܣ�����Ĵ������ڽ�ͼ��ͨ��VTK��ʾ����/// //
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
