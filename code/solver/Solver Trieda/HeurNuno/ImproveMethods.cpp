#include "ImproveMethods.h"
#include "SalaHeur.h"
#include "TurmaHeur.h"
#include "OfertaDisciplina.h"
#include "HeuristicaNuno.h"
#include "ParametrosHeuristica.h"
#include "SolucaoHeur.h"
#include "AbridorTurmas.h"
#include "../ProblemData.h"
#include "../ParametrosPlanejamento.h"

ImproveMethods::ImproveMethods(void)
{
}


ImproveMethods::~ImproveMethods(void)
{
}


// [TENTAR REALOCAR ALUNOS POR ORDENAMENTO DE DISPONIBILIDADE]

// tentar realocar alunos nas aulas principais, quando só há um tipo de aula para maximizar demanda atendida
void ImproveMethods::tryRealocAlunosPrinc(SolucaoHeur* const solucao, OfertaDisciplina* const ofertaDisc, bool mudarSala, bool equiv,
											int priorAluno, double relaxMin)
{
	if(ofertaDisc->nrTiposAula() != 1 && ParametrosHeuristica::relacPraticas != ParametrosHeuristica::abrePraticas::N_N)
	{
		HeuristicaNuno::warning("ImproveMethods::tryRealocAlunosPrinc", "Oferta disciplina não tem só uma componente!");
		return;
	}

	HeuristicaNuno::logMsg("ImproveMethods::tryRealocAlunosPrinc", 2);

	// alocados no inicio
	int nrAlocCompInicio = ofertaDisc->nrAlunosCompleto();

	// guardar alocacao atual
	unordered_map<TurmaHeur*, pair<unordered_set<AlunoHeur*>, SalaHeur*>> oldAlocOft;
	ofertaDisc->getAlocacaoAtual(oldAlocOft);

	// get turmas principais
	unordered_set<TurmaHeur*> turmasPrinc;
	ofertaDisc->getTurmasTipo(ofertaDisc->temCompTeorica(), turmasPrinc);
	if(turmasPrinc.size() <= 1)
	{
		HeuristicaNuno::logMsg("so uma turma princ. return.", 2);
		return;
	}
	resetNrDisp(turmasPrinc);

	// verificar se ha demandas suficientes
	unordered_set<AlunoDemanda *> demandas;
	solucao->getDemandasNaoAtendidas(ofertaDisc->getCampus(), ofertaDisc->getDisciplina(), 1, demandas, true, priorAluno);
	unordered_set<AlunoDemanda *> demandasEquiv;
	if(equiv)
		solucao->getDemandasNaoAtendidas(ofertaDisc->getCampus(), ofertaDisc->getDisciplina(), -1, demandasEquiv, true, priorAluno);
	if(demandas.size() == 0 && demandasEquiv.size() == 0)
	{
		HeuristicaNuno::logMsg("sem demandas adicionais. return.", 2);
		return;
	}

	// remover os alunos das turmas
	unordered_set<AlunoHeur*> alunosInc;
	ofertaDisc->removerTodosAlunos(alunosInc);
	if(alunosInc.size() == 0)
	{
		HeuristicaNuno::logMsg("sem alunos removidos. return.", 2);
		return;
	}

	// se mudar as salas, reduzir
	if(mudarSala)
		ImproveMethods::tryReduzirSalas(ofertaDisc);

	// contar quantos alunos estao disponiveis por turma (considerar não atendidos)
	unordered_map<TurmaHeur*, int> dispTurma;
	unordered_map<AlunoHeur*, unordered_set<TurmaHeur*>> alunosTurma;
	checkAlunosDispTurmas(alunosInc, turmasPrinc, dispTurma, alunosTurma);

	// agora verificar demanda não atendida
	unordered_set<AlunoHeur*> alunosNA;
	unordered_map<AlunoHeur*, unordered_set<TurmaHeur*>> alunosTurmaNA;		// associação de alunos não atendidos
	bool algumNa = false;
	if(demandas.size() != 0)
	{
		// usar estes alunos só para contabilizar disponibilidade pelas turmas
		solucao->getSetAlunosFromSetAlunoDemanda(demandas, alunosNA);
		filtrarAlunosExcCreds(ofertaDisc->getNrCreds(), alunosNA);
		algumNa = checkAlunosDispTurmas(alunosNA, turmasPrinc, dispTurma, alunosTurmaNA);
	}

	// verificar demanda não atendida equivalente!
	bool algumEquiv = false;
	unordered_set<AlunoHeur*> alunosNAEquiv;
	unordered_map<AlunoHeur*, unordered_set<TurmaHeur*>> alunosTurmaNAEquiv;		// associação de alunos não atendidos
	if(equiv && demandasEquiv.size() != 0)
	{
		// usar estes alunos só para contabilizar disponibilidade pelas turmas
		solucao->getSetAlunosFromSetAlunoDemanda(demandasEquiv, alunosNAEquiv);
		filtrarAlunosExcCreds(ofertaDisc->getNrCreds(), alunosNAEquiv);
		algumEquiv = checkAlunosDispTurmas(alunosNAEquiv, turmasPrinc, dispTurma, alunosTurmaNAEquiv);
	}

	// tentar alocar alunos incompletos que já pertenciam à turma
	tryAlocAlunosComp(dispTurma, alunosTurma, mudarSala);

	// tentar alocar alunos que não tinham demanda atendida (prioridade 1)
	if(algumNa)
		tryAlocAlunosComp(dispTurma, alunosTurmaNA, mudarSala);

	// tentar alocar alunos que não tinham demanda atendida (prioridade -1)
	if(algumEquiv)
		tryAlocAlunosComp(dispTurma, alunosTurmaNAEquiv, mudarSala);

	// check se deve repor solução anterior
	checkPosRealoc(ofertaDisc, oldAlocOft, nrAlocCompInicio, "princ", relaxMin);
}

// tentar realocar alunos nas aulas práticas, quando estas são componente secundária
void ImproveMethods::tryRealocAlunosCompSec(OfertaDisciplina* const ofertaDisc, bool mudarSala, int priorAluno, double relaxMin)
{
	if(ofertaDisc->nrTiposAula() != 2)
	{
		HeuristicaNuno::warning("ImproveMethods::tryRealocAlunosCompSec", "Oferta disciplina não tem duas componentes!");
		return;
	}

	//if(ParametrosHeuristica::relacPraticas == ParametrosHeuristica::abrePraticas::UM_UM)
	//	return;
	
	HeuristicaNuno::logMsg("ImproveMethods::tryRealocAlunosCompSec", 2);

	if(ofertaDisc->nrAlunosIncompleto() == 0)
	{
		HeuristicaNuno::logMsg("sem alunos incompleto. return", 2);
		return;
	}

	// nr de alunos alocado no inicio
	int nrAlocCompInicio = ofertaDisc->nrAlunosCompleto();

	// guardar alocacao atual
	unordered_map<TurmaHeur*, pair<unordered_set<AlunoHeur*>, SalaHeur*>> oldAlocOft;
	ofertaDisc->getAlocacaoAtual(oldAlocOft);

	// verificar se há turmas praticas abertas sequer
	unordered_set<TurmaHeur*> turmasPrat;
	ofertaDisc->getTurmasTipo(false, turmasPrat);
	if(turmasPrat.size() <= 1)
	{
		HeuristicaNuno::logMsg("so uma turma prat. return.", 2);
		return;
	}
	resetNrDisp(turmasPrat);		//não deve ser necessário

	// remover os alunos de todas as turmas práticas
	unordered_set<AlunoHeur*> alunosInc;
	ofertaDisc->removerTodosAlunos(false, alunosInc);
	if(alunosInc.size() == 0)
	{
		HeuristicaNuno::logMsg("sem alunos removidos. return", 2);
		return;
	}

	// reduzir salas
	if(mudarSala)
		ImproveMethods::tryReduzirSalas(ofertaDisc);
	
	unordered_map<TurmaHeur*, int> dispTurmas;
	unordered_map<AlunoHeur*, unordered_set<TurmaHeur*>> alunosDispTurma;
	if(ParametrosHeuristica::relacPraticas == ParametrosHeuristica::abrePraticas::N_N)
	{
		alunosInc.clear();
		ofertaDisc->getAlunosIncompleto(alunosInc);
		if(alunosInc.size() == 0)
		{
			HeuristicaNuno::logMsg("sem alunos incompleto II. return", 2);
			return;
		}
		
		// tentar realocar alunos nas praticas
		if(checkAlunosDispTurmas(alunosInc, turmasPrat, dispTurmas, alunosDispTurma))
			tryAlocAlunosComp(dispTurmas, alunosDispTurma, mudarSala);
	}
	else
	{
		// fazer o mesmo mas para cada associação de turmas
		unordered_set<TurmaHeur*> turmasTeor;
		ofertaDisc->getTurmasTipo(true, turmasTeor);
		unordered_set<AlunoHeur*> alunosTurma;
		unordered_set<TurmaHeur*> turmasAssoc;
		for(auto itTeor = turmasTeor.begin(); itTeor != turmasTeor.end(); ++itTeor)
		{
			TurmaHeur* const turmaTeor = *itTeor;
			turmasAssoc.clear();
			ofertaDisc->getTurmasAssoc(turmaTeor, turmasAssoc);
			if(turmasAssoc.size() == 0)
			{
				//HeuristicaNuno::warning("ImproveMethods::tryRealocAlunosCompSec", "sem turmas praticas associadas");
				continue;
			}

			alunosTurma.clear();
			alunosDispTurma.clear();
			// get alunos turma
			turmaTeor->getAlunos(alunosTurma);
			if(alunosTurma.size() == 0)
			{
				HeuristicaNuno::logMsg("sem alunos na teorica. return", 2);
				continue;
			}
			// check disponibilidade e tentar realocar alunos nas praticas
			if(checkAlunosDispTurmas(alunosTurma, turmasAssoc, dispTurmas, alunosDispTurma))
				tryAlocAlunosComp(dispTurmas, alunosDispTurma, mudarSala);
		}
	}

	// check se deve repor solução anterior
	checkPosRealoc(ofertaDisc, oldAlocOft, nrAlocCompInicio, "compSec", relaxMin);
}

// verifica para cada turma quantos e quais alunos estão disponiveis
bool ImproveMethods::checkAlunosDispTurmas(unordered_set<AlunoHeur*> const &alunos, unordered_set<TurmaHeur*> const &turmas,
										unordered_map<TurmaHeur*, int> &dispTurma, unordered_map<AlunoHeur*, unordered_set<TurmaHeur*>> &alunosTurma)
{
	if(turmas.size() == 0)
		return false;

	bool teorico = (*(turmas.begin()))->tipoAula;
	OfertaDisciplina* const oferta = (*(turmas.begin()))->ofertaDisc;
	bool duasCompPrinc = (oferta->nrTiposAula() == 2) && teorico;
	bool algum = false;
	unordered_set<TurmaHeur*> emptySet;
	for(auto it = alunos.begin(); it != alunos.end(); ++it)
	{
		alunosTurma[*it] = emptySet;
	}
	for(auto it = turmas.begin(); it != turmas.end(); ++it)
	{
		// se nao tem disp turma, inicializar
		if(dispTurma.find(*it) == dispTurma.end())
			dispTurma[*it] = 0;
	}

	// obter turmas associadas
	unordered_map<TurmaHeur*, unordered_set<TurmaHeur*>> assocTurmas;
	unordered_set<TurmaHeur*> turmasCompInv;
	if(duasCompPrinc)
	{
		if(ParametrosHeuristica::relacPraticas != ParametrosHeuristica::abrePraticas::N_N)
			oferta->getAllTurmasAssoc(assocTurmas);
		else
			oferta->getTurmasTipo(!teorico, turmasCompInv);
	}

	for(auto itAluno = alunos.begin(); itAluno != alunos.end(); ++itAluno)
	{
		AlunoHeur* const aluno = *itAluno;
		if(oferta->temAlunoComp(aluno, teorico))
			continue;

		for(auto it = turmas.begin(); it != turmas.end(); ++it)
		{
			TurmaHeur* const turma = *it;

			if(!aluno->estaDisponivel(turma))
				continue;

			// se estivermos a alocar a componente principal, verificar se está disponivel para alguma das praticas
			if(duasCompPrinc)
			{
				auto itAssoc = assocTurmas.find(turma);
				if(ParametrosHeuristica::relacPraticas != ParametrosHeuristica::abrePraticas::N_N && 
					itAssoc != assocTurmas.end() && !aluno->estaDisponivelAlguma(itAssoc->second))
				{
					continue;
				}
				else if(ParametrosHeuristica::relacPraticas == ParametrosHeuristica::abrePraticas::N_N && 
					!aluno->estaDisponivelAlguma(turmasCompInv))
				{
					continue;
				}
			}

			// contabilizar
			int disp = dispTurma.at(turma);
			dispTurma[turma] = (disp+1);
			alunosTurma.at(aluno).insert(turma);

			algum = true;
		}
	}
	return algum;
}


// verifica o pos realocação. retorna true se fica, false se solucao é reposta
bool ImproveMethods::checkPosRealoc(OfertaDisciplina* const oferta, unordered_map<TurmaHeur*, pair<unordered_set<AlunoHeur*>, SalaHeur* > > &oldAlocOft,
							int nrAlocCompIni, char* metodo, double relaxMin)
{
	HeuristicaNuno::logMsg("check pos realoc", 2);
	bool retroceder = false;

	// acertar oferta
	oferta->removerAlunosIncompleto();

	// verificar se a solucao piorou
	int nrAlocCompFim = oferta->nrAlunosCompleto();
	int diff = nrAlocCompFim - nrAlocCompIni;

	stringstream ss;
	ss << "realoc " << metodo << ":";
	if(diff > 0)
	{
		ss << " +";
		HeuristicaNuno::logMsgInt(ss.str(), diff, 1);
	}
	else if(diff < 0)
	{
		HeuristicaNuno::logMsgInt(ss.str(), diff, 1);
		HeuristicaNuno::warning("ImproveMethods::tryRealocAlunosPrinc", "Nr de alocados no fim menor que no principio! Repor alocacao");
		retroceder = true;
	}

	// verificar se há turmas não carregadas ilegais
	if(!retroceder)
	{
		unordered_set<TurmaHeur*> turmas;
		oferta->getTurmas(turmas);
		for(auto it = turmas.begin(); it != turmas.end(); ++it)
		{
			if( (*it)->ehIlegal(relaxMin) && ((*it)->getNrAlunosFix() == 0) )
			{
				retroceder = true;
				break;
			}
		}
	}
	
	// se nada mudou voltar
	if(!retroceder && (diff == 0))
		retroceder = true;

	if(retroceder)
	{
		reporAlocacao(oferta, oldAlocOft);
		return true;
	}

	return false;
}

// [END REGION]

// tentar alocar alunos não atendidos
void ImproveMethods::tryAlocNaoAtendidos(SolucaoHeur* const solucao, OfertaDisciplina* const ofertaDisc, bool mudarSala, bool equiv,
										bool realoc, int priorAluno, double relaxMin)
{
	if((ofertaDisc->nrTiposAula() == 1) && realoc)
	{
		//HeuristicaNuno::logMsg("tryRealocAlunosPrinc", 1);
		tryRealocAlunosPrinc(solucao, ofertaDisc, mudarSala, equiv, priorAluno, relaxMin);
		//HeuristicaNuno::logMsg("out. tryRealocAlunosPrinc", 1);
	}
	else
	{
		//HeuristicaNuno::logMsg("tryAlocNaoAtendSemRealoc", 1);
		tryAlocNaoAtendSemRealoc(solucao, ofertaDisc, mudarSala, equiv, priorAluno, relaxMin);
		//HeuristicaNuno::logMsg("out. tryAlocNaoAtendSemRealoc", 1);
	}

	// segundo tipo de realocação
	if(realoc)
	{
		//HeuristicaNuno::logMsg("tryAlocNaoAtendidosRealocOld", 1);
		tryAlocNaoAtendidosRealocOld(solucao, ofertaDisc, mudarSala, equiv, priorAluno, relaxMin);
		//HeuristicaNuno::logMsg("out. tryAlocNaoAtendidosRealocOld", 1);
	}

	// por fim tentar reduzir salas para aumentar ocupação
	//HeuristicaNuno::logMsg("tryReduzirSalas", 1);
	tryReduzirSalas(ofertaDisc);
	//HeuristicaNuno::logMsg("out. tryReduzirSalas", 1);
}

void ImproveMethods::tryAlocNaoAtendSemRealoc(SolucaoHeur* const solucao, OfertaDisciplina* const ofertaDisc, bool mudarSala, 
											bool equiv, int priorAluno, double relaxMin)
{
	HeuristicaNuno::logMsg("ImproveMethods::tryAlocNaoAtendidosSemRealoc", 2);

	// verificar demanda não atendida
	unordered_set<AlunoDemanda *> demandas;
	solucao->getDemandasNaoAtendidas(ofertaDisc->getCampus(), ofertaDisc->getDisciplina(), 1, demandas, true, priorAluno);
	if(equiv)
		solucao->getDemandasNaoAtendidas(ofertaDisc->getCampus(), ofertaDisc->getDisciplina(), -1, demandas, true, priorAluno);
	if(demandas.size() == 0)
	{
		HeuristicaNuno::logMsg("sem demandas. return.", 2);
		return;
	}

	// get alunos correspondentes
	unordered_set<AlunoHeur*> alunosNA;
	solucao->getSetAlunosFromSetAlunoDemanda(demandas, alunosNA);
	// filtrar alunos que excedem creditos
	filtrarAlunosExcCreds(ofertaDisc->getNrCreds(), alunosNA);
	if(alunosNA.size() == 0)
		return;

	// Recupera turmas principais
	auto turmasPrincipais = ofertaDisc->getTurmasPrincipal();
	unordered_set<TurmaHeur*> turmasPrinc;
	for(auto it = turmasPrincipais.cbegin(); it != turmasPrincipais.cend(); ++it)
		turmasPrinc.insert(it->second);
	if(turmasPrinc.size() == 0)
	{
		HeuristicaNuno::logMsg("sem turmas princ. return.", 2);
		return;
	}

	// verificar disponibilidade para turmas principais
	unordered_map<TurmaHeur*, int> dispTurma;
	unordered_map<AlunoHeur*, unordered_set<TurmaHeur*>> alunosTurmaNA;	
	// try alocar na turma principal
	if(checkAlunosDispTurmas(alunosNA, turmasPrinc, dispTurma, alunosTurmaNA))
		tryAlocAlunosComp(dispTurma, alunosTurmaNA, mudarSala);

	// Se oferta tiver dois componentes alocar o segundo
	if(ofertaDisc->nrTiposAula() > 1)
	{
		//HeuristicaNuno::logMsg("tryAlocCompSecSemRealoc", 1);
		tryAlocCompSecSemRealoc(ofertaDisc, mudarSala);
		//HeuristicaNuno::logMsg("out. tryAlocCompSecSemRealoc", 1);
	}
}
void ImproveMethods::tryAlocNaoAtendSemRealoc(OfertaDisciplina* const ofertaDisc, unordered_set<AlunoHeur*> const &alunos, 
												bool mudarSala, double relaxMin)
{
	HeuristicaNuno::logMsg("ImproveMethods::tryAlocNaoAtendidosSemRealoc", 2);

	// verificar a disponibilidade para turmas principais
	auto turmasPrincipais = ofertaDisc->getTurmasPrincipal();
	unordered_set<TurmaHeur*> turmasPrinc;
	for(auto it = turmasPrincipais.cbegin(); it != turmasPrincipais.cend(); ++it)
		turmasPrinc.insert(it->second);
	if(turmasPrinc.size() == 0)
	{
		HeuristicaNuno::logMsg("sem turmas princ. return.", 2);
		return;
	}

	// filtrar alunos que excedem creditos
	unordered_set<AlunoHeur*> alunosNA = alunos;
	filtrarAlunosExcCreds(ofertaDisc->getNrCreds(), alunosNA);
	if(alunosNA.size() == 0)
		return;

	// verificar disponibilidade
	unordered_map<TurmaHeur*, int> dispTurma;
	unordered_map<AlunoHeur*, unordered_set<TurmaHeur*>> alunosTurmaNA;	
	// try alocar na turma principal
	if(checkAlunosDispTurmas(alunosNA, turmasPrinc, dispTurma, alunosTurmaNA))
		tryAlocAlunosComp(dispTurma, alunosTurmaNA, mudarSala);

	// Se oferta tiver dois componentes alocar o segundo
	if(ofertaDisc->nrTiposAula() > 1)
	{
		tryAlocCompSecSemRealoc(ofertaDisc, mudarSala);
	}
}

// alocar alunos incompletos sem realocar
void ImproveMethods::tryAlocCompSecSemRealoc(OfertaDisciplina* const ofertaDisc, bool mudarSala)
{
	if((ofertaDisc->nrTiposAula() != 2) || (ofertaDisc->nrAlunosIncompleto() == 0))
		return;

	// get alunos turma
	unordered_map<TurmaHeur*, unordered_set<AlunoHeur*>> turmasAlunos;
	ofertaDisc->getTurmasAlunos(turmasAlunos);

	unordered_set<AlunoHeur*> alunosInc;
	unordered_set<TurmaHeur*> turmasAssoc;
	unordered_map<TurmaHeur*, int> dispTurmas;
	unordered_map<AlunoHeur*, unordered_set<TurmaHeur*>> alunosDispTurma;
	for(auto itTurma = turmasAlunos.cbegin(); itTurma != turmasAlunos.cend(); ++itTurma)
	{
		TurmaHeur* const turma = itTurma->first;
		alunosInc.clear();
		
		// procurar alunos incompletos
		for(auto itAl = itTurma->second.cbegin(); itAl != itTurma->second.cend(); ++itAl)
		{
			if(ofertaDisc->ehIncompleto(*itAl))
				alunosInc.insert(*itAl);
		}
		if(alunosInc.size() == 0)
			continue;

		// turmas associadas
		turmasAssoc.clear();
		dispTurmas.clear();
		alunosDispTurma.clear();
		ofertaDisc->getTurmasAssoc(turma, turmasAssoc);
		if(turmasAssoc.size() == 0)
			continue;

		// tentar alocar
		bool check = checkAlunosDispTurmas(alunosInc, turmasAssoc, dispTurmas, alunosDispTurma);
		if(check)
		{
			//HeuristicaNuno::logMsg("tryAlocAlunosComp", 1);
			tryAlocAlunosComp(dispTurmas, alunosDispTurma, mudarSala);
			//HeuristicaNuno::logMsg("out. tryAlocAlunosComp", 1);
		}
	}

	// remover incompletos
	ofertaDisc->removerAlunosIncompleto();
}

// tentar realocar alunos nas turmas dando prioridade às que têm menos disponibilidade
void ImproveMethods::tryAlocAlunosComp(unordered_map<TurmaHeur*, int> &dispTurma, 
									unordered_map<AlunoHeur*, unordered_set<TurmaHeur*>> &alunosTurma, 
									bool mudarSala)
{
	unordered_set<TurmaHeur*> turmasLotadas;
	map<int, unordered_set<AlunoHeur*>> ordAlunos;
	do
	{
		if(alunosTurma.size() == 0)
			break;
		ordAlunos.clear();
		getAlunosOrd(alunosTurma, ordAlunos);

		// escolher aluno
		auto itFst = (*ordAlunos.begin()).second.begin();
		AlunoHeur* const aluno = (*itFst);

		TurmaHeur* turma = nullptr;
		//HeuristicaNuno::logMsg("escolheTurma", 1);
		turma = escolheTurma(aluno, alunosTurma, dispTurma, turmasLotadas, mudarSala);
		//HeuristicaNuno::logMsg("out. escolheTurma", 1);
		if(turmasLotadas.size() > 0)
		{
			//HeuristicaNuno::logMsg("updateDisponibilidade", 1);
			updateDisponibilidade(turmasLotadas, dispTurma, alunosTurma);
			turmasLotadas.clear();
			//HeuristicaNuno::logMsg("out. updateDisponibilidade", 1);
		}
		if(turma == nullptr)
			continue;

		if(!turma->ofertaDisc->temAlunoComp(aluno, turma->tipoAula))
			turma->ofertaDisc->addAlunoTurma(aluno, turma, "ImproveMethods::tryAlocAlunosComp");
	}
	while(alunosTurma.size() > 0);
}

// prioridade a turmas ilegais
TurmaHeur* ImproveMethods::escolheTurma(AlunoHeur* const aluno, unordered_map<AlunoHeur*, unordered_set<TurmaHeur*>> &alunosTurma, 
							unordered_map<TurmaHeur*, int> &dispTurma, 
							unordered_set<TurmaHeur*> &turmasLotadas, bool mudarSala)
{
	auto itAluno = alunosTurma.find(aluno);
	if(itAluno == alunosTurma.end())
		return nullptr;

	//HeuristicaNuno::logMsg("juntar turmas, atualizar disp", 1);
	set<TurmaHeur*, compTurmaRealoc> turmasOrd;
	for(auto it = itAluno->second.begin(); it != itAluno->second.end(); ++it)
	{
		auto itDispT = dispTurma.find(*it);
		if(itDispT != dispTurma.end())
		{
			int disp = itDispT->second;
			itDispT->second = (disp - 1);
			if((disp - 1) < 0)
				HeuristicaNuno::warning("ImproveMethods::escolheTurma", "disp turma negativo!!");

			(*it)->setNrDisp(disp-1);
		}
		else
		{
			//HeuristicaNuno::warning("ImproveMethods::escolheTurma", "disp turma nao encontrado!!");
			continue;
		}

		// verificar se tem capacidade
		if((*it)->getCapacidadeRestante() == 0 && !mudarSala)
		{
			turmasLotadas.insert(*it);
			continue;
		}

		turmasOrd.insert(*it);
	}

	// comparador dá prioridade a turmas ilegais
	TurmaHeur* turmaEscolh = nullptr;
	for(auto it = turmasOrd.begin(); it != turmasOrd.end(); ++it)
	{
		if((*it)->getCapacidadeRestante() > 0)
		{
			turmaEscolh = *it;
			break;
		}
		else if(mudarSala)
		{
			bool mudou = tryMudarSala(*it, true);
			if(mudou)
			{
				turmaEscolh = *it;
				break;
			}
			else
				turmasLotadas.insert(*it);
			
		}
		else
			turmasLotadas.insert(*it);
	}

	alunosTurma.erase(itAluno);

	return turmaEscolh;
}

// update disponibilidade
void ImproveMethods::updateDisponibilidade(unordered_set<TurmaHeur*> const &turmasLotadas, unordered_map<TurmaHeur*, int> &dispTurma,
								         unordered_map<AlunoHeur*, unordered_set<TurmaHeur*>> &alunosTurma)
{
	for(auto it = turmasLotadas.cbegin(); it != turmasLotadas.cend(); ++it)
	{
		dispTurma.erase(*it);
		for(auto itAluno = alunosTurma.begin(); itAluno != alunosTurma.end();)
		{
			AlunoHeur* const aluno = itAluno->first;
			auto itAlTurma = itAluno->second.find(*it);
			if(itAlTurma == itAluno->second.end())
			{
				++itAluno;
				continue;
			}

			itAluno->second.erase(itAlTurma);
			// verificar se ficou sem turmas
			if(itAluno->second.size() == 0)
			{
				itAluno = alunosTurma.erase(itAluno);
				continue;
			}
			else
				++itAluno;
		}
	}
}

// [TRY NAO ATENDIDOS COM REALOCACAO POR ALUNO]

// tentar alocar nao atendidos olhando para os alunos e tentar realoca-los a outras disciplinas para alocar nesta
void ImproveMethods::tryAlocNaoAtendidosRealocOld(SolucaoHeur* const solucao, OfertaDisciplina* const ofertaDisc, bool mudarSala, bool equiv, 
												int priorAluno, bool relaxMin)
{
	HeuristicaNuno::logMsg("ImproveMethods::tryAlocNaoAtendidosRealocOld", 2);

	if(ofertaDisc == nullptr)
	{
		HeuristicaNuno::excepcao("ImproveMethods::tryAlocNaoAtendidosRealocOld", "Oferta nula!");
		return;
	}

	// verificar demanda não atendida
	unordered_set<AlunoDemanda *> demandas;
	solucao->getDemandasNaoAtendidas(ofertaDisc->getCampus(), ofertaDisc->getDisciplina(), 1, demandas, true, priorAluno);
	if(equiv)
		solucao->getDemandasNaoAtendidas(ofertaDisc->getCampus(), ofertaDisc->getDisciplina(), -1, demandas, true, priorAluno);
	if(demandas.size() == 0)
	{
		HeuristicaNuno::logMsg("sem demandas. return.", 2);
		return;
	}

	// get alunos correspondentes
	unordered_set<AlunoHeur*> alunos;
	solucao->getSetAlunosFromSetAlunoDemanda(demandas, alunos);
	// filtrar alunos que excedem os creditos
	ImproveMethods::filtrarAlunosExcCreds(ofertaDisc->getNrCreds(), alunos);
	if(alunos.size() == 0)
		return;

	unordered_set<OfertaDisciplina*> ofertasAluno;
	vector<TurmaHeur*> turmasOft;
	unordered_set<TurmaHeur*> turmasAluno;
	for(auto it = alunos.begin(); it != alunos.end(); ++it)
	{
		AlunoHeur* const aluno = (*it);
		turmasAluno.clear();
		ofertasAluno.clear();
		// nao fazer com formandos!
		if(aluno->ehFormando() && HeuristicaNuno::probData->parametros->violar_min_alunos_turmas_formandos)
			continue;

		aluno->getOfertasDisciplina(ofertasAluno);
		// ainda não foi alocado a nada
		if(ofertasAluno.size() == 0)
			continue;

		// verificar se aluno ja esta na disciplina
		if(ofertasAluno.find(ofertaDisc) != ofertasAluno.end())
		{
			HeuristicaNuno::warning("ImproveMethods::tryAlocNaoAtendidosRealocOld", "Aluno ja esta na oferta");
			continue;
		}
		// limpar as ofertas que o aluno tá fixado
		for(auto itOft = ofertasAluno.begin(); itOft != ofertasAluno.end(); )
		{
			if((*itOft)->ehFixado(aluno) || (*itOft)->keepAluno(aluno->getId()))
				itOft = ofertasAluno.erase(itOft);
			else
				++itOft;
		}

		// ainda não foi alocado a nada sem ser o que foi carregado
		if(ofertasAluno.size() == 0)
			continue;
		
		// verificar se alguma turma fica ilegal se remover o aluno. se for o caso nao deixar realocar nessa disciplina
		for(auto itOft = ofertasAluno.begin(); itOft != ofertasAluno.end(); )
		{
			OfertaDisciplina* const oferta = (*itOft);
			turmasOft.clear();
			oferta->getTurmasAluno(aluno, turmasOft);
			bool remove = false;
			for(auto itTurmas = turmasOft.begin(); itTurmas != turmasOft.end(); ++itTurmas)
			{
				TurmaHeur* const turma = *itTurmas;
				// remover para ver se fica ilegal! Se ficar não remover desta oferta
				oferta->removeAlunoTurma(aluno, turma);
				bool ilegal = turma->ehIlegal(relaxMin);
				oferta->addAlunoTurma(aluno, turma, "ImproveMethods::tryAlocNaoAtendidosRealocOld");
				if(ilegal)
				{
					remove = true;
					break;
				}
			}
			if(remove)
			{
				itOft = ofertasAluno.erase(itOft);
				continue;
			}
				
			// guardar turmas da oferta que o aluno estava alocado
			for(auto itTurmas = turmasOft.begin(); itTurmas != turmasOft.end(); ++itTurmas)
				turmasAluno.insert(*itTurmas);

			++itOft;
		}

		// se nao houver nada que possa ser realocado retornar.
		if(ofertasAluno.size() == 0)
			continue;
		
		// remover de todas as turmas de cada oferta. Ofertas a que o aluno está fixado já foram removidas
		for(auto itOft = ofertasAluno.begin(); itOft != ofertasAluno.end(); ++itOft)
			(*itOft)->removeAluno(aluno);

		// incluir a nova oferta que se quer atender
		ofertasAluno.insert(ofertaDisc);

		// tentar alocar em todas as ofertas!
		bool success = false;
		success = tryRealocTurmasRec(aluno, ofertasAluno, mudarSala);
		// report
		if(!success)
		{
			// restaurar as turmas a que estava alocado
			for(auto itTurmas = turmasAluno.begin(); itTurmas != turmasAluno.end(); ++itTurmas)
			{
				OfertaDisciplina* const oferta = (*itTurmas)->ofertaDisc;
				oferta->addAlunoTurma(aluno, *itTurmas, "ImproveMethods::tryAlocNaoAtendidosRealocOld");
			}
		}
	}
}

// algoritmo recursivo para testar varias combinações de turmas para ver se é possivel a realocação
bool ImproveMethods::tryRealocTurmasRec(AlunoHeur* const &aluno, unordered_set<OfertaDisciplina*> const &ofertasAluno, bool mudarSala)
{
	auto itOft = ofertasAluno.cbegin();
	auto itEnd = ofertasAluno.cend();
	unordered_set<TurmaHeur*> turmasComb;

	vector<pair<unordered_set<OfertaDisciplina*>::const_iterator, unordered_set<TurmaHeur*>>*> setCombs;
	auto par = new pair<unordered_set<OfertaDisciplina*>::const_iterator, unordered_set<TurmaHeur*>>(itOft, turmasComb);
	setCombs.push_back(par);

	bool success = false;
	while(setCombs.size() > 0)
	{
		pair<unordered_set<OfertaDisciplina*>::const_iterator, unordered_set<TurmaHeur*>>* const nextStep = setCombs.back();
		setCombs.pop_back();

		if(tryRealocTurmasRecIter(aluno, nextStep->second, nextStep->first, itEnd, setCombs, mudarSala))
		{
			success = true;
			break;
		}

		delete nextStep;
	}

	// deletar steps
	for(auto it = setCombs.begin(); it != setCombs.end();)
	{
		pair<unordered_set<OfertaDisciplina*>::const_iterator, unordered_set<TurmaHeur*>>* itPar = (*it);
		setCombs.erase(it++);
		delete itPar;
	}

	return success;
}

// iteração
bool ImproveMethods::tryRealocTurmasRecIter(AlunoHeur* const &aluno, unordered_set<TurmaHeur*> const &turmasComb,
										unordered_set<OfertaDisciplina*>::const_iterator const &itOft,
										unordered_set<OfertaDisciplina*>::const_iterator const &itEnd,
										vector<pair<unordered_set<OfertaDisciplina*>::const_iterator, unordered_set<TurmaHeur*>>*> &setCombs, 
										bool mudarSala)
{
	OfertaDisciplina* const currOft = *itOft;
	auto itNext = std::next(itOft);

	// verificar se o aluno viola o máximo de créditos
	if(aluno->excedeMaxCreds(currOft->getNrCreds()))
		return false;

	// get turmas principal
	unordered_set<TurmaHeur*> turmasPrinc;
	currOft->getTurmasTipo(currOft->temCompTeorica(), turmasPrinc);

	unordered_set<TurmaHeur*> newTurmas;
	unordered_set<TurmaHeur*> newTurmasSec;
	unordered_set<TurmaHeur*> turmasAssoc;
	for(auto it = turmasPrinc.begin(); it != turmasPrinc.end(); ++it)
	{
		TurmaHeur* const turmaPrinc = (*it);
		// verificar se a turma tem capacidade restante
		bool cheia = (turmaPrinc->getCapacidadeRestante() == 0);
		if(cheia && !mudarSala)
			continue;
		// verificar se o aluno está disponivel
		if(!aluno->estaDisponivel(turmaPrinc))
			continue;
		// verificar se é compativel com as turmas da combinação
		if(turmaPrinc->ehIncompativel(turmasComb, true))
			continue;
		// verificar se consegue mudar a sala (depois de tudo)
		if(mudarSala && cheia && !ImproveMethods::tryMudarSala(turmaPrinc, true))
			continue;

		// acrescentar turma principal à combinacao
		newTurmas = turmasComb;
		newTurmas.insert(turmaPrinc);

		// se tem dois tipos de aula temos que verificar as turmas associadas
		turmasAssoc.clear();
		if(currOft->nrTiposAula() == 2)
			currOft->getTurmasAssoc(turmaPrinc, turmasAssoc);

		// se tiver turmas associadas, so deixar entrar na combinação se estiver disponivel para uma
		// se nao houver turmas associadas deixar so alocar a principal
		if(turmasAssoc.size() > 0)
		{
			for(auto itSec = turmasAssoc.begin(); itSec != turmasAssoc.end(); ++itSec)
			{
				TurmaHeur* const turmaSec = (*itSec);
				bool secCheia = (turmaSec->getCapacidadeRestante() == 0);

				// verificar se a turma tem capacidade restante
				if(secCheia && !mudarSala)
					continue;
				// verificar se o aluno está disponivel
				if(!aluno->estaDisponivel(turmaSec))
					continue;
				// verificar se é compativel com as turmas da combinação
				if(turmaSec->ehIncompativel(newTurmas, true))
					continue;
				// verificar se consegue mudar a sala (depois de tudo)
				if(mudarSala && secCheia && !ImproveMethods::tryMudarSala(turmaSec, true))
					continue;

				newTurmasSec = newTurmas;
				newTurmasSec.insert(turmaSec);

				return ImproveMethods::checkImplementEnd(aluno, newTurmasSec, itNext, itEnd, setCombs);
			}
			// chegar aqui é pq nao achou nenhuma turma associada a q tenha sido alocado
			return false;
		}
		else
		{
			// deixar continuar se nao houver turmas associadas à principal
			return ImproveMethods::checkImplementEnd(aluno, newTurmas, itNext, itEnd, setCombs);
		}
	}

	return false;
}


// check implement if end
bool ImproveMethods::checkImplementEnd(AlunoHeur* const &aluno, unordered_set<TurmaHeur*> const &turmasComb,
										unordered_set<OfertaDisciplina*>::const_iterator const &itOft,
										unordered_set<OfertaDisciplina*>::const_iterator const &itEnd,
										vector<pair<unordered_set<OfertaDisciplina*>::const_iterator, unordered_set<TurmaHeur*>>*> &setCombs)
{
	// se tiver chegado ao fim adicionar aluno às turmas e retornar
	if(itOft == itEnd)
	{
		for(auto it = turmasComb.begin(); it != turmasComb.end(); ++it)
		{
			OfertaDisciplina* const oferta = (*it)->ofertaDisc;
			oferta->addAlunoTurma(aluno, *it, "ImproveMethods::tryRealocTurmasRec");
		}
		return true;
	}
	else
	{
		// add proximo passo
		auto par = new pair<unordered_set<OfertaDisciplina*>::const_iterator, unordered_set<TurmaHeur*>>(itOft, turmasComb);
		setCombs.push_back(par);
		return false;
	}

	return false;
}

// [ENDREGION]

// [UTIL]

// reset nrDisp
void ImproveMethods::resetNrDisp(unordered_set<TurmaHeur*> const &turmas)
{
	for(auto it = turmas.begin(); it != turmas.end(); ++it)
	{
		(*it)->setNrDisp(0);
	}
}
void ImproveMethods::resetDispTurma(unordered_map<TurmaHeur*, int> &dispTurma)
{
	for(auto it = dispTurma.begin(); it != dispTurma.end(); ++it)
	{
		dispTurma[it->first] = 0;
	}
}


// tentar mudar de sala para a menor sala maior que a actual, que esteja disponível. Todos os alunos têm que estar disponíveis
bool ImproveMethods::tryMudarSala(TurmaHeur* const turma, bool maior, bool igual)
{
	// verificar se turma já está no máximo de alunos possível da disciplina
	if(!turma->podeTerMaisAlunos())
		return false;

	// get salas associadas
	unordered_set<SalaHeur*> salasAssoc;
	turma->ofertaDisc->getSalasAssociadas(salasAssoc, turma->tipoAula);

	// search info
	int nrAlunos = turma->getNrAlunos();
	int minCap = 10000;
	int indDem = 1000000;
	bool found = false;
	SalaHeur* bestSala = nullptr;

	// get old aulas
	unordered_map<int, AulaHeur*> oldAulas;
	turma->getAulas(oldAulas);

	int capAtual = turma->getSala()->getCapacidade();
	for(auto itSalasAssoc = salasAssoc.cbegin(); itSalasAssoc != salasAssoc.cend(); ++itSalasAssoc)
	{
		SalaHeur* const newSala = (*itSalasAssoc);
		if(newSala->getId() == turma->getSala()->getId())
			continue;
		int newCap = newSala->getCapacidade();
		if(newCap < nrAlunos)
			continue;

		int newInd = newSala->getIndicDem();
		if(!igual && newCap == capAtual)
			continue;

		if(maior && (newCap < capAtual))
			continue;

		if(!maior && (newCap > capAtual))
			continue;

		if(!newSala->estaDisponivel(oldAulas))
			continue;

		// verificar se alunos estão disponiveis para esta realocação
		if(!turma->getSala()->mesmoLocal(newSala))
		{
			continue; // nao deixar trocar de unidade!!!

			/*unordered_map<int, AulaHeur*> newAulas;
			SalaHeur::getNovasAulas(oldAulas, newSala, newAulas);
			turma->setNewAulas(newAulas);
			bool disp = false;
			disp = alunosDispRealoc(turma);
			turma->setNewAulas(oldAulas);
			if(!disp)
				continue;*/
		}

		if(newCap < minCap || (newCap == minCap && newInd < indDem))
		{
			minCap = newCap;
			bestSala = newSala;
			indDem = newInd;
			found = true;
		}

		// minimum increment possivel
		if(maior && (newCap == capAtual + 1))
			break;
		if(!maior && (newCap == nrAlunos))
			break;
	}
	if(found)
	{
		turma->setSala(bestSala);
		int capDiff = bestSala->getCapacidade() - capAtual;
		if(maior)
			HeuristicaNuno::logMsgInt("nova sala maior. cap diff: ", capDiff, 2);
		else
			HeuristicaNuno::logMsgInt("nova sala menor. cap diff: ", capDiff, 2);
	}

	return found;
}

// tentar reduzir as salas da oferta de maneira a libertá-las para outras turmas
bool ImproveMethods::tryReduzirSalas(OfertaDisciplina* const oferta)
{
	// get turmas ordenadas por ordem crescente de ocupação
	set<TurmaHeur*, compTurmaOcup> turmasOrd;
	oferta->getTurmasOrd<compTurmaOcup>(turmasOrd);

	// tentar mudar para sala mais pequena
	bool mudou = false;
	for(auto it = turmasOrd.begin(); it != turmasOrd.end(); ++it)
		if(tryMudarSala(*it, false))
			mudou = true;

	return mudou;
}

// verificar se alunos estão disponiveis para o realoc
bool ImproveMethods::alunosDispRealoc(TurmaHeur* const turma)
{
	unordered_set<AlunoHeur*> alunos;
	turma->getAlunos(alunos);
	for(auto itAlunos = alunos.cbegin(); itAlunos != alunos.cend(); ++itAlunos)
	{
		if(!(*itAlunos)->estaDisponivelRealoc(turma))
		{
			return false;
		}
	}
	return true;
}

void ImproveMethods::getOrdTurmas(unordered_map<TurmaHeur*, int> &dispTurma, set<TurmaHeur*> &turmasOrd)
{
	for(auto it = dispTurma.begin(); it != dispTurma.end(); ++it)
	{
		TurmaHeur* const turma = it->first;
		turma->setNrDisp(it->second);
		turmasOrd.insert(turma);
	}
}

void ImproveMethods::getOrdTurmasInv(unordered_map<TurmaHeur*, int> &dispTurma, set<TurmaHeur*> &turmasOrd)
{
	for(auto it = dispTurma.begin(); it != dispTurma.end(); ++it)
	{
		TurmaHeur* const turma = it->first;
		turma->setNrDisp(-it->second);
		turmasOrd.insert(turma);
	}
}

// get mapa alunos ordenado por ordem crescente por nr turmas que ta disponivel
void ImproveMethods::getAlunosOrd(unordered_map<AlunoHeur*, unordered_set<TurmaHeur*>> const &alunosTurma, map<int, unordered_set<AlunoHeur*>> &ordAlunos)
{
	unordered_set<AlunoHeur*> emptySet;
	for(auto it = alunosTurma.begin(); it != alunosTurma.end(); ++it)
	{
		int nrTurmas = it->second.size();
		auto itNr = ordAlunos.find(nrTurmas);
		if(itNr == ordAlunos.end())
			itNr = ordAlunos.insert(make_pair<int, unordered_set<AlunoHeur*>>(nrTurmas, emptySet)).first;

		itNr->second.insert(it->first);
	}
}

// get turmasPorAluno
void ImproveMethods::getTurmasPorAluno(unordered_map<TurmaHeur*, unordered_set<AlunoHeur*>> const &alunosTurma, 
						unordered_map<AlunoHeur*, unordered_set<TurmaHeur*>> &turmasPorAluno)
{
	unordered_set<TurmaHeur*> emptySet;
	for(auto it = alunosTurma.begin(); it != alunosTurma.end(); ++it)
	{
		for(auto itAluno = it->second.begin(); itAluno != it->second.end(); ++itAluno)
		{
			auto itTurmasAluno = turmasPorAluno.find(*itAluno);
			if(itTurmasAluno == turmasPorAluno.end())
			{
				itTurmasAluno = turmasPorAluno.insert(make_pair<AlunoHeur*, unordered_set<TurmaHeur*>>(*itAluno, emptySet)).first;
			}

			itTurmasAluno->second.insert(it->first);
		}
	}
}

// repor alocacao da oferta
void ImproveMethods::reporAlocacao(OfertaDisciplina* const oferta, unordered_map<TurmaHeur*, pair<unordered_set<AlunoHeur*>, SalaHeur*>> const &oldAlocOft)
{
	unordered_set<AlunoHeur*> rem;
	oferta->removerTodosAlunos(rem, false);

	for(auto itTurma = oldAlocOft.begin(); itTurma != oldAlocOft.end(); ++itTurma)
	{
		TurmaHeur* const turma = itTurma->first;
		turma->setSala(itTurma->second.second);
		for(auto itAluno = itTurma->second.first.begin(); itAluno != itTurma->second.first.end(); ++itAluno)
		{
			AlunoHeur* const aluno = (*itAluno);
			bool ehFixado = turma->ehFixado(aluno->getId());
			bool temAluno = oferta->temAlunoComp(aluno, turma->tipoAula);

			if(temAluno)
				continue;

			// fixados deviam ter permanecido
			if(ehFixado)
				HeuristicaNuno::warning("ImproveMethods::reporAlocacao", "Aluno fixado nao ficou na turma !");	

			// repor o aluno na turma
			oferta->addAlunoTurma(aluno, itTurma->first, "ImproveMethods::reporAlocacao");
		}
	}
}

// init disp turmas
void ImproveMethods::initDispTurmas(unordered_set<TurmaHeur*> const &turmas, unordered_map<TurmaHeur*, int> &dispTurma)
{
	dispTurma.clear();
	for(auto it = turmas.begin(); it != turmas.end(); ++it)
		dispTurma[*it] = 0;
}


// Filtrar alunos que excedem creditos
void ImproveMethods::filtrarAlunosExcCreds(int nrCreds, unordered_set<AlunoHeur*> &alunos)
{
	for(auto it = alunos.begin(); it != alunos.end();)
	{
		if((*it)->excedeMaxCreds(nrCreds))
			alunos.erase(it++);
		else
			++it;
	}
}

// [ENDREGION]