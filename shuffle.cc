#include <assert.h>

#include"shuffle.h"

Shuffle::Shuffle(unsigned int size) :
  _size(size),
  _choices(size)
{
  for(unsigned int i = 0; i < _size; i++)
    _choices[i] = i;
}

Shuffle::Shuffle(Shuffle & other) :
  _size(other._size),
  _choices(other._choices)
{}

unsigned int Shuffle::Draw(void) {
  unsigned int index;
  unsigned int value;
  RandomGenerator * rg(RandomGenCreator::Instance());
  
  assert(_choices.size() > 0);
  index = (unsigned int) rint(rg->GetRand(_choices.size() - 1));
  value = _choices[index];
  
  _choices.erase(&_choices[index]);

  return value;
}

Shuffle * Shuffle::_instance = NULL;

Shuffle * Shuffle::Instance(unsigned int size) {
  if (_instance == NULL) {
    _instance = new Shuffle(size);
  }
  return _instance;
}

unsigned int Shuffle::size(void) const {
  return _choices.size();
}

void Shuffle::Reset(void) {
  _choices.clear();
  for(unsigned int i = 0; i < _size; i++)
    _choices.push_back(i);
}



  


  
  
  

                                       
