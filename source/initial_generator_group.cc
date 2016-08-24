#include "node_generator.h"

InitialGeneratorGroup::InitialGeneratorGroup(HTPTask & ttask) : 
  Generator_Group(ttask) { }

bool InitialGeneratorGroup::Schedule_New_Tasks(float radius) {
  bool spawned(false);

  for (unsigned int i=0; i < _pt_setup->Get_Num_Tasks(); i++)
    if(Spawn_Neighbor_Configurations(_relevant_results[i], radius, 2))
      spawned = true;
  
  for (unsigned int i = _relevant_results.size() - 1; 
       i > _relevant_results.size() -2; i--) {
    if(Spawn_Neighbor_Configurations(_relevant_results[i], 100, 1))
      spawned = true;
  }
  return spawned;
}

PRIORITY_TYPE InitialGeneratorGroup::GetPriorityRandomTask(void) {
  return 1.0;
}
