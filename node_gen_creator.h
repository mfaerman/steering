#ifndef __NODE_GEN_CREATOR_H__
#define __NODE_GEN_CREATOR_H__

#include "creator.h"
#include "parameters.h"
#include "node_generator.h"

class NodeGenCreator : public Creator<Generator_Group> {
private:
  static Parameters * _pt_setup;
  HTPTask _current_node;
  Generator_Group * CreateProduct(void);

public:
  static void Set_Configuration(Parameters *);
  NodeGenCreator(HTPTask &);
};

#endif
