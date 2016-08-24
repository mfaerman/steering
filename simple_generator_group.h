#ifndef __SIMPLEGENERATORGROUP__

class SimpleGeneratorGroup : public Generator_Group {
  SimpleGeneratorGroup(TTask *);
  void  Schedule_New_Tasks(float);
};
#endif
