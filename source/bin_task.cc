#include"partition_local_search.h"

PartitionTask::PartitionTask(NAME_TYPE name, 
			     WORKLOAD_TYPE workload, 
			     PRIORITY_TYPE priority, 
			     PartitionFirstImproveLocalSearch * search_partition) :
  BaseTask(name, workload, priority, Parameters::Instance()->Get_Num_Dim()),
  _search_partition(search_partition)  {}

PartitionTask::PartitionTask(NAME_TYPE name, 
			     WORKLOAD_TYPE workload, 
			     PRIORITY_TYPE priority) :
  BaseTask(name, workload, priority, 1),
  _search_partition(NULL)  {}


PartitionFirstImproveLocalSearch * PartitionTask::GetMetadata(void) {
  return _search_partition;
}

// copy constructor
PartitionTask::PartitionTask(const PartitionTask & t) : 
  BaseTask(t) {
  cout << "I am at the copy PartitionTask & CONSTRUCTOR" << endl;
  _search_partition = t._search_partition;
}
