#include"tree.h"
#include"tasks.h"
#include"tree_tasks.h"

void main(void) {

  int a,b,c,d,i;

  vector <Tree<BaseTask> *> * v;
  Tree<BaseTask> * i0;
  Tree<BaseTask> *i1, *i2, *i3, *i4;
  Tree<int>  *ii0, *ii1, *ii2, *ii3;
  TTask * it0;

  BaseTask t1("T1", 10, 1, 3);
  BaseTask t2("T2", 20, 1, 3);
  BaseTask t3("T3", 30, 1, 3);
  BaseTask t4("T4", 40, 1, 3);
  BaseTask t5("T5", 50, 1, 3);
  BaseTask t6("T6", 60, 1, 3);
  BaseTask t7("T7", 70, 1, 3);

  BaseTask tt8("TT8", 80, 1, 3);
  BaseTask tt9("TT9", 90, 1, 3);

  TTask tt2;

  TTask tt(tt8);
  tt.Set_Fraction(1.23456789);
  tt.Print();
  
  it0 = tt.Insert(tt9);
  it0->Set_Fraction(0.2);
  i4 = it0;
  i4->Print(); 
  it0->Print();
  tt.Print();

  tt2 = tt;
  tt2.Print();

  a = 5;
  b = 3;
  c = 1;
  d = -2;

  Tree<int> tint(a);
  ii0 = tint.Insert(b);
  
  ii0->Print();
  tint.Print();

  ii1 = ii0->Insert(c);

  ii0->Print();
  tint.Print();

  ii2 = tint.Insert(d);

  tint.Print();
  
  // ii0 = find(tint.children()->begin(), tint.children()->end(), b);
  ii0->Remove();

  tint.Print();

  tint.Remove();
  tint.Print();

  //  i = tint.children().front().content();
  // tint.children().back().Print();

  //cout << i << endl;

  Tree<BaseTask> ttask(t1);

  v = ttask.children();

  ttask.Print();

  i1 = ttask.Insert(t2);  

  //i1 = find(v->begin(), v->end(), "T2");
 
  i1->Print();ttask.Print();

  i0 = i1->Insert(t4);

  //i1 = find(v->begin(), v->end(), "T2");

  i1->Insert(t6);

  i1->Print();ttask.Print();

  i0->Remove();

  ttask.Print();
  
  i2 = ttask.Insert(t3);  

  // i2 = find(v->begin(), v->end(), "T3");

  i2->Print();ttask.Print();

  i3 = i2->Insert(t5);

  // i3 = find(v->begin(), v->end(), "T5");

  i2->Print();ttask.Print();

  i3->Print();ttask.Print();
  i3->Insert(t7);
  i3->Print();i2->Print();ttask.Print();

  //i3 = find (i2->children()->begin(), i2->children()->end(), "T5");

  
  i3->Remove();
  ttask.Print();

  ttask.Remove();
  ttask.Print();
}


//    for (it = v->begin(); ! v->empty(); it = v->begin()) {
//      cout << v->size() << endl;
//      cout << it->content().name() << endl;
//      it->Remove();
//    }


