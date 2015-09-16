#include "MIPAlocarAlunos.h"
#include "opt_lp.h"
#include "opt_row.h"
#include "AulaHeur.h"
#include "SalaHeur.h"
#include "AlunoHeur.h"
#include "TurmaHeur.h"
#include "OfertaDisciplina.h"
#include "UtilHeur.h"
#include "../Demanda.h"
#include "../AlunoDemanda.h"
#include "SolucaoHeur.h"
#include "DadosHeuristica.h"
#include "../CentroDados.h"
#include "../ProblemData.h"
#include "../ParametrosPlanejamento.h"
#include "ParametrosHeuristica.h"
#include "../ConjUnidades.h"
#include "SaveSolucao.h"
#include <ctime>
#include <math.h>
#include "MIPAlocarAlunoVar.h"


MIPAlocarAlunos::MIPAlocarAlunos(std::string nome, SolucaoHeur* const solucao, SaveSolucao* const &solucaoHeur, bool realocSalas, double minRecCred, 
								int priorAluno, bool alocarP2)
	: MIPAloc(0, nome, solucao), solucaoHeur_(solucaoHeur), realocSalas_(realocSalas), priorAluno_(priorAluno), alocarP2_(alocarP2), 
	  soTrocarSalas_(false), minRecCred_(minRecCred)
{
}

MIPAlocarAlunos::~MIPAlocarAlunos(void)
{
}

// [MAIN METHODS]

// alocar!
void MIPAlocarAlunos::alocar(void)
{
	HeuristicaNuno::logMsg("", 1);
	HeuristicaNuno::logMsg("MIP Realocar alunos.", 1);

	// remover os alunos das turmas (de uma certa prioridade se for o caso!)
	HeuristicaNuno::logMsg("Remover alunos das turmas... ", 1);
	solucao_->removerTodosAlunos_(ParametrosHeuristica::fecharTurmasCarregadas);

	// limpar associacoes turma da oferta disciplina
	if(ParametrosHeuristica::relacPraticas != ParametrosHeuristica::abrePraticas::N_N)
	{
		HeuristicaNuno::logMsg("Remover associacoes de turmas nao fixadas... ", 1);
		solucao_->limparTurmasAssocAllOfertasPreMIP();
	}

	if(!realocSalas_)
		HeuristicaNuno::logMsg("criar o modelo MIPAlocarAlunos...", 1);
	else
		HeuristicaNuno::logMsg("criar o modelo MIPAlocarAlunos com realocacao de salas...", 1);

	// construir modelo
	buildLP_();
	HeuristicaNuno::logMsg("modelo criado...", 1);
	if(lp_->getNumCols() == 0)
	{
		HeuristicaNuno::warning("MIPAlocarAlunos::alocar", "MIPAlocarAlunos tem 0 variaveis!");
		return;
	}

	// definir MIP Start
	definirMIPStart_();
	HeuristicaNuno::logMsg("MIP start carregado...", 1);

	// set os parâmetros da optimização
	setParametrosLP_();
	
	HeuristicaNuno::logMsg("Solve MIP...", 1);
	// solve lp Fase I
	if(!solveLP_())
	{
		HeuristicaNuno::warning("MIPAlocarAlunos::alocar", "Solução não encontrada!");
		exit(1);
	}

	// carregar solução
	carregarSolucao();

	// tentar reduzir salas
	solucao_->tryReduzirSalas_();

	// acertar solução
	//solucao_->acertarSolucao_();

	HeuristicaNuno::logMsg("MIP alocar alunos terminado.", 1);
}
// só trocar salas e fechar turmas caso seja necessario
void MIPAlocarAlunos::realocarSalas(void)
{
	soTrocarSalas_ = true;
	realocSalas_ = true;

	HeuristicaNuno::logMsg("", 1);
	HeuristicaNuno::logMsg("MIP só realocar salas!", 1);

	// construir modelo
	buildLP_();
	HeuristicaNuno::logMsg("modelo criado...", 1);
	if(lp_->getNumCols() == 0)
	{
		HeuristicaNuno::warning("MIPAlocarAlunos::realocarSalas", "MIPAlocarAlunos tem 0 variaveis!");
		return;
	}

	// definir MIP Start
	definirMIPStart_();
	HeuristicaNuno::logMsg("MIP start carregado...", 1);

	// set os parâmetros da optimização
	setParametrosLP_();

	// solve lp Fase I
	if(!solveLP_())
	{
		HeuristicaNuno::warning("MIPAlocarAlunos::alocar", "Solução não encontrada!");
		exit(1);
	}

	// carregar solução
	carregarSolucao();

	// tentar reduzir salas
	solucao_->tryReduzirSalas_();

	soTrocarSalas_ = false;
}

// set Parametros LP
void MIPAlocarAlunos::setParametrosLP_(void)
{
	// chamar parâmetros gerais!
	MIPAloc::setParametrosLP_();

	// algorithm for root node relaxation
	//lp_->setMIPStartAlg(METHOD::METHOD_PRIMAL);
	// testar alternativa
	lp_->setMIPStartAlg(METHOD_CONCURRENT);

	// tentar enfase em procurar novas soluções em vez de provar optimalidade
	lp_->setMIPEmphasis(1);
}
// Carregar solução
void MIPAlocarAlunos::carregarSolucao(void)
{
	// carregar solução
	HeuristicaNuno::logMsg("carregar solução ...", 1);
	solFinal_ = new double [nrVars_];
	if(!lp_->getX(solFinal_))
		HeuristicaNuno::excepcao("MIPAlocarAlunos::carregarSolucao", "Nao conseguiu retornar a solucao.");
	HeuristicaNuno::logMsg("valores carregados ...", 1);

	// carregar salas
	int nrMudancas = 0;
	for(auto itTurma = varsTurmaSala_.cbegin();  itTurma != varsTurmaSala_.cend(); ++itTurma)
	{
		// repor a turma na sala para não dar excepção
		//SalaHeur* const oldSala = itTurma->first->getSala();
		//oldSala->addTurma(itTurma->first);
		for(auto itSala = itTurma->second.begin(); itSala != itTurma->second.end(); ++itSala)
		{
			if(solFinal_[itSala->second] > 0.999)
			{
				if(itSala->first->getId() != itTurma->first->getSala()->getId())
				{
					nrMudancas++;
					itTurma->first->setSala(itSala->first);
				}
				break;
			}
		}
	}

	// colocar os alunos nas turmas
	for(auto itTurmas = varsAlunoPorTurma_.begin(); itTurmas != varsAlunoPorTurma_.end(); ++itTurmas)
	{
		TurmaHeur* const turma = itTurmas->first;
		OfertaDisciplina* const oferta = turma->ofertaDisc;
		for(auto itAluno = itTurmas->second.cbegin(); itAluno != itTurmas->second.cend(); ++itAluno)
		{
			AlunoHeur* const aluno = solucao_->alunosHeur.at(itAluno->first);
			if(solFinal_[itAluno->second] > 0.999)
			{
				int colNrAlunoOferta = varsAlunoOferta_.at(aluno->getId()).at(oferta);
				if(solFinal_[colNrAlunoOferta] < 0.999)
					HeuristicaNuno::excepcao("MIPAlocarAlunos::carregarSolucao", "Aluno alocado à turma sem ser alocado à oferta!");

				HeuristicaNuno::logMsg("remover demanda", 2);
				
				HeuristicaNuno::logMsg("addAlunoTurma", 2);

				// adicionar aluno à turma
				oferta->addAlunoTurma(aluno, turma, "MIPAlocarAlunos::carregarSolucao");
			}
		}
	}

	// fechar as turmas que devem ser fechadas
	int nrTurmasFech = 0;
	int nrTurmasLoadFech = 0;
	int nrAlunosLoadRealoc = 0;
	for(auto itTurmasAbrir = varsAbrirTurma_.begin(); itTurmasAbrir != varsAbrirTurma_.end(); ++itTurmasAbrir)
	{
		if(solFinal_[itTurmasAbrir->second] > 0.9999)
			continue; // turma aberta

		TurmaHeur* turma = itTurmasAbrir->first;
		//if(turma->getNrAlunos() > 0)
		//	HeuristicaNuno::excepcao("MIPAlocarAlunos::carregarSolucao", "Turma que deve ser fechada tem alunos!");

		nrTurmasFech++;
		if(turma->carregada)
		{
			nrAlunosLoadRealoc += turma->getNrAlunosFix();
			nrTurmasLoadFech++;
		}

		// fechar turma
		OfertaDisciplina* const oft = turma->ofertaDisc;
		oft->fecharTurma(turma, false);
	}

	// Estatísticas
	HeuristicaNuno::logMsgInt("Nr turmas fechadas: ", nrTurmasFech, 1);
	HeuristicaNuno::logMsgInt("Nr turmas carregadas fechadas: ", nrTurmasLoadFech, 1);
	HeuristicaNuno::logMsgInt("Nr alunos fixados realocados/removidos: ", nrAlunosLoadRealoc, 1);
	HeuristicaNuno::logMsgInt("Nr mudancas de sala: ", nrMudancas, 1);
}


// [ENDREGION]

// [CRIAR VARIAVEIS]

// criar variaveis do modelo
void MIPAlocarAlunos::criarVariaveis_(void)
{
	nrVars_ = 0;
	nrVarsAlunoOferta_ = 0;
	nrVarsAlunoTurma_ = 0;
	nrVarsAbrirTurma_ = 0;
	nrVarsTurmaSala_ = 0;

	unordered_set<TurmaHeur*> turmas;

	// criar variáveis para demandas não atendidas
	for(auto itCampus = solucao_->demandasNaoAtendidas_.cbegin(); itCampus != solucao_->demandasNaoAtendidas_.cend(); ++itCampus)
	{
		// encontrar ofertas disciplina com este campus
		auto itCampOd = solucao_->ofertasDisciplina_.find(itCampus->first);
		if(itCampOd == solucao_->ofertasDisciplina_.end())
			continue;

		Campus* const campus = itCampus->first;

		// colocar campus na estrutura varsAlunoOfertaCampusDisc_
		unordered_map<int, unordered_map<int, int>> mapCampus;
		std::pair<int, unordered_map<int, unordered_map<int, int>>> pairCamp (campus->getId(), mapCampus);
		auto itVarsCampus = varsAlunoOfertaCampusDisc_.insert(pairCamp).first;

		unordered_map<OfertaDisciplina*, int> mapOferta;
		for(auto itDisc = itCampus->second.begin(); itDisc != itCampus->second.end(); itDisc++)
		{
			Disciplina* const disciplina = itDisc->first;
			// encontrar ofertas disciplina com esta disciplina
			auto itDiscOd = itCampOd->second.find(disciplina);
			if(itDiscOd == itCampOd->second.end())
				continue;

			// colocar disciplina na estrutura varsAlunoOfertaCampusDisc_
			unordered_map<int, int> mapDisc;
			std::pair<int, unordered_map<int, int>> pairDisc (disciplina->getId(), mapDisc);
			auto itVarsDisc = itVarsCampus->second.insert(pairDisc).first;

			// Criar variaveis abrir turma
			OfertaDisciplina* const ofDisc = itDiscOd->second;
			turmas.clear();
			ofDisc->getTurmas(turmas);
			criarVariaveisAbrirTurma_(turmas);

			// Criar variaveis turma sala!
			if(ofDisc->temCompTeorica())
				criarVariaveisTurmaSala_(ofDisc, true);
			if(ofDisc->temCompPratica())
				criarVariaveisTurmaSala_(ofDisc, false);

			// só trocar salas
			if(soTrocarSalas_)
				continue;

			// se não for NxN criar variáveis de associação de turma
			if(ParametrosHeuristica::relacPraticas != ParametrosHeuristica::abrePraticas::N_N && 
				ofDisc->nrTiposAula() == 2)
			{
				criarVariaveisAssocTurmas_(ofDisc);
			}

			for(auto itPrior = itDisc->second.begin(); itPrior != itDisc->second.end(); ++itPrior)
			{
				int prioridade = itPrior->first;

				// verificar se vamos alocar P2
				if(!alocarP2_ && (std::abs(prioridade) > 1))
					continue;

				for(auto itAluno = itPrior->second.begin(); itAluno != itPrior->second.end(); ++itAluno)
				{
					AlunoDemanda* const demanda = *itAluno;
					if(prioridade > 0 && demanda->getExigeEquivalenciaForcada())
						continue;

					// get aluno
					int alunoId = demanda->getAlunoId();
					AlunoHeur* const aluno = solucao_->alunosHeur.at(alunoId);

					// verificar se excede o máximo de créditos
					if(aluno->excedeMaxCreds(ofDisc->getNrCreds()))
						continue;
					// verificar se a prioridade não pode ser atendida agora
					if((priorAluno_ > 0) && (aluno->getPriorAluno() > priorAluno_))
						continue;

					// verifica se já tem variavel
					if(jaTemVarAlunoOferta_(ofDisc, alunoId))
					{
						stringstream ss;
						ss << "Ja tem var aluno oferta! prior: " << prioridade;
						HeuristicaNuno::warning("MIPAlocarAlunos::criarVariaveis_", ss.str());
						continue;
					}

					// check se esta demanda, se for P2, não está associada a uma disciplina equivalente de P1
					if(std::abs(demanda->getPrioridade()) != std::abs(prioridade))
					{
						HeuristicaNuno::warning("MIPAlocarAlunos::criarVariaveis_", "Prioridade e demanda diferentes!");
						continue;
					}

					// adicionar aluno à estrutura varsAlunoOferta_
					if(varsAlunoOferta_.find(alunoId) == varsAlunoOferta_.end())
						varsAlunoOferta_[alunoId] = mapOferta;

					// criar variavel aluno oferta
					criarVariavelAlunoOferta_(campus->getId(), disciplina->getId(), ofDisc, aluno, prioridade);
					// criar variaveis aluno turma
					criarVariaveisAlunoTurma_(ofDisc, turmas, alunoId, demanda);
				}
			}
		}
	}

	HeuristicaNuno::logMsgInt("nr vars folga min: ", (int)varsViolaMin.size(), 1);

	// verificação
	double sum = 0;
	for(auto itVarsAlOft = varsAlunoOferta_.begin(); itVarsAlOft != varsAlunoOferta_.end(); ++itVarsAlOft)
	{
		sum += itVarsAlOft->second.size();
	}
	double media = 0;
	if(varsAlunoOferta_.size() > 0)
		media = double(sum / varsAlunoOferta_.size());
	HeuristicaNuno::logMsgDouble("Media de ofertas por aluno: ", media, 1);
}
// criar variavel binaria aluno oferta -> 1 se o aluno for alocado à oferta disciplina
void MIPAlocarAlunos::criarVariavelAlunoOferta_(int campusId, int disciplinaId, OfertaDisciplina* const oferta, AlunoHeur* aluno, int prioridade)
{
	int alunoId = aluno->getId();

	// coeficiente so considera demanda atendida ou não
	double coef = getCoefVarAlunoOft_(oferta, aluno, prioridade);

	// adicionar variavel ao modelo
	MIPAlocarAlunoVar var;
	var.setType(MIPAlocarAlunoVar::V_X_ALUNO_DISC);
	var.setAluno(aluno);
	var.setOfDisciplina(oferta);

	int colNr = addBinaryVarLP_(coef, var.toString().c_str());
	nrVarsAlunoOferta_++;
	
	varsAlunoOferta_[alunoId][oferta] = colNr;
	varsAlunoOfertaCampusDisc_[campusId][disciplinaId][alunoId] = colNr;

	auto itOft = varsOfertaAluno_.find(oferta);
	if(itOft == varsOfertaAluno_.end())
	{
		unordered_map<int, int> emptyMap;
		itOft = varsOfertaAluno_.insert(make_pair(oferta, emptyMap)).first;
	}
	if(!itOft->second.insert(make_pair(alunoId, colNr)).second)
		HeuristicaNuno::excepcao("MIPAlocarAlunos::criarVariavelAlunoOferta_", "Var nao inserida em varsOfertaAluno_ !");
}
// criar variaveis binarias aluno turma -> 1 se o aluno for alocado à turma
void MIPAlocarAlunos::criarVariaveisAlunoTurma_(OfertaDisciplina* const oferta, unordered_set<TurmaHeur*> &turmas, int alunoId, AlunoDemanda* demanda)
{
	for(auto itTurma = turmas.cbegin(); itTurma != turmas.cend(); itTurma++)
	{
		TurmaHeur* const turma = (*itTurma);

		// adicionar turma à estrutura
		auto itVarPorTurma = varsAlunoPorTurma_.find(turma);
		if(itVarPorTurma == varsAlunoPorTurma_.end())
		{
			unordered_map<int, int> varsAlunos;
			std::pair<TurmaHeur*, unordered_map<int, int>> par (turma, varsAlunos);
			itVarPorTurma = varsAlunoPorTurma_.insert(par).first;
		}

		// procurar AlunoHeur
		auto itAlunoSol = solucao_->alunosHeur.find(alunoId);
		if(itAlunoSol == solucao_->alunosHeur.end())
		{
			HeuristicaNuno::excepcao("MIPAlocarAlunos::criarVariaveisAlunoTurma_", "Estrutura AlunoHeur não encontrada");
		}

		AlunoHeur* const aluno = itAlunoSol->second;
		
		// check
		//if(aluno->temTurmas())
		//	HeuristicaNuno::excepcao("MIPAlocarAlunos::criarVariaveisAlunoTurma_", "Aluno ainda alocado a turmas!!");

		// se o aluno não esta disponível para esta turma não criar variavel. Se for fixado criar sempre!!
		if(!aluno->estaDisponivel(turma) && !turma->ehAlunoFixado(aluno->getId()))
			continue;

		MIPAlocarAlunoVar var;
		var.setType(MIPAlocarAlunoVar::V_Y_ALUNO_TURMA);
		var.setAluno(aluno);
		var.setTurma(turma);

		// adicionar variavel ao modelo. OLD: incentivo se turma for do calendario da demanda
		int colNr = addBinaryVarLP_(0.0, var.toString().c_str());
		nrVarsAlunoTurma_++;

		// adicionar a estruturas
		std::pair<int, int> parCol (alunoId, colNr);
		itVarPorTurma->second.insert(parCol);

		auto itVarPorAluno = varsAlunoTurma_.find(alunoId);
		if(itVarPorAluno == varsAlunoTurma_.end())
		{
			unordered_map<TurmaHeur*, int> turmasVars;
			std::pair<int, unordered_map<TurmaHeur*, int>> parAl (alunoId, turmasVars);
			itVarPorAluno = varsAlunoTurma_.insert(parAl).first;
		}
		std::pair<TurmaHeur*, int> parTurCol (turma, colNr);
		itVarPorAluno->second.insert(parTurCol);
	}
}
// criar variaveis binarias abrir turma -> 1 se a turma for de facto aberta
void MIPAlocarAlunos::criarVariaveisAbrirTurma_(unordered_set<TurmaHeur*> const &turmas)
{
	for(auto itTurma = turmas.cbegin(); itTurma != turmas.cend(); ++itTurma)
	{
		TurmaHeur* const turma = (*itTurma);
		double coef = getCoefVarAbrirTurma_(turma);

		MIPAlocarAlunoVar var;
		var.setType(MIPAlocarAlunoVar::V_AT_ABRIR_TURMA);
		var.setTurma(turma);

		int colNr = addBinaryVarLP_(coef, var.toString().c_str());

		nrVarsAbrirTurma_++;
		varsAbrirTurma_[turma] = colNr;

		// guardar colNrs de turmas carregadas
		if(turma->mustAbrirMIP())
		{
			colNrsTurmasAbrir.insert(colNr);

			// folga violação min alunos
			int ub = ParametrosHeuristica::getMinAlunos(turma->discEhLab()) - 1;
			int colNrMin = addIntVarLP_(ParametrosHeuristica::coefPenalViolaMinAlunos, 0, ub, "viola_min");
			varsViolaMin[turma] = colNrMin;
		}

		// remover salas das turmas. Nota: a outra sala fica lá registada ainda mas só como tag
		//SalaHeur* const sala = turma->getSala();
		//sala->removeTurma(turma);
	}
}
// criar variáveis binárias de associação entre turmas teóricas e práticas
// obs: só se for 1x1 ou 1xN
void MIPAlocarAlunos::criarVariaveisAssocTurmas_(OfertaDisciplina* const oferta)
{
	if(oferta->nrTiposAula() != 2)
		return;

	auto turmasTeoricas = oferta->getTurmasTipo(true);
	auto turmasPraticas = oferta->getTurmasTipo(false);

	if (oferta->getDisciplina()->getId()==14658)
		std::cout<<"\n14658";

	int it = 0;
	for(auto itTeor = turmasTeoricas.begin(); itTeor != turmasTeoricas.end(); ++itTeor)
	{
		unordered_map<TurmaHeur*, int> mapaVazio;
		varsAssocTurmasTP[itTeor->second] = mapaVazio;
		for(auto itPrat = turmasPraticas.begin(); itPrat != turmasPraticas.end(); ++itPrat)
		{
			unordered_map<TurmaHeur*, int> mapaVazioP;
			// primeira vez, inserir tambem associacao para a pratica
			if(it == 0)
				varsAssocTurmasTP[itPrat->second] = mapaVazioP;

			MIPAlocarAlunoVar var;
			var.setType(MIPAlocarAlunoVar::V_W_ASSOC_TURMA_PT);
			var.setTurmaP(itPrat->second);
			var.setTurmaT(itTeor->second);
			
			double lb=0.0;
			double ub=1.0;
			if (!oferta->assocAulaContValida(itTeor->second, itPrat->second))
			{
				std::cout<<"\n\nimpedir var " << var.toString();
				std::cout<<"\nTem assoc registradas na teor: " << oferta->temAssoc(itTeor->second, itPrat->second);
				std::cout<<"\nTem assoc registradas na prat: " << oferta->temAssoc(itPrat->second,itTeor->second);
				ub=0.0;
			}

			int colNr = addBinaryVarLP_(0.0, var.toString().c_str(), lb, ub);
			nrVarsAssoc_++;
			if(!varsAssocTurmasTP[itTeor->second].insert(make_pair(itPrat->second, colNr)).second)
				HeuristicaNuno::excepcao("MIPAlocarAlunos::criarVariaveisAssocTurmas_", "Var nao adicionada as varsAssocTurmasTP (T)");
			if(!varsAssocTurmasTP[itPrat->second].insert(make_pair(itTeor->second, colNr)).second)
				HeuristicaNuno::excepcao("MIPAlocarAlunos::criarVariaveisAssocTurmas_", "Var nao adicionada as varsAssocTurmasTP (P)");

			// verificar se já têm associação
			if(oferta->temAssoc(itTeor->second, itPrat->second))
				assocsFixadas.insert(colNr);
		}
		it++;
	}
}
// criar variaveis turma sala
void MIPAlocarAlunos::criarVariaveisTurmaSala_(OfertaDisciplina* const oferta, bool teorico)
{
	if(!oferta->temComp(teorico))
		return;

	unordered_set<TurmaHeur*> turmas;
	oferta->getTurmasTipo(teorico, turmas);

	if(turmas.size() == 0)
		return;

	int invalid = -1;

	// como sao ordenadas e é dado um coeficiente à sala com base na ordem, dá prioridade a salas mais pequenas.
	set<SalaHeur*> salasAssoc;
	oferta->getSalasAssociadas(salasAssoc, teorico);
	unordered_map<SalaHeur*, int> emptyMap;
	unordered_map<TurmaHeur*, int> emptyTurmasMap;
	
	for(auto it = turmas.begin(); it != turmas.end(); ++it)
	{
		TurmaHeur* const turma = (*it);

		// contar nr de salas para adicionar coeficiente (diminuir simetria)
		int salaNr = 0;

		// adicionar par a varsTurmaSala_
		auto itTSala = varsTurmaSala_.insert(make_pair(turma, emptyMap)).first;
		SalaHeur* const salaAtual = turma->getSala();

		// procurar sala da solucao carregada
		int salaIniId = invalid;
		auto itStart = solucaoHeur_->alocacoes.find(turma);
		if(itStart != solucaoHeur_->alocacoes.end())
		{
			auto itSalaHeur = solucao_->salasHeur.find(itStart->second.second);
			if(itSalaHeur == solucao_->salasHeur.end())
				continue;

			SalaHeur* const salaIni = itSalaHeur->second;
			if(!salaIni->estaDisponivelHorarios(turma))
				continue;

			salaIniId = salaIni->getId();
			auto itSTurma = varsSalaTurma_.find(salaIni);
			if(itSTurma == varsSalaTurma_.end())
				itSTurma = varsSalaTurma_.insert(make_pair(salaIni, emptyTurmasMap)).first;

			// criar variavel e guardar
			MIPAlocarAlunoVar var;
			var.setType(MIPAlocarAlunoVar::V_R_SALA_TURMA);
			var.setSala(salaIni);
			var.setTurma(turma);

			int colNr = addBinaryVarLP_(0.0, var.toString().c_str());

			nrVarsTurmaSala_++;
			itTSala->second[salaIni] = colNr;
			itSTurma->second[turma] = colNr;
		}

		if (turma->salaFixada())
		{
			if(salaIniId==invalid)
				HeuristicaNuno::warning("MIPAlocarAlunos::criarVariaveisTurmaSala_", "Turma com sala fixa sem var salaturma criada.");
			continue;
		}

		for(auto itSala = salasAssoc.cbegin(); itSala != salasAssoc.cend(); ++itSala)
		{
			SalaHeur* const sala = (*itSala);
			++salaNr;
			int capacidade = sala->getCapacidade();

			// se ja foi adicionado continuar
			if(sala->getId() == salaIniId)
				continue;

			// nao deixar criar variavel se nao tem capacidade para os alunos deixados na turma
			if(capacidade < turma->getNrAlunos())
				continue;

			// se nao deixar realocar, so criar para a sala atual
			if(!realocSalas_ && (sala->getId() != salaAtual->getId()))
				continue;

			// sala do mesmo conjunto de unidades (NOTA: mudança de sala proibida para unidades diferentes!)
			if(sala->getConjUnidades()->getUnidadeId() != salaAtual->getConjUnidades()->getUnidadeId())
				continue;

			// check se está disponivel nos horários
			if(!sala->estaDisponivelHorarios(turma))
				continue;

			auto itSTurma = varsSalaTurma_.find(sala);
			if(itSTurma == varsSalaTurma_.end())
				itSTurma = varsSalaTurma_.insert(make_pair(sala, emptyTurmasMap)).first;

			// criar variavel e guardar			
			MIPAlocarAlunoVar var;
			var.setType(MIPAlocarAlunoVar::V_R_SALA_TURMA);
			var.setSala(sala);
			var.setTurma(turma);

			int colNr = addBinaryVarLP_(0.0, var.toString().c_str());

			nrVarsTurmaSala_++;
			itTSala->second[sala] = colNr;
			itSTurma->second[turma] = colNr;
		}
	}
}

// obter coeficiente da var aluno oferta
double MIPAlocarAlunos::getCoefVarAlunoOft_(OfertaDisciplina* const oferta, AlunoHeur* const aluno, int prioridade)
{
	//double coef = UtilHeur::getValDemPrior(prioridade);

	double coef = ParametrosHeuristica::coefDemAtendP1;
	bool ehP1 = (std::abs(prioridade) == 1);
	if(!ehP1)
		coef = ParametrosHeuristica::coefDemAtendP2;

	// coeficiente proporcional à prioridade do aluno
	coef = coef / aluno->getPriorAluno();

	// equivalencia
	if(prioridade < 0)
		coef = coef * ParametrosHeuristica::coefEquivalenciaMIP;

	// dar algum incentivo se for formando (ou calouro)
	if(aluno->ehFormando() && HeuristicaNuno::probData->parametros->priorizarFormandos)
		coef = coef * ParametrosHeuristica::pesoExtraFormando;
	if(aluno->ehCalouro() && HeuristicaNuno::probData->parametros->priorizarCalouros)
		coef = coef * ParametrosHeuristica::pesoExtraCalouro;

	// se o aluno já vinha alocado a esta oferta
	if(oferta->ehFixado(aluno))
		coef = coef * ParametrosHeuristica::pesoExtraCarregado;

	return coef;
}
// obter coeficiente da var abrir turma
double MIPAlocarAlunos::getCoefVarAbrirTurma_(TurmaHeur* const turma)
{
	// coeficiente definido pela demanda já atendida + coeficiente fixo de abertura
	double coef = turma->getCoefMIPAbrir() + ParametrosHeuristica::coefAbrirTurmaMIP;
	
	return coef;
}
// verifica se o aluno já tem uma var para aquela oferta
bool MIPAlocarAlunos::jaTemVarAlunoOferta_(OfertaDisciplina* const oferta, int alunoId)
{
	auto itAluno = varsAlunoOferta_.find(alunoId);
	if(itAluno == varsAlunoOferta_.end())
		return false;

	auto itOferta = itAluno->second.find(oferta);

	return (itOferta != itAluno->second.end());
}

// [ENDREGION]


// [CRIAR RESTRIÇÕES]

// criar restrições do modelo
void MIPAlocarAlunos::criarRestricoes_(void)
{
	// só trocar turmas de sala
	if(soTrocarSalas_)
	{
		for(auto itTurmas = varsAbrirTurma_.cbegin(); itTurmas != varsAbrirTurma_.cend(); itTurmas++)
			criarRestricaoAssignTurmaSala_(itTurmas->first);

		return;
	}

	// restrições associadas aos alunos
	for(auto itAlunos = varsAlunoOferta_.cbegin(); itAlunos != varsAlunoOferta_.cend(); itAlunos++)
	{
		int alunoId = itAlunos->first;
		auto itAl = solucao_->alunosHeur.find(alunoId);
		if(itAl == solucao_->alunosHeur.end())
			HeuristicaNuno::excepcao("MIPAlocarAlunos::criarRestricoes_", "AlunoHeur não encontrado!");

		AlunoHeur* const aluno = itAl->second;

		// maximo uma disciplina de entre as equivalentes [Não funciona quando há equivalencias com demandas originais semelhantes!]
		criarRestricoesMaxUmEquiv_(aluno);

		// impedir alocação a turmas incompatíveis
		criarRestricoesTurmasIncompat_(alunoId);

		// máximo de créditos
		criarRestricoesMaxCredsAluno_(aluno);

		// co-requisitos
		if(HeuristicaNuno::probData->parametros->considerarCoRequisitos)
			criarRestricoesCoReqsAluno_(alunoId);
	}

	// restrições relativas à alocação dos alunos às ofertas
	unordered_set<TurmaHeur*> allTurmas;
	unordered_map<int, TurmaHeur*> turmasTeoricas;
	unordered_map<int, TurmaHeur*> turmasPraticas;
	for(auto itOft = varsOfertaAluno_.cbegin(); itOft != varsOfertaAluno_.cend(); ++itOft)
	{
		OfertaDisciplina* const oferta = itOft->first;
		// turmas das componentes
		allTurmas.clear();
		oferta->getTurmas(allTurmas);
		turmasTeoricas.clear();
		turmasPraticas.clear();
		if(oferta->temCompTeorica())
			turmasTeoricas = oferta->getTurmasTipo(true);
		if(oferta->temCompPratica())
			turmasPraticas = oferta->getTurmasTipo(false);

		// restrições na alocação do aluno à oferta
		for(auto itAluno = itOft->second.cbegin(); itAluno != itOft->second.cend(); ++itAluno)
		{
			// obrigar aluno a cursar o número certo de disciplinas
			criarRestricoesTurmasOferta_(itAluno->first, oferta, itAluno->second, turmasTeoricas, turmasPraticas);
			// respeitar relação 1xN ou 1x1 (caso seja imposta)
			if(oferta->nrTiposAula() == 2 && ParametrosHeuristica::relacPraticas != ParametrosHeuristica::abrePraticas::N_N)
				criarRestAssocTurmasAluno_(itAluno->first, oferta, itAluno->second, turmasTeoricas);
		}
	}

	// restricoes associadas às turmas
	for(auto itTurmas = varsAbrirTurma_.cbegin(); itTurmas != varsAbrirTurma_.cend(); ++itTurmas)
	{
		// para abrir tem que ter pelo menos uma sala
		criarRestricaoAssignTurmaSala_(itTurmas->first);

		// maximo de alunos depende da capacidade da sala
		criarRestricoesMaxMinAlunosTurma_(itTurmas->first);

		// se não é NxN obrigar turma a ser associada a outra turma
		if(ParametrosHeuristica::relacPraticas != ParametrosHeuristica::abrePraticas::N_N && 
			itTurmas->first->ofertaDisc->nrTiposAula() == 2)
		{
			criarRestAssocTurma_(itTurmas->first);
		}
	}

	// restrições de turmas incompativeis para salas
	for(auto itSala = varsSalaTurma_.cbegin(); itSala != varsSalaTurma_.cend(); ++itSala)
	{
		criarRestricoesTurmasIncompat_(itSala);
	}

	// restrição de mínimo de produtividade de crédito
	if(minRecCred_ > 0.0)
		criarRestricaoMinProdutCred_();

	// obriga certas turmas a ficarem abertas
	criarRestricaoMustAbrirTurmas_();

	// obriga a manter associações fixadas entre turmas teóricas e práticas
	criarRestricaoAssocsFix_();

	turmasIncompativeis_.clear();
	turmasCompativeis_.clear();
}

// criar restrições que obrigam o aluno a, quando é alocado a uma oferta disciplina, ter que ser alocado a uma turma para cada componente
void MIPAlocarAlunos::criarRestricoesTurmasOferta_(int alunoId, OfertaDisciplina* const oferta, int colNrAlunoOft, 
									unordered_map<int, TurmaHeur*> const &turmasTeoricas, unordered_map<int, TurmaHeur*> const &turmasPraticas)
{
	unordered_map<TurmaHeur*, int> varsAlunoTurma;
	auto it = varsAlunoTurma_.find(alunoId);
	if(it != varsAlunoTurma_.end())
		varsAlunoTurma = it->second;

	// componente teorica
	if(oferta->temCompTeorica())
	{
		criarRestricoesTurmasOfertaComp_(alunoId, oferta, colNrAlunoOft, turmasTeoricas, varsAlunoTurma);
	}

	// componente pratica
	if(oferta->temCompPratica())
	{
		criarRestricoesTurmasOfertaComp_(alunoId, oferta, colNrAlunoOft, turmasPraticas, varsAlunoTurma);
	}
}
// obriga o aluno a cursar pelo menos uma destas turmas se estiver alocado à disciplina
bool MIPAlocarAlunos::criarRestricoesTurmasOfertaComp_(int alunoId, OfertaDisciplina* const oferta, int colNrAlunoOft, 
										unordered_map<int, TurmaHeur*> const &turmas, unordered_map<TurmaHeur*, int> const &varsAlunoTurma)
{
	// criar row e inserir varaluno-oferta
	OPT_ROW row (OPT_ROW::ROWSENSE::EQUAL, 0);
	row.insert(colNrAlunoOft, -1);

	bool any = false;
	// o aluno tem que cursar pelo menos uma turma deste tipo
	for(auto itTurmas = turmas.begin(); itTurmas != turmas.end(); itTurmas++)
	{
		TurmaHeur* const turma = itTurmas->second;

		// verificar se aluno pode cursar esta turma
		auto itVarAluno = varsAlunoTurma.find(turma);
		if(itVarAluno == varsAlunoTurma.end())
			continue;

		// inserir var aluno-turma
		int colNr = itVarAluno->second;
		row.insert(colNr, 1.0);
		any = true;

		// se o aluno for fixado, obrigar a ficar na turma se ela se mantiver aberta
		if(turma->ehAlunoFixado(alunoId))
		{
			auto itVarAbrir = varsAbrirTurma_.find(turma);
			if(itVarAbrir == varsAbrirTurma_.end())
				HeuristicaNuno::excepcao("MIPAlocarAlunos::criarRestricoesTurmasOfertaComp_", "Var abrir turma nao encontrada");

			OPT_ROW rowFix (OPT_ROW::ROWSENSE::EQUAL, 0);
			rowFix.insert(colNr, 1.0);
			rowFix.insert(itVarAbrir->second, -1.0);
			addRow_(rowFix);
		}
	}

	// adicionar restrição
	addRow_(row);

	return any;
}


// criar as restrições que limitam a uma so o número de disciplinas alocadas a um aluno, de entre um grupo de disciplinas equivalentes
void MIPAlocarAlunos::criarRestricoesMaxUmEquiv_(AlunoHeur* const aluno)
{
	//HeuristicaNuno::logMsg("criarRestricoesMaxUmEquiv_...", 1);
	auto itAluno = varsAlunoOferta_.find(aluno->getId());
	if(itAluno == varsAlunoOferta_.end())
		return;

	// obter todas as demandas do aluno
	unordered_set<AlunoDemanda*> demandas;
	Aluno* const alunoObj = aluno->getAluno();
	// P1
	for(auto it = alunoObj->demandas.begin(); it != alunoObj->demandas.end(); ++it)
		demandas.insert(*it);
	// P2
	for(auto it = alunoObj->demandasP2.begin(); it != alunoObj->demandasP2.end(); ++it)
		demandas.insert(*it);

	for(auto itDem = demandas.cbegin(); itDem != demandas.cend(); ++itDem)
	{
		AlunoDemanda* const demanda = (*itDem);
		Campus* const campus = demanda->getCampus();

		// restrição max 1 equiv var original
		OPT_ROW row (OPT_ROW::ROWSENSE::LESS, 1.0);
		int colNr = getVarAlunoOferta_(campus->getId(), demanda->demanda->getDisciplinaId(), aluno->getId());
		if(colNr >= 0)
			row.insert(colNr, 1.0);

		// adicionar vars das demanda equivalentes
		auto itDiscEquiv = demanda->demanda->discIdSubstitutasPossiveis.begin();
		for(; itDiscEquiv != demanda->demanda->discIdSubstitutasPossiveis.end(); ++itDiscEquiv)
		{
			Disciplina* const discEquiv = itDiscEquiv->second;
			if(discEquiv->getId() < 0)
				continue;

			// pode tbm ser original ou associada a outra demanda
			if(!DadosHeuristica::checkEquivalencia(demanda, campus, discEquiv, aluno->getId()))
				continue;

			// adicionar variavel da demanda equivalente à restrição
			int colNrEquiv = getVarAlunoOferta_(campus->getId(), discEquiv->getId(), aluno->getId());
			if(colNrEquiv >= 0)
				row.insert(colNrEquiv, 1.0);
		}
		if(row.getnnz() > 1)
		{
			addRow_(row);
		}
	}
}
// restrição que limita o numero de creditos que um aluno pode cursar
void MIPAlocarAlunos::criarRestricoesMaxCredsAluno_(AlunoHeur* const aluno)
{
	// maximo de créditos
	int max = aluno->maxCredsAloc() - aluno->getNrCreditosAlocados();

	auto itVars = varsAlunoOferta_.find(aluno->getId());
	if(itVars != varsAlunoOferta_.end())
	{
		// rhs da restrição de máximo de disciplinas
		Aluno* const alunoObj = aluno->getAluno();
		int demsP1 = alunoObj->demandas.size();
		int demsP2 = alunoObj->demandasP2.size();
		int demP1Left = demsP1 - aluno->getNrDemAtendP1();
		int maxDiscRhs = demP1Left*demsP2;

		// limitar numero de creditos
		OPT_ROW rowCredsOft (OPT_ROW::ROWSENSE::LESS, max);
		// restringir maximo de disciplinas (se tiver demanda P2)
		OPT_ROW rowMaxDisc (OPT_ROW::ROWSENSE::LESS, maxDiscRhs);
		for(auto it = itVars->second.begin(); it != itVars->second.end(); ++it)
		{
			// max de créditos
			rowCredsOft.insert(it->second, it->first->getNrCreds());

			// max disciplinas
			if(demsP2 != 0)
			{
				AlunoDemanda* const demanda = aluno->getDemandaOriginal(it->first);
				if(demanda != nullptr)
				{
					if (demanda->getPrioridade() == 1)
						rowMaxDisc.insert(it->second, demsP2);
					else
						rowMaxDisc.insert(it->second, 1.0);
				}	
			}
		}
		addRow_(rowCredsOft);
		if(demsP2 != 0)
			addRow_(rowMaxDisc);
	}

	// limitar creds e numero de disciplinas com turmas
	auto itVarsTurma = varsAlunoTurma_.find(aluno->getId());
	if(itVarsTurma != varsAlunoTurma_.end())
	{
		OPT_ROW rowCredsTurmas (OPT_ROW::ROWSENSE::LESS, max);
		for(auto it = itVarsTurma->second.cbegin(); it != itVarsTurma->second.cend(); ++it)
			rowCredsTurmas.insert(it->second, it->first->getNrCreditos());

		addRow_(rowCredsTurmas);
	}
}
// restições de co-requisitos do aluno
void MIPAlocarAlunos::criarRestricoesCoReqsAluno_(int alunoId)
{
	auto itCoReqs = DadosHeuristica::coRequisitosPorAluno.find(alunoId);
	if(itCoReqs == DadosHeuristica::coRequisitosPorAluno.end())
		return;
	if(itCoReqs->second.size() == 0)
		return;

	// NOTA: se houver co-requisito, equivalência não é usada!
	for(auto it = itCoReqs->second.cbegin(); it != itCoReqs->second.cend(); ++it)
	{
		if(it->size() <= 1)
			continue;

		// analisar cada par
		bool podeAtender = true;
		for(auto itDem = it->cbegin(); itDem != it->cend(); ++itDem)
		{
			int colNr = getVarAlunoOferta_((*itDem)->getCampus()->getId(), (*itDem)->demanda->getDisciplinaId(), alunoId);
			if(colNr < 0)
			{
				podeAtender = false;
				break;
			}

			for(auto itDemNxt = next(itDem); itDemNxt != it->cend(); ++itDemNxt)
			{
				int colNrNxt = getVarAlunoOferta_((*itDemNxt)->getCampus()->getId(), (*itDemNxt)->demanda->getDisciplinaId(), alunoId);

				OPT_ROW row (OPT_ROW::ROWSENSE::EQUAL, 0.0);
				row.insert(colNr, 1.0);
				if(colNrNxt >= 0)
				{
					row.insert(colNrNxt, -1.0);
					addRow_(row);
				}
				else
				{
					podeAtender = false;
					break;
				}
			}
			if(!podeAtender)
				break;
		}

		// não pode atender o co-requisito
		if(!podeAtender)
		{
			OPT_ROW rowNo (OPT_ROW::ROWSENSE::EQUAL, 0.0);
			for(auto itDem = it->cbegin(); itDem != it->cend(); ++itDem)
			{
				int colNr = getVarAlunoOferta_((*itDem)->getCampus()->getId(), (*itDem)->demanda->getDisciplinaId(), alunoId);
				if(colNr > 0)
					rowNo.insert(colNr, 1.0);
			}
			if(rowNo.getnnz() > 0)
				addRow_(rowNo);
		}
	}
}

// criar as restrições que impedem que um aluno curse duas turmas que são incompatíveis
void MIPAlocarAlunos::criarRestricoesTurmasIncompat_(int alunoId)
{
	//HeuristicaNuno::logMsg("criarRestricoesTurmasIncompat_...", 1);
	auto itVarsAlunoT = varsAlunoTurma_.find(alunoId);
	if(itVarsAlunoT == varsAlunoTurma_.end())
		return;

	if(itVarsAlunoT->second.size() < 2)
		return;

	criarRestrTurmasIncompatHorarios_(itVarsAlunoT->second, true);
	return;
}
// criar restricoes assign turma a sala
void MIPAlocarAlunos::criarRestricaoAssignTurmaSala_(TurmaHeur* const turma)
{
	auto itVarsTurma = varsTurmaSala_.find(turma);
	if(itVarsTurma == varsTurmaSala_.end())
		HeuristicaNuno::excepcao("MIPAlocarAlunos::criarRestricaoAssignTurmaSala_", "Vars turma sala da turma nao encontradas");

	auto itVarAbrir = varsAbrirTurma_.find(turma);
	if(itVarAbrir == varsAbrirTurma_.end())
		HeuristicaNuno::excepcao("MIPAlocarAlunos::criarRestricaoAssignTurmaSala_", "Var de abrir a turma nao encontrada");

	// se turma abrir, tem que ser alocada a uma sala
	OPT_ROW row (OPT_ROW::ROWSENSE::EQUAL, 0.0);
	row.insert(itVarAbrir->second, -1.0);
	for(auto it = itVarsTurma->second.cbegin(); it != itVarsTurma->second.cend(); ++it)
	{
		row.insert(it->second, 1.0);
	}
	addRow_(row);
}
// criar restrições que determinam o maximo e minimo de alunos numa turma
void MIPAlocarAlunos::criarRestricoesMaxMinAlunosTurma_(TurmaHeur* const turma)
{
	//HeuristicaNuno::logMsg("criarRestricoesMaxMinAlunosTurma_...", 1);
	auto itAlunosT = varsAlunoPorTurma_.find(turma);
	if(itAlunosT == varsAlunoPorTurma_.end())
		return;

	auto itAbrir = varsAbrirTurma_.find(turma);
	if(itAbrir == varsAbrirTurma_.end())
	{
		HeuristicaNuno::warning("MIPAlocarAlunos::criarRestricoesMaxMinAlunosTurma_", "Var abrir turma nao encontrada");
		return;
	}

	// col abrir turma
	int colNrAbrirTurma = itAbrir->second;

	// vars turma sala
	auto itVarsSala = varsTurmaSala_.find(turma);
	if(itVarsSala == varsTurmaSala_.end())
		HeuristicaNuno::excepcao("MIPAlocarAlunos::criarRestricoesMaxMinAlunosTurma_", "Vars turma sala da turma nao encontradas");

	// restrição número mínimo alunos
	int nrAlunosTurma = turma->getNrAlunos();
	bool addMin = true;
	OPT_ROW rowMin (OPT_ROW::ROWSENSE::GREATER, 0);
	int minAlunos = ParametrosHeuristica::getMinAlunos(turma->discEhLab());
	int coefMin = minAlunos - nrAlunosTurma;

	// verificar se o mínimo já foi atingido
	if(turma->getNrFormados() > 0 && HeuristicaNuno::probData->parametros->violar_min_alunos_turmas_formandos)
		coefMin = 0;
	if(turma->getNrCoRequisito() > 0 && HeuristicaNuno::probData->parametros->considerarCoRequisitos)
		coefMin = 0;
	if(coefMin < 1)
		addMin = false;

	if(addMin)
		rowMin.insert(colNrAbrirTurma, -coefMin);

	// inserir folga no mínimo se estiver em modo improve solução e não poder fechar turmas carregadas
	if(addMin && turma->mustAbrirMIP())
	{
		auto it = varsViolaMin.find(turma);
		if(it == varsViolaMin.end())
			HeuristicaNuno::warning("MIPAlocarAlunos::criarRestricoesMaxMinAlunosTurma_", "Var viola min alunos nao encontrada");
		else
			rowMin.insert(it->second, coefMin);
	}

	// restrição capacidade maxima turma
	OPT_ROW rowMax (OPT_ROW::ROWSENSE::LESS, 0);

	// inserir vars sala. capacidade restante = cap da sala - alunos ja na turma
	for(auto it = itVarsSala->second.cbegin(); it != itVarsSala->second.cend(); ++it)
	{
		int maxAlunos = min(it->first->getCapacidade(), turma->ofertaDisc->getMaxAlunos(turma->tipoAula));
		int capRest = maxAlunos - nrAlunosTurma;
		if(capRest < 0)
			HeuristicaNuno::warning("MIPAlocarAlunos::criarRestricoesMaxMinAlunosTurma_", "Capacidade restante menor que zero");

		rowMax.insert(it->second, -capRest);
	}

	// inserir alunos
	for(auto itAl = itAlunosT->second.begin(); itAl != itAlunosT->second.end(); itAl++)
	{
		int colNrAluno = itAl->second;
		AlunoHeur* const aluno = solucao_->alunosHeur.at(itAl->first);

		// row máximo
		rowMax.insert(colNrAluno, 1.0);

		if(addMin)
		{
			double coefAluno = 1.0;
			// se o aluno for formando então garantimos que a turma pode ser aberta independemente do numero de alunos (se permitido)
			// o mesmo acontece se o aluno tiver co-requisito
			if((aluno->ehFormando() && HeuristicaNuno::probData->parametros->violar_min_alunos_turmas_formandos)
				|| (aluno->temCoRequisito(turma->ofertaDisc) && HeuristicaNuno::probData->parametros->considerarCoRequisitos))
			{
				coefAluno = minAlunos;
			}
			
			// row minimo
			rowMin.insert(colNrAluno, coefAluno);
		}

		// criar restrição varAlunoTurma <= varAbrirTurma
		OPT_ROW rowLim (OPT_ROW::ROWSENSE::LESS, 0);
		rowLim.insert(colNrAbrirTurma, -1.0);
		rowLim.insert(colNrAluno, 1.0);
		addRow_(rowLim);
	}

	// se tem que abrir não impor mínimo
	if(addMin)
		addRow_(rowMin);
		
	// máximo tem que haver sempre
	addRow_(rowMax);
}
// criar restrições de turmas incompativeis para salas
void MIPAlocarAlunos::criarRestricoesTurmasIncompat_(unordered_map<SalaHeur*, unordered_map<TurmaHeur*, int>>::const_iterator const &itSala)
{
	if(itSala->second.size() < 2)
		return;

	criarRestrTurmasIncompatHorarios_(itSala->second, false);
	return;
}
// criar restrição para não deixar fechar turmas carregadas
void MIPAlocarAlunos::criarRestricaoMustAbrirTurmas_(void)
{
	if(colNrsTurmasAbrir.size() == 0)
		return;

	OPT_ROW row (OPT_ROW::ROWSENSE::EQUAL, colNrsTurmasAbrir.size());
	for(auto it = colNrsTurmasAbrir.cbegin(); it != colNrsTurmasAbrir.cend(); ++it)
		row.insert(*it, 1.0);

	addRow_(row);
}
// restrições que obrigam a deixar associações de turmas fixadas
void MIPAlocarAlunos::criarRestricaoAssocsFix_(void)
{
	if(assocsFixadas.size() == 0)
		return;

	OPT_ROW row (OPT_ROW::ROWSENSE::EQUAL, assocsFixadas.size());
	for(auto it = assocsFixadas.cbegin(); it != assocsFixadas.cend(); ++it)
		row.insert(*it, 1.0);

	addRow_(row);
}

// criar restrições que impedem o aluno de ser alocado a um par de turmas teorica e pratica se não estiverem associadas
// obs: só se 1x1 ou 1xN
void MIPAlocarAlunos::criarRestAssocTurmasAluno_(int alunoId, OfertaDisciplina* const oferta, int colNrAlunoOft,
									unordered_map<int, TurmaHeur*> const &turmasTeoricas)
{
	if(oferta->nrTiposAula() != 2 || ParametrosHeuristica::relacPraticas == ParametrosHeuristica::abrePraticas::N_N)
		return;

	// encontrar variaveis alunoturma do aluno (caso tenha)
	auto itVarsAluno = varsAlunoTurma_.find(alunoId);
	if(itVarsAluno  == varsAlunoTurma_.end())
		return;

	for(auto itT = turmasTeoricas.cbegin(); itT != turmasTeoricas.cend(); ++itT)
	{
		// procurar vars associação com esta turma teórica
		auto itAssoc = varsAssocTurmasTP.find(itT->second);
		if(itAssoc == varsAssocTurmasTP.end())
			continue;

		// procurar var aluno turma
		auto itVarTeor = itVarsAluno->second.find(itT->second);
		if(itVarTeor == itVarsAluno->second.end())
			continue;

		// só alocar o aluno na teorica se ela for associada a uma pratica que possa cursar
		OPT_ROW rowAssoc (OPT_ROW::ROWSENSE::LESS, 0.0);
		rowAssoc.insert(itVarTeor->second, 1.0);	

		// praticas associadas
		for(auto itPratAssoc = itAssoc->second.begin(); itPratAssoc != itAssoc->second.end(); ++itPratAssoc)
		{
			auto itVarPrat = itVarsAluno->second.find(itPratAssoc->first);
			if(itVarPrat == itVarsAluno->second.end())
				continue;

			rowAssoc.insert(itPratAssoc->second, -1.0);

			OPT_ROW row (OPT_ROW::ROWSENSE::LESS, 1);
			row.insert(itVarTeor->second, 1.0);			// alocação à teorica
			row.insert(itVarPrat->second, 1.0);			// alocação à prática
			row.insert(itPratAssoc->second, -1.0);		// associação entre a turma teórica e prática
			addRow_(row);
		}

		addRow_(rowAssoc);
	}
}

// obrigar cada turma de ofertas disciplina que têm duas componentes a ter pelo menos uma associação
// obs: só se 1x1 ou 1xN
void MIPAlocarAlunos::criarRestAssocTurma_(TurmaHeur* const turma)
{
	if(turma->ofertaDisc->nrTiposAula() != 2 || ParametrosHeuristica::relacPraticas == ParametrosHeuristica::abrePraticas::N_N)
	{
		return;
	}

	// variável abrir turma
	auto itAbrir = varsAbrirTurma_.find(turma);
	if(itAbrir == varsAbrirTurma_.end())
	{
		HeuristicaNuno::warning("MIPAlocarAlunos::criarRestAssocTurma_", "Var abrir turma não encontrada");
		return;
	}

	// se for teorica pode ser associada a varias praticas, se prática só a uma teorica
	OPT_ROW row = NULL;
	
	// se 1x1 ou a turma é prática só pode ser associada a uma turma
	if(ParametrosHeuristica::relacPraticas == ParametrosHeuristica::abrePraticas::UM_UM || !turma->tipoAula)
	{
		row = OPT_ROW(OPT_ROW::ROWSENSE::EQUAL, 0);
	}
	// se for 1xN e teórica
	else
	{
		row = OPT_ROW(OPT_ROW::ROWSENSE::GREATER, 0);
	}
	row.insert(itAbrir->second, -1.0);

	// encontrar variaveis associacao
	auto itAssocTurma = varsAssocTurmasTP.find(turma);
	if(itAssocTurma != varsAssocTurmasTP.end() && itAssocTurma->second.size() > 0)
	{
		// impedir de ser associado se não abrir!
		OPT_ROW rowNaoAbrir = OPT_ROW(OPT_ROW::ROWSENSE::LESS, 0);				
		int nrAssoc = (int)itAssocTurma->second.size();
		rowNaoAbrir.insert(itAbrir->second, -nrAssoc);

		for(auto itAssoc = itAssocTurma->second.begin(); itAssoc != itAssocTurma->second.end(); ++itAssoc)
		{
			row.insert(itAssoc->second, 1.0);
			rowNaoAbrir.insert(itAssoc->second, 1.0);
		}

		addRow_(rowNaoAbrir);
		addRow_(row);
	}
	// não pode ser associado a nenhuma turma. não deixar abrir !
	else
	{
		row = OPT_ROW(OPT_ROW::ROWSENSE::EQUAL, 0);
		row.insert(itAbrir->second, 1.0);
		addRow_(row);
	}
	
}

// criar restrição que impõe um mínimo de produtividade por crédito
void MIPAlocarAlunos::criarRestricaoMinProdutCred_(void)
{
	if(minRecCred_ <= 0)
		return;

	OPT_ROW rowMin (OPT_ROW::ROWSENSE::GREATER, 0.0);
	double rhs = 0.0;

	// preencher créditos aluno (variaveis)
	for(auto itAluno = varsAlunoOferta_.cbegin(); itAluno != varsAlunoOferta_.cend(); ++itAluno)
	{
		for(auto itOft = itAluno->second.cbegin(); itOft != itAluno->second.cend(); ++itOft)
		{
			rowMin.insert(itOft->second, itOft->first->getNrCreds());
		}
	}

	// olhar para todas as turmas (tambem as fixadas)
	unordered_set<TurmaHeur*> turmasOft;
	for(auto itOft = solucao_->allOfertasDisc_.cbegin(); itOft != solucao_->allOfertasDisc_.cend(); ++itOft)
	{
		turmasOft.clear();
		(*itOft)->getTurmas(turmasOft);
		for(auto it = turmasOft.cbegin(); it != turmasOft.cend(); ++it)
		{
			TurmaHeur* const turma = (*it);
			double coef = turma->getNrCreditos() * minRecCred_;
			auto itVarAbrir = varsAbrirTurma_.find(turma);
			if(itVarAbrir != varsAbrirTurma_.end())
				rowMin.insert(itVarAbrir->second, -coef);
			else	// vai ficar aberta
				rhs += coef;

			// créditos de alunos já fixados
			rhs -= (turma->getNrCreditos()*turma->getNrAlunos());
		}
	}

	rowMin.setRhs(rhs); 

	if(rowMin.getnnz() > 0)
		addRow_(rowMin);
}

// obter a variavel oferta aluno a partir do id do campus, disciplina e do aluno. retorna -1 se não for encontrada
int MIPAlocarAlunos::getVarAlunoOferta_(int campusId, int discId, int alunoId)
{
	// encontrar campus
	auto itCampus = varsAlunoOfertaCampusDisc_.find(campusId);
	if(itCampus == varsAlunoOfertaCampusDisc_.end())
		return -1;

	// encontrar disciplina
	auto itDisc = itCampus->second.find(discId);
	if(itDisc == itCampus->second.end())
		return -1;

	// encontrar aluno
	auto itAluno = itDisc->second.find(alunoId);
	if(itAluno == itDisc->second.end())
		return -1;
	else
		return itAluno->second;
}
// obter as variáveis oferta aluno a partir da disciplina e do aluno.
unordered_set<int> MIPAlocarAlunos::getVarsAlunoOferta_(int discId, int alunoId)
{
	unordered_set<int> result;
	for(auto itCamp = varsAlunoOfertaCampusDisc_.cbegin(); itCamp != varsAlunoOfertaCampusDisc_.cend(); ++itCamp)
	{
		auto itDisc = itCamp->second.find(discId);
		if(itDisc == itCamp->second.end())
			continue;

		auto itAluno = itDisc->second.find(alunoId);
		if(itAluno == itDisc->second.end())
			continue;

		result.insert(itAluno->second);
	}
	return result;
}

// verifica se tem o mínimo de alunos
bool MIPAlocarAlunos::temMinimo(unordered_map<int, int> const &alunosTurma, const int min)
{
	if(alunosTurma.size() >= min)
		return true;

	// se não tiver formando retornar false
	if(!HeuristicaNuno::probData->parametros->violar_min_alunos_turmas_formandos)
		return false;

	// ver se tem formando
	for(auto it = alunosTurma.cbegin(); it != alunosTurma.cend(); ++it)
	{
		auto itAl = solucao_->alunosHeur.find(it->first);
		if(itAl == solucao_->alunosHeur.end())
			HeuristicaNuno::excepcao("MIPAlocarAlunos::temMinimo_", "Aluno heur nao encontrado!");

		if(itAl->second->ehFormando())
			return true;
	}
	return false;
}
// intersectar sets. retorna nr de formandos
int MIPAlocarAlunos::intersectAlunosTurma(unordered_map<int, int> const &alunosUm, unordered_map<int, int> const &alunosDois, 
										unordered_set<int> &inter, int &outUm, int &outDois)
{
	outUm = 0;
	outDois = 0;
	inter.clear();
	int nr = 0;
	for(auto it = alunosUm.cbegin(); it != alunosUm.cend(); ++it)
	{
		int alunoId = it->first;
		if(alunosDois.find(alunoId) == alunosDois.end())
			continue;

		auto itAl = solucao_->alunosHeur.find(alunoId);
		if(itAl == solucao_->alunosHeur.end())
			HeuristicaNuno::excepcao("MIPAlocarAlunos::intersectAlunosTurma", "Aluno heur nao encontrado!");

		if(itAl->second->ehFormando())
			++nr;

		inter.insert(alunoId);
	}

	// set outs
	outUm = (int)(alunosUm.size() - inter.size());
	outDois = (int)(alunosDois.size() - inter.size());

	return nr;
}


// [ENDREGION]


// pega na solução herdade da heurística e carrega-a para o MIP
void MIPAlocarAlunos::definirMIPStart_(void)
{
	const int nrCols = lp_->getNumCols();
	// inicializar estruturas
	int* indices_ = new int [nrCols]();
	for(int idx = 0; idx < nrCols; ++idx)
		indices_[idx] = idx;

	double* values_ = new double [nrCols]();
	for(int idx = 0; idx < nrCols; ++idx)
		values_[idx] = 0;

	for(auto itTurmas = solucaoHeur_->alocacoes.cbegin(); itTurmas != solucaoHeur_->alocacoes.cend(); ++itTurmas)
	{
		TurmaHeur* const turma = itTurmas->first;

		// parvoice, alunos já foram removidos (-_-)"
		//if(turma->ehIlegal())
		//	continue;

		// get vars aluno
		auto itAlunosTurma = varsAlunoPorTurma_.find(turma);
		if(itAlunosTurma == varsAlunoPorTurma_.end())
		{
			//HeuristicaNuno::warning("MIPAlocarAlunos::definirMIPStart_", "Não consegue encontrar vars aluno da turma.");
			continue;
		}

		// fixar varAbrirTurma
		auto itAbrirTurma = varsAbrirTurma_.find(turma);
		if(itAbrirTurma == varsAbrirTurma_.end())
		{
			HeuristicaNuno::warning("MIPAlocarAlunos::definirMIPStart_", "Variavel abrir turma nao encontrada");
			continue;
		}

		int colAbrirTurma = itAbrirTurma->second;
		values_[colAbrirTurma] = 1;

		// fixar vars aluno turma
		for(auto itAlunos = itTurmas->second.first.cbegin(); itAlunos != itTurmas->second.first.cend(); ++itAlunos)
		{
			int alunoId = *itAlunos;
			auto itAlunoHeur = solucao_->alunosHeur.find(alunoId);
			if(itAlunoHeur == solucao_->alunosHeur.end())
				HeuristicaNuno::excepcao("MIPAlocarAlunos::definirMIPStart_", "Aluno heur nao encontrado!");
			AlunoHeur* const aluno = itAlunoHeur->second;

			auto itVarAluno = itAlunosTurma->second.find(alunoId);
			if(itVarAluno == itAlunosTurma->second.end())
				continue;

			// fixar varAlunoTurma
			int colAlunTurma = itVarAluno->second;
			values_[colAlunTurma] = 1;

			// fixar varAlunoOferta
			auto itAlunoOft = varsAlunoOferta_.find(alunoId);
			if(itAlunoOft == varsAlunoOferta_.end())
				HeuristicaNuno::excepcao("MIPAlocarAlunos::definirMIPStart_", "Não consegue encontrar o conj de vars oferta do aluno!");

			auto itOft = itAlunoOft->second.find(turma->ofertaDisc);
			if(itOft == itAlunoOft->second.end())
				HeuristicaNuno::excepcao("MIPAlocarAlunos::definirMIPStart_", "Não consegue encontrar a var oferta do aluno!");

			int colNrOft = itOft->second;
			values_[colNrOft] = 1;
		}

		// fixar var turma sala
		auto itTurmaS = varsTurmaSala_.find(turma);
		if(itTurmaS == varsTurmaSala_.end())
			HeuristicaNuno::excepcao("MIPAlocarAlunos::definirMIPStart_", "Não consegue encontrar vars turma sala da turma!");
		
		// nao remover a sala, fica lá.
		auto itSala = solucao_->salasHeur.find(itTurmas->second.second);
		if(itSala == solucao_->salasHeur.end())
			HeuristicaNuno::excepcao("MIPAlocarAlunos::definirMIPStart_", "SalaHeur nao encontrada!");

		SalaHeur* const sala = itSala->second;
		auto itSalaT = itTurmaS->second.find(sala);
		if(itSalaT != itTurmaS->second.end())
		{
			int colTrmSala = itSalaT->second;
			values_[colTrmSala] = 1;
		}

		// adicionar associação de turmas
		if(turma->tipoAula && ParametrosHeuristica::relacPraticas != ParametrosHeuristica::abrePraticas::N_N)
			definirTurmasAssocMIPStart_(turma, values_);
	}

	// PARTIAL FIX
	addMIPStartParcial(indices_, values_, nrCols);

	delete indices_;
	delete values_;
}

// adicionar associação de turmas ao mip start
void MIPAlocarAlunos::definirTurmasAssocMIPStart_(TurmaHeur* const turma, double* &values_)
{
	if(soTrocarSalas_)
		return;

	// só associar pelo lado da teórica
	if(!turma->tipoAula || ParametrosHeuristica::relacPraticas == ParametrosHeuristica::abrePraticas::N_N)
		return;

	if(turma->ofertaDisc->nrTiposAula() != 2)
		return;

	// get turmas assoc da solução carregada
	unordered_set<TurmaHeur*> turmasAssoc;
	solucaoHeur_->getTurmasAssoc(turma, turmasAssoc);
	if(turmasAssoc.size() == 0)
		return;

	// get variaveis associacao da turma
	auto itVars = varsAssocTurmasTP.find(turma);
	if(itVars == varsAssocTurmasTP.end())
	{
		if(turmasAssoc.size() > 0)
			HeuristicaNuno::excepcao("MIPAlocarAlunos::definirTurmasAssocMIPStart_", "Variaveis associacao da turma nao encontradas!!");
		return;
	}

	// set associação
	for(auto it = turmasAssoc.begin(); it != turmasAssoc.end(); ++it)
	{
		auto itAssoc = itVars->second.find(*it);
		if(itAssoc != itVars->second.end())
		{
			values_[itAssoc->second] = 1.0;
		}
		else
			HeuristicaNuno::warning("MIPAlocarAlunos::definirTurmasAssocMIPStart_", "Var associacao com pratica nao encontrada!");
	}
}