#include <iostream>
using namespace std;

#include "grid_limited.h"

GridLimited::GridLimited(const RESOURCE_TYPE rt, 
			 const unsigned int number, 
			 const double speed) : 
  Grid(rt, number, speed) 
{}

bool GridLimited::ProcessorAvailable(unsigned int p) {
  SG_Task * result(SG_getTasksScheduledOnResource((*_processors)[p]));
  unsigned int num_tasks(GetArraySize(result));
  free(result);

  return (num_tasks < Parameters::Instance()->GetTaskLimit());
}
  
SG_Resource & GridLimited::Choose_Processor(void) {
  unsigned int num_proc(0);
  unsigned int proc_index(0);

  assert (!GridFull());

  if(ProcessorAvailable(_next_processor_cyclic)) {
    proc_index = _next_processor_cyclic;
    _next_processor_cyclic = (_next_processor_cyclic + 1) % _processors->size();
  }
  else {
    while(!ProcessorAvailable(_next_processor_cyclic) &&
	  num_proc < _processors->size()                 ) {
      _next_processor_cyclic = (_next_processor_cyclic + 1) % _processors->size();
      proc_index = _next_processor_cyclic;
      num_proc ++;
    }  
  }
    assert(num_proc < _processors->size());

    return Get_Processor(_next_processor_cyclic);
}

bool GridLimited::GridFull(void) {
  unsigned int p(0);

  do {
    p++;
  }
  while(p < _processors->size() && 
	!ProcessorAvailable(p));

  return (p == _processors->size());
}
