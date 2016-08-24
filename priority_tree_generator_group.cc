// $Id: priority_tree_generator_group.cc,v 1.15 2002/12/07 23:12:14 mfaerman Exp $

#include "node_generator.h"
#include "main.h"

PriorityTreeGeneratorGroup::PriorityTreeGeneratorGroup(HTPTask & ttask) : 
  Generator_Group(ttask) {}

bool PriorityTreeGeneratorGroup::Schedule_New_Tasks(float radius) { 
  bool spawned(false);


  switch(Parameters::Instance()->GetSteeringRegime()) {
    case STEADY_STATE:
      if(Spawn_Neighbor_Configurations
	 (_current_node, 
	  radius, 
	  Priority_Policy((-(*_current_node))->Get_Feedback())))
	spawned = true;
      break;
    case LIMITED:
    case SELECT_BEST:
      for (unsigned int i = 0; i < _relevant_results.size(); i++) 
	if(Spawn_Neighbor_Configurations
	   (_relevant_results[i], 
	    radius, 
	    Priority_Policy((-(*_relevant_results[i]))->Get_Feedback())))
	  spawned = true;
      break;
    default:
      cout << "Steering Regime Error!" << endl;
      exit (-1);
  }
  return spawned;
}

PRIORITY_TYPE PriorityTreeGeneratorGroup::GetPriorityRandomTask(void) {
  return LOW_PRIORITY;
}

//  void PriorityTreeGeneratorGroup::GenerateRandomTasks(unsigned int count) {
//    char name[30]; 
//    CONFIGURATION_TYPE neighbor;
//    HTPTask newtask;
//    RandomGenerator * rg(RandomGenCreator::Instance());
//    static unsigned int task_index(1000000);
//    static unsigned int root_count(0);

//    root_count++;
//    sprintf(name, "Root_%d", root_count);
//    HTPTask newroot (new TPTask(HBaseTask(new BaseTask(name, 1, 1, 1))));
//    //newroot = new HTPTask(new TPTask(HBaseTask(TaskCreator(name, 0, 0, 0).GetProduct())));
//    //  newroot = Simulation::GetRoot();
//    for(unsigned int i = 0; i < count; i++) {
//      newtask = 
//        dynamic_cast<TPTask *>(newroot->
//  			     Insert(HBaseTask(TaskCreator
//  					      (Name_Policy(name, task_index), 
//  					       Workload_Policy(), 
//  					       LOW_PRIORITY, 
//  					       Parameters::Instance()->Get_Num_Dim())
//  					      .GetProduct())).get());

//      vector<float>center(Parameters::Instance()->Get_Num_Dim(),0);

//      neighbor = Spherical_Neighborhood(&center,_pt_setup->Get_Scatter_Radius());
//      (-(*newtask))->Set_Config(neighbor);

//      if(_pt_setup->Get_Priority_Queue())
//        QueueCreator::Instance()->Insert(newtask);
//      else
//        (-(*newtask))->Schedule(_pt_grid->Choose_Processor(), new HTPTask(newtask));
//      task_index++;
//    }
//    Simulation::NewRandomRestart();
//    //  (newroot)->NewTrial();
//  }

