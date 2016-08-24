#include "matrix.h"

//  template<class CONTENT>
//  Multi_Dim_Matrix<CONTENT>::Multi_Dim_Matrix(DIMENSIONS_TYPE dimensions) {

//      dim = dimensions;
//      Multi_Dim_Matrix t = *this;

//      v = new valarray<CONTENT>(t.size());
//  }


//----------------------------------------------------------------
template<class CONTENT>
Multi_Dim_Matrix<CONTENT>& Multi_Dim_Matrix<CONTENT>::operator *= (CONTENT c)
{
  (*v) *= c;
  return *this;
}

//----------------------------------------------------------------

template<class CONTENT>
vector<size_t>& Multi_Dim_Matrix<CONTENT>::cum_prod(void) {
 //Converts from flat array index to matrix subscript format
 
  vector<size_t>::reverse_iterator d, rpd; 
  vector<size_t> product(dim.size());
  size_t cum_p;

  d     = dim.rbegin();
  rpd   = product.rbegin();
  cum_p = 1;
  
  while (d != dim_list.rend()) {
    (*rpd) = cum_p;
    cum_p *= (*d);
    
    d++;
    rpd++;
  }
  return(product);
}

//----------------------------------------------------------------

//  template<class CONTENT>
//  inline CONTENT& Multi_Dim_Matrix<CONTENT>::operator[] (INDICES_TYPE indices)
//  {
//      size_t stride = 1;
//      size_t ind = 0;
//      INDICES_TYPE::reverse_iterator i;
//      DIMENSIONS_TYPE::reverse_iterator d;

//      // Still need to check if indices.size() == dim.size() and 
//      // issue exception otherwise 
     
//      i = indices.rbegin();
//      d = dim.rbegin();

//      while(i != indices.rend()){
//        ind    += (*i)*stride;
//        stride *= (*d);
//        i++;
//        d++;
//      }
//      return ((*v)[ind]);
//  }
































