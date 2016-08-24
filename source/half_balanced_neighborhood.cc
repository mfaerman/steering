#include"partition.h"

// constructor
HalfBalancedPartition::HalfBalancedPartition(vector<bool> & configuration) :
  Partition(configuration),
  _shuffle_unbalanced(_graph.size()/2 + 1)
{}

HalfBalancedPartition::HalfBalancedPartition(void) :
  Partition(),
  _shuffle_unbalanced(_graph.size()/2 + 1)
{}

HalfBalancedPartition::HalfBalancedPartition(HalfBalancedPartition & other) :
  Partition(other),
  _shuffle_unbalanced(other._shuffle_unbalanced)
{}

//  size_t HalfBalancedPartition::GetNumNeighbors(void) {
//    return _configuration.size();
//  }


HalfBalancedPartition * HalfBalancedPartition::GetNextNeighbor(void) {
  vector<bool> neighbor(_configuration);
  vector<unsigned int> list_true, list_false;
  unsigned int which, index;


//    //for debugging
//    int ones(0);
//    for (unsigned int i = 0; i < neighbor.size(); i++) {
//      if (neighbor[i])
//        ones++;
//    }
//    ////////////////

  switch (this->GetUnbalance()) {

    case  0: //balanced partition
      index = PickNewRandomNode();
      // change side of partition for node "index"  - flipping bit
      if (neighbor[index])
	neighbor[index] = false;
      else
	neighbor[index] = true;
      break;

    case  2:
      for(unsigned int i = 0; i < _configuration.size(); i++)
	if (_configuration[i] == true)
	  list_true.push_back(i);
      which = _shuffle_unbalanced.Draw();
      neighbor[list_true[which]] = false;
      break;  

    case -2: 
      for(unsigned int i = 0; i < _configuration.size(); i++)
	if (_configuration[i] == false)
	  list_false.push_back(i);
      which = _shuffle_unbalanced.Draw();
      neighbor[list_false[which]] = true;    
      break;

    default:
      cout << "Error in Half Balanced Partition\n";
      exit (-1);
  }   
//    //for debugging
//    ones = 0;
//    for (unsigned int i = 0; i < neighbor.size(); i++) {
//      if (neighbor[i])
//        ones++;
//    }
//    ////////////////

  return new HalfBalancedPartition(neighbor);
}

size_t HalfBalancedPartition::GetNumNeighbors(void) {
  if (GetUnbalance() == 0)
    return _configuration.size();
  else
    return _configuration.size()/2 + 1;
}

size_t HalfBalancedPartition::GetRemainingNeighbors(void) {
  if (GetUnbalance() == 0)
    return _shuffle.size();
  else
    return _shuffle_unbalanced.size();
}


/////////////////////////////////////////////////////////////////////////
