using namespace std;
#include "grid_creator.h"

Grid * GridCreator::Instance(const RESOURCE_TYPE rt, 
			     const unsigned int number, 
			     const double speed) {
  if (_instance == NULL) {
    if (Parameters::Instance()->GetGridMapping() == PRIORITY)
      _instance = new GridLimitedMapping(rt, number, speed);
    else {
      switch(Parameters::Instance()->GetSteeringRegime()) {
        case LIMITED        :
	  _instance = new GridLimited(rt, number, speed);
	  break;
        default:
	  _instance = new Grid(rt, number, speed);
      }
    }
  }
  return _instance;
}

Grid * GridCreator::Instance(void) {
  if (_instance == NULL) {
    cerr << "Error -- Accessing non initialized Grid \n";
    exit (-1);
  }
  else
    return _instance;
}


Grid  * GridCreator::_instance = NULL;

