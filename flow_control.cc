#include <iostream>
using namespace std;

#include "flow_control.h"

FlowControl::FlowControl(unsigned int burst_limit) {
  _burst_limit = burst_limit;
  _num_tokens = _burst_limit;
  _previous_update = 0;
}

Parameters * FlowControl::_pt_setup = NULL;
Grid * FlowControl::_pt_grid = NULL;

void FlowControl::Set_Configuration(Parameters * pt_setup) {
  _pt_setup = pt_setup;
}

void FlowControl::Set_Grid(Grid * pt_grid) {
  _pt_grid = pt_grid;
}

double FlowControl::GetIntervalSincePreviousUpdate(void) {
   return _pt_grid->GetCurrentTime() -  _previous_update;
}

bool FlowControl::Cycle_Complete(void) {
  return GetIntervalSincePreviousUpdate() >= _pt_setup->Get_Token_Period();
}
  
unsigned int FlowControl::GetNumberCycles(void) {
  return static_cast<unsigned int>
    (floor(GetIntervalSincePreviousUpdate()/_pt_setup->Get_Token_Period()));
}

void FlowControl::Update(void) {
  //  if (Cycle_Complete()) {

  if (_num_tokens < _burst_limit)    
    for (unsigned int i = 0; i < GetNumberCycles(); i++) {
	_num_tokens ++;
	_previous_update =  _pt_grid->GetCurrentTime();
    }
}

bool FlowControl::GetToken(void) {
  if(!Parameters::Instance()->GetFlowControlEnabled())
    return true;
  else {
    if (_num_tokens > 0) {
      cout << " Tokens = " << _num_tokens << endl;
      _num_tokens --;
      return true;
    }
  return false;
  }
}

//  unsigned int FlowControl::GetTokens(void) {
//    return static_cast<unsigned int>( 
//      floor(_pt_grid->GetElapsedTime() * 
//  	  _pt_setup->Get_Token_Rate()));
//  }
