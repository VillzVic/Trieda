#include "MIPAloc.h"
//#include "opt_lp.h"
#include "opt_row.h"
#include "ParametrosHeuristica.h"
#include "DadosHeuristica.h"
#include "HeuristicaNuno.h"
#include "../ProblemData.h"
#include "../ParametrosPlanejamento.h"
#include "OfertaDisciplina.h"
#include "SolucaoHeur.h"
#include "TurmasIncHorarioDia.h"
#include "../CentroDados.h"

MIPAloc::MIPAloc(int tipoMip, std::string nome, SolucaoHeur* const solucao)
	:  tipoMIP_(tipoMip), nome_(nome), solucao_(solucao), lp_(nullptr), solFinal_(nullptr), nrVars_(0)
{
}

MIPAloc::~MIPAloc(void)
{
	HeuristicaNuno::logMsg("free lp", 1);
	if(!lp_->freeProb())
		HeuristicaNuno::warning("MIPAloc::~MIPAloc", "LP nao apagado com sucesso!!");
	HeuristicaNuno::logMsg("done. free lp", 1);
	delete lp_;

	lp_ = nullptr;
	delete[] solFinal_;
}

//---------------------- MAIN---------------------------------

// alocar
void MIPAloc::alocar(void)
{
	HeuristicaNuno::excepcao("MIPAloc::alocar", "Método tem que ser implementado numa classe descendente");
}
// Construir o modelo
void MIPAloc::buildLP_(void)
{
	HeuristicaNuno::logMsg("new lp...", 1);

	#ifdef SOLVER_CPLEX 
		lp_ = new OPT_CPLEX;
	#endif
	#ifdef SOLVER_GUROBI 
		lp_ = new OPT_GUROBI;
	#endif


	HeuristicaNuno::logMsg("criar lp...", 1);
	// objectivo: maximizar valor de demanda atendida ou maximizar profs reais
	
	char* name = UtilHeur::stringToChar(nome_);
	lp_->createLP(name, OPTSENSE::OPTSENSE_MAXIMIZE, PROB_MIP);
	delete[] name;
	HeuristicaNuno::logMsg("lp criado.", 1);

	// adicionar variaveis e restrições
	HeuristicaNuno::logMsg("criar variaveis...", 1);
	criarVariaveis_();
	lp_->updateLP();
	HeuristicaNuno::logMsg("criar restrições...", 1);
	criarRestricoes_();
	lp_->updateLP();
	HeuristicaNuno::logMsg("criadas.", 1);

	// imprime o modelo
	if (CentroDados::getPrintLogs())
	{
		lp_->writeProbLP(nome_.c_str());
	}

	std::stringstream ss;
	ss << "modelo tem " << lp_->getNumCols() << " variaveis e " << lp_->getNumRows() << " restrições";
	std::string msg = ss.str();

	HeuristicaNuno::logMsg(msg, 1);
}
// Set parâmetros resolvedor matemático
void MIPAloc::setParametrosLP_(void)
{
	// parametros gerais

	// set time limit
	if(ParametrosHeuristica::timeLimitMIP > 0)
		lp_->setTimeLimit(ParametrosHeuristica::timeLimitMIP);

	// enfase heuristica
	if(ParametrosHeuristica::heurFreqMIP > 0 && ParametrosHeuristica::heurFreqMIP <= 1)
		lp_->setHeurFrequency(ParametrosHeuristica::heurFreqMIP);

	// set log file
	stringstream ss;
	ss << "log_" << nome_ << ".log";
	char* cstr = UtilHeur::stringToChar(ss.str());

	// criar arquivo para limpar
	std::ofstream logMip (cstr);
	logMip.clear();

	lp_->setLogFile(cstr);
	delete [] cstr;
	//HeuristicaNuno::logMsg("", 1);
	//lp_->setLogFile("heuristica.log");
	//HeuristicaNuno::logMsg("", 1);

	// set MIP gap tolerance
	if(!lp_->setMIPRelTol(ParametrosHeuristica::realocarMipGapTolerance))
		HeuristicaNuno::logMsg("[WARN] Tolerância de MIP não foi fixada!", 1);
}
// Resolver problema
bool MIPAloc::solveLP_(void)
{
	// time start
	int start = (int)time(0);
	// solve
	OPTSTAT status = lp_->optimize(METHOD::METHOD_MIP);
	// time end
	int elapsed = (int)time(0) - start;

	std::stringstream ss;
	ss << "Optimização do modelo " << nome_ << " demorou " << elapsed << " segundos. Status = " << status;
	std::string msg = ss.str();

	HeuristicaNuno::logMsg(msg, 1);

	// se houver solução, carrega-la
	if(status == OPTSTAT::OPTSTAT_MIPOPTIMAL || status == OPTSTAT::OPTSTAT_FEASIBLE)
	{
		if (CentroDados::getPrintLogs())
		{
			lp_->writeSol(nome_.c_str());
		}

		return true;
	}
	return false;
}
// Carregar solução
void MIPAloc::carregarSolucao(void)
{
	HeuristicaNuno::excepcao("MIPAloc::carregarSolucao", "Método tem que ser implementado numa classe descendente");
}
// criar variáveis do modelo
void MIPAloc::criarVariaveis_(void)
{
	HeuristicaNuno::excepcao("MIPAloc::criarVariaveis_", "Método tem que ser implementado numa classe descendente");
}

//------------------------------------------------------------------

//---------------------- RESTRIÇÕES ---------------------------------


// criar restrições do modelo
void MIPAloc::criarRestricoes_(void)
{
	HeuristicaNuno::excepcao("MIPAloc::criarRestricoes_", "Método tem que ser implementado numa classe descendente");
}

// NOVO: criar turmas incompativeis recorrendo à geração de cliques de incompatibilidade!
void MIPAloc::criarRestrTurmasIncompatCliques_(unordered_map<TurmaHeur*, int> const &turmas, bool deslocavel,
														bool considerarMesmaTurmaOft)
{
	//HeuristicaNuno::logMsgInt("nr turmas assoc: ", turmas.size(), 1);
	if(turmas.size() < 2)
		return;

	// gerar cliques
	vector<unordered_set<int>*> cliques;
	gerarMaxCliques(turmas, deslocavel, cliques, considerarMesmaTurmaOft);

	// adicionar restricoes
	int nrRest = 0;
	for(auto it = cliques.cbegin(); it != cliques.end(); )
	{
		unordered_set<int>* clique = *it;
		// so vale a pena se tiver 2 turmas
		if(clique->size() >= 2)
		{
			OPT_ROW row (OPT_ROW::ROWSENSE::LESS, 1.0);
			for(auto itCol = clique->begin(); itCol != clique->end(); ++itCol)
				row.insert(*itCol, 1.0);

			addRow_(row);
			nrRest++;
		}
		it = cliques.erase(it);
		delete clique;
	}
	//if(nrRest > 0)
	//	HeuristicaNuno::logMsgInt("nr restricoes incompat: ", nrRest, 1);
}
// NOVO: criar turmas incompativeis olhando para os horarios!
void MIPAloc::criarRestrTurmasIncompatHorarios_(unordered_map<TurmaHeur*, int> const &turmas, bool deslocavel, bool interjornada)
{
	//HeuristicaNuno::logMsgInt("nr turmas: ", turmas.size(), 1);
	vector<unordered_set<int>> turmasInc;
	getTurmasIncHorarios(turmas, turmasInc, deslocavel, interjornada);

	// adicionar restrições
	int nrRows = 0;
	for(auto it = turmasInc.begin(); it != turmasInc.end(); ++it)
	{
		if(it->size() >= 2)
		{
			OPT_ROW row (OPT_ROW::ROWSENSE::LESS, 1.0);
			for(auto itCol = it->begin(); itCol != it->end(); ++itCol)
				row.insert(*itCol, 1.0);

			addRow_(row);
			nrRows++;
		}
	}

	//if(nrRows > 0)
	//	HeuristicaNuno::logMsgInt("nr rest incompat: ", nrRows, 1);
}

//------------------------------------------------------------------


//---------------------- UTIL ---------------------------------

// adicionar variável binária. retorna número da coluna.
int MIPAloc::addBinaryVarLP_(double coef, const char* nome, int lb, int ub)
{
	// guardar número de coluna antes pois indexação começa em zero
	int colNr = nrVars_;

	OPT_COL col (OPT_COL::VARTYPE::VAR_BINARY, coef, lb, ub, (char*) nome);
	if(!lp_->addCol(col))
		HeuristicaNuno::excepcao("MIPAloc::addBinaryVarLP_", "Variavel não adicionada ao LP.");
	else
		nrVars_++;

	return colNr;
}
// adicionar variavel inteira ao modelo. retorna número da coluna.
int MIPAloc::addIntVarLP_(double coef, int lb, int ub, const char* nome)
{
	// guardar número de coluna antes pois indexação começa em zero
	int colNr = nrVars_;

	OPT_COL col (OPT_COL::VARTYPE::VAR_INTEGRAL, coef, lb, ub, (char*) nome);
	if(!lp_->addCol(col))
		HeuristicaNuno::excepcao("MIPAloc::addBinaryVarLP_", "Variavel não adicionada ao LP.");
	else
		nrVars_++;

	return colNr;
}
// adicionar variavel linear ao modelo. retorna numero da coluna.
int MIPAloc::addLinVarLP_(double coef, int ub, char* nome)
{
	int colNr = nrVars_;

	OPT_COL col (OPT_COL::VARTYPE::VAR_CONTINUOUS, coef, 0, ub, nome);
	if(!lp_->addCol(col))
		HeuristicaNuno::excepcao("MIPAloc::addBinaryVarLP_", "Variavel não adicionada ao LP.");
	else
		nrVars_++;

	return colNr;
}

// adicionar restrição
void MIPAloc::addRow_(OPT_ROW &row)
{
	// sem nonzero elements
	if(row.getnnz() == 0)
	{
		//HeuristicaNuno::warning("MIPAloc::addRow_", "row sem elementos");
		return;
	}

	if(!lp_->addRow(row))
	{
		//HeuristicaNuno::excepcao("MIPAloc::addRow_", "Restrição não adicionada ao LP.");
		HeuristicaNuno::warning("MIPAloc::addRow_", "Restrição não adicionada ao LP.");
		int* indices = row.getMatInd();
		for(int i = 0; i< row.getnnz(); ++i)
		{
			int idx = indices[i];
			if(idx < 0 || idx >= lp_->getNumCols())
				HeuristicaNuno::logMsgInt("bad idx: ", idx, 1);
		}
	}
}

// verifica se duas turmas são incompatíveis
bool MIPAloc::saoIncompativeis_(TurmaHeur* const turmaUm, TurmaHeur* const turmaDois, bool deslocavel)
{
	int comp = compatibilidadeJaRegistada_(turmaUm, turmaDois, deslocavel);
	if(comp < 0)
		return true;
	else if(comp > 0)
		return false;

	
	// estruturas para registar depois
	size_t hashUm = hasher<TurmaHeur>()(turmaUm);
	size_t hashDois = hasher<TurmaHeur>()(turmaDois);
	TurmaHeur* const primTurma = hashUm < hashDois ? turmaUm : turmaDois;
	TurmaHeur* const secTurma = hashUm < hashDois ? turmaDois : turmaUm;

	// verificar se é incompativel
	if(turmaUm->ehIncompativel(turmaDois, deslocavel))
	{
		registarIncompatibilidade_(primTurma, secTurma, deslocavel);
		return true;
	}

	// inserir par de turmas compativeis
	registarCompatibilidade_(primTurma, secTurma, deslocavel);

	return false;
}
// turmas incompativeis por disciplina equivalente?
bool MIPAloc::turmasOfertaIncompativel_(TurmaHeur* const primTurma, TurmaHeur* const secTurma)
{
	bool ofDiscIgual = (*primTurma->ofertaDisc) == (*secTurma->ofertaDisc);
	if(ofDiscIgual && (primTurma->tipoAula == secTurma->tipoAula))
		return true;

	Disciplina* primDisc = primTurma->ofertaDisc->getDisciplina();
	Disciplina* secDisc = secTurma->ofertaDisc->getDisciplina();

	// mesma disciplina mas ofertas diferentes
	if((primDisc->getId() == secDisc->getId()) && !ofDiscIgual)
		return true;

	// verificar se são substitutas
	/*if((primDisc->discEquivSubstitutas.find(secDisc) != primDisc->discEquivSubstitutas.end()) ||
		(secDisc->discEquivSubstitutas.find(primDisc) != secDisc->discEquivSubstitutas.end()) )
		return true;*/
		
	return false;
}
// verifica se incompatibilidade ja foi registada. retorna -1 se incompatível, 1 se compatível, 0 se não souber
int MIPAloc::compatibilidadeJaRegistada_(TurmaHeur* const turmaUm, TurmaHeur* const turmaDois, bool deslocavel)
{
	if(deslocavel)
		return compDeslocJaReg_(turmaUm, turmaDois);
	else
		return compHorsJaReg_(turmaUm, turmaDois);
}
int MIPAloc::compHorsJaReg_(TurmaHeur* const turmaUm, TurmaHeur* const turmaDois)
{
	size_t hashUm = hasher<TurmaHeur>()(turmaUm);
	size_t hashDois = hasher<TurmaHeur>()(turmaDois);

	TurmaHeur* const primTurma = hashUm < hashDois ? turmaUm : turmaDois;
	TurmaHeur* const secTurma = hashUm < hashDois ? turmaDois : turmaUm;

	// ver incompatibilidade com chave turmaUm
	auto itTurmaInc = turmasIncompHorarios_.find(primTurma);
	if(itTurmaInc != turmasIncompHorarios_.end())
	{
		if(itTurmaInc->second.find(secTurma) != itTurmaInc->second.end())
			return -1;
	}
	// ver compatibilidade com chave turmaDois
	auto itTurmaComp = turmasCompatHorarios_.find(primTurma);
	if(itTurmaComp != turmasCompatHorarios_.end())
	{
		if(itTurmaComp->second.find(secTurma) != itTurmaComp->second.end())
			return 1;
	}

	return 0;
}
int MIPAloc::compDeslocJaReg_(TurmaHeur* const turmaUm, TurmaHeur* const turmaDois)
{
	size_t hashUm = hasher<TurmaHeur>()(turmaUm);
	size_t hashDois = hasher<TurmaHeur>()(turmaDois);

	TurmaHeur* const primTurma = hashUm < hashDois ? turmaUm : turmaDois;
	TurmaHeur* const secTurma = hashUm < hashDois ? turmaDois : turmaUm;

	// ver incompatibilidade com chave turmaUm
	auto itTurmaInc = turmasIncompativeis_.find(primTurma);
	if(itTurmaInc != turmasIncompativeis_.end())
	{
		if(itTurmaInc->second.find(secTurma) != itTurmaInc->second.end())
			return -1;
	}
	// ver compatibilidade com chave turmaDois
	auto itTurmaComp = turmasCompativeis_.find(primTurma);
	if(itTurmaComp != turmasCompativeis_.end())
	{
		if(itTurmaComp->second.find(secTurma) != itTurmaComp->second.end())
			return 1;
	}

	return 0;
}
// registar compatibilidade
void MIPAloc::registarCompatibilidade_(TurmaHeur* const primTurma, TurmaHeur* const secTurma, bool deslocavel)
{
	if(deslocavel)
		registarCompDesloc_(primTurma, secTurma);
	else
		registarCompHors_(primTurma, secTurma);
}
void MIPAloc::registarCompHors_(TurmaHeur* const primTurma, TurmaHeur* const secTurma)
{
	auto itTurmaComp = turmasCompatHorarios_.find(primTurma);
	if(itTurmaComp == turmasCompatHorarios_.end())
	{
		unordered_set<TurmaHeur*> set;
		std::pair<TurmaHeur*, unordered_set<TurmaHeur*>> par (primTurma, set);
		itTurmaComp = turmasCompatHorarios_.insert(par).first;
	}
	itTurmaComp->second.insert(secTurma);
}
void MIPAloc::registarCompDesloc_(TurmaHeur* const primTurma, TurmaHeur* const secTurma)
{
	auto itTurmaComp = turmasCompativeis_.find(primTurma);
	if(itTurmaComp == turmasCompativeis_.end())
	{
		unordered_set<TurmaHeur*> set;
		std::pair<TurmaHeur*, unordered_set<TurmaHeur*>> par (primTurma, set);
		itTurmaComp = turmasCompativeis_.insert(par).first;
	}
	itTurmaComp->second.insert(secTurma);
}
// registar incompatibilidade
void MIPAloc::registarIncompatibilidade_(TurmaHeur* const primTurma, TurmaHeur* const secTurma, bool deslocavel)
{
	if(deslocavel)
		registarIncompDesloc_(primTurma, secTurma);
	else
		registarIncompHors_(primTurma, secTurma);
}
void MIPAloc::registarIncompHors_(TurmaHeur* const primTurma, TurmaHeur* const secTurma)
{
	auto itTurmasInc = turmasIncompHorarios_.find(primTurma);
	if(itTurmasInc == turmasIncompHorarios_.end())
	{
		unordered_set<TurmaHeur*> set;
		std::pair<TurmaHeur*, unordered_set<TurmaHeur*>> pair (primTurma, set);
		itTurmasInc = turmasIncompHorarios_.insert(pair).first;
	}
	itTurmasInc->second.insert(secTurma);
}
void MIPAloc::registarIncompDesloc_(TurmaHeur* const primTurma, TurmaHeur* const secTurma)
{
	// inserir par de turmas incompativeis
	auto itTurmasInc = turmasIncompativeis_.find(primTurma);
	if(itTurmasInc == turmasIncompativeis_.end())
	{
		unordered_set<TurmaHeur*> set;
		std::pair<TurmaHeur*, unordered_set<TurmaHeur*>> pair (primTurma, set);
		itTurmasInc = turmasIncompativeis_.insert(pair).first;
	}
	itTurmasInc->second.insert(secTurma);
}

//------------------------------------------------------------------

// ------------------------------------- CALCULADOR DE CLIQUES -------------------------------------------------------------

//  calcular cliques maximos de incompatibilidade com base nas turmas em questão
void MIPAloc::gerarMaxCliques(unordered_map<TurmaHeur*, int> const &turmasCols, bool deslocavel, vector<unordered_set<int>*> &cliques,
								bool considerarMmTipoOft)
{
	//HeuristicaNuno::logMsg("gerarMaxCliques...", 1);
	// criar nodes e registar incompatibilidades
	if(turmasCols.size() < 2)
		return;

	set<Node*, compNodes> nodesOrd;
	criarNodesTurmas(turmasCols, deslocavel, nodesOrd, considerarMmTipoOft);

	// preparar recursão
	unordered_set<Node*, Node::hashNode> R;
	unordered_set<Node*, Node::hashNode> P;
	unordered_set<Node*, Node::hashNode> X;
	for(auto it = nodesOrd.begin(); it != nodesOrd.end(); ++it)
		if(!P.insert(*it).second)
			HeuristicaNuno::excepcao("MIPAloc::gerarMaxCliques", "Node nao adicionado a P");

	// algoritmo de Bron–Kerbosch III
	unordered_set<Node*, Node::hashNode> newR;
	unordered_set<Node*, Node::hashNode> newP;
	unordered_set<Node*, Node::hashNode> newX;
	for(auto it = nodesOrd.begin(); it != nodesOrd.end(); ++it)
	{
		//HeuristicaNuno::logMsg("it node", 1);
		newR.clear();
		newR.insert(*it);

		// novo P
		UtilHeur::intersectSets<Node*, Node::hashNode>(P, (*it)->vizinhos, newP);
		// novo X
		UtilHeur::intersectSets<Node*, Node::hashNode>(X, (*it)->vizinhos, newX);

		// proximo passo da recursao
		Node::algBronKerboschRec(newR, newP, newX, cliques);
		// TESTAR COM PIVOTING
		//algBronKerboschRecPivoting(newR, newP, newX, cliques);

		auto itP = P.find(*it);
		if(itP != P.end())
			P.erase(itP);
		else
			HeuristicaNuno::excepcao("MIPAloc::gerarMaxCliques", "node nao deletado de P");
		X.insert(*it);
	}

	// libertar memoria dos nodes
	for(auto it = nodesOrd.begin(); it != nodesOrd.end(); )
	{
		Node* node = *it;
		it = nodesOrd.erase(it);
		delete node;
	}

	//HeuristicaNuno::logMsg("out. gerarMaxCliques", 1);
}

// criar nodes turmas (ordem crescente de grau)
void MIPAloc::criarNodesTurmas(unordered_map<TurmaHeur*, int> const &turmasCols, bool deslocavel, set<Node*, compNodes> &nodes,
								bool considerarMmTipoOft)
{
	unordered_map<int, Node*> turmasNode;	// colNr -> node
	for(auto it = turmasCols.begin(); it != turmasCols.end(); ++it)
	{
		Node* node = nullptr;
		auto itNode = turmasNode.find(it->second);
		if(itNode != turmasNode.end())
			node = itNode->second;
		else
		{
			node = new Node(it->second);	// colNr
			turmasNode[it->second] = node;
		}
		// verificar incompatibilidade. começar depois para evitar repeticao
		for(auto itIncomp = std::next(it); itIncomp != turmasCols.end(); ++itIncomp)
		{
			// to be sure
			if(itIncomp->first->getGlobalId() == it->first->getGlobalId())
				continue;

			// Se nao devemos considerar turmas do mesmo tipo da mesma oferta, ignorar
			if(!considerarMmTipoOft && (it->first->ofertaDisc == itIncomp->first->ofertaDisc) &&
				(it->first->tipoAula == itIncomp->first->tipoAula))
				continue;

			// se nao é incompativel continuar
			if(!saoIncompativeis_(it->first, itIncomp->first, deslocavel))
				continue;

			// obter node desta turma
			Node* nodeInc = nullptr;
			auto itNodeInc = turmasNode.find(itIncomp->second);
			if(itNodeInc != turmasNode.end())
				nodeInc = itNodeInc->second;
			else
			{
				nodeInc = new Node(itIncomp->second);	// colNr
				turmasNode[itIncomp->second] = nodeInc;
			}

			// registar incompatibilidade
			node->vizinhos.insert(nodeInc);
			nodeInc->vizinhos.insert(node);
		}
	}
	for(auto itNode = turmasNode.begin(); itNode != turmasNode.end(); ++itNode)
		nodes.insert(itNode->second);
}

// -------------- AGLOMERADOR DE TURMAS INCOMPATIVEIS POR HORARIO/DIA ------------------------------------

// retorna grupos de turmas incompativeis, baseado na sua sobreposição a um horario num dia
void MIPAloc::getTurmasIncHorarios(unordered_map<TurmaHeur*, int> const &turmasCols, vector<unordered_set<int>> &turmasIncompativeis, 
									bool deslocavel, bool interjornada)
{
	// verificar quais os calendarios que as turmas têm
	unordered_set<Calendario*> calendarios;
	for(auto it = turmasCols.begin(); it != turmasCols.end(); ++it)
	{
		if(calendarios.find(it->first->getCalendario()) == calendarios.end())
			calendarios.insert(it->first->getCalendario());
	}

	// associar dia-horario a turmas
	map<int, vector<TurmasIncHorarioDia*>> turmasTemHorarios;
	criarTurmasTemHorDia(calendarios, turmasTemHorarios);
	checkContemHorDia(turmasCols, turmasTemHorarios);

	// registar turmas incompativeis
	addGruposTurmasIncompativeis(turmasTemHorarios, turmasIncompativeis, deslocavel, interjornada);
}

// criar objectos TurmasIncHorarioDia. Deslocamento: OFF
void MIPAloc::criarTurmasTemHorDia(unordered_set<Calendario*> const &calendarios, map<int, vector<TurmasIncHorarioDia*>> &turmasTemHorarios)
{
	// registar o que já foi inserido para nao repetir
	unordered_map<ConjUnidades*, unordered_set<pair<int, HorarioAula*>, hashHorsDia, equalHorsDia>> horsDia;
	unordered_set<pair<int, HorarioAula*>, hashHorsDia, equalHorsDia> emptySet;

	for(auto itClust = DadosHeuristica::clustersUnidades.cbegin(); 
					 itClust != DadosHeuristica::clustersUnidades.cend(); ++itClust)
	{
		auto parIns = horsDia.insert(make_pair<ConjUnidades*, unordered_set<pair<int, HorarioAula*>, hashHorsDia, equalHorsDia>>(*itClust, emptySet));
		if(!parIns.second)
			HeuristicaNuno::excepcao("MIPAloc::criarTurmasTemHorDia", "Par conj unidade nao inserido");

		auto itClustHors = parIns.first;
		for(auto itCal = calendarios.cbegin(); itCal != calendarios.cend(); ++itCal)
		{
			for(auto itDia = (*itCal)->mapDiaDateTime.cbegin(); itDia != (*itCal)->mapDiaDateTime.cend(); ++itDia)
			{
				auto itFindDia = turmasTemHorarios.find(itDia->first);
				if(itFindDia == turmasTemHorarios.end())
				{
					vector<TurmasIncHorarioDia*> emptyVector;
					auto par = turmasTemHorarios.insert(make_pair<int, vector<TurmasIncHorarioDia*>>(itDia->first, emptyVector));
					if(!par.second)
						HeuristicaNuno::excepcao("MIPAloc::criarTurmasTemHorDia", "Dia nao adicionado ao mapa");
					itFindDia = par.first;
				}

				for(auto itHor = itDia->second.cbegin(); itHor != itDia->second.cend(); ++itHor)
				{
					pair<int, HorarioAula*> par (itDia->first, itHor->second);
					if(itClustHors->second.find(par) == itClustHors->second.end())
					{
						itClustHors->second.insert(par);
						itFindDia->second.push_back(new TurmasIncHorarioDia(par.first, par.second, *itClust));
					}
				}
			}
		}
	}
}

// juntar turmas que intersectem um determinado horario
void MIPAloc::checkContemHorDia(unordered_map<TurmaHeur*, int> const &turmasCols, map<int, vector<TurmasIncHorarioDia*>> const &turmasTemHorarios)
{
	// adicionar turmas que contêm horario dia
	for(auto it = turmasCols.cbegin(); it != turmasCols.cend(); ++it)
	{
		for(auto itDia = turmasTemHorarios.cbegin(); itDia != turmasTemHorarios.cend(); ++itDia)
		{
			// verificar se tem aula no dia
			AulaHeur* aula = nullptr;
			if(!it->first->getAulaDia(itDia->first, aula) || (aula == nullptr))
				continue;

			for(auto itHors = itDia->second.cbegin(); itHors != itDia->second.cend(); ++itHors)
			{
				// verificar se é no mesmo conjunto de unidades
				if(it->first->getSala()->getConjUnidades() != (*itHors)->clusterUnids)
					continue;

				// verificar se tem o horario
				if(aula->temHorario((*itHors)->horarioAula))
					(*itHors)->colNrsTurmas.insert(it->second);
			}
		}
	}
}

// verificar horarios incompativeis dentro do dia e passar restricoes incompativeis
void MIPAloc::addGruposTurmasIncompativeis(map<int, vector<TurmasIncHorarioDia*>> const &turmasTemHorarios, 
											vector<unordered_set<int>> &turmasIncompativeis, bool deslocavel, bool interjornada)
{
	ProblemData* const probData = HeuristicaNuno::probData;
	unordered_set<int> setCols;
	vector<TurmasIncHorarioDia*> ptrs;
	for(auto itDia = turmasTemHorarios.cbegin(); itDia != turmasTemHorarios.cend(); ++itDia)
	{
		int dia = itDia->first;
		// dia seguint
		auto itNext = turmasTemHorarios.find(UtilHeur::diaPosterior(itDia->first));

		// primeiro horario
		for(auto itHor1 = itDia->second.cbegin(); itHor1 != itDia->second.cend(); ++itHor1)
		{
			bool any = false;
			ptrs.push_back(*itHor1);

			// Verificar incompatibilidade com horarios do mesmo dia!
			for(auto itHor2 = std::next(itHor1); itHor2 != itDia->second.cend(); ++itHor2)
			{
				//if((*itHor1)->horarioAula->sobrepoe((*itHor2)->horarioAula))
				// horarios incompativeis
				setCols.clear();
				if(checkAddHorariosIncomp(*itHor1, dia, *itHor2, dia, deslocavel, interjornada, setCols))
				{
					turmasIncompativeis.push_back(setCols);
					any = true;
				}
			}

			// Verificar incompatibilidade com horarios do dia seguinte (interjornada)
			if(interjornada && probData->parametros->considerarDescansoMinProf && (itNext != turmasTemHorarios.end()))
			{
				for(auto itHorNext = itNext->second.begin(); itHorNext != itNext->second.end(); ++itHorNext)
				{
					setCols.clear();
					if(checkAddHorariosIncomp(*itHor1, dia, *itHorNext, itNext->first, deslocavel, interjornada, setCols))
					{
						turmasIncompativeis.push_back(setCols);
						any = true;
					}
				}
			}

			if(!any)
			{
				setCols.clear();
				for(auto it1 = (*itHor1)->colNrsTurmas.cbegin(); it1 != (*itHor1)->colNrsTurmas.cend(); ++it1)
					setCols.insert(*it1);
				turmasIncompativeis.push_back(setCols);
			}
		}
	}

	// libertar memoria
	for(auto it = ptrs.begin(); it != ptrs.end(); )
	{
		TurmasIncHorarioDia* ptr = *it;
		it = ptrs.erase(it);
		delete ptr;
	}
}

// check add horarios incompativeis
bool MIPAloc::checkAddHorariosIncomp(TurmasIncHorarioDia* const first, int diaFst, TurmasIncHorarioDia* const second, 
									int diaScd, bool deslocavel, bool interjornada, unordered_set<int> &setCols)
{
	bool add = false;
	if((diaFst == diaScd) && 
		!EntidadeAlocavel::horariosCompativeis(first->clusterUnids->campus->getId(), first->clusterUnids->getUnidadeId(), 
												first->horarioAula, 
												second->clusterUnids->campus->getId(), second->clusterUnids->getUnidadeId(), 
												second->horarioAula, deslocavel))
	{
		add = true;
	}
	else if (diaFst != diaScd && interjornada && 
				ProfessorHeur::violaInterjornada(first->horarioAula, diaFst, second->horarioAula, diaScd))
	{
		add = true;
	}

	if(add)
	{
		// inserir colunas das turmas
		for(auto it1 = first->colNrsTurmas.cbegin(); it1 != first->colNrsTurmas.cend(); ++it1)
			setCols.insert(*it1);
		for(auto it2 = second->colNrsTurmas.cbegin(); it2 != second->colNrsTurmas.cend(); ++it2)
			setCols.insert(*it2);

		return true;
	}
	
	return false;
}

// add MIP start parcial
void MIPAloc::addMIPStartParcial(int* indices, double* values, const int size)
{
	// check
	vector<int> indPos;
	vector<double> valPos;
	for(int idx = 0; idx < size; ++idx)
	{
		if(values[idx] > 0.0001)
		{
			/*std::stringstream ss;
			ss << "idx " << idx << "; col " << indices_[idx] << " = " << values_[idx];
			HeuristicaNuno::logMsg(ss.str(), 1);*/
			
			indPos.push_back(idx);
		}
	}

	int nrPos = indPos.size(); 
	HeuristicaNuno::logMsgInt("# cols positivas MIP Start: ", nrPos, 1);

	// só inserir positivos
	int* indicesPos = new int [nrPos]();
	double* valuesPos = new double [nrPos]();
	int i = 0;
	for(auto itPos = indPos.cbegin(); itPos != indPos.cend(); ++itPos, ++i)
	{
		indicesPos[i] = *itPos;
		valuesPos[i] = values[*itPos];
	}

	if(!lp_->copyMIPStartSol(nrPos, indicesPos, valuesPos))
		HeuristicaNuno::warning("MIPAlocarAlunos::definirMIPStart_", "MIP start não foi carregado.");

	delete indicesPos;
	delete valuesPos;
}