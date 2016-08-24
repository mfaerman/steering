#include "simulation.h"

void SimulationLimitedRandom::Get_Completed_Tasks(void) {
  SG_Task* SG_completed;

  _completed_tasks.clear();

  //Simulate Simgrid tasks
  SG_completed = SG_simulate(GetNextInterval(),
			     Get_Until_Task(), 
			     GetVerbosity());
  assert(SG_completed != NULL);
  SortSimgridTaskArray(SG_completed); // sort for reproduceability; 
  cout << "==================After SG_simulate===============\n";
  
  HTPTask node_task;
      
  for (unsigned int i=0; SG_completed[i] != NULL; i++) {
    HTPTask * handle(static_cast<HTPTask *>
		     (SG_getTaskMetadata(SG_completed[i])));
    node_task = *handle;
    delete handle;
    
    if (!IsOverhead(SG_completed[i])) {
      HMetaTaskLimitRandom ptr_meta_task
	(HMetaTaskLimitRandom(dynamic_cast<MetaTaskLimitRandom *>
			      ((-(*node_task)).get())));
      assert(ptr_meta_task != NULL);
      
      if (ptr_meta_task->Completed()) {
	_completed_tasks.push_back(node_task);
      }
      else {  // not completed yet
	ptr_meta_task->ScheduleNextStep(SG_getTaskResource
					(SG_completed[i]), node_task);
	cout << "Current Step: " 
	     << ptr_meta_task->GetCurrentStep();
	cout << " Remaining Time = " 
	     << ptr_meta_task->GetRemainingTime() 
	     << endl;    
	ptr_meta_task->SG_Clear();
	
      }
    }     
  }
  free(SG_completed);     
}
