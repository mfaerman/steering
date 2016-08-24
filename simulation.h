#ifndef __SIMULATION_H__
#define __SIMULATION_H__

//#include"tree_tasks.h"
#include"parameters.h"
#include"grid.h"
#include"array.h"
#include"cost_fnc.h"
#include"flow_control.h"
#include"helper.h"
#include"meta_task.h"
#include"meta_task_limit_random.h"
#include"main.h"

#include "creator.h"
#include "queue_creator.h"
#include "grid_creator.h"

class Simulation {
private:
  TPTask _ttree;
  void Initialize_Static_Methods(void);

protected:
  Parameters * _pt_setup;
  Grid * _pt_grid;

  unsigned int _generated_tasks;
  static unsigned int _random_restarts;
  vector<HTPTask> _completed_tasks;

  virtual bool IsOverhead(SG_Task);

  TPTask _TPTask_Minimum;
  unsigned int _total_completed_tasks;
  FlowControl _flow_control;

  virtual SG_Task Get_Until_Task(void);
  virtual double GetNextInterval(void);
  virtual bool Meet_Steering_Criteria(HTPTask &);
  virtual void Check_Completed_Tasks(void);   
  virtual void Update_Global_Minimum(HTPTask &);
  virtual void Finalization(void);
  virtual void MoveFromQueueToGrid(void);
  virtual bool Still_Steering(void);
  virtual void Get_Completed_Tasks(void);
  virtual unsigned int GetGeneratedTasks(void);

public:
  Simulation(void);

  static void NewRandomRestart(void);
  static unsigned int GetRandomRestarts(void);

  virtual void Init_Steering(void);
  virtual void Run(void);
  TPTask & GetRoot(void);

};

/////////////////////////////////////////////////////////////////////

class  PQueueSimulation : public Simulation {
 private:
  // FlowControl _flow_control;
  void Check_Completed_Tasks(void);
  //  void MoveFromQueueToGrid(void);
  void Create_New_Tasks(void);
  //  bool Still_Steering(void);
  //  double GetNextInterval(void);
  //  void Finalization(void);

 public:
  PQueueSimulation(void);
  void Run(void);
};

class  SimulationLimited : public Simulation {
  virtual void MoveFromQueueToGrid(void);
};

class  SimulationLimitedRandom : public SimulationLimited {
  virtual void Get_Completed_Tasks(void);
};

#endif

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

// Simulation Creator
#ifndef __SIMULATION_CREATOR_H__
#define __SIMULATION_CREATOR_H__

class SimulationCreator : public Creator<Simulation> {

private:
  static Simulation * _instance;
  static Parameters * _pt_setup;
//    int _argc;
//    const char * const * _argv;
  Simulation * CreateProduct(void);

public:
  static void Set_Configuration(Parameters *);
  static Simulation * Instance(void);
  SimulationCreator(void);
};

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#endif
