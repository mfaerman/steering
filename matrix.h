#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <vector>
#include <functional>
using namespace std;


//----------------------------------------------------------------

typedef vector<size_t> DIMENSIONS_TYPE;
typedef vector<size_t> INDICES_TYPE;

template <class CONTENT> class Multi_Dim_Matrix {
  vector<CONTENT>*v;
  DIMENSIONS_TYPE dim;    // vector of matrix dimensions

public:

  // no default constructor
  // no copy constructor, no assignment: rely on default definitions

  unsigned long int size() const {
    unsigned long int s = 1;

    for (size_t i = 0; i < dim.size(); i++)
      s *= dim[i];
    return (s);
  }

  size_t length(const size_t i) const { return dim[i]; }

  size_t num_dim() const {return dim.size();}
  
  // Fortran-style subscripts

  // CONTENT& operator() (vector<size_t> indices); 
  // CONTENT operator() (vector<size_t> indices) const;

  Multi_Dim_Matrix& operator *= (CONTENT);

  vector<CONTENT>& matrix() { return *v; }

  vector<unsigned long int>& cum_prod(void);

  void traverse(unsigned int dimension, 
		void (*f)(Multi_Dim_Matrix<CONTENT>&, INDICES_TYPE));

  Multi_Dim_Matrix(DIMENSIONS_TYPE dimensions);

  CONTENT& operator() (INDICES_TYPE indices);

  void set_fnc(INDICES_TYPE indices, CONTENT (*f)(INDICES_TYPE));

  void set_all_fnc(CONTENT (*f)(INDICES_TYPE));
  

  //  virtual int visit(INDICES_TYPE indices);

};

//----------------------------------------------------------------

template<class CONTENT>
Multi_Dim_Matrix<CONTENT>::Multi_Dim_Matrix(DIMENSIONS_TYPE dimensions) 
{

    dim = dimensions;
    Multi_Dim_Matrix t = *this;

    v = new vector<CONTENT>(t.size());
}

//  catch(std::bad_alloc) {
//    cerr << "Memory Allocation Exception in Multi_Dim_Matrix for 
//             container length  = " << t.size() << " fields! \n";
//  }



//----------------------------------------------------------------


template<class CONTENT>
inline CONTENT& Multi_Dim_Matrix<CONTENT>::operator() (INDICES_TYPE indices)
{
    unsigned long int stride = 1;
    unsigned long int ind = 0;
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
Multi_Dim_Matrix<CONTENT>& Multi_Dim_Matrix<CONTENT>::operator *= (CONTENT c)
{
  (*v) *= c;
  return *this;
}

//----------------------------------------------------------------

template<class CONTENT>
vector<unsigned long int>& Multi_Dim_Matrix<CONTENT>::cum_prod(void) {
 //Converts from flat vector index to matrix subscript format
 
  vector<unsigned long int>::reverse_iterator d, rpd; 
  vector<unsigned long int> product(dim.size());
  unsigned long int cum_p;

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
void Multi_Dim_Matrix<CONTENT>::
traverse(unsigned int d, 
	 void (*visit)(Multi_Dim_Matrix<CONTENT>&, INDICES_TYPE)) {

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
void Multi_Dim_Matrix<CONTENT>::
set_fnc(INDICES_TYPE indices, 
    CONTENT (*f)(INDICES_TYPE)) {

  Multi_Dim_Matrix t = *this;
  
  t(indices) = f(indices);

}

//----------------------------------------------------------------

//  template<class CONTENT>
//  void Multi_Dim_Matrix<CONTENT>::
//  set_all_fnc(CONTENT (*f)(INDICES_TYPE)) {

//     traverse(0, bind2nd(&Multi_Dim_Matrix<double>::set_fnc, f));

//  }




//----------------------------------------------------------------

//  template<class CONTENT>
//  int Multi_Dim_Matrix<CONTENT>::visit(INDICES_TYPE indices) {
//    return 0;
//  }


//----------------------------------------------------------------

//  template<class CONTENT, class Fct> class It_Mat : public Multi_Dim_Matrix {

//  public:
  

//  }


#endif







