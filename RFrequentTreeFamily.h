/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	Header: RFrequentTreeFamily.h
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Rmisc.h"
#include "RFreeTree.h"
#include "RBFCFTree.h"

using namespace std;

#ifndef RFrequentTreeFamily_H
#define RFrequentTreeFamily_H

class RFrequentTreeFamily
{
public:

	RFrequentTreeFamily(const vector<short>& cString); //constructor with a BFCS as
	//parameter, which is passed to the RBFCFTree--mother

	RFrequentTreeFamily(const RBFCFTree& originalTree, Leg& newLeg); 
	//constructor with an old mother plus a leg
	void hybrideExplore( vector<long>& frequency,
				  ostream& outFile,
				  const vector<ptrRFreeTree>& database,
				  const vector< vector<short> > & frequent2Tree,
				  const vector<long> & headIndex,
				  const long & threshold,
				  const bool & isFirstFamily );
	void test();

	friend istream& operator>>(istream&, RFrequentTreeFamily&);
	friend ostream& operator<<(ostream&, const RFrequentTreeFamily&);

	friend class RFrequentTreeList;

protected:
	RBFCFTree mother;
	vector<Occurrence> motherOccList;
	vector<Leg> legs;
};

#endif //RFrequentTreeFamily_H
