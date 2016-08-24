#ifndef __GRAPH_H__
#define __GRAPH_H__

#include<vector>
#include<string>
#include<fstream>
#include<math.h>

using namespace std;

#include "parameters.h"
#include "random_gen_creator.h"

class NodeInfo {
private:
  unsigned int _node;
  string _coordinates;
  unsigned int _edge_count;
  vector<unsigned int> * _edges;
public:
  NodeInfo(unsigned int, string, unsigned int, vector<unsigned int> *);
  unsigned int GetNode(void) const;
  vector<unsigned int> * GetEdges(void) const;
  unsigned int CountCrossingEdges(void) const;
};


class PartitionGraph {
 private:
  vector<NodeInfo> _graph_info;
  static PartitionGraph * _instance;
  PartitionGraph(void);

 public:
  static PartitionGraph * Instance(void);
  vector<NodeInfo> & GetInfo(void);  
  unsigned int GetSize(void);
};

#endif
