#ifndef __EALS_TASK_H__
#define __EALS_TASK_H__

class EALS_Task : public BaseTask {

private:
  Partition & _encoding;

public:
  void SpawnLS_Sniff(unsigned int);
}

#endif
