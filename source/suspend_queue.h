#ifndef __SUSPEND_QUEUE_H__
#define __SUSPEND_QUEUE_H__

#include <deque>
using namespace std;

#include "grid.h"
#include "parameters.h"
#include "main.h"

class SuspendQueue {
private:
  deque<HTPTask> * _SQ;
  unsigned int _dropped_tasks;
  unsigned int _inserted_tasks;

  static SuspendQueue* _instance;
  Parameters * _pt_setup;
  Grid * _pt_grid;  

  void Remove_Worst(void);
 
protected:
  SuspendQueue(void);

public:
  static SuspendQueue * Instance(void);
  void Insert(HTPTask &);
  HTPTask  GetNextTask(void);
  void Finalization(void);

};

#endif
