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

    ImageType::DirectionType direction;
    direction.SetIdentity();
    image->SetDirection(direction);
    // Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    //
    //  The direction can also be retrieved from an image by using the
    //  \code{GetDirection()} method. This will return a reference to a
    //  \code{Matrix}. The reference can be used to read the contents of
    //  the array. Note again the use of the \code{const} keyword to indicate
    //  that the matrix contents can not be modified.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    const ImageType::DirectionType &direct = image->GetDirection();

    std::cout << "Direction = " << std::endl;
    std::cout << direct << std::endl;

    
    // Software Guide : EndCodeSnippet

    // Software Guide : BeginLatex
    //
    // The image will map the point to an index using the values of the
    // current spacing and origin. An index object must be provided to
    // receive the results of the mapping. The index object can be
    // instantiated by using the \code{IndexType} defined in the image
    // type.
    //
    // Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    
    // Software Guide : EndCodeSnippet

    // Software Guide : BeginLatex
    //
    // The \code{TransformPhysicalPointToIndex()} method of the image class
    // will compute the pixel index closest to the point provided. The method
    // checks for this index to be contained inside the current buffered pixel
    // data. The method returns a boolean indicating whether the resulting
    // index falls inside the buffered region or not. The output index should
    // not be used when the returned value of the method is \code{false}.
    //
    // The following lines illustrate the point to index mapping and the
    // subsequent use of the pixel index for accessing pixel data from the
    // image.
    //
    // \index{itk::Image!TransformPhysicalPointToIndex()}
    //
    // Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    const bool isInside =
        image->TransformPhysicalPointToIndex(point, pixelIndex);
    if (isInside)
    {
        ImageType::PixelType pixelValue = image->GetPixel(pixelIndex);
        pixelValue += 5;
        image->SetPixel(pixelIndex, pixelValue);
    }
    // Software Guide : EndCodeSnippet

    // Software Guide : BeginLatex
    //
    // Remember that \code{GetPixel()} and \code{SetPixel()} are very
    // inefficient methods for accessing pixel data. Image iterators should be
    // used when massive access to pixel data is required.
    //
    // Software Guide : EndLatex
    //

    //  Software Guide : BeginLatex
    //
    //  The following example illustrates the mathematical relationships between
    //  image index locations and its corresponding physical point
    //  representation for a given Image.
    //
    // \index{itk::Image!PhysicalPoint}
    // \index{itk::Image!Index}
    //
    // Let us imagine that a graphical user interface exists
    // where the end user manually selects the voxel index location
    // of the left eye in a volume with a mouse interface.  We need to
    // convert that index location to a physical location so that
    // laser guided surgery can be accurately performed. The
    // \code{TransformIndexToPhysicalPoint} method can be used for this.
    //
    // SoftwareGuide : EndLatex

    // Software Guide : BeginCodeSnippet
    const ImageType::IndexType LeftEyeIndex = GetIndexFromMouseClick();
    ImageType::PointType LeftEyePoint;
    image->TransformIndexToPhysicalPoint(LeftEyeIndex, LeftEyePoint);
    // Software Guide : EndCodeSnippet

    std::cout << "===========================================" << std::endl;
    std::cout << "The Left Eye Location is " << LeftEyePoint << std::endl;

    // create new global def \NL in latex to avoid \\ multi-line warning
    // Software Guide : BeginLatex
    //
    // \gdef\NL{\\}
    //
    // For a given index $\vec{I}$ in 3D, the physical location $\vec{P}$ is
    // calculated as following:
    //
    // \begin{equation}
    // \begin{pmatrix}
    //   P_1\NL
    //   P_2\NL
    //   P_3
    // \end{pmatrix}
    // =
    // \begin{pmatrix}
    //   O_1\NL
    //   O_2\NL
    //   O_3
    // \end{pmatrix}
    // +
    // \begin{pmatrix}
    //   D_{1,1}     & D_{1,2} & D_{1,3}\NL
    //   D_{2,1}     & D_{2,2} & D_{2,3}\NL
    //   D_{3,1}     & D_{3,2} & D_{3,3}
    // \end{pmatrix}
    // *
    // \begin{pmatrix}
    //   S_1 & 0   & 0  \NL
    //   0   & S_2 & 0  \NL
    //   0   & 0   & S_3\NL
    // \end{pmatrix}
    // *
    // \begin{pmatrix}
    //   I_1\NL
    //   I_2\NL
    //   I_3
    // \end{pmatrix}
    // \end{equation}
    // Where:\newline
    // $\vec{I}$: image space index.\newline
    // $\vec{P}$: resulting physical space position of the image index
    // $\vec{I}$.\newline
    // $\vec{O}$: physical space origin of the first image index.\newline
    // $\mathcal{D}$: direction cosines matrix (orthonormal). It represents the
    // orientation relationship between the image and the physical space
    // coordinate system.\newline
    // $\vec{S}$: physical spacing between pixels of the same axis.
    // \newline
    //
    // \noindent The operation can be thought as a particular case of the linear
    // transform: \begin{equation} \vec{P} = \vec{O} + \mathcal{A} \cdot \vec{I}
    // \end{equation}
    // \noindent where $\mathcal{A}$:
    // \begin{equation}
    // \mathcal{A}
    // =
    // \begin{pmatrix}
    //   D_{1,1}\cdot S_1     & D_{1,2}\cdot S_2  & D_{1,3}\cdot S_3\NL
    //   D_{2,1}\cdot S_1     & D_{2,2}\cdot S_2  & D_{2,3}\cdot S_3\NL
    //   D_{3,1}\cdot S_1     & D_{3,2}\cdot S_2  & D_{3,3}\cdot S_3
    // \end{pmatrix}
    // \end{equation}
    //
    // In matlab syntax the conversions are:
    //
    // \begin{verbatim}
    // % Non-identity Spacing and Direction
    // spacing=diag( [0.9375, 0.9375, 1.5] );
    // direction=[0.998189, 0.0569345, -0.0194113;
    //            0.0194429, -7.38061e-08, 0.999811;
    //            0.0569237, -0.998378, -0.00110704];
    // point = origin + direction * spacing * LeftEyeIndex
    // \end{verbatim}
    //
    // A corresponding mathematical expansion of the C/C++ code is:
    // SoftwareGuide : EndLatex

    // Software Guide : BeginCodeSnippet
    using MatrixType = itk::Matrix<double, Dimension, Dimension>;
    MatrixType SpacingMatrix;
    SpacingMatrix.Fill(0.0F);

    const ImageType::SpacingType &ImageSpacing = image->GetSpacing();
    SpacingMatrix(0, 0) = ImageSpacing[0];
    SpacingMatrix(1, 1) = ImageSpacing[1];
    SpacingMatrix(2, 2) = ImageSpacing[2];

    const ImageType::DirectionType &ImageDirectionCosines =
        image->GetDirection();
    const ImageType::PointType &ImageOrigin = image->GetOrigin();

    using VectorType = itk::Vector<double, Dimension>;
    VectorType LeftEyeIndexVector;
    LeftEyeIndexVector[0] = LeftEyeIndex[0];
    LeftEyeIndexVector[1] = LeftEyeIndex[1];
    LeftEyeIndexVector[2] = LeftEyeIndex[2];

    ImageType::PointType LeftEyePointByHand =
        ImageOrigin + ImageDirectionCosines * SpacingMatrix * LeftEyeIndexVector;
    // Software Guide : EndCodeSnippet

    std::cout << "===========================================" << std::endl;
    std::cout << "Spacing:: " << std::endl
              << SpacingMatrix << std::endl;
    std::cout << "===========================================" << std::endl;
    std::cout << "DirectionCosines:: " << std::endl
              << ImageDirectionCosines << std::endl;
    std::cout << "===========================================" << std::endl;
    std::cout << "Origin:: " << std::endl
              << ImageOrigin << std::endl;
    std::cout << "===========================================" << std::endl;
    std::cout << "The Left Eye Location is " << LeftEyePointByHand << std::endl;

    //
    // Check if two results are identical
    //
    if ((LeftEyePointByHand - LeftEyePoint).GetNorm() < 0.01F)
    {
        std::cout << "===========================================" << std::endl;
        std::cout << "Two results are identical as expected!" << std::endl;
        std::cout << "The Left Eye from TransformIndexToPhysicalPoint is "
                  << LeftEyePoint << std::endl;
        std::cout << "The Left Eye from Math is " << LeftEyePointByHand
                  << std::endl;
    }

    return EXIT_SUCCESS;
}
