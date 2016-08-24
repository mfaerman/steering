#include "order_task.h"


OrderTask::OrderTask(NAME_TYPE name, WORKLOAD_TYPE work, 
		     PRIORITY_TYPE priority, unsigned int num_dim) :
  BaseTask(name, work, priority, num_dim) {}

void OrderTask::ReOrderScheduledTasks(SG_Resource processor) {

  SG_Task* SG_Scheduled;
  vector<SG_Task> scheduled_tasks;

  scheduled_tasks.push_back(this->SG_ID);

  SG_Scheduled = SG_getTasksScheduledOnResource(processor);
  for (unsigned int i=0; SG_Scheduled[i] != NULL; i++) {
    if (SG_getTaskState(SG_Scheduled[i]) != SG_RUNNING) {
      SG_unScheduleTask(SG_Scheduled[i]);
      scheduled_tasks.push_back(SG_Scheduled[i]);
    }
    else {
      cout << "Error unscheduling tasks" << endl;
      exit(-1);
    }
  }
  
  sort(scheduled_tasks.begin(),scheduled_tasks.end(),Larger_Priority());
  
  for (unsigned int i=0; i < scheduled_tasks.size(); i++)
    SG_scheduleTaskOnResource(scheduled_tasks[i], processor);
    
  free(SG_Scheduled);
}

void OrderTask::Schedule(SG_Resource processor, void * metadata) {

  SG_ID = SG_newTask(SG_COMPUTATION, NULL, Workload, metadata);
  SG_setTaskPriority(SG_ID, Priority);
  if (Parameters::Instance()->Is_VI_Simulated_User()) {
#ifdef VI_USER
    VidCommunicator *vidcommunicator = VidCommunicator::Instance();
    vidcommunicator->submitTask(SG_ID);
#else
    cerr << "Support for Virtual Instrument not compiled in\n";
    exit(1);
#endif
  }  
  else {
    ReOrderScheduledTasks(processor);
  }
}  
