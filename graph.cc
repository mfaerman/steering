#include"graph.h"

NodeInfo::NodeInfo(unsigned int node,
		   string coordinates,
		   unsigned int edge_count,
		   vector<unsigned int> * edges) :
  _node(node), _coordinates(coordinates), 
  _edge_count(edge_count), _edges(edges) {}

vector<unsigned int> * NodeInfo::GetEdges(void) const{
  return _edges;
}

unsigned int NodeInfo::GetNode(void) const {
  return _node;
}


PartitionGraph::PartitionGraph(void) {
 
  unsigned int node, quantity, aux;
  string coordinates;

  ifstream from(Parameters::Instance()->Get_Graph_File_Name().data());  

  while(!from.eof()) {
    from >> node;
    if (from.eof()) break;
    from >> coordinates >> quantity;

    vector<unsigned int> * edges = new vector<unsigned int>;
    for (unsigned int i = 0; i < quantity; i++) {
      from >> aux;
      edges->push_back(aux);
    } 
    _graph_info.push_back(NodeInfo(node, coordinates, quantity, edges));
  }
}

PartitionGraph * PartitionGraph::_instance = NULL;

PartitionGraph * PartitionGraph::Instance(void) {
  if (_instance == NULL) {
    _instance = new PartitionGraph();
  }
  return _instance;
}

vector<NodeInfo> & PartitionGraph::GetInfo(void) {
  return  _graph_info;
}

unsigned int PartitionGraph::GetSize(void) {
  return GetInfo().size();
}

