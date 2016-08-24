#include "partition.h"


BalancedPartition BalancedPartition::GetNextNeighbor(void) {
  vector<bool> neighbor(_configuration);

  first = PickNode();
  second = PickOppositeNode();

  //swap nodes between partitions
  swap(neighbor[first], neighbor[second]);

}




//  BalancedPartitionList BalancedPartition::GetNeighborhood(void) {
//    vector<bool> neighbor(_current_configuration);

//    for (unsigned int i = 0; i < _num_nodes; i++)
//      for (unsigned int j = 0; j < _num_nodes; i++) {
//        swap(neighbor[i], neighbor[j]);
//        _neighborhood_list.push(neighbor);   
//      }
//    return & _neighbor_list;
//  }

