//#include "simulation_creator.h"
using namespace std;
#include "simulation.h"

// (required) definition of static members
Parameters *  SimulationCreator::_pt_setup = NULL;

void SimulationCreator::Set_Configuration(Parameters * pt_setup) {
  _pt_setup = pt_setup;
}

Simulation * SimulationCreator::Instance(void) {
  if (_instance == NULL) {
    switch(Parameters::Instance()->GetSteeringRegime()) {
        case SELECT_BEST    : 
	  _instance = new Simulation;
	  break;
        case STEADY_STATE   : 
	  _instance = new PQueueSimulation;
	  break;
        case LIMITED        :
	  if (Parameters::Instance()->GetRandomSelection())
	    _instance = new SimulationLimitedRandom;
	  else
	    _instance = new SimulationLimited;
	  break;
        default:
	  cout << "Steering Regime Type Error!";
	  exit (-1);
    }
  }
  return _instance;
}


Simulation * SimulationCreator::_instance = NULL;

//SimulationCreator::SimulationCreator(void) : Creator<Simulation>() {}

//  Simulation * SimulationCreator::CreateProduct(void) {
//    switch(Parameters::Instance()->GetSteeringRegime()) {
//      case SELECT_BEST  : return new Simulation;
//      case STEADY_STATE : return new PQueueSimulation;
//      default:
//        cout << "Steering Regime Type Error!";
//        exit (-1);
//    }
//  }
