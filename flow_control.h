#ifndef __FLOW_CONTROL_H__
#define __FLOW_CONTROL_H__

#include <math.h>
#include "grid.h"
#include "parameters.h"

class FlowControl {
 private:
  unsigned int _num_tokens;
  unsigned int _burst_limit;
  double _previous_update;

  static Grid * _pt_grid;
  static Parameters * _pt_setup;

  bool Cycle_Complete(void);  
  double GetIntervalSincePreviousUpdate(void);
  unsigned int GetNumberCycles(void);

 public:
  FlowControl(unsigned int);
  static void Set_Configuration(Parameters *); 
  static void Set_Grid(Grid *); 

  void Update(void);
  bool GetToken(void);
};

#endif
