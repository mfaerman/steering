#ifndef __SEARCH_H__
#define __SEARCH_H__

#include"shuffle.h"
#include<vector>


template <class INDIVIDUAL> class LocalSearch {
protected:
  INDIVIDUAL _current_solution;
  unsigned int _evaluations;
  unsigned int _steps;
public:
  virtual INDIVIDUAL & GetOptimum(void) {cout << "Error: Abstract call\n";
  exit (-1);}
  unsigned int GetNumNeighbors(void);
  unsigned int GetRemainingNeighbors(void);
  INDIVIDUAL * GetNewNeighbor(void);
  unsigned int GetEvaluations(void) const;
  unsigned int GetSteps(void) const;
  INDIVIDUAL & GetSolution(void);

  LocalSearch<INDIVIDUAL> & operator= (const LocalSearch<INDIVIDUAL> &);

  //constructor
  LocalSearch(INDIVIDUAL & s) : 
    _current_solution(s), 
    _evaluations(0) ,
    _steps(0)
  {}

  LocalSearch(LocalSearch & other) : 
    _current_solution(other._current_solution), 
    _evaluations(other._evaluations),
    _steps(other._steps)
  {}


  virtual ~LocalSearch(void) {}
};

template  <class INDIVIDUAL> class SteepestDescent : 
  public LocalSearch<INDIVIDUAL> {
private:  
public:
  SteepestDescent(void) {};
  virtual INDIVIDUAL & GetOptimum(void);
};

template <class INDIVIDUAL> class FirstImprove 
  : public LocalSearch<INDIVIDUAL> {
private:
public:
  FirstImprove(INDIVIDUAL & s) : LocalSearch<INDIVIDUAL>(s) {}
  FirstImprove(FirstImprove & other) : LocalSearch<INDIVIDUAL>(other) {}
  virtual ~FirstImprove(void) {}
  virtual INDIVIDUAL & GetOptimum(void);
//    INDIVIDUAL & Sniff(unsigned int);
  void Sniff(unsigned int);
};

vector<bool> & Mutation(vector<bool> &);
vector<bool> Crossover(vector<bool> &, vector<bool> &);
bool MutateBit(void);

template <class INDIVIDUAL>
INDIVIDUAL & FirstImprove<INDIVIDUAL>::GetOptimum(void) {
  // less than means better, greater than means worse -> 
  // we assume minimum as optimum

  INDIVIDUAL * ptr_neighbor;
  unsigned int count(0);

  //  while (count < GetNumNeighbors()) {
  while (count < GetNumNeighbors()) {
    ptr_neighbor = GetNewNeighbor();

    if (_current_solution > (*ptr_neighbor)) {
      _current_solution = *ptr_neighbor;
      count = 0;
    }
    else
      count ++;

    _evaluations ++;
    delete(ptr_neighbor);
  }

  cout << "\n\nLocal Search Evaluations = " << _evaluations << endl;
  return _current_solution;
}

//  //  template <class INDIVIDUAL>
//  //  INDIVIDUAL & FirstImprove<INDIVIDUAL>::Sniff(unsigned int length) {

template <class INDIVIDUAL>
void FirstImprove<INDIVIDUAL>::Sniff(unsigned int length) {
  // less than means better, greater than means worse -> 
  // we assume minimum as optimum

  INDIVIDUAL * ptr_new_neighbor;
  unsigned int count(0);
  unsigned int moves(0);

  while (GetRemainingNeighbors() > 0 && 
	 moves < length) {

    ptr_new_neighbor = GetNewNeighbor();

    if (_current_solution > (*ptr_new_neighbor)) {
      _current_solution = *ptr_new_neighbor;
      moves ++;
      count = 0;
    }
    else
      count ++;

    _evaluations ++;
    delete(ptr_new_neighbor);
  }
  
  _steps += moves;

//    cout << "\n\nEVALUATION = " 
//         << GetSolution().GetEvaluation() 
//         << ", Local Search Steps = "
//         << GetSteps()
//         << ", Local Search Evaluations = " 
//         << GetEvaluations() 
//         << endl;
      
  //  return _current_solution;
}

template <class INDIVIDUAL>
INDIVIDUAL & SteepestDescent<INDIVIDUAL>::GetOptimum(void) {
  // less than means better, greater than means worse -> 
  // we assume minimum as optimum
  
  INDIVIDUAL * ptr_neighbor;
  bool found_better;

  do {
    found_better = false;
    for (unsigned int count = 0; count < GetNumNeighbors(); count++) {
      ptr_neighbor = GetNewNeighbor();
      if (_current_solution > ptr_neighbor) {
	_current_solution = ptr_neighbor;
	found_better = true;
      }
    }
  }
  while (found_better);

  return &(_current_solution);
}

template <class INDIVIDUAL>
unsigned int LocalSearch<INDIVIDUAL>::GetNumNeighbors(void) {
  return _current_solution.GetNumNeighbors();
}

template <class INDIVIDUAL>
unsigned int LocalSearch<INDIVIDUAL>::GetRemainingNeighbors(void) {
  return _current_solution.GetRemainingNeighbors();
}

template <class INDIVIDUAL>
INDIVIDUAL * LocalSearch<INDIVIDUAL>::GetNewNeighbor(void) {
  return _current_solution.GetNextNeighbor();
}

//  template <class INDIVIDUAL>
//  INDIVIDUAL LocalSearch<INDIVIDUAL>::GetCurrentNeighbors(void) {
//    return _current_solution.GetCurrentNeighbor();
//  }

template <class INDIVIDUAL>
INDIVIDUAL & LocalSearch<INDIVIDUAL>::GetSolution(void) {
  return _current_solution;
}


template <class INDIVIDUAL>
unsigned int LocalSearch<INDIVIDUAL>::GetEvaluations(void) const {
  return _evaluations;
}


template <class INDIVIDUAL>
unsigned int LocalSearch<INDIVIDUAL>::GetSteps(void) const {
  return _steps;
}

template <class INDIVIDUAL>
LocalSearch<INDIVIDUAL> & LocalSearch<INDIVIDUAL>::operator= 
(const LocalSearch<INDIVIDUAL> & other) {
  if (this != &other) {
    _current_solution = other._current_solution;
    _evaluations = other._evaluations;
    _steps = other._steps;   
  }    
  return *this;
}

template <class INDIVIDUAL>
vector<bool> Crossover(LocalSearch<INDIVIDUAL> * mother, 
		       LocalSearch<INDIVIDUAL> * father) {
			
  return(Crossover(mother->GetSolution().GetConfiguration(), 
		   father->GetSolution().GetConfiguration()));
}


#endif












