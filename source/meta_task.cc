#include "meta_task.h"

MetaTask::MetaTask(NAME_TYPE name, WORKLOAD_TYPE work, 
		   PRIORITY_TYPE priority, unsigned int num_dim) :
  _current_step(0),
  _remaining_time(work),
  BaseTask(name,work,priority,num_dim)
{}

bool MetaTask::Completed(void) const{
  return _remaining_time == 0;
}

//  void MetaTask::Update(void) {


//  }

double MetaTask::GetOverhead(void) const {
  return Parameters::Instance()->GetOverhead();
}

double MetaTask::GetCycle(void) const {
  return Parameters::Instance()->GetCycle();
}

double MetaTask::SumPriorities(SG_Resource processor) {
  SG_Task * SG_Scheduled;
  double sum(0.0);
  unsigned int total(0);

  cout << Name << endl;
  SG_Scheduled = SG_getTasksScheduledOnResource(processor);
  for (unsigned int i = 0; SG_Scheduled[i] != NULL; i++) {
    sum += SG_getTaskPriority(SG_Scheduled[i]); 
    total++;
  }
  free(SG_Scheduled);

  sum = sum + Priority; // include running task
  total = total + 1;
  cout << "Number of Tasks = " << total << ", Sum = " << sum << endl;
  return sum;
}

unsigned int MetaTask::GetCurrentStep(void) {
  return _current_step;
}

double MetaTask::GetRemainingTime(void) {
  return _remaining_time;
}

double MetaTask::GetStepWorkload(SG_Resource processor) {

  double fraction, step;

  fraction = this->Priority / SumPriorities(processor);
  cout << "Resource Fraction = " << fraction << endl;
  step = min(fraction * GetCycle(), _remaining_time);
  _remaining_time = _remaining_time - step;

  cout << "Step Workload = " << step << " " << endl;
  return step;
}

bool MetaTask::Contention(SG_Resource processor) {

  SG_Task * scheduled(SG_getTasksScheduledOnResource(processor));
  SG_Task * running(SG_getTasksRunningOnResource(processor));

  bool contention(*scheduled || *running);

  free(scheduled);
  free(running);

  return contention;
}
                                                             
void MetaTask::ScheduleNextStep(SG_Resource processor, HTPTask & handle) {
  _current_step++;
  //Schedule(processor, metadata);
  ScheduleOnResource(processor, handle);
}

void MetaTask::Schedule(SG_Resource processor, void * metadata) {
  SG_Task overhead;
  SG_Resource pipe;
  bool contention;
  char step_name[20];
  char overhead_name[30];

  double effective_load;

  HTPTask * meta_overhead;

  contention = Contention(processor);

  sprintf(step_name,"%s_s%d", Name.c_str(), _current_step);

  if(contention)
    effective_load = GetStepWorkload(processor) +  GetOverhead();
  else
    effective_load = GetStepWorkload(processor);

  SG_ID = SG_newTask(SG_COMPUTATION, step_name, effective_load, metadata);
  SG_setTaskPriority(SG_ID, Priority);

  SG_scheduleTaskOnResource(SG_ID, processor);

  if (contention) {
    sprintf(overhead_name, "Overhead<=>%s", step_name);

    meta_overhead = new HTPTask(new TPTask(HBaseTask(new MetaTask(overhead_name, 0, 0, 0))));

    overhead = SG_newTask(SG_TRANSFER, overhead_name, 
			  GetOverhead(), meta_overhead);
    pipe = SG_newLink("fat-pipe", SG_FAT_PIPE, NULL, 
		      0.0, 0.0, NULL, 0.0, 1.0, NULL);

    SG_addDependency(overhead, SG_ID); // overhead starts after step to simplify implementation
    SG_scheduleTaskOnResource(overhead, pipe);
  }
}  


