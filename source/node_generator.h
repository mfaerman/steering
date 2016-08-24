#ifndef __NODE_GENERATOR_H__
#define __NODE_GENERATOR_H__

#include<math.h>
//#include"tree_tasks.h"
#include"grid.h"
#include"parameters.h"
//#include"node_generator.h"
#include"random_gen_creator.h"
#include"priority_queue.h"
#include"task_creator.h"
#include"helper.h"
#include"simulation.h"

class Generator_Group {
private:
  float _radius;
  void GenerateRandomTasks(unsigned int);

protected:
  static RandomGenerator * _pt_rg;
  Simulation * GetSim(void) const;

  static Grid * _pt_grid;
  static Parameters * _pt_setup;
  
  vector<HTPTask> _relevant_results;
  HTPTask _parent;
  HTPTask _current_node;

 
  virtual bool Schedule_New_Tasks(float) {cout << "shouldn't call this!\n"; exit(-1);}
  virtual bool Spawn_Neighbor_Configurations(HTPTask &, float, PRIORITY_TYPE);
  
  //  CONFIGURATION_TYPE Ren_Spherical_Neighborhood(CONFIGURATION_TYPE *, float);

  //  void ScheduleOnResource(SG_Resource, HTPTask);
  virtual PRIORITY_TYPE GetPriorityRandomTask();

  Generator_Group(HTPTask &);

public:

  static void Set_Configuration(Parameters *); 
  static void Set_Grid(Grid *); 
  virtual void Trigger_Steering_Event (void);
};

//---------------------------------------------------------------------

class InitialGeneratorGroup : public Generator_Group {
public:
  InitialGeneratorGroup(HTPTask &);
  bool  Schedule_New_Tasks(float); 
  PRIORITY_TYPE GetPriorityRandomTask();
};

//---------------------------------------------------------------------
class SimpleGeneratorGroup : public Generator_Group {
public:
  SimpleGeneratorGroup(HTPTask &);
  bool  Schedule_New_Tasks(float);
  PRIORITY_TYPE GetPriorityRandomTask();
};

//---------------------------------------------------------------------
class PriorityTreeGeneratorGroup : public Generator_Group {
public:
  PriorityTreeGeneratorGroup(HTPTask &);
  bool  Schedule_New_Tasks(float);
  //  virtual void Trigger_Steering_Event (void);
  PRIORITY_TYPE GetPriorityRandomTask();
};

#endif

