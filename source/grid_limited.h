#ifndef __GRID_LIMITED_H__
#define __GRID_LIMITED_H__

#include "grid.h"
#include "array.h"

class GridLimited : public Grid {
private:
  virtual bool ProcessorAvailable(unsigned int p);

public:
  GridLimited(const RESOURCE_TYPE, const unsigned int, const double);
  virtual SG_Resource & Choose_Processor(void);
  bool GridFull(void);
};

#endif
