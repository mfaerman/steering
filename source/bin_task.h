#ifndef __BIN_TASK_H__
#define __BIN_TASK_H__

#include "tasks.h"
#include "partition.h"
#include "search.h"
#include "partition_local_search.h"


class PartitionTask : public BaseTask {
private:
  LocalSearch<Partition> * _partition;

public:
  PartitionTask(NAME_TYPE, 
		WORKLOAD_TYPE, 
		PRIORITY_TYPE, 
		PartitionFirstImproveLocalSearch *); 

  PartitionTask(const PartitionTask &);
  //  Partition * GetMetadata(void);
};

#endif
