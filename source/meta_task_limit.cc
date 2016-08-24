#include "meta_task_limit.h"


MetaTaskLimit::MetaTaskLimit(NAME_TYPE name, WORKLOAD_TYPE work, 
			     PRIORITY_TYPE priority, unsigned int num_dim) :
  MetaTask(name, work, priority, num_dim),
  _overhead(false)
{}


unsigned int MetaTaskLimit::GetNumProcessorTasks(SG_Resource & processor) const {
  SG_Task * scheduled(SG_getTasksScheduledOnResource(processor));
  unsigned int num_tasks(GetArraySize(scheduled));

  free(scheduled);
  return num_tasks;
}

Parameters * MetaTaskLimit::GetSetup(void) const {
  return Parameters::Instance();
}

unsigned int MetaTaskLimit::GetTaskLimit(SG_Resource & processor) const {
  if (GetSetup()->GetGridMapping() == ROUND_ROBIN)
    return GetSetup()->GetTaskLimit();
  else {
    return ((GridLimitedMapping *)GridCreator::Instance())->GetProcLimit(processor);
  }
}

void MetaTaskLimit::ScheduleNextStep(SG_Resource processor, HTPTask & handle) {
  _current_step++;
  //Schedule(processor, metadata);
  ScheduleOnResource(processor, handle);
}

bool MetaTaskLimit::HasOverhead(void) const{
  return _overhead;
}

void MetaTaskLimit::RemoveOverhead(void) {
  double new_cost;
  SG_Task new_sg_task;
  char name[20];
    
  assert(_overhead);
  _overhead = false;

  new_cost = SG_getTaskCost(SG_ID) - GetOverhead();
  sprintf(name, "%s", SG_getTaskName(SG_ID));
  new_sg_task = SG_newTask(SG_COMPUTATION, name, new_cost, SG_getTaskMetadata(SG_ID));

  SG_Clear();
 
  SG_ID = new_sg_task;
}

//  void MetaTaskLimit::AddOverhead(void) {
//    double new_cost;
//    SG_Task new_sg_task;
//    char name[20];

//    assert (!_overhead);
//    _overhead = true;

//    new_cost = SG_getTaskCost(SG_ID) + GetOverhead();
//    sprintf(name, "%s", SG_getTaskName(SG_ID));
//    new_sg_task = SG_newTask(SG_COMPUTATION, name, new_cost, SG_getTaskMetadata(SG_ID));
  
//    SG_Clear();
  
//    SG_ID = new_sg_task;
//  }

SCHEDULING_STATUS MetaTaskLimit::LimitTasks(SG_Resource & processor) {

  SG_Task* SG_Scheduled;
  vector<SG_Task> scheduled_tasks;
  vector<SG_Task>::iterator worst_scheduled;
  SCHEDULING_STATUS status;

  HTPTask node_worst;
  HMetaTaskLimit hbworst;

  //scheduled_tasks.push_back(this->SG_ID);

  if (GetNumProcessorTasks(processor) == GetTaskLimit(processor)) { 
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
      worst_scheduled = min_element(scheduled_tasks.begin(),
				    scheduled_tasks.end(),
				    Smaller_Priority());

      if(SG_getTaskPriority(*worst_scheduled) < this->Get_Priority()) {

	// unschedule and suspend worst node
	SG_unScheduleTask(*worst_scheduled);	
	node_worst = *static_cast<HTPTask *>(SG_getTaskMetadata(*worst_scheduled));
	hbworst = (HMetaTaskLimit(dynamic_cast<MetaTaskLimit *>
				  ((-(*node_worst)).get())));
	assert(hbworst);

	if (hbworst->HasOverhead())
	  hbworst->RemoveOverhead();
	MoveToSuspendQueue(node_worst);
	
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


void MetaTaskLimit::MoveToSuspendQueue(HTPTask & htpt) const {
  QueueCreator::Instance()->Insert(htpt);
}


void MetaTaskLimit::Schedule(SG_Resource processor, void * metadata) {
//    SG_Task overhead;
//    SG_Resource pipe;
  bool contention(false);
  char step_name[20];
//    char overhead_name[30];

  double effective_load(0.0);

//    HTPTask * meta_overhead;

  contention = Contention(processor);

  sprintf(step_name,"%s_s%d", Name.c_str(), _current_step);

  //  assert(!_overhead);
  if(contention) {
    _overhead = true;
    effective_load = GetStepWorkload(processor) +  GetOverhead();
  }
  else {
    _overhead = false;
    effective_load = GetStepWorkload(processor);
  }

  SG_ID = SG_newTask(SG_COMPUTATION, step_name, effective_load, metadata);
  SG_setTaskPriority(SG_ID, Priority);

  LimitTasks(processor);

//    if (LimitTasks(processor) != SUSPENDED) {
//    //    AddOverhead();
//      // add overhead of context-switch
//      if (contention) {
//        sprintf(overhead_name, "Overhead<=>%s", step_name);

//        meta_overhead = new HTPTask(new TPTask(HBaseTask(new MetaTask(overhead_name, 0, 0, 0))));

//        overhead = SG_newTask(SG_TRANSFER, overhead_name, 
//  			    GetOverhead(), meta_overhead);
//        pipe = SG_newLink("fat-pipe", SG_FAT_PIPE, NULL, 
//  			0.0, 0.0, NULL, 0.0, 1.0, NULL);

//        SG_addDependency(overhead, SG_ID);        // overhead starts after step to simplify implementation
//        SG_scheduleTaskOnResource(overhead, pipe);
//      }
//    }
}  


