/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: QuadEdgeMeshSphericalDiffeomorphicDemonsFilter1.cxx,v $
  Language:  C++
  Date:      $Date: 2008-03-10 19:46:31 $
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

#include "itkQuadEdgeMeshSphericalDiffeomorphicDemonsFilter.h"
#include "itkQuadEdgeMeshScalarDataVTKPolyDataWriter.h"
#include "itkQuadEdgeMeshVTKPolyDataReader.h"
#include "itkQuadEdgeMesh.h"

int main( int argc, char *argv[] )
{
  if( argc < 9 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " fixedMeshFile  movingMeshFile ";
    std::cerr << " outputMeshfile gamma sigmaX ";
    std::cerr << " lambda smoothingIterations ";
    std::cerr << " numberOfIterations" << std::endl;
    return EXIT_FAILURE;
    }
  
  typedef float      MeshPixelType;
  const unsigned int Dimension = 3;

  typedef itk::QuadEdgeMesh< MeshPixelType, Dimension >   FixedMeshType;
  typedef itk::QuadEdgeMesh< MeshPixelType, Dimension >   MovingMeshType;
  typedef itk::QuadEdgeMesh< MeshPixelType, Dimension >   RegisteredMeshType;

  typedef itk::QuadEdgeMeshSphericalDiffeomorphicDemonsFilter<
    FixedMeshType, MovingMeshType, RegisteredMeshType >   DemonsFilterType;

  DemonsFilterType::Pointer demonsFilter = DemonsFilterType::New();

  typedef itk::QuadEdgeMeshVTKPolyDataReader< FixedMeshType >   FixedReaderType;
  typedef itk::QuadEdgeMeshVTKPolyDataReader< MovingMeshType >  MovingReaderType;

  FixedReaderType::Pointer fixedReader = FixedReaderType::New();
  fixedReader->SetFileName( argv[1] );

  MovingReaderType::Pointer movingReader = MovingReaderType::New();
  movingReader->SetFileName( argv[2] );

  try
    {
    fixedReader->Update( );
    movingReader->Update( );
    }
  catch( itk::ExceptionObject & exp )
    {
    std::cerr << "Exception thrown while reading the input file " << std::endl;
    std::cerr << exp << std::endl;
    return EXIT_FAILURE;
    }

  demonsFilter->SetFixedMesh( fixedReader->GetOutput() );
  demonsFilter->SetMovingMesh( movingReader->GetOutput() );

  DemonsFilterType::PointType center0;
  center0.Fill( 0.0 );

  demonsFilter->SetSphereCenter( center0 );
  demonsFilter->SetSphereRadius( 100.0 );

  const double gamma = atof( argv[4] );
  demonsFilter->SetGamma( gamma );

  const double sigmaX = atof( argv[5] );
  demonsFilter->SetSigmaX( sigmaX );

  const double lambda = atof( argv[6] );
  demonsFilter->SetLambda( lambda );

  const unsigned int maximumNumberOfSmoothingIterations = atoi( argv[7] );
  demonsFilter->SetMaximumNumberOfSmoothingIterations( maximumNumberOfSmoothingIterations );

  const unsigned int maximumNumberOfIterations = atoi( argv[8] );
  demonsFilter->SetMaximumNumberOfIterations( maximumNumberOfIterations );


  try
    {
    demonsFilter->Update( );
    }
  catch( itk::ExceptionObject & exp )
    {
    std::cerr << exp << std::endl;
    return EXIT_FAILURE;
    }


  typedef itk::QuadEdgeMeshScalarDataVTKPolyDataWriter< FixedMeshType >   WriterType;

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[3] );
  writer->SetInput( demonsFilter->GetOutput() );


  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }


  return EXIT_SUCCESS;
}
