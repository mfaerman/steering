bool AvgSimulation::Meet_Steering_Criteria(TTask * node_task) {

  Task * pt_task = node_task->pt_content();

  if (node_task->Get_Feedback() < Get_Average_Feedback() + Tolerance()  &&
      node_task->Get_Level() < _setup.Get_Max_Levels()) {
    cout << "Steer Time! ---> ";
    Print_Info(pt_task);
    return true;
  }
  else
    return false;
}


  if (node_task->All_Done() && 
      node_task->Get_Level() < _setup.Get_Max_Levels()) {
    cout << "All Done! ---> ";
    Print_Info(pt_task);
    return true;
  }
  else 
    return false;
}
