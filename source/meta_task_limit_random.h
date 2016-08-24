#ifndef __META_TASK_LIMIT_RANDOM_H__
#define __META_TASK_LIMIT_RANDOM_H__

#include "meta_task_limit.h"

class MetaTaskLimitRandom : public MetaTaskLimit {
private:
  SCHEDULING_STATUS LimitTasks(SG_Resource &);
public:
  MetaTaskLimitRandom(NAME_TYPE, WORKLOAD_TYPE, PRIORITY_TYPE, unsigned int);
};

typedef Handle<MetaTaskLimitRandom> HMetaTaskLimitRandom;

#endif
