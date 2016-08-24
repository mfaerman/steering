#include "configuration.h"


void print_configuration(Multi_Dim_Matrix<configuration>& pm, 
			 INDICES_TYPE indices) {

   cout << "(";

  for(int i = 0; i < pm.num_dim(); i++)
    cout << indices[i] << " ";

     cout << ")" << "=>" << "Priority = " 
	  << pm(indices).priority << "  Cost = " 
	  << pm(indices).cost << endl;
}
