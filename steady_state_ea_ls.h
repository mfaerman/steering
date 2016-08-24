#ifndef __STEADY_STATE_EA_LS_H__
#define __STEADY_STATE_EA_LS_H__

#include<algorithm>
#include<vector>
using namespace std;

#include"partition.h"
#include"partition_local_search.h"
#include"simulation.h"
//#include"bin_task.h"
#include"array.h"

class SpawnLS_Sniff {
private:
  unsigned int _length;
  PRIORITY_TYPE _priority;

public:
  SpawnLS_Sniff(unsigned int, PRIORITY_TYPE);
  void operator()(PartitionFirstImproveLocalSearch *);
};


class EALS : public Simulation {
private:
  vector<PartitionFirstImproveLocalSearch *> _population;
  PartitionFirstImproveLocalSearch * GenerateNew(void);
  void GetAdditionalRefinement(vector<PartitionFirstImproveLocalSearch *> &);
  void UpdatePopulation(PartitionFirstImproveLocalSearch *); 
  bool StillSearching(void);
  PartitionFirstImproveLocalSearch * PopulationBest(void);
  vector<PartitionFirstImproveLocalSearch *> * SelectParents(void);

public:
  EALS(void);
  PartitionFirstImproveLocalSearch * SteadyStateEALS(void);
};

TPTask Root(new PartitionTask("Root", 1, 1));

#endif

