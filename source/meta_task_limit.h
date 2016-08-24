#ifndef __META_TASK_LIMIT_H__
#define __META_TASK_LIMIT_H__

#include "suspend_queue.h"
#include "queue_creator.h"
#include "meta_task.h"
#include "helper.h"
#include "array.h"
#include "grid_limited_mapping.h"

#include<algorithm>

typedef enum {SCHEDULED, SUSPENDED} SCHEDULING_STATUS;

class MetaTaskLimit : public MetaTask {
private:
  unsigned int GetTaskLimit(SG_Resource &) const;

protected:
  bool _overhead;

  bool HasOverhead(void) const;
  void RemoveOverhead(void);
  void AddOverhead(void);

  virtual SCHEDULING_STATUS LimitTasks(SG_Resource &);
  unsigned int GetNumProcessorTasks(SG_Resource &) const;
  Parameters * GetSetup(void) const;
  void MoveToSuspendQueue(HTPTask &) const;

public:
  MetaTaskLimit(NAME_TYPE, WORKLOAD_TYPE, PRIORITY_TYPE, unsigned int);
  void Schedule(SG_Resource, void *);
  void ScheduleNextStep(SG_Resource, HTPTask &);
};

typedef Handle<MetaTaskLimit> HMetaTaskLimit;

#endif
