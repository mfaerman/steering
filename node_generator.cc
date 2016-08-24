// $Id: node_generator.cc,v 1.41 2002/12/07 23:12:14 mfaerman Exp $

#include "simulation.h"
#include "node_generator.h"
#include "main.h"

#include<functional>
  
// (required) definition of static members
Parameters *  Generator_Group::_pt_setup = NULL;
Grid * Generator_Group::_pt_grid = NULL;
RandomGenerator * Generator_Group::_pt_rg = NULL;


// constructor
Generator_Group::Generator_Group(HTPTask & ttask) : 
  _current_node(ttask),
  _parent(dynamic_cast<TPTask *>((ttask->parent()).get()))
{
  _relevant_results = vector<HTPTask>(min(_pt_setup->Get_Num_Relevant(),_parent->children()->size()));
}

void Generator_Group::Set_Configuration(Parameters * pt_setup) {
  _pt_setup = pt_setup;
}

void Generator_Group::Set_Grid(Grid * pt_grid) {
  _pt_grid = pt_grid;
}

Simulation * Generator_Group::GetSim(void) const {
  return SimulationCreator::Instance();
}


bool Generator_Group::Spawn_Neighbor_Configurations(HTPTask & node_task, 
						    float radius, 
						    PRIORITY_TYPE priority) {
				
  static unsigned int task_index(_pt_setup->Get_Num_Dim()*2);
  char name [10]; 
  CONFIGURATION_TYPE neighbor;
  SG_Resource processor;
  HTPTask it, generator;
  HBaseTask pt_task;
  bool spawned(false);
    
  // assert(node_task->GetTrials() == 0);
  // node_task->ResetTrials();

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
    
      //processor = _pt_grid->Choose_Processor();
      processor = 0;
      ScheduleOnResource(processor, it);
      task_index++;
    }
  }
  if(spawned)
    generator->NewTrial();

  return spawned;
}

PRIORITY_TYPE Generator_Group::GetPriorityRandomTask(void) {
  cerr << "Error using Generator_Group::GetPriorityRandomTask" << endl;
  exit(-1);
}

void Generator_Group::GenerateRandomTasks(unsigned int count) {
  char name[30]; 
  CONFIGURATION_TYPE neighbor;
  HTPTask newtask;
  RandomGenerator * rg(RandomGenCreator::Instance());
  static unsigned int task_index(1000000);
  static unsigned int root_count(0);

  root_count++;
  sprintf(name, "Root_%d", root_count);
  HTPTask newroot (new TPTask(HBaseTask(new BaseTask(name, 1, 1, 1))));
  //newroot = new HTPTask(new TPTask(HBaseTask(TaskCreator(name, 0, 0, 0).GetProduct())));
  //  newroot = Simulation::GetRoot();
  for(unsigned int i = 0; i < count; i++) {
    newtask = 
      dynamic_cast<TPTask *>(newroot->
			     Insert(HBaseTask(TaskCreator
					      (Name_Policy(name, task_index), 
					       Workload_Policy(), 
					       GetPriorityRandomTask(), 
					       Parameters::Instance()->Get_Num_Dim())
					      .GetProduct())).get());

    vector<float>center(Parameters::Instance()->Get_Num_Dim(),0);

    neighbor = Spherical_Neighborhood(&center,_pt_setup->Get_Scatter_Radius());
    (-(*newtask))->Set_Config(neighbor);

    if(_pt_setup->Get_Priority_Queue())
      QueueCreator::Instance()->Insert(newtask);
    else
      (-(*newtask))->Schedule(_pt_grid->Choose_Processor(), new HTPTask(newtask));
    task_index++;
  }
  Simulation::NewRandomRestart();
  //  (newroot)->NewTrial();
}

//  void Generator_Group::GenerateRandomTasks(unsigned int count) {
//    char name[30]; 
//    CONFIGURATION_TYPE neighbor;
//    HTPTask newroot;
//    HTPTask newtask;
//    RandomGenerator * rg(RandomGenCreator::Instance());
//    static unsigned int task_index(1000000);
//    static unsigned int root_count(0);

//    root_count++;
//    sprintf(name, "Root_%d", root_count);

//    //newroot = new HTPTask(new TPTask(HBaseTask(TaskCreator(name, 0, 0, 0).GetProduct())));
//    //  newroot = Simulation::GetRoot();
//    for(unsigned int i = 0; i < count; i++) {
//      newtask = 
//        dynamic_cast<TPTask *>(GetSim()->GetRoot().
//  			     Insert(HBaseTask(TaskCreator
//  					      (Name_Policy(name, task_index), 
//  					       Workload_Policy(), 
//  					       1.0, 
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

//////////////////////////////////////////////////////////////////////////////
void Generator_Group::Trigger_Steering_Event(void) {  

  float radius(_pt_setup->Get_Initial_Radius());

  vector <HTPTask> * pt_children = (vector <HTPTask> *) _parent->children();

  stable_sort(pt_children->begin(), pt_children->end(), Smaller_Feedback());
  copy_n(pt_children->begin(), _relevant_results.size(), _relevant_results.begin());

  // Remove nodes that won't generate descendants
  pt_children->erase(pt_children->begin() + _relevant_results.size(), pt_children->end());
		     
  radius = radius / pow(2, _parent->Get_Level()); 
  cout << "Radius = " << radius << endl;

  if (! Schedule_New_Tasks(radius))
    GenerateRandomTasks(1);

//    // Remove children that will not generate offsprings
//    pt_children->clear();


  //pt_children->erase(pt_children->begin() + _relevant_results.size(), pt_children->end());
  //for_each(pt_children->begin(), pt_children->end(), mem_fun_ref(&TPTask::Remove_Node));

}




