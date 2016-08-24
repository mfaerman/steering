#ifndef __META_TASKS_H__
#define __META_TASKS_H__

#include<math.h>

#include"simgrid.h"
#include"parameters.h"
#include"tree_ptasks.h"
#include"helper.h"
#include"tasks.h"

class MetaTask : public BaseTask {
private:
  double _remaining_time;  
      
  virtual void Schedule(SG_Resource processor, void *);

protected:
  unsigned int _current_step;
  bool Contention(SG_Resource);
  double SumPriorities(SG_Resource);
  double GetStepWorkload(SG_Resource);
  double GetCycle(void) const;
  double GetOverhead(void) const;

public:
  MetaTask(NAME_TYPE,WORKLOAD_TYPE,PRIORITY_TYPE,unsigned int);

  virtual void ScheduleNextStep(SG_Resource processor, HTPTask &);
  unsigned int GetCurrentStep(void) const;
  bool Completed(void) const;

  double GetRemainingTime(void);
  unsigned int GetCurrentStep(void);

};

typedef Handle<MetaTask> HMetaTask;

#endif
