#ifndef __RANDOM_GEN_CREATOR_H__
#define __RANDOM_GEN_CREATOR_H__

#include "creator.h"
#include "parameters.h"
#include "random_generator.h"

class RandomGenCreator : public Creator<RandomGenerator> {
private:
  static RandomGenerator * _instance;
  static Parameters * _pt_setup;
  //RandomGenerator * CreateProduct(void);

public:
  static void Set_Configuration(Parameters *);
  static RandomGenerator * Instance(void);
  RandomGenCreator(void);
};

#endif
