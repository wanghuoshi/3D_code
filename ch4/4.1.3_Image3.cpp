// 访问像素数据

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

int main()
{
	// First the image type should be declared
	using ImageType = itk::Image<unsigned short, 3>;

	// Then the image object can be created
	ImageType::Pointer image = ImageType::New();

	// The image region should be initialized
	const ImageType::SizeType size = {
		{200, 200, 200}}; // Size along {X,Y,Z}
	const ImageType::IndexType start = {
		{0, 0, 0}}; // First index on {X,Y,Z}

	ImageType::RegionType region;
	region.SetSize(size);
	region.SetIndex(start);

	// Pixel data is allocated
	image->SetRegions(region);
	image->Allocate(true); // initialize buffer to zero

	// Software Guide : BeginLatex
	//
	// The individual position of a pixel inside the image is identified by a
	// unique index. An index is an array of integers that defines the position
	// of the pixel along each dimension of the image. The \code{IndexType}
	// is automatically defined by the image and can be accessed using the
	// scope operator \doxygen{Index}. The length of the array will match
	// the dimensions of the associated image.
	//
	// The following code illustrates the declaration of an index variable and
	// the assignment of values to each of its components. Please note that
	// no \code{SmartPointer} is used to access the \code{Index}. This is
	// because \code{Index} is a lightweight object that is not intended to be
	// shared between objects. It is more efficient to produce multiple copies
	// of these small objects than to share them using the SmartPointer
	// mechanism.
	//
	// The following lines declare an instance of the index type and initialize
	// its content in order to associate it with a pixel position in the image.
	//
	// Software Guide : EndLatex

	// Software Guide : BeginCodeSnippet
	const ImageType::IndexType pixelIndex = {
		{27, 29, 37}}; // Position of {X,Y,Z}
	// Software Guide : EndCodeSnippet

	// Software Guide : BeginLatex
	//
	// Having defined a pixel position with an index, it is then possible to
	// access the content of the pixel in the image. The \code{GetPixel()}
	// method allows us to get the value of the pixels.
	//
	// \index{itk::Image!GetPixel()}
	//
	// Software Guide : EndLatex

	// Software Guide : BeginCodeSnippet
	ImageType::PixelType pixelValue = image->GetPixel(pixelIndex);
	// Software Guide : EndCodeSnippet

	// Software Guide : BeginLatex
	//
	// The \code{SetPixel()} method allows us to set the value of the pixel.
	//
	// \index{itk::Image!SetPixel()}
	//
	// Software Guide : EndLatex

	// Software Guide : BeginCodeSnippet
	image->SetPixel(pixelIndex, pixelValue + 1);
	// Software Guide : EndCodeSnippet

	// Software Guide : BeginLatex
	//
	// Please note that \code{GetPixel()} returns the pixel value using copy
	// and not reference semantics. Hence, the method cannot be used to
	// modify image data values.
	//
	// Remember that both \code{SetPixel()} and \code{GetPixel()} are
	// inefficient and should only be used for debugging or for supporting
	// interactions like querying pixel values by clicking with the mouse.
	//
	// Software Guide : EndLatex

	std::cout<<"pixelIndex处的像素大小为：";
	std::cout<<pixelValue<<std::endl;

	std::cout<<"修改后pixelInedx处像素大小为：";
	pixelValue = image->GetPixel(pixelIndex);
	std::cout<<pixelIndex<<std::endl;

	getchar();	

	return EXIT_SUCCESS;
}
