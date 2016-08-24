#include "simulation.h"

void SimulationLimited::MoveFromQueueToGrid(void) {
  HTPTask node_task;

  while (! ((GridLimited *)GridCreator::Instance())->GridFull()          &&
	 ! QueueCreator::Instance()->Empty()) {
    node_task = QueueCreator::Instance()->GetNextTask();
    if (node_task != NULL)
      (-(*node_task))->Schedule(_pt_grid->Choose_Processor(), new HTPTask(node_task));
  }
}





