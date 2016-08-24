#include "meta_task_limit_random.h"

MetaTaskLimitRandom::MetaTaskLimitRandom(NAME_TYPE name, 
					 WORKLOAD_TYPE work, 
					 PRIORITY_TYPE priority, 
					 unsigned int num_dim) :
  MetaTaskLimit(name, work, priority, num_dim) 
{}



SCHEDULING_STATUS MetaTaskLimitRandom::LimitTasks(SG_Resource & processor) {

  SG_Task* SG_Scheduled;
  vector<SG_Task> scheduled_tasks;
  vector<SG_Task>::iterator chosen;
  SCHEDULING_STATUS status;
  RandomGenerator * rg(RandomGenCreator::Instance());

  HTPTask node_chosen;
  HMetaTaskLimitRandom hbchosen;

  //scheduled_tasks.push_back(this->SG_ID);

  if (GetNumProcessorTasks(processor) == GetSetup()->GetTaskLimit()) { 
    SG_Scheduled = SG_getTasksScheduledOnResource(processor);

    for (unsigned int i=0; SG_Scheduled[i] != NULL; i++) {
      if (SG_getTaskState(SG_Scheduled[i]) != SG_RUNNING) {
	scheduled_tasks.push_back(SG_Scheduled[i]);
      }
      else {
	cout << "Error unscheduling tasks" << endl;
	exit(-1);
      }
    }

    if (!scheduled_tasks.empty()) {
      chosen = &scheduled_tasks
	[(unsigned int) rint(rg->GetRand(scheduled_tasks.size() - 1))];

      if(rg->GetRand(1) < 0.5) {

	// unschedule and suspend a random node
	SG_unScheduleTask(*chosen);	
	node_chosen = *static_cast<HTPTask *>(SG_getTaskMetadata(*chosen));
	hbchosen = (HMetaTaskLimitRandom(dynamic_cast<MetaTaskLimitRandom *>
					 ((-(*node_chosen)).get())));
	assert(hbchosen);

	if (hbchosen->HasOverhead())
	  hbchosen->RemoveOverhead();
	MoveToSuspendQueue(node_chosen);
	
	// schedule task step
	//	this->AddOverhead();
	SG_scheduleTaskOnResource(this->SG_ID, processor); 
	status = SCHEDULED;
      }
      else {
	// suspend task step
	
	if (this->HasOverhead())
	  this->RemoveOverhead();
	MoveToSuspendQueue(*static_cast<HTPTask *>(SG_getTaskMetadata(this->SG_ID)));
	status = SUSPENDED;
      }
    }
    free(SG_Scheduled);
  }
  else {
    SG_scheduleTaskOnResource(this->SG_ID, processor); // processor is idle
    status = SCHEDULED;
  }
}
