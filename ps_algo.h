#ifndef __PS_ALGO_H__
#define __PS_ALGO_H__

#include "matrix.h"


//  emplate<class CONTENT>
//  void Multi_Dim_Matrix<CONTENT>::
//  traverse(unsigned int d, 
//  	 void (*visit)(Multi_Dim_Matrix<CONTENT>&, INDICES_TYPE)) {

//    static INDICES_TYPE indices(dim.size(), 0);

//    unsigned int next;

//    next = d + 1;

//    for (size_t i = 0; i < dim[d]; i++) {  
//      indices[d] = i;
//      if (next < dim.size()) {
//        traverse(next, visit);
//      }
//      else 
//        visit(*this, indices); 
//    }
//  }

//  // for_each.  Apply a function to every element of a range.
//  template <class _InputIter, class _Function>
//  _Function for_each(_InputIter __first, _InputIter __last, _Function __f) {
//    for ( ; __first != __last; ++__first)
//      __f(*__first);
//    return __f;
//  }


template <class CONTENT, class Function>
Function sbm_for_each(unsigned int d, Multi_Dim_Matrix<CONTENT>& pm, 
		      INDICES_TYPE first, INDICES_TYPE last, 
		      Function visit) {

  static INDICES_TYPE indices(first);

  unsigned int next;

  next = d + 1;

  for (size_t i = first[d]; i <= last[d]; i++) {  
    indices[d] = i;
    if (next < last.size()) {
      sbm_for_each(next, pm, first, last, visit);
    }
    else 
      visit(pm, indices); 
  }
  return visit;
}


template <class CONTENT, class Function>
Function sbm_for_each_try(unsigned int d, Multi_Dim_Matrix<CONTENT>& pm, 
		      INDICES_TYPE first, INDICES_TYPE last, 
		      Function visit) {

  static INDICES_TYPE indices(first);

  unsigned int next;

  next = d + 1;

  for (size_t i = first[d]; i <= last[d]; i++) {  
    indices[d] = i;
    if (next < last.size()) {
      sbm_for_each(next, pm, first, last, visit);
    }
    else 
      visit(pm(indices)); 
  }
  return visit;
}



#endif  

