#include"search.h"
#include"partition.h" 


///////////////////////////MUTATION////////////////////////////////////////////
vector<bool> & Mutation(vector<bool> & individual) {

  unsigned int pairnode;

  for(unsigned int i = 0; i < individual.size(); i++) {
    if(MutateBit()) {
      // do a swap
       pairnode = ChooseSwapNode(i, individual);

       individual[i] = !individual[i];
       individual[pairnode] = !individual[pairnode];
    }
  }
  return individual;
}
///////////////////////////CROSSOVER///////////////////////////////////////////
vector<bool> Crossover(vector<bool> & mother, vector<bool> & father) {
  unsigned int trues(0), falses(0), index(0);
  int excess(0);
  bool smallside(false);

  vector<unsigned int> list_differing_nodes;
  vector<bool>offspring(mother.size());

  for(unsigned int i = 0; i < mother.size(); i++) {
    // same bit value
    if(mother[i] == father[i]) {
      offspring[i] = mother[i];
    
      if(offspring[i] == true)
	trues++;
      else
	falses++;
    }
    // different bit value
    else {
      list_differing_nodes.push_back(i);
    }
  }

  excess = trues - falses;
  if (excess > 0)
    smallside = false;
  else
    smallside = true;
  unsigned int abs_excess(abs(excess));

 int ones;
  //for debugging
  ones = 0;
  for (unsigned int i = 0; i < offspring.size(); i++) {
    if (offspring[i])
      ones++;
  }
  ////////////////

  if (!list_differing_nodes.empty()) {
    Shuffle shuffle(list_differing_nodes.size());
    
    for (unsigned int i = 0; i < (list_differing_nodes.size()/2); i++) {
      offspring[list_differing_nodes[shuffle.Draw()]] = true;
      offspring[list_differing_nodes[shuffle.Draw()]] = false;
    }
    
    //for debugging
    ones = 0;
    for (unsigned int i = 0; i < offspring.size(); i++) {
      if (offspring[i])
	ones++;
    }
    ////////////////
 
  // odd list_differing_nodes.size()
    if (shuffle.size() > 0) {
      offspring[list_differing_nodes[shuffle.Draw()]] = smallside;
      abs_excess--;
    }
    
    // rebalance
    shuffle.Reset();
    for (unsigned int i = 0; i < abs_excess/2; i++) {
      do {
	index = shuffle.Draw();
      }
      while(offspring[list_differing_nodes[index]] == smallside);

      offspring[list_differing_nodes[index]] = smallside;
    }

    //for debugging
    ones = 0;
    for (unsigned int i = 0; i < offspring.size(); i++) {
      if (offspring[i])
	ones++;
    }
    ////////////////
  } 
  //assert(ones == offspring.size()/2);
  ////////////////


//    cout << "Offspring Eval = "   << pt_offspring->GetSolution().GetEvaluation()
//         << " <== Mother Eval = " << mother->GetSolution().GetEvaluation()
//         << " & Father Eval = "   << father->GetSolution().GetEvaluation()
//         << endl;

  return offspring;
}


//  template <class INDIVIDUAL>
//  LocalSearch<INDIVIDUAL> * Crossover(LocalSearch<INDIVIDUAL> * mother, 
//  				    LocalSearch<INDIVIDUAL> * father) {

//    unsigned int trues(0), falses(0), index(0);
//    int excess(0);
//    bool smallside(false);

//    vector<unsigned int> list_differing_nodes;
//    vector<bool> offspring_solution(mother->GetSolution().GetConfiguration().size());

//    for(unsigned int i = 0; i < mother->GetSolution().GetConfiguration().size(); i++) {
//      // same bit value
//      if(mother->GetSolution().GetConfiguration()[i] == father->GetSolution().GetConfiguration()[i]) {
//        offspring_solution[i] = mother->GetSolution().GetConfiguration()[i];
    
//        if(offspring_solution[i] == true)
//  	trues++;
//        else
//  	falses++;
//      }
//      // different bit value
//      else {
//        list_differing_nodes.push_back(i);
//      }
//    }

//    excess = trues - falses;
//    if (excess > 0)
//      smallside = false;
//    else
//      smallside = true;
//    unsigned int abs_excess(abs(excess));

//   int ones;
//    //for debugging
//    ones = 0;
//    for (unsigned int i = 0; i < offspring_solution.size(); i++) {
//      if (offspring_solution[i])
//        ones++;
//    }
//    ////////////////

//    if (!list_differing_nodes.empty()) {
//      Shuffle shuffle(list_differing_nodes.size());
    
//      for (unsigned int i = 0; i < (list_differing_nodes.size()/2); i++) {
//        offspring_solution[list_differing_nodes[shuffle.Draw()]] = true;
//        offspring_solution[list_differing_nodes[shuffle.Draw()]] = false;
//      }
    
//      //for debugging
//      ones = 0;
//      for (unsigned int i = 0; i < offspring_solution.size(); i++) {
//        if (offspring_solution[i])
//  	ones++;
//      }
//      ////////////////
 
//    // odd list_differing_nodes.size()
//      if (shuffle.size() > 0) {
//        offspring_solution[list_differing_nodes[shuffle.Draw()]] = smallside;
//        abs_excess--;
//      }
    
//      // rebalance
//      shuffle.Reset();
//      for (unsigned int i = 0; i < abs_excess/2; i++) {
//        do {
//  	index = shuffle.Draw();
//        }
//        while(offspring_solution[list_differing_nodes[index]] == smallside);

//        offspring_solution[list_differing_nodes[index]] = smallside;
//      }

//      //for debugging
//      ones = 0;
//      for (unsigned int i = 0; i < offspring_solution.size(); i++) {
//        if (offspring_solution[i])
//  	ones++;
//      }
//      ////////////////
//    } 

//    INDIVIDUAL offspring(offspring_solution);
  
//    //for debugging
//    ones = 0;
//    for (unsigned int i = 0; i < offspring_solution.size(); i++) {
//      if (offspring_solution[i])
//        ones++;
//    }
//    assert(ones == offspring_solution.size()/2);
//    ////////////////

//    LocalSearch<INDIVIDUAL> * pt_offspring (new LocalSearch<INDIVIDUAL>(offspring));
//    assert(pt_offspring);

//    cout << "Offspring Eval = "   << pt_offspring->GetSolution().GetEvaluation()
//         << " <== Mother Eval = " << mother->GetSolution().GetEvaluation()
//         << " & Father Eval = "   << father->GetSolution().GetEvaluation()
//         << endl;
//    return pt_offspring;
//  }


bool MutateBit(void) {
  RandomGenerator * rg(RandomGenCreator::Instance());

  double trial(rg->GetRand(1.0));
  if (trial < Parameters::Instance()->GetMutationProbability())
    return true;
  else
    return false;
}
