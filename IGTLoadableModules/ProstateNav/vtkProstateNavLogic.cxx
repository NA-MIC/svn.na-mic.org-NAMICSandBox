/*=auto=========================================================================

  Portions (c) Copyright 2007 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See Doc/copyright/copyright.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   3D Slicer
  Module:    $RCSfile: $
  Date:      $Date: $
  Version:   $Revision: $

=========================================================================auto=*/

#include "vtkObjectFactory.h"
#include "vtkCallbackCommand.h"

#include "vtkProstateNavLogic.h"

#include "vtkMRMLModelDisplayNode.h"
#include "vtkMRMLScalarVolumeNode.h"
#include "vtkMRMLLinearTransformNode.h"
#include "vtkSlicerApplication.h"
#include "vtkSlicerApplicationGUI.h"
#include "vtkSlicerColorLogic.h"
#include "vtkMRMLLabelMapVolumeDisplayNode.h"

#include "vtkMRMLProstateNavManagerNode.h"
#include "vtkMRMLRobotNode.h"

// for DICOM read
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkMetaDataDictionary.h"
#include "itkMetaDataObject.h"
#include "itkGDCMImageIO.h"
#include "itkSpatialOrientationAdapter.h"

#include "vtkMRMLBrpRobotCommandNode.h"

#include "vtkProstateNavGUI.h"

#include "ProstateNavMath.h"

const int COVERAGE_MAP_SIZE_MM=300; // 500
const int COVERAGE_MAP_RESOLUTION_MM=5;

vtkCxxRevisionMacro(vtkProstateNavLogic, "$Revision: 1.9.12.1 $");
vtkStandardNewMacro(vtkProstateNavLogic);

//---------------------------------------------------------------------------
vtkProstateNavLogic::vtkProstateNavLogic()
{
  // Timer Handling
  this->DataCallbackCommand = vtkCallbackCommand::New();
  this->DataCallbackCommand->SetClientData( reinterpret_cast<void *> (this) );
  this->DataCallbackCommand->SetCallback(vtkProstateNavLogic::DataCallback);

  this->TimerOn = 0;
}


//---------------------------------------------------------------------------
vtkProstateNavLogic::~vtkProstateNavLogic()
{
  if (this->DataCallbackCommand)
  {
    this->DataCallbackCommand->Delete();
  }
}

//---------------------------------------------------------------------------
void vtkProstateNavLogic::PrintSelf(ostream& os, vtkIndent indent)
{
    this->vtkObject::PrintSelf(os, indent);

    os << indent << "vtkProstateNavLogic:             " << this->GetClassName() << "\n";

}

//---------------------------------------------------------------------------
void vtkProstateNavLogic::DataCallback(vtkObject *caller, 
                                       unsigned long eid, void *clientData, void *callData)
{
    vtkProstateNavLogic *self = reinterpret_cast<vtkProstateNavLogic *>(clientData);
    vtkDebugWithObjectMacro(self, "In vtkProstateNavLogic DataCallback");
    self->UpdateAll();
}

//---------------------------------------------------------------------------
void vtkProstateNavLogic::UpdateAll()
{

}


int vtkProstateNavLogic::Enter(vtkSlicerApplication* app)
{      
  vtkKWTkUtilities::CreateTimerHandler(this->GetGUI()->GetApplication(), 200, this, "TimerHandler");
  return 1;
}

//----------------------------------------------------------------------------
void vtkProstateNavLogic::TimerHandler()
{
  if (this->TimerOn)
    {
      if (GUI!=NULL)
      {
        if (GUI->GetProstateNavManager()!=NULL)
        {
          if (GUI->GetProstateNavManager()->GetRobotNode()!=NULL)
          {
            this->GUI->GetProstateNavManager()->GetRobotNode()->OnTimer();
          }
        }
      }      
    }
}

//---------------------------------------------------------------------------
int vtkProstateNavLogic::RobotStop()
{

  std::cerr << "vtkProstateNavLogic::RobotStop()" << std::endl;
  return 1;

}


//---------------------------------------------------------------------------
int vtkProstateNavLogic::RobotMoveTo(float px, float py, float pz,
                                     float nx, float ny, float nz,
                                     float tx, float ty, float tz)
{

  std::cerr << "vtkProstateNavLogic::RobotMoveTo()" << std::endl;
  return 1;
}


//---------------------------------------------------------------------------
int vtkProstateNavLogic::RobotMoveTo(float position[3], float orientation[3])
{
  std::cerr << "vtkProstateNavLogic::RobotMoveTo()" << std::endl;

  return 1;
}

//---------------------------------------------------------------------------
int  vtkProstateNavLogic::RobotMoveTo()
{
  return this->GUI->GetProstateNavManager()->GetRobotNode()->MoveTo(this->GUI->GetProstateNavManager()->GetRobotNode()->GetTargetTransformNodeID());  
}
//---------------------------------------------------------------------------
int vtkProstateNavLogic::ScanStart()
{

  return 1;
}

//---------------------------------------------------------------------------
int vtkProstateNavLogic::ScanPause()
{
  return 1;
}

//---------------------------------------------------------------------------
int vtkProstateNavLogic::ScanStop()
{

  return 1;
}

//---------------------------------------------------------------------------
void vtkProstateNavLogic::SetSliceViewFromVolume( vtkSlicerApplication *app,vtkMRMLVolumeNode *volumeNode)
{
  if (!volumeNode)
    {
    return;
    }

  vtkSmartPointer<vtkMatrix4x4> matrix = vtkSmartPointer<vtkMatrix4x4>::New();
  vtkSmartPointer<vtkMatrix4x4> permutationMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
  vtkSmartPointer<vtkMatrix4x4> rotationMatrix = vtkSmartPointer<vtkMatrix4x4>::New();

  volumeNode->GetIJKToRASDirectionMatrix(matrix);
  //slicerCerr("matrix");
  //slicerCerr("   " << matrix->GetElement(0,0) <<
//             "   " << matrix->GetElement(0,1) <<
  //           "   " << matrix->GetElement(0,2));
  //slicerCerr("   " << matrix->GetElement(1,0) <<
    //         "   " << matrix->GetElement(1,1) <<
    //         "   " << matrix->GetElement(1,2));
  //slicerCerr("   " << matrix->GetElement(2,0) <<
   //          "   " << matrix->GetElement(2,1) <<
    //         "   " << matrix->GetElement(2,2));

  int permutation[3];
  int flip[3];
  ProstateNavMath::ComputePermutationFromOrientation(matrix, permutation, flip);

  //slicerCerr("permutation " << permutation[0] << " " <<
//             permutation[1] << " " << permutation[2]);
  //slicerCerr("flip " << flip[0] << " " <<
  //           flip[1] << " " << flip[2]);

  permutationMatrix->SetElement(0,0,0);
  permutationMatrix->SetElement(1,1,0);
  permutationMatrix->SetElement(2,2,0);

  permutationMatrix->SetElement(0, permutation[0],
                     (flip[permutation[0]] ? -1 : 1));
  permutationMatrix->SetElement(1, permutation[1],
                     (flip[permutation[1]] ? -1 : 1));
  permutationMatrix->SetElement(2, permutation[2],
                     (flip[permutation[2]] ? -1 : 1));

  //slicerCerr("permutationMatrix");
  //slicerCerr("   " << permutationMatrix->GetElement(0,0) <<
//             "   " << permutationMatrix->GetElement(0,1) <<
  //           "   " << permutationMatrix->GetElement(0,2));
  //slicerCerr("   " << permutationMatrix->GetElement(1,0) <<
    //         "   " << permutationMatrix->GetElement(1,1) <<
      //       "   " << permutationMatrix->GetElement(1,2));
  //slicerCerr("   " << permutationMatrix->GetElement(2,0) <<
        //     "   " << permutationMatrix->GetElement(2,1) <<
          //   "   " << permutationMatrix->GetElement(2,2));

  permutationMatrix->Invert();
  vtkMatrix4x4::Multiply4x4(matrix, permutationMatrix, rotationMatrix); 

  vtkSlicerApplicationLogic *appLogic =
    app->GetApplicationGUI()->GetApplicationLogic();

  
  // Set the slice views to match the volume slice orientation
  for (int i = 0; i < 3; i++)
    {
    static const char *panes[3] = { "Red", "Yellow", "Green" };

    vtkMatrix4x4 *newMatrix = vtkMatrix4x4::New();

    vtkSlicerSliceLogic *slice = appLogic->GetSliceLogic(
      const_cast<char *>(panes[i]));
    
    vtkMRMLSliceNode *sliceNode = slice->GetSliceNode();

    // Need to find window center and rotate around that

    // Default matrix orientation for slice
    newMatrix->SetElement(0, 0, 0.0);
    newMatrix->SetElement(1, 1, 0.0);
    newMatrix->SetElement(2, 2, 0.0);
    if (i == 0)
      {
      newMatrix->SetElement(0, 0, -1.0);
      newMatrix->SetElement(1, 1, 1.0);
      newMatrix->SetElement(2, 2, 1.0);
      }
    else if (i == 1)
      {
      newMatrix->SetElement(1, 0, -1.0);
      newMatrix->SetElement(2, 1, 1.0);
      newMatrix->SetElement(0, 2, 1.0);
      }
    else if (i == 2)
      {
      newMatrix->SetElement(0, 0, -1.0);
      newMatrix->SetElement(2, 1, 1.0);
      newMatrix->SetElement(1, 2, 1.0);
      }

    // Next, set the orientation to match the volume
    sliceNode->SetOrientationToReformat();
    vtkMatrix4x4::Multiply4x4(rotationMatrix, newMatrix, newMatrix);
    sliceNode->SetSliceToRAS(newMatrix);
    sliceNode->UpdateMatrices();
    newMatrix->Delete();
    }

}
//---------------------------------------------------------------------------
vtkMRMLScalarVolumeNode *vtkProstateNavLogic::AddVolumeToScene(vtkSlicerApplication* app, const char *fileName, VolumeType volumeType/*=VOL_GENERIC*/)
{
  if (fileName==0)
  {
    vtkErrorMacro("AddVolumeToScene: invalid filename");
    return 0;
  }

  vtksys_stl::string volumeNameString = vtksys::SystemTools::GetFilenameName(fileName);
  vtkMRMLScalarVolumeNode *volumeNode = this->AddArchetypeVolume(app, fileName, volumeNameString.c_str());

  if (volumeNode==NULL)
  {
    vtkErrorMacro("Error adding volume to the scene");
    return NULL;
  }

  vtkMRMLProstateNavManagerNode* manager=this->GUI->GetProstateNavManager();
  if (manager==NULL)
  {
    vtkErrorMacro("Error adding volume to the scene, manager is invalid");
    return NULL;
  }

  this->SetAutoScaleScalarVolume(volumeNode);
  this->SetSliceViewFromVolume(app, volumeNode);

  switch (volumeType)
  {
  case VOL_CALIBRATION:
    manager->SetCalibrationVolumeNodeID(volumeNode->GetID());
    break;
  case VOL_TARGETING:
    manager->SetTargetingVolumeNodeID(volumeNode->GetID());
    break;
  case VOL_VERIFICATION:
    manager->SetVerificationVolumeNodeID(volumeNode->GetID());
    break;
  default:
    vtkErrorMacro("AddVolumeToScene: unknown volume type: " << volumeType);
  }
  
  volumeNode->Modified();
  this->Modified();

  return volumeNode;
}

//---------------------------------------------------------------------------
vtkMRMLScalarVolumeNode *vtkProstateNavLogic::AddArchetypeVolume(vtkSlicerApplication* app, const char* fileName, const char *volumeName)
{
  // Set up storageNode
  vtkSmartPointer<vtkMRMLVolumeArchetypeStorageNode> storageNode = vtkSmartPointer<vtkMRMLVolumeArchetypeStorageNode>::New(); 
  storageNode->SetFileName(fileName);
  // check to see if can read this type of file
  if (storageNode->SupportedFileType(fileName) == 0)
    {
    vtkErrorMacro("AddArchetypeVolume: can't read this kind of file: " << fileName);
    return 0;
    }
  storageNode->SetCenterImage(false);
  storageNode->SetSingleFile(false);
  storageNode->SetUseOrientationFromFile(true);

  // Set up scalarNode
  vtkSmartPointer<vtkMRMLScalarVolumeNode> scalarNode = vtkSmartPointer<vtkMRMLScalarVolumeNode>::New();
  scalarNode->SetName(volumeName);
  scalarNode->SetLabelMap(false);

  // Set up displayNode
  vtkSmartPointer<vtkMRMLScalarVolumeDisplayNode> displayNode = vtkSmartPointer<vtkMRMLScalarVolumeDisplayNode>::New();   
  displayNode->SetAutoWindowLevel(false);
  displayNode->SetInterpolate(true);  
  vtkSmartPointer<vtkSlicerColorLogic> colorLogic = vtkSmartPointer<vtkSlicerColorLogic>::New(); 
  displayNode->SetAndObserveColorNodeID(colorLogic->GetDefaultVolumeColorNodeID());
  
  // Add nodes to scene
  this->GetMRMLScene()->SaveStateForUndo();  
  vtkDebugMacro("LoadArchetypeVolume: adding storage node to the scene");
  storageNode->SetScene(this->GetMRMLScene());
  this->GetMRMLScene()->AddNode(storageNode);
  vtkDebugMacro("LoadArchetypeVolume: adding display node to the scene");
  displayNode->SetScene(this->GetMRMLScene());
  this->GetMRMLScene()->AddNode(displayNode);
  vtkDebugMacro("LoadArchetypeVolume: adding scalar node to the scene");
  scalarNode->SetScene(this->GetMRMLScene());
  this->GetMRMLScene()->AddNode(scalarNode);
  scalarNode->SetAndObserveStorageNodeID(storageNode->GetID());
  scalarNode->SetAndObserveDisplayNodeID(displayNode->GetID());
  
  // Read the volume into the node
  vtkDebugMacro("AddArchetypeVolume: about to read data into scalar node " << scalarNode->GetName());
  storageNode->AddObserver(vtkCommand::ProgressEvent, this->LogicCallbackCommand);
  if (this->GetDebug())
    {
    storageNode->DebugOn();
    }
  storageNode->ReadData(scalarNode);
  vtkDebugMacro("AddArchetypeVolume: finished reading data into scalarNode");
  storageNode->RemoveObservers(vtkCommand::ProgressEvent, this->LogicCallbackCommand);
 
  return scalarNode;
}

//--------------------------------------------------------------------------------------
std::string vtkProstateNavLogic::GetFoRStrFromVolumeNodeID(const char* volNodeID)
{  
  vtkMRMLScalarVolumeNode *volNode=vtkMRMLScalarVolumeNode::SafeDownCast(this->GetMRMLScene()->GetNodeByID(volNodeID));  
  if (volNode==NULL)
  {
    vtkErrorMacro("Cannot get FoR, VolumeNode is undefined");
    return std::string("");
  }

  // remaining information to be had from the meta data dictionary     
  const itk::MetaDataDictionary &volDictionary = volNode->GetMetaDataDictionary();
  std::string tagValue; 

  // frame of reference uid
  tagValue.clear(); itk::ExposeMetaData<std::string>( volDictionary, "0020|0052", tagValue );
  
  return tagValue;
}

// Perform AutoWindowLevel computation with CalculateScalarAutoLevels
// Need to do it manually, because automatic implementation in vtkMRMLScalarVolumeNode::UpdateFromMRML()
// assumes that it is a stats volume if name contains 'stat'.
// The name could easily contain 'stat' as we are dealing with 'prostate' images.
void vtkProstateNavLogic::SetAutoScaleScalarVolume(vtkMRMLScalarVolumeNode *volumeNode)
{
  if (volumeNode==0)
  {
    vtkErrorMacro("Invalid volumeNode in SetAutoScaleScalarVolume");
    return;
  }
  vtkMRMLScalarVolumeDisplayNode *displayNode=volumeNode->GetScalarVolumeDisplayNode();
  if (displayNode==0)
  {
    vtkErrorMacro("Invalid displayNode in SetAutoScaleScalarVolume");
    return;
  }
  int modifyOld=displayNode->StartModify(); 
  displayNode->SetAutoWindowLevel(true);
  displayNode->SetAutoThreshold(false);
  displayNode->SetApplyThreshold(false);
  volumeNode->CalculateScalarAutoLevels(NULL,NULL);
  displayNode->SetAutoWindowLevel(false);
  displayNode->SetAutoThreshold(false);
  displayNode->SetApplyThreshold(false);
  displayNode->Modified();
  displayNode->EndModify(modifyOld); 
}

// Recreate, update coverage volume
// Add the volume and a display node to the scene
// return 0 if failed
//----------------------------------------------------------------------------
int vtkProstateNavLogic::ShowCoverage(vtkSlicerApplication *app) 
{

  vtkMRMLProstateNavManagerNode* manager=this->GUI->GetProstateNavManager();
  if (manager==NULL)
  {
    vtkErrorMacro("Error showing coverage, manager is invalid");
    return 0;
  }
  
  vtkMRMLScalarVolumeNode* coverageVolumeNode=vtkMRMLScalarVolumeNode::SafeDownCast(this->GetMRMLScene()->GetNodeByID(manager->GetCoverageVolumeNodeID()));
  if (coverageVolumeNode!=NULL)
  {
    DeleteCoverageVolume();
  }

  // :TODO: check calibration (make sure that the robot is calibrated with the currently displayed targeting volume)

  if (CreateCoverageVolume()==0)
  {
    return 0;
  }
  
  coverageVolumeNode=vtkMRMLScalarVolumeNode::SafeDownCast(this->GetMRMLScene()->GetNodeByID(manager->GetCoverageVolumeNodeID()));
  if (coverageVolumeNode==NULL)
  {
    // the volume node should have been created by now
    return 0;
  }

  // Create display node
  vtkSmartPointer<vtkMRMLLabelMapVolumeDisplayNode> labelDisplayNode  = vtkSmartPointer<vtkMRMLLabelMapVolumeDisplayNode>::New();
  labelDisplayNode->SetAndObserveColorNodeID ("vtkMRMLColorTableNodeLabels"); // set the display node to have a label map lookup table
  this->GetMRMLScene()->AddNode(labelDisplayNode);
  coverageVolumeNode->SetAndObserveDisplayNodeID( labelDisplayNode->GetID() );  

  // add the label volume to the scene
  this->GetMRMLScene()->AddNode(coverageVolumeNode);

  vtkSlicerApplicationGUI *applicationGUI = app->GetApplicationGUI();

  // Reset to original slice location 
  double oldSliceSetting[3];
  oldSliceSetting[0] = double(applicationGUI->GetMainSliceGUI("Red")->GetLogic()->GetSliceOffset());
  oldSliceSetting[1] = double(applicationGUI->GetMainSliceGUI("Yellow")->GetLogic()->GetSliceOffset());
  oldSliceSetting[2] = double(applicationGUI->GetMainSliceGUI("Green")->GetLogic()->GetSliceOffset());

  applicationGUI->GetMainSliceGUI("Red")->GetLogic()->GetSliceCompositeNode()->SetForegroundVolumeID(coverageVolumeNode->GetID());
  applicationGUI->GetMainSliceGUI("Yellow")->GetLogic()->GetSliceCompositeNode()->SetForegroundVolumeID(coverageVolumeNode->GetID());
  applicationGUI->GetMainSliceGUI("Green")->GetLogic()->GetSliceCompositeNode()->SetForegroundVolumeID(coverageVolumeNode->GetID());

  applicationGUI->GetMainSliceGUI("Red")->GetLogic()->GetSliceCompositeNode()->SetForegroundOpacity(0.6);
  applicationGUI->GetMainSliceGUI("Yellow")->GetLogic()->GetSliceCompositeNode()->SetForegroundOpacity(0.6);
  applicationGUI->GetMainSliceGUI("Green")->GetLogic()->GetSliceCompositeNode()->SetForegroundOpacity(0.6);

  // Reset to original slice location 
  applicationGUI->GetMainSliceGUI("Red")->GetLogic()->SetSliceOffset(oldSliceSetting[0]);
  applicationGUI->GetMainSliceGUI("Yellow")->GetLogic()->SetSliceOffset(oldSliceSetting[1]);
  applicationGUI->GetMainSliceGUI("Green")->GetLogic()->SetSliceOffset(oldSliceSetting[2]);

  return 1;
}

// Create and initialize coverage volume (stored in the main MRML node)
// return 0 if failed
int vtkProstateNavLogic::CreateCoverageVolume()
{
  vtkMRMLProstateNavManagerNode* manager=this->GUI->GetProstateNavManager();
  if (manager==NULL)
  {
    vtkErrorMacro("Error creating coverage volume, manager is invalid");
    return 0;
  }

  //vtkMRMLVolumeNode* volumeNode = vtkMRMLVolumeNode::SafeDownCast(this->GetMRMLScene()->GetNodeByID(manager->GetCalibrationVolumeNodeID()));
  vtkMRMLVolumeNode* volumeNode = vtkMRMLVolumeNode::SafeDownCast(this->GetMRMLScene()->GetNodeByID(manager->GetTargetingVolumeNodeID()));
  if (!volumeNode) 
  {
    return 0;
  }

  // Create volume node (as copy of calibration volume)
  vtkSmartPointer<vtkMRMLScalarVolumeNode> coverageVolumeNode = vtkSmartPointer<vtkMRMLScalarVolumeNode>::New();    
  int modifiedSinceRead = volumeNode->GetModifiedSinceRead();
  coverageVolumeNode->CopyWithScene(volumeNode);
  coverageVolumeNode->SetName("TRPBCoverage");
  manager->SetCoverageVolumeNodeID(coverageVolumeNode->GetID());   // Add ref to main MRML node

  // Create image data
  vtkSmartPointer<vtkImageData> coverageLabelMapImage=vtkSmartPointer<vtkImageData>::New();
  int dim=COVERAGE_MAP_SIZE_MM/COVERAGE_MAP_RESOLUTION_MM;
  coverageLabelMapImage->SetDimensions(dim, dim, dim);    
  coverageLabelMapImage->SetWholeExtent(0,dim-1,0,dim-1,0,dim-1);
  coverageLabelMapImage->SetScalarType(VTK_SHORT);
  coverageLabelMapImage->AllocateScalars();
  coverageVolumeNode->SetAndObserveImageData(coverageLabelMapImage);

  // Get the calibration volume centerpoint in RAS coordinates
  double rasCenterPoint[4]={0,0,0,1}; // centerpoint position in RAS coorindates
  {   
    int extent[6];
    volumeNode->GetImageData()->GetWholeExtent(extent);

    double ijkCenterPoint[4]={0,0,0,1}; // centerpoint position in IJK coorindates
    ijkCenterPoint[0]=(extent[0]+extent[1])/2;
    ijkCenterPoint[1]=(extent[2]+extent[3])/2;
    ijkCenterPoint[2]=(extent[4]+extent[5])/2;

    vtkSmartPointer<vtkMatrix4x4> ijkToRas=vtkSmartPointer<vtkMatrix4x4>::New();
    volumeNode->GetIJKToRASMatrix(ijkToRas);

    ijkToRas->MultiplyPoint(ijkCenterPoint, rasCenterPoint);
  }
   
  // Set coverage volume size and position
  coverageVolumeNode->SetOrigin(rasCenterPoint[0]-COVERAGE_MAP_SIZE_MM/2, rasCenterPoint[1]-COVERAGE_MAP_SIZE_MM/2, rasCenterPoint[2]-COVERAGE_MAP_SIZE_MM/2);
  coverageVolumeNode->SetSpacing(COVERAGE_MAP_RESOLUTION_MM,COVERAGE_MAP_RESOLUTION_MM,COVERAGE_MAP_RESOLUTION_MM);
  vtkSmartPointer<vtkMatrix4x4> ijkToRasDirectionMatrix=vtkSmartPointer<vtkMatrix4x4>::New();
  ijkToRasDirectionMatrix->Identity();
  coverageVolumeNode->SetIJKToRASDirectionMatrix(ijkToRasDirectionMatrix);
  
  coverageVolumeNode->SetAndObserveStorageNodeID(NULL);
  coverageVolumeNode->SetModifiedSinceRead(1);
  coverageVolumeNode->SetLabelMap(1);
  
  // Restore modifiedSinceRead value since copy cause Modify on image data.
  volumeNode->SetModifiedSinceRead(modifiedSinceRead);

  return UpdateCoverageVolumeImage();
}

// Update the coverage volume label map image
// return 0 if failed
int vtkProstateNavLogic::UpdateCoverageVolumeImage()
{
  vtkMRMLProstateNavManagerNode* manager=this->GUI->GetProstateNavManager();
  if (manager==NULL)
  {
    vtkErrorMacro("Error updating coverage volume, manager is invalid");
    return 0;
  }
  vtkMRMLVolumeNode* coverageVolumeNode = vtkMRMLVolumeNode::SafeDownCast(this->GetMRMLScene()->GetNodeByID(manager->GetCoverageVolumeNodeID()));
  if (coverageVolumeNode==NULL)
  {
    vtkWarningMacro("CoverageMapUpdate failed, the map is not initialized");
    return 0;
  }
  vtkImageData *coverageImage=coverageVolumeNode->GetImageData();
  if (coverageImage==NULL)
  {
    vtkWarningMacro("CoverageMapUpdate failed, the map is not initialized");
    return 0;
  }

//  std::string needleType = this->NeedleTypeMenuList->GetWidget()->GetValue();
  
  double *origin=coverageImage->GetOrigin();
  double *spacing=coverageImage->GetSpacing();

  double rasPoint[4]={0,0,0,1};
  double ijkPoint[4]={0,0,0,1};
  int extent[6];
  coverageImage->GetWholeExtent(extent);

  vtkSmartPointer<vtkMatrix4x4> ijkToRas=vtkSmartPointer<vtkMatrix4x4>::New();
  coverageVolumeNode->GetIJKToRASMatrix(ijkToRas);

  float value=0;  
  for (int z=extent[4]; z<=extent[5]; z++)
  {
    ijkPoint[2]=z;
    for (int y=extent[2]; y<=extent[3]; y++)
    {
      ijkPoint[1]=y;
      for (int x=extent[0]; x<=extent[1]; x++)
      {         
        ijkPoint[0]=x;           
        ijkToRas->MultiplyPoint(ijkPoint, rasPoint);
        
        value=0;
        if (z!=extent[4] && z!=extent[5] && 
          y!=extent[2] && y!=extent[3] &&
          x!=extent[0] && x!=extent[1])
        {
          // it is not a boundary voxel
          // (we leave a black boundary around the image to ensure that
          // contouring of the coverage area results in a closed surface)
          if (IsTargetReachable(manager->GetCurrentNeedleIndex(), rasPoint))
          {
            value=1;
          }
        }

        coverageImage->SetScalarComponentFromFloat(x, y, z, 0, value);
      }
    }
  }

  coverageImage->Update();
  coverageVolumeNode->Modified();
  
  return 1;
}

// Remove coverage volume and display node from the scene and from the main MRML node
void vtkProstateNavLogic::DeleteCoverageVolume() 
{
  vtkMRMLProstateNavManagerNode* manager=this->GUI->GetProstateNavManager();
  if (manager==NULL)
  {
    vtkErrorMacro("Error deleting coverage volume, manager is invalid");
    return;
  }
  vtkMRMLVolumeNode* coverageVolumeNode = vtkMRMLVolumeNode::SafeDownCast(this->GetMRMLScene()->GetNodeByID(manager->GetCoverageVolumeNodeID()));
  if (coverageVolumeNode==NULL)
  {
    vtkWarningMacro("Error deleting coverage volume, coverage volume node is not found");
    return;
  }
  
  // remove node from scene
  vtkMRMLScene *scene=this->GetMRMLScene();
  if (scene!=NULL)
  { 
    scene->RemoveNode(coverageVolumeNode);
  }
  
  // delete image data
  coverageVolumeNode->SetAndObserveImageData(NULL);
  
  // delete volume node
  manager->SetCoverageVolumeNodeID(NULL);
}

//--------------------------------------------------------------------------------------
bool vtkProstateNavLogic::IsTargetReachable(int needleIndex, double rasLocation[3])
{

  vtkMRMLProstateNavManagerNode* manager=this->GUI->GetProstateNavManager();
  if (manager==NULL)
  {
    vtkErrorMacro("Error determining if target is reachable, manager is invalid");
    return false;
  }

  static vtkProstateNavTargetDescriptor *targetDesc=NULL;
  if (targetDesc==NULL)
  {
    targetDesc=vtkProstateNavTargetDescriptor::New();
  }
  //vtkSmartPointer<vtkProstateNavTargetDescriptor> targetDesc=vtkSmartPointer<vtkProstateNavTargetDescriptor>::New();

  targetDesc->SetRASLocation(rasLocation[0], rasLocation[1], rasLocation[2]);
  
  targetDesc->SetNeedleType(manager->GetNeedleType(needleIndex), manager->GetNeedleLength(needleIndex), manager->GetNeedleOvershoot(needleIndex));

  // record the FoR str - takes quite long time
  //std::string FoR = this->GetFoRStrFromVolumeNodeID(manager->GetTargetingVolumeNodeID());
  //targetDesc->SetFoRStr(FoR);

  // 2) calculate targeting parameters for active needle, store in a target descriptor
  if(!manager->FindTargetingParams(targetDesc))
      {
      // error message
      // to do
      return false;
      }

  if (targetDesc->GetIsOutsideReach())
  {
    return false;
  }

  if (targetDesc->GetDepthCM()>manager->GetNeedleLength(needleIndex))
  {
    return false;
  }


  return true;
}

void vtkProstateNavLogic::UpdateTargetListFromMRML()
{
  vtkMRMLProstateNavManagerNode* manager=this->GUI->GetProstateNavManager();
  if (manager==NULL)
  {
    vtkErrorMacro("Error updating targetlist from mrml, manager is invalid");
    return;
  }
  vtkMRMLFiducialListNode* fidNode=manager->GetTargetPlanListNode();
  if (fidNode==NULL)
  {
    vtkErrorMacro("Error updating targetlist from mrml, fiducial node is invalid");
    return;
  }

  for (int i=0; i<manager->GetTotalNumberOfTargets(); i++)
  {
    vtkProstateNavTargetDescriptor *t=manager->GetTargetDescriptorAtIndex(i);
    if (fidNode->GetFiducialIndex(t->GetFiducialID())<0)
    {
      // fiducial not found, need to delete it
      manager->RemoveTargetDescriptorAtIndex(i);
      i--; // repeat check on the i-th element
    }
  }

  for (int i=0; i<fidNode->GetNumberOfFiducials(); i++)
  {
    int targetIndex=GetTargetIndexFromFiducialID(fidNode->GetNthFiducialID(i));
    if (targetIndex<0)
    {
      // New fiducial, create associated target
      vtkSmartPointer<vtkProstateNavTargetDescriptor> targetDesc=vtkSmartPointer<vtkProstateNavTargetDescriptor>::New();      
      
      targetDesc->SetFiducialID(fidNode->GetNthFiducialID(i));

      int needleIndex=manager->GetCurrentNeedleIndex();
      targetDesc->SetNeedleType(manager->GetNeedleType(needleIndex), manager->GetNeedleLength(needleIndex), manager->GetNeedleOvershoot(needleIndex));

      std::string FoR = this->GetFoRStrFromVolumeNodeID(manager->GetTargetingVolumeNodeID());
      targetDesc->SetFoRStr(FoR);

      manager->AddTargetDescriptor(targetDesc);
    }

    targetIndex=GetTargetIndexFromFiducialID(fidNode->GetNthFiducialID(i));
    if (targetIndex>=0)
    {
      // Update fiducial
      vtkProstateNavTargetDescriptor* targetDesc = manager->GetTargetDescriptorAtIndex(targetIndex);
      if (targetDesc!=NULL)
      {
        float *rasLocation=fidNode->GetNthFiducialXYZ(i);
        targetDesc->SetRASLocation(rasLocation[0], rasLocation[1], rasLocation[2]);

        float *rasOrientation=fidNode->GetNthFiducialOrientation(i);
        targetDesc->SetRASOrientation(rasOrientation[0], rasOrientation[1], rasOrientation[2], rasOrientation[3]);

        targetDesc->SetName(fidNode->GetNthFiducialLabelText(i));

        // :TODO: update needle,  etc. parameters ?

        // calculate targeting parameters for active needle, store in a target descriptor
        if(!manager->FindTargetingParams(targetDesc))
          {
          vtkErrorMacro("Error finding targeting parameters");
          }        
      }
      else
      {
        vtkErrorMacro("Invalid target descriptor");
      }
    }
    else
    {
      vtkErrorMacro("Invalid Fiducial ID");
    }
  }
}

int vtkProstateNavLogic::GetTargetIndexFromFiducialID(const char* fiducialID)
{
  if (fiducialID==NULL)
  {
    vtkWarningMacro("Fiducial ID is invalid");
    return -1;
  }
  vtkMRMLProstateNavManagerNode* manager=this->GUI->GetProstateNavManager();
  if (manager==NULL)
  {
    vtkErrorMacro("Manager is invalid");
    return -1;
  }
  for (int i=0; i<manager->GetTotalNumberOfTargets(); i++)
  {
    vtkProstateNavTargetDescriptor *t=manager->GetTargetDescriptorAtIndex(i);
    if (t->GetFiducialID().compare(fiducialID)==0)
    {
      // found the target corresponding to the fiducialID
      return i;
    }
  }
  return -1;
}
