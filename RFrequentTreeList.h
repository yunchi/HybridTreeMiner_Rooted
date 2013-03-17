/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	Header: RFrequentTreeList.h
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Rmisc.h"
#include "RFreeTree.h"
#include "RBFCFTree.h"
#include "RDFCFTree.h"
#include "RFrequentTreeFamily.h"

using namespace std;

#ifndef RFrequentTreeList_H
#define RFrequentTreeList_H

class RFrequentTreeList
{
public:
	long returnSize(); //return number of elements in the map

	//this set of functions are defined for extention-only exploration, DFCF
	void populate3(vector<ptrRFreeTree>& database);
	void finalize3(const long threshold); //remove elements whose support is less than a threshold
	void extensionExploreList3(const vector<ptrRFreeTree>& database, 
							  ostream& outFile,
							  vector<long> & frequency,
							  long & threshold); //starting from list, find all frequent trees

	//this set of functions are defined for extention-only exploration, BFCF
	void populate2(vector<ptrRFreeTree>& database);
	void finalize2(const long threshold); //remove elements whose support is less than a threshold
	void extensionExploreList2(const vector<ptrRFreeTree>& database, 
							  ostream& outFile,
							  vector<long> & frequency,
							  long & threshold); //starting from list, find all frequent trees

	//this set of functions are defined for hybrid exploration, BFCF
	void populate1(vector<ptrRFreeTree>& database);
	void finalize1(const long threshold);
	void hybridExploreList(const vector<ptrRFreeTree>& database, 
							  ostream& outFile,
							  vector<long> & frequency,
							  long & threshold); //starting from list, find all frequent trees

	void test();

	friend istream& operator>>(istream&, RFrequentTreeList&);
	friend ostream& operator<<(ostream&, const RFrequentTreeList&);

public:
	map<vector<short>, SupportNode> frequent2List;
	vector< vector<short> > frequent2Tree;
	map<short, SupportNode> frequent1List;
	map<vector<short>, LegSupportNode> frequent2Leg;
	vector<long> headIndex;
};

#endif //RFrequentTreeList_H
