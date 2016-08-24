#ifndef __SLICE_ITER_H__
#define __SLICE_ITER_H__

#include <valarray>
using namespace std;

template<class T> class Slice_iter {
  vallarray<T> * v;
  slice s;
  size_t curr;    // index of current element

  T& ref(size_t i) const {return (*v)[s.start() + i*s.stride()]; }

 public:
  Slice_iter(valarray<T>*vv, slice ss) : v(vv), s(ss) { }

  Slice_iter end()
    {
      Slice_iter t = *this;
      t.curr = s.start()+s.size()*s.stride(); //last-plus-one element position
      return t;
    }

  Slice_iter& operator++() { curr++; return *this; }
  Slice_iter operator++(int) {Slice_iter t = *this; curr++; return t; }

  T& operator[] (size_t i) { return ref(curr=i); } // C style subscript
  T& operator() (size_t i) { return ref(curr=i); } // Fortran style subscript
  T& operator*() { return ref(curr); }             // current element

};


template<class T>bool operator== (const Slice_iter<T>&p, const Slice_iter<T>&q)
{
  return p.curr == q.curr 
    && p.s.stride == q.s.stride && p.start == q.s.start;
}

template<class T>bool operator!= (const Slice_iter<T>&p, const Slice_iter<T>&q)
{
  return !(p == q);
}

template<class T>bool operator< (const Slice_iter<T>&p, const Slice_iter<T>&q)
{
  return p.curr < q.curr 
    && p.s.stride == q.s.stride && p.start == q.s.start;
}
























#endif
