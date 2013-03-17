/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	Header: RFreeTree.h
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Rmisc.h"

using namespace std;

#ifndef RFreeTree_H
#define RFreeTree_H

struct Tnode
{
  short v;		//the other vertex this link connects to
  short eLabel;	//edge label
  Tnode* next;	//head of the link list for adjacent vertices
  Tnode(short x, short e, Tnode* t) 
  { v = x; eLabel = e; next = t; }

  // shallow copy constructor
  Tnode(const Tnode& rhs) { v = rhs.v; eLabel = rhs.eLabel; next = 0; } 
  Tnode& operator=(const Tnode& rhs) 
  { v = rhs.v; eLabel = rhs.eLabel; next = 0; return *this; }
};

typedef Tnode *TnodeLink;

class RFreeTree
{
 public:
  RFreeTree(short v, long t);
  RFreeTree(long t);
  RFreeTree();
  RFreeTree(const RFreeTree& rhs); //copy constructor
  ~RFreeTree();

  RFreeTree& operator=(const RFreeTree& rhs);

  short vertexNumber() const;
  long returnTid() const;
  void changeVertexLabel(short idx, short vLabel);
  void insertEdge(Edge e);
  
  friend istream& operator>>(istream&, RFreeTree&);
  friend ostream& operator<<(ostream&, const RFreeTree&);

public:
  vector <TnodeLink> adj; //adjacency list (heads)
  short vCount; //number of vertices
  long tid;  //transaction ID
  vector <short> vertexLabel; //labels of vertices
  vector <short> degree; //degrees of vertices
};

#endif //RFreeTree_H
