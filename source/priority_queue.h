#ifndef __PRIORITY_QUEUE_H__
#define __PRIORITY_QUEUE_H__

#include <deque>
using namespace std;

#include "grid_creator.h"
#include "grid.h"
#include "parameters.h"
#include "main.h"

class PriorityQueue {
protected:
  deque<HTPTask> * _PQ;
  unsigned int _inserted_tasks;
  unsigned int _dropped_tasks;

  static PriorityQueue* _instance;
  Parameters * _pt_setup;
  Grid * _pt_grid;  

  virtual void Remove_Worst(void);
   
public:
  PriorityQueue();
  virtual void Insert(HTPTask &);
  virtual HTPTask GetNextTask(void);
  bool Empty(void);
  void Finalization(void);
};

class RandomQueue : public PriorityQueue {
private:
  void Remove_Worst(void);
public:
  RandomQueue(void);
  void Insert(HTPTask &);
  HTPTask GetNextTask(void);
};

#endif
