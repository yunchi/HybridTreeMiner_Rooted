/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	Class: RFrequentTreeList

	Description: Used to store the list of frequent 2-trees
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Rmisc.h"
#include "RFrequentTreeList.h"

using namespace std;

SupportNode::SupportNode() : support(0), lastTid(-1)
{
}

long SupportNode::occListSize() const
{
	return occList.size();
}

LegSupportNode::LegSupportNode() : support(0), lastTid(-1)
{
}

istream& operator>>(istream& in, Occurrence& rhs)
{
	return in;
}

ostream& operator<<(ostream& out, const Occurrence& rhs)
{
	out << rhs.tid << ": ";
	for ( short i = 0; i < rhs.nodeIndex.size(); i++ )
		out << rhs.nodeIndex[i] << ' ';
	out << endl;
	return out;
}

istream& operator>>(istream& in, SupportNode& rhs)
{
	return in;
}

ostream& operator<<(ostream& out, const SupportNode& rhs)
{
	out << "support: " << rhs.support << endl;
	out << "lastTid: " << rhs.lastTid << endl;
	for ( long s = 0; s < rhs.occList.size(); s++ )
		out << rhs.occList[s];
	out << endl;
	return out;
}

istream& operator>>(istream& in, RFrequentTreeList& rhs)
{
	return in;
}

ostream& operator<<(ostream& out, const RFrequentTreeList& rhs)
{
	map<vector<short>, SupportNode>::const_iterator pos;
	for ( pos = rhs.frequent2List.begin(); pos != rhs.frequent2List.end(); ++pos )
	{
		out << "label: " << pos->first[0] << ' ' << pos->first[1] << ' ' << pos->first[2] << endl;
		out << pos->second;
	}
	return out;
}

istream& operator>>(istream& in, LegOccurrence& rhs)
{
	return in;
}

ostream& operator<<(ostream& out, const LegOccurrence& rhs)
{
	out << "motherID: " << rhs.motherID;
	out << "  myIndex: " << rhs.myIndex << endl;
	return out;
}

istream& operator>>(istream& in, Leg& rhs)
{
	return in;
}

ostream& operator<<(ostream& out, const Leg& rhs)
{
	out << "connectTo: " << rhs.connectTo << endl;
	out << "legEdgeLabel: " << rhs.legEdgeLabel << endl;
	out << "legVertexLabel: " << rhs.legVertexLabel << endl;
	out << "legOccList: " << endl;
	for ( long s = 0; s < rhs.legOccList.size(); s++ )
		out << rhs.legOccList[s];
	out << endl;
	return out;
}

istream& operator>>(istream& in, LegSupportNode& rhs)
{
	return in;
}

ostream& operator<<(ostream& out, const LegSupportNode& rhs)
{
	out << "support: " << rhs.support << endl;
	out << "lastTid: " << rhs.lastTid << endl;
	out << "Leg: " << rhs.thisLeg << endl;
	return out;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	returnSize()

	Decription: return the size of the map (the list of 2-trees)
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
long RFrequentTreeList::returnSize()
{
	return frequent2List.size();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	populate3(vector<ptrFreeTree> database)

	Decription: scan the database to enumerate all 2-trees
	Note: assuming the tid for the database is from 1 to |size|
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RFrequentTreeList::populate3(vector<ptrRFreeTree>& database)
{
	TnodeLink t;
	pair<map<vector<short>,SupportNode>::iterator,bool> result;
	SupportNode sNode;
	RFreeTree *pft;
	vector<short> sTriple(3);
	Occurrence sOcc;

	for ( long s = 0; s < database.size(); s++ )
	{
		pft = database[s];
		for ( short i = 1; i <= pft->vertexNumber(); ++i )
		{
			t = pft->adj[i];
			while ( t != 0 )
			{
				sTriple[0] = pft->vertexLabel[i];
				sTriple[1] = t->eLabel;
				sTriple[2] = pft->vertexLabel[t->v];
				//update information on vertex and edge labels
				if ( i < t->v )
				{
					if ( sTriple[0] < MIN_VERTEX ) MIN_VERTEX = sTriple[0];
					if ( sTriple[0] > MAX_VERTEX ) MAX_VERTEX = sTriple[0];
					if ( sTriple[2] < MIN_VERTEX ) MIN_VERTEX = sTriple[2];
					if ( sTriple[2] > MAX_VERTEX ) MAX_VERTEX = sTriple[2];
					if ( sTriple[1] < MIN_EDGE ) MIN_EDGE = sTriple[1];
					if ( sTriple[1] > MAX_EDGE ) MAX_EDGE = sTriple[1];
				
					sNode.lastTid = pft->tid;
					sNode.support = 1;
					sNode.occList.clear();
					sOcc.tid = pft->tid;
					sOcc.nodeIndex.clear();
					sOcc.nodeIndex.push_back(i);
					sOcc.nodeIndex.push_back(t->v);
					sNode.occList.push_back(sOcc);
					result = frequent2List.insert(make_pair(sTriple,sNode));
					if ( !result.second ) //if the tree already exists
					{
						//if the new tree has a new tid, then the frequency count
						//should be increased by 1
						//NOTE: assuming tids are coming in increasing order
						if ( ((result.first)->second).lastTid != sNode.lastTid )
						{
							((result.first)->second).lastTid = sNode.lastTid;
							((result.first)->second).support++;
						}	
						((result.first)->second).occList.push_back(sOcc);
					}
				}
				t = t->next;
			}//end of while(t!=0)
		}//end of for (i = ...)
	}//end of for (s = ...)

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	finalize3()

	Decription: remove elements whose support is less than a threshold
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RFrequentTreeList::finalize3(const long threshold)
{
	vector<short> sTriple(3);

	sTriple[0] = -1;
	sTriple[1] = -1;
	sTriple[2] = -1;
	frequent2Tree.push_back(sTriple); //a dummy node

	long index = 1; //the index for the next position in frequent2Tree
	headIndex.resize(MAX_VERTEX - MIN_VERTEX + 1, 0);

	map<vector<short>, SupportNode>::iterator pos;
	for ( pos = frequent2List.begin(); pos != frequent2List.end(); )
	{
		if ( (pos->second).support < threshold )
			frequent2List.erase(pos++);
		else
		{
			frequent2Tree.push_back(pos->first);
			if ( (pos->first)[0] != sTriple[0] )
			{
				headIndex[(pos->first)[0] - MIN_VERTEX] = index;
				sTriple[0] = (pos->first)[0];
			}
			index++;
			++pos;
		}
	}

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	extensionExploreList3()

	Decription: starting from frequent2Tree list, find all frequent trees
	using extension operation only, DFCF
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RFrequentTreeList::extensionExploreList3(const vector<ptrRFreeTree>& database,
									ostream& outFile, vector<long>& frequency,
									long& threshold)
{
	RDFCFTree *pfd;
	vector<short> vString(7);
	vString[0] = 2; //2 vertices
	vString[1] = 0; //edge to parent of the root
	vString[5] = ENDOFTREETAG;
	map<vector<short>, SupportNode>::iterator pos;
	
	for ( pos = frequent2List.begin(); pos != frequent2List.end(); ++pos )
	{
		vString[2] = pos->first[0];
		vString[3] = pos->first[1];
		vString[4] = pos->first[2];
		pfd = new RDFCFTree(vString);
		pfd->extensionExplore(frequency, outFile, database, pos->second.occList, 
								frequent2Tree, headIndex, threshold );
		delete pfd;
	}
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	populate2(vector<ptrFreeTree> database)

	Decription: scan the database to enumerate all 2-trees
	Note: assuming the tid for the database is from 1 to |size|
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RFrequentTreeList::populate2(vector<ptrRFreeTree>& database)
{
	TnodeLink t;
	pair<map<vector<short>,SupportNode>::iterator,bool> result;
	SupportNode sNode;
	RFreeTree *pft;
	vector<short> sTriple(3);
	Occurrence sOcc;

	for ( long s = 0; s < database.size(); s++ )
	{
		pft = database[s];
		for ( short i = 1; i <= pft->vertexNumber(); ++i )
		{
			t = pft->adj[i];
			while ( t != 0 )
			{
				sTriple[0] = pft->vertexLabel[i];
				sTriple[1] = t->eLabel;
				sTriple[2] = pft->vertexLabel[t->v];
				//update information on vertex and edge labels
				if ( i < t->v )
				{
					if ( sTriple[0] < MIN_VERTEX ) MIN_VERTEX = sTriple[0];
					if ( sTriple[0] > MAX_VERTEX ) MAX_VERTEX = sTriple[0];
					if ( sTriple[2] < MIN_VERTEX ) MIN_VERTEX = sTriple[2];
					if ( sTriple[2] > MAX_VERTEX ) MAX_VERTEX = sTriple[2];
					if ( sTriple[1] < MIN_EDGE ) MIN_EDGE = sTriple[1];
					if ( sTriple[1] > MAX_EDGE ) MAX_EDGE = sTriple[1];
				
					sNode.lastTid = pft->tid;
					sNode.support = 1;
					sNode.occList.clear();
					sOcc.tid = pft->tid;
					sOcc.nodeIndex.clear();
					sOcc.nodeIndex.push_back(i);
					sOcc.nodeIndex.push_back(t->v);
					sNode.occList.push_back(sOcc);
					result = frequent2List.insert(make_pair(sTriple,sNode));
					if ( !result.second ) //if the tree already exists
					{
						//if the new tree has a new tid, then the frequency count
						//should be increased by 1
						//NOTE: assuming tids are coming in increasing order
						if ( ((result.first)->second).lastTid != sNode.lastTid )
						{
							((result.first)->second).lastTid = sNode.lastTid;
							((result.first)->second).support++;
						}	
						((result.first)->second).occList.push_back(sOcc);
					}
				}
				t = t->next;
			}//end of while(t!=0)
		}//end of for (i = ...)
	}//end of for (s = ...)

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	finalize2()

	Decription: remove elements whose support is less than a threshold
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RFrequentTreeList::finalize2(const long threshold)
{
	vector<short> sTriple(3);

	sTriple[0] = -1;
	sTriple[1] = -1;
	sTriple[2] = -1;
	frequent2Tree.push_back(sTriple); //a dummy node

	long index = 1; //the index for the next position in frequent2Tree
	headIndex.resize(MAX_VERTEX - MIN_VERTEX + 1, 0);

	map<vector<short>, SupportNode>::iterator pos;
	for ( pos = frequent2List.begin(); pos != frequent2List.end(); )
	{
		if ( (pos->second).support < threshold )
			frequent2List.erase(pos++);
		else
		{
			frequent2Tree.push_back(pos->first);
			if ( (pos->first)[0] != sTriple[0] )
			{
				headIndex[(pos->first)[0] - MIN_VERTEX] = index;
				sTriple[0] = (pos->first)[0];
			}
			index++;
			++pos;
		}
	}

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	extensionExploreList2()

	Decription: starting from frequent2Tree list, find all frequent trees
	using extension operation only
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RFrequentTreeList::extensionExploreList2(const vector<ptrRFreeTree>& database,
									ostream& outFile, vector<long>& frequency,
									long& threshold)
{
	RBFCFTree *pfb;
	vector<short> vString(7);
	vString[0] = 2; //2 vertices
	vString[1] = 0; //edge to parent of the root
	vString[3] = ENDTAG;
	vString[6] = ENDOFTREETAG;
	map<vector<short>, SupportNode>::iterator pos;
	
	for ( pos = frequent2List.begin(); pos != frequent2List.end(); ++pos )
	{
		vString[2] = pos->first[0];
		vString[4] = pos->first[1];
		vString[5] = pos->first[2];
		pfb = new RBFCFTree(vString);
		pfb->extensionExplore(frequency, outFile, database, pos->second.occList, 
								frequent2Tree, headIndex, threshold );
		delete pfb;
	}
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	populate1(vector<ptrFreeTree> database)

	Decription: scan the database to enumerate all single nodes and 2-trees
	Note: assuming the tid for the database is from 1 to |size|
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RFrequentTreeList::populate1(vector<ptrRFreeTree>& database)
{
	TnodeLink t;
	pair<map<short,SupportNode>::iterator,bool> resultSingle;
	pair<map<vector<short>, LegSupportNode>::iterator,bool> result2Leg;
	SupportNode sNode;
	LegSupportNode gsNode;
	RFreeTree *pft;
	vector<short> sTriple(3);
	Occurrence sOcc;
	LegOccurrence gsOcc;
	long m;

	for ( long s = 0; s < database.size(); s++ )
	{
		pft = database[s];
		for ( short i = 1; i <= pft->vertexNumber(); ++i )
		{
			//first, add the vertex to the list of single nodes
			sNode.lastTid = pft->tid;
			sNode.support = 1;
			sNode.occList.clear();
			sOcc.tid = pft->tid;
			sOcc.nodeIndex.clear();
			sOcc.nodeIndex.push_back(i);
			sNode.occList.push_back(sOcc);
			m = 1; //currently, there is only 1 occurrence in the 1-tree
			resultSingle = frequent1List.insert(make_pair(pft->vertexLabel[i],sNode));
			if ( !resultSingle.second ) //if the tree (single node) already exists
			{
				//if the new tree has a new tid, then the frequency count
				//should be increased by 1
				//NOTE: assuming tids are coming in increasing order
				if ( ((resultSingle.first)->second).lastTid != sNode.lastTid )
				{
					((resultSingle.first)->second).lastTid = sNode.lastTid;
					((resultSingle.first)->second).support++;
				}	
				((resultSingle.first)->second).occList.push_back(sOcc);
				m = ((resultSingle.first)->second).occListSize();
			}

			//second, add all legs resulted from this vertex
			t = pft->adj[i];
			while ( t != 0 )
			{
				sTriple[0] = pft->vertexLabel[i];
				sTriple[1] = t->eLabel;
				sTriple[2] = pft->vertexLabel[t->v];
				//update information on vertex and edge labels
				if ( i < t->v ) //this if should always result in "true"
				{
					if ( sTriple[0] < MIN_VERTEX ) MIN_VERTEX = sTriple[0];
					if ( sTriple[0] > MAX_VERTEX ) MAX_VERTEX = sTriple[0];
					if ( sTriple[2] < MIN_VERTEX ) MIN_VERTEX = sTriple[2];
					if ( sTriple[2] > MAX_VERTEX ) MAX_VERTEX = sTriple[2];
					if ( sTriple[1] < MIN_EDGE ) MIN_EDGE = sTriple[1];
					if ( sTriple[1] > MAX_EDGE ) MAX_EDGE = sTriple[1];

					gsNode.lastTid = pft->tid;
					gsNode.support = 1;
					gsNode.thisLeg.connectTo = 1;
					gsNode.thisLeg.legEdgeLabel = sTriple[1];
					gsNode.thisLeg.legVertexLabel = sTriple[2];
					gsNode.thisLeg.legOccList.clear();
					gsOcc.motherID = m - 1; //the index of mother starting from 0
					gsOcc.myIndex = t->v;
					gsNode.thisLeg.legOccList.push_back(gsOcc);
					result2Leg = frequent2Leg.insert(make_pair(sTriple,gsNode));
					if ( !result2Leg.second ) //if the 2-tree already exists
					{
						//if the new tree has a new tid, then the frequency count
						//should be increased by 1
						//NOTE: assuming tids are coming in increasing order
						if ( ((result2Leg.first)->second).lastTid != gsNode.lastTid )
						{
							((result2Leg.first)->second).lastTid = gsNode.lastTid;
							((result2Leg.first)->second).support++;
						}	
						((result2Leg.first)->second).thisLeg.legOccList.push_back(gsOcc);
					}
				}
				t = t->next;
			}//end of while(t!=0)			

		}//end of for(short i...)
	}//end of for(long s...)
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	finalize1()

	Decription: remove elements whose support is less than a threshold
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RFrequentTreeList::finalize1(const long threshold)
{
	vector<short> sTriple(3);

	sTriple[0] = -1;
	sTriple[1] = -1;
	sTriple[2] = -1;
	frequent2Tree.push_back(sTriple); //a dummy node

	long index = 1; //the index for the next position in frequent2Tree
	headIndex.resize(MAX_VERTEX - MIN_VERTEX + 1, 0);

	map<vector<short>, LegSupportNode>::iterator pos;
	for ( pos = frequent2Leg.begin(); pos != frequent2Leg.end(); )
	{
		if ( (pos->second).support < threshold )
			frequent2Leg.erase(pos++);
		else
		{
			frequent2Tree.push_back(pos->first);
			if ( (pos->first)[0] != sTriple[0] )
			{
				headIndex[(pos->first)[0] - MIN_VERTEX] = index;
				sTriple[0] = (pos->first)[0];
			}
			index++;
			++pos;
		}
	}

	map<short, SupportNode>::iterator pos1;
	for ( pos1 = frequent1List.begin(); pos1 != frequent1List.end(); )
	{
		if ( (pos1->second).support < threshold )
			frequent1List.erase(pos1++);
		else
			++pos1;
	}

}


void RFrequentTreeList::test()
{

	for ( short i = 0; i < frequent2Tree.size(); i++ )
	{
		for ( short j = 0; j < frequent2Tree[i].size(); j++ )
			cout << frequent2Tree[i][j] << ' ';
		cout << endl;
	}

	map<short, SupportNode>::iterator pos1;
	map<vector<short>, LegSupportNode>::iterator pos2;

	for ( pos1 = frequent1List.begin(); pos1 != frequent1List.end(); ++pos1)
	{
		cout << "vertex label: " << pos1->first << endl;
		cout << pos1->second;
	}

	for ( pos2 = frequent2Leg.begin(); pos2 != frequent2Leg.end(); ++pos2)
	{
		for ( int i = 0; i < pos2->first.size(); i++ )
			cout << pos2->first[i] << ' ';
		cout << endl;
		cout << pos2->second;
	}


}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	hybridExploreList()

	Decription: starting from frequent 1-tree as the mother of a family and
	corresponding legs, explore all frequent trees
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RFrequentTreeList::hybridExploreList(const vector<ptrRFreeTree>& database,
									ostream& outFile, vector<long>& frequency,
									long& threshold)
{
	vector<short> sTriple(3);
	sTriple[0] = 0;
	sTriple[1] = 0;
	sTriple[2] = 0;

	RFrequentTreeFamily *pff;
	vector<short> vString(4);
	vString[0] = 1; //1 vertices
	vString[1] = 0; //edge to parent of the root
	vString[3] = ENDOFTREETAG;
	
	map<short, SupportNode>::iterator pos1;
	map<vector<short>, LegSupportNode>::iterator pos2;
	
	for ( pos1 = frequent1List.begin(); pos1 != frequent1List.end(); ++pos1 )
	{
		vString[2] = pos1->first;
		pff = new RFrequentTreeFamily(vString);
		pff->motherOccList = pos1->second.occList;
		sTriple[0] = vString[2]; //the first node of an edge
		pos2 = frequent2Leg.lower_bound(sTriple);
		while ( pos2 != frequent2Leg.end() )
		{
			if ( pos2->first[0] != sTriple[0] )
				break;
			else
			{
				pff->legs.push_back(pos2->second.thisLeg);
			}
			++pos2;
		}
		pff->hybrideExplore(frequency, outFile, database, frequent2Tree,
							headIndex, threshold, true);
		//pff->test();
		delete pff;
	}

}
