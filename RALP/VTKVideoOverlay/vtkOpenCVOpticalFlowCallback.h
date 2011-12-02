/*=========================================================================

  Program:        OpenCV support for Visualization Toolkit
  Module:         vtkOpenCVRendererDelegate.h
  Contributor(s): Junichi Tokuda (tokuda@bwh.harvard.edu)

  Copyright (c) Brigham and Women's Hospital, All rights reserved.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkOpenCVRendererDelegate - abstract specification for actor
// .SECTION Description

// .SECTION See Also
// vtkRendererDelegate vtkRenderWindow vtkRendererDelegate vtkCamera vtkLight vtkVolume

#ifndef __vtkOpenCVOpticalFlowCallback_h
#define __vtkOpenCVOpticalFlowCallback_h


// VTK header files and prototypes
#include "vtkObject.h"
#include "vtkCommand.h"

// OpenCV header files and protoptyes
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/highgui/highgui.hpp"

#define BUFFER_SIZE 300
#define NGRID_X  20
#define NGRID_Y  20
#define TIME_POINTS (30*30)

class VTK_COMMON_EXPORT vtkOpenCVOpticalFlowCallback : public vtkCommand
{
  
public:
  vtkTypeMacro(vtkOpenCVOpticalFlowCallback, vtkCommand);

  static vtkOpenCVOpticalFlowCallback *New()
  {
    return new vtkOpenCVOpticalFlowCallback;
  };
  
  int InitBuffer();
  int StoreMotionField();
  int ProcessMotion(std::vector<cv::Point2f>& vector);
  virtual void Execute(vtkObject *vtkNotUsed(caller), unsigned long eventId, void *callData);
  
protected:
  vtkOpenCVOpticalFlowCallback();
  ~vtkOpenCVOpticalFlowCallback();
  
  //int           OpticalFlowTrackingFlag;
  cv::Mat GrayImage;
  cv::Mat PrevGrayImage;
  cv::Mat SwapTempImage;

  std::vector< cv::Point2f >  Points[2];
  std::vector< cv::Point2f >  GridPoints[2];// = {0,0};
  std::vector< cv::Point2f >  RVector;
  
  char*         OpticalFlowStatus;
  cv::Point2f * MotionFieldBuffer;
  int           BufferIndex;
  int           FileIndex;
  

  
};


#endif
