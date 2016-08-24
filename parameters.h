#ifndef __PARAMETERS_H__
#define __PARAMETERS_H__

#include <stdio.h>
#include <string>
#include <strstream>
using namespace std;


#define ASENV_SHORT_NAMES
#include "envseed.h"
#include "singleton.h"
#include "simgrid.h"
#include "cost_fnc.h"

typedef enum {SELECT_BEST, STEADY_STATE, LIMITED} STEERING_REGIME;
typedef enum {INITIAL, SIMPLE, PRIORITY_TREE, COMPLEX} POLICY_TYPE;
typedef enum {MCELL_RAN4, C_RAND} RANDOM_GEN_TYPE;
typedef enum {INVERSE, QUADRATIC, ROOT} PRIORITY_POLICY;
typedef enum {CONTINUOUS, CONTEXT_SWITCHING} TIME_SHARING;
typedef enum {ROUND_ROBIN, PRIORITY} GRID_MAPPING;
typedef enum {GRIEWANK, ROSENBROCK, RASTRIGIN} FUNCTION_TYPE;

typedef FEEDBACK_TYPE (*OBJ_FUNCTION_TYPE)(float, CONFIGURATION_TYPE&);

class Parameters : public Singleton<Parameters> {
 private:
  static Parameters* _instance;

  unsigned int _num_tasks;
  unsigned int _num_relevant;
  POLICY_TYPE _policy;
  STEERING_REGIME _steering_regime;
  PRIORITY_POLICY _priority_policy;

  GRID_MAPPING _grid_mapping;

  float _initial_radius;
  float _scatter_radius;
  float _threshold;
  float _workload;
  unsigned int _max_levels;

  unsigned int _num_dim;
  float _sigma;
  unsigned int _num_proc;
  double _proc_speed;
  string _grid_file_name;
  float _period;
  unsigned int _random_seed;
  RANDOM_GEN_TYPE _random_gen;  
  
  bool _priority_queue;
  unsigned int _priority_queue_size;
  bool _flow_control_enabled;
  double _token_rate;
  unsigned int _burst_limit;

  unsigned int _suspend_queue_size;
  string _partition_graph_file;
  unsigned int _population_size;
  unsigned int _number_execute;
  unsigned int _max_trials;
  SG_resourcesharing_t _resource_sharing;
  double _mutation_probability;

  TIME_SHARING _time_sharing;
  double _tsh_cycle;
  double _overhead;

  unsigned int _task_limit;
  bool _random_selection;

  SG_outputmode_t _verbose;

  bool _vi_simulated_user;

  string _vi_project_name;

//    FEEDBACK_TYPE(*_objective_function)(float, CONFIGURATION_TYPE&) ;
  OBJ_FUNCTION_TYPE _objective_function;
  FUNCTION_TYPE _aux_func;

  void Treat_Switch(char *);

 public:
  Parameters(void);
  Parameters(int, const char *const *);

  static Parameters * Instance(void);
  static Parameters * Instance(int, const char *const *);

  unsigned int Get_Num_Tasks(void) const {return _num_tasks;}
  unsigned int Get_Num_Relevant(void) const {return _num_relevant;}
  POLICY_TYPE Get_Policy(void) const {return _policy;}
  STEERING_REGIME GetSteeringRegime(void) const {return _steering_regime;}
  GRID_MAPPING GetGridMapping(void) const {return _grid_mapping;}

  float Get_Initial_Radius(void) const {return _initial_radius;}
  float Get_Scatter_Radius(void) const {return _scatter_radius;}
  float Get_Threshold(void) const {return _threshold;}
  float Get_Workload(void) const {return _workload;}
  unsigned int Get_Max_Levels(void) const {return _max_levels;}

  unsigned int Get_Num_Dim(void) const {return _num_dim;}
  float Get_Sigma(void) const {return _sigma;}
  unsigned int Get_Num_Proc(void) const {return _num_proc;}
  double Get_Proc_Speed(void) const {return _proc_speed;}
  string GetGridFileName(void) const {return _grid_file_name;}

  float Get_Period(void) const {return _period;}

  unsigned int Get_Random_Seed(void) const {return _random_seed;}
  RANDOM_GEN_TYPE Get_Random_Generator_Type (void) const {return _random_gen;}

  unsigned int Get_PQ_Size(void) const {return _priority_queue_size;} 
  bool Get_Priority_Queue(void) const {return _priority_queue;}
  bool GetFlowControlEnabled(void) const {return _flow_control_enabled;}
  double Get_Token_Rate(void) const {return _token_rate;}
  double Get_Token_Period(void) const {return 1 / _token_rate;}
  unsigned int Get_Burst_Limit(void) const {return _burst_limit;}

  unsigned int Get_SQ_Size(void) const {return _suspend_queue_size;}

  string Get_Graph_File_Name(void) const {return _partition_graph_file;}
  unsigned int GetPopulationSize(void) const {return _population_size;}
  unsigned int GetNumberExecute(void) const {return _number_execute;}
  unsigned int GetMaxTrials(void) const {return _max_trials;}
  PRIORITY_POLICY GetPriorityPolicy(void) const {return _priority_policy;}
  SG_resourcesharing_t  GetResourceSharing(void) const 
  {return _resource_sharing;}
  double GetMutationProbability(void) const {return _mutation_probability;}

  TIME_SHARING GetTimeSharing(void) const {return _time_sharing;}
  double GetCycle(void) const {return _tsh_cycle;}
  double GetOverhead(void) const {return _overhead;}

  unsigned int GetTaskLimit(void) const {return _task_limit;}

  SG_outputmode_t GetVerbosity(void) const {return _verbose;}
  bool GetRandomSelection(void) const {return _random_selection;}

  bool Is_VI_Simulated_User(void) const {return _vi_simulated_user;}
  string Get_VI_Project_Name(void) const {return _vi_project_name;}

  OBJ_FUNCTION_TYPE GetFunction(void) const {return _objective_function;}

//    FEEDBACK_TYPE(*)(float, CONFIGURATION_TYPE&) GetFunction(void) const {
//      return _objective_function;}
  
  void Set_Defaults(void);

  void Print (void);

};

#endif
