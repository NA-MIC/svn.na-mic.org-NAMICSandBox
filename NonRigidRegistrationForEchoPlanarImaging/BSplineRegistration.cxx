/*=========================================================================

  Program: BSplineRegistration

  Authors: Josh Snyder, Dennis Jen

  Vision: Software tool which can perform robust non-rigid, intrasubject
    registration between an EPI image and a conventional structural image.

  Goal: Use BSplineDeformableTransform, MattesMutualInformationMetric, and
    LBFGSBOptimizer to perform registration. Initialize registration with
    an affine transform generated by FSL's flirt registration tool.
    Evaluate reproducibility with a test/retest analysis. Brute force
    search the parameter space to help determine optimal settings.

  Acknowledgments: ITK example DeformableRegistration7, Vincent Magnotta,
    Xiaodong Tao, plenty of other helpful NA-MIC members.

  http://www.na-mic.org/Wiki/index.php/Engineering:Project:Non-rigid_EPI_registration
  http://www.fmrib.ox.ac.uk/fsl/flirt

=========================================================================*/

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkOutputWindow.h"
#include "itkTextOutput.h"
#include "itkTimeProbesCollectorBase.h"

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageMaskSpatialObject.h"
#include "itkImageRegionIteratorWithIndex.h"


// These are the core components of our registration framework.
// The ITK registration examples identify the LBFGS optimizer as
// appropriate for performing optimization when there are a large number
// of parameters, and LBFGSB as even more appropriate for parameter
// spaces of higher dimensions. Since we're using the
// BSplineDeformableTransform in 3D, we'll use LBFGSB.
//
#include "itkImageRegistrationMethod.h"
#include "itkMattesMutualInformationImageToImageMetric.h"
#include "itkBSplineDeformableTransform.h"
#include "itkLBFGSBOptimizer.h"
#include "itkLinearInterpolateImageFunction.h"


#include "itkShiftScaleImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkCheckerBoardImageFilter.h"


//  The following section of code implements a Command observer
//  used to monitor the evolution of the registration process.
//
#include "itkCommand.h"
class CommandIterationUpdate : public itk::Command {
protected:
  CommandIterationUpdate() {};

public:
  typedef CommandIterationUpdate Self;
  typedef itk::Command Superclass;
  typedef itk::SmartPointer< Self > Pointer;
  typedef itk::LBFGSBOptimizer OptimizerType;
  typedef const OptimizerType *OptimizerPointer;

  itkNewMacro( Self );

  void Execute( itk::Object *caller, const itk::EventObject & event ) {
    Execute( static_cast< const itk::Object * >( caller ), event );
  }

  void Execute( const itk::Object * object, const itk::EventObject & event ) {
      OptimizerPointer optimizer = dynamic_cast< OptimizerPointer >( object );

      if( !(itk::IterationEvent().CheckEvent( &event ) ) ) {
        return;
      }

      std::ostringstream outputMessage;

      outputMessage << optimizer->GetCurrentIteration() << "\t";

      // current value of the metric
      outputMessage << optimizer->GetValue() << "\t";

      // a measure of error
      outputMessage << optimizer->GetInfinityNormOfProjectedGradient() << "\t";

      itk::OutputWindowDisplayText( outputMessage.str().c_str() );
  }
};




int main( int argc, char *argv[] )
{
  itk::OutputWindow::SetInstance(itk::TextOutput::New().GetPointer() );
  itk::Object::SetGlobalWarningDisplay( true );
  
  std::string fixedImageFileName;
  std::string movingImageFileName;
  std::string outputImageFileName;
  std::string bulkTransformFileName;
  std::string checkerboardImageFileName;

  if( ( argc < 4 ) || ( argc > 6 ) ) {
    std::cerr << "Wrong number of parameters" << std::endl;
    std::cerr << "Usage: " << argv[0] << " ";
    std::cerr << "fixedImageFile movingImageFile outputImagefile ";
    std::cerr << "[bulkTransform] [checkerboardOutputfile]" << std::endl;
    return 1;
  }

  fixedImageFileName = static_cast<std::string>( argv[1] );
  movingImageFileName = static_cast<std::string>( argv[2] );
  outputImageFileName = static_cast<std::string>( argv[3] );

  switch ( argc ) {
  case 6: checkerboardImageFileName = static_cast<std::string>( argv[5] );
    // fall through
  case 5: bulkTransformFileName = static_cast<std::string>( argv[4] );
    break;
  }

#ifndef NDEBUG
  itk::OutputWindowDisplayDebugText( ( "Fixed image file name: " + fixedImageFileName ).c_str() );
  itk::OutputWindowDisplayDebugText( ( "Moving image file name: " + movingImageFileName ).c_str() );
  itk::OutputWindowDisplayDebugText( ( "Output image file name: " + outputImageFileName ).c_str() );
  itk::OutputWindowDisplayDebugText( ( "Bulk transform file name: " + bulkTransformFileName ).c_str() );
  itk::OutputWindowDisplayDebugText( ( "Checkerboard image file name: " + checkerboardImageFileName ).c_str() );
#endif

  const unsigned int ImageDimension = 3;
  typedef signed short PixelType;

  typedef itk::Image< PixelType, ImageDimension > FixedImageType;
  typedef itk::Image< PixelType, ImageDimension > MovingImageType;


  const unsigned int SpaceDimension = ImageDimension;
  const unsigned int SplineOrder = 3;
  typedef double CoordinateRepType;

  typedef itk::BSplineDeformableTransform<
                            CoordinateRepType,
                            SpaceDimension,
                            SplineOrder > TransformType;


  typedef itk::LBFGSBOptimizer OptimizerType;


  typedef itk::MattesMutualInformationImageToImageMetric< 
                                    FixedImageType, 
                                    MovingImageType > MetricType;

  typedef itk:: LinearInterpolateImageFunction< 
                                    MovingImageType,
                                    double > InterpolatorType;

  typedef itk::ImageRegistrationMethod< 
                                    FixedImageType, 
                                    MovingImageType > RegistrationType;


  MetricType::Pointer metric = MetricType::New();
  OptimizerType::Pointer optimizer = OptimizerType::New();
  InterpolatorType::Pointer interpolator = InterpolatorType::New();
  RegistrationType::Pointer registration = RegistrationType::New();
  

  registration->SetMetric( metric );
  registration->SetOptimizer( optimizer );
  registration->SetInterpolator( interpolator );

  TransformType::Pointer transform = TransformType::New();
  registration->SetTransform( transform );

  typedef itk::ImageFileReader< FixedImageType  > FixedImageReaderType;
  typedef itk::ImageFileReader< MovingImageType > MovingImageReaderType;

  FixedImageReaderType::Pointer fixedImageReader  = FixedImageReaderType::New();
  MovingImageReaderType::Pointer movingImageReader = MovingImageReaderType::New();

  fixedImageReader->SetFileName( fixedImageFileName.c_str() );
  movingImageReader->SetFileName( movingImageFileName.c_str() );

  FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();
  MovingImageType::Pointer movingImage = movingImageReader->GetOutput();

  registration->SetFixedImage( fixedImage );
  registration->SetMovingImage( movingImage );


  // HACK: FSL maps from voxel coordinates to physical coordinates
  // differently than ITK. FSL just does index * spacing, whereas
  // ITK does index * spacing + origin. Here we explicitly set origin
  // to 0, 0, 0 to account for this. The original val is saved for writing
  // out in the resampled output image.
  // TODO: apply this as a change in the bulkTransform rather than mucking
  // with origins which could have unintended consequences in ITK filters.
  fixedImageReader->Update();
  movingImageReader->Update();

  double zeroOrigin[ SpaceDimension ] = {0, 0, 0};
  FixedImageType::PointType fixedOrigin = fixedImage->GetOrigin();

  fixedImage->SetOrigin( zeroOrigin );
  movingImage->SetOrigin( zeroOrigin );
  
  // TODO: figure out difference between registration's fixed image region
  // and metric's fixed img region. this is a holdover from ITK example
  FixedImageType::RegionType fixedRegion = fixedImage->GetBufferedRegion();
  registration->SetFixedImageRegion( fixedRegion );

  typedef TransformType::RegionType RegionType;
  RegionType bsplineRegion;
  RegionType::SizeType gridSizeOnImage;
  RegionType::SizeType gridBorderSize;
  RegionType::SizeType totalGridSize;


  // BSpline transform consists of a grid of nodes. Set the dimensions here.
  gridSizeOnImage.Fill( 12 );

  // TODO: figure out more details on how grid border is used
  gridBorderSize.Fill( 3 ); // Border for spline order = 3 ( 1 lower, 2 upper )
  totalGridSize = gridSizeOnImage + gridBorderSize;

  bsplineRegion.SetSize( totalGridSize );

  typedef TransformType::SpacingType SpacingType;
  SpacingType spacing = fixedImage->GetSpacing();

  // TODO: does this play well with bulkTransform origin = 0,0,0 hack?
  typedef TransformType::OriginType OriginType;
  OriginType origin = fixedImage->GetOrigin();

  FixedImageType::SizeType fixedImageSize = fixedRegion.GetSize();

  // TODO: update. unchanged from example code.
  for(unsigned int r=0; r < ImageDimension; r++) {
    spacing[ r ] *= floor( static_cast< double >( fixedImageSize[r] - 1 ) / 
                           static_cast< double >( gridSizeOnImage[r] - 1 ) );
    origin[ r ] -= spacing[ r ]; 
  }

  transform->SetGridSpacing( spacing );
  transform->SetGridOrigin( origin );
  transform->SetGridRegion( bsplineRegion );
  
  typedef TransformType::ParametersType ParametersType;
  const unsigned int numberOfParameters =
               transform->GetNumberOfParameters();

  // bspline transform had 3 parameters per grid point, each indicating
  // the deformation offset in the corresponding dimension
  ParametersType parameters( numberOfParameters );
  parameters.Fill( 0.0 );

  transform->SetParameters( parameters );
  registration->SetInitialTransformParameters( transform->GetParameters() );


  OptimizerType::BoundSelectionType boundSelect( transform->GetNumberOfParameters() );
  OptimizerType::BoundValueType upperBound( transform->GetNumberOfParameters() );
  OptimizerType::BoundValueType lowerBound( transform->GetNumberOfParameters() );

  boundSelect.Fill( 2 );
  upperBound.Fill( 4 );
  lowerBound.Fill( -4 );

  // set boundary condition for each variable of the transform
  // 2 = lower bound and upper bound used
  optimizer->SetBoundSelection( boundSelect );
  optimizer->SetUpperBound( upperBound );
  optimizer->SetLowerBound( lowerBound );

  optimizer->SetCostFunctionConvergenceFactor( 1e+7 );
  optimizer->SetMaximumNumberOfIterations( 30 );
  optimizer->SetMaximumNumberOfEvaluations( 500 );
  optimizer->SetMaximumNumberOfCorrections( 12 );
  optimizer->SetProjectedGradientTolerance( 1e-5 );

  CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
  optimizer->AddObserver( itk::IterationEvent(), observer );

  metric->SetNumberOfHistogramBins( 50 );

  // TODO: update to use region defined by mask boundaries
  const unsigned int numberOfSamples = fixedRegion.GetNumberOfPixels() / 80;
  metric->SetNumberOfSpatialSamples( numberOfSamples );

  // TODO: randomize seed selection?
  metric->ReinitializeSeed( 76926294 );


  // initialize bulk transform to be identity, or read it in from user
  // supplied file. file must be ascii format output by FSL registration
  // tools.
  typedef itk::AffineTransform< CoordinateRepType, SpaceDimension >
          BulkTransformType;

  BulkTransformType::Pointer bulkTransform = BulkTransformType::New();

  if( bulkTransformFileName != "" ) {
    std::ifstream transformFile( bulkTransformFileName.c_str() );
    if ( transformFile.fail() ) {
      std::cerr << "ERROR: Couldn't read FSL transform matrix from ";
      std::cerr << bulkTransformFileName << std::endl;
      return -1;
    }

    BulkTransformType::Pointer fslTransform = BulkTransformType::New();
    BulkTransformType::ParametersType params( fslTransform->GetNumberOfParameters() );

    // Assuming SpaceDimension == 3
    // Read 4x4 FSL affine transform into flat 12 element array
    // array follows column major indexing of input matrix
    // see itkAffineTransform->SetParameters() for matrix -> array mapping
    double buffer;
    for ( int row = 0; row < 3; row++ ) {
      for ( int col = 0; col < 3; col++ ) {
        transformFile >> buffer;
        params.SetElement( ( row * 3 ) + col, buffer );
      }

      // 4th col (translation params) is stored as last 3 elements
      transformFile >> buffer;
      params.SetElement( 9 + row, buffer );
    }

    if ( transformFile.bad() ) {
      std::cerr << "ERROR: invalid bulk transform file: ";
      std::cerr << bulkTransformFileName << std::endl;
      return -1;
    }

    fslTransform->SetParameters( params );

    // FSL transform maps from input/moving to reference/fixed image
    // must invert for ITK, which expects mapping from fixed to moving
    fslTransform->GetInverse( bulkTransform );

    // init BSpline xform with inverted FSL xform
    transform->SetBulkTransform( bulkTransform );
  }   



  // set up fixed image mask
  typedef itk::Image< unsigned char, ImageDimension > MaskImageType;
  typedef itk::ResampleImageFilter< 
                            MovingImageType, 
                            FixedImageType > MaskResampleFilterType;

  // mask will be limited to intersection of fixed image and moving image
  // when mapped together using initial bulk transform
  MaskResampleFilterType::Pointer maskResample = MaskResampleFilterType::New();

  maskResample->SetTransform( bulkTransform );
  maskResample->SetInput( movingImage );
  maskResample->SetDefaultPixelValue( -1 );
  maskResample->SetReferenceImage( fixedImage );
  maskResample->UseReferenceImageOn();

  typedef itk::BinaryThresholdImageFilter< FixedImageType, MaskImageType >
          BinarizeFilterType;

  BinarizeFilterType::Pointer maskBinarize = BinarizeFilterType::New();
  maskBinarize->SetLowerThreshold( 0 );
  maskBinarize->SetUpperThreshold(
                  itk::NumericTraits< FixedImageType::PixelType >::max()
                );
  maskBinarize->SetInsideValue( 1 );
  maskBinarize->SetOutsideValue( 0 );
  maskBinarize->SetInput( maskResample->GetOutput() );

  maskBinarize->Update();
  MaskImageType::Pointer maskImage = maskBinarize->GetOutput();

  typedef itk::ImageMaskSpatialObject< ImageDimension > MaskType;
  MaskType::Pointer spatialObjectMask = MaskType::New();

  spatialObjectMask->SetImage( maskImage );
  metric->SetFixedImageMask( spatialObjectMask );
  //metric->DebugOn();


  // Find bounds of image mask. Using this as the metric's fixed
  // image region will speed up the metric's search for randomly
  // located samples within the FixedImageMask.
  itk::ImageRegionIteratorWithIndex<FixedImageType> iter( fixedImage, fixedImage->GetLargestPossibleRegion() );
  
  FixedImageType::IndexType idx;

  // double braces are itk convention. see itk::Index docs
  FixedImageType::IndexType maxIdx = { { 0, 0, 0 } };

  typedef FixedImageType::IndexType::IndexValueType IndexValueType;
  IndexValueType maxIdxValue = itk::NumericTraits< IndexValueType >::max();
  FixedImageType::IndexType minIdx = { { maxIdxValue, maxIdxValue, maxIdxValue } };

  for ( iter.GoToBegin(); !iter.IsAtEnd(); ++iter ) {
    idx = iter.GetIndex();

    // mask and fixed images match, so we can use same index for both
    if ( maskImage->GetPixel( idx ) > 0 ) {
      for ( int i = 0; i < ImageDimension; i++ ) {
        if ( idx[ i ] < minIdx[ i ] ) { minIdx[ i ] = idx[ i ]; }
        if ( idx[ i ] > maxIdx[ i ] ) { maxIdx[ i ] = idx[ i ]; }
      }
    }
  }

  FixedImageType::OffsetType offset = maxIdx - minIdx;
  FixedImageType::SizeType size = { offset[ 0 ], offset[ 1 ], offset[ 2 ] };
  FixedImageType::RegionType region( minIdx, size );

  metric->SetFixedImageRegion( region );



  // Add a time probe
  itk::TimeProbesCollectorBase collector;

  std::cout << std::endl << "Starting Registration" << std::endl;
  try { 
    collector.Start( "Registration" );
    registration->StartRegistration(); 
    collector.Stop( "Registration" );
  } 
  catch( itk::ExceptionObject & err ) { 
    std::cerr << "ExceptionObject caught !" << std::endl; 
    std::cerr << err << std::endl; 
    return -1;
  }
  
  // Report the time taken by the registration
  collector.Report();
  

  OptimizerType::ParametersType finalParameters =
    registration->GetLastTransformParameters();
  transform->SetParameters( finalParameters );


  typedef itk::ResampleImageFilter< 
                            MovingImageType, 
                            FixedImageType > ResampleFilterType;

  ResampleFilterType::Pointer resample = ResampleFilterType::New();

  resample->SetTransform( transform );
  resample->SetInput( movingImage );
  resample->SetReferenceImage( fixedImage );
  resample->UseReferenceImageOn();
  resample->SetDefaultPixelValue( 100 );

  resample->Update();
  resample->GetOutput()->SetOrigin( fixedOrigin );

  typedef itk::Image< PixelType, ImageDimension > OutputImageType;
  
  typedef itk::CastImageFilter< 
                        MovingImageType,
                        OutputImageType > CastFilterType;
                    
  typedef itk::ImageFileWriter< OutputImageType > WriterType;


  WriterType::Pointer writer = WriterType::New();
  CastFilterType::Pointer caster = CastFilterType::New();


  writer->SetFileName( outputImageFileName.c_str() );


  caster->SetInput( resample->GetOutput() );
  writer->SetInput( caster->GetOutput() );


  try {
    writer->Update();
  }
  catch( itk::ExceptionObject & err ) { 
    std::cerr << "ExceptionObject caught !" << std::endl; 
    std::cerr << err << std::endl; 
    return -1;
  }

  typedef itk::ImageFileWriter< MaskImageType > MaskWriterType;

  MaskWriterType::Pointer maskWriter = MaskWriterType::New();
  maskWriter->SetFileName( "imagemask.nii.gz" );
  maskWriter->SetInput( maskBinarize->GetOutput() );
  maskWriter->Update();


  // Compute the checkerboard image between the fixed and resampled moving image.
  if( checkerboardImageFileName != "" ) {
    typedef itk::ShiftScaleImageFilter< OutputImageType, OutputImageType > ScaleFilterType;
    typedef itk::CheckerBoardImageFilter< OutputImageType > CheckerBoardFilterType;
    typedef CheckerBoardFilterType::PatternArrayType CheckerBoardPatternArrayType;

    ScaleFilterType::Pointer scaleFilter = ScaleFilterType::New();
    CheckerBoardFilterType::Pointer checkerboard = CheckerBoardFilterType::New();
    WriterType::Pointer checkerboardWriter = WriterType::New();
    CheckerBoardPatternArrayType pattern;

    scaleFilter->SetScale(20.0);
    scaleFilter->SetInput(fixedImage);

    pattern[0] = fixedImage->GetLargestPossibleRegion().GetSize()[0] / 10;
    pattern[1] = fixedImage->GetLargestPossibleRegion().GetSize()[1] / 10;
    pattern[2] = fixedImage->GetLargestPossibleRegion().GetSize()[2] / 10;

    checkerboard->SetCheckerPattern(pattern);
    checkerboard->SetInput1( scaleFilter->GetOutput() );
    checkerboard->SetInput2( caster->GetOutput() );
    checkerboardWriter->SetInput( checkerboard->GetOutput() );
    checkerboardWriter->SetFileName( checkerboardImageFileName.c_str() );
    try {
      checkerboardWriter->Update();
    }
    catch( itk::ExceptionObject & err ) { 
      std::cerr << "ExceptionObject caught !" << std::endl; 
      std::cerr << err << std::endl; 
      return -1;
    }
  }

  return 0;
}
