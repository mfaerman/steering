#ifndef __RANDOM_GENERATOR_H__
#define __RANDOM_GENERATOR_H__

class RandomGenerator {
private:
public:
  virtual double GetRand(double) = 0;
};



//---------------------------------------------------------------
class Ran4RandomGenerator : public RandomGenerator {
private:
  unsigned int _seed;
public:
  Ran4RandomGenerator(unsigned int);
  double GetRand(double);
};

//---------------------------------------------------------------
class C_RandomGenerator : public RandomGenerator {
public:
  C_RandomGenerator(unsigned int);
  double GetRand(double);
};

#endif 
