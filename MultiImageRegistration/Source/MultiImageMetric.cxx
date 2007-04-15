/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCongealingMetric.txx,v $
  Language:  C++
  Date:      $Date: 2004/12/22 03:05:02 $
  Version:   $Revision: 1.25 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _MultiImageMetric_cxx
#define _MultiImageMetric_cxx


#include "MultiImageMetric.h"


namespace itk
{

/*
 * Constructor
 */
template <class TFixedImage> 
MultiImageMetric<TFixedImage>
::MultiImageMetric()
{
//   m_FixedImage    = 0; // has to be provided by the user.
//   m_MovingImage   = 0; // has to be provided by the user.
//   m_Transform     = 0; // has to be provided by the user.
//   m_Interpolator  = 0; // has to be provided by the user.
//  m_GradientImage = 0; // will receive the output of the filter;
  m_ComputeGradient = true; // metric computes gradient by default
  m_NumberOfImages = 0;
  m_NumberOfPixelsCounted = 0; // initialize to zero
//  m_GradientImage = NULL; // computed at initialization

  m_ImageArray.resize(0);
  m_GradientImageArray.resize(0);
  m_InterpolatorArray.resize(0);
  m_TransformArray.resize(0);
  m_ImageMaskArray.resize(0);

  m_Threader = MultiThreader::New();
  m_NumberOfThreads = m_Threader->GetNumberOfThreads();
}


/*
 * Set the parameters that define a unique transform
 */
template <class TFixedImage> 
void
MultiImageMetric<TFixedImage>
::SetTransformParameters( const ParametersType & parameters ) const
{

  int numParam = this->m_TransformArray[0]->GetNumberOfParameters();
  ParametersType currentParam(numParam);
  for(int i=0; i<this->m_NumberOfImages; i++ )
  {
    if( !m_TransformArray[i] )
    {
      itkExceptionMacro(<<"Transform "<< i <<" has not been assigned");
    }
    for(int j=0; j<numParam; j++)
      currentParam[j] = parameters[i*numParam+j];
    this->m_TransformArray[i]->SetParametersByValue( currentParam );
  }
}



/*
 * Initialize
 */
template <class TFixedImage> 
void
MultiImageMetric<TFixedImage>
::Initialize(void) throw ( ExceptionObject )
{

  for(int i=0; i<this->m_NumberOfImages; i++)
  {
    if( !m_TransformArray[i] )
    {
      itkExceptionMacro(<<"Transform is not present");
    }

  
    if( !m_InterpolatorArray[i] )
    {
      itkExceptionMacro(<<"Interpolator is not present");
    }
    
    if( !m_ImageArray[i] )
    {
      itkExceptionMacro(<<"FixedImage is not present");
    }
    
  }


  if( m_FixedImageRegion.GetNumberOfPixels() == 0 )
  {
    itkExceptionMacro(<<"FixedImageRegion is empty");
  }

  // Make sure the FixedImageRegion is within the FixedImage buffered region
  if ( !m_FixedImageRegion.Crop( m_ImageArray[0]->GetBufferedRegion() ) )
  {
    itkExceptionMacro(<<"FixedImageRegion does not overlap the fixed image buffered region" );
  }

  for(int i=0; i < this->m_NumberOfImages; i++)
  {
    m_InterpolatorArray[i]->SetInputImage( m_ImageArray[i] );
  }

  if ( m_ComputeGradient )
  {
    vector<GradientImageFilterPointer> gradientFilterArray;
    gradientFilterArray.resize(this->m_NumberOfImages);
 
    for(int j=0; j<this->m_NumberOfImages; j++)
    {
      gradientFilterArray[j]=GradientImageFilterType::New();
      gradientFilterArray[j]->SetInput( m_ImageArray[j] );

      std::cout << m_ImageArray[j] ;
      std::cout << gradientFilterArray[j];
      const typename MovingImageType::SpacingType& spacing = m_ImageArray[j]->GetSpacing();
      double maximumSpacing=0.0;
      for(unsigned int i=0; i<MovingImageDimension; i++)
      {
        if( spacing[i] > maximumSpacing )
        {
          maximumSpacing = spacing[i];
        }
      }
      gradientFilterArray[j]->SetSigma( maximumSpacing );
      gradientFilterArray[j]->SetNormalizeAcrossScale( false );
      gradientFilterArray[j]->UpdateLargestPossibleRegion();

      m_GradientImageArray[j] = gradientFilterArray[j]->GetOutput();
    }
  }

  numberOfParameters = m_TransformArray[0]->GetNumberOfParameters();
  
  // If there are any observers on the metric, call them to give the
  // user code a chance to set parameters on the metric
  this->InvokeEvent( InitializeEvent() );

}
 

/*
 * PrintSelf
 */
template <class TFixedImage> 
void
MultiImageMetric<TFixedImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "ComputeGradient: "
     << static_cast<typename NumericTraits<bool>::PrintType>(m_ComputeGradient)
     << std::endl;
  for( int i=0; i<this->m_NumberOfImages; i++)
  {
    os << indent << "Moving Image: "  << i << " " << m_ImageArray[i].GetPointer()  << std::endl;
    //os << indent << "Gradient Image: "<< i << " " << m_GradientImageArray[i].GetPointer()   << std::endl;
    os << indent << "Transform:    "  << i << " " << m_TransformArray[i].GetPointer()    << std::endl;
    os << indent << "Interpolator: "  << i << " " << m_InterpolatorArray[i].GetPointer() << std::endl;
    os << indent << "Moving Image Mask: " << i << " " << m_ImageMaskArray[i].GetPointer() << std::endl;
  }
  os << indent << "FixedImageRegion: " << m_FixedImageRegion << std::endl;
  os << indent << "Number of Pixels Counted: " << m_NumberOfPixelsCounted << std::endl;

}

template <class TFixedImage> 
void 
MultiImageMetric<TFixedImage>
::SetNumberOfImages(int N)
{

  m_ImageArray.resize(N);
  m_GradientImageArray.resize(N);
  m_InterpolatorArray.resize(N);
  m_TransformArray.resize(N);
  m_ImageMaskArray.resize(N);


  for(int i=m_NumberOfImages; i<N; i++ )
  {
    m_ImageArray[i] =0;
    m_GradientImageArray[i]=0;
    m_InterpolatorArray[i]=0;
    m_TransformArray[i]=0;
    m_ImageMaskArray[i]=0;
  }
    
  m_NumberOfImages = N;
}

template <class TFixedImage> 
int
MultiImageMetric<TFixedImage>
::GetNumberOfImages()
{
  return m_NumberOfImages;
}

template <class TFixedImage>
void
MultiImageMetric<TFixedImage>
  ::BeforeGetThreadedValue(const ParametersType & parameters) const
{
}

template <class TFixedImage>
void
MultiImageMetric<TFixedImage>
::AfterGetThreadedValue(MeasureType & value, DerivativeType & derivative) const
{
}

/*
 * Get the match Measure
 */
template < class TFixedImage >
void
MultiImageMetric < TFixedImage >
::GetThreadedValue ( int itkNotUsed( threadId ) ) const
{
  //int x;
  //std::cin >> x;
}


/*
 * Get the match Measure
 */
template < class TFixedImage >
typename MultiImageMetric < TFixedImage >::MeasureType
MultiImageMetric< TFixedImage >
::GetValue(const ParametersType & parameters) const
{
  // Call a method that perform some calculations prior to splitting the main
  // computations into separate threads
  this->BeforeGetThreadedValue(parameters);

  // Set up the multithreaded processing
  ThreadStruct str;
  str.Metric = this;

  this->GetMultiThreader()->SetNumberOfThreads(this->GetNumberOfThreads());
  this->GetMultiThreader()->SetSingleMethod(this->ThreaderCallback, &str);

  // multithread the execution
  this->GetMultiThreader()->SingleMethodExecute();

  // Call a method that can be overridden by a subclass to perform
  // some calculations after all the threads have completed
  MeasureType value = 0.0;
  //this->AfterGetThreadedValue(value);
  return value;

}





// Callback routine used by the threading library. This routine just calls
// the GetThreadedValue() method after setting the correct partition of data
// for this thread.
template < class TFixedImage >
ITK_THREAD_RETURN_TYPE
MultiImageMetric< TFixedImage >
::ThreaderCallback( void *arg )
{
  ThreadStruct *str;

  int threadId;
  int threadCount;

  threadId = ((MultiThreader::ThreadInfoStruct *)(arg))->ThreadID;
  threadCount = ((MultiThreader::ThreadInfoStruct *)(arg))->NumberOfThreads;

  str = (ThreadStruct *)(((MultiThreader::ThreadInfoStruct *)(arg))->UserData);

  str->Metric->GetThreadedValue( threadId );

  return ITK_THREAD_RETURN_VALUE;
}


} // end namespace itk

#endif
