/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	Class: RBFCFTree

	Description: Used to store one tree in BFCF
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Rmisc.h"
#include "RFreeTree.h"
#include "RBFCFTree.h"

using namespace std;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	constructor

	Decription: Assuming that the number of vertices v and the tid t are given
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
RBFCFTree::RBFCFTree(short v, long t) :
  RFreeTree(v, t), level(v+1,0), automorphism(v+1,0)
{
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	constructor

	Decription: Assuming that only tid t is given
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
RBFCFTree::RBFCFTree(long t) :
  RFreeTree(t)
{
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	constructor

	Decription: Assuming that nothing is given
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
RBFCFTree::RBFCFTree() : 
  RFreeTree()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	constructor

	Decription: copy constructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
RBFCFTree::RBFCFTree(const RBFCFTree& rhs)
  : RFreeTree(rhs)
{
	level = rhs.level;
	automorphism = rhs.automorphism;
	canonicalString = rhs.canonicalString;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	constructor

	Decription: copy from the parent in the enumeration tree, plus
	a new leg
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
RBFCFTree::RBFCFTree(const RBFCFTree& parent, 
				   short newEdgeLabel, short newVertexLabel, short position)
{
	/******************************************************************
	idea: copy the tree structure of the parent, plus one new leg
	Note: use deep copy and preserve the original order of link list
	at the end, recompute the BFCS and automorphisms
	******************************************************************/
	vCount = parent.vCount + 1;
	tid = parent.tid;
	adj.resize(vCount + 1);
	TnodeLink t1, t2, t3;
	for ( short i = 1; i <= vCount - 1; i++ ) //copy the parent part here
	{
		t1 = parent.adj[i];
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
	vertexLabel = parent.vertexLabel;
	vertexLabel.push_back(newVertexLabel);
	degree = parent.degree;
	degree.push_back(0);
	level = parent.level;
	level.push_back(level[position]+1);
	insertEdge(Edge(position,vCount,newEdgeLabel));
	automorphism.resize(vCount+1);

	computeBFCS();
	computeAutomorphism();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	constructor

	Decription: construct from a canonical string
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
RBFCFTree::RBFCFTree(const vector<short>& cString) 
{
	tid = 0; //there is no tid information in this case
	short idx = 0;
	short vertexIndex;
	short parentIndex;
	
	vCount = cString[0];
	adj.resize(vCount+1, (TnodeLink) 0);
	vertexLabel.resize(vCount+1);
	degree.resize(vCount+1,0);
	
	level.resize(vCount+1);
	automorphism.resize(vCount+1);

	//for (short i = 0; i <= vCount; i++)
	//{
	//	adj[i] = 0;
	//	degree[i] = 0;
	//}

	vertexLabel[1] = cString[2];
	vertexIndex = 2;
	parentIndex = 1;
	idx = 4;

	if ( vCount == 1 ) 
	{
		level[1] = 1;
		automorphism[1] = 1;
		return;
	}

	while ( cString[idx] != ENDOFTREETAG )
	{
		if ( cString[idx] == ENDTAG )
		{
			parentIndex++;
			idx++;
			continue;
		}
		else
		{
			vertexLabel[vertexIndex] = cString[idx+1];
			insertEdge(Edge(vertexIndex,parentIndex,cString[idx]));
			vertexIndex++;
			idx+=2;
		}
	}

	computeBFCS();
	computeAutomorphism();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	destructor

	Decription: defaultly call the destructor of RFreeTree
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
RBFCFTree::~RBFCFTree()
{
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	operator=()

	Decription: assignment copy, same as copy constructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
RBFCFTree& RBFCFTree::operator=(const RBFCFTree& rhs)
{
	RFreeTree::operator =((RFreeTree)rhs); //is this the correct way?
	level = rhs.level;
	automorphism = rhs.automorphism;
	canonicalString = rhs.canonicalString;	
	return *this;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	computeBFCS()

	Decription: compute the canonicalString of the BFCF tree, assuming
	that the tree has been normalized.
	Note: vector level is a byproduct of this step
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RBFCFTree::computeBFCS()
{
	short i;
	canonicalString.clear();
	short tempCount = vCount;
	short currentLevel = 1; //the level of the root
	queue<short> tempQ;
	stack<short> tempK;
	TnodeLink t;
	canonicalString.push_back(vCount);
	canonicalString.push_back(0); //the edgeToParent of the root
	canonicalString.push_back(vertexLabel[1]);
	canonicalString.push_back(ENDTAG);

	level[1] = currentLevel;
	tempQ.push(1);
	tempQ.push(-1);
	tempCount--;
	while ( !tempQ.empty() && tempCount > 0 )
	{
		i = tempQ.front();
		tempQ.pop();
		if ( i == -1 )
		{
			currentLevel++;
			tempQ.push(-1);
			continue;
		}
		t = adj[i];
		if ( i == 1 )
		{
			while ( t != 0 )
			{
				tempK.push( t->v );
				tempK.push( t->eLabel );
				t = t->next;
			}
		}
		else
		{
			while ( t->next != 0 ) //other than the parent
			{
				tempK.push( t->v );
				tempK.push( t->eLabel );
				t = t->next;
			}
		}

		while ( !tempK.empty() )
		{
			canonicalString.push_back(tempK.top());
			tempK.pop();
			canonicalString.push_back(vertexLabel[tempK.top()]);
			tempQ.push(tempK.top());
			level[tempK.top()] = currentLevel + 1;
			tempK.pop();
			tempCount--;
		}
		if ( tempCount > 0 )
			canonicalString.push_back(ENDTAG);
	}
	canonicalString.push_back(ENDOFTREETAG);

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	computeAutomorphism()

	Decription: compute the automorphisms of the BFCF tree, assuming
	that the tree has been normalized, and the computeBFCF() has be called
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RBFCFTree::computeAutomorphism()
{
	vector<short> rank(vCount+1);
	vector<short> order(vCount+1);
	vector<short> parent(vCount+1);
	vector< vector<short> > codeString(vCount+1);

	TnodeLink t;
	short head, tail, i, j, k;
	
	for ( i = 0; i <= vCount; i++ ) order[i] = i; //initialize vector order

	//stage 1: upward stage
	tail = vCount;
	while ( level[tail] != 1 ) //not the root
	{
		head = tail;
		while ( level[head-1] == level[head] ) head--;
		
		//stage1_1: construct codeString for current level
		for ( i = head; i <= tail; i++ )
		{
			codeString[i].push_back(0); //temporary edgeToParent label
			codeString[i].push_back(vertexLabel[i]);
			t = adj[i];
			while ( t->next != 0 )
			{
				codeString[i].push_back(rank[t->v]);
				t = t->next;
			}
			codeString[i].push_back(ENDTAG);
			parent[i] = t->v;
			codeString[i][0] = t->eLabel; //put in correct edgeToParent label
		}

		//stage1_2: sort all vertices in current level to get order
		for ( i = head; i < tail; i++ )
		{
			for ( j = tail; j > head; j-- )
			{
				if ( codeString[order[j - 1]] > codeString[order[j]] )
				{
					k = order[j-1];
					order[j-1] = order[j];
					order[j] = k;
				}
			}
		}

		//stage1_3: scan current level to get rank
		k = 1; //current rank
		rank[order[head]] = k;
		for ( i = head + 1; i <= tail; i++ )
		{
			if ( codeString[order[i]] > codeString[order[i-1]] ) k++;
			rank[order[i]] = k;
		}
		tail = head - 1;
	}

	//stage 2: downward stage, compute automorphisms
	automorphism[1] = 1;
	head = 2;
	while ( head <= vCount )
	{
		tail = head;
		while ( (tail <= vCount) && (level[tail] == level[head]) ) tail++;
		tail--;
		//stage2_1: bubblesort again, refine the order, 
		//          taking automorphisms of parents into consideration
		for ( i = head; i < tail; i++ )
		{
			for ( j = tail; j > head; j-- )
			{
				if ( (rank[order[j - 1]] == rank[order[j]]) &&
					 (automorphism[parent[order[j-1]]] > automorphism[parent[order[j]]]) )
				{
					k = order[j-1];
					order[j-1] = order[j];
					order[j] = k;
				}
			}
		}

		//stage2_2: compute automorphims of current level 
		k = order[head]; //current automorphism
		automorphism[order[head]] = k;
		for ( i = head + 1; i <= tail; i++ )
		{
			if ( !((rank[order[i - 1]] == rank[order[i]]) &&
					 (automorphism[parent[order[i-1]]] == automorphism[parent[order[i]]])) )
				k = order[i];
			automorphism[order[i]] = k;
		}

		head = tail + 1;
	}
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	possibleLegs()

	Decription: for a location i, compute the range of the possible legs
	Note: (0,0) means no restriction, (MAX_EDGE+1,MAX_VERTEX+1) means impossible
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RBFCFTree::possibleLegs(const short i, short& minEdge, short& minVertex )
{
	if ( automorphism[i] != i ) //no leg possible
	{
		minEdge = MAX_EDGE + 1;
		minVertex = MAX_VERTEX + 1;
		return;
	}

	minEdge = MIN_EDGE;
	minVertex = MIN_VERTEX;
	//first, check the position itself
	if ( (level[i] != 1 && degree[i] != 1) ||
			(level[i] == 1 && degree[i] != 0) ) 
	//degree[i] is 1 and i is not the root, it's a leaf, no restriction at this level
	{
		minEdge = adj[i]->eLabel; //the last leg
		minVertex = vertexLabel[adj[i]->v]; //the last leg
	}
	
	//second, check all above ancestors
	short j = i;
	short p, q;
	bool possible = true;
	while ( possible && level[j] != 1 )
	{
		//find the parent of j
		TnodeLink t = adj[j];
		while ( t->next != 0 ) t = t->next;
		p = t->v; //p is the parent of j;
		if ( level[p] != 1 ) //if p is not the root
		{
			if ( degree[p] <= 2 ) //if j has no sibling, nothing to do
			{
				j = p;
				continue;
			}
		}

		//find the sibling to the left of j
		t = adj[p];
		while ( t->v != j ) t = t->next; //find the correct sibling
		
		if ( (level[p] == 1 && t->next == 0) || 
				(level[p] != 1 && t->next->next == 0) ) //if j is the left most sibling
		{
			j = p;
			continue;
		}

		//now t points to j, t->next points to the left sibling of j
		if ( t->eLabel != t->next->eLabel || 
				vertexLabel[t->next->v] != vertexLabel[j] )
		{
			j = p;
			continue;
		}

		//find the restriction on the new leg by current sibling
		queue<short> vertexQ1, vertexQ2;
		stack<short> bufferK, labelK1, labelK2;
		vertexQ1.push(t->next->v); //compare two subtrees, then
		vertexQ2.push(j);          //neither cannot be a root
		short tempE1, tempE2, tempV1, tempV2;
		
		vertexQ1.push(ENDTAG);
		vertexQ2.push(ENDTAG);

		while ( true )
		{
			tempV1 = vertexQ1.front();
			vertexQ1.pop();
			tempV2 = vertexQ2.front();
			vertexQ2.pop();

			if ( tempV2 == ENDTAG )
			{
				if ( tempV1 != ENDTAG )
				{
					j = p;
					break;
				}
				else
					continue;
			}

			t = adj[tempV1];
			while ( t->next != 0 )
			{
				bufferK.push(t->v);
				labelK1.push(vertexLabel[t->v]);
				labelK1.push(t->eLabel);
				t = t->next;
			}
			while ( !bufferK.empty() )
			{
				vertexQ1.push(bufferK.top());
				bufferK.pop();
			}

			t = adj[tempV2];
			while ( t->next != 0 )
			{
				bufferK.push(t->v);
				labelK2.push(vertexLabel[t->v]);
				labelK2.push(t->eLabel);
				t = t->next;
			}
			while ( !bufferK.empty() )
			{
				vertexQ2.push(bufferK.top());
				bufferK.pop();
			}

			if ( tempV2 != i ) //if not reach the position for new leg yet
								//then the two stack either the same (then
								//continue), or stack1 is smaller, then no
								//need to continue
			{
				bool result = false;
				while ( !labelK1.empty() ) //if labelK1 is empty, so is labelK2
				{
					if ( labelK2.empty() )
					{
						result = true;
						break;
					}
					if ( labelK1.top() < labelK2.top() ) //if the edges resolve 
						//the order among the two subtrees...
						//note: it's impossible that labelK1.top > labelK2.top
					{
						result = true;
						break;
					}
					labelK1.pop();
					labelK2.pop();
					if ( labelK1.top() < labelK2.top() ) //if the vertices resolve
						//the order among the two subtrees...
					{
						result = true;
						break;
					}
					labelK1.pop();
					labelK2.pop();
				}
				if ( result )
				{
					j = p;
					break; //break the while(true) loop
				}
			}
			else //if now we are comparing something to the new position
			{
				if ( labelK1.empty() ) //no child, cannot add leg at all
				{
					possible = false;
					break;
				}
				else
				{
					while ( !labelK1.empty() )
					{
						tempE1 = labelK1.top();
						labelK1.pop();
						tempV1 = labelK1.top();
						labelK1.pop();
						if ( labelK2.empty() )
						{
							if ( tempE1 > minEdge ||
								( tempE1 == minEdge && tempV1 > minVertex) )
							{
								minEdge = tempE1;
								minVertex = tempV1;
							}
							break;
						}
						else
						{
							tempE2 = labelK2.top();
							labelK2.pop();
							tempV2 = labelK2.top();
							labelK2.pop();
							if ( tempE1 != tempE2 || tempV1 != tempV2 )
								break;
						}
					}
					j = p;
					break;//break the while(true) loop
				}
			}//end of if()else()
		}//end of while(true)
	}//end of while(possible...)

	if ( !possible )
	{
		minEdge = MAX_EDGE + 1;
		minVertex = MAX_VERTEX + 1;
	}
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	extensionExplore()

	Decription: find all possible children in the enumeration tree that
	are frequent. Using extension operation only.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RBFCFTree::extensionExplore( vector<long>&frequency,
						ostream & outFile,
						const vector<ptrRFreeTree>& database,
					    const vector<Occurrence> & occList,
					   	const vector< vector<short> > & frequent2Tree,
						const vector<long> & headIndex,
						const long & threshold )
{
	short firstPos, lastPos, child1, child2;
	TnodeLink t;

	/******************************************************************
	step0: output this tree
	******************************************************************/	
	frequency[vCount]++;

	/******************************************************************
	step1: find the locations where a new leg can grow
	******************************************************************/	
	firstPos = adj[vCount]->v; //the parent of the last leg
	lastPos = vCount;

	/******************************************************************
	step2: main loop, for each position, explore
	******************************************************************/	
	for ( short i = firstPos; i <= lastPos; i++ )
	{
		//step2_1: get the range of valid legs
		short minEdge;
		short minVertex;
		possibleLegs(i, minEdge, minVertex);

		if ( minEdge > MAX_EDGE ) continue; //continue the for loop

		if ( headIndex[vertexLabel[i] - MIN_VERTEX] == 0 ) continue;

		//step2_2: get the possible frequent legs
		vector<bool> isFrequent( (MAX_EDGE - MIN_EDGE + 1) 
								*(MAX_VERTEX - MIN_VERTEX + 1), false);
		for (short j = headIndex[vertexLabel[i] - MIN_VERTEX]; 
			  (j < frequent2Tree.size() && frequent2Tree[j][0] == vertexLabel[i]); j++ )
			isFrequent[( frequent2Tree[j][1] - MIN_EDGE ) * ( MAX_VERTEX - MIN_VERTEX + 1 )
				+ ( frequent2Tree[j][2] - MIN_VERTEX )] = true;


		//step2_3: explore each potential leg
		set<short> neighbors;
		set<short>::iterator pos;
		Occurrence tempOcc;
		vector<SupportNode> potential((MAX_EDGE - MIN_EDGE + 1) 
									  *(MAX_VERTEX - MIN_VERTEX + 1));

		for ( long s = 0; s < occList.size(); s++ )
		{
			neighbors.clear();
			t = adj[i];
			while ( t != 0 ) //insert index of all neighbors of the position i
			{
				neighbors.insert(occList[s].nodeIndex[t->v - 1]);//inconsistency on index
				t = t->next;
			}
			t = database[occList[s].tid]->adj[occList[s].nodeIndex[i-1]];
			while ( t != 0 )
			{
				if ( occList[s].nodeIndex[i-1] < t->v )
				{
					pos = neighbors.find( t->v );
					if ( pos == neighbors.end() ) //if the vertex hasn't been used
					{
						short tempE = t->eLabel;
						short tempV = database[occList[s].tid]->vertexLabel[t->v];
						short location = ( tempE - MIN_EDGE ) * ( MAX_VERTEX - MIN_VERTEX + 1 ) 
							+ ( tempV - MIN_VERTEX ); 
						if ( ((tempE > minEdge) || (tempE == minEdge && tempV >= minVertex)) &&
							isFrequent[location] ) //if the leg is potentially frequent
						{
							tempOcc = occList[s];
							tempOcc.nodeIndex.push_back(t->v);
							potential[location].occList.push_back(tempOcc);
							if ( tempOcc.tid != potential[location].lastTid )
							{
								potential[location].lastTid = tempOcc.tid;
								potential[location].support++;
							}
						}
					}
				}
				t = t->next;
			}//end of while ( t != 0 )
		}//end of for ( s = 0; ...)

		for ( long s = 0; s < potential.size(); s++ )
		{
			if ( potential[s].support >= threshold )
			{
				short tempE = MIN_EDGE + (short)(floor(s/(MAX_VERTEX - MIN_VERTEX + 1)));
				short tempV = MIN_VERTEX + (s % (MAX_VERTEX - MIN_VERTEX + 1));
				RBFCFTree *pbfcf = new RBFCFTree(*this,tempE,tempV,i);
				pbfcf->extensionExplore(frequency, outFile, database,potential[s].occList,
							   frequent2Tree,headIndex,threshold);
				delete pbfcf;
			}
		}

		////test
		//cout << "leg position is: " << i << " vertex label is: " << vertexLabel[i] << endl;
		//cout << "min edge and min vertex are: " << minEdge << ' ' << minVertex << endl;
		//for ( j = 0; j < isFrequent.size(); j++ )
		//	cout << isFrequent[j] << ' ';
		//cout << endl;
		//cout << endl;

	}//end of for(short i = ...)
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	operator<()

	Decription: compare two trees in BFCF
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool RBFCFTree::operator<(const RBFCFTree& rhs) const
{
	return canonicalString < rhs.canonicalString;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	operator>>()

	Decription: read in BFCF trees
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
istream& operator>>(istream& in, RBFCFTree& rhs)
{
  long t;	//tid
  short v;	//number of vertices
  short p1, p2;
  short vLabel, eLabel;

  in >> t;
  in >> v;

  rhs.adj.resize(v+1);
  rhs.vertexLabel.resize(v+1);
  rhs.degree.resize(v+1);

  rhs.level.resize(v+1);
  rhs.automorphism.resize(v+1);

  for (short i = 0; i <= v; i++)
  {
	rhs.adj[i] = 0;
	rhs.degree[i] = 0;
  }

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

	Decription: write out BFCF trees
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
ostream& operator<<(ostream& out, const RBFCFTree& rhs)
{
  out << "Tid: " << rhs.tid << endl;
  out << "Vertices Number: " << rhs.vCount << endl;
  for ( short i = 1; i <= rhs.vCount; i++ )
    {
		out << rhs.vertexLabel[i] << ' ' 
		<< rhs.degree[i] << ':';
		TnodeLink n = rhs.adj[i];
		while ( n != 0 ) 
		{
			out << "--" << n->v << '_' << n->eLabel << ' ';
			n = n->next;
		}
		out << endl;
    }
  out << "automorphism: ";
  for ( short i = 1; i <= rhs.vCount; i++ ) out << rhs.automorphism[i] << ' ';
  out << endl;
  out << "canonical string: ";
  for ( short i = 0; i < rhs.canonicalString.size(); i++ ) out << rhs.canonicalString[i] << ' ';
  out << endl;
  return out;
}  


