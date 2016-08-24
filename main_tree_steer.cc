#include<deque.h>
#include<string>
#include<list>
#include<math.h>
#include<stdio.h>
#include<iostream>
#include<iomanip>
#include<iosfwd>
#include<algorithm>
using namespace std;

#include"helper.h"
#include"tasks.h"
#include"tree_tasks.h"
#include"cost_fnc.h"
#include"parameters.h"
#include"grid.h"
#include"array.h"

#define SEED 2

#ifndef MAXFLOAT
#define MAXFLOAT 1e20
#endif

void Format_Output_Stream(void);
void Schedule_New_Tasks(vector<BaseTask> *, POLICY_TYPE);
void Run_Steering(unsigned int, float);
bool Steer_Now(SG_Task*, double, double, unsigned int);
vector<SG_Resource> Initialize_Grid(unsigned int);
void Spawn_Neighbor_Configurations(BaseTask, float, unsigned int, PRIORITY_TYPE);


//list of active tasks
deque<BaseTask> tasks;
//global list of processors

BaseTask Root_Task("Root", 1, 1, 1);
TTask tree(Root_Task);



void Format_Output_Stream(void) {
  cout.setf(ios::fixed,ios::floatfield);
  cout.precision(2);
}


CONFIGURATION_TYPE Spherical_Neighborhood(CONFIGURATION_TYPE * conf, 
					  float radius) {
  float coord = 0.0;
  CONFIGURATION_TYPE neighbor(conf->size());
  CONFIGURATION_TYPE real_coordinates(conf->size());
  
  float prev_coord = 0;
  for (unsigned int i = 0; i < conf->size(); i++) {
     coord =  2 * radius*cos(prev_coord) 
      * ((double)rand() / (double)(RAND_MAX+1.0)) - radius*cos(prev_coord);
    real_coordinates[i] = coord;
    prev_coord = coord;
    neighbor[i] = coord + (*conf)[i];
  }
  return(neighbor);
}

char * Name_Policy(char * name, int i) {
  sprintf(name,"T%d", i);
  return (name);
}
  
float Workload_Policy() {
  return 10;  
}

double Start_Priority_Policy() {
  return 1;
}

unsigned int Neighborhood_Policy(int i) {
  return i;
}

double Priority_Policy(double p) {
  //  return 1024/pow(2,p);
  return (1/p);
}

unsigned int Total_Tasks_to_Execute(Parameters & conf) {
  // calculated according to steering tree structure
  unsigned int sum;

  sum = 2*conf.Get_Num_Dim();    // initialization tasks

  for (unsigned int i = 2; i <= conf.Get_Max_Levels(); i++) 
    sum += conf.Get_Num_Tasks() * (int) pow(conf.Get_Num_Relevant(),(i - 1));
  
  return sum;
}
 
CONFIGURATION_TYPE Scatter (int point, float radius, int num_dim) {
  
  CONFIGURATION_TYPE config(num_dim, 0);
  if (point < num_dim)
    config[point % num_dim] =  radius;
  else 
    config[point % num_dim] = -radius; 
  return config;
}

void Schedule_Initial_Tasks_to_Run(Grid & grid, 
      				   float init_radius,
				   unsigned int num_dim) {
  char name [10]; 
  TTask * it;
  BaseTask * pt_task;
  CONFIGURATION_TYPE conf;
  
  for (unsigned int i = 0; i < num_dim*2; i++) {
    sprintf(name,"T%d", i); // task naming
    it = tree.Insert(BaseTask(Name_Policy(name, i), 
			  Workload_Policy(), 
			  Start_Priority_Policy(), 
			  num_dim));
    it->Set_Fraction(1);
    it->Set_Level(1); // level 0 is tree root
    pt_task = it->pt_content();
    conf = Scatter(i, init_radius, num_dim);
    pt_task->Set_Config(conf);
    pt_task->Schedule(grid.Choose_Processor(), it);
  }    
}


void Initialize_Steering (Grid & grid, Parameters & conf) {
  // Initialize_Parameter_Space();
  Schedule_Initial_Tasks_to_Run(grid,
				conf.Get_Scatter_Radius(),
				conf.Get_Num_Dim());
}





void Spawn_Neighbor_Configurations(TTask * parent, 
				   float radius, 
				   unsigned int num_neighbors,
				   PRIORITY_TYPE priority,
				   Grid & grid) {

  static unsigned int task_index = 0;
  char name [10]; 
  CONFIGURATION_TYPE neighbor;
  SG_Resource processor;
  TTask * it;
  BaseTask * pt_task;

  for(unsigned int i = 0; i < num_neighbors; i++) {
     it = parent->Insert(BaseTask(Name_Policy(name, task_index), 
			  Workload_Policy(), 
			  priority, 
			  parent->content().num_dim()));
    
    neighbor =  
      Spherical_Neighborhood(&parent->content().Get_Config(), radius);
    pt_task = it->pt_content();
    pt_task->Set_Config(neighbor);
    
    processor = grid.Choose_Processor();
    pt_task->Schedule(processor, it);
    task_index++;
  }
}


void Print_Info(BaseTask * it_task) {

  FEEDBACK_TYPE feedback;

  cout << "Task '" << it_task->name() << "', ";

  it_task->Print_SG_Info();
  it_task->Print_Conf();

  feedback = it_task->Get_Feedback();
  cout  << ", Feedback = "  << feedback << endl;
}



bool Look_at_Results(FEEDBACK_TYPE best, float threshold, Grid & grid) {
  return (best > threshold);
}


void Schedule_New_Tasks(vector<TTask *> & Locations, 
			POLICY_TYPE policy,
			float radius,
			unsigned int num_selected_tasks,
			Grid & grid) {
  switch(policy) {    

  case INITIAL: {
    for (unsigned int i=0; i < num_selected_tasks; i++)
      Spawn_Neighbor_Configurations(Locations[i], radius, 4, 2, grid);
  
    for (unsigned int i = Locations.size() - 1; 
	 i > Locations.size() -2; i--) {
      Spawn_Neighbor_Configurations(Locations[i], 100, 1, 1, grid);    
    }
    break;
  }

  case SIMPLE: {
    // no differentiated priorities ==> set all priorities to 1

    for (unsigned int i=0; i < Locations.size(); i++)
      Spawn_Neighbor_Configurations(Locations[i], 
				    radius, 
				    Neighborhood_Policy(num_selected_tasks), 
				    1,
				    grid); 
    break;
  }

  case PRIORITY_TREE: {

    for (unsigned int i=0; i < Locations.size(); i++) {

      FEEDBACK_TYPE feedback = Locations[i]->content().Get_Feedback();
      Spawn_Neighbor_Configurations(Locations[i], 
				    radius, 
				    Neighborhood_Policy(num_selected_tasks), 
				    Priority_Policy(feedback),
				    grid);  
    }
    break;
  }

  } //end switch(policy)
}

void Trigger_Steering_Event(TTask * pt,
			    unsigned int size,
			    vector<TTask *> &relevant_results,
			    bool Merge_With_Previous_Results,
			    Parameters & conf,
			    Grid & grid) { 

  float radius = conf.Get_Initial_Radius();

  vector <TTask *> * pt_children = (vector <TTask *> *) pt->children();

  sort(pt_children->begin(), pt_children->end(), Smaller_Feedback());
  copy_n(pt_children->begin(), size, relevant_results.begin());
  radius = radius / pow(2, pt->Get_Level()); 
  cout << "Radius = " << radius << endl;
    
  Schedule_New_Tasks(relevant_results, conf.Get_Policy(), 
		     radius, conf.Get_Num_Tasks(), grid);

  for (vector <TTask *>::iterator i = pt_children->begin() + size; 
       i != pt_children->end(); ){
    (**i).content().SG_Clear();
    (**i).Remove();
  }
  if (pt->Get_Level() > 0) {
    pt->content().SG_Clear();  // Free SG_Task memory
    pt->Remove_Node();         // Free TTask node
  }
}

 
bool Meet_Steering_Criteria(SG_Task sgt) {
BaseTask * pt_task;
TTask * node_task;

 vector<FEEDBACK_TYPE> sorted; 

  node_task = (TTask *) SG_getTaskMetadata(sgt);
  pt_task = node_task->pt_content();

  //  if (pt_task->Get_Feedback() < sorted.back()) {
  //}

    if (node_task->All_Done()) {
      cout << "All Done! ---> ";
      Print_Info(pt_task);
      return true;
    }
    else 
      return false;
}

bool
Check_Completed_Tasks(vector<TTask *>::iterator previous_results,
		      float sigma, unsigned int size,
		      vector<TTask *> &relevant_results,
		      Parameters & conf,
		      Grid & grid) {		      		      

  SG_Task* SG_completed;
  int level;
  TTask * node_task;
  bool Continue;
  static unsigned int total_completed_tasks = 0;
  static FEEDBACK_TYPE average_feedback = 0;

  BaseTask * pt_task;

  BaseTask Task_Minimum("Minimum", 1, 1, 1);
  static TTask TTask_Minimum(Task_Minimum);
 
  //Simulate tasks
  SG_completed = SG_simulate(conf.Get_Period(),SG_ANY_TASK,SG_SILENT);
  SortSimgridTaskArray(SG_completed); // sort for reproduceability; 
  cout << "==================After SG_simulate===============\n";

  for (unsigned int i=0; SG_completed[i] != NULL; i++) {
    total_completed_tasks++;
    node_task = (TTask *) SG_getTaskMetadata(SG_completed[i]);
    
    // Compute feedback of intermediate result from finished task
    pt_task = node_task->pt_content();
    pt_task->Set_Feedback(sigma, Fnc_Griewank);
    //task.Set_Feedback(power, Fnc_Power);
    average_feedback = (average_feedback + pt_task->Get_Feedback());
    
    // Store current Global Minimum Feedback value
    if (pt_task->Get_Feedback() < TTask_Minimum.content().Get_Feedback()) {
      TTask_Minimum <= (*node_task);
      cout << "Task Minimum : "; 
      cout << "Level = " << TTask_Minimum.Get_Level() << " ,";
      Print_Info(TTask_Minimum.pt_content());       
    }

    cout << "Level = " << node_task->Get_Level() << ", ";
    Print_Info(pt_task);


    // Checks if it is time to steer
    if (total_completed_tasks < Total_Tasks_to_Execute(conf)) {
	if (Meet_Steering_Criteria(SG_completed[i]) &&
	    node_task->Get_Level() < conf.Get_Max_Levels()) {

	  Trigger_Steering_Event((TTask *)node_task->parent(), 
				 size, 
				 relevant_results,
				 false,
				 conf,
				 grid);
	  Continue = true;
	}
    }
    else
      Continue = false;	
  }

  // Finalization
  if (!Continue) {
    cout << "Task Minimum : "; 
    cout << "Level = " << TTask_Minimum.Get_Level() << " ,";
    Print_Info(TTask_Minimum.pt_content());
    cout << "\nTotal Tasks to Execute = " << Total_Tasks_to_Execute(conf) 
	 << endl;
    cout << "Total Executed Tasks   = " << total_completed_tasks << "\n\n";
 }
  
  return Continue;
}


void Run_Steering(unsigned int const num_relevant_results,
		  Parameters & conf, Grid & grid) {

  bool Continue_Steering; 
  vector<TTask *> relevant_results(num_relevant_results);
  vector<TTask *> previous_results(num_relevant_results);
  vector<TTask *>::iterator it_prev;
  
  Continue_Steering = Check_Completed_Tasks(NULL, 
					    conf.Get_Sigma(), 
					    num_relevant_results,
					    relevant_results,
					    conf,
					    grid);
  previous_results = relevant_results;
  while (Continue_Steering) {
    it_prev = &previous_results[0];
    Continue_Steering = Check_Completed_Tasks(it_prev, 
					      conf.Get_Sigma(), 
					      num_relevant_results,
					      relevant_results,
					      conf,
					      grid);
    previous_results = relevant_results;
  }
}
 
   
int main(int argc, const char *const *argv) {  
  int i;
  Parameters conf (argc, argv);
  Format_Output_Stream();

 /* Initializing Simgrid */
  SG_init();
  
  srand(SEED);
  Grid grid(PROCESSOR, conf.Get_Num_Proc());
  Initialize_Steering(grid, conf);
  Run_Steering(conf.Get_Num_Relevant(), conf, grid);

  return 0;
}
