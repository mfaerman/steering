// $Id: main_obj.cc,v 1.28 2002/09/23 06:04:37 mfaerman Exp $

#include<string>
#include<list>
#include<math.h>
#include<stdio.h>
#include<iostream>
#include<iomanip>
#include<iosfwd>
#include<algorithm>
using namespace std;

#include"helper.h"
#include"tasks.h"
//#include"tree_tasks.h"
#include"cost_fnc.h"
#include"parameters.h"
#include"grid.h"
#include"array.h"
#include"simulation.h"
#include"simulation_creator.h"


#include"node_generator.h"
#include"flow_control.h"
#include"random_gen_creator.h"
#include"node_gen_creator.h"

#ifndef MAXFLOAT
#define MAXFLOAT 1e20
#endif

bool Look_at_Results(FEEDBACK_TYPE best, float threshold, Grid & grid) {
  return (best > threshold);
}
   
int main(int argc, const char *const *argv) {  
  Parameters::Instance(argc, argv);
  SimulationCreator::Instance();

  Simulation * S (SimulationCreator::Instance());
    
  S->Init_Steering();
  S->Run();

  delete S;

  return 0;
}
