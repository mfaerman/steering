/* $Id: priority_queue_simulation.cc,v 1.29 2002/10/21 00:03:24 mfaerman Exp $ */

#include "simulation.h"
#include "node_gen_creator.h"

PQueueSimulation::PQueueSimulation(void) :
  Simulation() {}

void PQueueSimulation::Check_Completed_Tasks(void) {
		     
  SG_Task* SG_completed;
  HTPTask node_task;
 
  //Simulate Simgrid tasks
  SG_completed = SG_simulate(GetNextInterval(),Get_Until_Task(),SG_SILENT);
  SortSimgridTaskArray(SG_completed); // sort for reproduceability; 
  //cout << "==================After SG_simulate===============\n";

  for (unsigned int i=0; SG_completed[i] != NULL; i++) {
    _total_completed_tasks++;
    node_task = *static_cast<HTPTask *>(SG_getTaskMetadata(SG_completed[i]));
    node_task->Complete();

    // Compute feedback of intermediate result from finished task
    (-(*node_task))->Set_Feedback(_pt_setup->Get_Sigma(), Fnc_Griewank);

    Print_Info(node_task);
    Update_Global_Minimum(node_task);
    
    // Generator_Group(node_task).GetGenerator()->Trigger_Steering_Event();
    NodeGenCreator(node_task).GetProduct()->Trigger_Steering_Event();
    //    node_task->Garbage_Collect();
  }
   if(_pt_setup->Get_Priority_Queue())
     if (! QueueCreator::Instance()->Empty())
       MoveFromQueueToGrid();
}

void  PQueueSimulation::Run(void) {

  while (Still_Steering()) {
    _flow_control.Update();
    Check_Completed_Tasks();
  }
  Finalization();
}




