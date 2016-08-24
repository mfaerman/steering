// $Id$

#include "node_generator.h"

bool Generator_Group_Limited::Spawn_Neighbor_Configurations(HTPTask & node_task, 
							    float radius, 
							    PRIORITY_TYPE priority) {
				
  static unsigned int task_index(_pt_setup->Get_Num_Dim()*2);
  char name [10]; 
  CONFIGURATION_TYPE neighbor;
  SG_Resource processor;
  HTPTask it, generator;
  HBaseTask pt_task;
  bool spawned(false);

  if ((-(*node_task->parent()))->Get_Feedback() > 
      (-(*node_task))->Get_Feedback())
    generator = node_task;
  else {
    generator = HTPTask(dynamic_cast<TPTask *>((node_task->parent()).get()));
    // node_task won't generate descendants so it is removed
    node_task->Remove_Node();
  }

  // conditions to spawn offsprings
  if(generator->Get_Level() < _pt_setup->Get_Max_Levels() &&
     generator->GetTrials() < _pt_setup->GetMaxTrials()) {
    
    spawned = true;
    for(unsigned int i = 0; i < _pt_setup->Get_Num_Tasks(); i++) {
      it = 
	dynamic_cast<TPTask *>((generator->
				Insert(HBaseTask
				       (TaskCreator(Name_Policy(name, task_index), 
						    Workload_Policy(), 
						    priority, 
						    generator->content()->num_dim())
					.GetProduct()))).get());
    
      neighbor =  
	Spherical_Neighborhood(&generator->content()->Get_Config(), radius);
      pt_task = it->content();
      pt_task->Set_Config(neighbor);
    
      PriorityQueue::Instance()->Insert(node_task);
      task_index++;
    }
  }
  if(spawned)
    generator->NewTrial();

  return spawned;
}
