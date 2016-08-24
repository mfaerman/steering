#ifndef __HELPER_H__
#define __HELPER_H__

#include"tasks.h"
//#include"tree_tasks.h"
#include"tree_ptasks.h"
#include"random_generator.h"
#include"random_gen_creator.h"
#include"partition.h"
#include"partition_local_search.h"
#include"search.h"
#include"simgrid.h"
#include"parameters.h"
#include<math.h>


//  bool operator <  (PartitionFirstImproveLocalSearch* ls1, 
//  		  PartitionFirstImproveLocalSearch* ls2) {
//    return (ls1->GetSolution().GetEvaluation() < 
//  	  ls2->GetSolution().GetEvaluation());
//  }

class SmallerEvaluation {
public:
  bool operator()(PartitionFirstImproveLocalSearch* ls1, 
		  PartitionFirstImproveLocalSearch* ls2) { 

    return (ls1->GetSolution().GetEvaluation() < 
	    ls2->GetSolution().GetEvaluation());
  }
};

class TTaskCompare {
private:
  virtual bool compare(BaseTask &, BaseTask &) const = 0;
  //  virtual bool compare(TPTask &, TPTask &) const = 0;
public:
//    bool operator()(HTTask tt1, HTTask tt2) { 
//      return compare(-(*tt1),-(*tt2));
//    }  
  bool operator()(BaseTask & bt1, BaseTask & bt2) {
    return compare(bt1, bt2);
}
  bool operator()(HTPTask tpt1, HTPTask tpt2) { 
    return compare(*-(*tpt1),*-(*tpt2));
  }
  bool operator()(SG_Task sg1, SG_Task sg2) { 
    return compare(*(-(**((HTPTask *)SG_getTaskMetadata(sg1)))),
 		   *(-(**((HTPTask *)SG_getTaskMetadata(sg2)))));
  }
};


class Smaller_Feedback : public TTaskCompare {
private:
  bool compare(BaseTask & t1, BaseTask & t2) const {
    if (t1.Get_Feedback() == t2.Get_Feedback())
      return(t1.name() > t2.name());
     else
       return(t1.Get_Feedback() < t2.Get_Feedback()); 
  }
};

class Larger_Feedback : public TTaskCompare {
private:
  bool compare(BaseTask & t1, BaseTask & t2) const {
    if (t1.Get_Feedback() == t2.Get_Feedback())
      return(t1.name() < t2.name());
    else 
      return(t1.Get_Feedback() > t2.Get_Feedback());
  }
};

class Smaller_Priority : public TTaskCompare {
private:
  bool compare(BaseTask & t1, BaseTask & t2) const {
    if (t1.Get_Priority() == t2.Get_Priority())
       return(t1.name() < t2.name());
     else
       return(t1.Get_Priority() < t2.Get_Priority());   
  }
};

class Larger_Priority : public TTaskCompare {
private:
  bool compare(BaseTask & t1, BaseTask & t2) const {
    //    return ! Smaller_Priority::operator()(t1,t2);
    if (t1.Get_Priority() == t2.Get_Priority())
      return(t1.name() > t2.name());
    else
      return(t1.Get_Priority() > t2.Get_Priority());   
  }
};


char * Name_Policy(char *, int);
void ScheduleOnResource(SG_Resource, HTPTask &);

void Print_Info(HBaseTask &);
void Print_Info(HTPTask &);
void Format_Output_Stream(void);

float Workload_Policy(void);
double Start_Priority_Policy(void);
unsigned int Neighborhood_Policy(int);
double Priority_Policy(double);

CONFIGURATION_TYPE Scatter(int, float, int);

unsigned int Total_Tasks_to_Execute(Parameters &);

CONFIGURATION_TYPE Spherical_Neighborhood(CONFIGURATION_TYPE *, 
					  float);

SG_outputmode_t GetVerbosity(void);

#endif


