#include "ClusterUnidades.h"
#include "../Campus.h"
#include "HeuristicaNuno.h"
#include "../ProblemData.h"
#include "../ParametrosPlanejamento.h"
#include "../ConjUnidades.h"


ClusterUnidades::ClusterUnidades(Campus* const campus)
	: campus_(campus)
{
}

ClusterUnidades::~ClusterUnidades(void)
{
}

// criar clusters de unidades para o campus
void ClusterUnidades::criarClusters(void)
{
	vector<parCluster> allClusters;

	// analisar cada unidade
	for(auto itUnid = campus_->unidades.begin(); itUnid != campus_->unidades.end(); ++itUnid)
	{
		bool add = false;
		for(auto itClust = allClusters.begin(); itClust != allClusters.end(); ++itClust)
		{
			if(checkUnidade(*itUnid, *itClust))
			{
				add = true;
				break;
			}
		}
		if(!add)
			addUnidadeUnica(*itUnid, allClusters);
	}

	loadClusters(allClusters);
}

// add unidade como cluster unico
void ClusterUnidades::addUnidadeUnica(Unidade* const unidade, vector<parCluster> &clusters)
{
	// criar set cluster
	unordered_set<Unidade*> cluster;
	cluster.insert(unidade);

	// verificar distancias
	unordered_map<Unidade*, pair<int, int>> deslocamento;
	for(auto it = campus_->unidades.begin(); it != campus_->unidades.end(); ++it)
	{
		if((*it)->getId() == unidade->getId())
			continue;

		int ida = HeuristicaNuno::probData->calculaTempoEntreCampusUnidades(campus_, campus_, unidade, *it);
		int volta = HeuristicaNuno::probData->calculaTempoEntreCampusUnidades(campus_, campus_, *it, unidade);
		pair<int, int> desloc (ida, volta);
		deslocamento[*it] = desloc;
	}

	parCluster par (cluster, deslocamento);
	clusters.push_back(par);
}

// check unidade. true if added to cluster
bool ClusterUnidades::checkUnidade(Unidade* const unidade, parCluster &cluster)
{
	// verificar se tem deslocamento zero para todas as unidades
	for(auto it = cluster.first.begin(); it != cluster.first.end(); ++it)
	{
		if((*it)->getId() == unidade->getId())
			HeuristicaNuno::excepcao("ClusterUnidades::checkUnidade", "Unidade ja se encontra no cluster!");

		pair<int, int> parDesloc = ClusterUnidades::parDeslocUnidades(campus_, unidade, *it);
		if((parDesloc.first != 0) || (parDesloc.second != 0))
			return false;
	}

	// verificar se o deslocamento para as outras unidades é igual às unidades do cluster
	auto itUnid = cluster.second.end();
	for(auto itDesloc = cluster.second.begin(); itDesloc != cluster.second.end(); ++itDesloc)
	{
		if(itDesloc->first->getId() == unidade->getId())
		{
			itUnid = itDesloc;
			continue;
		}

		pair<int, int> parDesloc = ClusterUnidades::parDeslocUnidades(campus_, unidade, itDesloc->first);
		if((parDesloc.first != itDesloc->second.first) || (parDesloc.second != itDesloc->second.second))
			return false;
	}

	// adicionar a unidade ao cluster
	if(!cluster.first.insert(unidade).second)
		HeuristicaNuno::excepcao("ClusterUnidades::checkUnidade", "Unidade nao adicionada ao cluster");

	// apagar o deslocamento para esta unidade
	if(itUnid == cluster.second.end())
		HeuristicaNuno::excepcao("ClusterUnidades::checkUnidade", "Deslocamento para unidade inserida nao encontrado");

	int size = (int)cluster.second.size();
	cluster.second.erase(itUnid);
	if(size == cluster.second.size())
		HeuristicaNuno::excepcao("ClusterUnidades::checkUnidade", "Deslocamento para unidade inserida nao apagado");

	return true;
}

// carregar clusters para o objecto Campus
void ClusterUnidades::loadClusters(vector<parCluster> &clusters)
{
	campus_->clustersUnidades.clear();
	for(auto it = clusters.begin(); it != clusters.end(); ++it)
	{
		// get unidades ids
		unordered_set<int> unidsIds;
		getSetUnidadesId(it->first, unidsIds);

		// criar conj unidades
		ConjUnidades* const cluster = new ConjUnidades(campus_, unidsIds);
		campus_->clustersUnidades.push_back(cluster);
	}
}


// retorna par de tempo de deslocamento (ida, volta)
pair<int, int> ClusterUnidades::parDeslocUnidades(Campus* campus, Unidade* unidade, Unidade* outra)
{
	int ida = HeuristicaNuno::probData->calculaTempoEntreCampusUnidades(campus, campus, unidade, outra);
	int volta = HeuristicaNuno::probData->calculaTempoEntreCampusUnidades(campus, campus, outra, unidade);
	pair<int, int> desloc (ida, volta);

	return desloc;
}

// transforma set de unidades em set de ids
void ClusterUnidades::getSetUnidadesId(unordered_set<Unidade*> const &unids, unordered_set<int> &ids)
{
	for(auto it = unids.begin(); it != unids.end(); ++it)
	{
		ids.insert((*it)->getId());
	}
}