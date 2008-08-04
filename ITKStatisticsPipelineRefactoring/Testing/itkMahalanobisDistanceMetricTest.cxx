/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageTest.cxx,v $
  Language:  C++
  Date:      $Date: 2005/04/11 13:52:36 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkMahalanobisDistanceMetric.h"

int itkMahalanobisDistanceMetricTest(int, char* [] )
{
  const unsigned int MeasurementVectorSize = 3;

  typedef itk::Array< float  >  MeasurementVectorType;

  typedef itk::Statistics::MahalanobisDistanceMetric< MeasurementVectorType >   DistanceMetricType;

  DistanceMetricType::Pointer distance = DistanceMetricType::New();

  std::cout << distance->GetNameOfClass() << std::endl;

  distance->Print(std::cout);

  distance->SetMeasurementVectorSize( MeasurementVectorSize ); 

  if( distance->GetMeasurementVectorSize() != MeasurementVectorSize )
    {
    std::cerr << "GetMeasurementVectorSize() Failed !" << std::endl;
    return EXIT_FAILURE;
    }

  //Test if the distance computed is correct
  DistanceMetricType::OriginType origin;
  ::itk::Statistics::MeasurementVectorTraits::SetLength( origin, 3);
  origin[0] = 1.5;
  origin[1] = 2.3;
  origin[2] = 1.0;
  distance->SetOrigin( origin );

  MeasurementVectorType measurement;
  ::itk::Statistics::MeasurementVectorTraits::SetLength( measurement, 3);
  measurement[0] = 2.5;
  measurement[1] = 3.3;
  measurement[2] = 4.0;

  double trueValue = 3.31662;
  double distanceComputed = distance->Evaluate( measurement );
  const double tolerance = 0.001;

  if( fabs( distanceComputed - trueValue) > tolerance )
    {
    std::cerr << "Distance computed not correct: " << "truevalue= " << trueValue
              << "ComputedValue=" << distanceComputed << std::endl;
    return EXIT_FAILURE;
    }

  //Test if we get the same result with identity covariance matrix set
  DistanceMetricType::CovarianceMatrixType   covarianceMatrix;
  covarianceMatrix.set_size( MeasurementVectorSize, MeasurementVectorSize );
  distance->SetCovariance( covarianceMatrix );

  if( distance->GetCovariance() != covarianceMatrix )
    {
    std::cerr << "Get/SetCovariance method error" << std::endl;
    return EXIT_FAILURE;
    }

  double epsilon   = 1e-200;  
  double doubleMax = 1e+25;  

  distance->SetEpsilon( epsilon );
  distance->SetDoubleMax( doubleMax );

  //Test Set/Get Epsilon method
  if( fabs( distance->GetEpsilon() - epsilon ) > tolerance )
    {
    std::cerr << "Get/SetEpsilon method error" << std::endl;
    return EXIT_FAILURE;
    }

  //Test Set/Get DoubleMax method
  if( fabs( distance->GetDoubleMax() - doubleMax ) > tolerance )
    {
    std::cerr << "Get/SetDoubleMax method error" << std::endl;
    return EXIT_FAILURE;
    }
  
  if( fabs( distanceComputed - trueValue) > tolerance )
    {
    std::cerr << "Distance computed not correct: " << "truevalue= " << trueValue
              << "ComputedValue=" << distanceComputed << std::endl;
    return EXIT_FAILURE;
    }
 
  
  //Test if an exception is thrown if a covariance matrix is set with different
  //size
  DistanceMetricType::CovarianceMatrixType   covarianceMatrix2;
  DistanceMetricType::MeasurementVectorSizeType  measurementSize2 = 4;
  covarianceMatrix2.set_size( measurementSize2, measurementSize2 );

  try
    {
    distance->SetCovariance( covarianceMatrix2 );
    std::cerr << "Exception should have been thrown: " << std::endl;
    return EXIT_FAILURE;
    }
  catch( itk::ExceptionObject & excpt )
    {
    std::cerr << "Exception caught: " << excpt << std::endl;
    }

  //Set a covariance matrix and check if the computed inverse matrix is 
  //correct
  //
  DistanceMetricType::CovarianceMatrixType   covarianceMatrix3;
  covarianceMatrix3.set_size( MeasurementVectorSize, MeasurementVectorSize );
  covarianceMatrix3[0][0] = 2.0;  
  covarianceMatrix3[0][1] = 1.4;  
  covarianceMatrix3[0][2] = 5.0;  

  covarianceMatrix3[1][0] = 3.0;  
  covarianceMatrix3[1][1] = 2.0;  
  covarianceMatrix3[1][2] = 5.4;  

  covarianceMatrix3[2][0] = 3.2;  
  covarianceMatrix3[2][1] = 1.4;  
  covarianceMatrix3[2][2] = 7.4;  

  distance->SetCovariance( covarianceMatrix3 );

  //establish the true inverse covariance matrix
  DistanceMetricType::CovarianceMatrixType   trueInverseCovarianceMatrix;
  trueInverseCovarianceMatrix.set_size( MeasurementVectorSize, MeasurementVectorSize );

  trueInverseCovarianceMatrix[0][0] = -2.124;  
  trueInverseCovarianceMatrix[0][1] = 0.986;  
  trueInverseCovarianceMatrix[0][2] = 0.716;  

  trueInverseCovarianceMatrix[1][0] = 1.444;  
  trueInverseCovarianceMatrix[1][1] = 0.352;  
  trueInverseCovarianceMatrix[1][2] = -1.232;  

  trueInverseCovarianceMatrix[2][0] = 0.646;  
  trueInverseCovarianceMatrix[2][1] = -0.493;  
  trueInverseCovarianceMatrix[2][2] = 0.059;  

  // Get the computed inverse covariance matrix 
  DistanceMetricType::CovarianceMatrixType   computedInverseCovarianceMatrix;
  computedInverseCovarianceMatrix = distance->GetInverseCovariance(); 

  if( fabs( trueInverseCovarianceMatrix[0][0] - computedInverseCovarianceMatrix[0][0] ) > tolerance  ||
      fabs( trueInverseCovarianceMatrix[0][1] - computedInverseCovarianceMatrix[0][1] ) > tolerance  ||
      fabs( trueInverseCovarianceMatrix[0][2] - computedInverseCovarianceMatrix[0][2] ) > tolerance  ||
      fabs( trueInverseCovarianceMatrix[1][0] - computedInverseCovarianceMatrix[1][0] ) > tolerance  ||
      fabs( trueInverseCovarianceMatrix[1][1] - computedInverseCovarianceMatrix[1][1] ) > tolerance  ||
      fabs( trueInverseCovarianceMatrix[1][2] - computedInverseCovarianceMatrix[1][2] ) > tolerance  ||
      fabs( trueInverseCovarianceMatrix[2][0] - computedInverseCovarianceMatrix[2][0] ) > tolerance  ||
      fabs( trueInverseCovarianceMatrix[2][1] - computedInverseCovarianceMatrix[2][1] ) > tolerance  ||
      fabs( trueInverseCovarianceMatrix[2][2] - computedInverseCovarianceMatrix[2][2] ) > tolerance ) 
    {
    std::cerr << "Inverse computation error" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
