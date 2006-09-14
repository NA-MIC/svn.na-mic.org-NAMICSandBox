/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBayesianClassificationImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2005/03/14 21:47:51 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
// This filter is a wrapper around the itk::BayesianClassifierInitializationImageFilter
// and the itk::BayesianClassifierImageFilter. It provides the minimal interfaces
// necessary to provide Slicer with a blackbox that takes an image (to be
// classified) as input and provides an output image. 
//
// TODO provide some documentation to a layman on what the filter actually does.
// 
// To sum up, the purpose of this class is to provide only the interfaces that
// can be accessed by the slicer GUI. Here we will provide methods to 
// set the number of classes (to be classified) and the number of smoothing 
// iterations (that must be applied to the posteriors). At a later point a
// method can be provided to switch between Curvature smoothing or Anisotropic
// diffusion. Here we will use Anisotropic diffusion.
//

#ifndef __itkBayesianClassificationImageFilter_h
#define __itkBayesianClassificationImageFilter_h

#include "itkBayesianClassifierImageFilter.h"
#include "itkBayesianClassifierInitializationImageFilter.h"


namespace itk
{
  
template< class TInputImage, class TLabelImage >
//template< class TInputImage, class TMaskImage, class TLabelImage >
class ITK_EXPORT BayesianClassificationImageFilter : public
ImageToImageFilter< TInputImage, TLabelImage >
//ImageToImageFilter< TInputImage, TMaskImage, TLabelImage >
{
public:
  typedef BayesianClassificationImageFilter          Self;
  typedef TInputImage                                InputImageType;
  typedef TLabelImage                                OutputImageType;
  typedef ImageToImageFilter< 
          InputImageType, OutputImageType >          Superclass;

  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( BayesianClassificationImageFilter, ImageToImageFilter );
  
  /** Input and Output image types */
  typedef typename InputImageType::ConstPointer      InputImagePointer;
  typedef typename OutputImageType::Pointer          OutputImagePointer;
  
  /** Set/Get methods for the number of classes. The user must supply this. */
  itkSetMacro( NumberOfClasses, unsigned int );
  itkGetMacro( NumberOfClasses, unsigned int );
  
  /** Number of iterations to apply the smoothing filter */
  itkSetMacro( NumberOfSmoothingIterations, unsigned int );
  itkGetMacro( NumberOfSmoothingIterations, unsigned int );

  /** Mask Image typedefs */
  typedef TInputImage                           MaskImageType;
  typedef typename MaskImageType::PixelType    MaskPixelType;

  /** Get/Set MaskImage **/
  itkSetObjectMacro( MaskImage, MaskImageType );
  itkGetObjectMacro( MaskImage, MaskImageType );

  /** Get/Set MaskValue **/
  itkSetMacro( MaskValue, MaskPixelType );
  itkGetMacro( MaskValue, MaskPixelType );
  
protected:
  BayesianClassificationImageFilter();
  virtual ~BayesianClassificationImageFilter() {}
  
  void PrintSelf(std::ostream& os, Indent indent) const;

  virtual void GenerateData();

  // Initialization filter
  typedef BayesianClassifierInitializationImageFilter< 
            InputImageType >                           BayesianInitializerType;
  typedef typename BayesianInitializerType::Pointer    BayesianInitializerPointer;
  typedef typename BayesianInitializerType::OutputImageType 
                                                       InitializerOutputImageType;
  // Classifier 
  typedef BayesianClassifierImageFilter< 
      InitializerOutputImageType >                     ClassifierFilterType;
  typedef typename ClassifierFilterType::Pointer       ClassifierFilterPointer;

private:
  BayesianClassificationImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  unsigned int                        m_NumberOfClasses;
  unsigned int                        m_NumberOfSmoothingIterations;
  BayesianInitializerPointer          m_Initializer;
  ClassifierFilterPointer             m_Classifier;

  typename MaskImageType::Pointer m_MaskImage;

  MaskPixelType m_MaskValue;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBayesianClassificationImageFilter.txx"
#endif

#endif    
