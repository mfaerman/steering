// $Id: tree_tasks.h,v 1.19 2002/09/19 03:02:11 mfaerman Exp $

#ifndef __TREE_TASKS_H__
#define __TREE_TASKS_H__

#include "tasks.h"
#include "tree.h"
#include "parameters.h"
//#include "priority_queue.h"

typedef Handle<Tree<BaseTask> > HTBTask;

class TTask : public Tree<BaseTask> {
  typedef Handle<TTask> HTTask;
  //  typedef Handle<vector<HTTask> > HVHTTask;

private:
  int _level;
  float _fraction;
  unsigned int _finished_children;
  unsigned int _dropped_children;
  unsigned int _num_trials;
  vector<HTTask> * _previous_results;
  BaseTask * _pt_task;
  
  //  void init_members(void);
 
public:

//constructors
TTask::TTask(void) : 
  Tree<BaseTask>(), 
  _level(0),
  _fraction(0),
  _finished_children(0),
  _dropped_children(0),
  _num_trials(0),
  _previous_results(NULL),
  _pt_task(NULL)
{}



  TTask(BaseTask);
  TTask(HTTask, BaseTask);
  TTask(HTTask, BaseTask *);

  // copy constructor
  TTask(const TTask&);

  // copy assignment
  TTask& operator = (const TTask&);

  TTask& operator <= (const TTask&);

  HTTask construct(HTTask, BaseTask);
  HTTask construct(HTTask, BaseTask *);

  // destructor
  ~TTask(void);

  // interface
  HTBTask Insert(const BaseTask);
  HTTask TTask::PInsert(BaseTask *);

  void Print(void) const;

  void Set_Fraction(const float);

  void Set_Level(const int);
  int Get_Level(void) const;

  float Get_Relative_Fraction(void) const;
  float Get_Absolute_Fraction(void) const;

  BaseTask * GetTask(void) const;

  bool All_Done(void);

  vector<HTTask> * TTask::previous_results(void) const;

  void TTask::Set_previous_results(vector<HTTask> &); 

  void TTask::update(vector<HTTask> &);

  void Complete(void);
  void Drop(void);

  void RemoveSubTree(void);
  void Remove_Node(void);
  void Garbage_Collect(void);

  unsigned int GetTrials(void) const;
  void NewTrial(void);
  void ResetTrials(void);

};

typedef Handle<TTask> HTTask;

#endif













