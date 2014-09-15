#ifndef _NODE_H_
#define _NODE_H_

#include <unordered_set>
#include <vector>

using namespace std;

class Node
{
public:
	Node(int val);
	~Node(void);

	const int id;
	struct hashNode
	{
		size_t operator() (Node* const node) const
		{
			return node->id;
		}
	};
	
	unordered_set<Node*, hashNode> vizinhos;

	bool operator == (Node const &other) { return id == other.id; }

	// recursão do algoritmo de Bron–Kerbosch
	static void algBronKerboschRec(unordered_set<Node*, hashNode> const &R, unordered_set<Node*, hashNode> &P, unordered_set<Node*, hashNode> &X,
									vector<unordered_set<int>*> &cliques);


	// recursão do algoritmo de Bron–Kerbosch com pivoting
	static void algBronKerboschRecPivoting(unordered_set<Node*, hashNode> const &R, unordered_set<Node*, hashNode> &P, unordered_set<Node*, hashNode> &X,
									vector<unordered_set<int>*> &cliques);
};

struct compNodes
{
	bool operator ()(Node* const first, Node* const second)
	{
		int diff = first->vizinhos.size() - second->vizinhos.size(); 
		if(diff != 0)
			return diff > 0;
		return first->id > second->id;
	}
};

#endif

