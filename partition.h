#ifndef __PARTITION_H__
#define __PARTITION_H__

#include<vector>
#include <math.h>
using namespace std;

#include"shuffle.h"
#include"graph.h"
#include"tree_ptasks.h"
#include"grid.h"
//#include"helper.h"
//#include"search.h"
//#include"bin_task.h"

#define EPSILON_PARTITION 1e-4

class Partition {
private:
  unsigned int _subset_size;
  unsigned int _task_index;
  unsigned int CountCrossingEdges(unsigned int) const;
  float GetPenalty(void) const;

protected:  
  vector<NodeInfo> _graph;
  unsigned int _current_node;
  vector<bool> _configuration;

  Partition(vector<bool> &);
  virtual Partition *  GetNextNeighbor(void) {cout << "error"; exit(-1);};
  Shuffle _shuffle;
  size_t PickNewNode(void);
  size_t PickNewRandomNode(void);

public:
  int GetUnbalance(void) const;
  float GetEvaluation(void) const;
  vector<bool> & GetConfiguration(void);
  bool operator > (Partition &);
  Partition(void);
  Partition(Partition &);
  virtual ~Partition(void) {}
  TPTask * GetTTask(void) const;
};

class BalancedPartition : public Partition {
public:
  BalancedPartition * GetNextNeighbor(void);
  ~BalancedPartition(void) {}
  
};

class UnbalancedPartition : public Partition {
private:
public:
  UnbalancedPartition(vector<bool> &); 
  UnbalancedPartition(void);
  UnbalancedPartition(UnbalancedPartition &);
  ~UnbalancedPartition(void) {}
  UnbalancedPartition * GetNextNeighbor(void);
virtual  size_t GetNumNeighbors(void);
virtual  size_t GetRemainingNeighbors(void);
};

class HalfBalancedPartition : public Partition {
private:
  Shuffle _shuffle_unbalanced;
public:
// constructor
  HalfBalancedPartition(vector<bool> &);
  HalfBalancedPartition(void);
  HalfBalancedPartition(HalfBalancedPartition &);
  ~HalfBalancedPartition(void) {}
  HalfBalancedPartition * GetNextNeighbor(void);
  size_t GetNumNeighbors(void);
  size_t GetRemainingNeighbors(void);
};

bool operator > (const Partition &, const Partition &);
bool operator < (const Partition &, const Partition &);

unsigned int ChooseSwapNode(unsigned int, vector<bool> &);

#endif





