
#ifndef __vtkMRMLPerkStationModuleNode_h
#define __vtkMRMLPerkStationModuleNode_h

#include <string>
#include <vector>

#include "vtkMRML.h"
#include "vtkMRMLNode.h"
#include "vtkMRMLStorageNode.h"

#include "vtkMatrix4x4.h"
#include "vtkTransform.h"
#include "vtkImageData.h"
#include "vtkSmartPointer.h"

#include "vtkPerkStationModule.h"
#include "vtkPerkStationPlan.h"

#include "vtkMRMLFiducialListNode.h"
#include "vtkMRMLLinearTransformNode.h"


class vtkImageData;
class vtkMRMLScalarVolumeNode;



// PerkStation specific enums -------------------------------------------------

enum
{
  WORKPHASE_CALIBRATION = 0,
  WORKPHASE_PLANNING = 1,
  WORKPHASE_INSERTION = 2,
  WORKPHASE_VALIDATION = 3
};

enum VolumeType
{
  VOL_GENERIC, // any other than the specific volumes 
  VOL_CALIBRATE_PLAN,  
  VOL_VALIDATION
};

enum PatientPositionEnum
  {
  PPNA, // Not defined
  HFP,  // Head First-Prone
  HFS,  // Head First-Supine
  HFDR, // Head First-Decubitus Right
  HFDL, // Head First-Decubitus Left
  FFDR, // Feet First-Decubitus Right
  FFDL, // Feet First-Decubitus Left
  FFP,  // Feet First-Prone
  FFS   // Feet First-Supine
  };

struct OverlayHardware
{
  std::string Name;
  double SizeX;
  double SizeY;
  bool FlipHorizontal;
  bool FlipVertical;
};

//BTX
// Struct to hold calibration data.
class OverlayCalibration
{
public:
  
  OverlayCalibration()
  {
    this->Name = "";
    this->SecondMonitorHorizontalFlip = false;
    this->SecondMonitorRotation = 0.0;
    this->SecondMonitorRotationCenter[ 0 ] = 0.0;
    this->SecondMonitorRotationCenter[ 1 ] = 0.0;
    this->SecondMonitorTranslation[ 0 ] = 0.0;
    this->SecondMonitorTranslation[ 1 ] = 0.0;
    this->SecondMonitorVerticalFlip = false;
    this->TableAtOverlay = 0.0;
  }
  
  std::string Name;
  double SecondMonitorRotation;
  double SecondMonitorRotationCenter[ 2 ];
  double SecondMonitorTranslation[ 2 ];
  
  bool SecondMonitorHorizontalFlip;
  bool SecondMonitorVerticalFlip;
  
  double TableAtOverlay; // Table position when target area is under the overlay laser.
};
//ETX

// ----------------------------------------------------------------------------


class
VTK_PERKSTATIONMODULE_EXPORT
vtkMRMLPerkStationModuleNode
: public vtkMRMLNode
{

public:

  static vtkMRMLPerkStationModuleNode *New();
  vtkTypeMacro( vtkMRMLPerkStationModuleNode, vtkMRMLNode );
  void PrintSelf( ostream& os, vtkIndent indent );

  virtual vtkMRMLNode* CreateNodeInstance();
  
  virtual void ReadXMLAttributes( const char** atts);
  virtual void WriteXML( ostream& of, int indent );
  
    // Copy the node's attributes to this object
  virtual void Copy( vtkMRMLNode *node );

    // Get unique node XML tag name (like Volume, Model)
  virtual const char* GetNodeTagName() {
    return "PS";
  };
  
  
  // Calibration parameters ---------------------------------------------------
  
  double GetSecondMonitorRotation() const;
  void SetSecondMonitorRotation( double rot );
  
  void GetSecondMonitorRotationCenter( double* rotCent) const;
  void SetSecondMonitorRotationCenter( const double* rotCent);
  
  bool GetSecondMonitorVerticalFlip() const;
  void SetSecondMonitorVerticalFlip( bool flip );
  
  bool GetSecondMonitorHorizontalFlip() const;
  void SetSecondMonitorHorizontalFlip( bool flip );
  
  void GetSecondMonitorTranslation( double* tx ) const;
  void SetSecondMonitorTranslation( const double* tx );
  
  double GetTableAtOverlay() const;
  void SetTableAtOverlay( double pos );
  
  vtkGetMacro( CurrentSliceOffset, double );
  vtkSetMacro( CurrentSliceOffset, double );
  
  vtkGetMacro( HardwareIndex, int );
  vtkSetMacro( HardwareIndex, int );
  
    // Calibration list management.
  
  unsigned int AddCalibration( OverlayCalibration* newCalibration );
  int RemoveCalibrationAtIndex( unsigned int index );
  OverlayCalibration* GetCalibrationAtIndex( unsigned int index );
  int GetNumberOfCalibrations() { return this->CalibrationList.size(); };
  vtkGetMacro( CurrentCalibration, int );
  int SetCurrentCalibrationIndex( int index );
  
  
  // Plan parameters ----------------------------------------------------------
  
  vtkGetVector3Macro( PlanEntryPoint, double );
  vtkSetVector3Macro( PlanEntryPoint, double );
  
  vtkGetVector3Macro( PlanTargetPoint, double );
  vtkSetVector3Macro( PlanTargetPoint, double );
  
  vtkGetMacro( TiltAngle, double );
  vtkSetMacro( TiltAngle, double );
  
  void SetSliceToRAS( vtkMatrix4x4* matrix ) {
    this->SliceToRAS->DeepCopy( matrix );
  };
  
  vtkMatrix4x4* GetSliceToRAS() {
    return this->SliceToRAS.GetPointer();
  };
  
  void AddCurrentPlan();
  
    // Plan list management.
  
  unsigned int AddPlan( vtkPerkStationPlan* newPlan );
  int RemovePlanAtIndex( unsigned int index );
  vtkPerkStationPlan *GetPlanAtIndex( unsigned int index );
  int GetNumberOfPlans() { return this->PlanList.size(); };
  vtkGetMacro( CurrentPlanIndex, int );
  int SetCurrentPlanIndex( int index );
  
  
    // Insertion parameters ---------------------------------------------------
  
  vtkGetMacro( ReferenceBodyToolPort, int );
  vtkSetMacro( ReferenceBodyToolPort, int );
  
  vtkGetMacro( NeedleToolPort, int );
  vtkSetMacro( NeedleToolPort, int );
  
  void SetTrackerToPhantomMatrix( vtkMatrix4x4 *matrix ) {
    this->TrackerToPhantomMatrix->DeepCopy( matrix );
  };
  
  vtkMatrix4x4* GetTrackerToPhantomMatrix() {
    return this->TrackerToPhantomMatrix;
  };
  
  void SetPhantomToImageRASMatrix( vtkMatrix4x4* matrix ) {
    this->PhantomToImageRASMatrix->DeepCopy( matrix );
  };
  
  vtkMatrix4x4* GetPhantomToImageRASMatrix() {
    return this->PhantomToImageRASMatrix;
  };
  
  
  // Description:
  // Get/Set tool tip offset. This offset (x, y, z) is added to needle tool
  // coordinate measured by the tracker. The needle tool is usually
  // pre-calibrated by the manufacturer and the tip offset is stored in its SROM,
  // but it is not loaded on an old tracker (e.g. Aurora firmware Rev.006 or
  // earlier version). Also it is usually recommended
  // to do pivoting calibration just before each experiment in order to ensure
  // the accuracy. So this variable is used to modify the offset in such case.
  
  void SetToolTipOffset( double *tool_tip_offset ) {
    memcpy( this->ToolTipOffset, tool_tip_offset, sizeof( double ) * 3 );
  };
  
  double *GetToolTipOffset() {
    return this->ToolTipOffset;
  };
  
  
    // Validation parameters --------------------------------------------------
  
  vtkGetVector3Macro(ValidateEntryPoint, double);
  vtkSetVector3Macro(ValidateEntryPoint, double);

  vtkGetVector3Macro(ValidateTargetPoint, double);
  vtkSetVector3Macro(ValidateTargetPoint, double);


    // Common parameters ------------------------------------------------------
  
  vtkGetStringMacro( PlanningVolumeRef );
  vtkSetStringMacro( PlanningVolumeRef );
  
  vtkGetStringMacro( ValidationVolumeRef );
  vtkSetStringMacro( ValidationVolumeRef );
  
  vtkGetStringMacro( VolumeInUse );
  vtkSetStringMacro( VolumeInUse );
  
  vtkMRMLScalarVolumeNode* GetPlanningVolumeNode();
  
  void SetPlanningVolumeNode( vtkMRMLScalarVolumeNode *planVolNode );
  
    // validation volume node
  vtkMRMLScalarVolumeNode *GetValidationVolumeNode()
    { return this->ValidationVolumeNode; };
  void SetValidationVolumeNode( vtkMRMLScalarVolumeNode *validationVolNode );
  
  vtkSetMacro( TimeOnCalibrateStep, double );
  vtkGetMacro( TimeOnCalibrateStep, double );
  
  vtkSetMacro( TimeOnPlanStep, double );
  vtkGetMacro( TimeOnPlanStep, double );
  
  vtkSetMacro( TimeOnInsertStep, double );
  vtkGetMacro( TimeOnInsertStep, double );
  
  vtkSetMacro( TimeOnValidateStep, double );
  vtkGetMacro( TimeOnValidateStep, double );
  
  
    // Non loadable parameters.
  
  vtkMRMLFiducialListNode* GetPlanMRMLFiducialListNode() {
    return this->PlanMRMLFiducialListNode.GetPointer();
  }
  
  
  vtkMRMLScalarVolumeNode* GetActiveVolumeNode();
  
    // Update the stored reference to another node in the scene
  virtual void UpdateReferenceID( const char *oldID, const char *newID );
  
  void AddVolumeInformationToList( vtkMRMLScalarVolumeNode *volNode,
                                   const char *diskLocation, char *type ); 
  
  int GetNumberOfSteps();
  int GetCurrentStep();
  int GetPreviousStep();
  int SwitchStep( int newStep );
  const char* GetStepName( int i );
  
  PatientPositionEnum GetPatientPosition();
  
  //BTX
  std::vector< OverlayHardware > GetHardwareList();
  //ETX
  
  
  // Computations -------------------------------------------------------------
  
  double GetCurrentTablePosition();
  
  double GetEntryPointError();
  double GetTargetPointError();
  
  double GetActualPlanInsertionAngle();
  double GetActualPlanInsertionDepth();
  
  double GetValidationDepth();
  
  
protected:
  
  vtkMRMLPerkStationModuleNode();
  ~vtkMRMLPerkStationModuleNode();
  vtkMRMLPerkStationModuleNode( const vtkMRMLPerkStationModuleNode& );
  void operator=( const vtkMRMLPerkStationModuleNode& );
  
  void InitializeFiducialListNode();
  
  
  // Calibration parameters ---------------------------------------------------
  
  //BTX
  std::vector< OverlayCalibration* > CalibrationList;
  int CurrentCalibration; // If <0, no calibration is selected.
  unsigned int CalibrationUID;
  //ETX
  
  int HardwareIndex;
  
    // Plan parameters --------------------------------------------------------
  
  double PlanEntryPoint[ 3 ];
  double PlanTargetPoint[ 3 ];
  
  double TiltAngle;
  
  //BTX
  
  std::vector< vtkPerkStationPlan* > PlanList;
  int CurrentPlanIndex; // If <0, no plan is selected.
  
  vtkSmartPointer< vtkMatrix4x4 > SliceToRAS;
  
  unsigned int PlanUID;
  
    // Insertion parameters ---------------------------------------------------
  
  vtkSmartPointer< vtkMatrix4x4 > TrackerToPhantomMatrix;
  vtkSmartPointer< vtkMatrix4x4 > PhantomToImageRASMatrix;
  double ToolTipOffset[ 3 ];
  
  int ReferenceBodyToolPort;
  int NeedleToolPort;
  //ETX
  
  
    // Validation parameters --------------------------------------------------
  
  double ValidateEntryPoint[ 3 ];
  double ValidateTargetPoint[ 3 ];
  
  
    // Common parameters ------------------------------------------------------
  
  char* PlanningVolumeRef;
  char* ValidationVolumeRef;
  char* VolumeInUse; 
  
  vtkMRMLScalarVolumeNode* PlanningVolumeNode;
  vtkMRMLScalarVolumeNode* ValidationVolumeNode;
  
  double CurrentSliceOffset;    // In RAS.
  PatientPositionEnum PatientPosition;
  
  double TimeOnCalibrateStep;
  double TimeOnPlanStep;
  double TimeOnInsertStep;
  double TimeOnValidateStep;
  
  //BTX
  vtkSmartPointer< vtkMRMLFiducialListNode > PlanMRMLFiducialListNode;
  
  vtkSmartPointer< vtkStringArray > StepList;
  //ETX
  int CurrentStep;
  int PreviousStep;
  
  
  //keep track of all the volumes that were opened, maintain a list
  //BTX
  typedef struct {
    std::string Type;
    std::string DiskLocation;
    bool Active;
    vtkMRMLScalarVolumeNode* VolumeNode;
  } VolumeInformationStruct;
  
  std::vector< VolumeInformationStruct > VolumesList;
  
  std::vector< OverlayHardware > HardwareList;
  //ETX
};

#endif

