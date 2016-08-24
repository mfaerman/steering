#include <iostream>
#include <algorithm>
using namespace std;

#include "parameters.h"

static int badSwitchSeen = 0;


/*
 * A switch error handler.  Passed the error, the element of argv that contains
 * the bad switch, the offset into that element of the switch name, the length
 * of the switch name, and the bad value, if appropriate.
 */
static int
BadSwitchHandler(ParseErrors error,
                 const char *badWord,
                 unsigned badSwitchOffset,
                 unsigned badSwitchLen,
                 const char *badValue) {

  int intLen = (int)badSwitchLen;

  if(error == ARGUMENTS_NOT_ALLOWED)
    fprintf(stderr, "This program takes no program arguments\n");
  else if(error == INVALID_VALUE)
    fprintf(stderr, "The value '%s' is invalid for switch '%*.*s'\n",
            badValue, intLen, intLen, badWord + badSwitchOffset);
  else if(error == MISSING_VALUE)
    fprintf(stderr, "The switch '%*.*s' requires a value\n",
            intLen, intLen, badWord + badSwitchOffset);
  else if(error == UNKNOWN_SWITCH)
    fprintf(stderr, "'%*.*s' is not a known switch\n",
            intLen, intLen, badWord + badSwitchOffset);

  badSwitchSeen = 1;
  return 1; /* Allow argv parsing to continue. */

}

void Parameters::Treat_Switch(char * switchName) {
 
  string aux;
  istrstream ist(SwitchValue(switchName, NULL));
  string s = switchName;

//  cerr << "s=" << s << "\n";

  if  (s == "vi") {
    _vi_simulated_user = true;
    ist >> _vi_project_name;
    replace(_vi_project_name.begin(), _vi_project_name.end(), '-', '_');
  } 

  if  (s == "nt")
    ist >> _num_tasks;
  if  (s == "nr")
    ist >> _num_relevant;   
  if  (s == "ir")
    ist >> _initial_radius;
  if  (s == "sr")
    ist >> _scatter_radius;
  if  (s == "th")
    ist >> _threshold; 
  if  (s == "w")
    ist >> _workload;
  if  (s == "ml")
    ist >> _max_levels;
  if  (s == "d")
    ist >> _num_dim;
  if  (s == "sigma")
    ist >> _sigma;
  if  (s == "p")
    ist >> _num_proc;
  if (s == "gfn")
    ist >> _grid_file_name;
  if  (s == "pr")
    ist >> _period;
  if  (s == "ps")
    ist >> _proc_speed;
  if (s == "-policy") {
    ist >> aux;
    if (aux == "simple")
      _policy = SIMPLE;
    else
      _policy = PRIORITY_TREE;
  }
  if (s == "rs")
    ist >> _random_seed;
  if (s == "rt") {
    ist >> aux;
    if (aux == "c")
      _random_gen = C_RAND;
    else 
      _random_gen = MCELL_RAN4;
  }
  if (s == "pqs")
    ist >> _priority_queue_size;
  if (s == "npq") {
    _priority_queue = false;
    _priority_queue_size = 0;
    _flow_control_enabled = false;
  }
  if (s == "tok")
    ist >> _token_rate;
  if (s == "bl")
    ist >> _burst_limit;
  if (s == "reg") {
    ist >> aux;
    if(aux == "sst")
      _steering_regime = STEADY_STATE;
    else 
      if(aux == "limit")
	_steering_regime = LIMITED;
      else
	_steering_regime = SELECT_BEST;
  }
  if (s == "pgf")
    ist >> _partition_graph_file;
  if (s == "pops")
    ist >> _population_size;
  if (s == "ne")
    ist >> _number_execute;
  if (s == "mt")
    ist >> _max_trials;
  if (s == "pripol") {
    ist >> aux;
    if(aux == "quadratic")
      _priority_policy = QUADRATIC;
    else
      if(aux == "root")
        _priority_policy = ROOT;
      else
	_priority_policy = INVERSE;
  }
  if (s == "sharing") {
    ist >> aux;
    if(aux == "seq") {
      _resource_sharing = SG_SEQUENTIAL_IN_ORDER;
      _flow_control_enabled = false;
    }
    else
      _resource_sharing = SG_TIME_SLICED;
  }
  
  if (s == "mp")
    ist >> _mutation_probability;
  if (s == "tsh") {
    ist >> aux;
    if(aux == "switch") {
      _resource_sharing = SG_SEQUENTIAL_IN_ORDER;
      //_flow_control_enabled = false;
      _time_sharing = CONTEXT_SWITCHING;
    }
    else
      _time_sharing = CONTINUOUS;
  }
  if (s == "tshc")
    ist >> _tsh_cycle;
  if (s == "map") {
    ist >> aux;
    if (aux == "on")
      _grid_mapping = PRIORITY;
    else
      _grid_mapping = ROUND_ROBIN;
  }
  if (s == "over")
    ist >> _overhead;
  if (s == "tl")
    ist >> _task_limit;
  if (s == "sqs")
    ist >> _suspend_queue_size;
  if (s == "verb") {
    ist >> aux;
    if (aux == "on")
      _verbose = SG_VERBOSE;
    else
      _verbose = SG_SILENT;
  }
  if (s == "rsel") {
    ist >> aux;
    if (aux == "on")
      _random_selection = true;
    else 
      _random_selection = false;
  }
  if (s == "obfn") {
    ist >> aux;
    if (aux == "grw") {
      _aux_func = GRIEWANK;
      _objective_function = Fnc_Griewank;
    }
    else 
      if (aux == "rst") {
	_objective_function = Fnc_Rastrigin;
	_aux_func = RASTRIGIN;
      }
	   else
	     if (aux == "rbr") {
	       _objective_function = Fnc_Rosenbrock;
	       _aux_func = ROSENBROCK;
	     }
  }
}

void Parameters::Set_Defaults(void) {
  _num_tasks = 10;
  _num_relevant = 2;
  _policy = PRIORITY_TREE;

  _initial_radius = 1024;
  _scatter_radius = 100;
  _threshold = 1e-1;
  _workload = 10;
  _max_levels = 5;

  _num_dim = 3;
  _sigma = 0.5;
  _num_proc = 100;
  _proc_speed = 1.0;
  _period = -1.0;
  _random_seed = 2;
  _random_gen =  MCELL_RAN4;

  _priority_queue = true;
  _flow_control_enabled = false;
  _priority_queue_size = 100;
  _token_rate = 0.1;
  _burst_limit = 10;
  _steering_regime = SELECT_BEST;
  _partition_graph_file = "./partition/G124.16";
  _grid_file_name = "./proclist";
  _population_size = 20;
  _number_execute = 306;
  _max_trials = 4;
  _priority_policy = INVERSE;
  _resource_sharing = SG_TIME_SLICED;
  _mutation_probability = 5.0/124.0;
  _time_sharing = CONTINUOUS;
  _tsh_cycle = 10.0;
  _overhead = 1.0;
  _task_limit = 2;
  _suspend_queue_size = 1000;
  _verbose = SG_SILENT;
  _random_selection = false;
  _grid_mapping = ROUND_ROBIN;
  _objective_function =  Fnc_Griewank;
  _aux_func = GRIEWANK;

  _vi_simulated_user = false;
}

void Parameters::Print(void) {
  cout << "Number of Tasks:          " << _num_tasks      << endl;
  cout << "Number of Relevant Tasks: " << _num_relevant   << endl;
  cout << "Scheduling Policy:        " << _policy         << endl;
  cout << "Initial Radius:           " << _initial_radius << endl;
  cout << "Scatter Radius:           " << _scatter_radius << endl;
  cout << "Steering Threshold:       " << _threshold      << endl;
  cout << "Deepest Tree Level:       " << _max_levels     << endl;
  cout << "Number of Dimensions:     " << _num_dim        << endl;
  cout << "Sigma:                    " << _sigma          << endl;
  cout << "Workload:                 " << _workload       << endl;
  cout << "Number of Processors:     " << _num_proc       << endl;
  cout << "Processor Relative Speed: " << _proc_speed     << endl;
  cout << "Grid File Name:           " << _grid_file_name << endl;
  cout << "Steering Periodicity:     " << _period         << endl;
  cout << "Random Seed:              " << _random_seed    << endl;
  cout << "Random Generator Type:    " << _random_gen     << endl;
  cout << "Priority Queue Size:      " << _priority_queue_size << endl;
  cout << "Flow Control              ";
  if (_flow_control_enabled)
    cout << "ON" << endl;
  else
    cout << "OFF" << endl;
  cout << "Token Rate:               " << _token_rate     << endl;
  cout << "Burst Limit:              " << _burst_limit    << endl;
  cout << "Priority Queue:           " << _priority_queue << endl;
  cout << "Steering Regime:          ";
 switch(_steering_regime) {
  case STEADY_STATE:
    cout << "Steady State" << endl;
    break;
  case LIMITED:
    cout << "Limited" << endl;
    break;
  case SELECT_BEST:
    cout << "Select Best" << endl;
    break;
 }
  cout << "Maximum Trials:           " << _max_trials           << endl;
  cout << "Priority Policy:          ";
  switch(_priority_policy) {
    case QUADRATIC:
      cout << "Squared Inverse Feedback" << endl;
      break;
    case INVERSE:
      cout << "Inverse Feedback" << endl;
      break;
    case ROOT:
      cout << "Root Squared Inverse Feedback" << endl;
      break;
  }
  cout << "Resource Sharing          ";
  if(_resource_sharing == SG_TIME_SLICED)
    cout << "SG_TIME_SLICED" << endl;
  else
    cout << "SG_SEQUENTIAL_IN_ORDER" << endl;
  cout << "Partition Graph File:     " << _partition_graph_file << endl;
  cout << "Population Size:          " << _population_size      << endl;
  cout << "Number to Execute         " << _number_execute       << endl;
  cout << "Mutation Probability      " << _mutation_probability << endl;
  cout << "Time Sharing              ";
  if(_time_sharing == CONTINUOUS)
    cout << "Continuous" << endl;
  else
    cout << "Context Switching" << endl;
  cout << "Time Sharing Cycle        " << _tsh_cycle << endl;
  cout << "Context Switch Overhead   " << _overhead << endl;
  cout << "Task Limit per Processor  " << _task_limit << " tasks(s)" << endl; 
  cout << "Suspend Queue Size        " << _suspend_queue_size << endl;
  cout << "Verbose                   ";
  if (_verbose == SG_VERBOSE)
    cout << "On" << endl;
  else
    cout << "Off" << endl;
  cout << "Random Selection          ";
  if (_random_selection == true)
    cout << "On" << endl;
  else
    cout << "Off" << endl;
  cout << "Mapping                   ";
  if (_grid_mapping == PRIORITY)
    cout << "Prioritized Mapping" << endl;
  else
    cout << "Round Robin" << endl;

  cout << "Objective Function:       ";
  switch(_aux_func) {
    case GRIEWANK:
      cout << "Griewank" << endl;
      break;
    case RASTRIGIN:
      cout << "Rastrigin" << endl;
      break;
    case ROSENBROCK:
      cout << "Rosenbrock" << endl;
      break;
  }
}

Parameters::Parameters(void) {
  Set_Defaults();
}

Parameters::Parameters(int argc,
     const char *const *argv) {

  /*
   * A typical valid switch specification.  Each of the newline-separated
   * elements specifies the switch name and the type of parameter it takes.
   * Any text after the type is ignored, making it easy to reuse the spec to
   * print switch help.  Note that "string" is not a recognized type, but
   * ParseArgv matches any unrecognized type to any text value.
   */
  const char *validSwitches = "nt      unsigned int number of tasks\n"
                              "nr    unsigned int number of relevant results\n"
                              "ir      double       initial radius\n"
                              "sr      double       scatter radius\n"
                              "th      double       threshold\n"
                              "w       double       task workload\n"
                              "d       unsigned int number of dimensions\n"
                              "sigma   double       sigma constant\n"
                              "p       unsigned int number of  processors\n"
                              "ps      double       processor speed\n"
                              "gfn     string       grif file name\n"
                              "map     string       mapping (on/off)\n"
                              "ml      unsigned int deepest tree level\n"
                              "pr      float        steering period\n"
                              "-policy string       scheduling policy\n"
                              "rs      unsigned int random seed\n"
                              "rt      string       random generator type\n"
                              "npq     unsigned int priority queue off\n"
                              "pqs     unsigned int priority queue size\n"
                              "tok     double       flow control token rate\n"
                              "bl      unsigned int burst limit\n"
                              "reg     string       steering regime (sst/sb)\n"
                              "pgf     string       partition graph file\n"
                              "pops    unsigned int population size\n"
                              "ne      unsigned int number of iterations\n"
                              "mt      unsigned int max number of trials\n"
                              "pripol  string       priority policy\n"
                              "sharing string       resource sharing policy\n"
                              "mp      double       mutation probability\n"
                              "tsh     string       time sharing\n"
                              "tshc    double       time sharing cycle\n"
                              "over    double       context switch overhead\n"
                              "tl      unsigned int task limit\n"
                              "sqs     unsigned int suspend queue size\n"
                              "verb    unsigned int verbosity\n"
                              "rsel    unsigned int random selection\n"
                              "obfn    string       Objective Function\n" 
			      "vi      string       VI project name\n"
;
                           
  const char *c;
  int i;
  const char *next;
  char switchName[20];
  const char *value;

  if(!ParseArgv(argv, validSwitches, &BadSwitchHandler) || badSwitchSeen) {
    cerr << endl << validSwitches << endl;
    exit(-1);
  }
  Set_Defaults();
  for(c = validSwitches, next = c; next != NULL; c = next + 1) {
    next = strchr(c, '\n');
    if(*c == '\n' || *c == '\0')
      continue; /* Ignore any empty lines or trailing newlines. */
    strncpy(switchName, c, sizeof(switchName));
    *strchr(switchName, ' ') = '\0';
    if(SwitchPresent(switchName)) {
      Treat_Switch(switchName);
    }
  }
  if (_steering_regime == STEADY_STATE)
    _num_relevant = 1;
  if (_time_sharing == CONTEXT_SWITCHING)
     _resource_sharing = SG_SEQUENTIAL_IN_ORDER;
  Print();
}

Parameters * Parameters::_instance = NULL;

Parameters * Parameters::Instance(void) {
  if (_instance == NULL) {
    cerr << "Error -- Accessing non initialized configuration \n";
    exit (-1);
  }
  else
    return _instance;
}

Parameters * Parameters::Instance(int argc, const char *const * argv) {
  if (_instance == NULL) {
    _instance = new Parameters(argc, argv);
  }
  return _instance;
}











