#ifndef __GRID_H__
#define __GRID_H__

#include<vector>
#include<stdio.h>
using namespace std;

#include"simgrid.h"
#include"parameters.h"

typedef enum {PROCESSOR, LINK} RESOURCE_TYPE;

class Grid {

protected:
//    static Grid* _instance;
  vector<SG_Resource> * _processors;
  unsigned int _next_processor_cyclic;
  double _previous_time;
  void Init_Grid(const RESOURCE_TYPE, const unsigned int, const double);
  unsigned int Total(void);

public:
  Grid(void);
  Grid(unsigned int);
  Grid(const RESOURCE_TYPE, const unsigned int); 
  Grid(const RESOURCE_TYPE, const unsigned int, const double);

  Grid(const Grid &); // copy constructor

  ~Grid(void);        // destructor

//    static Grid * Instance(const RESOURCE_TYPE, const unsigned int, 
//  			 const double); // singleton access point
//    static Grid * Instance(void);

  Grid & operator = (const Grid &);

  virtual SG_Resource & Get_Processor(const unsigned int) const;

  virtual SG_Resource & Choose_Processor(void);

  double GetCurrentTime(void);
  double GetElapsedTime(void);
  void UpdateElapsedTime(void);

  bool Empty(void);
  void Finalization(void);

}; 




#endif
