#include "Node.h"
#include "UtilHeur.h"


Node::Node(int val)
	: id(val)
{
}


Node::~Node(void)
{
}

// recursão do algoritmo de Bron–Kerbosch II
void Node::algBronKerboschRec(unordered_set<Node*, hashNode> const &R, unordered_set<Node*, hashNode> &P, unordered_set<Node*, hashNode> &X,
									vector<unordered_set<int>*> &cliques)
{
	// clique maximo encontrado
	if(P.size() == 0 && X.size() == 0)
	{
		if(R.size() >= 2)
		{
			unordered_set<int>* colNrs = new unordered_set<int>();
			//unordered_set<int> colNrs;
			for(auto it = R.begin(); it != R.end(); ++it)
				colNrs->insert((*it)->id);

			cliques.push_back(colNrs);
			//HeuristicaNuno::logMsgInt("nr combs: ", cliques.size(), 1);
		}
		return;
	}

	// iterar
	unordered_set<Node*, Node::hashNode> newR;
	unordered_set<Node*, Node::hashNode> newP;
	unordered_set<Node*, Node::hashNode> newX;
	// with pivoting
	for(unordered_set<Node*, Node::hashNode>::iterator it = P.begin(); it != P.end();)
	{
		// novo R
		newR = R;
		newR.insert(*it);
		// novo P
		UtilHeur::intersectSets<Node*, Node::hashNode>(P, (*it)->vizinhos, newP);
		// novo X
		UtilHeur::intersectSets<Node*, Node::hashNode>(X, (*it)->vizinhos, newX);
		
		// proximo passo da recursao
		//HeuristicaNuno::logMsg("inner it", 1);
		algBronKerboschRec(newR, newP, newX, cliques);

		//HeuristicaNuno::logMsg("inner erase P", 1);
		X.insert(*it);
		it = P.erase(it);
		//HeuristicaNuno::logMsg("done", 1);
	}
}

// recursão do algoritmo de Bron–Kerbosch III
void Node::algBronKerboschRecPivoting(unordered_set<Node*, Node::hashNode> const &R, unordered_set<Node*, Node::hashNode> &P, unordered_set<Node*, Node::hashNode> &X,
							vector<unordered_set<int>*> &cliques)
{
	// clique maximo encontrado
	if(P.size() == 0 && X.size() == 0)
	{
		if(R.size() >= 2)
		{
			unordered_set<int>* colNrs = new unordered_set<int>();
			//unordered_set<int> colNrs;
			for(auto it = R.begin(); it != R.end(); ++it)
				colNrs->insert((*it)->id);

			cliques.push_back(colNrs);
		}
		return;
	}

	// iterar
	unordered_set<Node*, Node::hashNode> newR;
	unordered_set<Node*, Node::hashNode> newP;
	unordered_set<Node*, Node::hashNode> newX;

	// with pivoting. get node u...
	unordered_set<Node*, Node::hashNode> unionPX;
	UtilHeur::unionSets<Node*, Node::hashNode>(P, X, unionPX);
	int rdIdx = rand() % unionPX.size();
	Node* const pivot = *std::next(unionPX.begin(), rdIdx);
	// get P \ N(u)
	unordered_set<Node*, Node::hashNode> pExcU = P;
	for(auto itU = pivot->vizinhos.cbegin(); itU != pivot->vizinhos.cend(); ++itU)
	{
		auto itFind = pExcU.find(*itU);
		if(itFind != pExcU.end())
			pExcU.erase(itFind);
	}

	for(auto it = pExcU.begin(); it != pExcU.end();)
	{
		// novo R
		newR = R;
		newR.insert(*it);
		// novo P
		UtilHeur::intersectSets<Node*, Node::hashNode>(P, (*it)->vizinhos, newP);
		// novo X
		UtilHeur::intersectSets<Node*, Node::hashNode>(X, (*it)->vizinhos, newX);
		
		// proximo passo da recursao
		//HeuristicaNuno::logMsg("inner it", 1);
		algBronKerboschRecPivoting(newR, newP, newX, cliques);

		//HeuristicaNuno::logMsg("inner erase P", 1);
		X.insert(*it);
		it = P.erase(it);
		//HeuristicaNuno::logMsg("done", 1);
	}
}