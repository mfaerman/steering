#ifndef __VIDCOMMUNICATOR_H__
#define __VIDCOMMUNICATOR_H__

#include <stdio.h>
#include <string>
#include <strstream>
#include <list>
using namespace std;

#include "simgrid.h"
#include "main.h"

#include "ParameterSpacePoint.h"
#include "VIInterface.h"

#define VI_PROJECT_NAME "simulated_user"
#define VI_PROJECT_DESCRIPTION "Simulated User Project"
#define VI_PROJECT_NUM_REPEATS 1

class VidCommunicator {

  // singleton implementation
  public:
    static VidCommunicator *Instance();
  private:
    static VidCommunicator * _instance;

  public:
    VidCommunicator(void);
    ~VidCommunicator(void);

    void submitTask(SG_Task t);
    SG_Task *getCompletedTasks(void);

  private:
    //ParameterSpacePoint *getParameterSpacePoint(BaseTask *task);

    // internal list of submitted tasks. TODO: remove
    list<SG_Task> submitted_list;

    // bogus host for Simgrid simulations to keep 
    // Marcio's code happy
    SG_Resource bogus_host;

    // Interface to the VI
    VIInterface *vi_interface;
    VIProject *vi_project;

    // Setup the VI 
    void setupVI(void);
    // Shutdown the VI
    void shutdownVI(void);

    // set a task's feedback value from the value returned by the VI
    void setFeedback(SG_Task sg_task, string result);

    // Extract a simgrid task from the submitted list when a corresponding
    // result has arrived from the VI
    SG_Task extractMatchingSGTask(ParameterSpacePoint psp);

    // Parse the result string to get the feedback
    float getFeedback(string result);

    // Print te submitted list
    void printSubmittedList();

};

#endif
