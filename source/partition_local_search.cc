#include "helper.h"
#include "partition_local_search.h"

PartitionFirstImproveLocalSearch::PartitionFirstImproveLocalSearch
(HalfBalancedPartition & s) : 
  FirstImprove<HalfBalancedPartition>(s), 
  _rebalance_evaluations(0),
  _node_task(NULL) {}

//  /////////////////////////////////////////////////////////////////////////
//  PartitionFirstImproveLocalSearch::PartitionFirstImproveLocalSearch
//  (void) : 
//    FirstImprove<HalfBalancedPartition>(HalfBalancedPartition()), 
//    _rebalance_evaluations(0),
//    _node_task(NULL) {}

//  /////////////////////////////////////////////////////////////////////////

PartitionFirstImproveLocalSearch::PartitionFirstImproveLocalSearch
(PartitionFirstImproveLocalSearch & other) : 
  FirstImprove<HalfBalancedPartition>(other), 
  _rebalance_evaluations(other._rebalance_evaluations),
  _node_task(other._node_task) {}

/////////////////////////////////////////////////////////////////////////

HalfBalancedPartition & PartitionFirstImproveLocalSearch::Rebalance(void) {

  // Do NOT use COPY constructor so _shuffle is reset
  HalfBalancedPartition minimum_solution(_current_solution.GetConfiguration());
  HalfBalancedPartition rebalancing_solution(_current_solution.GetConfiguration());
  HalfBalancedPartition * trial = 
    new HalfBalancedPartition(_current_solution.GetConfiguration());

  bool bigside(false);
  float minimum (1e20);
  
  if (_current_solution.GetUnbalance() != 0) {
    for (unsigned int i = 0; i < _current_solution.GetNumNeighbors(); i++) {

 //       //for debugging
//        int ones(0);
//        for (unsigned int i = 0; i < _current_solution.GetConfiguration().size(); i++) {
//  	if (_current_solution.GetConfiguration()[i])
//  	  ones++;
//        }
      ////////////////    

      trial = rebalancing_solution.GetNextNeighbor();
      if (minimum > trial->GetEvaluation()) {
	minimum = trial->GetEvaluation();
	minimum_solution = *trial;
	delete(trial);
      }
      _rebalance_evaluations ++;
    }
  }  
  //     cout << "Balanced Solution: " << " Unbalance " 
  //  	<< _current_solution.GetUnbalance() << " Eval = " 
  //  	<< _current_solution.GetEvaluation() << endl;
   
  _current_solution = minimum_solution;
  // Rebalance();

  return _current_solution;
}

/////////////////////////////////////////////////////////////////////////
//  HalfBalancedPartition & 
//  PartitionFirstImproveLocalSearch::Sniff(unsigned int length) {
//    return FirstImprove<HalfBalancedPartition>::Sniff(length);
//  }

void  PartitionFirstImproveLocalSearch::Sniff(unsigned int length) {
   FirstImprove<HalfBalancedPartition>::Sniff(length);
}

/////////////////////////////////////////////////////////////////////////
HalfBalancedPartition & PartitionFirstImproveLocalSearch::GetOptimum(void) {
  
  HalfBalancedPartition & 
    before_balance_solution(FirstImprove<HalfBalancedPartition>::GetOptimum());

  cout << "\nBefore Rebalancing : " << before_balance_solution.GetEvaluation() 
       << " Unbalance = " << before_balance_solution.GetUnbalance() << endl;
  
  HalfBalancedPartition & 
    balanced_solution(Rebalance());

  cout << "After  Rebalancing : " 
       << balanced_solution.GetEvaluation() 
       << " Unbalance = " << balanced_solution.GetUnbalance() << endl;
  

  cout << "\nRebalance Evaluations = " << _rebalance_evaluations << endl;
  cout << "\nTotal Evaluations = " << _evaluations + _rebalance_evaluations 
       << endl;
}


void PartitionFirstImproveLocalSearch::SpawnLS_Sniff(TPTask & parent, 
						     unsigned int length,
						     PRIORITY_TYPE priority) {
  char name [10]; 
  float workload(1);

  static unsigned int task_index(0);

  //  cout << "Sniff length(" << length << ") = " << Sniff(length);

  Sniff(length);
  PartitionTask * ptask(new PartitionTask(Name_Policy(name, task_index), 
					  workload, 
					  priority, 
					  this));
  
  //_node_task = parent.PInsert(ptask);_
  //_node_task = parent.Insert(ptask);
  _node_task = new TPTask(HBaseTask(ptask));
    
  ScheduleOnResource(GridCreator::Instance()->Choose_Processor(), _node_task);
  task_index++;
}


HTPTask & PartitionFirstImproveLocalSearch::GetTPTask(void) {
  return _node_task;
}


PartitionFirstImproveLocalSearch & 
PartitionFirstImproveLocalSearch::operator= (const LocalSearch<HalfBalancedPartition> & other) {
  if (this != &other) {
    LocalSearch<HalfBalancedPartition>::operator=(other);
    _rebalance_evaluations = 0;
    _node_task = NULL;
  }    
  return *this;
}
