#ifndef __CREATOR_H__
#define __CREATOR_H__

#include <stdlib.h>

//abstract virtual constructor

template <class Product>
class Creator {
private:
  Product * _product;
protected:
  Creator(void) : _product(NULL) {}
  virtual Product * CreateProduct(void) = 0;
public:
  Product * GetProduct(void);
};

// lazy initialization
template <class Product>
Product * Creator<Product>::GetProduct(void) {
  if (_product == NULL)
    _product = CreateProduct();
  return _product;
}


//  template <class TheProduct>
//  class StandardCreator : public Creator {
//    virtual Product * CreateProduct();
//  }

//  template <class TheProduct>
//  Product * StandardCreator<TheProduct>::CreateProduct() {
//    return new TheProduct;

#endif


