#include "random_generator.h"
#include "ran4.h"
#include "seed_array.h"


Ran4RandomGenerator::Ran4RandomGenerator(unsigned int seed_index) {
  // seed_index range: 1 to 3000
  _seed = seed_array[seed_index-1];
  ran4_init(&_seed);
}

double Ran4RandomGenerator::GetRand(double range) {
  double value;
  return ran4(&_seed, &value, 1, range);
}



    
