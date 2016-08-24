#include <iostream>
using namespace std;

#include "grid.h"

Grid::Grid(void) {
  Init_Grid(PROCESSOR, 10, 1.0); 
}

Grid::Grid(const unsigned int number) {
  Init_Grid(PROCESSOR, number, 1.0);
}

Grid::Grid(const RESOURCE_TYPE rt, const unsigned int number) {
  Init_Grid(rt, number, 1.0);
}

Grid::Grid(const RESOURCE_TYPE rt, const unsigned int number, 
	   const double speed) {
  Init_Grid(rt, number, speed);
}

void Grid::Init_Grid(const RESOURCE_TYPE rt, 
	   const unsigned int number, 
	   const double speed) {
  char name[10];  

  /* Initializing Simgrid */
  SG_init();

  _next_processor_cyclic = 0;

  switch (rt) {
    case PROCESSOR: {
      _processors = new vector<SG_Resource>(number);
      for (unsigned int i = 0; i < number; i++) {
	sprintf(name, "p%d", i);
	(*_processors)[i] = 
	  SG_newHost(name,speed,Parameters::Instance()->GetResourceSharing(),
		     NULL,0.0,1.0,NULL);
      }
      break;
    }
  }
}


// copy constructor
Grid::Grid(const Grid & other) {
  _processors = new vector<SG_Resource>(other._processors->size());
  for(unsigned int i = 0; i < other._processors->size(); i++)
    (*_processors)[i] = (*other._processors)[i];
  _next_processor_cyclic = other._next_processor_cyclic;
}

Grid::~Grid(void) {        // destructor
  if (_processors != NULL)
    delete _processors;
}

Grid & Grid::operator = (const Grid & other) {
  if (this != &other) {
    if (_processors != NULL)
      delete _processors;
    _processors = new vector<SG_Resource>(other._processors->size());
    for(unsigned int i = 0; i < other._processors->size(); i++)
      (*_processors)[i] = (*other._processors)[i];
    _next_processor_cyclic = other._next_processor_cyclic;
  }
  return *this;
}

SG_Resource & Grid::Get_Processor(const unsigned int i) const {
  return (*_processors)[i];
}

SG_Resource & Grid::Choose_Processor(void) {
  SG_Resource proc;

  proc = Get_Processor(_next_processor_cyclic);

  _next_processor_cyclic = 
    (_next_processor_cyclic + 1) % _processors->size();

  return (proc);
}

double Grid::GetCurrentTime(void) {
  return  SG_getClock();
}

void Grid::UpdateElapsedTime(void) {
 _previous_time = SG_getClock();
}

double Grid::GetElapsedTime(void) {
  return SG_getClock() - _previous_time;
}

bool Grid::Empty(void) {
  return (Total() == 0);
}

unsigned int Grid::Total(void) {
  int num_tasks_not_scheduled, num_tasks_scheduled, 
      num_tasks_ready, num_tasks_running;

  SG_getStateDistribution(&num_tasks_not_scheduled, 
			  &num_tasks_scheduled,
			  &num_tasks_ready, 
			  &num_tasks_running, 
			  NULL); 

//   return (num_tasks_not_scheduled + num_tasks_scheduled +
//  	 num_tasks_ready + num_tasks_running);

  //  return (num_tasks_scheduled + num_tasks_running + num_tasks_ready); // double check w/ Henri
  return (num_tasks_scheduled + num_tasks_running); // double check w/ Henri
}

void Grid::Finalization(void) {
  cout << "Tasks in the Grid = " << Total() << endl;
}


//  Grid * Grid::_instance = NULL;

//  Grid * Grid::Instance(const RESOURCE_TYPE rt, 
//  		      const unsigned int number, 
//  		      const double speed) {
//    if (_instance == NULL) {
//      _instance = new Grid(rt, number, speed);
//    }
//    return _instance;
//  }


//  Grid * Grid::Instance(void) {
//    if (_instance == NULL) {
//      cerr << "Error -- Accessing non initialized Grid \n";
//      exit (-1);
//    }
//    else
//      return _instance;
//  }
