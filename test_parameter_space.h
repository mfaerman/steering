#ifndef __TEST_PAR_SPACE_H__
#define __TEST_PAR_SPACE_H__

#include "parameter_space.h"
#include "configuration.h"

class Test_Parameter_Space : public Parameter_Space<double> {
 public:
  
  Test_Parameter_Space(DIMENSIONS_TYPE dim) :
    Parameter_Space<double>(dim) {};  
  
  int assign_cost_function();

  void print(INDICES_TYPE) const;

  int print_all(void) const;
   
};


class Test_Configuration : public Parameter_Space<configuration> {
public:
  
  Test_Configuration(DIMENSIONS_TYPE dim) :
    Parameter_Space<configuration>(dim) {};  
};

template<class CONTENT>
void print_test(Multi_Dim_Matrix<CONTENT>&, INDICES_TYPE);

template<class CONTENT>
void set_test(Multi_Dim_Matrix<CONTENT>&, INDICES_TYPE);

template<class CONTENT>
void set_test22(Multi_Dim_Matrix<CONTENT>&, INDICES_TYPE);

template<class CONTENT>
void set_all(Multi_Dim_Matrix<CONTENT>&, INDICES_TYPE);

template<class CONTENT>
void set(Multi_Dim_Matrix<CONTENT>&, 
	 INDICES_TYPE, 
	 CONTENT (*f)(INDICES_TYPE));

double f_test(INDICES_TYPE indices);



template<class CONTENT>
void print_test(Multi_Dim_Matrix<CONTENT>& pv, INDICES_TYPE indices) {

  cout << "(";

  for(int i = 0; i < pv.num_dim(); i++)
    cout << indices[i] << " ";

  cout << ")" << " = " << pv(indices) << endl;
 
};

template<class CONTENT>
void set_test(Multi_Dim_Matrix<CONTENT>& pv, INDICES_TYPE indices) {

  double acc = 0.0;

  for(int i = 0; i < pv.num_dim(); i++)
    acc += indices[i];
  
  pv(indices) = (acc*10+0.7);
 
};

template<class CONTENT>
void set_test22(Multi_Dim_Matrix<CONTENT>& pv, INDICES_TYPE indices) {

  double acc = 0.0;

  for(int i = 0; i < pv.num_dim(); i++)
    acc += indices[i];
  
  pv(indices) = (acc*10+0.22);
 
};


template<class CONTENT>
void set_all(Multi_Dim_Matrix<CONTENT>& pv, 
	     CONTENT (*f)(INDICES_TYPE)) {
  pv.traverse(0, f);
};


template<class CONTENT>
void set(Multi_Dim_Matrix<CONTENT>& pv, 
	 INDICES_TYPE indices, 
	 CONTENT (*f)(INDICES_TYPE)) {
  pv(indices) = f(indices);
};


#endif








