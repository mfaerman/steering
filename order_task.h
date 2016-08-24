#ifndef __ORDER_TASK_H__
#define __ORDER_TASK_H__
#include "tasks.h"
#include "helper.h"
#include<algorithm>

class OrderTask : public BaseTask {

private:
  void ReOrderScheduledTasks(SG_Resource);

public:
  OrderTask(NAME_TYPE, WORKLOAD_TYPE, PRIORITY_TYPE, unsigned int);
  void Schedule(SG_Resource, void *);
};

#endif
