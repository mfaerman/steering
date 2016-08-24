// $Id: simulation.cc,v 1.80 2003/04/06 05:44:30 mfaerman Exp $

#include "config.h"
#ifdef VI_USER
#include "vidcommunicator.h"
#endif

#include <unistd.h>
#include <time.h>

#include "simulation.h"
#include "node_gen_creator.h"
#include "main.h" //to be removed later on

// constructor
Simulation::Simulation(void) :   
  _pt_setup(Parameters::Instance()),
  _ttree(HBaseTask(new BaseTask("Root", 1, 1, 1))),
  _TPTask_Minimum(HBaseTask(new BaseTask("Minimum", 1, 1, 1))),
  _total_completed_tasks(0),
  _generated_tasks(0),
  _flow_control(Parameters::Instance()->Get_Burst_Limit()),
  _pt_grid(GridCreator::Instance(PROCESSOR, 
				 Parameters::Instance()->Get_Num_Proc(), 
				 Parameters::Instance()->Get_Proc_Speed()))
{
  Initialize_Static_Methods();
  Format_Output_Stream(); 
  srand(_pt_setup->Get_Random_Seed()); 
}


void Simulation::Initialize_Static_Methods(void) {
  Generator_Group::Set_Configuration(_pt_setup);
  Generator_Group::Set_Grid(_pt_grid);
  RandomGenCreator::Set_Configuration(_pt_setup);
  FlowControl::Set_Configuration(_pt_setup);
  FlowControl::Set_Grid(_pt_grid);
  NodeGenCreator::Set_Configuration(_pt_setup);
  SimulationCreator::Set_Configuration(_pt_setup);
  TaskCreator::Set_Configuration(_pt_setup);
}

SG_Task Simulation::Get_Until_Task(void) {
  if (_pt_setup->Get_Period() > 0) 
    return NULL;
  else
    return SG_ANY_TASK; 
}


double Simulation::GetNextInterval(void) {
  double interval;

  if(_pt_setup->GetFlowControlEnabled()) {
    if (_pt_grid->GetElapsedTime() < _pt_setup->Get_Token_Period()) {
      interval =  _pt_setup->Get_Token_Period() - _pt_grid->GetElapsedTime();
    }
    else
      interval = _pt_setup->Get_Token_Period();
  }
  else
    interval = _pt_setup->Get_Period();

  _pt_grid->UpdateElapsedTime();
  return interval;  
}


void Simulation::Finalization(void) {
  cout << "\nTask Minimum : "; 

  cout << "Level = " << _TPTask_Minimum.Get_Level() << ", ";  
  //  cout << "Parent = '" << ((TPTask&)_TPTask_Minimum.GetParent()).pt_content()->name() << "', ";
  cout << "Trial = " << HTPTask(dynamic_cast<TPTask *>
				(_TPTask_Minimum.parent().get()))->GetTrials() << ", ";
  
  Print_Info(-_TPTask_Minimum);
//    cout << "\nTotal Tasks to Execute = " << Total_Tasks_to_Execute(*_pt_setup) 
//         << endl;
  cout << "\nTotal Random Restarts  = " << _random_restarts;
  cout << "\nTotal Executed Tasks   = " << _total_completed_tasks << "\n\n";

//    if (Parameters::Instance()->Get_Priority_Queue())
//      QueueCreator::Instance()->Finalization();
  QueueCreator::Instance()->Finalization();
  _pt_grid->Finalization();

#ifdef VI_USER
  VidCommunicator *vidcommunicator = VidCommunicator::Instance();
  delete vidcommunicator;
#endif
  
}

void Simulation::MoveFromQueueToGrid(void) {
  HTPTask node_task;

  while (_flow_control.GetToken() &&
	 ! QueueCreator::Instance()->Empty()) {
    node_task = QueueCreator::Instance()->GetNextTask();
    if (node_task != NULL)
      (-(*node_task))->Schedule(_pt_grid->Choose_Processor(), new HTPTask(node_task));
  }
}

bool Simulation::Meet_Steering_Criteria(HTPTask & node_task) {

  bool meet_criteria(false);
  HBaseTask pt_task(-(*node_task));

  if(node_task->All_Done())
    if(!Parameters::Instance()->Get_Priority_Queue()) {
      if (node_task->Get_Level() < _pt_setup->Get_Max_Levels())
	meet_criteria = true;
    }
    else
      meet_criteria = true;

  if (meet_criteria) {
  	cout << "All Done! ---> ";
	Print_Info(pt_task);
  }
  return meet_criteria;
}

void Simulation::Update_Global_Minimum(HTPTask & ttask) {
  if ((ttask->content()->Get_Feedback() < 
       _TPTask_Minimum.content()->Get_Feedback()) &&
      (ttask->Get_Level() > 1))   // skip initialization values 
    {
      _TPTask_Minimum <= (*ttask);
      cout << "Task Minimum : "; 
      cout << "Level = " << _TPTask_Minimum.Get_Level() << " ,";
      Print_Info(-_TPTask_Minimum);       
    }
}

bool Simulation::IsOverhead(SG_Task sg) {
  return (SG_getTaskType(sg) == SG_TRANSFER);
}

void Simulation::Get_Completed_Tasks(void) {
  SG_Task* SG_completed;

  _completed_tasks.clear();

  if (Parameters::Instance()->Is_VI_Simulated_User()) {
#ifdef VI_USER
    VidCommunicator *vidcommunicator = VidCommunicator::Instance();
    SG_completed = vidcommunicator->getCompletedTasks();
    cout << "==================After Getting results from vid===============\n";
    //cout << "TIME " << time(NULL) << "   BEST SO FAR: " << (-_TPTask_Minimum).Get_Feedback() << "\n";
    cout << "sleeping 5 seconds....";
    sleep(5);
    cout << "\n";
#else
    cerr << "Support for Virtual Instrument Not compiled in!\n";
    exit(1);
#endif
  } else {
    //Simulate Simgrid tasks
    SG_completed = SG_simulate(GetNextInterval(),Get_Until_Task(), GetVerbosity());
    assert(SG_completed != NULL);
    SortSimgridTaskArray(SG_completed); // sort for reproduceability; 
    cout << "==================After SG_simulate===============\n";
  }
  
  if(Parameters::Instance()->GetTimeSharing() == CONTINUOUS) {  
    for (unsigned int i=0; SG_completed[i] != NULL; i++) {
      HTPTask * handle(static_cast<HTPTask *>(SG_getTaskMetadata(SG_completed[i])));
      _completed_tasks.push_back(*handle);
      delete handle;
    }
  }
  else 
    if (Parameters::Instance()->GetTimeSharing() == CONTEXT_SWITCHING) {
      HTPTask node_task;
      
      for (unsigned int i=0; SG_completed[i] != NULL; i++) {
	HTPTask * handle(static_cast<HTPTask *>(SG_getTaskMetadata(SG_completed[i])));
	node_task = *handle;
	delete handle;
	
	if (IsOverhead(SG_completed[i])) {
	  // node_task->Remove_Node(); // node_task is destructed automatically since it is a local variable
	}
	else {
	  HMetaTaskLimit ptr_meta_task
	    (HMetaTaskLimit(dynamic_cast<MetaTaskLimit *>
			    ((-(*node_task)).get())));
	  assert(ptr_meta_task != NULL);

	  if (ptr_meta_task->Completed()) {
	    _completed_tasks.push_back(node_task);
	  }
	  else {
	    ptr_meta_task->ScheduleNextStep(SG_getTaskResource(SG_completed[i]), node_task);
	    cout << "Current Step: " 
		 << ptr_meta_task->GetCurrentStep();
	    cout << " Remaining Time = " 
		 << ptr_meta_task->GetRemainingTime() 
		 << endl;    
	    ptr_meta_task->SG_Clear();
	    
	  }
	}
      }
    }
  free(SG_completed);     
}


void Simulation::Check_Completed_Tasks(void) {
  HTPTask node_task;	
	     
  Get_Completed_Tasks();
  for (unsigned int i=0; i < _completed_tasks.size(); i++) {
    _total_completed_tasks++;

    node_task = _completed_tasks[i];
    node_task->Complete();
    
    // Compute feedback of intermediate result from finished task
    if (!Parameters::Instance()->Is_VI_Simulated_User()) {
      (-(*node_task))->Set_Feedback(_pt_setup->Get_Sigma(), _pt_setup->GetFunction());
    }
    
    Update_Global_Minimum(node_task);

    Print_Info(node_task);

    // Checks if it is time to steer
    if (Meet_Steering_Criteria(node_task)) {
      Generator_Group * generator(NodeGenCreator(node_task).GetProduct());
      generator->Trigger_Steering_Event();
      delete generator;
    }
    
    if (HTPTask(dynamic_cast<TPTask *>(node_task->parent().get()))->GetTrials() == 
	Parameters::Instance()->GetMaxTrials()) {
      //node_task->Garbage_Collect();
      node_task->Remove_Node();
    }
  }
  if(_pt_setup->Get_Priority_Queue())
    if (! QueueCreator::Instance()->Empty())
      MoveFromQueueToGrid();

  _completed_tasks.clear();

//    sterile tasks are the ones to be deallocated:
//    (parent) reach max trials
//    not selected to generate  
//    dropped by flow control
  // removal should be bottom-up (leaves->root)

  // Nullify pointer members after deletion
}


void Simulation::Init_Steering(void) {
 
  char name [10]; 
  HTPTask it;
  HBaseTask pt_task;
  CONFIGURATION_TYPE conf;
  
  for (unsigned int i = 0; i < _pt_setup->Get_Num_Dim()*2; i++) {
    sprintf(name,"T%d", _generated_tasks); // task naming
    it = 
      HTPTask(dynamic_cast<TPTask *>
	      (_ttree.Insert
	       (HBaseTask
		(TaskCreator(Name_Policy(name, _generated_tasks), 
			     Workload_Policy(), 
			     Start_Priority_Policy(), 
			     _pt_setup->Get_Num_Dim()).GetProduct())).get()));
    it->Set_Fraction(1);
    it->Set_Level(1); // level 0 is tree root
    pt_task = -(*it);
    conf = Scatter(i,  _pt_setup->Get_Scatter_Radius(), _pt_setup->Get_Num_Dim());
    pt_task->Set_Config(conf);
    if(_pt_setup->Get_Priority_Queue())
      QueueCreator::Instance()->Insert(it);
    else
      pt_task->Schedule(_pt_grid->Choose_Processor(), new HTPTask(it));
    _generated_tasks++;
  }    
  if(_pt_setup->Get_Priority_Queue())
    if (! QueueCreator::Instance()->Empty())
      MoveFromQueueToGrid();
}

bool Simulation::Still_Steering(void) {
  return  ((-_TPTask_Minimum)->Get_Feedback() > _pt_setup->Get_Threshold()) &&  
    (_total_completed_tasks < Parameters::Instance()->GetNumberExecute());
}


void Simulation::Run(void) {
  while (Still_Steering()) {
    _flow_control.Update();
    Check_Completed_Tasks();
  }
  cout << "Finalizing\n";
  Finalization();
  //  cout << "Finalized\n";
}

unsigned int Simulation::_random_restarts(0);

void Simulation::NewRandomRestart(void) {
  _random_restarts++;
}

unsigned int Simulation::GetRandomRestarts(void) {
  return _random_restarts;
}

unsigned int Simulation::GetGeneratedTasks(void) {
  return _generated_tasks;
}

TPTask & Simulation::GetRoot(void) {
  return _ttree;
}

