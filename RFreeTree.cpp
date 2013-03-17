/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	Class: RFreeTree

	Description: Used to store one tree, i.e., a transaction in
	the database.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Rmisc.h"
#include "RFreeTree.h"

using namespace std;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	constructor

	Decription: Assuming that the number of vertices v and the tid t are given
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
RFreeTree::RFreeTree(short v, long t) :
  vCount(v), tid(t), adj(v+1, (TnodeLink) 0), vertexLabel(v+1), degree(v+1)
{
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	constructor

	Decription: Assuming that only tid t is given
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
RFreeTree::RFreeTree(long t) :
  vCount(0), tid(t), adj(0), vertexLabel(0), degree(0)
{ 
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	constructor

	Decription: Assuming that nothing is given
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
RFreeTree::RFreeTree() : 
  vCount(0), tid(0), adj(0), vertexLabel(0), degree(0)
{
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	constructor

	Decription: deep copy constructor, don't change the order in adj list
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
RFreeTree::RFreeTree(const RFreeTree& rhs)
{
	vCount = rhs.vCount;
	tid = rhs.tid;
	adj.resize(vCount + 1);
	TnodeLink t1, t2, t3;
	for ( short i = 1; i <= vCount; i++ )
	{
		t1 = rhs.adj[i];
		if ( t1 == 0 ) //unlike to happen for a tree
		{
			adj[i] = 0;	
			continue;
		}
		else
		{
			t2 = new Tnode(*t1);
			adj[i] = t2;
			while ( t1->next != 0 )
			{
				t1 = t1->next;
				t3 = new Tnode(*t1);
				t2->next = t3;
				t2 = t3;
			}
		}
	}
	vertexLabel = rhs.vertexLabel;
	degree = rhs.degree;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	operator=()

	Decription: assignment copy, same as copy constructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
RFreeTree& RFreeTree::operator=(const RFreeTree& rhs)
{
	vCount = rhs.vCount;
	tid = rhs.tid;
	adj.resize(vCount + 1);
	TnodeLink t1, t2, t3;
	for ( short i = 1; i <= vCount; i++ )
	{
		t1 = rhs.adj[i];
		if ( t1 == 0 ) //unlike to happen for a tree
		{
			adj[i] = 0;	
			continue;
		}
		else
		{
			t2 = new Tnode(*t1);
			adj[i] = t2;
			while ( t1->next != 0 )
			{
				t1 = t1->next;
				t3 = new Tnode(*t1);
				t2->next = t3;
				t2 = t3;
			}
		}
	}
	vertexLabel = rhs.vertexLabel;
	degree = rhs.degree;
	return *this;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	destructor

	Decription: deeply clean the vector of link lists
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
RFreeTree::~RFreeTree()
{
  TnodeLink holder1;
  TnodeLink holder2;
  for (short i = 1; i <= vCount; i++)	//there is nothing to do with adj[0]
    {
      holder1 = adj[i];
      while (holder1 != 0)
		{
			holder2 = holder1->next;
			delete holder1;
			holder1 = holder2;
		}
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	vertexNumber()

	Decription: return the number of vertices--vCount
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
short RFreeTree::vertexNumber() const { return vCount; }


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	returnTid()

	Decription: return the tid of current free tree
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
long RFreeTree::returnTid() const { return tid; }


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	changeVertexLabel()

	Decription: given an index, change the vertex label of the corresponding vertex 
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RFreeTree::changeVertexLabel(short idx, short vLabel)
{
	vertexLabel[idx] = vLabel;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	insertEdge()

	Decription: add an edge between to vertices. Used in the beginning of construction
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RFreeTree::insertEdge(Edge e)
{
  short x = e.vertex1, y = e.vertex2;
  short c = e.eLabel;
  TnodeLink n = new Tnode(x, c, adj[y]);
  adj[y] = n;
  degree[y]++; 
  n = new Tnode(y, c, adj[x]);
  adj[x] = n;
  degree[x]++;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	operator>>()

	Decription: read in free trees
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
istream& operator>>(istream& in, RFreeTree& rhs)
{
  long t;	//tid
  short v;	//number of vertices
  short p1, p2;
  short vLabel, eLabel;

  in >> t;
  in >> v;

  rhs.adj.resize(v+1, (TnodeLink) 0);
  rhs.vertexLabel.resize(v+1,0);
  rhs.degree.resize(v+1,0);

  rhs.tid = t;
  rhs.vCount = v;
  rhs.vertexLabel[0] = 0;	//null
  for (short i = 1; i <= v; i++)
  {
	in >> vLabel;
	rhs.vertexLabel[i] = vLabel;
  }
	  
  for (short i = 1; i < v; i++) //the number of edges is v-1
  {
	in >> eLabel;
	in >> p1;
	in >> p2;
	rhs.insertEdge(Edge(p1,p2,eLabel));
  }

  return in;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	operator<<()

	Decription: write out free trees
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
ostream& operator<<(ostream& out, const RFreeTree& rhs)
{
  out << rhs.tid << endl;
  out << rhs.vCount << endl;
  for (short i = 1; i <= rhs.vCount; i++)
	out << rhs.vertexLabel[i] << endl;

  TnodeLink n;
  for (short i = 1; i <= rhs.vCount; i++)
  {
	n = rhs.adj[i];
	while (n != 0) 
	{
		if ( i < n->v )
		{
			out << n->eLabel << ' ' << i << ' ' << n->v << endl;
		}
		n = n->next;
    }
  }
  return out;
}  
