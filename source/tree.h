// $Id: tree.h,v 1.34 2002/10/11 21:26:09 mfaerman Exp $

#ifndef __TREE_H__
#define __TREE_H__

#include<vector>
#include<algorithm>
#include<functional>
#include"Handle.h"
using namespace std;

template<class CONTENT> class Tree : public Handleable {

typedef Handle<Tree> HTree;
  //typedef Handle<CONTENT> HCONTENT;
  //typedef Handle<vector<HTree> > HVHTree;

private:
void Remove(void); 

protected:
  HTree Parent;
  vector<HTree> * Children;
  unsigned int Num_Children;
  CONTENT Content;

  void Print_Traversal(void) const;

  void Null_Parent(void) {Parent = HTree(NULL);}

public:
  // constructors
  Tree() :
    Parent(NULL),
    Children(NULL),
    Num_Children(0) {}

  Tree(const CONTENT & c) :
    Parent(NULL),
    Children(new vector<HTree>), 
    Num_Children(0),
    Content(c) {}
  
  Tree(const HTree & p, const CONTENT & c) :
    Parent(p),
    Children(new vector<HTree>),
    Num_Children(0),
    Content(c) {}

  virtual HTree construct(const HTree & p, const  CONTENT & c) 
  {return HTree(new Tree<CONTENT>(p,c));}

  // copy constructor
  Tree(const Tree &);
  // Tree(const HTree &);

  // copy assignment
  Tree& operator = (const Tree&);
  
  virtual ~Tree(void);

  //  HTree & operator = (HTree & ht) {return ht;}

  virtual void RemoveSubTree(void);
  //  virtual void RemoveChild(HTree &);
  virtual void Remove_Node(void);

  HTree & Insert(const HTree & pt);

  virtual HTree & Insert(const CONTENT & c) {
    Num_Children++;
    Children->push_back(construct(HTree(this),c));
    return Children->back();
  }

  virtual CONTENT & content (void) {return Content;}

  virtual CONTENT & operator -(void) {return Content;}

  //virtual HCONTENT pt_content (void) {return HCONTENT(&Content);}

  //virtual HCONTENT operator +(void) {return HCONTENT(&Content);}

  virtual HTree & parent(void) {return Parent;}
  //virtual Tree & GetParent(void) const {return *Parent;}
 
  vector<HTree> *  children(void) const {return Children;}  

  unsigned int GetNumChildren(void) const {return Num_Children;}

  virtual void Print (void) const;
  
  bool operator == (const SG_Task ID) const {return (ID == Content.SG_Id());}
  bool operator == (const NAME_TYPE ID) const {return (ID == Content.name());}
  bool operator == (const int ID) const {return (ID == Content);}
  bool operator == (const HTree ID) const {return (ID == Content);}

  NAME_TYPE Get_ID(const BaseTask t) const {return t.name();}

  int Get_ID(const int i) const {return i;}
  
};

 // copy constructor
template<class CONTENT>
Tree<CONTENT>::Tree(const Tree<CONTENT>& t) {
  unsigned int i;

  Parent = t.Parent;
  Num_Children = t.Num_Children;
  Children = new vector<HTree>(t.Num_Children);
  for(i = 0; i < t.Num_Children; i++) 
    (*Children)[i] = (*t.Children)[i];
  Content = t.Content;
  //cout << "I am at the copy & Tree CONSTRUCTOR" << endl;
}

//  template<class CONTENT>
//  Tree<CONTENT>::Tree(const HTree & t) {
//    unsigned int i;

//    Parent = t->Parent;
//    Num_Children = t->Num_Children;
//    Children = new vector<HTree>(t->Num_Children);
//    for(i = 0; i < t->Num_Children; i++) 
//      (*Children)[i] = (*t->Children)[i];
//    Content = t->Content;
//    //  cout << "I am at the copy * CONSTRUCTOR" << endl;
//  }

//destructor
template<class CONTENT>
Tree<CONTENT>::~Tree(void) {

 //   //cout << "I am at the Tree DESTRUCTOR" << endl;  
//    if (Num_Children > 0)
//      delete Children;
  // In test
  //  if (Content != NULL)
  //  delete Content;
}

//copy assignment
template<class CONTENT>
Tree<CONTENT> & Tree<CONTENT>::operator=(const Tree<CONTENT>& t) {
  unsigned int i;

  if (this != &t) {
//      if (Parent != NULL)
//        delete Parent;
    if (Children != NULL)
      delete Children;
    Num_Children = t.Num_Children;
    Parent = t.Parent;
    Children = new vector<HTree>(t.Num_Children);
    for(i = 0; i < t.Num_Children; i++) 
      (*Children)[i] = (*t.Children)[i];
    Content = t.Content;
  }
  //cout << "I am at the Tree assignment" << endl;
  return *this;
} 

//  template<class CONTENT>
//  HTree Tree<CONTENT>::operator=(HTree pt) {
//    return pt;
//  }

//  template<class CONTENT>
//  HTree Tree<CONTENT>::construct(HTree p, CONTENT c) {
//    return (new Tree<CONTENT>(p,c));
//  }

//  template<class CONTENT>
//  HTree Tree<CONTENT>::Insert(CONTENT c) {
//    HTree pt;
//    Num_Children++;
//    pt = construct(this,c);
//    //  Children->reserve(Num_Children);
//    Children->push_back(pt);
//    return pt;
//  }


template<class CONTENT>
Handle<Tree<CONTENT> > & Tree<CONTENT>::Insert(const HTree & pt) {
  Num_Children++;
  Children->push_back(pt);
  return Children->back();
}

template<class CONTENT>
void Tree<CONTENT>::Remove(void) {
  vector<HTree>::iterator it_node, it;

  //Remove subtree
  if(Num_Children > 0)
    for (it = Children->begin(); it != Children->end(); it = Children->begin())
      (*it)->Remove();

  if (Parent != NULL) {
    // Remove node from its parent
    Parent->Num_Children--;
    it_node = find(Parent->Children->begin(), 
		   Parent->Children->end(), 
		   this);
    Parent->Children->erase(it_node);
  }
  //  cout << "Deleting tree ... \n";
  delete this;
}

template<class CONTENT>
void Tree<CONTENT>::RemoveSubTree(void) {
  Remove();
}

//  template<class CONTENT>
//  void Tree<CONTENT>::RemoveChild(HTree & Hchild) {
//    Remove(Children->begin(), Children->end(), Hchild);
//  }

template<class CONTENT>
void Tree<CONTENT>::Remove_Node(void) {
  vector<HTree>::iterator it_node, it;

/*    if(Children->size() > 0) { */
/*      cout << "Error: Parent being removed with children still active" << endl; */
/*      exit(-1); */
/*    } */

  if (Parent != NULL) {
    // Remove node from its parent
    //    Parent->Num_Children--;
    if (Parent->Children->size() > 0) {
      it_node = find(Parent->Children->begin(), 
		     Parent->Children->end(), 
		     this);
      if (*it_node == this)
	Parent->Children->erase(it_node);
    }
  }

/*    //for_each(Children->begin(), Children->end(), mem_fun(&Tree::Null_Parent)); */
/*    for (unsigned int i = 0; i < Children->size(); i++) */
/*      (*Children)[i]->Null_Parent(); */
/*    //cout << "Deleting tree at Remove_Node ... \n"; */
/*    delete this; */
}

template<class CONTENT>
void Tree<CONTENT>::Print_Traversal(void) const{

  vector<HTree >::iterator it;

  cout << "(";
  //  cout << Get_ID(Content);

  if(Num_Children > 0)
    for (it = Children->begin(); it != Children->end(); it++)
      (*it)->Print_Traversal();

  cout << ")";
}

template<class CONTENT>
void Tree<CONTENT>::Print(void) const{  
  Print_Traversal();
  cout << endl;
}

#endif






