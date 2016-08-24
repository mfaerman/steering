#include <iostream>
using namespace std;

#include "grid_limited_mapping.h"

GridLimitedMapping::GridLimitedMapping(const RESOURCE_TYPE rt, 
				       const unsigned int number, 
				       const double speed) : 
  GridLimited(rt, number, speed),
  _limit_proc(0) {

  ifstream input(Parameters::Instance()->GetGridFileName().c_str());
 
  if (!input) {
    cerr << "grid file error" << endl;
    exit(-1);
  }
  
  for(unsigned int i = 0; i < _processors->size(); i++) {
    unsigned int aux;
    input >> aux;
    _limit_proc.push_back(aux);
  }

  for(unsigned int i = 0; i < _processors->size(); i++) 
    cout << _limit_proc[i] << " ";
 
  cout << endl;
}

unsigned int GridLimitedMapping::GetProcLoad(unsigned int p) {
  SG_Task * result(SG_getTasksScheduledOnResource((*_processors)[p]));
  unsigned int num_tasks(GetArraySize(result));
  free(result);

  return num_tasks;
}

bool GridLimitedMapping::ProcessorAvailable(unsigned int p) {
  SG_Task * result(SG_getTasksScheduledOnResource((*_processors)[p]));
  unsigned int num_tasks(GetArraySize(result));
  free(result);

  return (num_tasks < _limit_proc[p]);
}
  
unsigned int GridLimitedMapping::GetProcLimit(SG_Resource & SG_proc) {
  unsigned int p(0);

  while(p < _processors->size() &&
	SG_proc != (*_processors)[p])
    p++;

  assert(SG_proc == (*_processors)[p]);
  return _limit_proc[p];
}

bool load_comp(unsigned int p1, unsigned int p2) {
  GridLimitedMapping * g((GridLimitedMapping *) GridCreator::Instance());

  if(g->GetProcLoad(p1) == g->GetProcLoad(p2))
    return p1 < p2;
  else
    return g->GetProcLoad(p1) < g->GetProcLoad(p2);
}

SG_Resource & GridLimitedMapping::Choose_Processor(void) {
  unsigned int proc_index(0);
  vector<unsigned int> avail_proc;

  assert (!GridFull());

  for(unsigned int p(0); p < _processors->size(); p++)
    if (ProcessorAvailable(p))
	avail_proc.push_back(p);

  proc_index = 
    *min_element(avail_proc.begin(), avail_proc.end(), load_comp);
  
//    while(!ProcessorAvailable(proc_index) && proc_index < _processors->size()) 
//      proc_index++;

//    assert(proc_index < _processors->size());
  return Get_Processor(proc_index);
}


//  bool GridLimited::GridFull(void) {
//    unsigned int p(0);

//    do {
//      p++;
//    }
//    while(p < _processors->size() && 
//  	!ProcessorAvailable(p));

//    return (p == _processors->size());
//  }


//  unsigned int GridLimitedMapping::GridCapacity(void) {
//    unsigned int sum(0);

//    for(unsigned int i = 0; i < _limit_proc.size(); i++)
//      sum = sum + _limit_proc[i];
    
//    return sum;
//  }
