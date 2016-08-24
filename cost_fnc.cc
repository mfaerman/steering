// $Id: cost_fnc.cc,v 1.9 2003/04/06 05:44:29 mfaerman Exp $

#include "cost_fnc.h"

void set_energy(Multi_Dim_Matrix<configuration>& pm, 
		INDICES_TYPE indices, 
		double sigma) {

  float sum  = 0.0;
  float prod = 1.0;

  for(int i = 0; i < pm.num_dim(); i++)
    sum += sigma*pow(indices[i],2);
   
  sum = sum/4000;

  for(int i = 0; i < pm.num_dim(); i++)
    prod *= cos(indices[i] / sqrt(i));

  pm(indices).cost = (sum + (1 - prod));

};

FEEDBACK_TYPE Fnc_Griewank(float sigma, 
			   CONFIGURATION_TYPE & configuration) {

  float sum  = 0.0;
  float prod = 1.0;

  for(int i = 0; i < configuration.size(); i++)
    sum += sigma*pow(configuration[i],2);
   
  sum = sum/4000;

  for(int i = 0; i < configuration.size(); i++) 
    prod *= cos(configuration[i] / sqrt(i+1));

  return(sum + (1 - prod));
}


FEEDBACK_TYPE Fnc_Power (float power, 
			 CONFIGURATION_TYPE & configuration) {

  float sum  = 0.0;
  
  for(int i = 0; i < configuration.size(); i++)
    sum += pow(configuration[i],power);
   
  return(sum);
}





FEEDBACK_TYPE Fnc_Rastrigin (float dummy, 
			     CONFIGURATION_TYPE & configuration) {

  double x1(configuration[0]);
  double x2(configuration[1]);

  return pow(x1,2) + pow(x2,2) - cos(18*x1) - cos(18*x2) + 2.0;
}



FEEDBACK_TYPE Fnc_Rosenbrock (float dummy, 
			      CONFIGURATION_TYPE & configuration) {

  double x1(configuration[0]);
  double x2(configuration[1]);

  double f1(10 * (x2 - pow(x1,2)));
  double f2(1 - x1);

  return pow(f1,2) + pow(f2,2);
}



