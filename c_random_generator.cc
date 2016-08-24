#include "stdlib.h"
#include "random_generator.h"

C_RandomGenerator::C_RandomGenerator(unsigned int seed) {
  srand(seed);
}

double C_RandomGenerator::GetRand(double range) {  
return range * 
  (static_cast<double>(rand()) / static_cast<double>(RAND_MAX+1.0));
}
