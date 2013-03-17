/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	Header: RBFCFTree.h
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Rmisc.h"
#include "RFreeTree.h"

using namespace std;

#ifndef RBFCFTree_H
#define RBFCFTree_H

class RBFCFTree : public RFreeTree
{
public:
	RBFCFTree(short v, long t);
	RBFCFTree(long t);
	RBFCFTree();
	RBFCFTree(const RBFCFTree& rhs);
	RBFCFTree(const RBFCFTree& parent, 
				   short newEdgeLabel, short newVertexLabel, short position);

	RBFCFTree(const vector<short>& cString);
	~RBFCFTree();

	RBFCFTree& operator=(const RBFCFTree& rhs);

	void computeBFCS();
	void computeAutomorphism();
	void possibleLegs(const short i, short& minEdge, short& minVertex );
	void extensionExplore( vector<long>& frequency,
				  ostream& outFile,
				  const vector<ptrRFreeTree>& database,
				  const vector<Occurrence> & occList,
				  const vector< vector<short> > & frequent2Tree,
				  const vector<long> & headIndex,
				  const long & threshold);

	bool operator<(const RBFCFTree& rhs) const;

	friend istream& operator>>(istream&, RBFCFTree&);
	friend ostream& operator<<(ostream&, const RBFCFTree&);

	friend class RFrequentTreeFamily;

public:
	vector <short> canonicalString; //the canonical string of this tree

protected:
	vector <short> level; //the levels of vertices
	vector <short> automorphism; //the list of automorphisms
};

#endif //RBFCFTree_H
