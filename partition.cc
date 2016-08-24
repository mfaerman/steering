#include"partition.h"

Partition::Partition(void) :
  _graph(PartitionGraph::Instance()->GetInfo()),
  _current_node(0),
  _subset_size(0),
  _shuffle(_graph.size()),
  _configuration(_graph.size(), false),
  _task_index(0)
   //  _node_task(NULL)
  //produce random balanced partition
{
  for (unsigned int i = 0; i < _configuration.size()/2; i++)
    _configuration[_shuffle.Draw()] = true;
  _shuffle = Shuffle(_graph.size());
  _subset_size = _graph.size()/2;
}

Partition::Partition(vector<bool> & configuration) :
  _configuration(configuration), 
  _graph(PartitionGraph::Instance()->GetInfo()),
  _current_node(0),
  _shuffle(_graph.size()),
  _subset_size(0),
  _task_index(0)
{

  //cout << "\nConf = ";
  for (unsigned int i = 0; i < _configuration.size(); i++) {
    //cout << _configuration[i];
    if (_configuration[i])
      _subset_size++;
  }
  //cout << " Size = " << _configuration.size() << endl;
}

Partition::Partition(Partition & other) :  
  _graph(other._graph),
  _subset_size(other._subset_size),
  _task_index(other._task_index),
  _current_node(other._task_index),
  _configuration(other._configuration),
  _shuffle(other._shuffle)
{}

bool Partition::operator > (Partition & p) {
  return ((GetEvaluation() - p.GetEvaluation()) > EPSILON_PARTITION);

}

bool operator > (const Partition & p1, const Partition & p2) {
  return ((p1.GetEvaluation() - p2.GetEvaluation()) > EPSILON_PARTITION);
}

bool operator < (const Partition & p1, const Partition & p2) {
  return ((p2.GetEvaluation() - p1.GetEvaluation()) > EPSILON_PARTITION);
}


float Partition::GetEvaluation(void) const {
  unsigned int total_crossing(0);

  for (unsigned int i = 0; i < _configuration.size(); i++) {
    if (_configuration[i] == true)
      total_crossing += CountCrossingEdges(i);
  }
  return total_crossing + GetPenalty();
}


unsigned int Partition::CountCrossingEdges(unsigned int node) const {

  unsigned int count(0);
  unsigned int opposite_node;

  for (unsigned int i = 0; i < _graph[node].GetEdges()->size(); i++) {
    opposite_node = (*(_graph[node].GetEdges()))[i];
    if (_configuration[opposite_node] != _configuration[i])  
      count++;
  }
  
  return count; 
}

int Partition::GetUnbalance(void) const{
  int opposite_subset_size = _graph.size() - _subset_size;
  return _subset_size - opposite_subset_size;
}


float Partition::GetPenalty(void) const{
  return pow(GetUnbalance(),2) * 0.05;
}

vector<bool> & Partition::GetConfiguration(void) {
  return _configuration;
}

size_t Partition::PickNewNode(void) {

  //change to (non-repeatable) random choice
  _current_node++;
  return (_current_node - 1);
}

size_t Partition::PickNewRandomNode() {
  return _shuffle.Draw();
}

unsigned int ChooseSwapNode(unsigned int originator, vector<bool> & configuration) {

  unsigned int i(0);
  unsigned int pair(originator);
  bool partition_originator(configuration[originator]);
  vector<unsigned int> * pt_edges(PartitionGraph::Instance()->GetInfo()[originator].GetEdges()); 
  
  if (!pt_edges->empty()) {
    Shuffle shuffle_edges(pt_edges->size());
    do {
      pair = shuffle_edges.Draw();
      i++;
    }
    while(i < pt_edges->size()                                  &&
	  configuration[pair] == configuration[originator]);

    // check not all nodes are in the same partition
    if(configuration[pair] != configuration[originator])
      return pair;
    else 
      return originator;
  }
  else
    return originator;
}
//  void Partition::SpawnLS_Sniff(TTask & parent, 
//  			      unsigned int length,
//  			      PRIORITY_TYPE priority) {
//    char name [10]; 
//    float workload(1);
//    PartitionTask * ptask;

//    static unsigned int task_index(0);

//    //  cout << "Sniff length(" << length << ") = " << Sniff(length);

//    _search->Sniff(length);
//    ptask = new PartitionTask(Name_Policy(name, task_index), 
//  			     workload, 
//  			     priority, 
//  			     this);
  
//    _node_task = parent.PInsert(ptask);
    
//    ScheduleOnResource(Grid::Instance()->Choose_Processor(), _node_task);
//    task_index++;
//  }


//  //  TTask * Partition::GetTTask(void) const {
//  //    return _node_task;
//  //  }



