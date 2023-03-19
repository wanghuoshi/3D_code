#include "vtkImageMapToColors.h"
#include "vtkImageMapper3D.h"
#include "vtkImageReader2.h"
#include "vtkImageReslice.h"
#include "vtkInformation.h"
#include "vtkInteractorStyleImage.h"
#include "vtkLookupTable.h"
#include "vtkMatrix4x4.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkImageData.h"
#include "vtkImageActor.h"

#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);

// 定义一个用于交互的类
class vtkImageInteractionCallback : public vtkCommand
{
public:
    static vtkImageInteractionCallback *New() { return new vtkImageInteractionCallback; }

    vtkImageInteractionCallback()
    {
        this->Slicing = 0;
        this->ImageReslice = nullptr;
        this->Interactor = nullptr;
    }

    void SetImageReslice(vtkImageReslice *reslice) { this->ImageReslice = reslice; }

    vtkImageReslice *GetImageReslice() { return this->ImageReslice; }

    void SetInteractor(vtkRenderWindowInteractor *interactor) { this->Interactor = interactor; }

    vtkRenderWindowInteractor *GetInteractor() { return this->Interactor; }

    void Execute(vtkObject *, unsigned long event, void *) override
    {
        vtkRenderWindowInteractor *interactor = this->GetInteractor();

        int lastPos[2];
        interactor->GetLastEventPosition(lastPos);
        int currPos[2];
        interactor->GetEventPosition(currPos);

        if (event == vtkCommand::LeftButtonPressEvent)
        {
            this->Slicing = 1;
        }
        else if (event == vtkCommand::LeftButtonReleaseEvent)
        {
            this->Slicing = 0;
        }
        else if (event == vtkCommand::MouseMoveEvent)
        {
            if (this->Slicing)
            {
                vtkImageReslice *reslice = this->ImageReslice;

                //  Increment slice position by deltaY of mouse
                int deltaY = lastPos[1] - currPos[1];

                reslice->Update();
                double sliceSpacing = reslice->GetOutput()->GetSpacing()[2];
                vtkMatrix4x4 *matrix = reslice->GetResliceAxes();
                //  move the center point that we are slicing through
                double point[4];
                double center[4];
                point[0] = 0.0;
                point[1] = 0.0;
                point[2] = sliceSpacing * deltaY;
                point[3] = 1.0;
                matrix->MultiplyPoint(point, center);
                matrix->SetElement(0, 3, center[0]);
                matrix->SetElement(1, 3, center[1]);
                matrix->SetElement(2, 3, center[2]);
                interactor->Render();
            }
            else
            {
                vtkInteractorStyle *style =
                    vtkInteractorStyle::SafeDownCast(interactor->GetInteractorStyle());
                if (style)
                {
                    style->OnMouseMove();
                }
            }
        }
    }

private:
    // Actions (slicing only, for now)
    int Slicing;

    // Pointer to vtkImageReslice
    vtkImageReslice *ImageReslice;

    // Pointer to the interactor
    vtkRenderWindowInteractor *Interactor;
};

// The program entry point
int main(int argc, char *argv[])
{
    // Start by loading some data.
    vtkSmartPointer<vtkImageReader2> reader = vtkSmartPointer<vtkImageReader2>::New();
    reader->SetFilePrefix("D:/VTK/VTK-9.2.2-data/Testing/Data/headsq/quarter");
    reader->SetDataExtent(0, 63, 0, 63, 1, 93);
    reader->SetDataSpacing(3.2, 3.2, 1.5);
    reader->SetDataOrigin(0.0, 0.0, 0.0);
    reader->SetDataScalarTypeToUnsignedShort();
    reader->SetDataByteOrderToLittleEndian();
    reader->UpdateWholeExtent();

    // 计算体数据的中心
    reader->Update();
    int extent[6];
    double spacing[3];
    double origin[3];

    reader->GetOutputInformation(0)->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), extent);
    reader->GetOutput()->GetSpacing(spacing);
    reader->GetOutput()->GetOrigin(origin);

    double center[3];
    center[0] = origin[0] + spacing[0] * 0.5 * (extent[0] + extent[1]);
    center[1] = origin[1] + spacing[1] * 0.5 * (extent[2] + extent[3]);
    center[2] = origin[2] + spacing[2] * 0.5 * (extent[4] + extent[5]);

    // Matrices for axial, coronal, sagittal, oblique view orientations
    // static double axialElements[16] = {
    //         1, 0, 0, 0,
    //         0, 1, 0, 0,
    //         0, 0, 1, 0,
    //         0, 0, 0, 1 };

    // static double coronalElements[16] = {
    //         1, 0, 0, 0,
    //         0, 0, 1, 0,
    //         0,-1, 0, 0,
    //         0, 0, 0, 1 };

    static double sagittalElements[16] = {
        0, 0, -1, 0, // 
        1, 0, 0, 0,  // 
        0, -1, 0, 0, // 
        0, 0, 0, 1   // 
    };

    // static double obliqueElements[16] = {
    //         1, 0, 0, 0,
    //         0, 0.866025, -0.5, 0,
    //         0, 0.5, 0.866025, 0,
    //         0, 0, 0, 1 };

    // 设置切片的方向
    vtkSmartPointer<vtkMatrix4x4> resliceAxes = vtkSmartPointer<vtkMatrix4x4>::New();
    // 用于操纵４×４的矩阵信息
    resliceAxes->DeepCopy(sagittalElements);
    // 设置切片穿过的点
    resliceAxes->SetElement(0, 3, center[0]);
    resliceAxes->SetElement(1, 3, center[1]);
    resliceAxes->SetElement(2, 3, center[2]);

    // 通过过滤器抽取想要方向上的切片
    vtkSmartPointer<vtkImageReslice> reslice = vtkSmartPointer<vtkImageReslice>::New();
    reslice->SetInputConnection(reader->GetOutputPort());
    reslice->SetOutputDimensionality(2);     // 设置输出的维数
    reslice->SetResliceAxes(resliceAxes);    // 设置抽取切片的方向
    reslice->SetInterpolationModeToLinear();

    // 创建灰度查询表
    vtkSmartPointer<vtkLookupTable> table = vtkSmartPointer<vtkLookupTable>::New();
    table->SetRange(0, 2000);            // 设置图像强度范围
    table->SetValueRange(0.0, 1.0);      // 从黑色到白色
    table->SetSaturationRange(0.0, 0.0); // 设置饱和度为零
    table->SetRampToLinear();
    table->Build();

    // 通过颜色查询表映射颜色
    vtkSmartPointer<vtkImageMapToColors> color = vtkSmartPointer<vtkImageMapToColors>::New();
    color->SetLookupTable(table);
    color->SetInputConnection(reslice->GetOutputPort());

    // 显示图像
    vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
    actor->GetMapper()->SetInputConnection(color->GetOutputPort());

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);

    vtkSmartPointer<vtkRenderWindow> window = vtkSmartPointer<vtkRenderWindow>::New();
    window->AddRenderer(renderer);

    // 设置交互
    vtkSmartPointer<vtkInteractorStyleImage> imageStyle =
        vtkSmartPointer<vtkInteractorStyleImage>::New();
    vtkSmartPointer<vtkRenderWindowInteractor> interactor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetInteractorStyle(imageStyle);
    window->SetInteractor(interactor);
    window->Render();

    vtkSmartPointer<vtkImageInteractionCallback> callback =
        vtkSmartPointer<vtkImageInteractionCallback>::New();
    callback->SetImageReslice(reslice);
    callback->SetInteractor(interactor);

    // 设置监视器，用于调用相应的消息事件
    imageStyle->AddObserver(vtkCommand::MouseMoveEvent, callback);
    imageStyle->AddObserver(vtkCommand::LeftButtonPressEvent, callback);
    imageStyle->AddObserver(vtkCommand::LeftButtonReleaseEvent, callback);

    // 启动交互
    // The Start() method doesn't return until the window is closed by the user
    interactor->Start();

    return EXIT_SUCCESS;
}