#ifndef __vtkTRProstateBiopsyTargetingStep_h
#define __vtkTRProstateBiopsyTargetingStep_h

#include "vtkTRProstateBiopsyStep.h"

class vtkKWFrame;
class vtkKWEntry;
class vtkKWLabel;
class vtkKWEntryWithLabel;
class vtkKWEntrySet;
class vtkKWLoadSaveButton;
class vtkKWText;
class vtkKWPushButton;
class vtkKWMenuButton;
class vtkKWMenuButtonWithLabel;
class vtkKWMultiColumnList;
class vtkKWMultiColumnListWithScrollbars;
class VTK_TRPROSTATEBIOPSY_EXPORT vtkTRProstateBiopsyTargetingStep : public vtkTRProstateBiopsyStep
{
public:
  static vtkTRProstateBiopsyTargetingStep *New();
  vtkTypeRevisionMacro(vtkTRProstateBiopsyTargetingStep,vtkTRProstateBiopsyStep);
  void PrintSelf(ostream& os, vtkIndent indent);

  virtual void ShowUserInterface();
  virtual void ProcessGUIEvents(vtkObject *caller, unsigned long event, void *callData);  

  // Description:
  // Callbacks to capture mouse click on image
  virtual void ProcessImageClickEvents(vtkObject *caller, unsigned long event, void *callData);

  // Description
  // Reset
  virtual void Reset();


protected:
  vtkTRProstateBiopsyTargetingStep();
  ~vtkTRProstateBiopsyTargetingStep();

  void ShowLoadVolumeControls();
  void ClearLoadVolumeControls();

  void ShowManualAddTargetControls();
  void ClearManualAddTargetControls();

  void ShowNeedleTypeAndTargetsListControls();
  void ClearNeedleTypeAndTargetsListControls();

  void ShowMessageAndDeleteControls();
  void ClearMessageAndDeleteControls();


  void AddGUIObservers();
  void RemoveGUIObservers();
  void RecordClick(double rasPoint[3]);

  // load targeting volume controls
  vtkKWFrame *LoadVolumeDialogFrame;
  vtkKWLoadSaveButton *LoadTargetingVolumeButton;

  
  // RAS coordinates for manually entry of target
  vtkKWFrame *RASManualEntryFrame;
  vtkKWLabel *RASManualEntryLabel;
  vtkKWEntrySet      *RASManualEntry; // read only
  vtkKWPushButton *RASManualEntryButton;

  vtkKWFrame *ListFrame;
  // needle type list
  vtkKWMenuButtonWithLabel *NeedleTypeMenuList;  
  // multi-column list to display target, params, etc
  // this needle will display targets corresponding to the needle type selected  
  vtkKWMultiColumnListWithScrollbars *TargetsMultiColumnList;

  //BTX
  // Description:
  // The column orders in the list box
  enum
    {
    TargetNumberColumn = 0,
    NeedleTypeColumn = 1,
    RASLocationColumn = 2,
    ReachableColumn = 3,
    RotationColumn = 4,
    NeedleAngleColumn = 5,    
    DepthColumn = 6,
    NumberOfColumns = 7,
    };
  //ETX

  // delete button
  vtkKWFrame *MsgAndDelFrame;
  vtkKWText *Message;
  vtkKWPushButton *DeleteButton;

  bool ProcessingCallback;


private:
  vtkTRProstateBiopsyTargetingStep(const vtkTRProstateBiopsyTargetingStep&);
  void operator=(const vtkTRProstateBiopsyTargetingStep&);
};

#endif
