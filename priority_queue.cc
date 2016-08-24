#include <assert.h>

#include "priority_queue.h"
//#include "grid_creator.h"

PriorityQueue * PriorityQueue::_instance = NULL;

PriorityQueue::PriorityQueue(void) :
  _inserted_tasks(0),
  _dropped_tasks(0),
  _PQ(new deque<HTPTask>),
  _pt_setup(Parameters::Instance()),
  _pt_grid(GridCreator::Instance())
{}

//  PriorityQueue * PriorityQueue::Instance(void) {
//    if (_instance == NULL) {
//      _instance = new  PriorityQueue;
//    }
//    return _instance;
//  }

void PriorityQueue::Remove_Worst(void) {
  deque<HTPTask>::iterator 
    worst(min_element(_PQ->begin(), _PQ->end(), Larger_Feedback()));
  assert(*worst);

  _dropped_tasks++;
  (*worst)->Drop();

  _PQ->erase(worst);

  // assert(!(*worst));

  make_heap(_PQ->begin(),_PQ->end(), Larger_Feedback()); 
}

void PriorityQueue::Insert(HTPTask & node) {
  _inserted_tasks++;

  if (_PQ->size() == _pt_setup->Get_PQ_Size()) {               // queue is full!
    Remove_Worst();
  }
  if (_PQ->empty()) { // start filling --  need to "make_heap"
     _PQ->push_back(node); 
     make_heap(_PQ->begin(),_PQ->end(), Larger_Feedback());
  }
  else {  // already a heap
    _PQ->push_back(node);                                  // insert new
    push_heap(_PQ->begin(),_PQ->end(), Larger_Feedback());
  }    
  assert(is_heap(_PQ->begin(),_PQ->end(), Larger_Feedback()));
}

HTPTask PriorityQueue::GetNextTask(void) {
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
    next = _PQ->front();
    pop_heap(_PQ->begin(), _PQ->end(), Larger_Feedback());  // remove from queue

    assert(next == _PQ->back());
    _PQ->pop_back();
  }
  return next;
}

bool PriorityQueue::Empty(void) {
  return _PQ->empty();
}

void PriorityQueue::Finalization(void) {
  cout << "--- Priority Queue Information ---\n";
  cout << "Number of generated tasks      = " << _inserted_tasks  << endl;
  cout << "Number of dropped tasks        = " << _dropped_tasks   << endl;
  cout << "Number of tasks in the queue   = " << _PQ->size()      << endl;
}














//  void PriorityQueue::Reinitialize_Steering(void) { 
//    char name [10]; 
//    HTPTask it;
//    BaseTask * pt_task;
//    CONFIGURATION_TYPE conf;
  
//    for (unsigned int i = 0; i < _pt_setup->Get_Num_Dim()*2; i++) {
//      sprintf(name,"T%d", i); // task naming
//      it = _ttree.Insert(BaseTask(Name_Policy(name, i), 
//  			  Workload_Policy(), 
//  			  Start_Priority_Policy(), 
//  			  _pt_setup->Get_Num_Dim()));
//      it->Set_Fraction(1);
//      it->Set_Level(1); // level 0 is tree root
//      pt_task = it->pt_content();
//      pt_task->Set_Rand_Config(_pt_setup->Get_Scatter_Radius());
//      pt_task->Schedule(_pt_grid->Choose_Processor(), it);
//    }    
//  }



