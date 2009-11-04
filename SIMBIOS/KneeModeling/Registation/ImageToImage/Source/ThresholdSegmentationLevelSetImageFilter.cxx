/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: ThresholdSegmentationLevelSetImageFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2009-07-29 22:24:48 $
  Version:   $Revision: 1.37 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif


#include "itkImage.h"
#include "itkThresholdSegmentationLevelSetImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"


int main( int argc, char *argv[] )
{
  if( argc < 6 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage inputSegmentation outputSegmentation";
    std::cerr << " LowerThreshold";
    std::cerr << " UpperThreshold";
    std::cerr << " [CurvatureScaling == 1.0]";
    std::cerr << std::endl;
    return 1;
    }


  typedef   float           InternalPixelType;
  const     unsigned int    Dimension = 3;

  typedef itk::Image< InternalPixelType, Dimension >  InternalImageType;

  typedef unsigned char                            OutputPixelType;
  typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

  typedef itk::BinaryThresholdImageFilter< 
    InternalImageType, InternalImageType > InternalThresholdingFilterType;

  InternalThresholdingFilterType::Pointer thresholder2 = InternalThresholdingFilterType::New();
                        
  thresholder2->SetLowerThreshold( 0.0 );
  thresholder2->SetUpperThreshold( 127.5 );

  thresholder2->SetOutsideValue(   4.0  );
  thresholder2->SetInsideValue(   -4.0 );


  typedef itk::BinaryThresholdImageFilter<InternalImageType, OutputImageType>
                                                   ThresholdingFilterType;
  
  ThresholdingFilterType::Pointer thresholder = ThresholdingFilterType::New();
                        
  thresholder->SetLowerThreshold( -1000.0 );
  thresholder->SetUpperThreshold(     0.0 );

  thresholder->SetOutsideValue(  0  );
  thresholder->SetInsideValue(  255 );

  typedef  itk::ImageFileReader< InternalImageType >    ReaderType;
  typedef  itk::ImageFileWriter< OutputImageType  >     WriterType;
  typedef  itk::ImageFileWriter< InternalImageType  >   InternalWriterType;

  ReaderType::Pointer imageReader = ReaderType::New();
  ReaderType::Pointer maskReader  = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  InternalWriterType::Pointer internalWriter = InternalWriterType::New();

  imageReader->SetFileName( argv[1] );
  maskReader->SetFileName( argv[2] );
  writer->SetFileName( argv[3] );


  typedef  itk::ThresholdSegmentationLevelSetImageFilter< InternalImageType, 
    InternalImageType > ThresholdSegmentationLevelSetImageFilterType;

  ThresholdSegmentationLevelSetImageFilterType::Pointer thresholdSegmentation =
    ThresholdSegmentationLevelSetImageFilterType::New();


  thresholdSegmentation->SetUpperThreshold( atof( argv[4] ) );
  thresholdSegmentation->SetLowerThreshold( atof( argv[5] ) );

  thresholdSegmentation->SetPropagationScaling( 1.0 );

  const double curvatureScaling = atof( argv[6] );
  thresholdSegmentation->SetCurvatureScaling( curvatureScaling );
  
  thresholdSegmentation->SetMaximumRMSError( 1e-4 );
  thresholdSegmentation->SetNumberOfIterations( 1200 );

  thresholdSegmentation->SetIsoSurfaceValue( 0.0 );
  
  thresholder2->SetInput( maskReader->GetOutput() );
  thresholdSegmentation->SetInput( thresholder2->GetOutput() );
  thresholdSegmentation->SetFeatureImage( imageReader->GetOutput() );

  thresholder->SetInput( thresholdSegmentation->GetOutput() );
  writer->SetInput( thresholder->GetOutput() );

  internalWriter->SetFileName("inputInitialLevelSet.mha");
  internalWriter->SetInput( thresholder2->GetOutput() );
  internalWriter->Update();
  

  try
    {
//    writer->Update();
    }
  catch( itk::ExceptionObject & excep )
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    }

  // Print out some useful information 
  std::cout << std::endl;
  std::cout << "Max. no. iterations: " << thresholdSegmentation->GetNumberOfIterations() << std::endl;
  std::cout << "Max. RMS error: " << thresholdSegmentation->GetMaximumRMSError() << std::endl;
  std::cout << std::endl;
  std::cout << "No. elpased iterations: " << thresholdSegmentation->GetElapsedIterations() << std::endl;
  std::cout << "RMS change: " << thresholdSegmentation->GetRMSChange() << std::endl;

  return 0;
}
