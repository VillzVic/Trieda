#include "SolucaoHeur.h"
#include "AulaHeur.h"
#include "AlunoHeur.h"
#include "SalaHeur.h"
#include "ProfessorHeur.h"
#include "TurmaHeur.h"
#include "OfertaDisciplina.h"
#include "DadosHeuristica.h"
#include "../ProblemData.h"
#include "../ParametrosPlanejamento.h"
#include "ObjectiveFunctionHeur.h"
#include "ParametrosHeuristica.h"
#include "TurmaPotencial.h"
#include "HeuristicaNuno.h"
#include "StatsSolucaoHeur.h"
#include "AbridorTurmas.h"
#include "MIPAlocarAlunos.h"
#include "MIPAlocarProfs.h"
#include "../AlunoDemanda.h"
#include "../AtendimentoCampus.h"
#include "../AtendimentoUnidade.h"
#include "../AtendimentoSala.h"
#include "../AtendimentoDiaSemana.h"
#include "../AtendimentoTurno.h"
#include "../AtendimentoTatico.h"
#include "../AtendimentoHorarioAula.h"
#include "../AtendimentoOferta.h"
#include "../ProfessorVirtualOutput.h"
#include "../AlocacaoProfVirtual.h"
#include "../ProblemSolution.h"
#include "GeradorCombsDiv.h"
#include "../CentroDados.h"
#include "../Aluno.h"
#include "../Magisterio.h"
#include "../Professor.h"
#include "ImproveMethods.h"
#include "UtilHeur.h"
#include "SaveSolucao.h"
#include <stdio.h>
#include <ctime>

int SolucaoHeur::idVirtual_ = 0;

SolucaoHeur::SolucaoHeur(void)
	: demandasNaoAtendidas_(), stats_(new StatsSolucaoHeur()), loading_(false)
{
	prepararDados();
	inicializarDemandasNaoAtendidas_(DadosHeuristica::demandasAgregadas());
	// por curso !
	inicializarDemandasNaoAtendidasPorCurso_(DadosHeuristica::demandasAgregadasPorCurso());
}

SolucaoHeur::~SolucaoHeur(void)
{
	// limpar turmas loaded closed
	for(auto it = loadedTurmasClosed.begin(); it != loadedTurmasClosed.end();)
	{
		TurmaHeur* turma = (*it);
		loadedTurmasClosed.erase(it++);
		delete turma;
		turma = nullptr;
	}

	// limpar memória associada estruturas
	HeuristicaNuno::logMsg("limpar ofertasDisciplina", 1);
	limparOfertasDisciplina_();
	HeuristicaNuno::logMsg("limpar alunosHeur", 1);
	limparMap_<AlunoHeur> (alunosHeur);

	HeuristicaNuno::logMsg("limpar profsHeur", 1);
	//limparMap_<ProfessorHeur> (professoresHeur);
	for(auto itProfs = professoresHeur.begin(); itProfs != professoresHeur.end();)
	{
		ProfessorHeur* prof = itProfs->second;
		if(!prof->ehVirtual())
		{
			professoresHeur.erase(itProfs++);
			delete prof;
		}
		else
			++itProfs;
	}

	HeuristicaNuno::logMsg("limpar salasHeur", 1);
	limparMap_<SalaHeur> (salasHeur);

	// deletar profs virtuais
	for(auto itPerf = profsVirtuais.begin(); itPerf != profsVirtuais.end();)
	{
		ProfessorHeur* prof = itPerf->second;
		profsVirtuais.erase(itPerf++);
		delete prof;
	}

	// deleter perfis
	for(auto it = profsVirtuaisCursoPerfil.begin(); it != profsVirtuaisCursoPerfil.end();)
	{
		PerfilVirtual* perfil = it->first;
		profsVirtuaisCursoPerfil.erase(it++);
		delete perfil;
	}

	// Stats
	HeuristicaNuno::logMsg("deletar stats", 1);
	delete stats_;
}

// ------------------------------------- ACESSORES -------------------------------------

int SolucaoHeur::nrDemandasAtendidas(int prioridade, bool equiv) const
{
	return stats_->nrDemsAtend(prioridade, equiv);
}

// -------------------------------------------------------------------------------------------

// versão alternativa
SolucaoHeur* SolucaoHeur::gerarSolucaoInicial(void)
{
	HeuristicaNuno::logMsg("Criar objeto SolucaoHeur", 1);
	SolucaoHeur* solucaoInicial (new SolucaoHeur());

	HeuristicaNuno::logMsgInt("Demandas nao atendidas Prior 1 (antes): ", solucaoInicial->nrDemandasNaoAtendidas(1), 1);

	// [Pre-processamento] Criar as ofertas disciplina, i.e. oferta de disciplinas em campus
	solucaoInicial->criarOfertasDisciplina_();
	HeuristicaNuno::logMsgInt("Nr ofertas disciplina com Teorico e Pratico: ", solucaoInicial->nrDiscDuasComp(), 1);

	// Desenvolver solução
	solucaoInicial->developSolucao();

	return solucaoInicial;
}
// gera solução completa, fixando o atendimento da solução passada
SolucaoHeur* SolucaoHeur::gerarSolucaoInicial(ProblemSolution * const partialSol)
{
	HeuristicaNuno::logMsg("Criar objeto SolucaoHeur", 1);
	SolucaoHeur* solucaoInicial = SolucaoHeur::carregarSolucao(partialSol);

	HeuristicaNuno::logMsgInt("Demandas nao atendidas Prior 1 (antes): ", solucaoInicial->nrDemandasNaoAtendidas(1), 1);

	// [Pre-processamento] Criar as ofertas disciplina, i.e. oferta de disciplinas em campus
	solucaoInicial->criarOfertasDisciplina_(false);
	HeuristicaNuno::logMsgInt("Nr ofertas disciplina com Teorico e Pratico: ", solucaoInicial->nrDiscDuasComp(), 1);

	// Fixa solução parcial
	solucaoInicial->fixSolucao();

	// Desenvolver solução
	solucaoInicial->developSolucao();

	return solucaoInicial;
}
// melhorar solução carregada
void SolucaoHeur::improveSolucaoFixada(SolucaoHeur* const solucao)
{
	// (1) Remover alunos que venham incompletos
	if(ParametrosHeuristica::relacPraticas != ParametrosHeuristica::abrePraticas::N_N)
	{
		for(auto it = solucao->allOfertasDisc_.cbegin(); it != solucao->allOfertasDisc_.cend(); ++it)
			(*it)->removerAlunosIncompleto(true);
	}

	// (2) Fechar turmas vazias
	solucao->fecharTurmasVazias_();

	// (3) Tentar reduzir as salas por ordem de excesso de capacidade
	solucao->tryReduzirSalas_();

	// (4) Tentar alocar demandas nao atendidas em turmas já abertas
	solucao->tryAlocNaoAtendidos_(true, true, ParametrosHeuristica::heurRealocAlunos, 0);

	// (5) Se puder, fechar turmas carregadas !
	if(ParametrosHeuristica::fecharTurmasCarregadas)
		solucao->acertarSolucao_(true);

	// (6) Desenvolver a solução
	solucao->developSolucao();

	// (7) Log sugestões mudanças de sala
	solucao->checkLogMudancasSala();

	// (8) Log sugestões fechamento de turmas carregadas (e mudança de alunos)
	solucao->checkClosedTurmasLoad();
}

// tentar só fazer trocas de sala (e fechar turmas caso necessario) de uma solução carregada
void SolucaoHeur::realocSalasSolucaoFix(SolucaoHeur* const solucao)
{
	HeuristicaNuno::logMsg("", 1);
	HeuristicaNuno::logMsg(">>>> REALOC SALAS SOLUCAO FIX", 1);
	HeuristicaNuno::logMsg("", 1);
	// (1) Tentar mudar de sala heuristicamente
	HeuristicaNuno::logMsg(">> Tentar trocar salas indisponiveis", 1);
	if(solucao->tryMudarSalasIndisp_())
	{
		HeuristicaNuno::logMsg("Conseguiu mudar todas as salas indisponiveis com heuristica!", 1);
		return;
	}

	// (2) Tentar mudar de sala MIP
	SaveSolucao* solucaoAtual = new SaveSolucao();
	solucao->guardarSolucaoAtualAlunos_(solucaoAtual);
	string nome = getProxMIPNome("MIPAlocarAlunos");
	MIPAlocarAlunos mip (nome.c_str(), solucao, solucaoAtual, true, 0.0, 0, false);
	mip.realocarSalas();
	delete solucaoAtual;
	solucaoAtual = nullptr;

	// (3) Remover alunos incompletos e fechar turmas vazias
	HeuristicaNuno::logMsg(">> Remover alunos incompletos e fechar turmas vazias", 1);
	solucao->removerAllAlunosIncompletos_();
	solucao->fecharTurmasVazias_();

	// (4) Check mudanças de sala e fechamento de turmas carregadas
	solucao->checkLogMudancasSala();
	solucao->checkClosedTurmasLoad();
}

// develop solução carregada
void SolucaoHeur::developSolucao(void)
{
	// TESTE!
	developSolucaoPrior();
	return;

	// (1) Abrir turmas primeira fase (só turmas legais)
	abrirTurmas_(true, true, ParametrosHeuristica::heurRealocAlunos, false);
	verificacao_();

	// se nao for fast imprimir output preMIP
	if(checkFast())
	{
		// deixar abrir com formandos qualquer tamanho
		abrirTurmas_(true, true, false, true);
		verificacao_();
		// tentar alocar P2
		tryAlocP2_();
		verificacao_();
		return;
	}
	else
	{
		HeuristicaNuno::outputSolucao(this, "preMIP");
	}

	// (2) Guardar solução sem turmas ilegais
	SaveSolucao* solucaoAtual = new SaveSolucao();
	guardarSolucaoAtualAlunos_(solucaoAtual);

	// (3) Tentar criar mais turmas com um limite minimo de alunos para abertura relaxado
	// obs: sem realocar (deixar abrir com formandos, qualquer que seja o tamanho!)
	//abrirTurmas_(true, true, false, true, 0, ParametrosHeuristica::relaxMinAlunosTurma);
	if(ParametrosHeuristica::temMins())
	{
		abrirTurmas_(true, true, ParametrosHeuristica::heurRealocAlunos, true, 0, ParametrosHeuristica::relaxMinAlunosTurma);
		verificacao_(false);
	}

	// (4) Tentar realocar os alunos com o MIP (se ligado, realocar salas)
	realocarAlunosMIP_(solucaoAtual, ParametrosHeuristica::mipRealocSalas, ParametrosHeuristica::minProdutividadeCred);
	HeuristicaNuno::logMsg("saiu mip", 1);
	verificacao_();
	delete solucaoAtual;
	solucaoAtual = nullptr;

	// verificar se há turmas ilegais
	checkTurmasInv("SolucaoHeur::developSolucao");
	// imprimir demandas atendidas
	HeuristicaNuno::logMsg("imprimir demandas atendidas...", 1);
	printDemandasAtendidas("F");

	// (5) Tentar abrir mais turmas! (deixar abrir com formandos, qualquer que seja o tamanho!) sem realoc
	int nrTurmas = this->nrTurmas();
	abrirTurmasFinal_(true, true, false);
	//abrirTurmas_(true, true, false, true);
	verificacao_();
	int nrTurmasNew = this->nrTurmas();

	// se alguma turma tiver sido aberta, tentar alocar mais P2!
	if(nrTurmasNew > nrTurmas)
		tryAlocP2_();
	verificacao_();

	// (6) Re-alocar professores (e criar virtuais individuais)
	alocarProfessores();
}
// develop solução por prioridades (começar pelos mais prioritarios, etc..)
void SolucaoHeur::developSolucaoPrior(void)
{
	HeuristicaNuno::logMsg("", 1);
	HeuristicaNuno::logMsg(">>> Construir solucao por ordem de prioridades!", 1); HeuristicaNuno::logMsg("", 1);

	// (1) Abrir turmas legais por ordem de prioridades. Só realocar na primeira vez !
	for(auto it = DadosHeuristica::prioridadesAlunos.cbegin(); it != DadosHeuristica::prioridadesAlunos.cend(); ++it)
	{
		int priorAluno = (*it);
		HeuristicaNuno::logMsgInt(">> Prioridade ", priorAluno, 1);

		// (1.1) Abrir turmas legais. só realocar alunos desta prioridade
		abrirTurmas_(true, (true && ParametrosHeuristica::permitirProfVirt), 
						ParametrosHeuristica::heurRealocAlunos, false, priorAluno);
		verificacao_();

		if(!ParametrosHeuristica::versaoFast)
		{
			// (1.2) Guardar solução atual
			SaveSolucao* solucaoAtual = new SaveSolucao();
			guardarSolucaoAtualAlunos_(solucaoAtual);

			// (1.3) Tentar criar mais turmas com um limite minimo de alunos para abertura relaxado (turmas podem ser ilegais)
			// obs: sem realocar (deixar abrir com formandos, qualquer que seja o tamanho!)
			if(ParametrosHeuristica::temMins())
			{
				abrirTurmas_(true, (true && ParametrosHeuristica::permitirProfVirt),
					ParametrosHeuristica::heurRealocAlunos, true, priorAluno, ParametrosHeuristica::relaxMinAlunosTurma);
				verificacao_(false);
			}

			// (1.4) Tentar realocar os alunos (só desta prioridade) com o MIP e decidir quais das novas turmas manter.
			// alocar P2 junto com a ultima prioridade
			auto itNext = std::next(it);
			bool alocarP2 = (itNext == DadosHeuristica::prioridadesAlunos.cend());
			realocarAlunosMIP_(solucaoAtual, ParametrosHeuristica::mipRealocSalas, ParametrosHeuristica::minProdutividadeCred, priorAluno, alocarP2);
			HeuristicaNuno::logMsg("saiu mip", 1);
			verificacao_();
			delete solucaoAtual;
			solucaoAtual = nullptr;
		}

		// (1.5) fixar turmas e alunos
		fixSolucao();

		// (1.6) Tentar alocar mais nao atendidos (de todas as prioridades) nas turmas abertas. Nao realocar
		// obs: solução é acertada no fim
		tryAlocNaoAtendidos_(true, true, false, 0);

		HeuristicaNuno::logMsg("", 1);
	}

	// (2) Tentar abrir mais turmas! (deixar abrir com formandos, qualquer que seja o tamanho!) sem realoc
	int nrTurmas = this->nrTurmas();
	//abrirTurmas_(true, true, false, true, 0);
	abrirTurmasFinal_(true, (true && ParametrosHeuristica::permitirProfVirt), false);
	verificacao_();
	int nrTurmasNew = this->nrTurmas();

	// se alguma turma tiver sido aberta, tentar alocar mais P2!
	if(nrTurmasNew > nrTurmas)
		tryAlocP2_();
	verificacao_();

	// verificar se há turmas ilegais
	checkTurmasInv("SolucaoHeur::developSolucao");
	// imprimir demandas atendidas
	HeuristicaNuno::logMsg("imprimir demandas atendidas...", 1);
	printDemandasAtendidas("F");

	// (3) Re-alocar professores (e criar virtuais individuais)
	alocarProfessores();
}

// fixa turmas e alunos
void SolucaoHeur::fixSolucao(void)
{
	unordered_set<TurmaHeur*> turmas;
	for(auto it = allOfertasDisc_.cbegin(); it != allOfertasDisc_.cend(); ++it)
	{
		turmas.clear();
		(*it)->getTurmas(turmas);
		// flag para fixar as turmas
		for(auto itT = turmas.begin(); itT != turmas.end(); ++itT)
			(*itT)->setMustAbrirMIP(true);

		// flag para manter os alunos fixados
		(*it)->setKeepAlunos();
	}
}



// check versão fast. se for fast imprimir solução e retornar
bool SolucaoHeur::checkFast(void)
{
	// se for fast retornar
	if(ParametrosHeuristica::versaoFast)
	{
		// verificar se há turmas ilegais
		int inv = nrTurmasInvalidas();
		if(inv > 0)
		{
			HeuristicaNuno::logMsgInt("# turmas ilegais: ", inv, 1);
			HeuristicaNuno::warning("SolucaoHeur::gerarSolucaoInicial", "Solucao final tem turmas ilegais!");
		}
		printDemandasAtendidas("F");
		return true;
	}

	// se nao for fast imprimir output preMIP
	HeuristicaNuno::logMsg("Gerar output pre-MIP...", 1);
	HeuristicaNuno::outputSolucao(this, "preMIP");
	printDemandasAtendidas("preMIP");
	HeuristicaNuno::logMsg("output gerado.", 1);

	return false;
}
// check turmas invalidas
void SolucaoHeur::checkTurmasInv(string const &metodo)
{
	int inv = nrTurmasInvalidas();
	if(inv > 0)
	{
		HeuristicaNuno::logMsg("turmas ilegais! acertar solucao...", 1);
		acertarSolucao_();
		checkStats();
		HeuristicaNuno::logMsgInt("# turmas ilegais: ", inv, 1);
		HeuristicaNuno::warning(metodo, "Solucao final tem turmas ilegais!");
	}
}


// Gerar ProblemSolution
ProblemSolution* SolucaoHeur::getProblemSolution(void) const
{
	HeuristicaNuno::logMsg(">> Criar problem solution:", 1);

	ProblemSolution* const probSolution = new ProblemSolution(false);
	HeuristicaNuno::logMsg("criar output...", 1);
	criarOutputNovo_(probSolution);
	HeuristicaNuno::logMsg("criar output profs virtuais...", 1);
	criarOutProfsVirtuais_(probSolution);

	if(probSolution->alunosDemanda)
		delete probSolution->alunosDemanda;
	probSolution->alunosDemanda = new GGroup<AlunoDemanda*, Less<AlunoDemanda*>>(HeuristicaNuno::probData->alunosDemanda);

	HeuristicaNuno::logMsg("problem solution criada", 1);

	return probSolution;
}


// desalocar 'perc' % dos alunos. perc > 0 && per < 1. default: 0.30
void SolucaoHeur::desalocarAlunosRandom(int perc)
{
	HeuristicaNuno::logMsg("Desalocar alunos random.", 1);
	if((perc <= 0) || (perc >= 100))
		perc = 30;

	for(auto itCamp = ofertasDisciplina_.cbegin(); itCamp != ofertasDisciplina_.cend(); ++itCamp)
	{
		for(auto itDisc = itCamp->second.cbegin(); itDisc != itCamp->second.cend(); ++itDisc)
		{
			OfertaDisciplina* const oferta = itDisc->second;
			auto turmasPrinc = oferta->getTurmasPrincipal();
			for(auto itTurma = turmasPrinc.cbegin(); itTurma != turmasPrinc.cend(); ++itTurma)
			{
				TurmaHeur* const turma = itTurma->second;
				unordered_set<AlunoHeur*> alunos;
				turma->getAlunos(alunos);
				for(auto itAluno = alunos.cbegin(); itAluno != alunos.cend(); ++itAluno)
				{
					// remover alunos aleatoriamente
					int randNr = rand() % 100;
					if(randNr < perc)
						oferta->removeAlunoTurma(*itAluno, turma, true); // remover mesmo fixado
				}
			}
			if(oferta->nrAlunosIncompleto() > 0)
				oferta->removerAlunosIncompleto(true);
		}
	}
}

// this eh melhor que a solucao comparada?
bool SolucaoHeur::ehMelhor(SolucaoHeur* const solucao) const
{
	// recalcular stats
	this->checkStats(false);
	solucao->checkStats(false);

	// demandas atendidas P1 (equivalente ao atendimento)
	int diff = stats_->nrDemsAtend(1) - solucao->nrDemandasAtendidas(1);
	if(diff != 0)
		return diff > 0;

	// demandas atendidas P2
	diff = stats_->nrDemsAtend(2) - solucao->nrDemandasAtendidas(2);
	if(diff != 0)
		return diff > 0;

	// créditos professores virtuais
	diff = stats_->nrCreditosProfsVirtuais_ - solucao->nrCreditosProfVirtual();
	if(diff != 0)
		return diff < 0;

	// créditos professores
	diff = stats_->nrCreditosProfessores_ - solucao->nrCreditosProf();
	if(diff != 0)
		return diff < 0;

	return false;
}

// ------------------------------------- LOAD SOLUÇÃO -------------------------------------

// load ProblemSolution
SolucaoHeur* SolucaoHeur::carregarSolucao(ProblemSolution* const solution)
{
	HeuristicaNuno::logMsg(">>> Carregar solucaoheur...", 1);
	if(!DadosHeuristica::foiInicializado())
		HeuristicaNuno::excepcao("SolucaoHeur::carregarSolucao", "DadosHeuristica nao foram inicializados!");

	SolucaoHeur* const solucaoHeur = new SolucaoHeur();
	solucaoHeur->loading_ = true;

	auto turmasAlunos = new unordered_map<TurmaHeur*, unordered_set<AlunoHeur*>>();
	auto turmasHorarios = new unordered_map<TurmaHeur*, unordered_map<int, set<HorarioAula*>>>();

	// 1. Carregar atendimentos campus
	HeuristicaNuno::logMsg("carregar atendimentos...", 1);
	for(auto itCamp = solution->atendimento_campus->begin(); itCamp != solution->atendimento_campus->end(); ++itCamp)
		solucaoHeur->loadAtendimentoCampus(*itCamp, turmasAlunos, turmasHorarios);

	// 2. Carregar aulas para as turmas, associá-las a um calendário e associar à sala e professor
	HeuristicaNuno::logMsg("preencher aulas turmas...", 1);
	solucaoHeur->loadAulasTurmas(turmasHorarios);

	// 3. Carregar alunos
	HeuristicaNuno::logMsg("adicionar alunos...", 1);
	solucaoHeur->loadAlunosFixadosTurmas(turmasAlunos);

	solucaoHeur->loading_ = false;

	delete turmasAlunos;
	delete turmasHorarios;

	return solucaoHeur;
}

// -------------------------------------------------------------------------------------------

// Print Stats
void SolucaoHeur::printStats(void) const
{
	HeuristicaNuno::logMsg("print stats:", 1);
	int demAtendP1 = nrDemandasAtendidas(1);
	int demP1 = stats_->nrDemsPrior(1, false);

	if(demAtendP1 > demP1)
		HeuristicaNuno::warning("SolucaoHeur::printStats", "Demandas atendidas P1 maior que demandas P1 registadas.");

	double atendP1 = 0.0;
	if(demP1 >= 1)
		atendP1 = (double(demAtendP1) / demP1);

	HeuristicaNuno::logMsgInt("Nr turmas abertas: ", nrTurmas(), 0);
	HeuristicaNuno::logMsgInt("Nr turmas invalidas: ", nrTurmasInvalidas(), 0);
	HeuristicaNuno::logMsgInt("Nr turmas prof real: ", nrTurmasProfReal(), 0);
	HeuristicaNuno::logMsgInt("Nr turmas praticas abertas (c/ teorica): ", nrTurmasAbertasCompSec(), 0);
	HeuristicaNuno::logMsgInt("Nr créditos alunos: ", stats_->nrCreditosAlunos_, 0);
	HeuristicaNuno::logMsgInt("Nr créditos professor: ", stats_->nrCreditosProfessores_, 0);
	HeuristicaNuno::logMsgInt("Nr créditos professores virtuais: ", stats_->nrCreditosProfsVirtuais_, 0);
	HeuristicaNuno::logMsgDouble("Receita credito: ", stats_->getRatioCreds(), 0);
	HeuristicaNuno::logMsgDouble("Custo credito: ", stats_->getCustoCreds(), 0);
	HeuristicaNuno::logMsgInt("Demandas atendidas P1: ", demAtendP1, 0);
	HeuristicaNuno::logMsgInt("Demandas nao atendidas P1: ", nrDemandasNaoAtendidas(1), 0);
	HeuristicaNuno::logMsgDouble("Atendimento P1: ", atendP1, 0);
	HeuristicaNuno::logMsgInt("Demandas atendidas P2: ", nrDemandasAtendidas(2), 0);
	HeuristicaNuno::logMsgInt("Nr alunos incompletos: ", nrAlunosIncompletos(), 0);
}

// ------------------------------------- INICIALIZAR DADOS -------------------------------------

// criar estruturas salaheur, alunoheur, profheur
void SolucaoHeur::prepararDados(void)
{
	ProblemData* const probData = CentroDados::getProblemData();
	// Gerar AlunosHeur
	for(auto itAlunos = probData->alunos.begin(); itAlunos != probData->alunos.end(); ++itAlunos)
	{
		AlunoHeur* const alunoHeur = new AlunoHeur(*itAlunos);
		alunosHeur[(*itAlunos)->getAlunoId()] = alunoHeur;
		Curso* const curso = alunoHeur->getCurso();
		if(curso == nullptr)
			continue;
		auto itCurso = alunosPorCurso.find(curso);
		if(itCurso == alunosPorCurso.end())
		{
			unordered_set<AlunoHeur*> emptySet;
			auto par = alunosPorCurso.insert(make_pair(curso, emptySet));
			if(!par.second)
				HeuristicaNuno::excepcao("SolucaoHeur::prepararDados", "Alunos do curso nao inserido na estrutura");
			itCurso = par.first;
		}
		itCurso->second.insert(alunoHeur);
	}

	// Gerar ProfessoresHeur
	for(auto itProf = probData->mapProfessorDisciplinasAssociadas.begin(); itProf != probData->mapProfessorDisciplinasAssociadas.end(); ++itProf)
	{
		if(itProf->first->eVirtual())
			continue;

		professoresHeur[itProf->first->getId()] = new ProfessorHeur (itProf->first);
	}
	auto profVirtualUnico = new ProfessorHeur();
	professoresHeur[ParametrosHeuristica::profVirtualId] = profVirtualUnico;
	profsVirtuais[ParametrosHeuristica::profVirtualId] = profVirtualUnico;

	// Gerar SalasHeur
	gGroupSalas allSalas = probData->getSalas();
	for(auto itSala = allSalas.begin(); itSala != allSalas.end(); ++itSala)
	{
		salasHeur[(*itSala)->getId()] = new SalaHeur(*itSala);
	}
}
// criar a estrutura demandasNaoAtendidas_
void SolucaoHeur::inicializarDemandasNaoAtendidas_(unordered_map<Campus *, unordered_map<Disciplina *, unordered_map<int, unordered_set<AlunoDemanda *>>>>* const dados)
{
	unordered_map<Disciplina *, unordered_map<int, unordered_set<AlunoDemanda *>>> emptyMapDisc;
	unordered_map<int, unordered_set<AlunoDemanda *>> emptyMapPrior;
	for(auto itCamp = dados->cbegin(); itCamp != dados->cend(); itCamp++)
	{
		Campus* const ptrCampus = itCamp->first;
		auto parCamp = demandasNaoAtendidas_.insert(make_pair(ptrCampus, emptyMapDisc));
		if(!parCamp.second)
			HeuristicaNuno::excepcao("SolucaoHeur::initializarDemandasNaoAtendidas_", "Erro inserindo campus em demandas nao atendidas");
		auto itDemCamp = parCamp.first;

		for(auto itDisc = itCamp->second.cbegin(); itDisc != itCamp->second.cend(); ++itDisc)
		{
			Disciplina* const ptrDisc = itDisc->first;
			auto parDisc = itDemCamp->second.insert(make_pair(itDisc->first, emptyMapPrior));
			if(!parDisc.second)
				HeuristicaNuno::excepcao("SolucaoHeur::initializarDemandasNaoAtendidas_", "Erro inserindo disciplina em demandas nao atendidas");
			auto itDemDisc = parDisc.first;

			for(auto itPrior = itDisc->second.cbegin(); itPrior != itDisc->second.cend(); itPrior++)
			{
				auto parPrior = itDemDisc->second.insert(make_pair(itPrior->first, itPrior->second));
				if(!parPrior.second)
					HeuristicaNuno::excepcao("SolucaoHeur::initializarDemandasNaoAtendidas_", "Erro inserindo prioridade e demandas nao atendidas");
			}
		}
	}
}
// inicializar estrutura demsNaoAtendPorCurso_
void SolucaoHeur::inicializarDemandasNaoAtendidasPorCurso_(unordered_map<Campus *, unordered_map<Disciplina *, unordered_map<Curso*, unordered_map<int, unordered_set<AlunoDemanda *>>>>>* const dados)
{
	demsNaoAtendPorCurso_ = *dados;
}

// -------------------------------------------------------------------------------------------


// ------------------------------------- CONSTRUÇÃO GERAL -------------------------------------

// criar ofertas disciplina iniciais
void SolucaoHeur::criarOfertasDisciplina_(bool limpar)
{
	HeuristicaNuno::logMsg("Criar ofertas disciplina!", 1);
	if(limpar)
	{
		ofertasDisciplina_.clear();
		allOfertasDisc_.clear();
	}

	// disciplinas sem demanda (p1 e equiv)
	int nrDiscSemDem = 0;

	// para turmasPorDisc_
	unordered_map<Disciplina*, OfertaDisciplina*> emptyMap;
	unordered_set<TurmaHeur*> emptyTurmas;
	unordered_map<Campus*, unordered_map<Disciplina*, unordered_map<int, unordered_set<AlunoDemanda *>>>>* const demandas = DadosHeuristica::demandasAgregadas();	
	for(auto itCamp = demandas->cbegin(); itCamp != demandas->cend(); ++itCamp)
	{
		Campus* const campus = itCamp->first;

		HeuristicaNuno::logMsgInt("Demandas não atendidas Campus considerado: ", nrDemandasNaoAtendidas(campus, true), 1);

		for(auto itDisc = itCamp->second.cbegin(); itDisc != itCamp->second.cend(); ++itDisc)
		{
			Disciplina* const disciplina = itDisc->first;

			// Inicio só considerar demandas prioridade 1
			auto itPriorUm = itDisc->second.find(1);
			auto itPriorEquiv = itDisc->second.find(-1);

			bool temUm = (itPriorUm != itDisc->second.end());
			bool temEquiv = (itPriorEquiv != itDisc->second.end());
			if(!temUm && !temEquiv)
			{
				HeuristicaNuno::logMsgInt("Disc sem demandas: ", disciplina->getId(), 1);
				nrDiscSemDem++;
				continue;
			}

			OfertaDisciplina *finder = getOfertaDisciplina(disciplina, campus);
			if (finder) continue;

			stats_->nrDisc_++;

			Disciplina* discTeorica = nullptr;
			Disciplina* discPratica = nullptr;

			// fazer associação da disciplina teorica e pratica
			associarDisciplinas_(disciplina, discTeorica, discPratica);

			// Criar oferta
			OfertaDisciplina* ofertaDisc = new OfertaDisciplina (this, discTeorica, discPratica, campus);
			ofertaDisc->setDemandasNaoAtend(itDisc->second);
				
			// gerar/reprocessar divisões.
			ofertaDisc->gerarDivs();

			// Se a disciplina não tem salas associadas naqueles campus então não considerar
			if(!ofertaDisc->podeAbrir())
			{
				HeuristicaNuno::logMsgInt("Nao pode abrir, faltam salas assoc ou divisoes de creditos! Disc id: ", disciplina->getId(), 1);
				stats_->nrNaoPodeAbrir_++;
				delete ofertaDisc;
				continue;
			}

			if(ofertaDisc->nrTiposAula() == 2)
				stats_->nrDiscDuasComp_++;
				
			// adicionar à estrutura ofertasDisciplina_
			auto itCampusOf = ofertasDisciplina_.find(campus);
			if(itCampusOf == ofertasDisciplina_.end())
			{
				itCampusOf = ofertasDisciplina_.insert(make_pair(campus, emptyMap)).first;
			}
			itCampusOf->second[disciplina] = ofertaDisc;
			allOfertasDisc_.push_back(ofertaDisc);

			// inserir em turmasPorDisc_
			if(turmasPorDisc_.find(disciplina) == turmasPorDisc_.end())
				turmasPorDisc_[disciplina] = emptyTurmas;
		}
		HeuristicaNuno::logMsgInt("Nr ofertas disciplina abertas: ", stats_->nrDisc_, 1);
		HeuristicaNuno::logMsgInt("Nr ofertas disciplina sem demanda P1 (c/equiv): ", nrDiscSemDem, 1);
		HeuristicaNuno::logMsgInt("Nr ofertas disciplina nao abertas por falta de ambs/divs: ", stats_->nrNaoPodeAbrir_, 1);
		GeradorCombsDiv::printMaxCombsInfo();
	}
}
// com base na disciplina original identifica a teorica e a pratica
void SolucaoHeur::associarDisciplinas_(Disciplina* const disciplina, Disciplina* &discTeorica, Disciplina* &discPratica)
{
	// DEBUG
	/*if(disciplina->getId() == 13671)
	{
		HeuristicaNuno::logMsgInt(">>> Disciplina id (inicio): ", disciplina->getId(), 1);
		HeuristicaNuno::logMsgInt("cred teoricos: ", disciplina->getCredTeoricos(), 1);
		HeuristicaNuno::logMsgInt("cred praticos: ", disciplina->getCredPraticos(), 1);
		if(disciplina->eLab())
			HeuristicaNuno::logMsg("tem lab", 1);
	}*/

	int totalCreds = disciplina->getTotalCreditos();
	if((disciplina->getCredPraticos() > 0) && (disciplina->getCredTeoricos() > 0))
	{
		/*HeuristicaNuno::logMsgInt("disciplina id: ", disciplina->getId(), 1);
		stringstream ss;
		ss << "Disciplina " << disciplina->getId() << " com creditos pratico e teoricos! Todos colocados como teoricos!";
		HeuristicaNuno::warning("SolucaoHeur::associarDisciplinas_", ss.str());*/
		discTeorica = disciplina;
	}
	// prática
	else if(disciplina->getCredPraticos() > 0)
		discPratica = disciplina;
	// teórica
	else 
		discTeorica = disciplina;

	// não definido o tipo de disciplina
	if(discTeorica == nullptr && discPratica == nullptr)
		HeuristicaNuno::excepcao("SolucaoHeur::associarDisciplinas_", "Tipo de Disciplina [prático / teórico] não encontrado!");

	// Verificar se há componente prática
	if(discTeorica != nullptr)
	{
		Disciplina* discCompSec = nullptr;
		DadosHeuristica::getComponentePratica(discTeorica->getId(), discCompSec);
		if(discCompSec != nullptr && discCompSec->getCredTeoricos() > 0)
			HeuristicaNuno::excepcao("SolucaoHeur::associarDisciplinas_", "Componente pratica tem creditos teoricos!");

		if(discCompSec != nullptr && discPratica != nullptr)
			HeuristicaNuno::excepcao("SolucaoHeur::associarDisciplinas_", "Ja havia uma disciplina pratica na oferta!");

		discPratica = discCompSec;
	}
}

// POR CURSO!
void SolucaoHeur::criarOfertasDisciplinaPorCurso_(void)
{
	ofertasDiscPorCurso_.clear();
	allOfertasDisc_.clear();

	unordered_map<int, int> demandasNaoAtend;
	unordered_map<Disciplina*, unordered_map<Curso*, vector<OfertaDisciplina*>>> emptyMapDisc;
	unordered_map<Curso*, vector<OfertaDisciplina*>> emptyMapCurso;
	vector<OfertaDisciplina*> emptyVetor;

	auto cursosCompCampusDisc = DadosHeuristica::getCursosCompatCampusDisc();
	for(auto itCampus = cursosCompCampusDisc->cbegin(); itCampus != cursosCompCampusDisc->cend(); ++itCampus)
	{
		Campus* const campus = itCampus->first;
		for(auto itDisc = itCampus->second.cbegin(); itDisc != itCampus->second.cend(); ++itDisc)
		{
			Disciplina* const disciplina = itDisc->first;
			Disciplina* discTeorica = nullptr;
			Disciplina* discPratica = nullptr;

			// fazer associação da disciplina teorica e pratica
			associarDisciplinas_(disciplina, discTeorica, discPratica);

			for(auto itComb = itDisc->second.cbegin(); itComb != itDisc->second.cend(); ++itComb)
			{
				// get nr demandas não atend
				demandasNaoAtend.clear();
				this->getMapDemandasNaoAtend(campus, disciplina, *itComb, demandasNaoAtend);

				// Criar oferta
				OfertaDisciplina* ofertaDisc = new OfertaDisciplina (this, discTeorica, discPratica, campus, *itComb);
				ofertaDisc->setDemandasNaoAtend(demandasNaoAtend);
				
				// gerar/reprocessar divisões.
				ofertaDisc->gerarDivs();

				// Se a disciplina não tem salas associadas naqueles campus então não considerar
				if(!ofertaDisc->podeAbrir())
				{
					HeuristicaNuno::logMsgInt("Nao pode abrir, faltam salas assoc ou divisoes de creditos! Disc id: ", disciplina->getId(), 1);
					stats_->nrNaoPodeAbrir_++;
					delete ofertaDisc;
					continue;
				}

				if(ofertaDisc->nrTiposAula() == 2)
					stats_->nrDiscDuasComp_++;

				// adicionar à estrutura ofertasDiscPorCurso_
				// campus
				auto itCampusOf = ofertasDiscPorCurso_.find(campus);
				if(itCampusOf == ofertasDiscPorCurso_.end())
				{
					itCampusOf = ofertasDiscPorCurso_.insert(make_pair(campus, emptyMapDisc)).first;
				}
				// disciplina
				auto itDiscOf = itCampusOf->second.find(disciplina);
				if(itDiscOf == itCampusOf->second.end())
				{
					itDiscOf = itCampusOf->second.insert(make_pair(disciplina, emptyMapCurso)).first;
				}
				// cursos
				for(auto itC = itComb->cbegin(); itC != itComb->cend(); ++itC)
				{
					Curso* const curso = *itC;
					auto itCursoOf = itDiscOf->second.find(curso);
					if(itCursoOf == itDiscOf->second.end())
					{
						itCursoOf = itDiscOf->second.insert(make_pair(curso, emptyVetor)).first;
					}
					itCursoOf->second.push_back(ofertaDisc);
				}

				// vetor com todas as ofertas
				allOfertasDisc_.push_back(ofertaDisc);
			}
		}
	}
}

// criar turmas (usando equivalências)
void SolucaoHeur::abrirTurmas_(bool equiv, bool usarVirtual, bool realoc, bool formAnySize, int priorAluno, double relaxMin)
{
	HeuristicaNuno::logMsg("", 1);
	HeuristicaNuno::logMsg(">>> Abrir turmas...", 1);

	// reset indicadores de demandas das salas. Reset demandas nao atendidas das ofertas
	if(ParametrosHeuristica::indicDemSalas)
	{
		resetIndicDemSalas_();
		resetCountDemsOfts_();
	}

	// get todas as ofertas ordenadas
	set<OfertaDisciplina*, compOftDisc> setOrd;
	getOfertasDisciplinaOrd_<compOftDisc>(setOrd, priorAluno, equiv);

	// criar abridor turmas
	AbridorTurmas abridorTurmas(this, equiv, usarVirtual, realoc, formAnySize, priorAluno, relaxMin);
	abridorTurmas.abrirTurmas<compOftDisc>(setOrd);

	// colocar a solução num estado válido
	acertarSolucao_(false, relaxMin);

	checkStats();
}
// criar turmas dando uma ordem randomica às ofertas disciplina.
void SolucaoHeur::abrirTurmasRandom_(bool equiv, bool usarVirtual, bool realoc, bool formAnySize, int priorAluno, double relaxMin)
{
	HeuristicaNuno::logMsg("", 1);
	HeuristicaNuno::logMsg(">>> Abrir turmas (random order)...", 1);

	// reset indicadores de demandas das salas. Reset demandas nao atendidas das ofertas
	if(ParametrosHeuristica::indicDemSalas)
	{
		resetIndicDemSalas_();
		resetCountDemsOfts_();
	}
	// get turmas em ordem randomica
	vector<OfertaDisciplina*> copyOfts (allOfertasDisc_);
	vector<OfertaDisciplina*> ordemOfts;
	while(copyOfts.size() > 0)
	{
		int rdIdx = rand() % copyOfts.size();
		auto it = next(copyOfts.begin(), rdIdx);
		ordemOfts.push_back(*it);
		copyOfts.erase(it);
	}

	// criar abridor turmas
	AbridorTurmas abridorTurmas(this, equiv, usarVirtual, realoc, formAnySize, priorAluno, relaxMin);
	abridorTurmas.abrirTurmasRandom(ordemOfts);

	acertarSolucao_(false, relaxMin);

	checkStats();
}
// abrir turmas numa fase final, que tem que ficar viável.
void SolucaoHeur::abrirTurmasFinal_(bool equiv, bool usarVirtual, bool realoc)
{
	HeuristicaNuno::logMsg("", 1);
	HeuristicaNuno::logMsg(">>> Abrir turmas final...", 1);

	// reset indicadores de demandas das salas. Reset demandas nao atendidas das ofertas
	if(ParametrosHeuristica::indicDemSalas)
	{
		resetIndicDemSalas_();
		resetCountDemsOfts_();
	}

	// set ofertas ordenadas
	set<OfertaDisciplina*, compOftDisc> setOrd;

	// removidos por co-requisito incompleto
	unordered_map<OfertaDisciplina*, unordered_set<int>> removidosCoReq;

	bool remCoReq = true;
	bool stop = false;
	int demAntes = 0;
	int demDepois = 10;
	int diff = 10;
	int it = 0;
	while(remCoReq && !stop && it < ParametrosHeuristica::maxIterAbrirTurmasFinal)
	{
		HeuristicaNuno::logMsgInt("> iteração ", ++it, 1);
		remCoReq = false;
		demAntes = nrDemandasAtendidas(1);

		// criar abridor turmas
		setOrd.clear();
		getOfertasDisciplinaOrd_<compOftDisc>(setOrd, 0, equiv);
		AbridorTurmas abridorTurmas(this, equiv, usarVirtual, realoc, true);
		abridorTurmas.abrirTurmas<compOftDisc>(setOrd);
		// setar blacklist
		if(removidosCoReq.size() > 0 && (diff == 0))
			abridorTurmas.setBlacklist(removidosCoReq);

		demDepois = nrDemandasAtendidas(1);

		if(demDepois == demAntes)
			stop = true;
		else if(demDepois < demAntes)
			HeuristicaNuno::warning("SolucaoHeur::abrirTurmasFinal_", "Abrir turmas deixou menos demandas atendidas");

		// colocar a solução num estado válido
		acertarSolucao_(removidosCoReq, false);
		if(removidosCoReq.size() > 0)
			remCoReq = true;

		diff = nrDemandasAtendidas(1) - demAntes;
	}

	checkStats();
}

// local search (destroi e reconstroi, só com turmas com professores reais)
void SolucaoHeur::destroiReconstroi_(bool usarVirtual, int priorAluno, int maxSec, bool randomConstroi, double maxPerc, double minPerc)
{
	// log
	HeuristicaNuno::logMsg("", 1);
	stringstream ss;
	ss << ">>> LOCAL SEARCH: Destroy [" << minPerc << ";" << maxPerc << "] e Reconstroi";
	HeuristicaNuno::logMsg(ss.str(), 1);

	unordered_map<OfertaDisciplina*, unordered_map<TurmaHeur*, unordered_set<AlunoHeur*>>> oldAlocs;	// alocações das ofertas destruidas
	vector<OfertaDisciplina*> oftsDesalocadas;															// ofertas desalocadas

	// save current solution
	unordered_map<OfertaDisciplina*, unordered_map<TurmaHeur*, pair<SalaHeur*, unordered_set<AlunoHeur*>>>> currSolucao;
	saveSolucao_(currSolucao);

	double currMaxPerc = maxPerc;
	clock_t begin = clock();
	while((double(clock() - begin) / CLOCKS_PER_SEC) < maxSec)
	{
		// Guardar info da atual solução
		int demsAtendP1 = nrDemandasAtendidas(1);
		double custoCred = stats_->getCustoCreds();
		oldAlocs.clear();
		oftsDesalocadas.clear();

		// DESTRUIR
		double perc = SolucaoHeur::getRandPerc(currMaxPerc, minPerc);
		destroi_(perc, oftsDesalocadas);

		// RECONSTRUIR
		reconstroi_(usarVirtual, priorAluno, randomConstroi);
		verificacao_();

		// ACEITAR ou REPOR ?
		if(acceptReconstroi_(demsAtendP1, custoCred))
		{
			// primeiro deletar as turmas antigas que já não estão registadas
			deleteOldTurmas_(currSolucao);
			// guardar nova current solution
			currSolucao.clear();
			saveSolucao_(currSolucao);
			verificacao_();
			currMaxPerc = minPerc + 0.05;	// min mais 5%
		}
		else
		{
			reporSolucao_(currSolucao);
			verificacao_();
			currMaxPerc += 0.10;
			if(currMaxPerc > maxPerc)
				currMaxPerc = maxPerc;
		}
	}

	HeuristicaNuno::logMsg("", 1);
	HeuristicaNuno::logMsg("Local Search over!", 1);
	HeuristicaNuno::logMsg("", 1);
	printStats();
}
// desaloca totalmente uma percentagem das ofertasDisciplina, randomicamente escolhida no intervalo [minPerc, maxPerc]
void SolucaoHeur::destroi_(double perc, vector<OfertaDisciplina*> &oftsDesalocadas)
{
	HeuristicaNuno::logMsgDouble(">> Destroy: ", perc, 1);

	int nrOftsDesaloc = (int)std::ceil(perc*allOfertasDisc_.size());
	int count = 0;
	// copiar todas as ofertas disc para um vetor
	vector<OfertaDisciplina*> copyOfertas (allOfertasDisc_);

	// enquanto o nr de ofertas nao tiver sido atingido, escolher idx randomicamente e desalocar
	while((count < nrOftsDesaloc) && (copyOfertas.size() > 0))
	{
		int randIdx  = rand() % copyOfertas.size();
		auto it = std::next(copyOfertas.begin(), randIdx);
		OfertaDisciplina* const oferta = *it;
		copyOfertas.erase(it);
		oftsDesalocadas.push_back(oferta);

		// fechar turmas (solução deve ser gravada antes para as turmas antigas não serem deletadas!)
		oferta->fecharAllTurmas();
		++count;
	}
}
// guardar solução
void SolucaoHeur::saveSolucao_(unordered_map<OfertaDisciplina*, unordered_map<TurmaHeur*, pair<SalaHeur*, unordered_set<AlunoHeur*>>>> &currSolucao)
{
	currSolucao.clear();
	unordered_map<TurmaHeur*, pair<SalaHeur*, unordered_set<AlunoHeur*>>> turmasSalasAlunos;
	for(auto it = allOfertasDisc_.cbegin(); it != allOfertasDisc_.cend(); ++it)
	{
		turmasSalasAlunos.clear();
		OfertaDisciplina* const oferta = (*it);
		oferta->getTurmasSalasAlunos(turmasSalasAlunos);
		if(!currSolucao.insert(make_pair(oferta, turmasSalasAlunos)).second)
			HeuristicaNuno::excepcao("SolucaoHeur::saveSolucao_", "Turma, sala e alunos nao guardados!");
	}
}
// volta a abrir turmas (só com professores reais)
void SolucaoHeur::reconstroi_(bool usarVirtual, int priorAluno, bool random)
{
	// usar abridor de turmas standard
	if(!random)
		abrirTurmas_(true, usarVirtual, true, false, priorAluno);
	//tentar alocar as ofertas disciplinas numa ordem randomica
	else
		abrirTurmasRandom_(true, usarVirtual, true, false, priorAluno);
}
// check reconstroi. se solução tiver piorado, repor.
bool SolucaoHeur::acceptReconstroi_(int oldDemAtendP1, double oldCustoCredito)
{
	int demsP1 = nrDemandasAtendidas(1);
	double custoCredito = stats_->getCustoCreds();

	// print
	HeuristicaNuno::logMsg("", 1);
	stringstream ss;
	ss << "[OLD] Demandas P1: " << oldDemAtendP1 << " / Custo credito: " << oldCustoCredito;
	HeuristicaNuno::logMsg(ss.str(), 1);
	stringstream ssNew;
	ssNew << "[NEW] Demandas P1: " << demsP1 << " / Custo credito: " << custoCredito;
	HeuristicaNuno::logMsg(ssNew.str(), 1);

	// se atender mais demandas, ou as mesmas e tiver um custo de crédito menor, aceitar
	bool accept = true;
	int diff = demsP1 - oldDemAtendP1;
	if(diff < 0)
		accept = false;
	else if(diff == 0)
		accept = (custoCredito <= oldCustoCredito);

	if(accept)
		HeuristicaNuno::logMsg("SOLUCAO ACEITE!", 1);

	return accept;
}
// [rejeitada] repor solução anterior.
void SolucaoHeur::reporSolucao_(unordered_map<OfertaDisciplina*, unordered_map<TurmaHeur*, pair<SalaHeur*, unordered_set<AlunoHeur*>>>> const &oldSolucao)
{
	HeuristicaNuno::logMsg("Repor solucao anterior.", 1);
	
	// remover todos os alunos
	removerTodosAlunos_();

	// repor alocações
	unordered_set<TurmaHeur*> turmasOft;
	for(auto it = allOfertasDisc_.cbegin(); it != allOfertasDisc_.cend(); ++it)
	{
		OfertaDisciplina* const oferta = (*it);
		auto itOld = oldSolucao.find(oferta);
		if(itOld == oldSolucao.end())
			HeuristicaNuno::excepcao("SolucaoHeur::reporSolucao_", "Alocacao antiga da oferta nao encontrada");

		// get todas as turmas da oferta registadas
		turmasOft.clear();
		oferta->getTurmas(turmasOft);

		// procurar turmas antigas não registadas agora
		for(auto itTurma = itOld->second.begin(); itTurma != itOld->second.end(); ++itTurma)
		{
			if(turmasOft.find(itTurma->first) == turmasOft.end())
				turmasOft.insert(itTurma->first);
		}

		// verificar as turmas que estão registadas
		for(auto itTurma = turmasOft.begin(); itTurma != turmasOft.end(); ++itTurma)
		{
			TurmaHeur* turma = (*itTurma);

			auto itOldTurma = itOld->second.find(turma);
			if(itOldTurma == itOld->second.end())
			{
				// Turma Nova criada depois: Fechar!
				turma->setKeep(false);
				oferta->fecharTurma(turma);
				turma = nullptr;
				continue;
			}

			// Turma Antiga: Repor !
			// registar se turma já não estiver na oferta
			if(!oferta->temTurmaReg(turma->getGlobalId()))
			{
				turma->addTurmaProfSala();
				oferta->regAberturaTurma(turma);
				if(turma->getNrAlunos() > 0)
					HeuristicaNuno::excepcao("SolucaoHeur::reporSolucao_", "Turma nao registada com alunos!");
			}

			// set old sala
			SalaHeur* const newSala = itOldTurma->second.first;
			if(turma->getSala()->getId() != newSala->getId())
				turma->setSala(newSala);

			// adicionar alunos
			for(auto itAluno = itOldTurma->second.second.begin(); itAluno != itOldTurma->second.second.end(); ++itAluno)
				oferta->addAlunoTurma(*itAluno, turma, "SolucaoHeur::reporSolucao_");
		}
	}

	// CHECK
	checkStats();
}
// [aceite] deleta os objectos que já não serão usados
void SolucaoHeur::deleteOldTurmas_(unordered_map<OfertaDisciplina*, unordered_map<TurmaHeur*, pair<SalaHeur*, unordered_set<AlunoHeur*>>>> &currSolucao)
{
	HeuristicaNuno::logMsg("delete old turmas...", 1);
	for(auto itOft = currSolucao.begin(); itOft != currSolucao.end(); ++itOft)
	{
		OfertaDisciplina* const oferta = itOft->first;
		for(auto it = itOft->second.begin(); it != itOft->second.end();)
		{
			TurmaHeur* turma = it->first;
			// set todas as turmas em false
			turma->setKeep(false);
			// turma já não está registada, deletar!
			if(!oferta->temTurmaReg(turma->getGlobalId()))
			{
				itOft->second.erase(it++);
				delete turma;
				turma = nullptr;
				continue;
			}
			else
				++it;
		}
	}
}

// escolher randomicamente uma percentagem no intervalo [minPerc, maxPerc]
double SolucaoHeur::getRandPerc(double maxPerc, double minPerc)
{
	if(maxPerc < minPerc)
		HeuristicaNuno::excepcao("SolucaoHeur::getRandPerc", "maxPerc menor que minPerc!");

	// setar minPerc = 0.0 caso valor não esteja adequado
	if((minPerc > 1.0) || (minPerc < 0.0))
		minPerc = 0.0;

	// setar maxPerc = 0.20 caso valor não esteja adequado
	if((maxPerc > 1.0) || (maxPerc <= 0.0))
		maxPerc = 0.20;

	double f = (double)rand() / RAND_MAX;
    return minPerc + f * (maxPerc - minPerc);
}

string SolucaoHeur::getProxMIPNome(string nomeBase)
{
	static int counter=0;
	counter++;
	stringstream ss;
	ss << nomeBase << counter;
	return ss.str();
}

// re-alocar alunos e fechar turmas inválidas (so considerar demandas nao atendidas com prior <= priorAluno)
void SolucaoHeur::realocarAlunosMIP_(SaveSolucao* const &solucaoAtual, bool realocSalas, int minRecCred, int priorAluno, bool alocarP2)
{
	HeuristicaNuno::logMsg("", 1);
	HeuristicaNuno::logMsg("Realocar alunos com MIP...", 1);

	// contar número de turmas inválidas
	HeuristicaNuno::logMsgInt("Nr turmas invalidas: ", nrTurmasInvalidas(), 1);
		
	string nome = getProxMIPNome("MIPAlocarAlunos");

	// realocá-los
	HeuristicaNuno::logMsg("Criar MIP... ", 1);
	MIPAlocarAlunos mipI (nome.c_str(), this, solucaoAtual, realocSalas, minRecCred, priorAluno, alocarP2);
	mipI.alocar();

	// verificar
	verificacao_();

	// acertar
	acertarSolucao_();

	checkStats();
}

// alocar professores no final
void SolucaoHeur::alocarProfessores(void)
{
	// (1) Minimizar turmas com prof virtual
	HeuristicaNuno::logMsg("Realocar profs MIP...", 1);
	realocarProfsMIP_(false);

	// (2) Criar profs individualizados
	if(ParametrosHeuristica::profsVirtuaisIndiv && (stats_->nrTurmasAbertas_ != stats_->nrTurmasProfReal_))
	{
		HeuristicaNuno::logMsg("Criar profs individualizados...", 1);
		realocarProfsMIP_(true);
	}
}
// re-alocar professores
void SolucaoHeur::realocarProfsMIP_(bool profsVirtuaisIndiv)
{
	// realocar profs
	stringstream sstr;
	sstr << "MIPAlocarProfs" << profsVirtuaisIndiv;
	MIPAlocarProfs mipProfs (sstr.str(), this, profsVirtuaisIndiv);
	mipProfs.alocar();

	checkStats();
}

// try alocar nao atendidos em turmas já abertas
void SolucaoHeur::tryAlocNaoAtendidos_(bool mudarSala, bool equiv, bool realocar, int priorAluno)
{
	HeuristicaNuno::logMsg(">> Tentar alocar demandas nao atendidas em turmas ja abertas!", 1);
	int nrDemsAtend = stats_->nrDemsAtend(1);

	set<OfertaDisciplina*, compOftDisc> setOfertas;
	getOfertasDisciplinaOrd_<compOftDisc>(setOfertas, priorAluno, equiv);
	for(auto it = setOfertas.cbegin(); it != setOfertas.cend(); ++it)
		ImproveMethods::tryAlocNaoAtendidos(this, (*it), mudarSala, equiv, realocar, priorAluno);

	int nrDemsExtraP1 = stats_->nrDemsAtend(1) - nrDemsAtend;

	acertarSolucao_();

	HeuristicaNuno::logMsgInt("nr demandas extra atendidas (P1): ", nrDemsExtraP1, 1);
	HeuristicaNuno::logMsg("", 1);
	printStats();
}

// tentar alocar P2 heuristicamente
void SolucaoHeur::tryAlocP2_(int priorAluno)
{
	HeuristicaNuno::logMsg(">>> Try Aloc P2 (heuristica)", 1);
	int nrDemsAlocP2 = 0;
	unordered_set<AlunoHeur*> alunos;

	// get ofertas ordenadas por demanda P2
	set<OfertaDisciplina*, compOftDiscP2> ofertasOrdP2;
	getOfertasDisciplinaOrd_<compOftDiscP2>(ofertasOrdP2, priorAluno, true);
	for(auto it = ofertasOrdP2.cbegin(); it != ofertasOrdP2.cend(); ++it)
	{
		OfertaDisciplina* const oferta = (*it);
		alunos.clear();
		// get alunos com demanda P2 não atendida e disponíveis
		getAlunosDemP2_(oferta, priorAluno, alunos);

		// tentar alocar os alunos nas turmas já abertas respeitando a legalidade
		int nrComp = oferta->nrAlunosCompleto();
		ImproveMethods::tryAlocNaoAtendSemRealoc(oferta, alunos, true, false);
		// remover os incompletos!
		oferta->removerAlunosIncompleto();
		int nrNewComp = oferta->nrAlunosCompleto();
		if(nrNewComp < nrComp)
			HeuristicaNuno::warning("SolucaoHeur::tryAlocP2_", "Demandas completas da oferta diminuiram.");
		else
			nrDemsAlocP2 += (nrNewComp - nrComp);
	}

	HeuristicaNuno::logMsgInt("# demandas P2 aloc: ", nrDemsAlocP2, 1);

	// remover co-requisitos incompletos
	unordered_map<OfertaDisciplina*, unordered_set<int>> coReqsRem;
	this->removerCoRequisitosIncompletos_(coReqsRem, false);

	printStats();
}
// get alunos com demanda P2 nao atendida
void SolucaoHeur::getAlunosDemP2_(OfertaDisciplina* const oferta, int priorAluno, unordered_set<AlunoHeur*> &alunos)
{
	// obter as demandas
	unordered_set<AlunoDemanda*> alunosDem;
	getDemandasNaoAtendidas(oferta->getCampus(), oferta->getDisciplina(), 2, alunosDem, true, priorAluno);
	getDemandasNaoAtendidas(oferta->getCampus(), oferta->getDisciplina(), -2, alunosDem, true, priorAluno);

	// verificar os alunos e adicioná-los ao set
	int nrCredsOft = oferta->getNrCreds();
	for(auto it = alunosDem.cbegin(); it != alunosDem.cend(); ++it)
	{
		AlunoDemanda* const dem = (*it);
		auto itAluno = alunosHeur.find(dem->getAlunoId());
		if(itAluno == alunosHeur.end())
			HeuristicaNuno::excepcao("SolucaoHeur::getAlunosDemP2_", "Aluno nao encontrado");

		AlunoHeur* const aluno = itAluno->second;

		// já alocado
		if(aluno->jaAlocDemanda(dem))
			continue;

		// check se foi atendidos a todas de P1
		if(aluno->getNrDemAtendP1() == aluno->getNrDemsP1())
			continue;

		// check se já excede o maximo de creditos
		if(aluno->excedeMaxCreds(nrCredsOft))
			continue;

		// inserir aluno
		alunos.insert(aluno);
	}
}

// reset indice demanda das salas
void SolucaoHeur::resetIndicDemSalas_(void)
{
	for(auto it = salasHeur.begin(); it != salasHeur.end(); ++it)
		it->second->resetIndicDem();
}
// reset indice demandas das ODs
void SolucaoHeur::resetCountDemsOfts_(void)
{
	for(auto itCampus = ofertasDisciplina_.begin(); itCampus != ofertasDisciplina_.end(); ++itCampus)
	{
		for(auto itDisc = itCampus->second.begin(); itDisc != itCampus->second.end(); ++itDisc)
		{
			OfertaDisciplina* const oferta = itDisc->second;
			oferta->clearDemandasNaoAtend();
		}
	}
}

// -------------------------------------------------------------------------------------------



// ------------------------------------- UPDATE DEMANDAS ATENDIDAS ------------------------------------------

// pega na demanda não atendida original, apaga essa e todas as outras demandas equivalentes
void SolucaoHeur::apagarDemandaNaoAtendEquiv_(Campus* const campus, Disciplina* const disciplina, AlunoHeur* const aluno)
{
	// get demanda
	AlunoDemanda* const dem = aluno->getDemandaOriginal(campus, disciplina);
	int prioridade = dem->getPrioridade();
	Disciplina* const discOriginal = dem->demanda->disciplina;

	if(!dem->getExigeEquivalenciaForcada() && (prioridade != 0))
	{
		apagarDemandaNaoAtend_(campus, discOriginal, prioridade, aluno->getId());
		// por curso
		apagarDemandaNaoAtendPorCurso_(campus, discOriginal, aluno->getCurso(), prioridade, aluno->getId());
	}

	for(auto itSub = dem->demanda->discIdSubstitutasPossiveis.begin(); itSub != dem->demanda->discIdSubstitutasPossiveis.end(); ++itSub)
	{
		// id
		if(itSub->first < 0)
			continue;

		Disciplina* const discEquiv = itSub->second;

		// é original também ou não está associada a esta demanda!
		if(!DadosHeuristica::checkEquivalencia(dem, campus, discEquiv, aluno->getId()))
			continue;

		apagarDemandaNaoAtend_(campus, discEquiv, -prioridade, aluno->getId());
		// por curso
		apagarDemandaNaoAtendPorCurso_(campus, discEquiv, aluno->getCurso(), -prioridade, aluno->getId());
	}

	// contabilizar demandas atendidas
	if(prioridade != 0)
		stats_->addDemandaAtendPrior(prioridade);
}
// apaga demanda não atendida
void SolucaoHeur::apagarDemandaNaoAtend_(Campus* const campus, Disciplina* const disciplina, int prioridade, int alunoId)
{
	// campus
	auto itCampus = demandasNaoAtendidas_.find(campus);
	if(itCampus == demandasNaoAtendidas_.end())
	{
		if(!loading_ /*&& prioridade > 0*/)
		{
			HeuristicaNuno::logMsgInt("prior: ", prioridade, 1);
			HeuristicaNuno::warning("SolucaoHeur::apagarDemandaNaoAtend_", "Campus nao encontrado");
		}
		return;
	}

	// disciplina
	auto itDisc = itCampus->second.find(disciplina);
	if(itDisc == itCampus->second.end())
	{
		if(!loading_ /*&& prioridade > 0*/)
		{
			HeuristicaNuno::logMsgInt("prior: ", prioridade, 1);
			HeuristicaNuno::warning("SolucaoHeur::apagarDemandaNaoAtend_", "Disciplina nao encontrada");
		}
		return;
	}

	// prioridade
	auto it = itDisc->second.find(prioridade);
	if(it == itDisc->second.end())
	{
		if(!loading_ /*&& prioridade > 0*/)
		{
			HeuristicaNuno::logMsgInt("prior: ", prioridade, 1);
			HeuristicaNuno::warning("SolucaoHeur::apagarDemandaNaoAtend", "Prioridade nao encontrada!");
		}
		return;
	}

	// demandas
	auto itDem = it->second.begin();
	for(; itDem != it->second.end(); ++itDem)
	{
		if((*itDem)->getAlunoId() == alunoId)
			break;
	}
	if(itDem == it->second.end())
	{
		if(!loading_ /*&& prioridade > 0*/)
		{
			HeuristicaNuno::logMsgInt("prior: ", prioridade, 1);
			HeuristicaNuno::logMsgInt("aluno id: ", alunoId, 1);
			HeuristicaNuno::logMsgInt("disciplina id: ", disciplina->getId(), 1);
			HeuristicaNuno::warning("SolucaoHeur::apagarDemandaNaoAtend", "Demanda nao atendida nao atendida nao encontrada para esta disc prioridade!");
		}
		return;
	}

	it->second.erase(itDem);
}

// apaga demanda não atendida por curso!
void SolucaoHeur::apagarDemandaNaoAtendPorCurso_(Campus* const campus, Disciplina* const disciplina, Curso* const curso, 
													int prioridade, int alunoId)
{
	// campus
	auto itCampus = demsNaoAtendPorCurso_.find(campus);
	if(itCampus == demsNaoAtendPorCurso_.end())
	{
		if(!loading_ /*&& prioridade > 0*/)
		{
			HeuristicaNuno::logMsgInt("prior: ", prioridade, 1);
			HeuristicaNuno::warning("SolucaoHeur::apagarDemandaNaoAtendPorCurso_", "Campus nao encontrado");
		}
		return;
	}

	// disciplina
	auto itDisc = itCampus->second.find(disciplina);
	if(itDisc == itCampus->second.end())
	{
		if(!loading_ /*&& prioridade > 0*/)
		{
			HeuristicaNuno::logMsgInt("prior: ", prioridade, 1);
			HeuristicaNuno::warning("SolucaoHeur::apagarDemandaNaoAtendPorCurso_", "Disciplina nao encontrada");
		}
		return;
	}

	// curso
	auto itCurso = itDisc->second.find(curso);
	if(itCurso == itDisc->second.end())
	{
		if(!loading_ /*&& prioridade > 0*/)
		{
			HeuristicaNuno::logMsgInt("prior: ", prioridade, 1);
			HeuristicaNuno::warning("SolucaoHeur::apagarDemandaNaoAtendPorCurso_", "Curso nao encontrado");
		}
		return;
	}

	// prioridade
	auto it = itCurso->second.find(prioridade);
	if(it == itCurso->second.end())
	{
		if(!loading_ /*&& prioridade > 0*/)
		{
			HeuristicaNuno::logMsgInt("prior: ", prioridade, 1);
			HeuristicaNuno::warning("SolucaoHeur::apagarDemandaNaoAtendPorCurso_", "Prioridade nao encontrada");
		}
		return;
	}

	// demandas
	auto itDem = it->second.begin();
	for(; itDem != it->second.end(); ++itDem)
	{
		if((*itDem)->getAlunoId() == alunoId)
			break;
	}
	if(itDem == it->second.end())
	{
		if(!loading_ /*&& prioridade > 0*/)
		{
			HeuristicaNuno::logMsgInt("prior: ", prioridade, 1);
			HeuristicaNuno::logMsgInt("aluno id: ", alunoId, 1);
			HeuristicaNuno::logMsgInt("disciplina id: ", disciplina->getId(), 1);
			HeuristicaNuno::warning("SolucaoHeur::apagarDemandaNaoAtendPorCurso_", "Demanda nao atendida nao atendida nao encontrada para esta disc prioridade!");
		}
		return;
	}

	it->second.erase(itDem);
}

// pega na demanda não atendida original, adiciona essa e todas as outras demandas substitutas
void SolucaoHeur::addDemandaNaoAtendEquiv_(Campus * const campus, Disciplina * const disciplina, AlunoHeur* const aluno)
{
	// get demanda
	AlunoDemanda* const dem = aluno->getDemandaOriginal(campus, disciplina);
	int prioridade = dem->getPrioridade();
	Disciplina* const discOriginal = dem->demanda->disciplina;
	if(!dem->getExigeEquivalenciaForcada() && prioridade != 0)
	{
		addDemandaNaoAtend_(campus, discOriginal, prioridade, aluno, dem);
		// POR CURSO!
		addDemandaNaoAtendPorCurso_(campus, discOriginal, prioridade, aluno, dem);
	}

	for(auto itSub = dem->demanda->discIdSubstitutasPossiveis.begin(); itSub != dem->demanda->discIdSubstitutasPossiveis.end(); ++itSub)
	{
		// id
		if(itSub->first < 0)
			continue;

		Disciplina* const discEquiv = itSub->second;

		// é original também ou não está associada a esta demanda!
		if(!DadosHeuristica::checkEquivalencia(dem, campus, discEquiv, aluno->getId()))
			continue;

		addDemandaNaoAtend_(campus, discEquiv, -prioridade, aluno, dem);
		// por curso!
		addDemandaNaoAtendPorCurso_(campus, discEquiv, -prioridade, aluno, dem);
	}

	// contabilizar demandas atendidas
	if(prioridade != 0)
		stats_->removeDemandaAtendPrior(prioridade);
}
// adiciona demanda não atendida
void SolucaoHeur::addDemandaNaoAtend_(Campus * const campus, Disciplina * const disciplina, int prioridade, AlunoHeur* const aluno, 
										AlunoDemanda* const demanda)
{
	// campus
	auto itCampus = demandasNaoAtendidas_.find(campus);
	if(itCampus == demandasNaoAtendidas_.end())
	{
		HeuristicaNuno::logMsgInt("prior: ", prioridade, 1);
		HeuristicaNuno::warning("SolucaoHeur::addDemandaNaoAtend_", "Campus nao encontrado");
		return;
	}

	// disciplina
	auto itDisc = itCampus->second.find(disciplina);
	if(itDisc == itCampus->second.end())
	{
		HeuristicaNuno::logMsgInt("prior: ", prioridade, 1);
		HeuristicaNuno::warning("SolucaoHeur::addDemandaNaoAtend_", "Disciplina nao encontrada");
		return;
	}

	// prioridade
	auto it = itDisc->second.find(prioridade);
	if(it == itDisc->second.end())
	{
		HeuristicaNuno::logMsgInt("prior: ", prioridade, 1);
		HeuristicaNuno::warning("SolucaoHeur::addDemandaNaoAtend_", "Prioridade nao encontrada");
		return;
	}
	it->second.insert(demanda);	
}

// adiciona demanda não atendida por curso!
void SolucaoHeur::addDemandaNaoAtendPorCurso_(Campus * const campus, Disciplina * const disciplina, int prioridade, AlunoHeur* const aluno,
							AlunoDemanda* const demanda)
{
	// campus
	auto itCampus = demsNaoAtendPorCurso_.find(campus);
	if(itCampus == demsNaoAtendPorCurso_.end())
	{
		HeuristicaNuno::logMsgInt("prior: ", prioridade, 1);
		HeuristicaNuno::warning("SolucaoHeur::addDemandaNaoAtendPorCurso_", "Campus nao encontrado");
		return;
	}

	// disciplina
	auto itDisc = itCampus->second.find(disciplina);
	if(itDisc == itCampus->second.end())
	{
		HeuristicaNuno::logMsgInt("prior: ", prioridade, 1);
		HeuristicaNuno::warning("SolucaoHeur::addDemandaNaoAtendPorCurso_", "Disciplina nao encontrada");
		return;
	}

	// curso
	auto itCurso = itDisc->second.find(aluno->getCurso());
	if(itCurso == itDisc->second.end())
	{
		HeuristicaNuno::logMsgInt("prior: ", prioridade, 1);
		HeuristicaNuno::warning("SolucaoHeur::addDemandaNaoAtendPorCurso_", "Curso nao encontrado");
		return;
	}

	// prioridade
	auto it = itCurso->second.find(prioridade);
	if(it == itCurso->second.end())
	{
		HeuristicaNuno::logMsgInt("prior: ", prioridade, 1);
		HeuristicaNuno::warning("SolucaoHeur::addDemandaNaoAtendPorCurso_", "Prioridade nao encontrada");
		return;
	}
	it->second.insert(demanda);	
}

// -------------------------------------------------------------------------------------------


// ------------------------------------------ AJUSTAMENTO DA SOLUÇÃO ------------------------------------------

// dar ajustes à solução para eliminar casos que a façam inviável / incompleta
void SolucaoHeur::acertarSolucao_(bool fixados, double relaxMin)
{
	unordered_map<OfertaDisciplina*, unordered_set<int>> coReqsInc;
	acertarSolucao_(coReqsInc, fixados, relaxMin);
}
void SolucaoHeur::acertarSolucao_(unordered_map<OfertaDisciplina*, unordered_set<int>> &coReqsInc, bool fixados, double relaxMin)
{
	// reset
	HeuristicaNuno::logMsg("Acertar solucao...", 1);
	int nrTurmas = stats_->nrTurmasAbertas_;
	int demsP1 = stats_->nrDemsAtend(1);

	bool mudou = true;
	while(mudou)
	{
		mudou = false;

		// remover co-requisitos incompletos
		if(HeuristicaNuno::probData->parametros->considerarCoRequisitos)
		{
			HeuristicaNuno::logMsg("Remover atendimentos incompletos de co-requisitos", 1);
			if(removerCoRequisitosIncompletos_(coReqsInc, fixados))
				mudou = true;
		}

		// remover alunos que estão alocados em uma turma teórica mas não estão alocados à prática
		for(auto itOft = allOfertasDisc_.cbegin(); itOft != allOfertasDisc_.cend(); ++itOft)
		{
			if((*itOft)->acertarOferta(relaxMin, fixados))
				mudou = true;
		}
	}

	int nrClosed = nrTurmas - stats_->nrTurmasAbertas_;
	int nrDecP1 = demsP1 - stats_->nrDemsAtend(1);
	HeuristicaNuno::logMsgInt("nr turmas closed: ", nrClosed, 1);
	HeuristicaNuno::logMsgInt("dems P1 decrease: ", nrDecP1, 1);
}
// remover alunos que tenham os correquisitos 
bool SolucaoHeur::removerCoRequisitosIncompletos_(unordered_map<OfertaDisciplina*, unordered_set<int>> &removidos, bool fixados)
{
	int nrRem = 0;
	bool mudou = false;
	vector<OfertaDisciplina*> ofertasCoReqInc;
	for(auto itAluno = alunosHeur.cbegin(); itAluno != alunosHeur.cend(); ++itAluno)
	{
		AlunoHeur* const aluno = itAluno->second;
		ofertasCoReqInc.clear();

		// get ofertas de alocações incompletas de co-requisitos
		aluno ->getOfertasCoReqsIncompletos(ofertasCoReqInc);
		if(ofertasCoReqInc.size() == 0)
			continue;

		for(auto it = ofertasCoReqInc.begin(); it != ofertasCoReqInc.end(); ++it)
		{
			OfertaDisciplina* const oferta = *it;
			if(!fixados && oferta->ehFixado(aluno))
				continue;

			oferta->removeAluno(aluno);
			registarRemocao_(removidos, oferta, aluno->getId());
			mudou = true;
		}

		nrRem += ofertasCoReqInc.size();
	}

	HeuristicaNuno::logMsgInt("nr alunos removidos co-requisitos incompletos: ", nrRem, 1);
	return mudou;
}
void SolucaoHeur::registarRemocao_(unordered_map<OfertaDisciplina*, unordered_set<int>> &removidos, OfertaDisciplina* const &oferta, int alunoId)
{
	auto itOft = removidos.find(oferta);
	if(itOft == removidos.end())
	{
		unordered_set<int> alunos;
		auto par = removidos.insert(make_pair(oferta, alunos));
		if(!par.second)
			HeuristicaNuno::excepcao("SolucaoHeur::registarRemocao_", "Oferta nao inserida !");
		itOft = par.first;
	}

	itOft->second.insert(alunoId);
}
// remover alunos que estão alocados em uma turma teórica mas não estão alocados à prática ou viceversa
void SolucaoHeur::removerAllAlunosIncompletos_(void)
{
	for(auto it = allOfertasDisc_.cbegin(); it != allOfertasDisc_.cend(); ++it)
	{
		if((*it)->temAlunosIncompleto())
			(*it)->removerAlunosIncompleto(true);
	}
}

// remover todos os alunos das turmas já abertas
void SolucaoHeur::removerTodosAlunos_(bool fixados)
{
	unordered_set<AlunoHeur*> alunosRemovidos;
	for(auto itOft = allOfertasDisc_.cbegin(); itOft != allOfertasDisc_.cend(); ++itOft)
	{
		alunosRemovidos.clear();
		(*itOft)->removerTodosAlunos(alunosRemovidos, fixados);

		// verificação
		(*itOft)->checkConsistencia();
	}
}

// guardar solução actual
void SolucaoHeur::guardarSolucaoAtualAlunos_(SaveSolucao* const &saver) const
{
	// limpar
	saver->alocacoes.clear();
	saver->turmasAssoc.clear();

	int nrAlunosInc = 0;
	unordered_set<AlunoHeur*> alunos;
	unordered_set<TurmaHeur*> turmas;
	unordered_map<TurmaHeur*, unordered_set<TurmaHeur*>> turmasAssocOft;
	for(auto itOft = allOfertasDisc_.begin(); itOft != allOfertasDisc_.end(); ++itOft)
	{
		OfertaDisciplina* const oferta = (*itOft);
		nrAlunosInc += oferta->getAlunosIncompleto().size();

		// get all turmas
		turmas.clear();
		oferta->getTurmas(turmas);

		// get turmas assoc
		turmasAssocOft.clear();
		oferta->getAllTurmasAssoc(turmasAssocOft);
		saver->turmasAssoc.insert(turmasAssocOft.begin(), turmasAssocOft.end());
			
		for(auto itTurmas = turmas.begin(); itTurmas != turmas.end(); ++itTurmas)
		{
			TurmaHeur* const turma = *itTurmas;
			pair<unordered_set<int>, int> innerPar;
			pair<TurmaHeur*, pair<unordered_set<int>, int>> par (turma, innerPar);
			auto itAlTurma = saver->alocacoes.insert(par).first;

			alunos.clear();
			turma->getAlunos(alunos);
			// set alunos
			for(auto itAlunos = alunos.cbegin(); itAlunos != alunos.cend(); ++itAlunos)
			{
				itAlTurma->second.first.insert((*itAlunos)->getId());
			}

			// set sala
			itAlTurma->second.second = turma->getSala()->getId();
		}
	}
	// check
	HeuristicaNuno::logMsgInt("nr alunos incompletos: ", nrAlunosInc, 1);
}

// set todos os professores virtuais e guardar alocação actual dos professores (pré MIPAlocarProfs)
void SolucaoHeur::getAllTurmasProf_(unordered_map<TurmaHeur*, ProfessorHeur*> &turmasProfsAtual, bool setVirtual)
{
	HeuristicaNuno::logMsg(">>> get All Turmas Prof: ", 1);
	//ProfessorHeur* const profVirtUnico = professoresHeur.at(ParametrosHeuristica::profVirtualId);
	ProfessorHeur* profVirtUnico = nullptr;
	auto finder = professoresHeur.find(ParametrosHeuristica::profVirtualId);
	if (finder == professoresHeur.end())
		HeuristicaNuno::excepcao("SolucaoHeur::getAllTurmasProf_", "Professor virtual unico nao encontrado!");
	else profVirtUnico = finder->second;

	for(auto itOft = allOfertasDisc_.cbegin(); itOft != allOfertasDisc_.cend(); ++itOft)
	{
		OfertaDisciplina* const oferta = (*itOft);
		unordered_set<TurmaHeur*> turmasOft;
		oferta->getTurmas(turmasOft);
		for(auto it = turmasOft.begin(); it != turmasOft.end(); ++it)
		{
			// guardar alocação do prof atual
			turmasProfsAtual.insert(make_pair(*it, (*it)->getProfessor()));

			// set virtual?
			if(setVirtual)
			{
				(*it)->setProfessor(profVirtUnico);
				// check
				if((*it)->getProfessor()->getId() != ParametrosHeuristica::profVirtualId)
					HeuristicaNuno::excepcao("SolucaoHeur::setAllTurmasProfVirtual_", "Professor virtual não alocado à turma");
			}
		}
	}
}

// tentar reduzir o tamanho das salas alocadas às turmas já abertas
void SolucaoHeur::tryReduzirSalas_(void)
{
	HeuristicaNuno::logMsg(">> Tentar reduzir salas!", 1);
	// obter ofertas ordenadas inversamente (salas mais pequenas para turma com menos demanda nao atendida)
	set<OfertaDisciplina*, invNrDemandas> setOfertas;
	getOfertasDisciplinaOrd_<invNrDemandas>(setOfertas, true, true);

	// tentar reduzir salas
	for(auto it = setOfertas.cbegin(); it != setOfertas.cend(); ++it)
		ImproveMethods::tryReduzirSalas(*it);
}

// tentar mudar a sala em turmas em que a sala tem horario indisponivel
bool SolucaoHeur::tryMudarSalasIndisp_(void)
{
	bool all = true;
	unordered_set<TurmaHeur*> turmas;
	for(auto itOft = allOfertasDisc_.cbegin(); itOft != allOfertasDisc_.cend(); ++itOft)
	{
		turmas.clear();
		(*itOft)->getTurmas(turmas);
		for(auto it = turmas.cbegin(); it != turmas.cend(); ++it)
		{
			TurmaHeur* const turma = (*it);
			SalaHeur* const sala = turma->getSala();
			// se a sala alocada atualmente não está disponivel nesse horario entao tentar mudar
			if(!sala->estaDisponivelHorarios(turma))
			{
				// tentar mudar para sala menor (ou igual). se nao conseguir, tentar sala maior
				bool ok = ImproveMethods::tryMudarSala(turma, false, true);
				if(!ok)
					ok = ImproveMethods::tryMudarSala(turma, true);

				if(!ok)
					all = false;
			}
		}
	}
	return all;
}

// fechar turmas vazias
void SolucaoHeur::fecharTurmasVazias_(void)
{
	HeuristicaNuno::logMsg(">> Fechar turmas vazias", 1);
	int nr = 0;
	unordered_set<TurmaHeur*> turmas;
	for(auto it = allOfertasDisc_.cbegin(); it != allOfertasDisc_.cend(); ++it)
	{
		OfertaDisciplina* const oferta = (*it);
		turmas.clear();
		for(auto itT = turmas.begin(); itT != turmas.end(); )
		{
			TurmaHeur* turma = *itT;
			if((*itT)->getNrAlunos() == 0)
			{
				oferta->fecharTurma(turma);
				nr++;
			}

			turmas.erase(itT++);
		}
	}
	HeuristicaNuno::logMsgInt("nr turmas closed: ", nr, 1);
}

// -------------------------------------------------------------------------------------------

// ------------------------------ PROFS VIRTUAIS INDIVIDUALIZADOS ------------------------------

// criar copias dos perfis de professores virtuais
void SolucaoHeur::criarCopiasPerfisProfsVirtuaisPorCurso(void)
{
	HeuristicaNuno::logMsg(">>> Criar professores virtuais individualizados: ", 1);
	int totalProfs = 0;
	int nrCursosMin = 0; // cursos com algum minimo
	ProblemData* const probData = CentroDados::getProblemData();

	// para cada curso
	unordered_set<TipoTitulacao*> titulacoes;
	unordered_set<TipoContrato*> contratos;
	unordered_set<Disciplina*> discsCurr;
	for(auto it = probData->cursos.begin(); it != probData->cursos.end(); ++it)
	{
		Curso* const curso = (*it);
		// obter perfis relevantes
		titulacoes.clear();
		contratos.clear();
		getTitulacoesContratosCurso(curso, titulacoes, contratos);

		// disciplinas do curso
		discsCurr.clear();
		UtilHeur::getDisciplinasCurso(curso, discsCurr, true);

		// nr de professores a criar por curso? nr de turmas abertas de disciplinas desse curso
		int nrProfs = nrTurmasCurso(curso, true);
		if(nrProfs == 0)
			continue;

		// criar n professores para cada perfil
		for(auto itTit = titulacoes.cbegin(); itTit != titulacoes.cend(); ++itTit)
		{
			if(*itTit == nullptr || *itTit == NULL)
			{
				HeuristicaNuno::warning("SolucaoHeur::criarCopiasPerfisProfsVirtuaisPorCurso", "Tipo de titulacao nulo");
				continue;
			}

			for(auto itCon = contratos.cbegin(); itCon != contratos.cend(); ++itCon)
			{
				if(*itCon == nullptr || *itCon == NULL)
				{
					HeuristicaNuno::warning("SolucaoHeur::criarCopiasPerfisProfsVirtuaisPorCurso", "Tipo de contrato nulo");
					continue;
				}

				criaProfessoresVirtuaisPorCurso(nrProfs, *itTit, *itCon, curso, discsCurr);
				totalProfs += nrProfs;
			}
		}
	}
	HeuristicaNuno::logMsgInt("nr profs virtuais individualizados: ", totalProfs, 1);
}
// criar copias de perfis. (todos os campi!)
void SolucaoHeur::criaProfessoresVirtuaisPorCurso(int n, TipoTitulacao* const titulacao, TipoContrato* const contrato, Curso* const curso,
													unordered_set<Disciplina*> const &discsCurr)
{
	ProblemData* const probData = CentroDados::getProblemData();
	if(contrato == nullptr || contrato == NULL)
		HeuristicaNuno::excepcao("SolucaoHeur::criaProfessoresVirtuaisPorCurso", "Contrato nulo!");
	if(titulacao == nullptr || titulacao == NULL)
		HeuristicaNuno::excepcao("SolucaoHeur::criaProfessoresVirtuaisPorCurso", "Titulacao nula!");

	// profs virtuais curso/perfil
	PerfilVirtual* perfil = new PerfilVirtual(titulacao->getTitulacao(), contrato->getContrato(), curso);
	auto itPerfil = profsVirtuaisCursoPerfil.find(perfil);
	if(itPerfil == profsVirtuaisCursoPerfil.end())
	{
		set<ProfessorHeur*, cmpProfIds> emptySet;
		auto par = profsVirtuaisCursoPerfil.insert(make_pair(perfil, emptySet));
		if(!par.second)
			HeuristicaNuno::excepcao("SolucaoHeur::criaProfessoresVirtuaisPorCurso", "Perfil nao inserido em profs virtuais curso!");
		itPerfil = par.first;
	}

	int titId = titulacao->getId();

	// criar professores
	GGroup<Disciplina*, LessPtr<Disciplina>> emptyDisc;
	for ( int p=1; p <= n; ++p )
	{
		Professor* const professor = new Professor( true );
		--idVirtual_;
		while((idVirtual_ == ParametrosHeuristica::profVirtualId) ||  (professoresHeur.find(idVirtual_) != professoresHeur.end()) 
				|| (probData->profs_virtuais_ids.find(idVirtual_) != probData->profs_virtuais_ids.end()))
		{
			--idVirtual_;
		}
		professor->setId( idVirtual_ );
		professor->setTitulacaoId(titId);
		professor->titulacao = titulacao;

		std::string nome = professor->getNome();
		stringstream ss; 
		ss << nome << idVirtual_;
		professor->setNome( ss.str() );

		professor->setCursoAssociado( curso );
		professor->tipo_contrato = contrato;
		professor->setTipoContratoId( contrato->getId() );
		professor->setChMax(1000);
		professor->setChMin(0);
		professor->setMinCredsDiarios(1);
		professor->setMaxDiasSemana(6);
		professor->setChAnterior(0);

		// ainda é usado ?
		auto parItProfDisc = probData->mapProfessorDisciplinasAssociadas.insert(make_pair(professor, emptyDisc));
		if(!parItProfDisc.second)
			HeuristicaNuno::warning("SolucaoHeur::criaProfessoresVirtuaisPorCurso", "Professor nao adicionado a mapProfessorDisciplinasAssociadas");
		auto itProfDisc = parItProfDisc.first;

		// adicionar o magisterio
		for(auto itDisciplina = discsCurr.begin(); itDisciplina != discsCurr.end(); ++itDisciplina)
		{
			Disciplina* const disciplina = *itDisciplina;
			if(disciplina == nullptr)
				HeuristicaNuno::excepcao("SolucaoHeur::criaProfessoresVirtuaisPorCurso", "Disciplina nula");

			if ( professor->possuiMagisterioEm( disciplina ) )
				continue;

			Magisterio * mag = new Magisterio();
			mag->setId( 1 );
			mag->disciplina = ( *itDisciplina );
			mag->setDisciplinaId( disciplina->getId() );
			mag->setNota( 10 );
			mag->setPreferencia( 1 );
			professor->magisterio.add( mag );

			// mapProfessoresDisciplinas
			itProfDisc->second.add( disciplina );

			// adicionar horários disponivel!
			addHorariosDiscProf(professor, disciplina);
		}

		// associar a todos os campi
		for(auto itCampi = probData->campi.begin(); itCampi != probData->campi.end(); ++itCampi)
		{
			itCampi->professores.add(professor);
		}

		// criar professor heur
		ProfessorHeur* const profHeur = new ProfessorHeur(professor);

		// -------------------------------- guardar nas estruturas -------------------------------
		// profs virtuais
		auto itVirt = profsVirtuais.find(professor->getId());
		if(itVirt != profsVirtuais.end())
			HeuristicaNuno::excepcao("SolucaoHeur::criaProfessoresVirtuaisPorCurso", "Professor virtual com mesmo id ja inserido!");
		profsVirtuais.insert(make_pair(professor->getId(), profHeur));

		// profs virtuais curso
		itPerfil->second.insert(profHeur);

		// todos os profs
		auto itAll = professoresHeur.find(professor->getId());
		if(itAll != professoresHeur.end())
			HeuristicaNuno::excepcao("SolucaoHeur::criaProfessoresVirtuaisPorCurso", "Professor com mesmo id ja inserido!");
		professoresHeur.insert(make_pair(professor->getId(), profHeur));

		probData->professores_virtuais.push_back( professor );
		probData->profs_virtuais_ids.insert(professor->getId());
	}
}
// adicionar horarios de uma discipina
void SolucaoHeur::addHorariosDiscProf(Professor* const professor, Disciplina* const disciplina)
{
	ProblemData* const problemData = CentroDados::getProblemData();
	for(auto itHor = disciplina->horarios.begin(); itHor != disciplina->horarios.end(); ++itHor)
	{
		Horario* const horario = (*itHor);
		auto itHorJa = professor->horarios.find(horario);
		if(itHorJa == professor->horarios.end())
		{
			professor->horarios.add(horario);
			for(auto itDia = horario->dias_semana.begin(); itDia != horario->dias_semana.end(); ++itDia)
			{
				HorarioDia* const hd = problemData->getHorarioDiaCorrespondente( horario->horario_aula, *itDia );
				if(hd != nullptr)
					professor->horariosDia.add( hd );
			}
		}
		else // HorarioAula ja existe para o prof. Adiciona possivelmente mais dias
		{
			for(auto itDia = horario->dias_semana.begin(); itDia != horario->dias_semana.end(); ++itDia)
			{
				auto itDiaHor = itHorJa->dias_semana.find(*itDia);
				if ( itDiaHor == itHorJa->dias_semana.end() )
				{
					itHorJa->dias_semana.add(*itDia);
					HorarioDia* const hd = problemData->getHorarioDiaCorrespondente( itHorJa->horario_aula, *itDia );
					if(hd != nullptr)
						professor->horariosDia.add( hd );
				}
			}
		}
	}

	// Dias Letivos ??
	std::pair< int, int > ids_Prof_Disc( professor->getId(), disciplina->getId() );
	GGroup<int> diasLetivos;
	auto insPar = problemData->prof_Disc_Dias.insert(make_pair(ids_Prof_Disc, diasLetivos));
	if(!insPar.second)
	{
		HeuristicaNuno::warning("SolucaoHeur::addHorariosDiscProf", "Par profvirtual-disciplina nao inserido em problemData->prof_Disc_Dias");
		return;
	}
	for(auto itDias = disciplina->diasLetivos.begin(); itDias != disciplina->diasLetivos.end(); ++itDias )
	{
		insPar.first->second.add( *itDias );
	}
}
// retorna nr de turmas de disciplinas de um determinado curso
int SolucaoHeur::nrTurmasCurso(Curso* const curso, bool soProfVirtual)
{
	int nr = 0;
	// get disciplinas do curso
	unordered_set<Disciplina*> discsCurso;
	UtilHeur::getDisciplinasCurso(curso, discsCurso, true);
	// contar turmas. procurar em cada campus todas as disciplinas do curso
	for(auto itCamp = ofertasDisciplina_.cbegin(); itCamp != ofertasDisciplina_.cend(); ++itCamp)
	{
		for(auto itDisc = discsCurso.cbegin(); itDisc != discsCurso.cend(); ++itDisc)
		{
			auto itOft = itCamp->second.find(*itDisc);
			if(itOft == itCamp->second.cend())
				continue;

			nr += itOft->second->nrTurmasVirt(soProfVirtual);
		}
	}

	return nr;
}
// get titulacoes/tipo contrato curso
void SolucaoHeur::getTitulacoesContratosCurso(Curso* const curso, unordered_set<TipoTitulacao*> &titulacoes, 
									unordered_set<TipoContrato*> &contratos)
{
	ProblemData* const probData = CentroDados::getProblemData();
	// inserir o mínimo para ter um tipo standard
	TipoTitulacao* const titMin = probData->retornaTipoTitulacaoMinimo();
	if(titMin != nullptr && titMin != NULL)
		titulacoes.insert(titMin);
	else
		HeuristicaNuno::excepcao("SolucaoHeur::getTitulacoesContratosCurso", "Titulacao 'Licenciado' nao encontrada");

	TipoContrato* const conMin = probData->retornaTipoContratoMinimo();
	if(conMin != nullptr && conMin != NULL)
		contratos.insert(conMin);
	else
		HeuristicaNuno::excepcao("SolucaoHeur::getTitulacoesContratosCurso", "Tipo de contrato 'Outro' nao encontrado");
	
	// [titulação] mestres
	if(curso->temMinMestres() && (TipoTitulacao::Mestre != titMin->getTitulacao()))
	{
		TipoTitulacao* const tit = probData->retornaTitulacao(TipoTitulacao::Mestre);
		if(tit != nullptr && tit != NULL)
			titulacoes.insert(tit);
		else
			HeuristicaNuno::warning("SolucaoHeur::getTitulacoesContratosCurso", "Titulacao de Mestre nao encontrada");
	}
	// [titulação] doutores
	if(curso->temMinDoutores() && (TipoTitulacao::Doutor != titMin->getTitulacao()))
	{
		TipoTitulacao* const tit = probData->retornaTitulacao(TipoTitulacao::Doutor);
		if(tit != nullptr && tit != NULL)
			titulacoes.insert(tit);
		else
			HeuristicaNuno::warning("SolucaoHeur::getTitulacoesContratosCurso", "Titulacao de Doutor nao encontrada");
	}

	// [contrato] tempo parcial
	if(curso->temMinTempoIntegralParcial() && (TipoContrato::Parcial != conMin->getContrato()))
	{
		TipoContrato* const con = probData->retornaTipoContrato(TipoContrato::Parcial);
		if(con != nullptr && con != NULL)
			contratos.insert(con);
		else
			HeuristicaNuno::warning("SolucaoHeur::getTitulacoesContratosCurso", "Tipo de contrato Parcial nao encontrado");
	}
	// [contrato] tempo integral
	if(curso->temMinTempoIntegral() && (TipoContrato::Integral != conMin->getContrato()))
	{
		TipoContrato* const con = probData->retornaTipoContrato(TipoContrato::Integral);
		if(con != nullptr && con != NULL)
			contratos.insert(con);
		else
			HeuristicaNuno::warning("SolucaoHeur::getTitulacoesContratosCurso", "Tipo de contrato Integral nao encontrado");
	}
}

// -------------------------------------------------------------------------------------------


// ------------------------------------------ MÉTODOS DE VERIFICAÇÃO ------------------------------------------

// verifica a consistência da solução
void SolucaoHeur::verificacao_(bool incompletos) const
{
	bool ok = true;

	HeuristicaNuno::logMsg("Check conflitos", 1);
	if(!checkConflitos())
	{
		HeuristicaNuno::warning("SolucaoHeur::verificacao_", "Solução tem conflitos de horarios ou alunos com excesso de creditos!");
		ok = false;
	}

	if(incompletos)
	{
		HeuristicaNuno::logMsg("Check alocacoes completas e maximo de creditos", 1);
		if(!checkMaxCredsAlocComp())
		{
			HeuristicaNuno::warning("SolucaoHeur::verificacao_", "Solucao tem alunos com alocacoes/co-requisitos incompletos!");
			ok = false;
		}
	}

	HeuristicaNuno::logMsg("Check links", 1);
	if(!checkLinks(incompletos))
	{
		HeuristicaNuno::warning("SolucaoHeur::verificacao_", "Solução tem problemas de linkagem!");
		ok = false;
	}

	HeuristicaNuno::logMsg("Check links", 1);
	if(ParametrosHeuristica::relacPraticas != ParametrosHeuristica::abrePraticas::N_N)
	{
		if(!checkRelacTeorPrat())
		{
			HeuristicaNuno::warning("SolucaoHeur::verificacao_", "Solucao nao respeita relacao de praticas e teoricas!");
			ok = false;
		}
	}

	HeuristicaNuno::logMsg("Check aulas continuas", 1);
	if(!checkAllAulasContinuas())
	{
		HeuristicaNuno::warning("SolucaoHeur::verificacao_", "Solução tem problemas de aulas continuas!");
		ok = false;
	}


	// verificar as estatísticas e imprimir
	checkStats();

	printSolucaoLog_();

	if(!ok)
		HeuristicaNuno::excepcao("SolucaoHeur::verificacao_", "Solução em estado inválido! Abortar!");
}

bool SolucaoHeur::checkLinks(bool incompletos) const
{
	bool ok = true;
	int confOfertas = 0;
	int confTurmas = 0;
	int confSalas = 0;
	int confProfs = 0;
	int confAlunos = 0;
	int nrAlunosInc = 0;	// nr alunos incompletos

	for(auto itOft = allOfertasDisc_.cbegin(); itOft != allOfertasDisc_.cend(); ++itOft)
	{
		OfertaDisciplina* const oferta = (*itOft);
		// check consistencia
		if(!oferta->checkConsistencia() || !oferta->checkCompletos())
		{
			confOfertas++;
			ok = false;
		}

		// contabilizar alunos incompletos
		if(oferta->nrTiposAula() == 2)
			nrAlunosInc += oferta->nrAlunosIncompleto();

		unordered_set<TurmaHeur*> turmas;
		oferta->getTurmas(turmas);
		for(auto itTurmas = turmas.begin(); itTurmas != turmas.end(); ++itTurmas)
		{
			if(!TurmaHeur::checkLink(*itTurmas))
			{
				confTurmas++;
				ok = false;
			}
			if((*itTurmas)->getNrAlunos() > oferta->getMaxAlunos((*itTurmas)->tipoAula))
			{
				HeuristicaNuno::warning("SolucaoHeur::checkLinks", "Turma tem mais alunos do que o maximo permitido para a disciplina!");
				confTurmas++;
				ok = false;
			}
		}
	}

	// Salas
	for(auto itSalas = salasHeur.begin(); itSalas != salasHeur.end(); ++itSalas)
	{
		if(!itSalas->second->checkLinks())
		{
			ok = false;
			//HeuristicaNuno::warning("SolucaoHeur::checkLinks", "Sala com erros de link!");
			//break;
			confSalas++;
		}
	}

	// Professores
	auto itProfs = professoresHeur.begin();
	for(; itProfs != professoresHeur.end(); ++itProfs)
	{
		if(itProfs->second->ehVirtualUnico())
			continue;

		if(!itProfs->second->checkLinks())
		{
			ok = false;
			//HeuristicaNuno::warning("SolucaoHeur::checkLinks","Professor com erros de link!");
			//break;
			confProfs++;
		}
	}

	// Alunos
	auto itAlunos = alunosHeur.begin();
	for(; itAlunos != alunosHeur.end(); ++itAlunos)
	{
		if(!itAlunos->second->checkLinks())
		{
			ok = false;
			//HeuristicaNuno::warning("SolucaoHeur::checkLinks", "Aluno com erros de link!");
			//break;
			confAlunos++;
		}
	}

	if(nrAlunosInc > 0 && incompletos)
		ok = false;

	if(!ok)
	{
		HeuristicaNuno::logMsgInt(">> nr ofertas com erros de link: ", confOfertas, 1);
		HeuristicaNuno::logMsgInt(">> nr turmas com erros de link: ", confTurmas, 1);
		HeuristicaNuno::logMsgInt(">> nr salas com erros de link: ", confSalas, 1);
		HeuristicaNuno::logMsgInt(">> nr profs com erros de link: ", confProfs, 1);
		HeuristicaNuno::logMsgInt(">> nr alunos com erros de link: ", confAlunos, 1);
		HeuristicaNuno::logMsgInt(">> nr alunos incompletos: ", nrAlunosInc, 1);
	}
	else
		HeuristicaNuno::logMsg("Solucao sem conflitos de links.", 1);

	return ok;
}
bool SolucaoHeur::checkConflitos() const
{
	bool ok = true;
	int confSalas = 0;
	int confProfs = 0;
	int confAlunos = 0;

	// Salas
	for(auto itSalas = salasHeur.begin(); itSalas != salasHeur.end(); ++itSalas)
	{
		if(itSalas->second->temConflitos())
		{
			ok = false;
			//HeuristicaNuno::warning("SolucaoHeur::checkConflitos", "Sala com conflitos de horarios!");
			//break;
			confSalas++;
		}
	}

	// Professores
	for(auto itProfs = professoresHeur.begin(); itProfs != professoresHeur.end(); ++itProfs)
	{
		if(itProfs->second->ehVirtualUnico())
			continue;

		if(itProfs->second->temConflitos())
		{
			ok = false;
			//HeuristicaNuno::warning("SolucaoHeur::checkConflitos","Professor com conflitos de horarios!");
			//break;
			confProfs++;
		}
	}

	// Alunos
	for(auto itAlunos = alunosHeur.begin(); itAlunos != alunosHeur.end(); ++itAlunos)
	{
		AlunoHeur* const aluno = itAlunos->second;
		// conflitos de horarios
		if(aluno->temConflitos())
		{
			ok = false;
			//HeuristicaNuno::warning("SolucaoHeur::checkConflitos", "Aluno com conflitos de horarios!");
			//break;
			confAlunos++;
		}
	}

	if(!ok)
	{
		HeuristicaNuno::logMsgInt(">> nr salas com conflito: ", confSalas, 1);
		HeuristicaNuno::logMsgInt(">> nr profs com conflito: ", confProfs, 1);
		HeuristicaNuno::logMsgInt(">> nr alunos com conflito: ", confAlunos, 1);
	}
	else
		HeuristicaNuno::logMsg("Solucao sem conflitos de horarios.", 1);

	return ok;
}
// verificar alocação completa e max creditos
bool SolucaoHeur::checkMaxCredsAlocComp(void) const
{
	bool ok = true;
	int alunosCreds = 0;
	int alunosInc = 0;
	int alunosCoReqInc = 0;

	// check co-requisitos?
	bool checkCoReq = HeuristicaNuno::probData->parametros->considerarCoRequisitos;

	// Alunos
	for(auto itAlunos = alunosHeur.begin(); itAlunos != alunosHeur.end(); ++itAlunos)
	{
		AlunoHeur* const aluno = itAlunos->second;
		// creditos a mais
		if(aluno->getNrCreditosAlocados() > aluno->maxCredsAloc())
		{
			//ok = false;
			alunosCreds++;
			stringstream ss;
			ss << "Aluno " << itAlunos->second->getId() << " alocado a " << aluno->getNrCreditosAlocados() << "! Max: " << aluno->maxCredsAloc();
			HeuristicaNuno::warning("SolucaoHeur::checkConflitos", ss.str());
		}
		// tem alocações incompletas
		if(!aluno->alocCompleto())
		{
			ok = false;
			++alunosInc;
		}
		// tem co-requisitos incompletos
		if(checkCoReq && aluno->temCoReqsIncompletos())
		{
			//ok = false;
			++alunosCoReqInc;
		}
	}

	HeuristicaNuno::logMsgInt(">> nr alunos com co-requisitos incompletos: ", alunosCoReqInc, 1);
	if(!ok)
	{
		HeuristicaNuno::logMsgInt(">> nr alunos com creditos a mais: ", alunosCreds, 1);
		HeuristicaNuno::logMsgInt(">> nr alunos com alocacoes incompletas: ", alunosCreds, 1);
	}
	else
		HeuristicaNuno::logMsg("Solucao sem alocacoes incompletas.", 1);

	return ok;
}
// [só 1x1 e 1xN] verificar se relação é cumprida
bool SolucaoHeur::checkRelacTeorPrat(void) const
{
	for(auto itOft = allOfertasDisc_.cbegin(); itOft != allOfertasDisc_.cend(); ++itOft)
	{
		if(!(*itOft)->checkRelacTeoricasPraticas())
			return false;
	}

	HeuristicaNuno::logMsg("Solucao respeita relacao teoricas x praticas.", 1);
	return true;
}

bool SolucaoHeur::checkAllAulasContinuas(void) const
{
	bool ok=true;
	for(auto itOft = allOfertasDisc_.cbegin(); itOft != allOfertasDisc_.cend(); ++itOft)
	{
		OfertaDisciplina* const oferta = (*itOft);
		
		if (oferta->nrTiposAula() != 2) continue;
		if (!oferta->getDisciplina()->aulasContinuas()) continue;

		unordered_set<TurmaHeur*> turmast;
		oferta->getTurmasTipo(true,turmast);
		for(auto itTeor = turmast.begin(); itTeor != turmast.end(); ++itTeor)
		{
			TurmaHeur* teor = *itTeor;

			ok = ok && checkAulasContPorTurmaTeor(oferta,teor);
		}
	}

	if (ok) HeuristicaNuno::logMsg("Solucao respeita aulas continuas teoricas x praticas.", 1);

	return ok;
}

bool SolucaoHeur::checkAulasContPorTurmaTeor(OfertaDisciplina* const oferta, TurmaHeur* const teor) const
{
	if (oferta->nrTiposAula() != 2)
		return true;
	if (!oferta->getDisciplina()->aulasContinuas())
		return true;

	bool ok=true;
	unordered_map<int, AulaHeur*> aulast;
	teor->getAulas(aulast);

	unordered_set<TurmaHeur*> turmasp;
	oferta->getTurmasAssoc(teor,turmasp);
	for(auto itPrat = turmasp.begin(); itPrat != turmasp.end(); ++itPrat)
	{
		TurmaHeur* prat = *itPrat;
		
		unordered_map<int, AulaHeur*> aulasp;
		prat->getAulas(aulasp);

		if(!checkAulasContinuas(aulasp,aulast))
		{
			stringstream ss;
			ss << "Turma prat " << prat->id << " da disc " << oferta->getDisciplina()->getId()
				 << " nao respeita aulas continuas com turma teor " << teor->id;
			HeuristicaNuno::warning("SolucaoHeur::checkAulasContPorTurmaTeor", ss.str());
			ok = false;
		}
	}
	return ok;
}

bool SolucaoHeur::checkAulasContinuas(unordered_map<int, AulaHeur*> const &aulasp, 
									unordered_map<int, AulaHeur*> const &aulast) const
{
	// verifica se toda aula pratica esta imediatamente após alguma aula teorica
	// atenção: aulas devem ser todas da mesma disciplina e de turmas associadas (1xN).

	bool ok=true;
	for(auto itAulap = aulasp.begin(); itAulap != aulasp.end(); ++itAulap)
	{
		int diap = itAulap->first;
		AulaHeur* aulap = itAulap->second;
		DateTime dti;
		aulap->getPrimeiroHor(dti);

		auto findDiaT = aulast.find(diap);
		if(findDiaT == aulast.end())
		{
			HeuristicaNuno::warning("SolucaoHeur::checkAulasContinuas", "Par pratica-teorica em dias diferentes!");
			ok = false;
		}
		else
		{
			DateTime dtf;
			findDiaT->second->getLastHor(dtf);
			if (dti<dtf || (dti-dtf).timeMin() > ParametrosHeuristica::maxIntervAulas)
			{
				HeuristicaNuno::warning("SolucaoHeur::checkAulasContinuas", "Aulas pratica-teorica nao continuas!");
				ok = false;
			}
		}
	}
	return ok;
}

// verificar stats
void SolucaoHeur::checkStats(bool print) const
{
	stats_->reset();

	HeuristicaNuno::logMsg("", 1);
	HeuristicaNuno::logMsg(">> CHECK STATS:", 1);

	for(auto itCampus = ofertasDisciplina_.begin(); itCampus != ofertasDisciplina_.end(); ++itCampus)
	{
		
		for(auto itDisc = itCampus->second.begin(); itDisc != itCampus->second.end(); ++itDisc)
		{
			OfertaDisciplina* const oferta = itDisc->second;
			if(oferta->nrAlunosIncompleto() > 0)
				HeuristicaNuno::warning("SolucaoHeur::checkStats", "Oferta tem alunos incompletos!");

			unordered_set<TurmaHeur*> turmas;
			oferta->getTurmas(turmas);

			stats_->nrAlunosIncompletos_ += oferta->nrAlunosIncompleto();
			
			for(auto itTurma = turmas.begin(); itTurma != turmas.end(); ++itTurma)
			{
				TurmaHeur* const turma = *itTurma;
				int creds = turma->getNrCreditos();
				stats_->nrCreditosProfessores_ += creds;
				stats_->nrCreditosAlunos_ += creds * turma->getNrAlunos();
				if(turma->getProfessor()->ehVirtual())
					stats_->nrCreditosProfsVirtuais_ += creds;

				stats_->nrTurmasAbertas_++;
				if(turma->ehIlegal())
				{
					stats_->nrTurmasInvalidas_++;
					//HeuristicaNuno::excepcao("SolucaoHeur::checkStats", "Turma ilegal!");
				}

				if(!(*itTurma)->tipoAula && oferta->nrTiposAula() == 2)
					stats_->nrTurmasAbertasCompSec_++;

				if(!(*itTurma)->getProfessor()->ehVirtual())
					stats_->nrTurmasProfReal_++;
			}

			unordered_set<AlunoHeur*> alunos;
			oferta->getAlunos(alunos);
			for(auto itAluno = alunos.begin(); itAluno != alunos.end(); ++itAluno)
			{
				int prior = (*itAluno)->getDemandaOriginal(oferta->getCampus(), oferta->getDisciplina())->getPrioridade();
				if(prior != 0)
					stats_->addDemandaAtendPrior(prior);
			}
		}
	}
	// imprimir stats revistos
	if(print)
		printStats();
}


// verificar e loggar turmas carregadas que tenham sido fechadas
void SolucaoHeur::checkClosedTurmasLoad(void) const
{
	HeuristicaNuno::logMsg(">> Check Turma carregadas fechadas!", 1);
	HeuristicaNuno::logMsgInt("nr turmas load fechadas: ", loadedTurmasClosed.size(), 1);
	for(auto it = loadedTurmasClosed.begin(); it != loadedTurmasClosed.end(); ++it)
	{
		HeuristicaNuno::logFecharTurmaLoad(this, *it);
	}
}
// verificar e loggar mudanças de sala em turmas carregadas de solução fixada
void SolucaoHeur::checkLogMudancasSala(void) const
{
	HeuristicaNuno::logMsg(">> Check mudanças de sala de turmas carregadas!", 1);

	unordered_set<TurmaHeur*> turmas;
	int nr = 0;
	for(auto it = allOfertasDisc_.begin(); it != allOfertasDisc_.end(); ++it)
	{
		turmas.clear();
		(*it)->getTurmas(turmas);
		for(auto itT = turmas.cbegin(); itT != turmas.cend(); ++itT)
		{
			TurmaHeur* const turma = *itT;
			if(!turma->carregada || (turma->salaLoaded == nullptr))
				continue;

			SalaHeur* const sala = turma->getSala();
			if(sala->getId() != turma->salaLoaded->getId())
			{
				HeuristicaNuno::logNovaSala(turma, turma->salaLoaded, sala);
				nr++;
			}
		}
	}
	HeuristicaNuno::logMsgInt("nr mudancas: ", nr, 1);
}

// ---------------------------------------------------------------------------------------------------------


// ------------------------------------------ CRIAR OUTPUT ------------------------------------------


// cria o output dos atendimentos
void SolucaoHeur::criarOutput_(ProblemSolution* const solution) const
{
	HeuristicaNuno::logMsg("Criar output atendimento da solução!", 1);
	GGroup<AtendimentoCampus *>* const outputAtendimento = solution->atendimento_campus;
	
	// Problem Data
	ProblemData* const probData = HeuristicaNuno::probData;

	// Cenário id
	solution->setCenarioId( HeuristicaNuno::probData->getCenarioId() );

	unordered_set<TurmaHeur*> turmas;
	unordered_map<int, AulaHeur*> aulas;
	unordered_map<Demanda*, set<AlunoDemanda*>> alunosDemanda;

	// CAMPUS
	for(auto itCampus = ofertasDisciplina_.begin(); itCampus != ofertasDisciplina_.end(); ++itCampus)
	{
		Campus* const campus = itCampus->first;
		AtendimentoCampus* const atendCampus = new AtendimentoCampus(campus->getId());
		atendCampus->campus = campus;
		atendCampus->setCampusId(campus->getCodigo());
		outputAtendimento->add(atendCampus);

		// DISCIPLINAS / OFERTAS
		for(auto itDisc = itCampus->second.begin(); itDisc != itCampus->second.end(); ++itDisc)
		{
			Disciplina* const disciplina = itDisc->first;
			OfertaDisciplina* const ofertaDisc = itDisc->second;

			// TURMAS
			turmas.clear();
			ofertaDisc->getTurmas(turmas);
			for(auto itTurmas = turmas.begin(); itTurmas != turmas.end(); ++itTurmas)
			{
				TurmaHeur* const turma = *itTurmas;
				ProfessorHeur* const professor = turma->getProfessor();
				if(professor == nullptr)
					HeuristicaNuno::excepcao("SolucaoHeur::criarOutput_", "Professor e nulo !");
				int unidadeId = turma->unidadeId();

				// get atendimento unidade
				AtendimentoUnidade* const atendUnid = atendCampus->getAddAtendUnidade(unidadeId);
				auto itUnid = probData->refUnidade.find(unidadeId);
				if(itUnid != probData->refUnidade.end())
				{
					atendUnid->unidade = itUnid->second;
					atendUnid->setCodigoUnidade(itUnid->second->getCodigo());
				}
				else
					HeuristicaNuno::warning("SolucaoHeur::criarOutput_", "Unidade nao encontrada");

				// get atendimento sala
				AtendimentoSala* const atendSala = atendUnid->getAddAtendSala(turma->getSala()->getId());
				atendSala->sala = turma->getSala()->getSala();
				atendSala->setSalaId(turma->getSala()->getSala()->getCodigo());

				// get alunos demanda satisfeitas por oferta
				alunosDemanda.clear();
				turma->getDemandasAlunos(alunosDemanda);

				// AULAS
				aulas.clear();
				turma->getAulas(aulas);
				for(auto itAulas = aulas.cbegin(); itAulas != aulas.cend(); ++itAulas)
				{
					int dia = itAulas->first;
					AulaHeur* const aula = itAulas->second;

					// get id turno. NOTA: geração não tem mt em conta os turnos na discretização do output
					DateTime primHorario;
					aula->getPrimeiroHor(primHorario);
					int turnoId = turma->getCalendario()->getTurnoIES(primHorario);
					auto itTurno = HeuristicaNuno::probData->refTurnos.find(turnoId);
					if(itTurno == HeuristicaNuno::probData->refTurnos.end())
						HeuristicaNuno::excepcao("SolucaoHeur::criarOutput_", "Turno nao criado");

					TurnoIES* const turno = itTurno->second;

					// get atendimento diaSemana
					AtendimentoDiaSemana* const atendDiaSem = atendSala->getAddAtendDiaSemana(dia);
					// get atendimento turno
					AtendimentoTurno* const atendTurno = atendDiaSem->getAddAtendTurno(turnoId);
					atendTurno->turno = turno;
					atendTurno->setTurnoId(turno->getId());

					// HORARIOS
					for(auto itHor = aula->horarios.begin(); itHor != aula->horarios.end(); ++itHor)
					{
						HorarioAula* const horario = *itHor;
						AtendimentoHorarioAula* const atendHor = atendTurno->getAddAtendHorarioAula(horario->getId());
						// set informação
						atendHor->horario_aula = horario;
						atendHor->setProfessorId(professor->getId());
						atendHor->professor = professor->getProfessor();
						atendHor->setProfVirtual(professor->ehVirtual());
						atendHor->setCreditoTeorico(turma->ehTeoricaTag());

						// ITERADOR OFERTAS/ALUNOS
						for(auto itDem = alunosDemanda.cbegin(); itDem != alunosDemanda.cend(); ++itDem)
						{
							Demanda* const demanda = itDem->first;
							Oferta* const oferta = demanda->oferta;
							AtendimentoOferta* const atendOferta = atendHor->getAddAtendOferta(oferta->getId());
							// set informação
							stringstream ss;
							ss << oferta->getId();
							atendOferta->setOfertaCursoCampiId(ss.str());
							// set disciplina da alocação
							atendOferta->disciplina = disciplina;
							// set disciplina original
							atendOferta->setDisciplinaId(demanda->getDisciplinaId());
							// get disciplina substituta (caso equivalencia)
							if(demanda->getDisciplinaId() != disciplina->getId())
								atendOferta->setDisciplinaSubstitutaId(disciplina->getId());
							// set nr alunos
							atendOferta->addQuantidade(itDem->second.size());
							// set turma
							atendOferta->setTurma(turma->id);

							// ITERADOR ALUNOS
							for(auto itAlunosDem = itDem->second.begin(); itAlunosDem != itDem->second.end(); ++itAlunosDem)
							{
								atendOferta->alunosDemandasAtendidas.add((*itAlunosDem)->getId());
							}
						}
					}
				}
			}
		}
	}

	HeuristicaNuno::logMsg("Output atendimento criado!", 1);
}



// cria o output dos atendimentos
void SolucaoHeur::criarOutputNovo_(ProblemSolution* const solution) const
{
	HeuristicaNuno::logMsg("Criar output atendimento da solução!", 1);
	GGroup<AtendimentoCampus *>* const outputAtendimento = solution->atendimento_campus;
	
	// Cenário id
	solution->setCenarioId( HeuristicaNuno::probData->getCenarioId() );
	
	unordered_set<TurmaHeur*> turmas;

	// CAMPUS
	for(auto itCampus = ofertasDisciplina_.begin(); itCampus != ofertasDisciplina_.end(); ++itCampus)
	{
		Campus* const campus = itCampus->first;
		AtendimentoCampus* const atendCampus = new AtendimentoCampus(campus->getId());
		atendCampus->campus = campus;
		atendCampus->setCampusId(campus->getCodigo());
		outputAtendimento->add(atendCampus);

		// DISCIPLINAS / OFERTAS
		for(auto itDisc = itCampus->second.begin(); itDisc != itCampus->second.end(); ++itDisc)
		{
			Disciplina* const disciplina = itDisc->first;
			OfertaDisciplina* const ofertaDisc = itDisc->second;

			// TURMAS
			turmas.clear();
			ofertaDisc->getTurmas(turmas);
			for(auto itTurmas = turmas.begin(); itTurmas != turmas.end(); ++itTurmas)
			{
				TurmaHeur* const turma = *itTurmas;

				criarTurmaOutput_(*atendCampus, turma);
			}
		}
	}

	HeuristicaNuno::logMsg("Output atendimento criado!", 1);
}

void SolucaoHeur::criarTurmaOutput_(AtendimentoCampus &atendCampus, TurmaHeur* const turma) const
{
	// Problem Data
	ProblemData* const probData = HeuristicaNuno::probData;
	
	unordered_map<int, AulaHeur*> aulas;
	unordered_map<Demanda*, set<AlunoDemanda*>> alunosDemanda;

	ProfessorHeur* const professor = turma->getProfessor();
	if(professor == nullptr)
		HeuristicaNuno::excepcao("SolucaoHeur::criarTurmaOutput_", "Professor e nulo !");
	
	// get atendimento unidade
	int unidadeId = turma->unidadeId();
	AtendimentoUnidade* const atendUnid = atendCampus.getAddAtendUnidade(unidadeId);
	auto itUnid = probData->refUnidade.find(unidadeId);
	if(itUnid != probData->refUnidade.end())
	{
		atendUnid->unidade = itUnid->second;
		atendUnid->setCodigoUnidade(itUnid->second->getCodigo());
	}
	else
		HeuristicaNuno::warning("SolucaoHeur::criarTurmaOutput_", "Unidade nao encontrada");

	// get atendimento sala
	AtendimentoSala* const atendSala = atendUnid->getAddAtendSala(turma->getSala()->getId());
	atendSala->sala = turma->getSala()->getSala();
	atendSala->setSalaId(turma->getSala()->getSala()->getCodigo());

	// get alunos demanda satisfeitas por oferta
	alunosDemanda.clear();
	turma->getDemandasAlunos(alunosDemanda);

	// AULAS
	aulas.clear();
	turma->getAulas(aulas);
	for(auto itAulas = aulas.cbegin(); itAulas != aulas.cend(); ++itAulas)
	{
		int dia = itAulas->first;
		AulaHeur* const aula = itAulas->second;

		criarAulaOutput_(*atendSala, turma, professor, aula, dia, alunosDemanda);
	}
}

void SolucaoHeur::criarAulaOutput_(AtendimentoSala &atendSala, TurmaHeur* const turma, ProfessorHeur* const professor, 
			AulaHeur* const aula, int dia, unordered_map<Demanda*, set<AlunoDemanda*>> const &alunosDemanda) const
{
	Disciplina* const disciplina = turma->ofertaDisc->getDisciplina();

	// get atendimento diaSemana
	AtendimentoDiaSemana* const atendDiaSem = atendSala.getAddAtendDiaSemana(dia);

	// get id turno. NOTA: geração não tem mt em conta os turnos na discretização do output
	DateTime primHorario;
	aula->getPrimeiroHor(primHorario);
	
	// ITERADOR OFERTAS/ALUNOS
	for(auto itDem = alunosDemanda.cbegin(); itDem != alunosDemanda.cend(); ++itDem)
	{
		Demanda* const demanda = itDem->first;		
		Oferta* const oferta = demanda->oferta;
		TurnoIES* const turno = oferta->turno;
		Calendario* const calendario = demanda->getCalendario();

		// get atendimento turno
		AtendimentoTurno* const atendTurno = atendDiaSem->getAddAtendTurno(turno->getId());
		atendTurno->turno = turno;
		atendTurno->setTurnoId(turno->getId());

		// HORARIOS
		for(auto itHor = aula->horarios.begin(); itHor != aula->horarios.end(); ++itHor)
		{			
			HorarioAula* const h = *itHor;		
			HorarioAula* const horario = turno->getHorarioDiaOuCorrespondente(calendario, h, dia);
			if(!horario)
			{
				stringstream msg;
				msg << "Horario " << h->getInicio() << " nao encontrado no dia " << dia 
					<< " em turno " << turno->getId() << " e calendario " << calendario->getId()
					<< " para demanda " << demanda->getId();
				HeuristicaNuno::excepcao("SolucaoHeur::criarAulaOutput_",msg.str());
			}

			AtendimentoHorarioAula* const atendHor = atendTurno->getAddAtendHorarioAula(horario->getId());
			// set informação
			atendHor->horario_aula = horario;
			atendHor->setProfessorId(professor->getId());
			atendHor->professor = professor->getProfessor();
			atendHor->setProfVirtual(professor->ehVirtual());
			atendHor->setCreditoTeorico(turma->ehTeoricaTag());

			AtendimentoOferta* const atendOferta = atendHor->getAddAtendOferta(oferta->getId());
			// set informação
			stringstream ss;
			ss << oferta->getId();
			atendOferta->setOfertaCursoCampiId(ss.str());
			// set disciplina da alocação
			atendOferta->disciplina = disciplina;
			// set disciplina original
			atendOferta->setDisciplinaId(demanda->getDisciplinaId());
			// get disciplina substituta (caso equivalencia)
			if(demanda->getDisciplinaId() != disciplina->getId())
				atendOferta->setDisciplinaSubstitutaId(disciplina->getId());
			// set nr alunos
			atendOferta->addQuantidade(itDem->second.size());
			// set turma
			atendOferta->setTurma(turma->id);

			// ITERADOR ALUNOS
			for(auto itAlunosDem = itDem->second.begin(); itAlunosDem != itDem->second.end(); ++itAlunosDem)
			{
				atendOferta->alunosDemandasAtendidas.add((*itAlunosDem)->getId());
			}
		}
	}
}

// cria o output de professores virtuais
void SolucaoHeur::criarOutProfsVirtuais_(ProblemSolution* const solution) const
{
	//HeuristicaNuno::logMsg("Criar output prof virtual!", 1);
	GGroup<ProfessorVirtualOutput*>* profsVirtuaisOutput = solution->professores_virtuais;

	// iterar profs virtuais
	for(auto it = profsVirtuais.cbegin(); it != profsVirtuais.cend(); ++it)
	{
		// criar output prof virtual único (se ele tiver turmas)
		int virtualId = it->first;
		ProfessorHeur* const profVirtual = it->second;
		if(!profVirtual->temTurmas())
			continue;

		ProfessorVirtualOutput* const profVirtualOut = new ProfessorVirtualOutput(virtualId);
		if(profVirtual->tipoTitulacao() != nullptr)
			profVirtualOut->setTitulacaoId(profVirtual->tipoTitulacao()->getId());
		if(profVirtual->tipoContrato() != nullptr)
			profVirtualOut->setContratoId(profVirtual->tipoContrato()->getContrato());
		profsVirtuaisOutput->add(profVirtualOut);

		unordered_set<TurmaHeur*> turmas;
		profVirtual->getTurmas(turmas);
		for(auto itTurmas = turmas.begin(); itTurmas != turmas.end(); ++itTurmas)
		{
			TurmaHeur* const turma = *itTurmas;
			int disciplina = turma->ofertaDisc->getDisciplina()->getId();
			int turmaNr = turma->id;
			int campus = turma->ofertaDisc->getCampus()->getId();
			bool ehPrat = !turma->ehTeoricaTag();
		
			AlocacaoProfVirtual* alocacao = new AlocacaoProfVirtual(disciplina, turmaNr, campus, ehPrat);
			profVirtualOut->alocacoes.add(alocacao);
		}
	}

	HeuristicaNuno::logMsg("criado output prof virtual!", 1);
	return;
}

// ---------------------------------------------------------------------------------------------------------


// ------------------------------------------ LOAD SOLUÇÃO ------------------------------------------

// atendimento campus
void SolucaoHeur::loadAtendimentoCampus(AtendimentoCampus* const atendCampus, 
										unordered_map<TurmaHeur*, unordered_set<AlunoHeur*>>* const &turmasAlunos,
										unordered_map<TurmaHeur*, unordered_map<int, set<HorarioAula*>>>* const &turmasHorarios)
{
	if(atendCampus->campus == nullptr || atendCampus->campus == NULL)
		HeuristicaNuno::excepcao("SolucaoHeur::checkAtendimentoCampus", "Campus nulo!");

	// inserir o campus no mapa de ofertas disciplina
	auto itCamp = ofertasDisciplina_.find(atendCampus->campus);
	if(itCamp == ofertasDisciplina_.end())
	{
		unordered_map<Disciplina*, OfertaDisciplina*> emptyMap;
		auto parIns = ofertasDisciplina_.insert(make_pair(atendCampus->campus, emptyMap));
		if(!parIns.second)
			HeuristicaNuno::excepcao("SolucaoHeur::checkAtendimentoCampus", "Campus nao inserido em OfertasDisciplina");

		itCamp = parIns.first;
	}

	// mergulhar para atendimento sala (unidade nao interessa)
	for(auto itUnid = atendCampus->atendimentos_unidades->begin(); itUnid != atendCampus->atendimentos_unidades->end(); ++itUnid)
	{
		for(auto itSala = (*itUnid)->atendimentos_salas->begin(); itSala != (*itUnid)->atendimentos_salas->end(); ++itSala)
		{
			loadAtendimentoSala(*itSala, itCamp, turmasAlunos, turmasHorarios);
		}
	}
}
// atendimento sala
void SolucaoHeur::loadAtendimentoSala(AtendimentoSala* const atendSala, itCampODs const &itCampus,
										unordered_map<TurmaHeur*, unordered_set<AlunoHeur*>>* const &turmasAlunos,
										unordered_map<TurmaHeur*, unordered_map<int, set<HorarioAula*>>>* const &turmasHorarios)
{
	if(atendSala->sala == nullptr || atendSala->sala == NULL)
		HeuristicaNuno::excepcao("SolucaoHeur::checkAtendimentoSala", "Sala nula!");

	// procurar a sala
	auto itSala = salasHeur.find(atendSala->sala->getId());
	if(itSala == salasHeur.end())
		HeuristicaNuno::excepcao("SolucaoHeur::checkAtendimentoSala", "SalaHeur nao encontrada!");

	SalaHeur* const sala = itSala->second;

	// analisar atendimento horario aula
	for(auto itDia = atendSala->atendimentos_dias_semana->begin(); itDia != atendSala->atendimentos_dias_semana->end(); ++itDia)
	{
		const int dia = (*itDia)->getDiaSemana();
		// Tag atendimentosTurnos
		for(auto itTurno = (*itDia)->atendimentos_turno->begin(); itTurno != (*itDia)->atendimentos_turno->end(); ++itTurno)
		{
			for(auto itHor = (*itTurno)->atendimentos_horarios_aula->begin(); itHor != (*itTurno)->atendimentos_horarios_aula->end(); ++itHor)
			{
				loadAtendimentoHorarioAula(*itHor, itCampus, sala, dia, turmasAlunos, turmasHorarios);
			}
		}
		// Tag atendimentosTatico
		for(auto itTat = (*itDia)->atendimentos_tatico->begin(); itTat != (*itDia)->atendimentos_tatico->end(); ++itTat)
		{
			loadAtendimentoTatico(*itTat, itCampus, sala, dia, turmasAlunos, turmasHorarios);
		}
	}
}
// atendimento horario aula
void SolucaoHeur::loadAtendimentoHorarioAula(AtendimentoHorarioAula* const atendHoraAula, itCampODs const &itCampus, 
											SalaHeur* const &sala, int const &dia, 
											unordered_map<TurmaHeur*, unordered_set<AlunoHeur*>>* const &turmasAlunos,
											unordered_map<TurmaHeur*, unordered_map<int, set<HorarioAula*>>>* const &turmasHorarios)
{
	if(atendHoraAula->horario_aula == nullptr || atendHoraAula->horario_aula == NULL)
		HeuristicaNuno::excepcao("SolucaoHeur::checkAtendimentoHorarioAula", "Horario nulo!");

	// obter horario aula
	HorarioAula* const horario = atendHoraAula->horario_aula;

	// No carregamento da solução já é alocado o professor virtual genérico por isso devia ser encontrado
	auto itProf = professoresHeur.find(atendHoraAula->professor->getId());
	if(itProf == professoresHeur.end())
		HeuristicaNuno::excepcao("SolucaoHeur::checkAtendimentoHorarioAula", "Professor nao encontrado!");

	ProfessorHeur* const professor = itProf->second;
	bool ehTeorico = atendHoraAula->getCreditoTeorico();

	// load atendimentos oferta
	for(auto it = atendHoraAula->atendimentos_ofertas->begin(); it != atendHoraAula->atendimentos_ofertas->end(); ++it)
	{
		loadAtendimentoOferta(*it, itCampus, sala, dia, horario, professor, ehTeorico, turmasAlunos, turmasHorarios);
	}
}
// atendimento oferta
void SolucaoHeur::loadAtendimentoOferta(AtendimentoOferta* const atendOferta, itCampODs const &itCampus, SalaHeur* const &sala, 
								int const &dia, HorarioAula* const &horario, ProfessorHeur* const &professor, bool const &teorico,
								unordered_map<TurmaHeur*, unordered_set<AlunoHeur*>>* const &turmasAlunos,
								unordered_map<TurmaHeur*, unordered_map<int, set<HorarioAula*>>>* const &turmasHorarios)
{
	if(atendOferta->disciplina == nullptr || atendOferta->disciplina == NULL)
		HeuristicaNuno::excepcao("SolucaoHeur::loadAtendimentoOferta", "Disciplina nula");

	// Disciplina
	Disciplina* const disciplina = atendOferta->disciplina;

	// Turmas por disciplina
	auto itDisc = turmasPorDisc_.find(disciplina);
	if(itDisc == turmasPorDisc_.end())
	{
		unordered_set<TurmaHeur*> emptySet;
		itDisc = turmasPorDisc_.insert(make_pair(disciplina, emptySet)).first;
	}

	// obter os alunos atendidos
	unordered_set<AlunoHeur*> alunos;
	getAlunosAtendidos(atendOferta, alunos);
	if(alunos.size() == 0)
	{
		HeuristicaNuno::warning("SolucaoHeur::loadAtendimentoOferta", "Atendimento oferta com 0 alunos atendidos.");
		return;
	}

	// obter a oferta disciplina associada (adicionar se ainda não tiver sido criada)
	OfertaDisciplina* const ofertaDisc = getAddOfertaDisciplina(disciplina, itCampus);

	// get/add turmaHeur
	
	// TODO: diferenciar o set da fixação para somente solução carregada!
//	AtendFixacao fixacoes( true, true, (true && !professor->ehVirtual()), true, true, true );
	AtendFixacao fixacoes( true, true, false, true, true, true );
	const int turmaId = atendOferta->getTurma();
	TurmaHeur* const turma = getAddTurma(ofertaDisc, turmaId, ofertaDisc->tipoTurmaFromTag(teorico), sala, professor, turmasAlunos, turmasHorarios, fixacoes);

	// registar turma por disciplina
	if(itDisc->second.find(turma) == itDisc->second.end())
		itDisc->second.insert(turma);

	// check professor
	if(turma->getProfessor()->getId() != professor->getId())
		HeuristicaNuno::excepcao("SolucaoHeur::loadAtendimentoOferta", "Turma já estava associada a um professor diferente!");
	// check sala
	if(turma->getSala()->getId() != sala->getId())
		HeuristicaNuno::excepcao("SolucaoHeur::loadAtendimentoOferta", "Turma já estava associada a uma sala diferente!");

	// associar os alunos à turma inserindo-os em turmasAlunos
	auto itAlunosT = turmasAlunos->find(turma);
	if(itAlunosT == turmasAlunos->end())
		HeuristicaNuno::excepcao("SolucaoHeur::loadAtendimentoOferta", "Turma nao encontrada em turmasAlunos");
	for(auto itA = alunos.begin(); itA != alunos.end(); ++itA)
		itAlunosT->second.insert(*itA);

	// adicionar dia/horário a turmasHorarios
	auto itHorsT = turmasHorarios->find(turma);
	if(itHorsT == turmasHorarios->end())
		HeuristicaNuno::excepcao("SolucaoHeur::loadAtendimentoOferta", "Turma nao encontrada em turmasHorarios");
	// dia (pode ja ter sido adicionado)
	auto itDia = itHorsT->second.find(dia);
	if(itDia == itHorsT->second.end())
	{
		set<HorarioAula*> emptySet;
		auto par = itHorsT->second.insert(make_pair(dia, emptySet));
		if(!par.second)
			HeuristicaNuno::excepcao("SolucaoHeur::loadAtendimentoOferta", "Dia nao adicionado a estrutura turmasHorarios");
		itDia = par.first;
	}
	// horário (pode ja ter sido adicionado)
	itDia->second.insert(horario);
}

// atendimento tatico
void SolucaoHeur::loadAtendimentoTatico(AtendimentoTatico* const atendTatico, itCampODs const &itCampus, 
											SalaHeur* const &sala, int const &dia, 
											unordered_map<TurmaHeur*, unordered_set<AlunoHeur*>>* const &turmasAlunos,
											unordered_map<TurmaHeur*, unordered_map<int, set<HorarioAula*>>>* const &turmasHorarios)
{
	if(atendTatico->atendimento_oferta == nullptr)
		HeuristicaNuno::excepcao("SolucaoHeur::loadAtendimentoTatico", "atendimento_oferta null!");
	
	int credsTeor = atendTatico->getQtdCreditosTeoricos();
	int credsPrat = atendTatico->getQtdCreditosPraticos();

	// obter horario aula
	unordered_set<HorarioAula*> horarios;
	GGroup<int> horsId = atendTatico->getHorariosAula();
	for(auto itHor = horsId.begin(); itHor != horsId.end(); itHor++)
	{
		HorarioAula* h = CentroDados::getHorarioAula(*itHor);
		horarios.insert(h);
	}

	if(credsTeor+credsPrat!=horarios.size())
		HeuristicaNuno::excepcao("SolucaoHeur::loadAtendimentoTatico", "nro de horarios difere do nro de creditos!");

	// AtendimentoTatico não informa professor, então é alocado inicialmente prof virtual.
	auto itProf = professoresHeur.find(ParametrosHeuristica::professorVirtual->getId());
	if(itProf == professoresHeur.end())
		HeuristicaNuno::excepcao("SolucaoHeur::checkAtendimentoHorarioAula", "Professor virtual nao encontrado!");
	ProfessorHeur* const professor = itProf->second;

	bool ehTeorico = atendTatico->getQtdCreditosTeoricos();

	// load atendimentos oferta
	for (auto itHor=horarios.cbegin(); itHor!=horarios.cend(); itHor++ )
	{
		loadAtendimentoOferta(atendTatico->atendimento_oferta, itCampus, sala, dia, 
			*itHor, professor, ehTeorico, turmasAlunos, turmasHorarios);
	}
}


// get alunos atendidos no atendimento oferta
void SolucaoHeur::getAlunosAtendidos(AtendimentoOferta* const atendOferta, unordered_set<AlunoHeur*> &alunos)
{
	for(auto it = atendOferta->alunosDemandasAtendidas.begin(); it != atendOferta->alunosDemandasAtendidas.end(); ++it)
	{
		// obter aluno associado à demanda
		Aluno* aluno = nullptr;
		aluno = CentroDados::getAlunoIdFromAlunoDemanda(*it);
		if(aluno == nullptr)
		{
			HeuristicaNuno::logMsgInt("aluno demanda id: ", *it, 1);
			HeuristicaNuno::warning("SolucaoHeur::loadAtendimentoOferta", "Demanda de aluno nao encontrada!");
			continue;
		}

		// procurar estrutura alunoHeur
		int alunoId = aluno->getAlunoId();
		auto itFind = alunosHeur.find(alunoId);
		if(itFind == alunosHeur.end())
		{
			HeuristicaNuno::logMsgInt("aluno id: ", alunoId, 1);
			HeuristicaNuno::excepcao("SolucaoHeur::loadAtendimentoOferta", "AlunoHeur nao encontrado");
		}

		alunos.insert(itFind->second);
	}
}
// get oferta disciplina se já foi criada. se não criar e retornar
OfertaDisciplina* SolucaoHeur::getAddOfertaDisciplina(Disciplina* const &disciplina, itCampODs const &itCampus)
{
	auto itFind = itCampus->second.find(disciplina);
	// já foi criada
	if(itFind != itCampus->second.end())
		return itFind->second;

	// criar oferta disciplina
	Disciplina* discTeorica = nullptr;
	Disciplina* discPratica = nullptr;
	associarDisciplinas_(disciplina, discTeorica, discPratica);

	OfertaDisciplina* ofertaDisc = new OfertaDisciplina (this, discTeorica, discPratica, itCampus->first);

	// gerar/reprocessar divisões.
	ofertaDisc->gerarDivs();
	// stats
	if(ofertaDisc->nrTiposAula() == 2)
		stats_->nrDiscDuasComp_++;

	// inserir em ofertasDisciplina
	itCampus->second[disciplina] = ofertaDisc;
	// inserir em allOfertasDisc_
	allOfertasDisc_.push_back(ofertaDisc);

	return ofertaDisc;
}
// get oferta disciplina se já foi criada. se não retorna nullptr
OfertaDisciplina* SolucaoHeur::getOfertaDisciplina(Disciplina* const &disciplina, Campus* const campus)
{
	OfertaDisciplina* ofertaDisc=nullptr;
	
	// se já foi criada
	auto itFindCp = ofertasDisciplina_.find(campus);
	if(itFindCp != ofertasDisciplina_.end())
	{
		auto itFindDisc = itFindCp->second.find(disciplina);
		if(itFindDisc != itFindCp->second.end())
			ofertaDisc = itFindDisc->second;
	}

	return ofertaDisc;
}
// get turma se já foi criada, se não, criar e retornar
TurmaHeur* SolucaoHeur::getAddTurma(OfertaDisciplina* const &ofertaDisc, int const &turmaId, bool const &teorico,
									SalaHeur* const &sala, ProfessorHeur* const &professor,
									unordered_map<TurmaHeur*, unordered_set<AlunoHeur*>>* const &turmasAlunos,
									unordered_map<TurmaHeur*, unordered_map<int, set<HorarioAula*>>>* const &turmasHorarios,
									const AtendFixacao &fixacoes)
{
	// ver se já tem turma com esse id
	TurmaHeur* turma = ofertaDisc->getTurma(teorico, turmaId);
	if(turma != nullptr)
		return turma;

	// criar uma nova turma
	turma = ofertaDisc->abrirTurma(teorico, turmaId, sala, professor, fixacoes);
	if(turma == nullptr)
		HeuristicaNuno::excepcao("SolucaoHeur::getAddTurma", "Turma nula");

	// adicionar à estrutura turmasAlunos
	unordered_set<AlunoHeur*> emptySet;
	auto par = turmasAlunos->insert(make_pair(turma, emptySet));
	if(!par.second)
		HeuristicaNuno::excepcao("SolucaoHeur::getAddTurma", "[Load] Turma nao adicionada a estrutura turmasAluno");

	// adicionar à estrutura turmasHorarios
	unordered_map<int, set<HorarioAula*>> emptyMap;
	auto parHor = turmasHorarios->insert(make_pair(turma, emptyMap));
	if(!parHor.second)
		HeuristicaNuno::excepcao("SolucaoHeur::getAddTurma", "[Load] Turma nao adicionada a estrutura turmasHorarios");

	return turma;
}

// carregar as aulas para as turmas, define o calendário e associa a turma a professor e sala
void SolucaoHeur::loadAulasTurmas(unordered_map<TurmaHeur*, unordered_map<int, set<HorarioAula*>>>* const &turmasHorarios)
{
	// load aulas para turmas
	for(auto itTurma = turmasHorarios->begin(); itTurma != turmasHorarios->end(); ++itTurma)
	{
		// set calendario da turma
		findSetCalendarioTurma(itTurma->first, itTurma->second);
		// load aulas
		TurmaHeur* const turma = itTurma->first;
		int campusId = turma->getSala()->campusId();
		int unidId = turma->getSala()->unidadeId();
		for(auto itDia = itTurma->second.begin(); itDia != itTurma->second.end(); ++itDia)
		{
			AulaHeur aula (itDia->first, campusId, unidId, itDia->second);
			AulaHeur* const ptrAula = UtilHeur::saveAula(aula);
			turma->addAula(itDia->first, ptrAula);
		}
		// associa a professor e sala
		turma->addTurmaProfSala();
		// só depois pois antes não tinha aulas
		stats_->nrCreditosProfessores_ += turma->getNrCreditos();
	}
}
// find calendario turma com base nos horarios
void SolucaoHeur::findSetCalendarioTurma(TurmaHeur* const turma, unordered_map<int, set<HorarioAula*>> const &aulas)
{
	Disciplina* const disciplina = turma->ofertaDisc->getDisciplina();

	// escolher o primeiro calendario dominante que contenha todos os horários
	if(disciplina->calendariosReduzidos.size() == 0)
		HeuristicaNuno::excepcao("SolucaoHeur::findSetCalendarioTurma", "Calendarios dominantes ainda nao foram determinados!");

	Calendario* calendario = nullptr;
	for(auto itCal = disciplina->calendariosReduzidos.begin(); itCal != disciplina->calendariosReduzidos.end(); ++itCal)
	{
		bool contemAll = true;
		for(auto itDia = aulas.begin(); itDia != aulas.end(); ++itDia)
		{
			if(!UtilHeur::calendarioAbrange(*itCal, itDia->first, itDia->second))
			{
				contemAll = false;
				break;
			}
		}
		if(contemAll)
		{
			calendario = (*itCal);
			break;
		}
	}

	if(calendario == nullptr)
		HeuristicaNuno::excepcao("SolucaoHeur::findSetCalendarioTurma", "Calendario da turma nao encontrado!");

	turma->setCalendario(calendario);
}

// carregar alunos para as turmas (colocando a flag de alunos fixados!)
void SolucaoHeur::loadAlunosFixadosTurmas(unordered_map<TurmaHeur*, unordered_set<AlunoHeur*>>* const &turmasAlunos)
{
	for(auto itT = turmasAlunos->cbegin(); itT != turmasAlunos->cend(); ++itT)
	{
		OfertaDisciplina* const oferta = itT->first->ofertaDisc;
		TurmaHeur* const turma = itT->first;
		for(auto itAluno = itT->second.cbegin(); itAluno != itT->second.cend(); ++itAluno)
			oferta->addAlunoTurma(*itAluno, turma, "SolucaoHeur::loadAlunosFixadosTurmas", true);
	}
}

// ------------------------------------------------------------------------------------------------------------



// ------------------------------------ UTIL ------------------------------------------------------

// retorna as demandas de um certo tuplo (campus, disciplina). retorna false se não encontrar
bool SolucaoHeur::getDemandasNaoAtendidas(Campus* const campus, Disciplina* const disciplina, unordered_map<int, unordered_set<AlunoDemanda*>> &demandas,
											bool filtrar, int priorAluno)
{
	// encontrar campus
	auto itCampus = demandasNaoAtendidas_.find(campus);
	if(itCampus == demandasNaoAtendidas_.end())
		return false;

	// encontrar disciplina
	auto itDisc = itCampus->second.find(disciplina);
	if(itDisc == itCampus->second.end())
		return false;

	int nrCredsDisc = disciplina->getTotalCreditos();
	for(auto it = itDisc->second.cbegin(); it != itDisc->second.cend(); ++it)
	{
		// filtrar alunos que excedam o máximo de créditos
		unordered_set<AlunoDemanda*> demsPrior = it->second;
		if(filtrar)
			filtrarDemsNaoAtendidas(nrCredsDisc, priorAluno, demsPrior);

		demandas[it->first] = demsPrior;
	}
	return true;
}
// retorna as demandas de um certo tuplo (campus, disciplina, prioridade). retorna false se não encontrar
bool SolucaoHeur::getDemandasNaoAtendidas(Campus* const campus, Disciplina* const disciplina, int prioridade, unordered_set<AlunoDemanda *> &demandas,
											bool filtrar, int priorAluno)
{
	// encontrar campus
	auto itCampus = demandasNaoAtendidas_.find(campus);
	if(itCampus == demandasNaoAtendidas_.end())
		return false;

	// encontrar disciplina
	auto itDisc = itCampus->second.find(disciplina);
	if(itDisc == itCampus->second.end())
		return false;

	// encontrar prioridade
	auto itPrior = itDisc->second.find(prioridade);
	if(itPrior == itDisc->second.end())
		return false;

	demandas.insert(itPrior->second.begin(), itPrior->second.end());
	// tirar alunos que já excedam o numero de créditos com esta disciplina!
	if(filtrar)
		filtrarDemsNaoAtendidas(disciplina->getTotalCreditos(), priorAluno, demandas);

	return true;
}

// POR CURSO
bool SolucaoHeur::getDemandasNaoAtendidas(OfertaDisciplina* const oferta, int prioridade, unordered_set<AlunoDemanda *> &demandas, 
										bool filtrar, int priorAluno)
{
	// encontrar campus
	auto itCampus = demsNaoAtendPorCurso_.find(oferta->getCampus());
	if(itCampus == demsNaoAtendPorCurso_.end())
		return false;

	// encontrar disciplina
	auto itDisc = itCampus->second.find(oferta->getDisciplina());
	if(itDisc == itCampus->second.end())
		return false;

	// por curso
	for(auto it = oferta->cursos.cbegin(); it != oferta->cursos.cend(); ++it)
	{
		// curso
		auto itCurso = itDisc->second.find(*it);
		if(itCurso == itDisc->second.end())
			continue;

		// prioridade
		auto itPrior = itCurso->second.find(prioridade);
		if(itPrior == itCurso->second.end())
			continue;

		demandas.insert(itPrior->second.begin(), itPrior->second.end());
	}

	// tirar alunos que já excedam o numero de créditos com esta disciplina!
	if(filtrar)
		filtrarDemsNaoAtendidas(oferta->getDisciplina()->getTotalCreditos(), priorAluno, demandas);
}

// filtrar das demandas não atendidas alunos que já não têm margem de créditos para cursar essa disciplina
void SolucaoHeur::filtrarDemsNaoAtendidas(int nrCredsDisc, int priorAluno, unordered_set<AlunoDemanda*> &demandas)
{
	for(auto it = demandas.begin(); it != demandas.end();)
	{
		auto aluno = alunosHeur.find((*it)->getAlunoId());
		if(aluno == alunosHeur.end())
			HeuristicaNuno::excepcao("SolucaoHeur::filtrarDemsNaoAtendidas", "Aluno heur nao encontrado");

		// verificar se excede creditos ou ele é calouro/inadimplente e não é suposto considerar
		// obs:: prior aluno <= 0  => nao excluir por prioridade de aluno
		if(aluno->second->excedeMaxCreds(nrCredsDisc) || ((priorAluno > 0) && (aluno->second->getPriorAluno() > priorAluno)))
		{
			demandas.erase(it++);
		}
		else
			++it;
	}
}

// limpar turmas assoc ofertas disciplina
void SolucaoHeur::limparTurmasAssocAllOfertasPreMIP(void)
{
	for(auto itOft = allOfertasDisc_.cbegin(); itOft != allOfertasDisc_.cend(); ++itOft)
	{
		(*itOft)->limparTurmasAssocPreMIP();
	}
}


// retorna o número de demandas não atendidas (original)
int SolucaoHeur::nrDemandasNaoAtendidas(int prior) const
{
	int nrDem = DadosHeuristica::nrDemandas(1);

	nrDem -= stats_->nrDemsAtend(1);

	return nrDem;
}
// retorna o número de demandas não atendidas de um campus (só de prioridade 1 ou todas)
int SolucaoHeur::nrDemandasNaoAtendidas(Campus* const campus, bool soPrioridadeUm) const
{
	int nrDem = 0;

	auto itCampus = demandasNaoAtendidas_.find(campus);
	if(itCampus == demandasNaoAtendidas_.end())
		HeuristicaNuno::excepcao("SolucaoHeur::nrDemandasNaoAtendidas", "Campus não encontrado nas demandas não atendidas.");

	for(auto itDisc = itCampus->second.cbegin(); itDisc != itCampus->second.cend() ; itDisc++)
	{
		for(auto itPrior = itDisc->second.cbegin(); itPrior != itDisc->second.cend(); itPrior++)
		{
			if(itPrior->first != 1 && soPrioridadeUm)
				continue;

			nrDem += (int)itPrior->second.size();
		}
	}

	return nrDem;
}
// retorna o número de demandas atendidas para um determinado campus, disciplina e prioridade
int SolucaoHeur::nrDemandasNaoAtendidas(Campus* const campus, Disciplina* const disciplina, int prioridade) const
{
	return demandasNaoAtendidas_.at(campus).at(disciplina).at(prioridade).size();
}

// get mapa de demandas não atendidas
void SolucaoHeur::getMapDemandasNaoAtend(Campus* const campus, Disciplina* const disciplina, set<Curso*> const &cursos, 
											unordered_map<int, int> &demsNaoAtend) const
{
	// campus
	auto itCampus = demsNaoAtendPorCurso_.find(campus);
	if(itCampus == demsNaoAtendPorCurso_.end())
		return;

	// disciplina
	auto itDisc = itCampus->second.find(disciplina);
	if(itDisc == itCampus->second.end())
		return;

	// cada curso
	for(auto it = cursos.cbegin(); it != cursos.cend(); ++it)
	{
		auto itCurso = itDisc->second.find(*it);
		if(itCurso == itDisc->second.end())
			continue;

		for(auto itPrior = itCurso->second.begin(); itPrior != itCurso->second.end(); ++itPrior)
		{
			int prior = itPrior->first;
			auto itDemsPrior = demsNaoAtend.find(prior);
			if(itDemsPrior == demsNaoAtend.end())
				itDemsPrior = demsNaoAtend.insert(make_pair(prior, 0)).first;

			int val = itDemsPrior->second;
			itDemsPrior->second = (val + itPrior->second.size());
		}
	}
}

// retorna um conjunto de alunosheur associado a um set de alunodemanda
void SolucaoHeur::getSetAlunosFromSetAlunoDemanda(unordered_set<AlunoDemanda *> const &alunosDemanda, unordered_set<AlunoHeur*> &alunos)
{
	HeuristicaNuno::logMsg("get alunos from set alunodemanda", 3);
	auto itDem = alunosDemanda.begin();
	for( ; itDem != alunosDemanda.end(); ++itDem)
	{
		int alunoId = (*itDem)->getAlunoId();
		auto itAluno = alunosHeur.find(alunoId);
		if(itAluno == alunosHeur.end())
		{
			HeuristicaNuno::excepcao("SolucaoHeur::getSetAlunosFromSetAlunoDemanda", "Aluno não encontrado.");
		}
		alunos.insert(itAluno->second);
	}
}
// retorna um conjunto de alunosheur associado a um set de alunodemanda (nao deixar adicionar aluno na blacklist)
void SolucaoHeur::getSetAlunosFromSetAlunoDemanda(unordered_set<AlunoDemanda *> const &alunosDemanda, unordered_set<int> const &blacklist,
									unordered_set<AlunoHeur*> &alunos)
{
	auto itDem = alunosDemanda.begin();
	for( ; itDem != alunosDemanda.end(); ++itDem)
	{
		int alunoId = (*itDem)->getAlunoId();
		if(blacklist.find(alunoId) != blacklist.end())
			continue;

		auto itAluno = alunosHeur.find(alunoId);
		if(itAluno == alunosHeur.end())
		{
			HeuristicaNuno::excepcao("SolucaoHeur::getSetAlunosFromSetAlunoDemanda", "Aluno não encontrado.");
		}
		alunos.insert(itAluno->second);
	}
}

// retorna um conjunto de alunosheur associado a um set de alunodemanda
void SolucaoHeur::getSetAlunosFromSetAlunoDemanda(unordered_set<AlunoDemanda *> const &alunosDemanda, set<AlunoHeur*> &alunos)
{
	HeuristicaNuno::logMsg("get alunos from set alunodemanda", 3);
	auto itDem = alunosDemanda.begin();
	for( ; itDem != alunosDemanda.end(); ++itDem)
	{
		int alunoId = (*itDem)->getAlunoId();
		auto itAluno = alunosHeur.find(alunoId);
		if(itAluno == alunosHeur.end())
		{
			HeuristicaNuno::excepcao("SolucaoHeur::getSetAlunosFromSetAlunoDemanda", "Aluno não encontrado.");
		}
		alunos.insert(itAluno->second);
	}
}


// get turmas de uma disciplina
void SolucaoHeur::getTurmasDisciplina(Disciplina* const disciplina, unordered_set<TurmaHeur*> &turmas) const
{
	// contar turmas. procurar em cada campus todas as disciplinas do curso
	unordered_set<TurmaHeur*> turmasOft;
	for(auto itCamp = ofertasDisciplina_.cbegin(); itCamp != ofertasDisciplina_.cend(); ++itCamp)
	{
		auto itOft = itCamp->second.find(disciplina);
		if(itOft == itCamp->second.cend())
			continue;

		turmasOft.clear();
		itOft->second->getTurmas(turmasOft);
		for(auto it = turmasOft.begin(); it != turmasOft.end();)
		{
			turmas.insert(*it);
			turmasOft.erase(it++);
		}
	}
}

// --------------------------------------------------------------------------------------------


// ------------------------------------- MEMORY -------------------------------------

void SolucaoHeur::limparOfertasDisciplina_(void)
{
	for(auto itCampus = ofertasDisciplina_.begin(); itCampus != ofertasDisciplina_.end(); ++itCampus)
	{
		for(auto itDisc = itCampus->second.begin(); itDisc != itCampus->second.end();)
		{
			OfertaDisciplina* oft = itDisc->second;
			itDisc = itCampus->second.erase(itDisc);
			delete oft;
		}

	}
}

// --------------------------------------------------------------------------

// ------------------------------------- LOG -------------------------------------

// print demandas atendidas
void SolucaoHeur::printDemandasAtendidas(char* append) const
{
	std::stringstream ss;
	ss << "demandas_atendidas_" << append << ".log";
	std::ofstream logDem (ss.str());

	for(auto itCamp = ofertasDisciplina_.cbegin(); itCamp != ofertasDisciplina_.cend(); ++itCamp)
	{
		for(auto itDisc = itCamp->second.cbegin(); itDisc != itCamp->second.cend(); ++itDisc)
		{
			OfertaDisciplina* const oferta = itDisc->second;
			unordered_set<AlunoHeur*> alunos;
			oferta->getAlunos(alunos);

			for(auto it = alunos.cbegin(); it != alunos.cend(); ++it)
			{
				int discOrig = (*it)->getDemandaOriginal(oferta->getCampus(), oferta->getDisciplina())->demanda->getDisciplinaId();
				logDem << "Aluno: " << (*it)->getId() << " -> Disc. Original: " << discOrig << endl; 
				logDem.flush();
			}
		}
	}
	logDem.close();
}

// imprime resumo de todas as oferta-disciplinas na ordem corrente
template<typename T>
void SolucaoHeur::imprimeOfertasDisc(set<OfertaDisciplina*, T> const & setOrd, int prior)
{
	std::stringstream ssTime;
	ssTime << std::endl << "-----------------------------------------------------"
	<< "-------------------------------------------------------------------------";
	ssTime << std::endl << "[" << UtilHeur::currentDateTime() << "] - Prior " << prior << std::endl
		<< "Status atual da solucao:" << std::endl
		<< "\tTotal de atendimentos: " << nrDemandasAtendidas(0) << std::endl
		<< "\tTotal de turmas: " << nrTurmas() << std::endl
		<< "\tTotal de creds com profs virtuais: " << nrCreditosProfVirtual() << std::endl;

	HeuristicaNuno::logOrdemOfertas( ssTime.str() );

	for( auto itOft = setOrd.cbegin();
		 itOft != setOrd.cend(); itOft++)
	{
		std::ostringstream out;
		out << (**itOft);
		
		std::stringstream ssMsg;
		ssMsg << out.str() << endl;
		
		HeuristicaNuno::logOrdemOfertas( ssMsg.str() );
	}
}

// imprime solucao atual
void SolucaoHeur::printSolucaoLog_() const
{
	if (!CentroDados::getPrintLogs())
		return;

	static int counter=0;
	counter++;

	std::stringstream ss;
	ss << "solucaoAtual_" << counter << ".log";
	std::ofstream logSol (ss.str());
	
	HeuristicaNuno::logMsg("Imprime solução atual", 1);
	
	// Problem Data
	ProblemData* const probData = HeuristicaNuno::probData;
		
	unordered_set<TurmaHeur*> turmas;
	unordered_map<int, AulaHeur*> aulas;
	unordered_map<Demanda*, set<AlunoDemanda*>> alunosDemanda;

	// CAMPUS
	for(auto itOft = allOfertasDisc_.begin(); itOft != allOfertasDisc_.end(); ++itOft)
	{
			OfertaDisciplina* const ofertaDisc = *itOft;

			Campus* const campus = ofertaDisc->getCampus();	
			Disciplina* const disciplina = ofertaDisc->getDisciplina();
			
			// TURMAS
			turmas.clear();
			ofertaDisc->getTurmas(turmas);
			for(auto itTurmas = turmas.begin(); itTurmas != turmas.end(); ++itTurmas)
			{
				TurmaHeur* const turma = *itTurmas;
				ProfessorHeur* const professor = turma->getProfessor();
				int unidadeId = turma->unidadeId();
				
				// get alunos demanda satisfeitas por oferta
				alunosDemanda.clear();
				turma->getDemandasAlunos(alunosDemanda);

				logSol << "\n\n--------------------------------------------------";
				logSol << "\n(Cp" << campus->getId() << ",Disc"	<< ofertaDisc->getDisciplina()->getId() 
					<< ",Id" << turma->id << "):  "
					<< "sala" << turma->getSala()->getId();
				if (professor) logSol << ", prof" << professor->getId();

				if(turma->ehCompSec())
				{
					unordered_set<TurmaHeur*> turmasTeor;
					ofertaDisc->getTurmasAssoc(turma,turmasTeor);
					for(auto it=turmasTeor.cbegin(); it!=turmasTeor.cend(); it++)
						logSol << ", turma teorica associada: " << (*it)->id;
				}

				// AULAS
				aulas.clear();
				turma->getAulas(aulas);
				logSol << "\n------------";
				for(auto itAulas = aulas.cbegin(); itAulas != aulas.cend(); ++itAulas)
				{
					int dia = itAulas->first;
					AulaHeur* const aula = itAulas->second;

					DateTime primHorario;
					aula->getPrimeiroHor(primHorario);
					
					DateTime endHorario;
					aula->getLastHor(endHorario);

					logSol << "\n\tDia" << dia << ", " << primHorario.hourMinToStr() << " - "
						<< endHorario.hourMinToStr();										
				}
				// ALUNOS
				int nrAlunos=0;
				logSol << "\n------------\n\t";
				for(auto itDem = alunosDemanda.cbegin(); itDem != alunosDemanda.cend(); ++itDem)
				{
					for(auto itAlunosDem = itDem->second.begin(); itAlunosDem != itDem->second.end(); ++itAlunosDem)
					{
						logSol << " AlDem" << (*itAlunosDem)->getId();
						nrAlunos++;
					}
				}
				logSol << "\t(" << nrAlunos << " alunos)";
			}
		
	}

	HeuristicaNuno::logMsg("Solucao atual impressa", 1);
}


// --------------------------------------------------------------------------
