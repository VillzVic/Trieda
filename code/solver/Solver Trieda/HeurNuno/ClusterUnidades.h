#ifndef _CLUSTER_UNID_H_
#define _CLUSTER_UNID_H_

#include "UtilHeur.h"
#include "../Campus.h"
#include "../Unidade.h"

using std::pair;

// conjuntos de unidades e deslocamento para outras unidades.
typedef std::pair<unordered_set<Unidade*>, unordered_map<Unidade*, pair<int /*ida*/, int /*volta*/>>> parCluster;

class ClusterUnidades
{
public:
	ClusterUnidades(Campus* const campus);
	~ClusterUnidades(void);

	// criar clusters de unidades para o campus
	void criarClusters(void);

private:
	Campus* const campus_;

	// add unidade como cluster unico
	void addUnidadeUnica(Unidade* const unidade, vector<parCluster> &clusters);
	// check unidade. true if added to cluster
	bool checkUnidade(Unidade* const unidade, parCluster &cluster);

	// carregar clusters para o objecto Campus
	void loadClusters(vector<parCluster> &clusters);

	// verifica se duas unidades são compatíveis. Para tal têm que:
	//
	// 1. Ter deslocação zero entre elas.
	// 2. Ter uma deslocação igual para todas as outras unidades do campus.
	// 3. [OFF] considerar deslocamento para unidades de outros campi ??
	static bool unidsCompativeis(Unidade* unidUm, Unidade* unidDois);
	
	// retorna par de tempo de deslocamento (ida, volta)
	static pair<int, int> parDeslocUnidades(Campus* campus, Unidade* unidade, Unidade* outra);

	// transforma set de unidades em set de ids
	static void getSetUnidadesId(unordered_set<Unidade*> const &unids, unordered_set<int> &ids);
};

#endif