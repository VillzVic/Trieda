#include "AbridorTurmas.h"
#include "SalaHeur.h"
#include "AulaHeur.h"
#include "AlunoHeur.h"
#include "RecursivePreProc.h"
#include "ProfessorHeur.h"
#include "OfertaDisciplina.h"
#include "TurmaHeur.h"
#include "SolucaoHeur.h"
#include "ParametrosHeuristica.h"
#include "HeuristicaNuno.h"
#include "DadosHeuristica.h"
#include "../ProblemData.h"
#include "../ParametrosPlanejamento.h"
#include "../AlunoDemanda.h"
#include "../Disciplina.h"
#include "../ConjUnidades.h"
#include "GeradorCombsDiv.h"
#include "UtilHeur.h"
#include "ImproveMethods.h"
#include <math.h>

#ifdef DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

long AbridorTurmas::nrCombAnalise = 0;
int AbridorTurmas::nrGeraUnids_ = 0;
OfertaDisciplina* AbridorTurmas::currOferta = nullptr;

AbridorTurmas::AbridorTurmas(SolucaoHeur* const solucao, bool equiv, bool usarVirtual, bool realoc, bool anySize, int priorAluno,
							double relaxMin)
	: solucao_(solucao), equiv_(equiv), abrirVirtual(usarVirtual), realocar(realoc), anySize_(anySize), priorAluno_(priorAluno),
	relaxMin_(relaxMin)
{
	if(relaxMin <= 0)
		relaxMin_ = 1.0;
}

AbridorTurmas::~AbridorTurmas(void)
{
}

// definição da funcao
template<typename T>
void AbridorTurmas::abrirTurmas(set<OfertaDisciplina*, T> &setOrd)
{
	HeuristicaNuno::logMsg("abrir turmas !", 1);

	if(setOrd.size() == 0)
	{
		HeuristicaNuno::warning("AbridorTurmas::abrirTurmas", "set ord está vazio!");
		return;
	}

	// enquanto ainda houver ofertas disciplina para abrir turmas
	while(setOrd.size() > 0)
	{
		OfertaDisciplina* const oferta = *(setOrd.begin());
		setOrd.erase(setOrd.begin());					// remover da lista
		
		// tentar abrir turmas
		bool abriu = preAbrirPosTurmasOfertaDisc_(oferta);

		// se abriu, voltar a considerar
		if(!abriu || ((ParametrosHeuristica::relacPraticas == ParametrosHeuristica::abrePraticas::N_N) && (oferta->nrTiposAula() == 2)))
			continue;

		if(!equiv_)
		{
			// readicionar oferta
			unordered_map<int, unordered_set<AlunoDemanda *>> demandasNaoAtend;
			solucao_->getDemandasNaoAtendidas(oferta->getCampus(), oferta->getDisciplina(), demandasNaoAtend, true, priorAluno_);
			oferta->setDemandasNaoAtend(demandasNaoAtend, equiv_);
			setOrd.insert(oferta);
		}
		else
		{
			setOrd.insert(oferta);
			solucao_->reordenarOfertasDisciplina_<T>(setOrd, priorAluno_, equiv_);
			//
		}
	}
	currOferta = nullptr;
}

template void AbridorTurmas::abrirTurmas<compOftDisc>(set<OfertaDisciplina*, compOftDisc> &setOrd);

// abrir turmas [ordem randomica]
void AbridorTurmas::abrirTurmasRandom(vector<OfertaDisciplina*> &setOfts)
{
	HeuristicaNuno::logMsg("abrir turmas (random)!", 1);

	if(setOfts.size() == 0)
	{
		HeuristicaNuno::warning("AbridorTurmas::abrirTurmasRandom", "set ofts está vazio!");
		return;
	}

	// enquanto ainda houver ofertas disciplina para abrir turmas
	while(setOfts.size() > 0)
	{
		OfertaDisciplina* const oferta = *(setOfts.begin());
		setOfts.erase(setOfts.begin());					// remover da lista

		// teste
		currOferta = oferta;

		// tentar abrir turmas
		bool abriu = preAbrirPosTurmasOfertaDisc_(oferta);

		// se abriu, voltar a considerar
		if(abriu)
			UtilHeur::insertRandomIndex<OfertaDisciplina*>(setOfts, oferta);
	}
	currOferta = nullptr;
}

// pré-pos abrir turmas de uma oferta disciplina
bool AbridorTurmas::preAbrirPosTurmasOfertaDisc_(OfertaDisciplina* const oferta)
{
	if(oferta == nullptr)
	{
		HeuristicaNuno::warning("AbridorTurmas::preAbrirPosTurmasOfertaDiscV2_", "oferta e nula");
		return false;
	}

	// print info!
	//oferta->printInfo();

	// setar comparador alunos
	bool duasComp = oferta->duasComp();
	if(duasComp)
		AlunoHeur::flagCompAlunos = AlunoHeur::flagComp::MENOS_CRED;	
	else
		AlunoHeur::flagCompAlunos = AlunoHeur::flagComp::MAIS_CRED;

	// tentar abrir turmas
	int nrCompBef = oferta->nrAlunosCompleto();
	bool abriu = abrirTurmasOfertaDisc_(oferta);
	int nrCompAft = oferta->nrAlunosCompleto();

	if(abriu)
	{
		// remover os incompletos que ficaram e acerta oferta
		if(duasComp)
		{
			unordered_set<AlunoHeur*> incompletos;
			oferta->removerAlunosIncompleto(incompletos, false);
			// acertar oferta
			oferta->acertarOferta(relaxMin_);
			ImproveMethods::tryReduzirSalas(oferta);

			// se for igual
			nrCompAft = oferta->nrAlunosCompleto();
			if(nrCompAft <= nrCompBef)
			{
				if(incompletos.size() > 0)
					addToBlackList(oferta, incompletos);
				else
					abriu = false;
			}
		}

		// tentar alocar nao atendidos
		ImproveMethods::tryAlocNaoAtendidos(solucao_, oferta, true, equiv_, realocar, priorAluno_, relaxMin_);
	}
	else
	{
		// acertar oferta
		oferta->acertarOferta(relaxMin_);
		nrCompAft = oferta->nrAlunosCompleto();
		ImproveMethods::tryReduzirSalas(oferta);
	}

	if(!duasComp && (nrCompAft <= nrCompBef))
		abriu = false;

	// print stats rodada
	int credsAluno = solucao_->nrCreditosAluno();
	int credsProf = solucao_->nrCreditosProf();
	HeuristicaNuno::logMsgInt("# creditos aluno: ", credsAluno, 1);
	HeuristicaNuno::logMsgInt("# creditos prof: ", credsProf, 1);
	double racCred = credsProf == 0 ? 0 : double(credsAluno)/credsProf;
	HeuristicaNuno::logMsgDouble("ratio cred: ", racCred, 1);
	HeuristicaNuno::logMsgInt("# demandas atend p1: ", solucao_->nrDemandasAtendidas(1), 1);
	//HeuristicaNuno::logMsgInt("# turmas abertas: ", solucao_->nrTurmas(), 1);
	HeuristicaNuno::logMsg("", 1);

	return abriu;
}
// abrir turmas de uma oferta disciplina
bool AbridorTurmas::abrirTurmasOfertaDisc_(OfertaDisciplina* const ofertaDisc)
{
	// reset global id count turma potencial
	TurmaPotencial::resetGlobalIdCount();

	HeuristicaNuno::logMsg("abrir turmas oferta disc", 3);
	if(ofertaDisc->nrTiposAula() == 2)
		HeuristicaNuno::logMsg("oferta disc 2 componentes", 3);

	if(ofertaDisc == nullptr)
		HeuristicaNuno::excepcao("AbridorTurmas::abrirTurmasOfertaDisc_", "nullptr");

	std::stringstream sstm;
	sstm << "Campus: " << ofertaDisc->getCampus()->getId() << ", Disciplina: " << ofertaDisc->getDisciplina()->getNome() << "\n";
	HeuristicaNuno::logMsg(sstm.str(), 2);

	// Tem demanda suficiente ?
	if((relaxMin_ < 0) && !tentarAbrirTurmas_(ofertaDisc))
		return false;

	bool teorico = ofertaDisc->temCompTeorica();
	bool duasComp = ofertaDisc->duasComp();
	bool abriu = false;								// abriu turma principal?

	// abrir turmas teóricas (se precisar)
	if(teorico)
	{
		abriu = abrirTurmasOfertaDiscComp_(ofertaDisc, true);
		int nrTurmas = ofertaDisc->nrTurmas(true);
		if(duasComp)
			HeuristicaNuno::logMsgInt("turmas teoricas abertas: ", nrTurmas, 1);

		if(!duasComp)
		{
			HeuristicaNuno::logMsg("out abrir turmas oferta disc", 3);
			return abriu;
		}
		else if(abriu && ofertaDisc->temAlunosIncompleto())
		{
			abrirTurmasOfertaDiscComp_(ofertaDisc, false);
			int nrTurmas = ofertaDisc->nrTurmas(false);
			HeuristicaNuno::logMsgInt("turmas praticas abertas: ", nrTurmas, 1);
		}
	}
	// só prático
	else
	{
		abriu = abrirTurmasOfertaDiscComp_(ofertaDisc, false);
	}

	return abriu;
}
// abrir turmas de uma componente de uma oferta disciplina
bool AbridorTurmas::abrirTurmasOfertaDiscComp_(OfertaDisciplina* const ofertaDisc, bool teorico)
{
	int nrAbertas = 0;
	HeuristicaNuno::logMsg("abrir turmas oferta disc comp", 2);

	Disciplina* const disciplina = ofertaDisc->getDisciplina(teorico);
	if(disciplina->getTotalCreditos() == 0)
	{
		HeuristicaNuno::warning("AbridorTurmas::abrirTurmasOfertaDiscComp_", "Componente tem zero créditos!");
		return false;
	}

	// verificar se é componente secundaria
	const bool compSec = ((ofertaDisc->nrTiposAula() == 2) && !teorico);
	if(compSec && !ofertaDisc->temAlunosIncompleto())
	{
		HeuristicaNuno::warning("AbridorTurmas::abrirTurmasOfertaDiscComp_", "Comp sec sem alunos incompletos!");
		return (nrAbertas > 0);
	}
	
	// abrir turmas
	do
	{
		// gera turmas potenciais
		HeuristicaNuno::logMsg("gerar turmas potenciais", 2);
		turmasPotOrd turmasPotenciais;

		//AbridorTurmas::nrGeraUnids_ = 0;
		geraTurmasPotenciais_(ofertaDisc, teorico, turmasPotenciais, compSec);
		//HeuristicaNuno::logMsgInt("nr gera unids: ", AbridorTurmas::nrGeraUnids_, 1);

		// escolhe uma turma para abrir
		HeuristicaNuno::logMsg("escolher turma", 2);
		const TurmaPotencial* turmaEscolhida;
		bool stop = false;
		if(escolherTurmaPotencial_(turmasPotenciais, turmaEscolhida) && turmaEscolhida->ok)
		{
			if(compSec && turmaEscolhida->tipoAula)
				HeuristicaNuno::excepcao("AbridorTurmas::abrirTurmasOfertaDiscComp_", "Turma secundária não é prática!");

			abrirTurmaPotencial_(ofertaDisc, turmaEscolhida, compSec);
			nrAbertas++;
		}
		else
			stop = true;
		
		// limpar memória turmas potenciais
		limparTurmasPotenciais_(turmasPotenciais);
		
		// realocar
		int nrTurmasTipo = ofertaDisc->nrTurmas(teorico);
		int nrAlunosInc = ofertaDisc->nrAlunosIncompleto();
		if(nrTurmasTipo > 1 && realocar)
		{
			if((teorico && ParametrosHeuristica::relacPraticas == ParametrosHeuristica::abrePraticas::N_N)
				|| ofertaDisc->nrTiposAula() == 1)
			{
				ImproveMethods::tryRealocAlunosPrinc(solucao_, ofertaDisc, true, equiv_, priorAluno_, relaxMin_);
			}
			if(compSec && (ParametrosHeuristica::relacPraticas != ParametrosHeuristica::abrePraticas::UM_UM)
				&& nrAlunosInc > 0)
			{
				ImproveMethods::tryRealocAlunosCompSec(ofertaDisc, true, priorAluno_, relaxMin_);
			}
		}

		// só uma componente, abrir uma turma e re-inserir na ordem
		if(ofertaDisc->nrTiposAula() == 1)
			break;

		// se for 1x1 ou teorica e 1xN só abrir uma!
		if(ParametrosHeuristica::relacPraticas == ParametrosHeuristica::abrePraticas::UM_UM ||
			(ParametrosHeuristica::relacPraticas == ParametrosHeuristica::abrePraticas::UM_N && !compSec))
		{
			break;
		}

		if(stop)
			break;

		// se for componente secundaria parar se nao houver mais alunos incompleto
		if(compSec && ofertaDisc->nrAlunosIncompleto() == 0)
			break;

	} while(true);

	HeuristicaNuno::logMsg("out turmas oferta disc comp", 2);

	return nrAbertas > 0;
}
// abrir uma turma
void AbridorTurmas::abrirTurmaPotencial_(OfertaDisciplina* const ofertaDisc, const TurmaPotencial* &turmaPot, const bool compSec)
{
	//HeuristicaNuno::logMsg("abrir turma potencial", 2);
	TurmaHeur* turma = nullptr;
	turma = ofertaDisc->abrirTurma(turmaPot);
	if(turma == nullptr)
		return;

	int nr = turmaPot->nrAlunos();
	if(nr < ParametrosHeuristica::getMinAlunos(turma->discEhLab()))
		HeuristicaNuno::logMsgInt("low nr alunos: ", nr, 1);
	

	//HeuristicaNuno::logMsg("out. abrir turma potencial", 2);
}

// verifica se vale a pena tentar abrir turmas para esta oferta disciplina, i.e. se tem demanda suficiente
bool AbridorTurmas::tentarAbrirTurmas_(OfertaDisciplina* const ofertaDisc)
{
	if(ofertaDisc == nullptr)
		HeuristicaNuno::excepcao("AbridorTurmas::tentarAbrirTurmas_", "nullptr");

	// verificar se tem salas associadas para todas as componentes
	//if(!ofertaDisc->podeAbrir()) 
	//	return false;

	// get demandas nao atendidas
	unordered_set<AlunoDemanda*> demandas;
	solucao_->getDemandasNaoAtendidas(ofertaDisc->getCampus(), ofertaDisc->getDisciplina(), 1, demandas, true, priorAluno_);
	if(equiv_)
		solucao_->getDemandasNaoAtendidas(ofertaDisc->getCampus(), ofertaDisc->getDisciplina(), -1, demandas, true, priorAluno_);

	bool temFormando = false;
	bool temCoReq = false;
	UtilHeur::temFormandoCoReq(demandas, temFormando, temCoReq);

	return podeAbrirTurma_((int)demandas.size(), temFormando, 0, ofertaDisc->getDisciplina()->eLab(), temCoReq);
}



// [CRIAÇÃO DE TURMAS POTENCIAIS]

// gerar turmas potenciais para uma oferta disciplina. se for a componente secundária da disciplina, i.e. parte prática de uma disciplina
	// prática + teórica, só poderá usar os alunos já alocados!
void AbridorTurmas::geraTurmasPotenciais_(OfertaDisciplina* const ofertaDisc, bool teorico, turmasPotOrd &turmasPotenciais, 
											const bool compSec)
{
	// 0 -> turmas OK , 1 -> turmas com prof virtual, 2-> turmas com prof virtual e sem prof disponivel naquele horario
	HeuristicaNuno::logMsg("gera turmas potenciais...", 2);

	// reset nr combinacoes
	AbridorTurmas::nrCombAnalise = 0;
	nrTurmas_ = 0;

	// salas associadas
	unordered_set<SalaHeur*> salasAssoc;
	ofertaDisc->getSalasAssociadas(salasAssoc, teorico);
	HeuristicaNuno::logMsgInt("# salas assoc: ", (int)salasAssoc.size(), 2);
	if(salasAssoc.size() == 0)
	{
		HeuristicaNuno::warning("AbridorTurmas::geraTurmasPotenciais_", "Componente da oferta sem salas associadas. Retornar.");
		return;
	}

	// profs associados
	unordered_set<ProfessorHeur*> profsAssoc;
	ofertaDisc->getProfessoresAssociados(profsAssoc);
	HeuristicaNuno::logMsgInt("# profs assoc: ", (int)profsAssoc.size(), 2);

	Disciplina* const disciplina = ofertaDisc->getDisciplina(teorico);

	// alunos
	unordered_set<AlunoHeur*> alunosDem;
	getAlunosDemandaNaoAtend_(ofertaDisc, alunosDem, compSec);
	bool temFormando = false;
	bool temCoReq = false;
	UtilHeur::temFormandoCoReq(ofertaDisc, alunosDem, temFormando, temCoReq);
	if(!podeAbrirTurma_((int)alunosDem.size(), ofertaDisc->getNrCreds(teorico), temFormando, disciplina->eLab(), temCoReq))
		return;
	
	// get divisões de créditos
	vector<vector<pair<int,int>>> const &divisoesCreditos = disciplina->combinacao_divisao_creditos;

	if(divisoesCreditos.size() == 0)
	{
		HeuristicaNuno::logDiscSemDivisao(disciplina->getId(), teorico, compSec);
		if(teorico)
			HeuristicaNuno::warning("AbridorTurmas::geraTurmasPotenciais_", "[T] Disciplina sem divisoes de creditos!");
		else if(!compSec)
			HeuristicaNuno::warning("AbridorTurmas::geraTurmasPotenciais_", "[P-princ] Disciplina sem divisoes de creditos!");
		else
			HeuristicaNuno::warning("AbridorTurmas::geraTurmasPotenciais_", "[P-compSec] Disciplina sem divisoes de creditos!");

		return;
	}

	// declare outside;
	unordered_set<AlunoHeur*> alunosDispCal;
	int nrCal = 0;
	for(auto itCal = disciplina->calendariosReduzidos.begin(); itCal != disciplina->calendariosReduzidos.end(); ++itCal)
	{
		Calendario* const cal = *itCal;
		if(cal == nullptr)
		{
			HeuristicaNuno::warning("AbridorTurmas::geraTurmasPotenciais_", "Calendario nullptr!");
			continue;
		}
		
		// obter alunos disponiveis para esta disciplina neste calendario
		alunosDispCal.clear();
		getAlunosDispCalendario_(ofertaDisc, cal, alunosDem, alunosDispCal, temFormando, temCoReq);
		if(alunosDispCal.size() == 0)
			continue;

		// verificar se pode abrir
		if(!podeAbrirTurma_((int)alunosDispCal.size(), ofertaDisc->getNrCreds(teorico), temFormando, disciplina->eLab(), temCoReq))
			continue;

		HeuristicaNuno::logMsgInt("calendario #", ++nrCal, 2);

		// gera turmas com pré-processamento
		geraTurmasComPreProc_(ofertaDisc, cal, teorico, divisoesCreditos, salasAssoc, profsAssoc, alunosDispCal, turmasPotenciais,
									compSec);
	}
	HeuristicaNuno::logMsg("out. gerar turmas potenciais", 3);

}
// gera turmas potenciais com preprocessamento
void AbridorTurmas::geraTurmasComPreProc_(OfertaDisciplina* const ofertaDisc, Calendario * const calendario, 
							bool teorico, vector<vector<pair<int,int>>> const &divisoesCreditos, 
							unordered_set<SalaHeur*> const &salasAssoc, unordered_set<ProfessorHeur*> const &profsAssoc,
							unordered_set<AlunoHeur*> const &alunosDem,
							turmasPotOrd &turmasPotenciais, const bool componenteSec)
{
	// 1. obter creditos possiveis no dia com base nas divisoes.
	HeuristicaNuno::logMsg("[preproc] minMaxCredsDia", 2);
	unordered_map<int, unordered_set<int>> credsPossDia;
	UtilHeur::credsPossiveisDias(divisoesCreditos, credsPossDia);

	// 2. obter para cada tuplo (dia, nr creds, aula) um par (salas, alunos) disponiveis. Verificar também a partir de que nr.
	// de créditos deixa de ser possivel abrir turma
	HeuristicaNuno::logMsg("[preproc] fillMapDisponibilidade_", 2);
	mapDispDiaCredAula disponibilidades;
	fillMapDisponibilidade_(ofertaDisc, teorico, calendario, salasAssoc, alunosDem, credsPossDia, disponibilidades);

	// 3. Algoritmo recursivo que gera combinações de aulas consoante as combinações, intersecta alunos e salas disponiveis e 
	// chama geraTurmasUnidade_ caso valha a pena.
	HeuristicaNuno::logMsg("[preproc] gera turmas", 2);
	//const bool compSec = (ofertaDisc->temCompTeorica() && !teorico);
	//const bool primeira = !ofertaDisc->temTurmaTipo(teorico);
	Disciplina* const disciplina = ofertaDisc->getDisciplina(teorico);
	if(disciplina == nullptr)
		HeuristicaNuno::excepcao("AbridorTurmas::geraTurmasComPreProc_", "Disciplina nao encontrada");
	const bool usaLab = disciplina->eLab();

	unordered_set<const TurmaPotencial*> turmasDivisao;
	for(auto itDiv = divisoesCreditos.begin(); itDiv != divisoesCreditos.end(); ++itDiv)
	{
		if(!UtilHeur::checkDivisaoDisc(*itDiv, ofertaDisc->getDisciplina(teorico)))
			continue;

		turmasDivisao.clear();
		geraTurmasDivisaoPosPreProc_(ofertaDisc, calendario, *itDiv, disponibilidades, profsAssoc, teorico, usaLab, turmasPotenciais,
									turmasDivisao);
	}
}

// preenche para cada tuplo (dia, creds, aula) as salas e alunos disponiveis, se houverem)
void AbridorTurmas::fillMapDisponibilidade_(OfertaDisciplina* const ofertaDisc, bool teorico, Calendario * const calendario, 
								unordered_set<SalaHeur*> const &salasAssoc, unordered_set<AlunoHeur*> const &alunosDem,
								unordered_map<int, unordered_set<int>> const &credsPossDia,
								mapDispDiaCredAula &disponibilidades)
{
	auto itCal = UtilHeur::aulasCalendario.find(calendario);
	if(itCal == UtilHeur::aulasCalendario.end())
		HeuristicaNuno::excepcao("AbridorTurmas::fillMapDisponibilidade_", "aulasCalendario do calendario nao encontradas!");

	unordered_set<SalaHeur*> salasDisp;
	unordered_set<AlunoHeur*> alunosDisp;
	vector<AulaHeur*> aulasDiaCreds;
	unordered_map<int, AulaHeur*> aulasMap;
	bool temFormando = false;
	bool temCoReq = false;
	const bool compSec = (ofertaDisc->temCompTeorica() && !teorico);
	const bool primeira = !ofertaDisc->temTurmaTipo(teorico);
	Disciplina* const disciplina = ofertaDisc->getDisciplina(teorico);
	const bool haProfHab = ofertaDisc->haProfessorAssociado();

	const bool aulasContinuas = ofertaDisc->getDisciplina()->aulasContinuas();
	unordered_map<AlunoHeur*, TurmaHeur*> mapAlunoTurmaTeor;
	if (aulasContinuas && compSec)
	{
		getMapTurmaTeoricaAlunos_(ofertaDisc, alunosDem, mapAlunoTurmaTeor);
	}

	//int nrProfsLivresEst = nrProfsLivresAssocEstimado(ofertaDisc);
	//HeuristicaNuno::logMsgInt("nrProfsLivresEst: ", nrProfsLivresEst, 2);
	//HeuristicaNuno::logMsgInt("haProfHab: ", haProfHab, 2);

	for(auto itDia = credsPossDia.cbegin(); itDia != credsPossDia.cend(); ++itDia)
	{
		int dia = itDia->first;
		if(disciplina->diasLetivos.find(dia) == disciplina->diasLetivos.end())
			continue;

		unordered_map<AlunoHeur*, TurmaHeur*> mapAlunoTurmaTeorNoDia;
		if (aulasContinuas && compSec)
		{
			// filtra alunos com aulas da disciplina teorica no dia
			getMapTurmaTeoricaAlunosNoDia_(dia,mapAlunoTurmaTeorNoDia,mapAlunoTurmaTeor);						
			if(mapAlunoTurmaTeorNoDia.size() == 0)
				continue;
		}

		for(auto itCreds = itDia->second.cbegin(); itCreds != itDia->second.cend(); ++itCreds)
		{
			int nrCreds = (*itCreds);
			aulasDiaCreds.clear();
			UtilHeur::getAulasCalDiaCreds(calendario, dia, nrCreds, aulasDiaCreds);
			// posso parar, porque nao vai ser possivel aulas maiores
			if(aulasDiaCreds.size() == 0)
				continue;

			for(auto itAula = aulasDiaCreds.cbegin(); itAula != aulasDiaCreds.cend(); ++itAula)
			{
				AulaHeur* const aula = *itAula;

				// turma nao disponivel nesse conjunto de horarios
				if(!checkAulaDisciplina(disciplina, dia, aula))
					continue;

				// limpar estruturas
				alunosDisp.clear();
				salasDisp.clear();
				aulasMap.clear();
				temFormando = false;

				// verificar se ultrapassa o máximo de aulas simultaneas da disciplina
				if(ParametrosHeuristica::limTurmasSimultaneas)
				{					
					int nrSimult = nrTurmasSimultaneasDisc_(disciplina, dia, aula) + 1;
					int nrProfsAssoc = nrProfsAssociadosHorario(ofertaDisc, dia, aula);
					if(nrSimult - nrProfsAssoc > ParametrosHeuristica::slackTurmasSimult)
						continue;
					//if(nrProfsLivresEst>0 && nrSimult - nrProfsAssoc > ParametrosHeuristica::slackTurmasSimult)
					//	continue;
					//else if (nrProfsLivresEst<=0 && nrSimult - nrProfsAssoc > ParametrosHeuristica::slackTurmasSimultSemProf)
					//	continue;
				}

				// set aula
				aulasMap[dia] = aula;
				
				unordered_set<AlunoHeur*> const *alunosDemConsiderados=nullptr;

				// disciplina de aulas teorico-pratico continuas
				unordered_set<AlunoHeur*> alunosDemPTCont;
				if (aulasContinuas && compSec)
				{
					// filtra alunos com aulas teoricas no horario anterior					
					getAlunosAulaCont_(ofertaDisc, dia, aula, mapAlunoTurmaTeorNoDia, alunosDemPTCont);
					if(alunosDemPTCont.size() == 0)
						continue;
					alunosDemConsiderados = &alunosDemPTCont;
				}
				else
				{	
					alunosDemConsiderados = &alunosDem;
				}

				// obter as salas disponiveis
				getSalasDisponiveis_(salasAssoc, aulasMap, teorico, salasDisp);
				if(salasDisp.size() == 0)
					continue;

				// obter os alunos disponiveis
				getAlunosDisponiveis_(ofertaDisc, *alunosDemConsiderados, aulasMap, temFormando, temCoReq, teorico, alunosDisp);
				if(!podeAbrirTurma_((int)alunosDisp.size(), ofertaDisc->getNrCreds(teorico), temFormando, temCoReq, disciplina->eLab()))
					continue;

				parAlunosSalaDisp parDisp (alunosDisp, salasDisp);
				addParAlunosSalaDisp_(dia, nrCreds, *itAula, parDisp, disponibilidades);
			}
		}
	}
}

// preenche um map com a turma teorica (se houver) de cada aluno
void AbridorTurmas::getMapTurmaTeoricaAlunos_(OfertaDisciplina* const ofertaDisc,
								unordered_set<AlunoHeur*> const &alunosDem,
								unordered_map<AlunoHeur*, TurmaHeur*> &mapAlunoTurmaTeor)
{
	auto itAluno = alunosDem.cbegin();
	for (; itAluno != alunosDem.cend(); itAluno++)
	{
		AlunoHeur* aluno = *itAluno;
		TurmaHeur* turma = nullptr;
		ofertaDisc->getTurmaAlunoTipo(aluno, true, turma);
		mapAlunoTurmaTeor[aluno] = turma;		
	}
}

// preenche um map com a turma teorica (se houver) de cada aluno
void AbridorTurmas::getMapTurmaTeoricaAlunosNoDia_(int dia,
						unordered_map<AlunoHeur*, TurmaHeur*> &mapAlunoTurmaTeorNoDia,
						unordered_map<AlunoHeur*, TurmaHeur*> const &mapAlunoTurmaTeor)
{
	// filtra alunos com aulas teoricas no dia
	for (auto itAluno = mapAlunoTurmaTeor.cbegin();
		itAluno != mapAlunoTurmaTeor.cend(); itAluno++)
	{
		AulaHeur* aula;
		if (itAluno->second->getAulaDia(dia, aula))
			mapAlunoTurmaTeorNoDia[itAluno->first] = itAluno->second;
	}
}

// verifica se a disciplina pode ter uma aula nesses dias.
bool AbridorTurmas::checkAulaDisciplina(Disciplina* const disciplina, int dia, AulaHeur* const aula)
{
	for(auto it = aula->horarios.begin(); it != aula->horarios.end(); ++it)
	{
		if(!disciplina->possuiHorarioDiaOuAnalogo(dia, *it))
			return false;
	}

	return true;
}

// adiciona parAlunosSalaDisp ao mapa de disponibilidade
void AbridorTurmas::addParAlunosSalaDisp_(int dia, int nrCreds, AulaHeur* const aula, parAlunosSalaDisp const &parDisp,
											mapDispDiaCredAula &disponibilidades)
{
	// dia
	auto itDia = disponibilidades.find(dia);
	if(itDia == disponibilidades.end())
	{
		unordered_map<int, unordered_map<AulaHeur*, parAlunosSalaDisp>> emptyCreds;
		itDia = disponibilidades.insert(make_pair(dia, emptyCreds)).first;
	}

	// nr creds
	auto itCreds = itDia->second.find(nrCreds);
	if(itCreds == itDia->second.end())
	{
		unordered_map<AulaHeur*, parAlunosSalaDisp> emptyAula;
		itCreds = itDia->second.insert(make_pair(nrCreds, emptyAula)).first;
	}

	// aula id
	auto itAula = itCreds->second.find(aula);
	if(itAula != itCreds->second.end())
		HeuristicaNuno::excepcao("AbridorTurmas::addParAlunosSalaDisp_", "Tuplo (dia, creds, aula) ja registado!");

	if(!itCreds->second.insert(make_pair(aula, parDisp)).second)
		HeuristicaNuno::excepcao("AbridorTurmas::addParAlunosSalaDisp_", "par disponibilidade nao inserido");
}

// gera turmas para uma divisao depois do pre-processamento
void AbridorTurmas::geraTurmasDivisaoPosPreProc_(OfertaDisciplina* const &ofertaDisc, Calendario* const &calendario, 
										vector<pair<int,int>> const &divisao, mapDispDiaCredAula const &disponibilidades, 
										unordered_set<ProfessorHeur*> const &profsAssoc, bool const &teorico, bool const &usaLab,
										turmasPotOrd &turmasPotenciais, unordered_set<const TurmaPotencial*> &turmasDivisao)
{
	if(divisao.size() == 0)
	{
		HeuristicaNuno::warning("AbridorTurmas::geraTurmasDivisaoPosPreProc_", "divisao vazia");
		return;
	}

	// stage inicial
	unordered_set<AlunoHeur*> alunosDisp;
	unordered_set<SalaHeur*> salasDisp;
	unordered_map<int, AulaHeur*> emptyAulas;
	vector<pair<int,int>>::const_iterator itStart = divisao.cbegin();

	vector<RecursivePreProc*> queueSteps;
	RecursivePreProc* firstStep = new RecursivePreProc(itStart, alunosDisp, salasDisp, emptyAulas);
	queueSteps.push_back(firstStep);


	// HEAP
	while(queueSteps.size() > 0)
	{
		RecursivePreProc* const step = queueSteps.back();
		queueSteps.pop_back();

		// gera proximos steps apartir deste
		geraTurmasDivisaoRecHeap_(ofertaDisc, calendario, divisao, disponibilidades, profsAssoc, teorico, usaLab, turmasPotenciais,
									turmasDivisao, step, queueSteps);

		// liberta a memória
		delete step;
		
		//UtilHeur::printMemoryUsed();

		//HeuristicaNuno::logMsgInt("steps queue size: ", queueSteps.size(), 1);
	}
}

void AbridorTurmas::geraTurmasDivisaoRecHeap_(OfertaDisciplina* const &ofertaDisc, Calendario* const &calendario, 
							vector<pair<int,int>> const &divisao, mapDispDiaCredAula const &disponibilidades, 
							unordered_set<ProfessorHeur*> const &profsAssoc, bool const &teorico, bool const &usaLab, 
							turmasPotOrd &turmasPotenciais, unordered_set<const TurmaPotencial*> &turmasDivisao,
							RecursivePreProc* const step, vector<RecursivePreProc*> &queueSteps)
{
	// verificar se ja acabou a divisao. se tiver acabado gerar turmas por unidade
	if(step->itDia == divisao.cend())
	{
		HeuristicaNuno::logMsg("acabou, gerar turmas unidade", 3);
		auto clustersUnid = ofertaDisc->getCampus()->getClustersUnids();
		for(auto itClust = clustersUnid.cbegin(); itClust != clustersUnid.cend(); ++itClust)
		{
			const bool compSec = (ofertaDisc->temCompTeorica() && !teorico);
			geraTurmasUnidade_(ofertaDisc, *itClust, calendario, teorico, *(step->aulas), *(step->alunosDisp), 
								*(step->salasDisp), profsAssoc, turmasPotenciais, turmasDivisao, compSec, true);
		}
		return;
	}

	// atual dia da divisão
	HeuristicaNuno::logMsg("acess info iterador", 3);
	int dia = step->itDia->first;
	int nrCreds = step->itDia->second;

	// proxima posição do iterador
	HeuristicaNuno::logMsg("get next iterador", 3);
	vector<pair<int,int>>::const_iterator itNext = std::next(step->itDia);

	// verificar se há creditos neste dia. se não houver criar proximo step e inserir na queue
	if(nrCreds == 0)
	{
		// nest step
		HeuristicaNuno::logMsg("zero creditos continuar", 3);
		RecursivePreProc* nextStep = new RecursivePreProc(itNext, *(step->alunosDisp), *(step->salasDisp), *(step->aulas));
		queueSteps.push_back(nextStep);

		// apagar step atual à saída
		return;
	}

	// encontrar disponibilidades no dia
	HeuristicaNuno::logMsg("check disponibilidades", 3);
	auto itDispDia = disponibilidades.find(dia);
	if(itDispDia == disponibilidades.end())
		return;

	// encontrar disponibilidades de creditos
	auto itDispCreds = itDispDia->second.find(nrCreds);
	if(itDispCreds == itDispDia->second.end())
		return;

	if(itDispCreds->second.size() == 0)
		return;

	// tentar combinacao com cada aula
	unordered_set<AlunoHeur*> alunosComb;
	unordered_set<SalaHeur*> salasComb;
	unordered_set<AlunoHeur*> alunosDispAula;
	unordered_set<SalaHeur*> salasDispAula;
	HeuristicaNuno::logMsg("copy aulas", 3);
	unordered_map<int, AulaHeur*> aulasMap (*(step->aulas));
	bool temFormando = false;
	bool temCoReq = false;
	HeuristicaNuno::logMsg("it aulas", 3);
	for(auto itAula = itDispCreds->second.cbegin(); itAula != itDispCreds->second.cend(); ++itAula)
	{
		alunosComb.clear();
		salasComb.clear();
		alunosDispAula.clear();
		salasDispAula.clear();
		aulasMap[dia] = itAula->first;

		// verificar se criar divisao com esta aula viola interjornada
		if(HeuristicaNuno::probData->parametros->considerarDescansoMinProf)
		{
			int diaAnt = UtilHeur::diaAnterior(dia);
			auto itDivAnt = aulasMap.find(diaAnt);
			if(itDivAnt != aulasMap.end() && ProfessorHeur::violaInterjornada(itDivAnt->second, itDivAnt->first, itAula->first, dia))
				continue;
		}


		// primeiro dia da divisão
		if(step->aulas->size() == 0)
		{
			HeuristicaNuno::logMsg("primeiro dia, copiar disp", 3);
			alunosComb = itAula->second.first;
			salasComb = itAula->second.second;
		}
		else
		{
			// primeiro verificar salas
			HeuristicaNuno::logMsg("intersetar salas", 3);
			salasDispAula = itAula->second.second;
			UtilHeur::intersectSets<SalaHeur*>(*(step->salasDisp), salasDispAula, salasComb);
			if(salasComb.size() == 0)
				continue;

			// verificar alunos
			HeuristicaNuno::logMsg("intersetar alunos", 3);
			alunosDispAula = itAula->second.first;
			UtilHeur::intersectSetsAlunos(ofertaDisc, *(step->alunosDisp), alunosDispAula, alunosComb, temFormando, temCoReq);
			if(!podeAbrirTurma_((int)alunosComb.size(), ofertaDisc->getNrCreds(teorico), temFormando, temCoReq, usaLab))
				continue;
		}

		// next step
		HeuristicaNuno::logMsg("add next step", 3);
		RecursivePreProc* nextStep = new RecursivePreProc(itNext, alunosComb, salasComb, aulasMap);
		queueSteps.push_back(nextStep);
	}
}

// gera turmas potenciais ofertadisciplina, unidade, componente, calendario, divisao e aulas possiveis
void AbridorTurmas::geraTurmasUnidade_(OfertaDisciplina* const ofertaDisc, const ConjUnidades* const clusterUnid, Calendario * const calendario, 
							bool teorico, unordered_map<int, AulaHeur*> const &aulas,  unordered_set<AlunoHeur*> const &alunosDisp,
							unordered_set<SalaHeur*> const &salasDisp, unordered_set<ProfessorHeur*> const &profsDisp,
							turmasPotOrd &turmas, unordered_set<const TurmaPotencial*> &turmasDivisao,
							const bool componenteSec, bool checkProfs)
{
	AbridorTurmas::nrGeraUnids_++;

	// setar o campus e unidade das aulas
	unordered_map<int, AulaHeur*> newAulas;
	UtilHeur::aulasUnidade(aulas, clusterUnid->getUnidadeId(), newAulas);

	Disciplina* const disciplina = ofertaDisc->getDisciplina(teorico);
	//AbridorTurmas::nrGeraUnids_++;
		
	
	// encontrar alunos disponíveis
	bool temFormando, temCoReq;
	set<AlunoHeur*> alunosDisponiveis;
	getAlunosDispUnidade_(ofertaDisc, alunosDisp, newAulas, temFormando, temCoReq, alunosDisponiveis);
	HeuristicaNuno::logMsgInt("nr alunos disp: ", (int)alunosDisponiveis.size(), 3); 

	// verificar se pode abrir a turma. NOTA: deixar violar quando a turma é componente prática
	if(!podeAbrirTurma_((int)alunosDisponiveis.size(), ofertaDisc->getNrCreds(teorico), temFormando, disciplina->eLab(), temCoReq))
	{
		HeuristicaNuno::logMsg("não pode abrir turma!", 3);
		return;
	}

	// encontrar salas disponíveis na unidade
	unordered_set<SalaHeur*> salasDispUnidade;
	getSalasDispUnidade_(salasDisp, clusterUnid, newAulas, teorico, salasDispUnidade);
	int nrSalas = (int)salasDispUnidade.size();
	HeuristicaNuno::logMsgInt("nr salas disp unidade: ", nrSalas, 3); 
	if(nrSalas == 0)
	{
		HeuristicaNuno::logMsg("sem salas disp na unidade. continue", 2);
		return;
	}
	// escolher já a sala!!!!
	SalaHeur* const sala = escolherSala_(salasDispUnidade, (int)alunosDisponiveis.size());
	if(sala == nullptr)
	{
		HeuristicaNuno::logMsg("nenhuma sala pode ser escolhida", 2);
		return;
	}

	// encontrar profs disponíveis considerando deslocação para unidade
	set<ProfessorHeur*> profsDispUnidade;
	int algumPotDisp=0;
	int algumPotNaoDisp=0;
	// check n foi feito
	if(checkProfs)
	{		
		unordered_set<ProfessorHeur*> profsRealDisp;
		getProfessoresDisponiveis_(profsDisp, newAulas, algumPotDisp, algumPotNaoDisp, profsRealDisp);
		for(auto itPS = profsRealDisp.cbegin(); itPS != profsRealDisp.cend(); ++itPS)
			profsDispUnidade.insert(*itPS);
	}
	else
	{
		getProfessoresDispUnidade_(profsDisp, newAulas, profsDispUnidade);
	}

	HeuristicaNuno::logMsgInt("nr profs disp: ", (int)profsDispUnidade.size(), 2); 
		
	HeuristicaNuno::logMsg("pode abrir turma!", 2);
	HeuristicaNuno::logMsgInt("sala id: ", sala->getSala()->getId(), 2);
	HeuristicaNuno::logMsgInt("sala size: ", sala->getCapacidade(), 2);
	ProfessorHeur* const prof = escolherProfessor_(profsDispUnidade);
	if(!prof->ehVirtual())
		HeuristicaNuno::logMsgInt("prof id: ", prof->getProfessor()->getId(), 2);
	else
		HeuristicaNuno::logMsg("prof virtual", 2);

	// Escolher alunos
	int nrAlunosDisp = (int)alunosDisponiveis.size();
	int maxPossivel = std::min(sala->getCapacidade(), ofertaDisc->getMaxAlunos(teorico)); 
	if(maxPossivel == 0)
	{
		HeuristicaNuno::warning("AbridorTurmas::geraTurmasUnidade_", "Max alunos da disciplina ou capacidade da sala sao zero");
		return;
	}
	if(nrAlunosDisp > maxPossivel)
		escolherAlunos_(alunosDisponiveis, maxPossivel);

	// Criar objecto turma potencial!
	HeuristicaNuno::logMsg("criar turma potencial", 2);
	const TurmaPotencial* turmaPot = new TurmaPotencial(++nrTurmas_, ofertaDisc, calendario, teorico, newAulas, prof, sala, alunosDisponiveis, 
										nrAlunosDisp, algumPotDisp, algumPotNaoDisp);
			
	// adicionar às turmas potenciais
	HeuristicaNuno::logMsg("add turma potencial", 3);
	if(!addTurmaPotencial(turmaPot, turmas, turmasDivisao))
	{
		HeuristicaNuno::logMsg("nao adicionada deletar", 3);
		delete turmaPot;
		turmaPot = nullptr;
	}
	else
		HeuristicaNuno::logMsg("added", 3);
}

// [ESCOLHER TURMA POTENCIAL]

// com base nas turmas potenciais (separadas em várias lista de prioridade) escolher qual abrir
bool AbridorTurmas::escolherTurmaPotencial_(turmasPotOrd const &turmasPotenciais, const TurmaPotencial* &turmaEscolhida)
{
	HeuristicaNuno::logMsg("escolher turma", 2);

	// contar nr de turmas
	int nr = 0;
	for(auto it = turmasPotenciais.cbegin(); it != turmasPotenciais.cend(); ++it)
		nr += (int)it->second.size();
	HeuristicaNuno::logMsgInt("nr turmas pot: ", nr, 2);

	// Nivel 0 - Turmas com professor disponivel
	auto itTipo = turmasPotenciais.find(0);
	if((itTipo != turmasPotenciais.end()) && (itTipo->second.size() > 0))
	{
		HeuristicaNuno::logMsg("turmas prof disp", 2);
		return escolherTurmaPotencialLista_(itTipo->second, turmaEscolhida);
	}

	// se não abrir virtual retornar falso!
	if(!abrirVirtual)
		return false;

	// Nivel 1 - Turmas com horario com professores disponiveis mas sem professor atualmente
	itTipo = turmasPotenciais.find(1);
	if((itTipo != turmasPotenciais.end()) && (itTipo->second.size() > 0))
	{
		HeuristicaNuno::logMsg("turmas horarios com profs disp mas sem prof", 2);
		return escolherTurmaPotencialLista_(itTipo->second, turmaEscolhida);
	}

	// Nivel 2 - Turmas com horários que nenhum professor cadastrado pode cobrir
	itTipo = turmasPotenciais.find(2);
	if((itTipo != turmasPotenciais.end()) && (itTipo->second.size() > 0))
	{
		HeuristicaNuno::logMsg("turmas prof virtual nao disp", 2);
		return escolherTurmaPotencialLista_(itTipo->second, turmaEscolhida);
	}

	return false;
}
// escolher qual turma potencial abrir de uma lista
bool AbridorTurmas::escolherTurmaPotencialLista_(set<const TurmaPotencial*> const &turmasPotenciais, const TurmaPotencial* &turmaEscolhida)
{
	if(turmasPotenciais.size() == 0)
	{
		HeuristicaNuno::warning("escolherTurmaPotencialLista_", "A lista de turmas para escolher está vazia!!!");
		return false;
	}

	// ordenar por ordem decrescente de valor
	set<const TurmaPotencial*> candidatosFinais;

	auto itTurmas = turmasPotenciais.cbegin(); 
	double valorMax = (*itTurmas)->getValor();
	//HeuristicaNuno::logMsgInt("valorMax: ", valorMax, 2);
	for(;itTurmas != turmasPotenciais.cend(); ++itTurmas)
	{
		double valor = (*itTurmas)->getValor();
		//HeuristicaNuno::logMsgInt("valor: ", valor, 2);

		if( valorMax != 0 && ((valorMax-valor) / valorMax) <= ParametrosHeuristica::desvioMaximoValorTurma)
		{
			candidatosFinais.insert(*itTurmas);
		}
		else if ( valorMax == 0 && (valorMax-valor) <= ParametrosHeuristica::desvioMaximoValorTurma)
		{
			candidatosFinais.insert(*itTurmas);
		}
		else
			break;
	}

	if(candidatosFinais.size() == 0)
		HeuristicaNuno::excepcao("escolherTurmaPotencialLista_", "Candidatos finais não tem nenhuma turma potencial!");

	HeuristicaNuno::logMsg("escolher turma cands finais", 2);
	int rdIdx = rand() % candidatosFinais.size();
	if(rdIdx >= candidatosFinais.size())
		HeuristicaNuno::excepcao("AbridorTurmas::escolherTurmaPotencialLista_", "rdIdx out of bounds");
	turmaEscolhida = *(std::next(candidatosFinais.begin(), rdIdx));
	HeuristicaNuno::logMsg("turma escolhida", 2);

	if(turmaEscolhida == nullptr || turmaEscolhida == NULL)
		HeuristicaNuno::excepcao("AbridorTurmas::escolherTurmaPotencialLista_", "turma escolhida é nula");
	
	return true;
}

// [DISPONIBILIDADE ALUNOS]

// obter alunos que demandam a disciplina
void AbridorTurmas::getAlunosDemandaNaoAtend_(OfertaDisciplina* const ofertaDisc, unordered_set<AlunoHeur*>& alunosDem, bool compSec)
{
	// turma principal
	if(!compSec)
	{
		// Só alunos nível 1 prioridade
		unordered_set<AlunoDemanda*> demandas;
		solucao_->getDemandasNaoAtendidas(ofertaDisc->getCampus(), ofertaDisc->getDisciplina(), 1, demandas, true, priorAluno_);
		if(equiv_)
			solucao_->getDemandasNaoAtendidas(ofertaDisc->getCampus(), ofertaDisc->getDisciplina(), -1, demandas, true, priorAluno_);

		if(demandas.size() == 0)
		{
			HeuristicaNuno::logMsg("demandas não encontradas.", 3);
			return;
		}

		if(ofertaDisc->nrTiposAula() != 2)
		{
			solucao_->getSetAlunosFromSetAlunoDemanda(demandas, alunosDem);
		}
		else
		{
			// verificar se há alunos blacklisted
			auto itBlack = blacklist_.find(ofertaDisc);
			if(itBlack != blacklist_.end())
				solucao_->getSetAlunosFromSetAlunoDemanda(demandas, itBlack->second, alunosDem);
			else
				solucao_->getSetAlunosFromSetAlunoDemanda(demandas, alunosDem);
		}
		HeuristicaNuno::logMsgInt("alunos nao aloc disp: ", (int)alunosDem.size(), 2);
	}
	else
	{
		ofertaDisc->getAlunosIncompleto(alunosDem);
		if(alunosDem.size() == 0)
			HeuristicaNuno::warning("AbridorTurmas::getAlunosDisponiveis_", "nr de alunos incompletos é zero!");

		HeuristicaNuno::logMsgInt("alunos na comp principal: ", (int)alunosDem.size(), 2);
	}
}
// obter alunos disponiveis no calendario
void AbridorTurmas::getAlunosDispCalendario_(OfertaDisciplina* const ofertaDisc, Calendario* const calendario, unordered_set<AlunoHeur*> const &alunos, 
									unordered_set<AlunoHeur*> &alunosDisp, bool &temFormando, bool &temCoReq)
{
	temFormando = false;
	temCoReq = false;
	for(auto it = alunos.begin(); it != alunos.end(); ++it)
	{
		AlunoDemanda* const demanda = (*it)->getDemandaOriginal(ofertaDisc);
		if(UtilHeur::calendarioContemAnyHorarioTurno(calendario, demanda->demanda->getTurnoIES()))
		{
			alunosDisp.insert(*it);
			if(!temFormando && (*it)->ehFormando())
				temFormando = true;
			if(!temCoReq && (*it)->temCoRequisito(ofertaDisc))
				temCoReq = true;
		}
	}
}
// obter alunos (só considera alunos prioridade 1) que estejam disponíveis naquele horários naquela unidade
void AbridorTurmas::getAlunosDisponiveis_(OfertaDisciplina* const ofertaDisc, unordered_set<AlunoHeur*> const &alunosDem, 
										  const unordered_map<int, AulaHeur*> &aulas, bool &temFormando, bool &temCoReq,
										  const bool teorico, unordered_set<AlunoHeur*>& alunosDisponiveis)
{
	temFormando = false;
	temCoReq = false;
	for(auto itAluno = alunosDem.cbegin(); itAluno != alunosDem.cend(); ++itAluno)
	{
		AlunoHeur* const aluno = (*itAluno);

		// check
		if(ofertaDisc->temAlunoComp(aluno, teorico))
		{
			HeuristicaNuno::warning("AbridorTurmas::getAlunosDisponiveis_", "Aluno ja numa turma deste tipo da oferta!");
			continue;
		}

		// check disponibilidade
		if(!aluno->estaDisponivel(ofertaDisc, aulas))
			continue;

		alunosDisponiveis.insert(aluno);

		// check formando
		if(!temFormando && aluno->ehFormando())
			temFormando = true;
		// check co-requisito
		if(!temCoReq && aluno->temCoRequisito(ofertaDisc))
			temCoReq = true;
	}
}
// obter alunos que estejam disponíveis para aquelas aulas, de entre um conjunto dado à partida pois é tida em conta a deslocação
void AbridorTurmas::getAlunosDispUnidade_(OfertaDisciplina* const ofertaDisc, unordered_set<AlunoHeur*> const &alunos, const unordered_map<int, AulaHeur*> &aulas, 
											bool &temFormando, bool &temCoReq, set<AlunoHeur*>& alunosDisponiveis)
{
	temFormando = false;
	temCoReq = false;
	for(auto itAluno = alunos.begin(); itAluno != alunos.end(); ++itAluno)
	{
		AlunoHeur* aluno = *itAluno;
		// só basta verificar a disponibilidade relativamente a deslocações
		if(aluno->violaAlgumaDeslocacao(aulas))
			continue;
		
		alunosDisponiveis.insert(aluno);
		if(!temFormando && aluno->ehFormando())
			temFormando = true;
		if(!temCoReq && aluno->temCoRequisito(ofertaDisc))
			temCoReq = true;
	}
}

// obter alunos cuja aula teórica é imediatamente antes da aula pratica pretendida
void AbridorTurmas::getAlunosAulaCont_(OfertaDisciplina* const ofertaDisc, int dia, AulaHeur* const aulaPrat,
								  unordered_map<AlunoHeur*, TurmaHeur*> const &mapAlunoTurmaTeor,
								  unordered_set<AlunoHeur*>& alunosDisponiveis)
{
	if (!ofertaDisc->getDisciplina()->aulasContinuas())
		return;

	for(auto itAluno = mapAlunoTurmaTeor.cbegin(); itAluno != mapAlunoTurmaTeor.cend(); ++itAluno)
	{
		AlunoHeur* const aluno = itAluno->first;

		// check
		if(ofertaDisc->temAlunoComp(aluno, false))
		{
			HeuristicaNuno::warning("AbridorTurmas::getAlunosDispAulaCont_", "Aluno ja numa turma deste tipo da oferta!");
			continue;
		}

		TurmaHeur* turmaTeor = itAluno->second;

		AulaHeur *aulaTeor=nullptr;
		turmaTeor->getAulaDia(dia,aulaTeor);

		DateTime endTimeTeor;
		aulaTeor->getLastHor(endTimeTeor);

		DateTime startTimePrat;
		aulaPrat->getPrimeiroHor(startTimePrat);

		// check se aula prática vem logo depois da aula teórica
		if (startTimePrat >= endTimeTeor)
		{
			int diff = (startTimePrat - endTimeTeor).timeMin();
			if(diff<=ParametrosHeuristica::maxIntervAulas)
			{
				alunosDisponiveis.insert(aluno);
			}
		}
	}
}

// [DISPONIBILIDADE SALAS]

// obter salas que tenham um numero de horarios disponiveis suficiente para atender a divisão
void AbridorTurmas::getSalasDispDivisao_(Calendario* const calendario, unordered_set<SalaHeur*> const &salasAssoc, vector<pair<int, int>> const &divisao,
							unordered_set<SalaHeur*> &salasDisp)
{
	// TESTE
	//salasDisp = salasAssoc;
	//return;

	// inserir horarios do calendario por dia
	unordered_map<int, GGroup<HorarioAula*, LessPtr<HorarioAula>>> horariosCalDia;
	for(auto itD = divisao.cbegin(); itD != divisao.cend(); ++itD)
	{
		if(itD->second <= 0)
			continue;

		horariosCalDia[itD->first] = calendario->retornaHorariosDisponiveisNoDia(itD->first);
	}

	for(auto it = salasAssoc.begin(); it != salasAssoc.end(); ++it)
	{
		bool add = true;
		for(auto itDiv = divisao.cbegin(); itDiv != divisao.cend(); ++itDiv)
		{
			if(itDiv->second <= 0)
				continue;

			auto& horarios = horariosCalDia[itDiv->first];
			// verificar se o numero de horarios disponiveis é menor que o numero de creditos
			if(!(*it)->temHorsSeqDisponivel(itDiv->second, itDiv->first, horarios))
			{
				add = false;
				break;
			}
		}
		if(add)
			salasDisp.insert(*it);
	}
}
// obter salas que estejam disponíveis naquele horário
void AbridorTurmas::getSalasDisponiveis_(unordered_set<SalaHeur*> const &salasAssoc, const unordered_map<int, AulaHeur*> &aulas, 
											bool teorico, unordered_set<SalaHeur*> &salasDisponiveis)
{
	for(auto itSalas = salasAssoc.begin(); itSalas != salasAssoc.end(); itSalas++)
	{
		if((*itSalas)->estaDisponivel(aulas))
			salasDisponiveis.insert(*itSalas);
	}
}
// obter salas daquele conjunto de unidades equivalentes que estejam disponíveis naquele horário
void AbridorTurmas::getSalasDispUnidade_(unordered_set<SalaHeur*> const &salasDisponiveis, const ConjUnidades* const clusterUnid, 
											const unordered_map<int, AulaHeur*> &aulas, bool teorico, unordered_set<SalaHeur*> &salasDispUnid)
{
	HeuristicaNuno::logMsg("AbridorTurmas::getSalasDisponiveisUnidade_", 3);
	
	for(auto itSalas = salasDisponiveis.begin(); itSalas != salasDisponiveis.end(); itSalas++)
	{
		if(clusterUnid->temUnidade((*itSalas)->getSala()->getIdUnidade()))
			salasDispUnid.insert(*itSalas);
	}
}

// [DISPONIBILIDADE PROFESSORES]

// obter professores disponiveis para divisao
void AbridorTurmas::getProfessoresDispDivisao_(Calendario* const calendario, unordered_set<ProfessorHeur*> const &profsAssoc, vector<pair<int, int>> const &divisao,
							unordered_set<ProfessorHeur*> &profsDisp)
{
	// inserir horarios do calendario por dia
	unordered_map<int, GGroup<HorarioAula*, LessPtr<HorarioAula>>> horariosCalDia;
	for(auto itD = divisao.cbegin(); itD != divisao.cend(); ++itD)
	{
		if(itD->second <= 0)
			continue;

		horariosCalDia[itD->first] = calendario->retornaHorariosDisponiveisNoDia(itD->first);
	}

	for(auto it = profsAssoc.begin(); it != profsAssoc.end(); ++it)
	{
		bool add = true;
		for(auto itDiv = divisao.cbegin(); itDiv != divisao.cend(); ++itDiv)
		{
			if(itDiv->second <= 0)
				continue;

			auto& horarios = horariosCalDia[itDiv->first];
			// verificar se o numero de horarios disponiveis é menor que o numero de creditos
			if(!(*it)->temHorsSeqDisponivel(itDiv->second, itDiv->first, horarios))
			{
				add = false;
				break;
			}
		}
		if(add)
			profsDisp.insert(*it);
	}
}
// recebe as salas que estam disponíveis naquele horário e verifica quais pertencem ao conjunto de unidades
void AbridorTurmas::getProfessoresDisponiveis_(unordered_set<ProfessorHeur*> const &profsAssoc, const unordered_map<int, AulaHeur*> &aulas, 
							int &algumPotDisp, int &algumPotNaoDisp, unordered_set<ProfessorHeur*>& profsDisponiveis)
{
	algumPotDisp = 0;
	for(auto itProf = profsAssoc.cbegin(); itProf != profsAssoc.cend(); ++itProf)
	{
		// verificar se neste momento específico está disponível
		if((*itProf)->estaDisponivel(aulas))
		{
			profsDisponiveis.insert(*itProf);
			algumPotDisp++;
		}
	}

	algumPotNaoDisp=0;
	if(!algumPotDisp)
	{
		for(auto itProf = profsAssoc.cbegin(); itProf != profsAssoc.cend(); ++itProf)
		{
			// verificar se ele tem os horarios das aulas cadastrado nas disponibilidades
			if((*itProf)->estaDisponivelHorarios(aulas))
				algumPotNaoDisp++;
		}
	}
}
// recebe os profs que estam disponíveis naquele horário e verifica quais estão disponiveis tendo em conta a deslocação
void AbridorTurmas::getProfessoresDispUnidade_(unordered_set<ProfessorHeur*> const &profsDisp, const unordered_map<int, AulaHeur*> &aulas,
												set<ProfessorHeur*>& profsDispUnidade)
{
	auto itProf = profsDisp.begin();
	for( ; itProf != profsDisp.end(); ++itProf)
	{
		// só basta verificar a disponibilidade relativamente a deslocações
		if((*itProf)->violaAlgumaDeslocacao(aulas))
			continue;
		
		profsDispUnidade.insert(*itProf);
	}
}


// nr turmas simultaneas da disciplina
int AbridorTurmas::nrTurmasSimultaneasDisc_(Disciplina* const disciplina, int dia, AulaHeur* const aula) const
{
	auto itDisc = solucao_->turmasPorDisc_.find(disciplina);
	if(itDisc == solucao_->turmasPorDisc_.end())
		return 0;

	int nr = 0;
	for(auto it = itDisc->second.begin(); it != itDisc->second.end(); ++it)
	{
		if((*it)->ehIncompativel(dia, aula, true))
			nr++;
	}
	return nr;
}

// verifica se a turma pode ser aberta
bool AbridorTurmas::podeAbrirTurma_(int nrAlunos, int nrCreds, bool temFormando, bool usaLab, bool temCoReq)
{
	if(nrAlunos == 0)
		return false;

	// minimo de alunos. se não for estabelecido considerar o parâmetro
	int limMinAlunos = ParametrosHeuristica::getMinAlunos(usaLab);

	// tem formando
	if(HeuristicaNuno::probData->parametros->violar_min_alunos_turmas_formandos && temFormando)
	{
		if(anySize_)
			return true;
		else
			limMinAlunos = ParametrosHeuristica::limMinAlunosTurma;
	}
	// tem aluno com co-requisito
	else if(HeuristicaNuno::probData->parametros->considerarCoRequisitos && temCoReq)
	{
		return true;
	}

	if(relaxMin_ > 0)
		limMinAlunos = (int)std::ceil(relaxMin_ * limMinAlunos);

	// tem min alunos
	if(nrAlunos >= limMinAlunos)
		return true;

	return false;
}
// escolher a sala de entre as disponíveis
SalaHeur* AbridorTurmas::escolherSala_(unordered_set<SalaHeur*> const &salas, int nrAlunos)
{
	if(salas.size() == 0)
		HeuristicaNuno::excepcao("SolucaoHeur::escolherSala_", "Conjunto de Salas vazio!!!");

	set<SalaHeur*> salasMaioresIguais;
	set<SalaHeur*> salasMenores;

	// separar entre salas com capacidade suficiente e sem
	for(auto itSala = salas.cbegin(); itSala != salas.cend(); ++itSala)
	{
		SalaHeur* sala = (*itSala);
		int capacidade = sala->getSala()->getCapacidade();
		if(capacidade >= nrAlunos)
			salasMaioresIguais.insert(sala);
		else
			salasMenores.insert(sala);
	}

	// salas com capacidade
	if(salasMaioresIguais.size() > 0)
	{
		// escolher uma sala randomica entre as que têm a capacidade minima
		SalaHeur* const bestMaior = *(salasMaioresIguais.begin());
		int capacidade = bestMaior->getCapacidade();
		double indicDem = -1.0;
		if(ParametrosHeuristica::indicDemSalas)
			indicDem = bestMaior->getIndicDem();

		// get menores salas maiores com capacidade e indic demanda igual
		set<SalaHeur*> minSalasMaior;
		salasCapIgual_(salasMaioresIguais, capacidade, indicDem, minSalasMaior, true);

		int rdIdx = rand() % minSalasMaior.size();
		if(rdIdx >= minSalasMaior.size())
			HeuristicaNuno::excepcao("SolucaoHeur::escolherSala_", "rdIdx out of bounds!");
		SalaHeur* minSalaMaior = *(std::next(minSalasMaior.begin(), rdIdx));
		if(minSalaMaior == nullptr)
			HeuristicaNuno::excepcao("SolucaoHeur::escolherSala_", "minSalaMaior é um nullptr!");
		

		double capUsada = double(nrAlunos)/capacidade;
		// não há restrição estricta no uso de capacidade da sala maior
		if(capUsada >= ParametrosHeuristica::minCapacidadeUsoSala || salasMenores.size() == 0)
			return minSalaMaior;
	}

	if(salasMenores.size() == 0)
		return nullptr;

	// escolher maior sala entre as menores
	int maiorCap = (*salasMenores.rbegin())->getCapacidade();
	// get salas com maior capacidade
	set<SalaHeur*> maxSalasMenor;
	salasCapIgual_(salasMenores, maiorCap, -1.0, maxSalasMenor, false);

	if(!ParametrosHeuristica::indicDemSalas)
	{
		int rdIdxMax = rand() % maxSalasMenor.size();
		if(rdIdxMax >= maxSalasMenor.size())
			HeuristicaNuno::excepcao("SolucaoHeur::escolherSala_", "rdIdxMax out of bounds!");
		SalaHeur* const maxSalaMenor = *(std::next(maxSalasMenor.begin(), rdIdxMax));
		if(maxSalaMenor == nullptr)
			HeuristicaNuno::excepcao("SolucaoHeur::escolherSala_", "maxSalaMenor é um nullptr!");

		return maxSalaMenor;
	}
	else
	{
		if((*maxSalasMenor.begin())->getCapacidade() != maiorCap)
			HeuristicaNuno::excepcao("SolucaoHeur::escolherSala_", "Capacidade diferente da esperada!");

		// get salas com menor indice demanda de entre as maiores
		double menorIndDem = (*maxSalasMenor.begin())->getIndicDem();
		set<SalaHeur*> maxSalasInd;
		salasCapIgual_(maxSalasMenor, maiorCap, menorIndDem, maxSalasInd, true);

		if(maxSalasInd.size() > 0)
		{
			int rdIdxMax = rand() % maxSalasInd.size();
			if(rdIdxMax >= maxSalasInd.size())
				HeuristicaNuno::excepcao("SolucaoHeur::escolherSala_", "rdIdxMax out of bounds!");

			return *(std::next(maxSalasInd.begin(), rdIdxMax));
		}
		else
		{
			HeuristicaNuno::warning("SolucaoHeur::escolherSala_", "Algo estranho, indice igual nao encontrado!");
			return(*maxSalasMenor.begin());
		}
	}
}
// get salas com capacidade igual a essa. OBS: salas têm que estar ordenadas por capacidade
void AbridorTurmas::salasCapIgual_(set<SalaHeur*> const &salas, int capacidade, double indicDem, set<SalaHeur*> &result, bool ascend)
{
	bool found = false;

	if(salas.size() == 0)
		return;

	// procura ascendente
	if(ascend)
	{
		for(auto it = salas.begin(); it != salas.end(); ++it)
		{
			SalaHeur* const sala = *it;
			if(sala->getCapacidade() == capacidade && 
				(!ParametrosHeuristica::indicDemSalas || (indicDem == -1.0) || (sala->getIndicDem() <= indicDem)))
			{
				result.insert(sala);
				found = true;
			}
			else if(found)
				return;
		}
	}
	// procura descendente
	else
	{
		for(auto it = salas.rbegin(); it != salas.rend(); ++it)
		{
			SalaHeur* const sala = *it;
			if(sala->getCapacidade() == capacidade && 
				(!ParametrosHeuristica::indicDemSalas || (indicDem == -1.0) || (sala->getIndicDem() <= indicDem)))
			{
				result.insert(sala);
				found = true;
			}
			else if(found)
				return;
		}
	}

	if( result.size() == 0 )
		HeuristicaNuno::excepcao("AbridorTurmas::salasCapIgual_", "Nao encontrada nenhuma sala!");
}


// escolher o professor de entre os disponíveis
ProfessorHeur* AbridorTurmas::escolherProfessor_(set<ProfessorHeur*> const &professores)
{
	if(professores.size() > 0)
	{
		int rdIdx = rand() % professores.size();
		ProfessorHeur* escolhido = *(std::next(professores.begin(), rdIdx));
		return escolhido;
	}

	// Retornar Professor Virtual
	ProfessorHeur* const ptrProf = solucao_->professoresHeur.at(ParametrosHeuristica::profVirtualId);

	return ptrProf;
}
// escolher os alunos de entre os disponíveis
void AbridorTurmas::escolherAlunos_(set<AlunoHeur*> &alunos, int max)
{
	int diff = (int)alunos.size() - max;
	if(diff <= 0)
		return;

	set<AlunoHeur*>::const_iterator it = alunos.end();
	int nr = 0;
	for(; nr < diff; --it)
	{
		nr++;
	}

	// eliminar os 'diff' piores
	alunos.erase(it, alunos.end());
}


// add turma potencial
bool AbridorTurmas::addTurmaPotencial(const TurmaPotencial* turma, turmasPotOrd &turmasPot, unordered_set<const TurmaPotencial*> &turmasDivisao)
{
	// procurar conjunto de turmas daquele tipo
	int tipo = turma->getTipoTurma();
	auto it = turmasPot.find(tipo);
	if(it == turmasPot.end())
	{
		set<const TurmaPotencial*> emptySet;
		it = turmasPot.insert(make_pair(tipo, emptySet)).first;
	}
	// adicionar às turmas daquele tipo
	return addTurmaPotencialTipo(turma, turmasPot, it->second, turmasDivisao);
}
// add turma potencial
bool AbridorTurmas::addTurmaPotencialTipo(const TurmaPotencial* turma, turmasPotOrd &turmasPot, set<const TurmaPotencial*> &turmasPotTipo, unordered_set<const TurmaPotencial*> &turmasDivisao)
{
	// se tiver vazio inserir!
	if(turmasPotTipo.size() == 0)
	{
		HeuristicaNuno::logMsg("turmas pot vazias", 3);
		if(!turmasPotTipo.insert(turma).second)
			HeuristicaNuno::excepcao("AbridorTurmas::geraTurmasDivisaoUnidade_", "Turma potencial nao adicionada!");

		if(!turmasDivisao.insert(turma).second)
			HeuristicaNuno::excepcao("AbridorTurmas::addTurmaPotencial", "Turma potencial nao adicionada as turmas divisao!");

		return true;
	}

	// verificar se o desvio do máximo é maior que o permitido
	HeuristicaNuno::logMsg("calcular valor", 3);
	double valor = turma->getValor();
	double valorMax = (*turmasPotTipo.begin())->getValor();
	double desvio = ((valorMax-valor) / valorMax);
	if(desvio > ParametrosHeuristica::desvioMaximoValorTurma)
		return false;
	
	// verificar se nova turma é dominada. se for dominada não adicionar
	HeuristicaNuno::logMsg("verificar dominancia", 3);
	if(ParametrosHeuristica::dominanciaTurmas && verificaDominancia_(turma, turmasPot, turmasDivisao))
		return false;

	// se o tamanho for menor que o maximo adicionar
	if(turmasPotTipo.size() < ParametrosHeuristica::maxTurmasPotenciais)
	{
		HeuristicaNuno::logMsg("nr de turmas potenciais menor que max", 3);

		// não é dominada, então adicionar
		if(!turmasPotTipo.insert(turma).second)
			HeuristicaNuno::excepcao("AbridorTurmas::addTurmaPotencial", "Turma potencial nao adicionada!");

		// turmas divisao
		if(!turmasDivisao.insert(turma).second)
			HeuristicaNuno::excepcao("AbridorTurmas::addTurmaPotencial", "Turma potencial nao adicionada as turmas divisao!");

		return true;
	}

	// excede o tamanho.
	HeuristicaNuno::logMsg("excede o tamanho", 3);
	double valorMin = (*turmasPotTipo.rbegin())->getValor();
	if(valor < valorMin)
		return false;
	else if(valor == valorMin)			// escolher randomicamente se entra ou não. 50% chance
	{
		int r = rand() % 100;
		if(r < 50)
			return false;
	}

	// Agora ela sim ela vai entrar! Remover randomicamente um de entre os piores e adicionar novo.
	HeuristicaNuno::logMsg("remover pior", 3);
	removePiorRandom(turmasPotTipo, turmasDivisao);

	HeuristicaNuno::logMsg("adicionar", 3);
	if(!turmasPotTipo.insert(turma).second)
		HeuristicaNuno::excepcao("AbridorTurmas::addTurmaPotencial", "Turma potencial nao adicionada!");
	if(!turmasDivisao.insert(turma).second)
		HeuristicaNuno::excepcao("AbridorTurmas::addTurmaPotencial", "Turma potencial nao adicionada as turmas divisao!");

	// remover as turmas que já não são boas suficientes
	HeuristicaNuno::logMsg("check remover piores", 3);
	valorMax = (*turmasPotTipo.begin())->getValor();
	removeBadTurmas(turmasPotTipo, turmasDivisao, valorMax);
	HeuristicaNuno::logMsg("done", 3);

	if(turmasPotTipo.size() == 0)
		HeuristicaNuno::excepcao("AbridorTurmas::addTurmaPotencial", "Zero turmas potenciais !");

	return true;
}
// verifica se a turma é dominada por outras turmas já geradas. retorna true se a nova turma for dominada
bool AbridorTurmas::verificaDominancia_(const TurmaPotencial* novaTurma, turmasPotOrd &turmas, unordered_set<const TurmaPotencial*> &turmasDivisao)
{
	HeuristicaNuno::logMsg("AbridorTurmas::verificaDominancia_", 3);
	// verificar se a turma é dominada ou domina algumas turmas potenciais previamente geradas
	for(auto itTurmasDiv = turmasDivisao.cbegin(); itTurmasDiv != turmasDivisao.cend();)
	{
		const TurmaPotencial* const turma = (*itTurmasDiv);
		// é dominada?
		if(turma->domina(novaTurma))
		{
			HeuristicaNuno::logMsg("out. AbridorTurmas::verificaDominancia_", 3);
			return true;
		}

		// domina?
		if(!novaTurma->domina(turma))
		{
			++itTurmasDiv;
			continue;
		}

		// nova turma domina old turma:
		// eliminar das turmas potenciais
		auto itOldTipo = turmas.find(turma->getTipoTurma());
		if(itOldTipo == turmas.end())
			HeuristicaNuno::excepcao("AbridorTurmas::verificaDominancia_", "Tipo da turma nao encontrado nas turmas potenciais.");
		auto itOld = itOldTipo->second.find(*itTurmasDiv);
		if(itOld == itOldTipo->second.end())
			HeuristicaNuno::excepcao("AbridorTurmas::verificaDominancia_", "Turma ja tinha sido eliminada das turmas potenciais.");
		itOldTipo->second.erase(itOld);

		// eliminar das turmas divisão
		itTurmasDiv = turmasDivisao.erase(itTurmasDiv);
	}

	HeuristicaNuno::logMsg("out. AbridorTurmas::verificaDominancia_", 3);
	return false;
}
// remove randomicamente uma turma das piores
void AbridorTurmas::removePiorRandom(set<const TurmaPotencial*> &turmasPot, unordered_set<const TurmaPotencial*> &turmasDivisao)
{
	if(turmasPot.size() == 0)
		return;

	auto itP = std::prev(turmasPot.end());
	double piorValor = (*itP)->getValor();

	unordered_set<const TurmaPotencial*> piores;
	for(auto it = turmasPot.begin(); it != turmasPot.end(); it++)
	{
		if((*it)->getValor() <= piorValor)
		{
			const TurmaPotencial* const ptr = (*it);
			piores.insert(ptr);
		}
	}

	if(piores.size() == 0)
		HeuristicaNuno::excepcao("AbridorTurmas::removePiorRandom", "Pior não encontrado");

	int rdIdx = rand() % piores.size();
	if(rdIdx >= piores.size())
		HeuristicaNuno::excepcao("AbridorTurmas::removePiorRandom", "rdIdx out of bounds!");

	const TurmaPotencial* turmaRem =  *(std::next(piores.begin(), rdIdx));

	// removerturma potencial
	auto itPot = turmasPot.find(turmaRem);
	if(itPot != turmasPot.end())
		turmasPot.erase(itPot);
	else
		HeuristicaNuno::warning("AbridorTurmas::removePiorRandom", "Pior a remover nao encontrado em turmas potenciais");

	auto itDiv = turmasDivisao.find(turmaRem);
	if(itDiv != turmasDivisao.end())
		turmasDivisao.erase(itDiv);

	// apagar turma removida
	delete turmaRem;
}
// remove as tuas cujo desvio do melhor valor excede o maximo
void AbridorTurmas::removeBadTurmas(set<const TurmaPotencial*> &turmasPot, unordered_set<const TurmaPotencial*> &turmasDivisao, double valorMax)
{
	double valor;
	double desvio;
	for(auto it = turmasPot.begin(); it != turmasPot.end();)
	{
		valor = (*it)->getValor();
		desvio = ((valorMax-valor) / valorMax);
		if((valor != valorMax) && (desvio > ParametrosHeuristica::desvioMaximoValorTurma))
		{
			const TurmaPotencial* tp = *it;
			HeuristicaNuno::logMsg("remover pior turmasPot", 3);
			it = turmasPot.erase(it);

			HeuristicaNuno::logMsg("remover pior turmasDivisao", 3);
			auto itDiv = turmasDivisao.find(tp);
			if(itDiv != turmasDivisao.end())
				turmasDivisao.erase(itDiv);

			// apagar turma removida
			delete tp;

			continue;
		}
		else
			++it;
	}
}

	// limpar turmas potenciais. limpa memória
void AbridorTurmas::limparTurmasPotenciais_(turmasPotOrd &turmas)
{
	HeuristicaNuno::logMsg("in limpar turmas potenciais", 3);
	for(auto itTipo = turmas.begin(); itTipo != turmas.end();)
	{
		for(auto it = itTipo->second.begin(); it != itTipo->second.end();)
		{
			const TurmaPotencial* turma = (*it);
			it = itTipo->second.erase(it);
			delete turma;
		}
		itTipo = turmas.erase(itTipo);
	}

	if(turmas.size() > 0)
		HeuristicaNuno::excepcao("AbridorTurmas::limparTurmasPotenciais_", "Conjunto nao esta vazio!");

	HeuristicaNuno::logMsg("out limpar turmas potenciais", 3);
}

// [ENDREGION]


// CRIAÇÃO DE TURMAS POTENCIAIS V2

// TODO

// [ENDREGION]



// [UTIL]

// adicionar a blacklist
void AbridorTurmas::addToBlackList(OfertaDisciplina* const oferta, unordered_set<AlunoHeur*> const &alunos)
{
	// alunos que já tinham sido removidos uma vez
	unordered_set<int> repetentes;

	auto itRemOft = removidos_.find(oferta);
	if(itRemOft == removidos_.end())
	{
		unordered_set<int> alunosIds;
		auto par = removidos_.insert(make_pair(oferta, alunosIds));
		if (!par.second)
			HeuristicaNuno::excepcao("AbridorTurmas::addToBlackList", "mapa de removidos da oferta nao adicionado");

		itRemOft = par.first;
	}
	// verificar se estao nos removidos
	for(auto itAlunos = alunos.begin(); itAlunos != alunos.end(); ++itAlunos)
	{
		int alunoId = (*itAlunos)->getId();
		auto itRemAluno = itRemOft->second.find(alunoId);
		if(itRemAluno == itRemOft->second.end())
			itRemOft->second.insert(alunoId);
		else
		{
			itRemOft->second.erase(itRemAluno);
			repetentes.insert(alunoId);
		}
	}

	if(repetentes.size() == 0)
		return;

	// adicionar à blacklist
	auto itOft = blacklist_.find(oferta);
	if(itOft == blacklist_.end())
	{
		unordered_set<int> alunosIds;
		auto par = blacklist_.insert(make_pair(oferta, alunosIds));
		if(!par.second)
			HeuristicaNuno::excepcao("AbridorTurmas::addToBlackList", "blacklist da oferta nao adicionada");

		itOft = par.first;
	}
	
	for(auto itRep = repetentes.begin(); itRep != repetentes.end(); ++itRep)
	{
		itOft->second.insert(*itRep);
	}

	HeuristicaNuno::logMsgInt("blacklist + ", (int)repetentes.size(), 1);
}

// nr profs assoc da disciplina
int AbridorTurmas::nrProfsAssociadosHorario(OfertaDisciplina* const ofertaDisc, int dia, AulaHeur* const aula) const
{
	unordered_set<ProfessorHeur*> profs;
	ofertaDisc->getProfessoresAssociados(profs);
	int nr = 0;
	for(auto it = profs.cbegin(); it != profs.cend(); ++it)
	{
		if((*it)->estaDisponivelHorarios(dia, aula))
			nr++;
	}
	return nr;
}

// nr profs livres assoc da disciplina
int AbridorTurmas::nrProfsLivresAssocEstimado(OfertaDisciplina* const ofertaDisc) const
{
	unordered_set<ProfessorHeur*> profs;
	ofertaDisc->getProfessoresAssociados(profs);
	int nr = 0;
	for(auto it = profs.cbegin(); it != profs.cend(); ++it)
	{
		if((*it)->nroCredsLivresEstimados() >= ofertaDisc->getNrCreds())
			nr++;
	}
	return nr;
}

// [ENDREGION]