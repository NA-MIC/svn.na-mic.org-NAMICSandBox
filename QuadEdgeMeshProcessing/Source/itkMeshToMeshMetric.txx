/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMeshToMeshMetric.txx,v $
  Language:  C++
  Date:      $Date: 2003-11-08 17:58:32 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkMeshToMeshMetric_txx
#define _itkMeshToMeshMetric_txx

#include "itkMeshToMeshMetric.h"

namespace itk
{

/** Constructor */
template <class TFixedMesh, class TMovingMesh> 
MeshToMeshMetric<TFixedMesh,TMovingMesh>
::MeshToMeshMetric()
{
  m_FixedMesh = 0; // has to be provided by the user.
  m_MovingMesh   = 0; // has to be provided by the user.
  m_Transform     = 0; // has to be provided by the user.
}

/** Set the parameters that define a unique transform */
template <class TFixedMesh, class TMovingMesh> 
void
MeshToMeshMetric<TFixedMesh,TMovingMesh>
::SetTransformParameters( const ParametersType & parameters ) const
{
  if( !m_Transform )
    {
    itkExceptionMacro(<<"Transform has not been assigned");
    }
  m_Transform->SetParameters( parameters );
}


/** Initialize the metric */
template <class TFixedMesh, class TMovingMesh> 
void
MeshToMeshMetric<TFixedMesh,TMovingMesh>
::Initialize(void) throw ( ExceptionObject )
{

  if( !m_Transform )
    {
    itkExceptionMacro(<<"Transform is not present");
    }

  if( !m_MovingMesh )
    {
    itkExceptionMacro(<<"MovingMesh is not present");
    }

  if( !m_FixedMesh )
    {
    itkExceptionMacro(<<"FixedMesh is not present");
    }

  // If the Mesh is provided by a source, update the source.
  if( m_MovingMesh->GetSource() )
    {
    m_MovingMesh->GetSource()->Update();
    }

  // If the point set is provided by a source, update the source.
  if( m_FixedMesh->GetSource() )
    {
    m_FixedMesh->GetSource()->Update();
    }
}
 

/** PrintSelf */
template <class TFixedMesh, class TMovingMesh> 
void
MeshToMeshMetric<TFixedMesh,TMovingMesh>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Moving Mesh: " << m_MovingMesh.GetPointer()  << std::endl;
  os << indent << "Fixed  Mesh: " << m_FixedMesh.GetPointer()   << std::endl;
  os << indent << "Transform:    " << m_Transform.GetPointer()    << std::endl;
}


} // end namespace itk

#endif

