#include "node_gen_creator.h"

// (required) definition of static members
Parameters *  NodeGenCreator::_pt_setup = NULL;

void NodeGenCreator::Set_Configuration(Parameters * pt_setup) {
  _pt_setup = pt_setup;
}

NodeGenCreator::NodeGenCreator(HTPTask & ttask) : 
  _current_node(ttask),  
  Creator<Generator_Group>()  
{}


Generator_Group * NodeGenCreator::CreateProduct(void) {
  switch (_pt_setup->Get_Policy()) {
    case INITIAL      : return new InitialGeneratorGroup(_current_node);
    case SIMPLE       : return new SimpleGeneratorGroup(_current_node);
    case PRIORITY_TREE: return new PriorityTreeGeneratorGroup(_current_node);
    default:
      cout << "Policy Type Error!" << endl;
      exit (-1);
  }
}

