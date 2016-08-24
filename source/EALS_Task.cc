#include "EALS_Task.h"

void EALS_Task::SpawnLS_Sniff(unsigned int length,
			      PRIORITY_TYPE priority) {
  
  char name [10]; 
  CONFIGURATION_TYPE neighbor;
  TTask * it;
  BaseTask * pt_task;

  it = parent->Insert(EALS_Task(Name_Policy(name, task_index), 
				Workload_Policy(), 
				priority, 
				parent->content().num_dim()));
    
  pt_task = it->pt_content();
  pt_task->Set_Config(neighbor);
    
  ScheduleOnResource(Grid::Instance()->Choose_Processor(), it);
  
  task_index++;
}











