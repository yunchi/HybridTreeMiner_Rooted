GCC=g++ -O3
OBJS=RootedTreeMiner.o RFrequentTreeList.o RFrequentTreeFamily.o RBFCFTree.o RDFCFTree.o RFreeTree.o
HybridTreeMiner_Rooted: RootedTreeMiner.o RFrequentTreeList.o RFrequentTreeFamily.o RBFCFTree.o RDFCFTree.o RFreeTree.o
	${GCC} -o HybridTreeMiner_Rooted RootedTreeMiner.o RFrequentTreeList.o RFrequentTreeFamily.o RBFCFTree.o RDFCFTree.o RFreeTree.o;
	rm -f ${OBJS}
RootedTreeMiner.o: RootedTreeMiner.cpp RFrequentTreeList.h RFrequentTreeFamily.h RBFCFTree.h RDFCFTree.h RFreeTree.h Rmisc.h
	${GCC} -c RootedTreeMiner.cpp
RFrequentTreeList.o: RFrequentTreeList.cpp RFrequentTreeList.h RFrequentTreeFamily.h RBFCFTree.h RDFCFTree.h RFreeTree.h Rmisc.h
	${GCC} -c RFrequentTreeList.cpp 
RFrequentTreeFamily.o: RFrequentTreeFamily.cpp RFrequentTreeFamily.h RBFCFTree.h RFreeTree.h Rmisc.h
	${GCC} -c RFrequentTreeFamily.cpp
RDFCFTree.o: RDFCFTree.cpp RDFCFTree.h RFreeTree.h Rmisc.h
	${GCC} -c RDFCFTree.cpp
RBFCFTree.o: RBFCFTree.cpp RBFCFTree.h RFreeTree.h Rmisc.h
	${GCC} -c RBFCFTree.cpp
RFreeTree.o: RFreeTree.cpp RFreeTree.h Rmisc.h
	${GCC} -c RFreeTree.cpp
