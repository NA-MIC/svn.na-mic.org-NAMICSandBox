/*==========================================================================

  Portions (c) Copyright 2010-2011 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See Doc/copyright/copyright.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   3D Slicer
  Module:    $HeadURL: $
  Date:      $Date: $
  Version:   $Revision: $

==========================================================================*/
/// vtkAbdoNavGUI - manages the GUI of the abdominal navigation module
///
/// This class implements the GUI associated with the abdominal navigation
/// module.

#ifndef __vtkAbdoNavGUI_h
#define __vtkAbdoNavGUI_h

/* AbdoNav includes */
#include "vtkAbdoNavLogic.h"
#include "vtkAbdoNavWin32Header.h"

/* Slicer includes */
#include "vtkSlicerModuleGUI.h"

/* Slicer forward declarations */
class vtkSlicerNodeSelectorWidget;

/* VTK forward declarations */
class vtkKWPushButton;
class vtkKWSeparator;

class VTK_AbdoNav_EXPORT vtkAbdoNavGUI : public vtkSlicerModuleGUI
{
 public:
  //----------------------------------------------------------------
   // Usual VTK class functions.
  static vtkAbdoNavGUI *New();
  vtkTypeRevisionMacro(vtkAbdoNavGUI, vtkSlicerModuleGUI);
  void PrintSelf(ostream& os, vtkIndent indent);
  // Get the module's category
  const char *GetCategory() const { return "IGT"; }

  // Get methods on class members (no Set methods required)
  vtkGetObjectMacro(Logic, vtkAbdoNavLogic);

  // API for setting VolumeNode, VolumeLogic and
  // for both setting and observing them.
  void SetModuleLogic(vtkSlicerLogic *logic) { this->SetLogic(vtkObjectPointer(&this->Logic), logic); }

  // Build the GUI.
  virtual void BuildGUI();

  //----------------------------------------------------------------
  // Define behavior at module initialization, startup and exit.
  virtual void Init();  // called upon Slicer startup; not implemented
  virtual void Enter(); // called upon entering the module
  virtual void Exit();  // called upon exiting the module; not implemented

  //----------------------------------------------------------------
  // Add/Remove observers, event handlers, etc.
  virtual void AddGUIObservers();
  virtual void RemoveGUIObservers();
  void         AddLogicObservers();
  void         RemoveLogicObservers();
  void         AddMRMLObservers();
  void         RemoveMRMLObservers();

  //----------------------------------------------------------------
  // Mediator methods for processing events invoked by logic, GUI,
  // MRML, timer or mouse
  virtual void ProcessGUIEvents(vtkObject *caller, unsigned long event, void *callData);
  virtual void ProcessLogicEvents(vtkObject *caller, unsigned long event, void *callData);
  virtual void ProcessMRMLEvents(vtkObject *caller, unsigned long event, void *callData);
  void ProcessTimerEvents();
  void HandleMouseEvent(vtkSlicerInteractorStyle *style);
  //BTX
  static void  DataCallback(vtkObject *caller, unsigned long eid, void *clientData, void *callData);
  //ETX
  void UpdateAll();

 protected:
  //----------------------------------------------------------------
  // Usual VTK class functions.
  vtkAbdoNavGUI();
  virtual ~vtkAbdoNavGUI();

 private:
  //----------------------------------------------------------------
  // Usual VTK class functions.
  vtkAbdoNavGUI(const vtkAbdoNavGUI&);  // not implemented
  void operator=(const vtkAbdoNavGUI&); // not implemented

  //----------------------------------------------------------------
  // Logic values.
  vtkAbdoNavLogic* Logic;
  int TimerFlag;
  int TimerInterval;
  vtkCallbackCommand* DataCallbackCommand;

  //----------------------------------------------------------------
  // Helper functions to build the different GUI frames.
  void BuildGUIHelpFrame();
  void BuildGUIConnectionFrame();

  //----------------------------------------------------------------
  // Widgets of the connection frame.
  vtkSlicerNodeSelectorWidget* GuidanceNeedleSelectorWidget;
  vtkSlicerNodeSelectorWidget* CryoprobeSelectorWidget;
  vtkKWSeparator* SeparatorBeforeButtons;
  vtkKWPushButton* ConfigurePushButton;
  vtkKWPushButton* ResetPushButton;
  vtkKWPushButton* PausePushButton;
};

#endif // __vtkAbdoNavGUI_h
