// $Id: tree_ptasks.h,v 1.4 2002/09/19 03:02:11 mfaerman Exp $

#ifndef __TREE_PTASKS_H__
#define __TREE_PTASKS_H__

#include "tasks.h"
#include "tree.h"
#include "parameters.h"
//#include "priority_queue.h"
typedef Handle<Tree<HBaseTask> > HTBPTask;

class TPTask : public Tree<HBaseTask> {
  typedef Handle<TPTask> HTPTask;

private:
  int _level;
  float _fraction;
  unsigned int _finished_children;
  unsigned int _dropped_children;
  unsigned int _num_trials;
  
  //  void init_members(void);
 
public:
  // constructors
  TPTask(void);
  TPTask(const HBaseTask &);
  TPTask(const HTPTask &, const HBaseTask &);

  // copy constructor
  TPTask(const TPTask&);

  // copy assignment
  TPTask& operator = (const TPTask&);

  TPTask& operator <= (TPTask&);

  HTPTask construct(const HTPTask &, const HBaseTask &);

  // destructor
  ~TPTask(void);

  // interface
  HTBPTask & Insert(const HBaseTask &);

  void Print(void) const;

  void Set_Fraction(const float);

  void Set_Level(const int);
  int Get_Level(void) const;

  float Get_Relative_Fraction(void) const;
  float Get_Absolute_Fraction(void) const;

  HBaseTask & GetTask(void) const;

  bool All_Done(void);

  vector<HTPTask> * TPTask::previous_results(void) const;

  void TPTask::Set_previous_results(vector<HTPTask> &); 

  // void TPTask::update(vector<HTPTask> &);

  void Complete(void);
  void Drop(void);

  void RemoveSubTree(void);
  void Remove_Node(void);
  void Garbage_Collect(void);

  unsigned int GetTrials(void) const;
  void NewTrial(void);
  void ResetTrials(void);

};

typedef Handle<TPTask> HTPTask;

#endif













