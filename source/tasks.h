#ifndef __TASKS_H__
#define __TASKS_H__

#include<string>
#include<vector>
#include<deque>
#include<iostream>
#include<iomanip>
#include <math.h>
#include <stdio.h>
#include <iostream>
using namespace std;

#include"simgrid.h"
#include"config.h"
#include"Handle.h" 
#ifdef VI_USER
#include "vidcommunicator.h"
#endif

#define MAX_FEEDBACK 1e20
#define LOW_PRIORITY 1e-6

//typedef enum {SCHEDULED, SUSPEND_TASK} SCHEDULING_STATUS;

typedef double PRIORITY_TYPE;
typedef float  WORKLOAD_TYPE;
typedef float  FEEDBACK_TYPE;
typedef string NAME_TYPE;
typedef vector<float> CONFIGURATION_TYPE;


typedef struct _SG_Task_Info {
  double SG_Start;
  double SG_Finish;
  double SG_Priority;
  SG_Resource Processor_Handle;
  string Processor_Name;
};

class BaseTask : public Handleable {
  
private:
  FEEDBACK_TYPE Feedback;
  CONFIGURATION_TYPE * Configuration;
  unsigned int Num_Dim;
  struct _SG_Task_Info * ptr_SG_Task_Info;

protected:
  SG_Task SG_ID;
  WORKLOAD_TYPE Workload;
  PRIORITY_TYPE Priority;
  //void Schedule(SG_Resource);
  NAME_TYPE Name;

public:
  BaseTask(NAME_TYPE, WORKLOAD_TYPE, PRIORITY_TYPE, unsigned int);
  BaseTask();
  BaseTask(const BaseTask& t);
  BaseTask(const BaseTask* t);

  ~BaseTask();
  
  BaseTask& operator=(const BaseTask & t);
  
  void Set_Rand_Config(float radius);
  CONFIGURATION_TYPE& Get_Config(void) const {return (*Configuration);}
  void Set_Priority(PRIORITY_TYPE p) {Priority = p;}
  PRIORITY_TYPE Get_Priority(void) const {return Priority;}
  //  void Set_Feedback(float value);
  FEEDBACK_TYPE Get_Feedback(void) const {return(Feedback);}

  virtual void Schedule(SG_Resource, void *);
  void Print_Conf(void);
  SG_Task SG_Id(void) const {return SG_ID;}
  int operator==(const SG_Task ID) const {return (ID == SG_ID);}
  int operator==(CONFIGURATION_TYPE c) 
     {return (*Configuration == c);}
  NAME_TYPE name(void) const {return Name;}
  unsigned int num_dim(void) const {return Num_Dim;}
  void Set_Config(CONFIGURATION_TYPE); 
  void SG_Clear(void);
  void Assign_SG_Info(BaseTask &);
  void Print_SG_Info(void);

  void Set_Feedback(float, FEEDBACK_TYPE (*f) (float, CONFIGURATION_TYPE&));
  void Set_Feedback(float);
};
 
typedef Handle<BaseTask> HBaseTask;

#endif







