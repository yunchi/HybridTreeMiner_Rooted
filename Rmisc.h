/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	Header: Rmisc.h
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef Rmisc_H
#define Rmisc_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <functional>
#include <vector>
#include <list>
#include <iterator>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <utility>

using namespace std;

const short ENDOFTREETAG = 30002; // the "#" symbol
const short ENDTAG = 30001; // the "$" symbol

//extern long SUPPORT_THRESHOLD;
extern short MIN_EDGE;
extern short MAX_EDGE;
extern short MIN_VERTEX;
extern short MAX_VERTEX;
//as a result, the maximal number of vertices in a tree is 3000

struct Edge
{
  short vertex1; //one end of the edge
  short vertex2; //the other end of the edge
  short eLabel; //the label of the edge
  Edge(short v = 0, short w = 0, short eLabel = 0) : 
    vertex1(v), vertex2(w), eLabel(eLabel) { }
  Edge(const Edge& rhs) { vertex1 = rhs.vertex1; vertex2 = rhs.vertex2; eLabel = rhs.eLabel; }
  Edge& operator=(const Edge& rhs) 
  { vertex1 = rhs.vertex1; vertex2 = rhs.vertex2; eLabel = rhs.eLabel; return *this; }
};

struct Occurrence
{
	long tid; //the tid for the occurrence
	vector<short> nodeIndex; //the indices of the vertices in the occurrences
	
	friend istream& operator>>(istream&, Occurrence&);
	friend ostream& operator<<(ostream&, const Occurrence&);
};

struct SupportNode //implementation in RFrequentTreeList.cpp
{
	long support;
	long lastTid;
	vector<Occurrence> occList;
	SupportNode();
	long occListSize() const;
	
	friend istream& operator>>(istream&, SupportNode&);
	friend ostream& operator<<(ostream&, const SupportNode&);
};

struct LegOccurrence //implementation in RFrequentTreeList.cpp
{
	long motherID; //the index for the occurrence in the occList of mother
	short myIndex; //the indices of the vertices in the occurrences
	LegOccurrence(long x, short y) : motherID(x), myIndex(y) { };
	LegOccurrence( ) : motherID(0), myIndex(0) { };
	
	friend istream& operator>>(istream&, LegOccurrence&);
	friend ostream& operator<<(ostream&, const LegOccurrence&);
};

struct Leg
{
	short connectTo;
	short legEdgeLabel;
	short legVertexLabel;
	vector<LegOccurrence> legOccList;
	
	friend istream& operator>>(istream&, Leg&);
	friend ostream& operator<<(ostream&, const Leg&);
};

struct LegSupportNode //implemented in RFrequentList.cpp
{
	long support;
	long lastTid;
	Leg thisLeg;
	LegSupportNode();

	friend istream& operator>>(istream&, LegSupportNode&);
	friend ostream& operator<<(ostream&, const LegSupportNode&);
};


class RFreeTree;
typedef RFreeTree *ptrRFreeTree;

#endif //Rmisc_H
