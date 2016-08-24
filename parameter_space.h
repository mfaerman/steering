#ifndef __PAR_SPACE_H__
#define __PAR_SPACE_H__

#include <vector>
using namespace std;

#include "matrix.h"


template<class CONTENT, class INDEX> class Multi_Vec : public vector<CONTENT>  
{

public:

  Multi_Vec() : vector <CONTENT>() {};
  Multi_Vec(const int dim) : vector <CONTENT>(dim) {};
  Multi_Vec(const int dim, const int size) : 
    vector <CONTENT>((const int)dim*(const int)size) {};
 
  CONTENT& operator[] (INDEX id);
  const CONTENT& operator[] (INDEX id) const; 
 
};


class Configuration : public vector<int> {

public:
  
  explicit Configuration() : vector<int>() {};
  explicit Configuration (const int dimension) : vector<int>(dimension) {}; 

};


template<class CONTENT> class Parameter_Space : 
  public Multi_Dim_Matrix<CONTENT> {

public:

  //virtual int assign_cost_function() = 0;

   Parameter_Space(DIMENSIONS_TYPE dim) :  Multi_Dim_Matrix<CONTENT>(dim) {};
};  

#endif





































































































