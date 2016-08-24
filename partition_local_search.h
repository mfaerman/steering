#ifndef __PARTITION_LOCAL_SEARCH_H__
#define __PARTITION_LOCAL_SEARCH_H__

#include "search.h"
#include "partition.h"
#include "tasks.h"
#include "grid_creator.h"

class PartitionFirstImproveLocalSearch : 
  public FirstImprove<HalfBalancedPartition> {

private:
  unsigned int _rebalance_evaluations;
  HalfBalancedPartition & Rebalance(void);
  HTPTask  _node_task;

public:
  PartitionFirstImproveLocalSearch(HalfBalancedPartition &);
  PartitionFirstImproveLocalSearch(void);
  PartitionFirstImproveLocalSearch(PartitionFirstImproveLocalSearch &);
  ~PartitionFirstImproveLocalSearch(void) {}

  unsigned int GetRebalanceEvaluations(void) const;
  //  HalfBalancedPartition & Sniff(unsigned int);
  void Sniff(unsigned int);
  HalfBalancedPartition & GetOptimum(void);
  void SpawnLS_Sniff(TPTask &, unsigned int, PRIORITY_TYPE);
  HTPTask & GetTPTask(void);
  PartitionFirstImproveLocalSearch & operator= (const LocalSearch<HalfBalancedPartition> &);
};

/*  PartitionFirstImproveLocalSearch * Crossover */
/*  (PartitionFirstImproveLocalSearch *, PartitionFirstImproveLocalSearch *); */


#endif


#ifndef __BIN_TASK_H__
#define __BIN_TASK_H__

class PartitionTask : public BaseTask {
private:
  //  LocalSearch<Partition> * _search_partition;
  PartitionFirstImproveLocalSearch *  _search_partition;

public:
  PartitionTask(NAME_TYPE, 
		WORKLOAD_TYPE, 
		PRIORITY_TYPE, 
		PartitionFirstImproveLocalSearch *); 

  PartitionTask(NAME_TYPE, 
		WORKLOAD_TYPE, 
		PRIORITY_TYPE); 

  PartitionTask(const PartitionTask &);
  
  PartitionFirstImproveLocalSearch * GetMetadata(void);

  ~PartitionTask(void) {}
};

#endif






