#include <iostream>
using namespace std;

#include "random_gen_creator.h"

// (required) definition of static members
Parameters *  RandomGenCreator::_pt_setup = NULL;

void RandomGenCreator::Set_Configuration(Parameters * pt_setup) {
  _pt_setup = pt_setup;
}

RandomGenerator * RandomGenCreator::Instance(void) {
  if (_instance == NULL) {
      switch (Parameters::Instance()->Get_Random_Generator_Type()) {
        case MCELL_RAN4      : 
	  _instance = new Ran4RandomGenerator
	    (Parameters::Instance()->Get_Random_Seed());
	  break;
        case C_RAND          : 
	  _instance = new C_RandomGenerator
	    (Parameters::Instance()->Get_Random_Seed());
	  break;
        default:
	  cout << "Random Generator Type Error!";
	  exit (-1);
      }
  }
      return _instance;
}

RandomGenerator * RandomGenCreator::_instance = NULL;


RandomGenCreator::RandomGenCreator(void) : Creator<RandomGenerator>() {}

//  RandomGenerator * RandomGenCreator::CreateProduct(void) {
//    switch (_pt_setup->Get_Random_Generator_Type()) {
//      case MCELL_RAN4      : 
//        return new Ran4RandomGenerator(_pt_setup->Get_Random_Seed());
//      case C_RAND          : 
//        return new C_RandomGenerator(_pt_setup->Get_Random_Seed());
//      default:
//        cout << "Random Generator Type Error!";
//        exit (-1);
//    }
//  }
