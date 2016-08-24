#ifndef __GRID_LIMITED_MAPPING_H__
#define __GRID_LIMITED_MAPPING_H__

#include<fstream>
#include "grid_limited.h"
#include "array.h"
#include "grid_creator.h"


bool load_comp(unsigned int, unsigned int);


class GridLimitedMapping : public GridLimited {
private:
  vector<unsigned int> _limit_proc;
  bool ProcessorAvailable(unsigned int);

  //  unsigned int GridCapacity(void);
  //  bool GridFull(void);

public:
  unsigned int GetProcLimit(SG_Resource &);
  GridLimitedMapping(const RESOURCE_TYPE, const unsigned int, const double);
  unsigned int GetProcLoad(unsigned int);
  SG_Resource & Choose_Processor(void);
};



#endif
