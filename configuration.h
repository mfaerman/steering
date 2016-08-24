#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <iostream>
using namespace std;

#include "matrix.h"

typedef double PRIORITY_TYPE;
typedef float  COST_TYPE;

class configuration {

 public:
  PRIORITY_TYPE priority;
  COST_TYPE cost;

  void print(void) {
    cout << "Priority = " << priority << "Cost = " << cost;
  }
  
};
  
void print_configuration(Multi_Dim_Matrix<configuration>& pm, 
			 INDICES_TYPE indices);

#endif  

