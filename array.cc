#include "array.h"

unsigned int GetArraySize(SG_Task * array) {
  unsigned int i = 0;
  while (array[i] != NULL)
    i++;
  return i;
}


int name_cmp (const void * sg1, const void * sg2) {
  HTPTask node1, node2;

  node1 = *static_cast<HTPTask *> 
    (SG_getTaskMetadata(*static_cast<const SG_Task*>(sg1)));

  node2 = *static_cast<HTPTask *>
    (SG_getTaskMetadata(*static_cast<const SG_Task*>(sg2)));

  return (-(*node1))->name().compare((-(*node2))->name());
}

int priority_cmp(const void * sg1, const void * sg2) {
  HTPTask node1, node2;

  node1 = *(static_cast<HTPTask *> 
    (SG_getTaskMetadata(*static_cast<const SG_Task*>(sg1))));

  node2 = *(static_cast<HTPTask *>
    (SG_getTaskMetadata(*static_cast<const SG_Task*>(sg2))));

}


void SortSimgridTaskArray (SG_Task * array) {
  qsort(array, GetArraySize(array), sizeof(SG_Task), name_cmp);
}
  
void SortScheduledTasksArray (SG_Task * array) {
  qsort(array, GetArraySize(array), sizeof(SG_Task), priority_cmp);
}
