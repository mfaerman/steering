#ifndef __TASK_CREATOR_H__
#define __TASK_CREATOR_H__

#include "creator.h"
#include "parameters.h"
#include "tasks.h"
#include "order_task.h"
#include "meta_task.h"
#include "meta_task_limit.h"
#include "meta_task_limit_random.h"

class TaskCreator : public Creator<BaseTask> {
private:
  static Parameters * _pt_setup;

  PRIORITY_TYPE _priority;
  NAME_TYPE _name;
  WORKLOAD_TYPE _work;
  unsigned int _num_dim;

  BaseTask * CreateProduct(void);

public:
  static void Set_Configuration(Parameters *);
  TaskCreator(NAME_TYPE, WORKLOAD_TYPE, PRIORITY_TYPE, unsigned int);
};

#endif
