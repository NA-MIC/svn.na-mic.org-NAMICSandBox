/*==========================================================================

  Portions (c) Copyright 2010-2011 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See Doc/copyright/copyright.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   3D Slicer
  Module:    $HeadURL: $
  Date:      $Date: $
  Version:   $Revision: $

==========================================================================*/

/* AbdoNav includes */
#include "vtkMRMLAbdoNavNode.h"

/* MRML includes */
#include "vtkMRMLScene.h"

/* STL includes */
#include <limits>
#include <sstream>

//---------------------------------------------------------------------------
vtkCxxRevisionMacro(vtkMRMLAbdoNavNode, "$Revision: $");
vtkStandardNewMacro(vtkMRMLAbdoNavNode);


//---------------------------------------------------------------------------
vtkMRMLNode* vtkMRMLAbdoNavNode::CreateNodeInstance()
{
  // first try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkMRMLAbdoNavNode");
  if(ret)
    {
      return (vtkMRMLAbdoNavNode*)ret;
    }
  // if the factory was unable to create the object, then create it here
  return new vtkMRMLAbdoNavNode;
}


//---------------------------------------------------------------------------
vtkMRMLAbdoNavNode::vtkMRMLAbdoNavNode()
{
  this->HideFromEditors = true;

  this->OriginalTrackerTransformID = NULL;
  this->RegisteredTrackerTransformID = NULL;
  this->TrackingSystemUsed = NULL;
  this->SetGuidanceNeedleTipRAS(std::numeric_limits<double>::quiet_NaN(),
                                std::numeric_limits<double>::quiet_NaN(),
                                std::numeric_limits<double>::quiet_NaN());
  this->SetGuidanceNeedleSecondRAS(std::numeric_limits<double>::quiet_NaN(),
                                   std::numeric_limits<double>::quiet_NaN(),
                                   std::numeric_limits<double>::quiet_NaN());
}


//---------------------------------------------------------------------------
vtkMRMLAbdoNavNode::~vtkMRMLAbdoNavNode()
{
  this->OriginalTrackerTransformID = NULL;
  this->RegisteredTrackerTransformID = NULL;
  this->TrackingSystemUsed = NULL;
  this->SetGuidanceNeedleTipRAS(std::numeric_limits<double>::quiet_NaN(),
                                std::numeric_limits<double>::quiet_NaN(),
                                std::numeric_limits<double>::quiet_NaN());
  this->SetGuidanceNeedleSecondRAS(std::numeric_limits<double>::quiet_NaN(),
                                   std::numeric_limits<double>::quiet_NaN(),
                                   std::numeric_limits<double>::quiet_NaN());
}


//---------------------------------------------------------------------------
void vtkMRMLAbdoNavNode::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkMRMLNode::PrintSelf(os, indent);

  os << indent << "OriginalTrackerTransformID: " << (this->OriginalTrackerTransformID ? this->OriginalTrackerTransformID : "(none)") << "\n";
  os << indent << "RegisteredTrackerTransformID: " << (this->RegisteredTrackerTransformID ? this->RegisteredTrackerTransformID : "(none)") << "\n";
  os << indent << "TrackingSystemUsed: " << (this->TrackingSystemUsed ? this->TrackingSystemUsed : "(none)") << "\n";
  os << indent << "GuidanceNeedleTipRAS: " << this->GuidanceNeedleTipRAS[0] << " " << this->GuidanceNeedleTipRAS[1] << " " << this->GuidanceNeedleTipRAS[2] << "\n";
  os << indent << "GuidanceNeedleSecondRAS: " << this->GuidanceNeedleSecondRAS[0] << " " << this->GuidanceNeedleSecondRAS[1] << " " << this->GuidanceNeedleSecondRAS[2] << "\n";
}


//---------------------------------------------------------------------------
void vtkMRMLAbdoNavNode::ReadXMLAttributes(const char** atts)
{
  vtkMRMLNode::ReadXMLAttributes(atts);

  // read all MRML node attributes from two arrays of names and values
  const char* attName;
  const char* attValue;
  while (*atts != NULL)
    {
    attName = *(atts++);
    attValue = *(atts++);
    if (!strcmp(attName, "OriginalTrackerTransformID"))
      {
      this->SetOriginalTrackerTransformID(attValue);
      this->Scene->AddReferencedNodeID(this->OriginalTrackerTransformID, this);
      }
    else if (!strcmp(attName, "RegisteredTrackerTransformID"))
      {
      this->SetRegisteredTrackerTransformID(attValue);
      this->Scene->AddReferencedNodeID(this->RegisteredTrackerTransformID, this);
      }
    else if (!strcmp(attName, "TrackingSystemUsed"))
      {
      this->SetTrackingSystemUsed(attValue);
      }
    else if (!strcmp(attName, "GuidanceNeedleTipRAS"))
      {
      std::stringstream ss;
      double val;
      ss << attValue;
      for (int i = 0; i < 3; i++)
        {
        ss >> val;
        this->GuidanceNeedleTipRAS[i] = val;
        }
      }
    else if (!strcmp(attName, "GuidanceNeedleSecondRAS"))
      {
      std::stringstream ss;
      double val;
      ss << attValue;
      for (int i = 0; i < 3; i++)
        {
        ss >> val;
        this->GuidanceNeedleSecondRAS[i] = val;
        }
      }
    }
}


//---------------------------------------------------------------------------
void vtkMRMLAbdoNavNode::WriteXML(ostream& os, int nIndent)
{
  Superclass::WriteXML(os, nIndent);

  // write all MRML node attributes into output stream
  vtkIndent indent(nIndent);
  {
    std::stringstream ss;
    if (this->OriginalTrackerTransformID)
      {
      ss << this->OriginalTrackerTransformID;
      os << indent << " OriginalTrackerTransformID=\"" << ss.str() << "\"";
      }
  }
  {
    std::stringstream ss;
    if (this->RegisteredTrackerTransformID)
      {
      ss << this->RegisteredTrackerTransformID;
      os << indent << " RegisteredTrackerTransformID=\"" << ss.str() << "\"";
      }
  }
  {
    std::stringstream ss;
    if (this->TrackingSystemUsed)
      {
      ss << this->TrackingSystemUsed;
      os << indent << " TrackingSystemUsed=\"" << ss.str() << "\"";
      }
  }
  os << indent << " GuidanceNeedleTipRAS=\"" << this->GuidanceNeedleTipRAS[0] << " " << this->GuidanceNeedleTipRAS[1] << " " << this->GuidanceNeedleTipRAS[2] << "\"";
  os << indent << " GuidanceNeedleSecondRAS=\"" << this->GuidanceNeedleSecondRAS[0] << " " << this->GuidanceNeedleSecondRAS[1] << " " << this->GuidanceNeedleSecondRAS[2] << "\"";
}


//---------------------------------------------------------------------------
void vtkMRMLAbdoNavNode::Copy(vtkMRMLNode* anode)
{
  Superclass::Copy(anode);
  vtkMRMLAbdoNavNode* node = (vtkMRMLAbdoNavNode*)anode;

  this->SetOriginalTrackerTransformID(node->GetOriginalTrackerTransformID());
  this->SetRegisteredTrackerTransformID(node->GetRegisteredTrackerTransformID());
  this->SetTrackingSystemUsed(node->GetTrackingSystemUsed());
  this->SetGuidanceNeedleTipRAS(node->GetGuidanceNeedleTipRAS());
  this->SetGuidanceNeedleSecondRAS(node->GetGuidanceNeedleSecondRAS());
}


//---------------------------------------------------------------------------
void vtkMRMLAbdoNavNode::UpdateReferenceID(const char* oldID, const char* newID)
{
  if (!strcmp(oldID, this->OriginalTrackerTransformID))
    {
    this->SetOriginalTrackerTransformID(newID);
    }
  else if (!strcmp(oldID, this->RegisteredTrackerTransformID))
    {
    this->SetRegisteredTrackerTransformID(newID);
    }
}
