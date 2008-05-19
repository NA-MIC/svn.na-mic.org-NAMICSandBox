/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: cisstDeviceInterface.i.h,v 1.4 2006/05/13 21:26:50 kapoor Exp $

  Author(s):  Anton Deguet
  Created on:   2006-05-02

--- begin cisst license - do not edit ---

CISST Software License Agreement(c)

Copyright 2005 Johns Hopkins University (JHU) All Rights Reserved.

This software ("CISST Software") is provided by The Johns Hopkins
University on behalf of the copyright holders and
contributors. Permission is hereby granted, without payment, to copy,
modify, display and distribute this software and its documentation, if
any, for research purposes only, provided that (1) the above copyright
notice and the following four paragraphs appear on all copies of this
software, and (2) that source code to any modifications to this
software be made publicly available under terms no more restrictive
than those in this License Agreement. Use of this software constitutes
acceptance of these terms and conditions.

The CISST Software has not been reviewed or approved by the Food and
Drug Administration, and is for non-clinical, IRB-approved Research
Use Only.

IN NO EVENT SHALL THE COPYRIGHT HOLDERS AND CONTRIBUTORS BE LIABLE TO
ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION,
EVEN IF THE COPYRIGHT HOLDERS AND CONTRIBUTORS HAVE BEEN ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.

THE COPYRIGHT HOLDERS AND CONTRIBUTORS SPECIFICALLY DISCLAIM ANY
EXPRESS OR IMPLIED WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE, AND NON-INFRINGEMENT.

THE SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS." THE COPYRIGHT HOLDERS AND CONTRIBUTORS HAVE NO OBLIGATION TO
PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

--- end cisst license ---

*/


/* This file is to be used only for the generation of SWIG wrappers.
   It includes all the regular header files from the libraries as well
   as some header files created only for the wrapping process

   For any wrapper using %import "cisstDeviceInterface.i", the file
   cisstDeviceInterface.i.h should be included in the %header %{ ... %} section
   of the interface file. */


#ifndef _cisstDeviceInterface_i_h
#define _cisstDeviceInterface_i_h


/* Put header files here */
#include "cisstCommon/cisstCommon.i.h"
#include "cisstVector/cisstVector.i.h"

#include "cisstDeviceInterface/ddiTypes.h"
#include "cisstDeviceInterface/ddiBasicTypeDataObject.h"
#include "cisstDeviceInterface/ddiDynamicVectorDataObject.h"
#include "cisstDeviceInterface/ddiFixedSizeVectorDataObject.h"

#include "cisstDeviceInterface/ddiCommandBase.h"

#include "cisstDeviceInterface/ddiDeviceInterface.h"
#include "cisstDeviceInterface/ddiCompositeDevice.h"
#include "cisstDeviceInterface/ddiLoPoMoCo.h"
#include "cisstDeviceInterface/ddiTwoLoPoMoCo.h"
#include "cisstDeviceInterface/ddiThreeLoPoMoCo.h"
#include "cisstDeviceInterface/ddiFourLoPoMoCo.h"
#include "cisstDeviceInterface/ddiMEI.h"


#include "cisstDeviceInterface/ddiDMM16AT.h"
#include "cisstDeviceInterface/ddiRMM1612.h"

#include "cisstDeviceInterface/ddiCompositeMRRobotController.h"


#endif // _cisstDeviceInterface_i_h


// ****************************************************************************
//                              Change History
// ****************************************************************************
//
//  $Log: cisstDeviceInterface.i.h,v $
//  Revision 1.4  2006/05/13 21:26:50  kapoor
//  cisstDeviceInterface and IRE: Added wrappers for 2, 3, 4 lopomocos.
//
//  Revision 1.3  2006/05/07 04:45:26  kapoor
//  cisstDeviceInterface and IRE: Fixed printing of dataobjects through python.
//
//  Revision 1.2  2006/05/03 21:37:36  anton
//  cisstDeviceInterface wrappers: Added prelimiary wrapping for LoPoMoCo and MEI
//
//  Revision 1.1  2006/05/03 02:42:57  anton
//  cisstDeviceInterface wrappers: Added wrappers for ddiTypes, i.e. vectors of
//  data (work in progress ...)
//
//
// ****************************************************************************
