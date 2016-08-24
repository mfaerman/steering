#include "node_generator.h"

SimpleGeneratorGroup::SimpleGeneratorGroup(HTPTask & ttask) : 
  Generator_Group(ttask) { }

bool SimpleGeneratorGroup::Schedule_New_Tasks(float radius) {
  // no differentiated priorities ==> set all priorities to 1
 bool spawned(false);

 switch(Parameters::Instance()->GetSteeringRegime()) {
    case STEADY_STATE:
      if(Spawn_Neighbor_Configurations(_current_node, radius, 1))
	spawned = true;
      break;
    case LIMITED:
    case SELECT_BEST:
      for (unsigned int i = 0; i < _relevant_results.size(); i++)
	if(Spawn_Neighbor_Configurations(_relevant_results[i], radius, 1))
	  spawned = true;
      break;
    default:
      cout << "Steering Regime Error!" << endl;
      exit (-1);
 }
 return spawned;
}

PRIORITY_TYPE SimpleGeneratorGroup::GetPriorityRandomTask(void) {
  return 1.0;
}


