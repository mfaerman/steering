#ifndef __COST_FNC_H__
#define __COST_FNC_H__

#include <math.h>
#include "configuration.h"
#include "matrix.h"
#include "tasks.h"

void set_energy(Multi_Dim_Matrix<configuration>&, INDICES_TYPE, double);
FEEDBACK_TYPE Fnc_Griewank(float, CONFIGURATION_TYPE&);
FEEDBACK_TYPE Fnc_Power(float, CONFIGURATION_TYPE&);
FEEDBACK_TYPE Fnc_Rastrigin (float, CONFIGURATION_TYPE &);
FEEDBACK_TYPE Fnc_Rosenbrock (float, CONFIGURATION_TYPE &);


class set_Griewank {

  float sigma;

 public:
  set_Griewank(float s) {sigma = s;}
  
  void operator() (Multi_Dim_Matrix<configuration>& pm, 
		   INDICES_TYPE indices) const {

    float sum  = 0.0;
    float prod = 1.0;
    float sr;

    for(int i = 0; i < pm.num_dim(); i++)
      sum += sigma*pow(indices[i],2);
   
    sum = sum/4000;

    for(int i = 0; i < pm.num_dim(); i++) 
      prod *= cos(indices[i] / sqrt(i+1));

    pm(indices).cost = (sum + (1 - prod));
  }
};


  
#endif
