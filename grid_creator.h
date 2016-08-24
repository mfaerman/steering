#ifndef __GRID_CREATOR_H__
#define __GRID_CREATOR_H__

#include "parameters.h"
#include "grid.h"
#include "grid_limited.h"
#include "grid_limited_mapping.h"

class GridCreator {

private:
  static Grid * _instance;

public:
  static Grid * Instance(const RESOURCE_TYPE, const unsigned int, const double);
  static Grid * Instance(void);
  GridCreator(int, const char * const *);
};

#endif
