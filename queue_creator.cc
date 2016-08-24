using namespace std;
#include "queue_creator.h"

PriorityQueue * QueueCreator::Instance(void) {
  if (_instance == NULL) {
    switch(Parameters::Instance()->GetRandomSelection()) {
        case false    : 
	  _instance = new PriorityQueue;
	  break;
        case true  : 
	  _instance = new RandomQueue;
	  break;
        default:
	  cout << "Queue Type Error!" << endl;
	  exit (-1);
    }
  }
  return _instance;
}


PriorityQueue * QueueCreator::_instance = NULL;
