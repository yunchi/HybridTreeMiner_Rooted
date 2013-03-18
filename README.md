HybridTreeMiner Rooted: an algorithm for mining frequent rooted unordered trees

Citation information:

[1] Yun Chi, Yirong Yang, and Richard R. Muntz. Hybridtreeminer: An efficient algorithm for mining frequent rooted trees and free trees using canonical form. In SSDBM'04: Proceedings of the 16th International Conference on and Statistical Database Management, pages 11-20, Santorini Island, Greece, 2004. IEEE Computer Society.

1) The input should be text file in the following format for each transaction:

tid_1

number_of_nodes

node1_label

...

noden_label

edge1_label connect_from_which_node connect_to_which_node

...

edge(n-1)_label connect_from_which_node connect_to_node

2) All transactions are put one after one in the text file. A sample database "example_rooted.txt" is given. Notice that the order of edges are given should be in such a way that the tree is always connected as edges are added one by one.

3) Usage: HybridTreeMiner_Rooted support input_file output_file
where support is an integer, i.e., minimal number of occurrence

4) Output file contains the running time, the number of frequent trees for each tree size.
