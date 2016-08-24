#ifndef __QUEUE_CREATOR_H__
#define __QUEUE_CREATOR_H__

#include "parameters.h"
#include "priority_queue.h"

class QueueCreator {
private:
  static PriorityQueue * _instance;

public:
  static PriorityQueue * Instance(void);
  QueueCreator(void);
};

#endif
