/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGrayscaleFunctionErodeImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/17 14:22:27 $
  Version:   $Revision: 1.15 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGrayscaleFunctionErodeImageFilter_h
#define __itkGrayscaleFunctionErodeImageFilter_h

#include "itkMorphologyImageFilter.h"

namespace itk {

/** \class GrayscaleFunctionErodeImageFilter
 * \brief gray scale function erosion of an image
 *
 * Erode an image using functional grayscale morphology. Function
 * erosion takes the minimum of all the pixels identified by the
 * structuring element minus the structuring element value.
 *
 * The structuring element can be composed of arbitrary nonnegative
 * values (not restricted to zero or one). Element values greater than
 * zero indicate pixels that will be considered during the dilation.
 * The function erosion operation is defined as the minimum over the
 * elements of the image value MINUS the structuring element value.
 * 
 * For the each input image pixel, 
 *   - NeighborhoodIterator gives neighbors of the pixel. 
 *   - Evaluate() member function returns the minimum value among 
 *     the image neighbors minus the kernel value where the kernel has
 *     elements > 0.
 *   - Replace the original value with the min value
 *
 * \sa MorphologyImageFilter, GrayscaleFunctionErodeImageFilter, BinaryErodeImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TOutputImage, class TKernel>
class ITK_EXPORT GrayscaleFunctionErodeImageFilter : 
    public MorphologyImageFilter<TInputImage, TOutputImage, TKernel>
{
public:
  /** Standard class typedefs. */
  typedef GrayscaleFunctionErodeImageFilter Self;
  typedef MorphologyImageFilter<TInputImage, TOutputImage, TKernel>
  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(GrayscaleFunctionErodeImageFilter, 
               MorphologyImageFilter);
  
  /** Declaration of pixel type. */
  typedef typename Superclass::PixelType PixelType;

  /** Kernel (structuring element) iterator. */
  typedef typename Superclass::KernelIteratorType  KernelIteratorType;
 
  /** Neighborhood iterator type. */
  typedef typename Superclass::NeighborhoodIteratorType NeighborhoodIteratorType ;

  /** Kernel typedef. */
  typedef typename Superclass::KernelType KernelType;

  /** Default boundary condition type */
  typedef typename Superclass::DefaultBoundaryConditionType DefaultBoundaryConditionType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  itkStaticConstMacro(KernelDimension, unsigned int,
                      TKernel::NeighborhoodDimension);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(SameDimensionCheck1,
     (Concept::SameDimension<InputImageDimension, OutputImageDimension>));
  itkConceptMacro(SameDimensionCheck2,
    (Concept::SameDimension<InputImageDimension, KernelDimension>));
  itkConceptMacro(InputConvertibleToOutputCheck,
    (Concept::Convertible<PixelType, typename TOutputImage::PixelType>));
  itkConceptMacro(KernelConvertibleToInputCheck,
    (Concept::Convertible<typename TKernel::PixelType, PixelType>));
  itkConceptMacro(InputAdditiveOperatorsCheck,
    (Concept::AdditiveOperators<PixelType>));
  itkConceptMacro(InputLessThanComparableCheck,
    (Concept::LessThanComparable<PixelType>));
  itkConceptMacro(KernelGreaterThanIntCheck,
    (Concept::GreaterThanComparable<typename TKernel::PixelType, int>));
  /** End concept checking */
#endif

protected:
  GrayscaleFunctionErodeImageFilter();
  ~GrayscaleFunctionErodeImageFilter() {};

  /** Evaluate image neighborhood with kernel to find the new value 
   * for the center pixel value
   *
   * It will return the minimum value of the image pixels minus the
   * structuring element values whose corresponding element in the
   * structuring element is positive. This version of Evaluate is used
   * for non-boundary pixels. */
  PixelType Evaluate(const NeighborhoodIteratorType &nit,
                     const KernelIteratorType kernelBegin,
                     const KernelIteratorType kernelEnd);

private:
  GrayscaleFunctionErodeImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  // Default boundary condition for erosion filter, defaults to
  // NumericTraits<PixelType>::max()
  DefaultBoundaryConditionType m_ErodeBoundaryCondition;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkGrayscaleFunctionErodeImageFilter.txx"
#endif

#endif


