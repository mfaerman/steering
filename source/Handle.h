/*
 * $Id: Handle.h,v 1.1 2002/09/21 18:09:31 mfaerman Exp $
 * 
 * handle - An intrusive reference-counting smart pointer.
 *
 */

#ifndef __HANDLE_H
#define __HANDLE_H

/*
  If you want to refer to a class using refence counting smart pointers,
  you'll need to make it a descendant of the following Handleable class.
*/
class Handleable
{
  public:
    Handleable() { handleable_count = 0; };
    ~Handleable() {};

  public:                                                         
    int incrementHandleableCount() { return ++handleable_count;};   
    int decrementHandleableCount() { return --handleable_count;};   
    int getHandleableCount()       { return handleable_count; };    
  private:                                                        
    int handleable_count;
};


template <class X> class Handle
{
  public:
    typedef X element_type;

    // The following constructor handles the case:
    //     foo *ptr = new foo;
    //     Handle<foo> h(ptr);
    explicit Handle(X* p = 0) // allocate a new counter
    {
      acquire(p);
    }
 
    ~Handle() 
    { 
      release();
    }

    // The following constructor handles the case:
    //    Handle<foo> h1(ptr);
    //    Handle<foo> h2(h1);
    Handle(const Handle& r) throw() 
    {   
      acquire(r.get()); 
    }

    // The following assignment operator handles the case:
    //     Handle<foo> h1(p1), h2(p2);
    //     h1 = h2;
    Handle& operator=(const Handle& r)
    {
      if (this != &r) 
      {
        release();
        acquire(r.get());
      }
      return *this;
    }

    Handle& operator=(const X *p)
    {
      if (ptr != p)
      {
        release();
        acquire((X*) p);
      }
      return *this;
    }

    ////////////////////////////////////////////////////
    //  Dereference operators.
    X& operator*()  const throw()   {return *ptr;}
    X* operator->() const throw()   {return ptr;}
    X* get()        const throw()   {return ptr;}

    /////////////////////////////////////////////////////////
    // Test operators
    bool operator!() const throw()  {return (ptr == NULL);};
    bool operator==(const X *rhs) const throw()   
    { 
      return (ptr == rhs);
    };

    bool operator==(const Handle &rhs) const throw()   
    { 
      return (ptr == rhs.get());
    };

    bool operator!=(const X *rhs) const throw()   
    { 
      return (ptr != rhs);
    };

    bool operator!=(const Handle &rhs) const throw()   
    { 
      return (ptr != rhs.get());
    };
    
    operator bool() const throw() { return (ptr != NULL); };

  private: 
    void acquire(X *p) throw()
    { 
      // increment the count
      ptr = p;
      if (ptr)
        ptr->incrementHandleableCount();
    }

    void release()
    { 
      if (ptr) 
      {
        // decrement the count, delete if it is 0
        if (ptr->decrementHandleableCount() == 0) 
        {
          delete ptr;
          ptr = NULL;
        }
      }
    }
  
  private:
    X*          ptr;

};

#endif // __HANDLE_H

