#ifndef __SHUFFLE_H__
#define __SHUFFLE_H__

#include<vector>
#include<math.h>
using namespace std;

#include"random_gen_creator.h"
#include"random_generator.h"

class Shuffle {

private:
  unsigned int _size;
  vector<unsigned int> _choices;
  static Shuffle * _instance;

public:  
  Shuffle(unsigned int);
  Shuffle(Shuffle &);
  unsigned int Draw(void);
  unsigned int size(void) const;
  void Reset(void);
  static Shuffle * Instance(unsigned int);
};

#endif
