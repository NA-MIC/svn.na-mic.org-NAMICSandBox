/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSubsample.h,v $
  Language:  C++
  Date:      $Date: 2006/08/17 17:57:22 $
  Version:   $Revision: 1.31 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSubsample_h
#define __itkSubsample_h

#include "itkSample.h"
#include "itkMacro.h"
#include "itkObjectFactory.h"

namespace itk { 
namespace Statistics {

/** \class Subsample
 * \brief This class stores a subset of instance identifiers from another sample
 * object. You can create a subsample out of another sample object or another
 * subsample object. The class is useful when storing or extracting a portion
 * of a sample object. Note that when the elements of a subsample are sorted, 
 * the instance identifiers of the subsample are sorted without changing the
 * original source sample. Most Statistics algorithms (that derive from
 * StatisticsAlgorithmBase accept Subsample objects as inputs).
 *
 */ 
template< class TSample >
class ITK_EXPORT Subsample : 
    public Sample< typename TSample::MeasurementVectorType >
{
public:
  /** Standard class typedefs */
  typedef Subsample                                         Self;
  typedef Sample< typename TSample::MeasurementVectorType > Superclass;
  typedef SmartPointer< Self >                              Pointer;
  typedef SmartPointer<const Self>                          ConstPointer;

  /** Run-time type information (and related methods).*/
  itkTypeMacro(Subsample, Sample);

  /** standard New() method support */
  itkNewMacro(Self);

  /** Smart pointer to the actual sample data holder */
  typedef typename TSample::Pointer SamplePointer;
  
  /** Typedefs for Measurement vector, measurement, Instance Identifier, 
   * frequency, size, size element value from the template argument TSample*/
  typedef typename TSample::MeasurementVectorType MeasurementVectorType;
  typedef typename TSample::MeasurementType       MeasurementType;
  typedef typename TSample::InstanceIdentifier    InstanceIdentifier;
  typedef typename TSample::FrequencyType         FrequencyType;
  typedef typename TSample::TotalFrequencyType    TotalFrequencyType;
  typedef MeasurementVectorType                   ValueType;

  /** Type of the storage for instances that belong to the class 
   * represented by a Subsample object. A Subsample object stores
   * only the InstanceIdentifiers. The actual data is still in the Sample
   * object */
  typedef std::vector< InstanceIdentifier > InstanceIdentifierHolder;

  /** Plug in the actual sample data */
  void SetSample(const TSample* sample)
    { 
    m_Sample = sample; 
    this->SetMeasurementVectorSize( m_Sample->GetMeasurementVectorSize() );
    }

  const TSample* GetSample() const
    { 
    return m_Sample;
    } 


  void InitializeWithAllInstances()
    {
    m_IdHolder.resize(m_Sample->Size());
    typename InstanceIdentifierHolder::iterator idIter = m_IdHolder.begin();
    typename TSample::ConstIterator iter = m_Sample->Begin();
    typename TSample::ConstIterator last = m_Sample->End();
    m_TotalFrequency = NumericTraits< FrequencyType >::Zero;
    while (iter != last)
      {
      *idIter++ = iter.GetInstanceIdentifier();
      m_TotalFrequency += iter.GetFrequency();
      ++iter;
      }
    }

  void AddInstance(InstanceIdentifier id)
    { 
    if ( id > m_Sample->Size() )
      {
      itkExceptionMacro("MeasurementVector " << id << " does not exist in the Sample");
      }

    m_IdHolder.push_back(id); 
    m_TotalFrequency += m_Sample->GetFrequency(id);
    }

  /** returns SizeType object whose each element is the number of
   * elements in each dimension */
  unsigned int Size() const
    { 
    return static_cast<unsigned int>( m_IdHolder.size() );
    }

  void Clear()
    { 
    m_IdHolder.clear();
    m_TotalFrequency = NumericTraits< FrequencyType >::Zero;
    }

  /** retunrs the measurement of the instance which is identified 
   * by the 'id' */
  const MeasurementVectorType & GetMeasurementVector( InstanceIdentifier id) const
    {
    if ( id > m_IdHolder.size() )
      {
      itkExceptionMacro("MeasurementVector " << id << " does not exist");
      }

    // translate the id to its Sample container id 
    InstanceIdentifier idInTheSample = m_IdHolder[id];
    return m_Sample->GetMeasurementVector( idInTheSample );
    }

  /** returns the frequency of the instance which is identified by the 'id' */
  FrequencyType GetFrequency( InstanceIdentifier id ) const
    {
    if ( id > m_IdHolder.size() )
      {
      itkExceptionMacro("MeasurementVector " << id << " does not exist");
      }

    return m_Sample->GetFrequency(id);
    }
  
  /** returns the total frequency for the 'd' dimension */
  TotalFrequencyType GetTotalFrequency() const
    {
    return m_TotalFrequency;
    }
  
  void Swap(int index1, int index2);
  
  MeasurementVectorType GetMeasurementVectorByIndex(int index) const;

  FrequencyType GetFrequencyByIndex(int index) const;

  InstanceIdentifier GetInstanceIdentifier(int index) const;
  
  class Iterator;

  class ConstIterator
    {
  public:
    ConstIterator(typename InstanceIdentifierHolder::const_iterator iter, 
             const Self* classSample)
      :m_Iter(iter), m_Subsample(classSample), m_Sample(classSample->m_Sample)
    {}
    
    FrequencyType GetFrequency() const
      {
      return  m_Sample->GetFrequency(*m_Iter);
      }
    
    const MeasurementVectorType & GetMeasurementVector() const
      {
      return m_Sample->GetMeasurementVector(*m_Iter);
      } 
    
    InstanceIdentifier GetInstanceIdentifier() const   
      {
      return *m_Iter;
      }
    
    ConstIterator& operator++() 
      { 
      ++m_Iter;
      return *this;
      }
    
    ConstIterator& operator+(int n)
      {
      m_Iter += n;
      return *this;
      }
    
    ConstIterator& operator-(int n)
      {
      m_Iter -= n;
      return *this;
      }

    bool operator!=(const ConstIterator& it) 
      {
      return (m_Iter != it.m_Iter);
      }
    
    bool operator==(const ConstIterator& it) 
      { 
      return (m_Iter == it.m_Iter);
      }
    
    ConstIterator& operator=(const ConstIterator& iter)
      {
      m_Iter = iter.m_Iter;
      m_Subsample = iter.m_Subsample;
      m_Sample = iter.m_Sample;
      return *this;
      }

    ConstIterator& operator=(const Iterator& iter)
      {
      m_Iter = iter.m_Iter;
      m_Subsample = iter.m_Subsample;
      m_Sample = iter.m_Sample;
      return *this;
      }

    ConstIterator(const ConstIterator& iter)
      {
      m_Iter = iter.m_Iter;
      m_Subsample = iter.m_Subsample;
      m_Sample = iter.m_Sample;
      }

    ConstIterator(const Iterator& iter)
      {
      m_Iter = iter.m_Iter;
      m_Subsample = iter.m_Subsample;
      m_Sample = iter.m_Sample;
      }

  private:
    // ConstIterator pointing to ImageToListAdaptor
    typename InstanceIdentifierHolder::const_iterator m_Iter; 

    // Pointer to Subsample object
    const Self*    m_Subsample;
    const TSample* m_Sample;
  };

  class Iterator: public ConstIterator
    {
  public:
    Iterator(typename InstanceIdentifierHolder::iterator iter, 
             Self* classSample)
      :ConstIterator( iter, classSample )
      {}
    
    Iterator& operator=(const Iterator& iter)
      {
      this->ConstIterator::operator=( iter );
      return *this;
      }

    Iterator(const Iterator& iter): ConstIterator( iter )
      {
      }
    
    bool operator!=(const ConstIterator& iter)
      {
      return this->ConstIterator::operator!=( iter );
      } 

    bool operator==(const ConstIterator& iter)
      {
      return this->ConstIterator::operator==( iter );
      } 

  private:
  };

  /** This method returns an iterator to the beginning of the
      measurement vectors */
  Iterator Begin()
    { 
    Iterator iter(m_IdHolder.begin(), this);
    return iter; 
    }

  /** This method returns an iterator to the beginning of the
      measurement vectors */
  Iterator  End()
    {
    Iterator iter(m_IdHolder.end(), this); 
    return iter; 
    }

  ConstIterator Begin() const
    { 
    ConstIterator iter(m_IdHolder.begin(), this);
    return iter; 
    }
  
  ConstIterator  End()  const
    {
    ConstIterator iter(m_IdHolder.end(), this); 
    return iter; 
    }
 
protected:
  Subsample();
  virtual ~Subsample() {}
  void PrintSelf(std::ostream& os, Indent indent) const;
  
private:
  Subsample(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  const TSample*              m_Sample;
  InstanceIdentifierHolder    m_IdHolder;
  unsigned int                m_ActiveDimension;
  FrequencyType               m_TotalFrequency;
}; // end of class


} // end of namespace Statistics 
} // end of namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSubsample.txx"
#endif

#endif
