/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	Header: RDFCFTree.h
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Rmisc.h"
#include "RFreeTree.h"

using namespace std;

#ifndef RDFCFTree_H
#define RDFCFTree_H

class RDFCFTree : public RFreeTree
{
public:
	RDFCFTree(short v, long t);
	RDFCFTree(long t);
	RDFCFTree();
	RDFCFTree(const RDFCFTree& rhs);
	RDFCFTree(const RDFCFTree& parent, 
				   short newEdgeLabel, short newVertexLabel, short position);

	RDFCFTree(const vector<short>& cString);
	~RDFCFTree();

	RDFCFTree& operator=(const RDFCFTree& rhs);

	void computeDFCS();
	void computeSubDFCS(short root, vector<short>& cString);
	void computeAutomorphism();
	void possibleLegs(const short i, short& minEdge, short& minVertex );
	void extensionExplore( vector<long>& frequency,
				  ostream& outFile,
				  const vector<ptrRFreeTree>& database,
				  const vector<Occurrence> & occList,
				  const vector< vector<short> > & frequent2Tree,
				  const vector<long> & headIndex,
				  const long & threshold);

	bool operator<(const RDFCFTree& rhs) const;

	friend istream& operator>>(istream&, RDFCFTree&);
	friend ostream& operator<<(ostream&, const RDFCFTree&);

	friend class RFrequentTreeFamily;

public:
	vector <short> canonicalString; //the canonical string of this tree

protected:
	vector <short> level; //the levels of vertices
	vector <short> automorphism; //the list of automorphisms
};

#endif //RDFCFTree_H
