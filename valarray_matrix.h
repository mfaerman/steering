#ifndef __VALARRAY_MATRIX_H__
#define __VALARRAY_MATRIX_H__

#include <vector>
#include <valarray>
#include <functional>
using namespace std;


//----------------------------------------------------------------

typedef vector<size_t> DIMENSIONS_TYPE;
typedef vector<size_t> INDICES_TYPE;

template <class CONTENT> class Multi_Dim_ValArray {
  valarray<CONTENT>*v;
  DIMENSIONS_TYPE dim;    // array of matrix dimensions

public:

  // no default constructor
  // no copy constructor, no assignment: rely on default definitions

  size_t size() const {
    size_t s = 1;

    for (size_t i = 0; i < dim.size(); i++)
      s *= dim[i];
    return (s);
  }

  size_t length(const size_t i) const { return dim[i]; }

  size_t num_dim() const {return dim.size();}
  
  // Fortran-style subscripts

  // CONTENT& operator() (vector<size_t> indices); 
  // CONTENT operator() (vector<size_t> indices) const;

  Multi_Dim_ValArray& operator *= (CONTENT);

  valarray<CONTENT>& array() { return *v; }

  vector<size_t>& cum_prod(void);

  void traverse(unsigned int dimension, 
		void (*f)(Multi_Dim_ValArray<CONTENT>&, INDICES_TYPE));

  Multi_Dim_ValArray(DIMENSIONS_TYPE dimensions);

  CONTENT& operator() (INDICES_TYPE indices);

  void set_fnc(INDICES_TYPE indices, CONTENT (*f)(INDICES_TYPE));

  void set_all_fnc(CONTENT (*f)(INDICES_TYPE));
  

  //  virtual int visit(INDICES_TYPE indices);

};

//----------------------------------------------------------------

template<class CONTENT>
Multi_Dim_ValArray<CONTENT>::Multi_Dim_ValArray(DIMENSIONS_TYPE dimensions) {

    dim = dimensions;
    Multi_Dim_ValArray t = *this;

    v = new valarray<CONTENT>(t.size());
}


//----------------------------------------------------------------


template<class CONTENT>
inline CONTENT& Multi_Dim_ValArray<CONTENT>::operator() (INDICES_TYPE indices)
{
    size_t stride = 1;
    size_t ind = 0;
    INDICES_TYPE::reverse_iterator i;
    DIMENSIONS_TYPE::reverse_iterator d;

    // Still need to check if indices.size() == dim.size() and 
    // issue exception otherwise 
     
    i = indices.rbegin();
    d = dim.rbegin();

    while(i != indices.rend()){
      ind    += (*i)*stride;
      stride *= (*d);
      i++;
      d++;
    }
    return ((*v)[ind]);
}


//----------------------------------------------------------------
template<class CONTENT>
Multi_Dim_ValArray<CONTENT>& Multi_Dim_ValArray<CONTENT>::operator *= (CONTENT c)
{
  (*v) *= c;
  return *this;
}

//----------------------------------------------------------------

template<class CONTENT>
vector<size_t>& Multi_Dim_ValArray<CONTENT>::cum_prod(void) {
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

template<class CONTENT>
void Multi_Dim_ValArray<CONTENT>::
traverse(unsigned int d, 
	 void (*visit)(Multi_Dim_ValArray<CONTENT>&, INDICES_TYPE)) {

  static INDICES_TYPE indices(dim.size(), 0);

  unsigned int next;

  next = d + 1;

  for (size_t i = 0; i < dim[d]; i++) {  
    indices[d] = i;
    if (next < dim.size()) {
      traverse(next, visit);
    }
    else 
      visit(*this, indices); 
  }
}

//----------------------------------------------------------------

template<class CONTENT>
void Multi_Dim_ValArray<CONTENT>::
set_fnc(INDICES_TYPE indices, 
    CONTENT (*f)(INDICES_TYPE)) {

  Multi_Dim_ValArray t = *this;
  
  t(indices) = f(indices);

}

//----------------------------------------------------------------

//  template<class CONTENT>
//  void Multi_Dim_ValArray<CONTENT>::
//  set_all_fnc(CONTENT (*f)(INDICES_TYPE)) {

//     traverse(0, bind2nd(&Multi_Dim_ValArray<double>::set_fnc, f));

//  }




//----------------------------------------------------------------

//  template<class CONTENT>
//  int Multi_Dim_ValArray<CONTENT>::visit(INDICES_TYPE indices) {
//    return 0;
//  }


//----------------------------------------------------------------

//  template<class CONTENT, class Fct> class It_Mat : public Multi_Dim_ValArray {

//  public:
  

//  }


#endif







