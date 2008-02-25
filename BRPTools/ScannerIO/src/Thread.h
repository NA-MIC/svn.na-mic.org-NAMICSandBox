/*=========================================================================

  Program:   Imaging Scanner Interface
  Module:    $RCSfile: $
  Language:  C++
  Date:      $Date: $
  Version:   $Revision: $

  Copyright (c) Brigham and Women's Hospital. All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __THREAD_H
#define __THREAD_H

#ifdef _DEBUG
#define _DEBUG_THREAD
#endif //_DEBUG

#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/errno.h>
#include <signal.h>

#include <unistd.h>
#include <string.h>

// ACE Thread
#include <ace/Thread.h>
#include <ace/Synch.h>
#include <ace/Thread_Mutex.h>
#include <ace/Condition_T.h>

class Thread {

 public:
  // thread status
  enum {  
    RUN,
    STOP,
    SLEEP,
    WAIT,
  };

 protected:
  // Trigger Mode
  enum { 
    NORMAL,
    COUNT,
    COUNT_WITH_ARG
  };

 public:
  Thread();
  virtual        ~Thread();

  int             Sleep();
  int             Resume();
  int             Run();
  int             Stop();

  inline int      GetStatus() { return runStatus;};

  void            PullTrigger();
  int             PullTrigger(void*);

 protected:

  virtual void    Process() = 0;
  static void*    CallProcess(void*);
  void            Exit();  // called from thread itself

  void*           WaitForTrigger();
  inline void     ResetTriggerCounter()   { triggerCnt = 0; };
  inline void     SetTriggerMode(int mode){ triggerMode = mode; };
  inline void     EnableTrigger()         { fTrigger = 1; };
  inline void     DisableTrigger()        { fTrigger = 0; };

 protected:
  //pthread_t       thread;
  ACE_thread_t    thread;
  ACE_hthread_t   hthread;

  int             runStatus;
  //pthread_mutex_t mtxStatus;     // Critical section in status changinging.
  ACE_Thread_Mutex mtxStatus;
  int             fTrigger;      // Use trigger or not.
  //pthread_cond_t  trigger;       // Condition variable to keep waiting.
  ACE_Thread_Condition<ACE_Thread_Mutex> trigger;       // Condition variable to keep waiting.
  //pthread_mutex_t mtxTriggerCnt; // Mutex for trigger.
  ACE_Thread_Mutex mtxTriggerCnt;
  int             triggerMode;   // Trigger mode. See enum declaration above.
  int             triggerCnt;    // Trigger counter

 private: 
  // old FIFO used in Trigger Counting with Argument Mode
  void**          queue;
  int             nQueue;
  int             pStart, pEnd;
  void            InitFifo(int);
  void            DeleteFifo();
  int             Push(void*);
  void*           Pull();

  // new FIVO using Smart pointer
  //std::queue<TObjectType> TriggerQueue;


};

#endif //__THREAD_H
