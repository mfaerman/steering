#include"random_generator.h"

RandomGenerator * RandomGenerator::Instance(void) {
  if (_instance == NULL) {
    _instance = RandomGenCreator().GetProduct();
  }
  return _instance;
}
