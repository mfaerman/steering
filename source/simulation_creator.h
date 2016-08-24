#ifndef __SIMULATION_CREATOR_H__
#define __SIMULATION_CREATOR_H__

#include "creator.h"
#include "parameters.h"
#include "simulation.h"

class SimulationCreator : public Creator<Simulation> {

private:
  static Simulation * _instance;
  static Parameters * _pt_setup;
//    int _argc;
//    const char * const * _argv;
  Simulation * CreateProduct(void);

public:
  static void Set_Configuration(Parameters *);
  static Simulation * Instance(void);
  SimulationCreator(int, const char * const *);
};

#endif
