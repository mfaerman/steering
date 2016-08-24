#include"helper.h"
#include"queue_creator.h"

//  void Initialize_Static_Methods(void) {
//    Generator_Group::Set_Configuration(&_setup);
//    Generator_Group::Set_Grid(&_grid);
//    RandomGenCreator::Set_Configuration(&_setup);
//    FlowControl::Set_Configuration(&_setup);
//    FlowControl::Set_Grid(&_grid);
//    PriorityQueue::Set_Configuration(&_setup);
//    PriorityQueue::Set_Grid(&_grid);
//    NodeGenCreator::Set_Configuration(&_setup);
//    SimulationCreator::Set_Configuration(&_setup);
//  }

//  void ScheduleOnResource(SG_Resource processor, TTask * node_task) {
//    if(Parameters::Instance()->Get_Priority_Queue())
//      PriorityQueue::Instance()->Insert(node_task);
//    else
//      node_task->pt_content()->Schedule(processor, node_task);
//  }

void ScheduleOnResource(SG_Resource processor, HTPTask & node_task) {
  if(Parameters::Instance()->Get_Priority_Queue())
    QueueCreator::Instance()->Insert(node_task);
  else 
    (node_task->content())->Schedule(processor, new HTPTask(node_task));
}

SG_outputmode_t GetVerbosity(void) {
  return Parameters::Instance()->GetVerbosity();
}


char * Name_Policy(char * name, int i) {
  sprintf(name,"T%d", i);
  return (name);
}
  
void Format_Output_Stream(void) {
  cout.setf(ios::fixed,ios::floatfield);
  cout.precision(2);
}

void Print_Info(HBaseTask & it_task) {

  FEEDBACK_TYPE feedback;

  cout << "Task '" << it_task->name() << "', ";

  it_task->Print_SG_Info();
  it_task->Print_Conf();

  feedback = it_task->Get_Feedback();
  cout  << ", Feedback = "  << feedback << endl;
}

void Print_Info(HTPTask & node_task) {
  cout << "Level = " << node_task->Get_Level() << ", ";
  cout << "Parent = '" << (-(*(node_task->parent())))->name() << "', ";
  cout << "Trial = " << dynamic_cast<TPTask *>((node_task->parent()).get())->GetTrials() << ", ";
  Print_Info(-(*node_task));
}


CONFIGURATION_TYPE Spherical_Neighborhood(CONFIGURATION_TYPE * conf, 
					  float radius) {
  float coord = 0.0;
  CONFIGURATION_TYPE neighbor(conf->size());
  CONFIGURATION_TYPE real_coordinates(conf->size());
  //static RandomGenerator * rg(RandomGenCreator().GetProduct()); 
  //RandomGenerator * rg(RandomGenCreator::GetProduct());

  RandomGenerator * rg(RandomGenCreator::Instance());

  float prev_coord = 0;
  for (unsigned int i = 0; i < conf->size(); i++) {
//      coord =  2 * radius*cos(prev_coord) 
//        * ((double)rand() / (double)(RAND_MAX+1.0)) - radius*cos(prev_coord);
    coord = rg->GetRand(2 * radius*cos(prev_coord)) - radius*cos(prev_coord);
    real_coordinates[i] = coord;
    prev_coord = coord;
    neighbor[i] = coord + (*conf)[i];
  }
  return(neighbor);
}

float Workload_Policy() {
  return Parameters::Instance()->Get_Workload();  
}

double Start_Priority_Policy() {
  return 1;
}

unsigned int Neighborhood_Policy(int i) {
  return i;
}

double Priority_Policy(double p) {
  //  return 1024/pow(2,p);
  switch(Parameters::Instance()->GetPriorityPolicy()) {
    case INVERSE:
      return (1/p);
    case QUADRATIC:
      return (1/(p*p));
    case ROOT:
      return (1/sqrt(p));
    default:
      cout << "Error in priority policy" << endl;
      exit(-3);
  }
}

unsigned int Total_Tasks_to_Execute(Parameters & conf) {
  // calculated according to steering tree structure
  unsigned int sum;

  sum = 2*conf.Get_Num_Dim();    // initialization tasks

  for (unsigned int i = 2; i <= conf.Get_Max_Levels(); i++) 
    sum += conf.Get_Num_Tasks() * (int) pow(conf.Get_Num_Relevant(),(i - 1));
  
  return sum;
}
 
CONFIGURATION_TYPE Scatter(int point, float radius, int num_dim) {
  
  CONFIGURATION_TYPE config(num_dim, 0);
  if (point < num_dim)
    config[point % num_dim] =  radius;
  else 
    config[point % num_dim] = -radius; 
  return config;
}
