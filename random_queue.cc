#include <assert.h>

#include "priority_queue.h"

RandomQueue::RandomQueue(void) : PriorityQueue() {}

void RandomQueue::Remove_Worst(void) {
  unsigned int index((unsigned int) 
    rint(RandomGenCreator::Instance()->GetRand(_PQ->size() - 1)));
  deque<HTPTask>::iterator chosen(_PQ->begin() + index);
  
  _dropped_tasks++;
  (*chosen)->Drop();

  _PQ->erase(chosen);
}

void RandomQueue::Insert(HTPTask & node) {
  _inserted_tasks++;

  if (_PQ->size() == _pt_setup->Get_PQ_Size()) {               // queue is full!
    Remove_Worst();
  }
  _PQ->push_back(node); 
}

HTPTask RandomQueue::GetNextTask(void) {
  HTPTask next;

  if (_PQ->empty()) {
    if(_pt_grid->Empty()) {
      cout << "EMPTY SYSTEM\n\n";
      exit(-2);
      // Insert Random Task(s)
    }
    next = NULL;
  }
  else {
    unsigned int index((unsigned int) 
      rint(RandomGenCreator::Instance()->GetRand(_PQ->size() - 1)));

    deque<HTPTask>::iterator chosen(_PQ->begin() + index);
    next = *chosen;
    _PQ->erase(chosen);
  }
  return next;
}

