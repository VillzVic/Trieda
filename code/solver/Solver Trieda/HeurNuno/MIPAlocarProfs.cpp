#include "MIPAlocarProfs.h"
#include "HeuristicaNuno.h"
#include "AulaHeur.h"
#include "TurmaHeur.h"
#include "OfertaDisciplina.h"
#include "SolucaoHeur.h"
#include "MIPAlocarProfVar.h"
#include "MIPAlocarProfCons.h"

#include "../ProblemData.h"
#include "../ParametrosPlanejamento.h"
#include "../CentroDados.h"
#include "../TipoContrato.h"
#include "../TipoTitulacao.h"
#include "../CentroDados.h"

MIPAlocarProfs::MIPAlocarProfs(std::string nome, SolucaoHeur* const solucao, bool profsIndiv)
	: MIPAloc(1, nome, solucao), nrVarsProfTurmaVirtualIndiv_(0), profsVirtuaisIndiv_(profsIndiv)
{
}


MIPAlocarProfs::~MIPAlocarProfs(void)
{
}




// ----------------------------  MAIN METHODS ----------------------------

// alocar professores
void MIPAlocarProfs::alocar(void)

{
	HeuristicaNuno::logMsg("", 1);
	HeuristicaNuno::logMsg("MIP Realocar Professores.", 1);

	// preparar dados se professor virtual for individualizado
	prepararDados();

	// construir modelo
	HeuristicaNuno::logMsg("criar o modelo MIPAlocarProfs...", 1);
	buildLP_();
	HeuristicaNuno::logMsg("modelo criado...", 1);
	if(lp_->getNumCols() == 0)
	{
		HeuristicaNuno::logMsg("MIPAlocarProfs tem 0 variaveis!", 1);
		return;
	}

	// definir MIP Start
	HeuristicaNuno::logMsg("Add MIP Start solucao atual", 1);
	definirMIPStart_();
	HeuristicaNuno::logMsg("MIP start carregado...", 1);

	// set os parâmetros da optimização
	setParametrosLP_();

	// solve lp
	if(solveLP_())
	{
		// load solução
		carregarSolucao();
	}
	else
		HeuristicaNuno::warning("MIPAlocarProfs::alocar", "Solucao nao encontrada! Turmas com professores não fixados ficam com virtual unico!");
}

// Preparar dados
void MIPAlocarProfs::prepararDados(void)
{
	// criar professores individualizados antes para saber o numero de turmas virtuais
	if(profsVirtuaisIndiv_)
		solucao_->criarCopiasPerfisProfsVirtuaisPorCurso();

	// guardar a solução actual para input do MIP (e setar todas as turmas para profs virtuais)
	turmasProfs_.clear();
	solucao_->getAllTurmasProf_(turmasProfs_, !profsVirtuaisIndiv_);

	// get turmas curso
	getTurmasCurso();
}
void MIPAlocarProfs::getTurmasCurso(void)
{
	unordered_set<TurmaHeur*> turmas;
	unordered_set<TurmaHeur*> turmasAluno;
	// verificar em que turmas estão os alunos dos cursos
	for(auto itCurso = solucao_->alunosPorCurso.cbegin(); itCurso != solucao_->alunosPorCurso.cend(); ++itCurso)
	{
		turmas.clear();
		for(auto itAluno = itCurso->second.cbegin(); itAluno != itCurso->second.cend(); ++itAluno)
		{
			turmasAluno.clear();
			(*itAluno)->getTurmas(turmasAluno);
			for(auto it = turmasAluno.begin(); it != turmasAluno.end(); ++it)
			{
				if(turmas.find(*it) == turmas.end())
					turmas.insert(*it);
			}
		}

		turmasPorCurso[itCurso->first] = turmas;
	}
}

// Carregar solução
void MIPAlocarProfs::carregarSolucao(void)
{
	HeuristicaNuno::logMsg("carregar solucao ...", 1);
	solFinal_ = new double [lp_->getNumCols()];
	if(!lp_->getX(solFinal_))
		HeuristicaNuno::excepcao("MIPAlocarProfs::carregarSolução", "Nao conseguiu retornar a solucao.");
	HeuristicaNuno::logMsg("valores carregados ...", 1);

	// colocar os profs nas turmas
	ProfessorHeur* const virtualUnico = solucao_->professoresHeur.at(ParametrosHeuristica::profVirtualId);
	int nrTurmasProfIndiv = 0;
	int nrTurmasVirtualUnico = 0;
	for(auto itTurma = varsTurmaProf.begin(); itTurma != varsTurmaProf.end(); ++itTurma)
	{
		TurmaHeur* const turma = itTurma->first;
		for(auto itProf = itTurma->second.begin(); itProf != itTurma->second.end(); ++itProf)
		{
			ProfessorHeur* const prof = itProf->first;
			if(solFinal_[itProf->second] > 0.9)
			{
				// setar primeiro para virtual unico para evitar conflito
				turma->setProfessor(virtualUnico);
				// seta prof
				turma->setProfessor(prof);
				if(prof->ehVirtualUnico())
					nrTurmasVirtualUnico++;
				else if(prof->ehVirtual())
					nrTurmasProfIndiv++;
			}
		}
	}

	HeuristicaNuno::logMsgInt("nr turmas virtual unico: ", nrTurmasVirtualUnico, 1);
	HeuristicaNuno::logMsgInt("nr turmas virtual indiv: ", nrTurmasProfIndiv, 1);

	// verificar nr professores virtuais individuais contratados
	int nrProfsVirtIndiv = 0;
	for(auto itProf = varsProfUsed.cbegin(); itProf != varsProfUsed.cend(); ++itProf)
	{
		auto itProfHeur = solucao_->professoresHeur.find(itProf->first);
		if(itProfHeur == solucao_->professoresHeur.end())
			HeuristicaNuno::excepcao("MIPAlocarProfs::carregarSolucao", "Professor nao encontrado!");

		if(itProfHeur->second->ehVirtualUnico() || !itProfHeur->second->ehVirtual())
			continue;

		if(solFinal_[itProf->second] > 0.9)
			nrProfsVirtIndiv++;
	}
	HeuristicaNuno::logMsgInt("nr profs virtuais indiv: ", nrProfsVirtIndiv, 1);

	// check CH Anterior
	checkChAnterior_();

	checkGapsProf_();
}

// ------------------------------------------------------------------------------------


// ---------------------------- CRIAR VARIÁVEIS ----------------------------

// criar variáveis do modelo
void MIPAlocarProfs::criarVariaveis_(void)
{
	varsProfTurma.clear();
	varsTurmaProf.clear();
	varsProfMinChSem.clear();
	varsProfDia.clear();

	// vars de assignment de professor à turma
	criarVariaveisProfTurma_();
	// obs : só chamar criarVarsMinChProfDia_ depois de criarVariaveisProfTurma_
	criarVarsMinChProfDia_();

	// vars de tempo de inicio e fim das aulas do professor em cada fase do dia
	criarVarsHiHfProfFaseDoDia_();

	lp_->updateLP();

	//HeuristicaNuno::logMsgInt("cols lp: ", lp_->getNumCols(), 1);
	//HeuristicaNuno::logMsgInt("nr vars: ", nrVars_, 1);
}

// criar variáveis que alocam um professor a uma turma
void MIPAlocarProfs::criarVariaveisProfTurma_(void)
{
	// Campus
	for(auto itCampus = solucao_->ofertasDisciplina_.begin(); itCampus != solucao_->ofertasDisciplina_.end(); ++itCampus)
	{
		//Disciplina
		for(auto itDisc = itCampus->second.begin(); itDisc != itCampus->second.end(); ++itDisc)
		{
			// OfertaDisciplina
			OfertaDisciplina* const oferta = itDisc->second;
			unordered_set<TurmaHeur*> turmas;
			oferta->getTurmas(turmas);
			unordered_set<ProfessorHeur*> profsAssoc;
			oferta->getProfessoresAssociados(profsAssoc);

			for(auto itTurma = turmas.begin(); itTurma != turmas.end(); itTurma++)
			{
				auto itProfTurma = turmasProfs_.find(*itTurma);
				if(itProfTurma == turmasProfs_.end())
					HeuristicaNuno::excepcao("MIPAloc::criarVariaveisProfTurma_", "Professor da turma nao encontrado!");

				// só alocar professor a turmas com virtual quando estamos a individualizar
				if(profsVirtuaisIndiv_ && !itProfTurma->second->ehVirtual())
					continue;

				criarVarsProfTurma_(*itTurma, profsAssoc);
			}
		}
	}

	HeuristicaNuno::logMsgInt("nr vars prof virtual indiv turma: ", nrVarsProfTurmaVirtualIndiv_, 1);
}
// criar variáveis do modelo
void MIPAlocarProfs::criarVarsProfTurma_(TurmaHeur* const turma, unordered_set<ProfessorHeur*> &profsAssoc)
{
	// vars turma prof
	unordered_map<ProfessorHeur*, int> mapProfs;
	pair<TurmaHeur*, unordered_map<ProfessorHeur*, int>> parTurma (turma, mapProfs);
	auto itVarsTurma = varsTurmaProf.insert(parTurma).first;
	unordered_map<int, AulaHeur*> aulas;
	turma->getAulas(aulas);

	// var to be added in lp
	if(!turma->profFixado())
	{
		// Var relativa ao Prof virtual único
		ProfessorHeur* const profVirtualUnico = solucao_->professoresHeur.at(ParametrosHeuristica::profVirtualId);
	
		MIPAlocarProfVar var;
		var.reset();
		var.setType( MIPAlocarProfVar::V_X_PROF_TURMA );
		var.setProfessor(profVirtualUnico);
		var.setTurma(turma);
	
		int colNrVirt = addBinaryVarLP_(getCoefObjVarProfTurma(profVirtualUnico), var.toString().c_str());
		itVarsTurma->second[profVirtualUnico] = colNrVirt;
	}

	// para cada professor associado
	for(auto itProf = profsAssoc.begin(); itProf != profsAssoc.end(); ++itProf)
	{
		// virtual unico
		if((*itProf)->ehVirtualUnico())
			continue;

		// se segunda fase, só associar virtual indiv
		if(!(*itProf)->ehVirtual() && profsVirtuaisIndiv_)
			continue;

		// Se a turma possuir professor pré-fixado
		if(turma->profFixado() && turma->getProfessor()!=(*itProf))
			continue;

		// disponibilidade
		if(!(*itProf)->estaDisponivel(aulas))
		{
			if(profsVirtuaisIndiv_)
				HeuristicaNuno::warning("MIPAloc::criarVarsProfTurma_", "Professor virtual indiv nao disponivel!");

			continue;
		}

		// prof virtual.
		if((*itProf)->ehVirtual())
			nrVarsProfTurmaVirtualIndiv_++;

		// var to be added in lp
		MIPAlocarProfVar var;
		var.reset();
		var.setType( MIPAlocarProfVar::V_X_PROF_TURMA );
		var.setProfessor(*itProf);
		var.setTurma(turma);

		int profId = (*itProf)->getId();
		double coef = getCoefObjVarProfTurma(*itProf);
		int colNr = addBinaryVarLP_(coef, var.toString().c_str());
		itVarsTurma->second[*itProf] = colNr;

		// adicionar a varsProfTurma
		auto itVarsProf = varsProfTurma.find(*itProf);
		if(itVarsProf == varsProfTurma.end())
		{
			unordered_map<TurmaHeur*, int> mapTurmas;
			pair<ProfessorHeur*, unordered_map<TurmaHeur*, int>> parProf (*itProf, mapTurmas);
			auto parIns = varsProfTurma.insert(parProf);
			if(!parIns.second)
				HeuristicaNuno::warning("MIPAlocarProfs::criarVarsTurma_", "insert varsProfTurma sem sucesso");
			itVarsProf = parIns.first;
		}
		itVarsProf->second[turma] = colNr;
	}
}
// criar variáveis que identificam a violação do mínimo de carga horária diária e semanal, e o uso do professor
void MIPAlocarProfs::criarVarsMinChProfDia_(void)
{
	for(auto itProfs = varsProfTurma.begin(); itProfs != varsProfTurma.end(); ++itProfs)
	{
		ProfessorHeur* const prof = itProfs->first;
		if(prof->ehVirtualUnico())
			continue;

		// varProfUse		
		MIPAlocarProfVar var;
		var.reset();
		var.setType( MIPAlocarProfVar::V_Y_PROF_USADO );
		var.setProfessor(prof);

		double coef = prof->ehVirtual() ? ParametrosHeuristica::coefHireProfVirtual : 0.0;
		int col = addBinaryVarLP_(coef, var.toString().c_str());
		varsProfUsed[prof->getId()] = col;

		// varProfMinChSem
		int minCh = prof->getProfessor()->getChMin();
		if(ParametrosHeuristica::minChForte == false)
		{
			MIPAlocarProfVar var;
			var.reset();
			var.setType( MIPAlocarProfVar::V_CH_PROF_MIN_CH );
			var.setProfessor(prof);

			int colNr = addLinVarLP_(ParametrosHeuristica::penalCredMinChSemana, /*minCh*/ 10000, (char*)var.toString().c_str());
			varsProfMinChSem[prof->getId()] = colNr;
		}

		// varProfChAnterior
		int chAnt = prof->getProfessor()->getChAnterior();
		double chDouble = double(prof->getProfessor()->getChAnterior()*ParametrosHeuristica::percChAnterior);
		int chMinAnt = (int)ceil(chDouble);
		if(ParametrosHeuristica::considerarChAnterior && (chAnt > 0))
		{
			MIPAlocarProfVar var;
			var.reset();
			var.setType( MIPAlocarProfVar::V_CHA_PROF_CH_ANT );
			var.setProfessor(prof);

			int colNr = addLinVarLP_(ParametrosHeuristica::penalCredChAnterior, /*chMinAnt*/ 10000, (char*)var.toString().c_str());
			varsProfChAnterior[prof->getId()] = colNr;
		}

		// varsProfDia
		unordered_map<int, int> mapProfDia;
		pair<int, unordered_map<int, int>> par (prof->getId(), mapProfDia);
		auto itPdia = varsProfDia.insert(par).first;

		for(int dia=2; dia<9; ++dia)
		{
			MIPAlocarProfVar var;
			var.reset();
			var.setType( MIPAlocarProfVar::V_Z_PROF_DIA );
			var.setProfessor(prof);
			var.setDia(dia);

			int colNr = addBinaryVarLP_(ParametrosHeuristica::coefProfDia, var.toString().c_str());
			itPdia->second[dia] = colNr;
		}
	}
}
// criar vars inteiras que indicam o primeiro e o último horário da fase do dia (M/T/N) usados pelo professor
void MIPAlocarProfs::criarVarsHiHfProfFaseDoDia_(void)
{
	if ( CentroDados::getProblemData()->parametros->proibirProfGapMTN !=
		 ParametrosPlanejamento::ConstraintLevel::Strong )
		return;
	
	// Prof -> Dia -> Fase Do Dia -> (lb de hip, ub de hfp)
	unordered_map< ProfessorHeur*, unordered_map<int, unordered_map<int, pair<int,int> > > > mapProfDiaMTN;

	for(auto itProfs = varsProfTurma.begin(); itProfs != varsProfTurma.end(); ++itProfs)
	{
		ProfessorHeur* const prof = itProfs->first;
		
		if ( prof->ehVirtualUnico() )
		{
			continue;
		}

		if ( prof->ehVirtual() && ! ParametrosHeuristica::proibirGapProfVirt )
		{
			continue;
		}

		for(auto itTurmas = itProfs->second.begin(); itTurmas != itProfs->second.end(); ++itTurmas)
		{
			TurmaHeur* const turma = itTurmas->first;
			
			unordered_map<int, AulaHeur*> aulas;
			turma->getAulas(aulas);

			auto itAula = aulas.begin();
			for ( ; itAula != aulas.end(); itAula++ )
			{
				DateTime dti, dtf;
				itAula->second->getPrimeiroHor(dti);
				itAula->second->getLastHor(dtf);
				int dtiMinutes = dti.getDateMinutes();
				int dtfMinutes = dtf.getDateMinutes();
				int faseDoDia = CentroDados::getFaseDoDia(dti);

				auto ifFinderFase = mapProfDiaMTN[prof][itAula->first].find(faseDoDia);
				if ( ifFinderFase != mapProfDiaMTN[prof][itAula->first].end() )
				{
					ifFinderFase->second = make_pair( min( ifFinderFase->second.first, dtiMinutes ), 
												  max( ifFinderFase->second.second, dtfMinutes ) );
				}
				else
				{
					mapProfDiaMTN[prof][itAula->first][faseDoDia] = make_pair( dtiMinutes, dtfMinutes );
				}
			}
		}
	}
	
	for(auto itProf = mapProfDiaMTN.begin(); itProf != mapProfDiaMTN.end(); ++itProf)
	{
		for(auto itDia = itProf->second.begin(); itDia != itProf->second.end(); ++itDia)
		{
			for(auto itFase = itDia->second.begin(); itFase != itDia->second.end(); ++itFase)
			{
				double coef = 0.0;
				int lbi = itFase->second.first;
				int ubf = itFase->second.second;
				int ubi = ubf;
				int lbf = lbi;

				// varProfHi
				MIPAlocarProfVar var;
				var.reset();
				var.setType( MIPAlocarProfVar::V_HIP_PROF_HOR_INIC );
				var.setProfessor(itProf->first);
				var.setDia(itDia->first);
				var.setFase(itFase->first);

				int colHi = addIntVarLP_(coef, lbi, ubi, var.toString().c_str());
				varsProfDiaFaseHiHf[itProf->first][itDia->first][itFase->first].first = colHi;
		
				// varProfHf
				var.reset();
				var.setType( MIPAlocarProfVar::V_HFP_PROF_HOR_FIN );
				var.setProfessor(itProf->first);
				var.setDia(itDia->first);
				var.setFase(itFase->first);

				int colHf = addIntVarLP_(coef, lbf, ubf, var.toString().c_str());
				varsProfDiaFaseHiHf[itProf->first][itDia->first][itFase->first].second = colHf;
			}
		}
	}
}



// get coeficiente var prof turma
double MIPAlocarProfs::getCoefObjVarProfTurma(ProfessorHeur* const professor)
{
	// virtual unico
	if(professor->ehVirtualUnico())
	{
		return ParametrosHeuristica::coefProfVirtualUnico;
	}
	// real (negativo pois custo é negativo)
	else if(!professor->ehVirtual())
	{
		return -professor->getValorCredito();
	}
	// virtual individual
	else
	{
		double coef = ParametrosHeuristica::coefProfVirtual;
		coef -= (professor->tipoTitulacao()->getTitulacao() * 10);
		coef -= (professor->tipoContrato()->getContrato() * 1);
		return coef;
	}
}
// get var prof dia
int MIPAlocarProfs::getVarProfDia(ProfessorHeur* const professor, int dia)
{
	auto itProf = varsProfDia.find(professor->getId());
	if(itProf == varsProfDia.end())
		return -1;

	auto itDia = itProf->second.find(dia);
	if(itDia == itProf->second.end())
		return -1;
	else
		return itDia->second;
}

// ------------------------------------------------------------------------------------


// ---------------------------- CRIAR RESTRIÇÕES ----------------------------

// criar restrições do modelo
void MIPAlocarProfs::criarRestricoes_(void)
{
	// restrições por turma
	for(auto itTurmas = varsTurmaProf.begin(); itTurmas != varsTurmaProf.end(); ++itTurmas)
	{
		// restrições assign prof
		criarRestProfAssignment_(itTurmas->first, itTurmas->second);
	}

	// restrições por professor
	int nrRestAtivar = 0;
	for(auto itProf = varsProfTurma.begin(); itProf != varsProfTurma.end(); ++itProf)
	{
		ProfessorHeur* const professor = itProf->first;
		if(professor->ehVirtualUnico())
			continue;

		// controlar se profesor é usado ou nao
		criarRestAtivarProf_(professor);
		if(professor->ehVirtual())
			nrRestAtivar++;

		// restrições turmas incompativeis
		criarRestProfIncompTurmas_(itProf->second);
		// restrições min/max ch semanais
		criarRestProfLimitesChSem_(professor, itProf->second);
		// máximo dias na semana
		criarRestProfMaxDiasSem_(professor);

		for(int dia = 2; dia<9; ++dia)
		{
			// activar a variável professor dia
			criarRestProfAtivProfDia_(professor, dia, itProf->second);
			// criar restrições mínimo ch por dia
			criarRestProfMinChDia_(professor, dia, itProf->second);
		}
	}
	HeuristicaNuno::logMsgInt("nr restricoes ativar prof virtual: ", nrRestAtivar, 1);

	// criar restrições do MEC
	for(auto itCurso = turmasPorCurso.cbegin(); itCurso != turmasPorCurso.cend(); ++itCurso)
	{
		// min mestres e doutores
		criarRestMinMestres_(itCurso->first, itCurso->second);
		criarRestMinDoutores_(itCurso->first, itCurso->second);
		// min integral e parcial
		criarRestMinIntegral_(itCurso->first, itCurso->second);
		criarRestMinParcial_(itCurso->first, itCurso->second);
	}

	// restrições de sequência de professores do mesmo curso com mesmo perfil
	if(profsVirtuaisIndiv_)
		criarRestsSeqProfsVirtuais_();

	// restrições para impedir gap em um mesmo dia/faseDoDia para o professor
	criarRestProfHiHf_();

	lp_->updateLP();
}

// criar restrições de ativação do professor virtual
void MIPAlocarProfs::criarRestAtivarProf_(ProfessorHeur* const professor)
{
	if(professor->ehVirtualUnico())
		return;

	auto itTurmas = varsProfTurma.find(professor);
	if(itTurmas == varsProfTurma.end())
		return;

	// criar restrição
	MIPAlocarProfCons cons;
	cons.reset();
	cons.setType(MIPAlocarProfCons::C_PROF_USADO);
	cons.setProfessor(professor);

	OPT_ROW row (OPT_ROW::ROWSENSE::LESS, 0.0, (char*) cons.toString().c_str());
	auto itHire = varsProfUsed.find(professor->getId());
	if(itHire == varsProfUsed.end())
		HeuristicaNuno::excepcao("MIPAlocarProfs::criarRestAtivarProfVirtual_", "variavel de ativacao de professor virtual nao encontrada");

	// rhs
	int rhs = -(itTurmas->second.size());
	row.insert(itHire->second, rhs);
	// lhs
	for(auto it = itTurmas->second.begin(); it != itTurmas->second.end(); ++it)
		row.insert(it->second, 1.0);

	addRow_(row);
}
// criar restrições de assignment de professor à turma
void MIPAlocarProfs::criarRestProfAssignment_(TurmaHeur* const turma, unordered_map<ProfessorHeur*, int> &varsProf)
{
	MIPAlocarProfCons cons;
	cons.reset();
	cons.setType(MIPAlocarProfCons::C_TURMA_PROF_ASSIGN);
	cons.setTurma(turma);
	
	OPT_ROW row (OPT_ROW::ROWSENSE::EQUAL, 1.0, (char*) cons.toString().c_str());
	for(auto itProf = varsProf.begin(); itProf != varsProf.end(); ++itProf)
	{
		row.insert(itProf->second, 1.0);
	}

	addRow_(row);
}
// criar restrições de incompatibilidade de turmas
void MIPAlocarProfs::criarRestProfIncompTurmas_(unordered_map<TurmaHeur*, int> &varsProf)
{
	if(varsProf.size() < 2)
		return;

	criarRestrTurmasIncompatHorarios_(varsProf, true, HeuristicaNuno::probData->parametros->considerarDescansoMinProf);
	return;
}
// criar restrições que activam as variáveis prof dia
void MIPAlocarProfs::criarRestProfAtivProfDia_(ProfessorHeur* const professor, int dia, unordered_map<TurmaHeur*, int> &varsProf)
{
	if(professor->ehVirtualUnico())
		return;

	MIPAlocarProfCons cons;
	cons.reset();
	cons.setType(MIPAlocarProfCons::C_PROF_DIA);
	cons.setProfessor(professor);
	cons.setDia(dia);

	int nr = 0;
	OPT_ROW row (OPT_ROW::ROWSENSE::LESS, 0.0, (char*) cons.toString().c_str());
	for(auto itTurma = varsProf.begin(); itTurma != varsProf.end(); ++itTurma)
	{
		AulaHeur* aula = nullptr;
		if(itTurma->first->getAulaDia(dia, aula))
		{
			row.insert(itTurma->second, 1.0);
			nr++;
		}
	}

	auto itProf = varsProfDia.find(professor->getId());
	if(itProf == varsProfDia.end())
		HeuristicaNuno::excepcao("MIPAloc::criarRestProfAtivProfDia_", "Vars dia do professor nao encontradas");
	auto itDia = itProf->second.find(dia);
	if(itDia == itProf->second.end())
	{
		HeuristicaNuno::logMsgInt("dia: ", dia, 1);
		HeuristicaNuno::excepcao("MIPAloc::criarRestProfAtivProfDia_", "Var dia do professor nao encontrada");
	}
	row.insert(itDia->second, -nr);

	addRow_(row);
}
// criar restrição máximo de dias por semana
void MIPAlocarProfs::criarRestProfMaxDiasSem_(ProfessorHeur* const professor)
{
	if(professor->ehVirtualUnico())
		return;

	int maxDias = professor->getProfessor()->getMaxDiasSemana();
	if(HeuristicaNuno::probData->parametros->considerarMaxDiasPorProf == false || maxDias >= 7)
		return;

	auto itVars = varsProfDia.find(professor->getId());
	if(itVars == varsProfDia.end())
		HeuristicaNuno::excepcao("MIPAlocarProfs::criarRestMaxDiasSem_", "Vars prof dia não encontradas");

	MIPAlocarProfCons cons;
	cons.reset();
	cons.setType(MIPAlocarProfCons::C_PROF_MAX_DIAS);
	cons.setProfessor(professor);
	
	// máximo de dias por semana
	OPT_ROW rowMaxDias (OPT_ROW::ROWSENSE::LESS, maxDias, (char*) cons.toString().c_str());

	int nr = 0;
	for(auto it = itVars->second.begin(); it != itVars->second.end(); ++it)
	{
		rowMaxDias.insert(it->second, 1.0);
		nr++;
	}

	if(nr > maxDias)
		addRow_(rowMaxDias);
}
// criar restrições que detectam violação de minimo e o máximo de créditos semanais
void MIPAlocarProfs::criarRestProfLimitesChSem_(ProfessorHeur* const professor, unordered_map<TurmaHeur*, int> &varsProf)
{
	if(professor->ehVirtualUnico())
		return;

	bool eVirtual = professor->ehVirtual();
	
	// ----------------------------------------------
	// máximo de carga horária alocada na semana
	MIPAlocarProfCons cons;
	cons.reset();
	cons.setType(MIPAlocarProfCons::C_PROF_MAX_CH);
	cons.setProfessor(professor);

	int chMax = professor->getProfessor()->getChMax();
	OPT_ROW rowMaxChSem (OPT_ROW::ROWSENSE::LESS, chMax, (char*) cons.toString().c_str());
	
	// ----------------------------------------------
	// minimo de carga horária alocada na semana
	cons.setType(MIPAlocarProfCons::C_PROF_MIN_CH);

	int chMin = professor->getProfessor()->getChMin();
	OPT_ROW rowMinChSem (OPT_ROW::ROWSENSE::GREATER, 0.0, (char*) cons.toString().c_str());

	bool addMin = false;
	if(ParametrosHeuristica::minChForte == false)
	{
		auto itVar = varsProfMinChSem.find(professor->getId());
		if(itVar != varsProfMinChSem.end())
		{
			rowMinChSem.setRhs(chMin);
			rowMinChSem.insert(itVar->second, 1.0);
			addMin = true;
		}
	}
	else if(chMin > 0)
	{
		auto itUse = varsProfUsed.find(professor->getId());
		if(itUse == varsProfUsed.end())
			HeuristicaNuno::excepcao("MIPAlocarProfs::criarRestProfLimitesChSem_", "Variavel uso do professor nao encontrada");
		
		rowMinChSem.insert(itUse->second, -chMin);
		addMin = true;
	}
	
	// ----------------------------------------------
	// carga horaria anterior
	cons.setType(MIPAlocarProfCons::C_PROF_CH_ANT);

	double chDouble = double(professor->getProfessor()->getChAnterior()*ParametrosHeuristica::percChAnterior);
	int chMinAnt = (int)ceil(chDouble);
	OPT_ROW rowMinChAnt (OPT_ROW::ROWSENSE::GREATER, chMinAnt, (char*) cons.toString().c_str());
	bool addAnt = false;
	auto itAnt = varsProfChAnterior.find(professor->getId());
	if(itAnt != varsProfChAnterior.end())
	{
		addAnt = true;
		rowMinChAnt.insert(itAnt->second, 1.0);
	}

	// add vars prof turma
	int totCreds = 0;
	for(auto it = varsProf.begin(); it != varsProf.end(); ++it)
	{
		int creds = it->first->getNrCreditos();
		int colNr = it->second;

		rowMaxChSem.insert(colNr, creds);
		rowMinChSem.insert(colNr, creds);
		rowMinChAnt.insert(colNr, creds);
		totCreds += creds;
	}

	// add rows
	if((totCreds > chMax) && (chMax > 0))
		addRow_(rowMaxChSem);
	if(addMin)
		addRow_(rowMinChSem);
	if(!eVirtual && addAnt)
		addRow_(rowMinChAnt);
}
// criar restrições mínimo ch por dia
void MIPAlocarProfs::criarRestProfMinChDia_(ProfessorHeur* const professor, int dia, unordered_map<TurmaHeur*, int> &varsProf)
{
	if(professor->ehVirtualUnico())
		return;

	int minCreds = professor->getProfessor()->getMinCredsDiarios();
	if(minCreds <= 1)
		return;

	// get var prof dia
	int varProfDia = getVarProfDia(professor, dia);
	if(varProfDia < 0)
		return;
	
	// minimo de carga horária alocada por dia
	MIPAlocarProfCons cons;
	cons.reset();
	cons.setType(MIPAlocarProfCons::C_PROF_MIN_CH_DIA);
	cons.setProfessor(professor);
	cons.setDia(dia);

	OPT_ROW rowMinChDia (OPT_ROW::ROWSENSE::GREATER, 0.0, (char*) cons.toString().c_str());
	rowMinChDia.insert(varProfDia, -minCreds);

	for(auto itTurmas = varsProf.begin(); itTurmas != varsProf.end(); ++itTurmas)
	{
		AulaHeur* aula = nullptr;
		if(!itTurmas->first->getAulaDia(dia, aula) || (aula == nullptr))
			continue;

		int colNr = itTurmas->second;
		if(colNr < 0 || colNr >= lp_->getNumCols())
			HeuristicaNuno::excepcao("MIPAlocarProfs::criarRestAssignment_", "index de coluna varsProf invalido!");

		rowMinChDia.insert(colNr, aula->nrCreditos());
	}

	// restrição forte
	addRow_(rowMinChDia);
}

// criar restrições de uso sequencial de professores virtuais com mesmo perfil e do mesmo curso
void MIPAlocarProfs::criarRestsSeqProfsVirtuais_(void)
{
	if(!profsVirtuaisIndiv_)
		return;

	for(auto itPerf = solucao_->profsVirtuaisCursoPerfil.cbegin(); itPerf != solucao_->profsVirtuaisCursoPerfil.cend(); ++itPerf)
	{
		for(auto it = itPerf->second.begin(); it != itPerf->second.end(); ++it)
		{
			int id = (*it)->getId();
			if(!(*it)->ehVirtual())
				HeuristicaNuno::excepcao("MIPAlocarProfs::criarRestsSeqProfsVirtuais_", "Prof nao e virtual!");
			auto itHire = varsProfUsed.find(id);
			if(itHire == varsProfUsed.end())
				continue;
			
			MIPAlocarProfCons cons;
			cons.reset();
			cons.setType(MIPAlocarProfCons::C_PROF_VIRT_SEQ);
			cons.setPerfilVirt(itPerf->first);
			cons.setProfessor(*it);
				
			// proximo prof só pode ser usado se este for
			OPT_ROW row (OPT_ROW::ROWSENSE::LESS, 0.0, (char*) cons.toString().c_str());
			row.insert(itHire->second, -1.0);

			// proximo professor
			auto itNext = std::next(it);
			if(itNext == itPerf->second.end())
				break;

			// obter var use do proximo
			int idOther = (*itNext)->getId();
			auto itHireOth = varsProfUsed.find(idOther);
			if(itHireOth == varsProfUsed.end())
				continue;

			row.insert(itHireOth->second, 1.0);
			addRow_(row);
		}
	}
}

// criar restricoes que determinam o minimo de alunos por titulacao
void MIPAlocarProfs::criarRestMinMestres_(Curso* const curso, unordered_set<TurmaHeur*> const &turmas)
{
	if(!curso->temMinMestres())
		return;

	double perc = (curso->regra_min_mestres.second/100.0);
	criarRestMinTitulacao_(curso, turmas, perc, TipoTitulacao::Mestre);
}
void MIPAlocarProfs::criarRestMinDoutores_(Curso* const curso, unordered_set<TurmaHeur*> const &turmas)
{
	if(!curso->temMinDoutores())
		return;

	double perc = (curso->regra_min_doutores.second/100.0);
	criarRestMinTitulacao_(curso, turmas, perc, TipoTitulacao::Doutor);
}
void MIPAlocarProfs::criarRestMinTitulacao_(Curso* const curso, unordered_set<TurmaHeur*> const &turmas, double perc, int titulacao)
{
	int nr = 0;
	int nrTit = 0;
			
	if(perc < 0 || perc > 1)
	{
		stringstream ss;
		ss << "Percentagem negativa ou maior que 1! Valor: " << perc;
		HeuristicaNuno::excepcao("MIPAlocarProfs::criarRestMinTitulacao_", ss.str());
	}

	MIPAlocarProfCons cons;
	cons.reset();
	cons.setType(MIPAlocarProfCons::C_PROF_MIN_TITULO);
	cons.setCurso(curso);
				
	OPT_ROW row (OPT_ROW::ROWSENSE::GREATER, 0.0, (char*) cons.toString().c_str());
	for(auto itTurma = turmas.begin(); itTurma != turmas.end(); ++itTurma)
	{
		// professor atual
		auto itProfTurma = turmasProfs_.find(*itTurma);
		if(itProfTurma == turmasProfs_.end())
			HeuristicaNuno::excepcao("MIPAlocarProfs::criarRestMinTitulacao_", "Professor atual da turma nao encontrado!");

		// ver se ha vars
		auto itProfsT = varsTurmaProf.find(*itTurma);
		if(itProfsT == varsTurmaProf.end())
		{
			nr++;
			// contabilizar o prof atual  (fixado)
			if(itProfTurma->second->ehVirtualUnico() || (itProfTurma->second->tipoTitulacao()->getTitulacao() >= titulacao))
				nrTit++;
		}
		else
		{	// adicionar variaveis
			for(auto itProf = itProfsT->second.begin(); itProf != itProfsT->second.end(); ++itProf)
			{
				double coef = -perc;
				if(itProf->first->ehVirtualUnico() || (itProf->first->tipoTitulacao()->getTitulacao() >= titulacao))
					coef += 1;

				row.insert(itProf->second, coef);
			}
		}
	}
	// update no rhs.
	double rhs = (nr*perc) - nrTit;
	row.setRhs(rhs);

	addRow_(row);
}
// criar restricoes que determinam o minimo de alunos por tipo contrato
void MIPAlocarProfs::criarRestMinIntegral_(Curso* const curso, unordered_set<TurmaHeur*> const &turmas)
{
	if(!curso->temMinTempoIntegral())
		return;

	double perc = (curso->getMinTempoIntegral()/100.0);
	criarRestMinTipoContrato_(curso, turmas, perc, TipoContrato::Integral);
}
void MIPAlocarProfs::criarRestMinParcial_(Curso* const curso, unordered_set<TurmaHeur*> const &turmas)
{
	if(!curso->temMinTempoIntegralParcial())
		return;

	double perc = (curso->getMinTempoIntegralParcial()/100.0);
	criarRestMinTipoContrato_(curso, turmas, perc, TipoContrato::Parcial);
}
void MIPAlocarProfs::criarRestMinTipoContrato_(Curso* const curso, unordered_set<TurmaHeur*> const &turmas, double perc, int tipoContrato)
{
	int nr = 0;
	int nrTipo = 0;
		
	if(turmas.size() == 0)
		return;

	if(perc < 0 || perc > 1)
	{
		stringstream ss;
		ss << "Percentagem negativa ou maior que 1! Valor: " << perc;
		HeuristicaNuno::excepcao("MIPAlocarProfs::criarRestMinTipoContrato_", ss.str());
	}

	MIPAlocarProfCons cons;
	cons.reset();
	cons.setType(MIPAlocarProfCons::C_PROF_MIN_CONTRATO);
	cons.setCurso(curso);

	OPT_ROW row (OPT_ROW::ROWSENSE::GREATER, 0.0, (char*) cons.toString().c_str());
	for(auto itTurma = turmas.begin(); itTurma != turmas.end(); ++itTurma)
	{
		// professor atual
		auto itProfTurma = turmasProfs_.find(*itTurma);
		if(itProfTurma == turmasProfs_.end())
			HeuristicaNuno::excepcao("MIPAlocarProfs::criarRestMinTipoContrato_", "Professor atual da turma nao encontrado!");

		// procurar se tem var
		auto itProfsT = varsTurmaProf.find(*itTurma);
		if(itProfsT == varsTurmaProf.end())
		{
			nr++;
			// contabilizar o prof atual (fixado)
			if(itProfTurma->second->ehVirtualUnico() || itProfTurma->second->tipoContrato()->getContrato() >= tipoContrato)
				nrTipo++;
		}
		else
		{
			for(auto itProf = itProfsT->second.begin(); itProf != itProfsT->second.end(); ++itProf)
			{
				double coef = -perc;
				if(itProf->first->ehVirtualUnico() || (itProf->first->tipoContrato()->getContrato() >= tipoContrato))
					coef += 1;

				row.insert(itProf->second, coef);
			}
		}
	}
	// update no rhs.
	double rhs = (nr*perc) - nrTipo;
	row.setRhs(rhs);

	addRow_(row);
}

// criar restrições que impedem gaps nos horários do professor em uma mesma fase de um dia
void MIPAlocarProfs::criarRestProfHiHf_()
{	
	if ( CentroDados::getProblemData()->parametros->proibirProfGapMTN !=
		 ParametrosPlanejamento::ConstraintLevel::Strong )
		return;

	// Par: ( DateTimeInicial -> set<(col,coef)> ), ( DateTimeFinal -> set<(col,coef)> ) 
	typedef pair< map<DateTime, set< pair<int,double> >>, 
				  map<DateTime, set< pair<int,double> >> > ParMapDtiDtf;

	// Prof -> Dia -> Fase Do Dia -> Par-DateTime
	unordered_map<ProfessorHeur*, unordered_map<int, unordered_map<int,ParMapDtiDtf>>> mapProfDiaFaseHiHf;
	
	// Prof -> Dia -> Fase do dia -> <(col,coef)>
	map<ProfessorHeur*, map<int, map< int, set<pair<int,double>> > >> mapProfDiaFase;
	
	unordered_map<int, unordered_set<int>> mapDiasFases;

	// -------------------------------------------------------------------------------------------------
	// Agrupa variáveis em mapProfDiaFaseHiHf e mapProfDiaFase
	
	for(auto itProf = varsProfTurma.begin(); itProf != varsProfTurma.end(); ++itProf)
	{
		ProfessorHeur* const professor = itProf->first;

		if ( professor->ehVirtualUnico() )
		{
			continue;
		}

		if ( professor->ehVirtual() && ! ParametrosHeuristica::proibirGapProfVirt )
		{
			continue;
		}

		unordered_map<int, unordered_map<int,ParMapDtiDtf>> *mapProf1 = &mapProfDiaFaseHiHf[professor];
		map<int, map<int, set< pair<int,double> >> > *mapProf2 = &mapProfDiaFase[professor];

		for(auto itTurmas = itProf->second.begin(); itTurmas != itProf->second.end(); ++itTurmas)
		{
			TurmaHeur* const turma = itTurmas->first;
			const int col = itTurmas->second;

			unordered_map<int, AulaHeur*> aulas;
			turma->getAulas(aulas);

			auto itAula = aulas.begin();
			for ( ; itAula != aulas.end(); itAula++ )
			{
				DateTime dti, dtf;
				itAula->second->getPrimeiroHor(dti);
				itAula->second->getLastHor(dtf);
				int faseDoDia = CentroDados::getFaseDoDia(dti);
				int dia = itAula->first;
				
				mapDiasFases[dia].insert(faseDoDia);
				
				// ----------- mapProf1

				DateTime maiorDt = CentroDados::getFimDaFase(faseDoDia);
				double bigM = maiorDt.getDateMinutes();
				double coefXiUB = bigM;
				((*mapProf1)[dia][faseDoDia].first)[dti].insert( make_pair(col,coefXiUB) );

				double coefXfLB = - dtf.getDateMinutes();
				((*mapProf1)[dia][faseDoDia].second)[dtf].insert( make_pair(col,coefXfLB) );

				// ----------- mapProf2
				
				double duracaoAula = (dtf - dti).getDateMinutes();
				(*mapProf2)[dia][faseDoDia].insert( make_pair(col,duracaoAula) );

			}
		}
	}	
	
	// -------------------------------------------------------------------------------------------------
	// Prof -> Dia -> Fase Do Dia -> Par-DateTime

	unordered_map<ProfessorHeur*, unordered_map<int, unordered_map<int,ParMapDtiDtf>>> mapProfDiaFaseHilbHfub;

	// -------------------------------------------------------------------------------------------------
	// - Restrições para setar ub de hip e lb de hfp de cada dia-faseDoDia do professor
	// - Construção de mapProfDiaFaseHilbHfub

	for(auto itProf = mapProfDiaFaseHiHf.begin(); itProf != mapProfDiaFaseHiHf.end(); ++itProf)
	{
		ProfessorHeur *prof = itProf->first;

		auto itProfHiHf = varsProfDiaFaseHiHf.find(prof);
		if ( itProfHiHf == varsProfDiaFaseHiHf.end() )
		{
			stringstream ss;
			ss << "var hip-hfp nao encontrada para prof " << prof->getId();
			HeuristicaNuno::excepcao("MIPAlocarProfs::criarRestProfHiHf_", ss.str());
		}
		
		unordered_map<int, unordered_map<int,ParMapDtiDtf>> *mapProf3 = &mapProfDiaFaseHilbHfub[prof];

		for(auto itDia = itProf->second.begin(); itDia != itProf->second.end(); ++itDia)
		{
			int dia = itDia->first;

			auto itDiaHiHf = itProfHiHf->second.find(dia);
			if ( itDiaHiHf == itProfHiHf->second.end() )
			{
				stringstream ss;
				ss << "var hip-hfp nao encontrada para dia " << dia;
				HeuristicaNuno::excepcao("MIPAlocarProfs::criarRestProfHiHf_", ss.str());
			}
			
			unordered_map<int,ParMapDtiDtf> *mapDia3 = &(*mapProf3)[dia];

			for(auto itFase = itDia->second.begin(); itFase != itDia->second.end(); ++itFase)
			{
				int faseDoDia = itFase->first;

				auto itFaseHiHf = itDiaHiHf->second.find(faseDoDia);
				if ( itFaseHiHf == itDiaHiHf->second.end() )
				{
					stringstream ss;
					ss << "var hip-hfp nao encontrada para fase " << faseDoDia;
					HeuristicaNuno::excepcao("MIPAlocarProfs::criarRestProfHiHf_", ss.str());
				}
				
				// Procura vars hip e hfp
				int colHi = -1, colHf = -1;
				colHi = itFaseHiHf->second.first;
				colHf = itFaseHiHf->second.second;

				if ((colHi < 0) || (colHi >= lp_->getNumCols()))
				{
					HeuristicaNuno::logMsgInt("bad col idx: ", colHi, 1);
					HeuristicaNuno::excepcao("MIPAlocarProfs::criarRestProfHiHf_", "index de coluna hip invalido!");
				}
				if ((colHf < 0) || (colHf >= lp_->getNumCols()))
				{
					HeuristicaNuno::logMsgInt("bad col idx: ", colHf, 1);
					HeuristicaNuno::excepcao("MIPAlocarProfs::criarRestProfHiHf_", "index de coluna hfp invalido!");
				}
				
				map<DateTime, set< pair<int,double> >> *mapEqualDti = &itFase->second.first;
				map<DateTime, set< pair<int,double> >> *mapEqualDtf = &itFase->second.second;

				criarRestProfHiUB_( prof, dia, faseDoDia, colHi, *mapEqualDti );
				criarRestProfHfLB_( prof, dia, faseDoDia, colHf, *mapEqualDtf );

				
				// ----------------------------- preenche mapProf3 (pointer para parte de mapProfDiaFaseHilbHfub)
				
				DateTime maiorDt = CentroDados::getFimDaFase(faseDoDia);
				double bigM = maiorDt.getDateMinutes();

				ParMapDtiDtf *parMapFase3 = &(*mapDia3)[faseDoDia];
				map<DateTime, set< pair<int,double> >> *mapLessDti3 = &parMapFase3->first;
				map<DateTime, set< pair<int,double> >> *mapGreaterDtf3 = &parMapFase3->second;

				// preenche map com vars para setar lower bound de hi:  percorre do menor para o maior dt
				for ( auto itDti_2 = mapEqualDti->begin(); itDti_2 != mapEqualDti->end(); itDti_2++ )
				{
					DateTime dti_2 = itDti_2->first;
					double coefXiLB = dti_2.getDateMinutes();

					// Adiciona todas as vars x cujo tempo de inicio é MENOR que dti_2
					for ( auto itDti_1 = mapEqualDti->begin(); itDti_1 != itDti_2; itDti_1++ )
					{
						// Percore vars
						for ( auto itVars_1 = itDti_1->second.begin(); itVars_1 != itDti_1->second.end(); itVars_1++ )
						{
							int col1 = itVars_1->first;
							
							(*mapLessDti3)[dti_2].insert( make_pair(col1,coefXiLB) );
						}
					}
				}
				
				// preenche map com vars para setar upper bound de hf:  percorre do maior para o menor dt
				for ( auto itDtf_1 = mapEqualDtf->rbegin(); itDtf_1 != mapEqualDtf->rend(); itDtf_1++ )
				{
					DateTime dtf_1 = itDtf_1->first;
					double coefXfUB = - bigM;

					// Adiciona todas as vars x cujo tempo de fim é MAIOR que dtf_2
					for ( auto itDtf_2 = mapEqualDtf->rbegin(); itDtf_2 != itDtf_1; itDtf_2++ )
					{
						// Percore vars
						for ( auto itVars_2 = itDtf_2->second.begin(); itVars_2 != itDtf_2->second.end(); itVars_2++ )
						{
							int col2 = itVars_2->first;
							
							(*mapGreaterDtf3)[dtf_1].insert( make_pair(col2,coefXfUB) );
						}
					}
				}			
				
				// -----------------------------
			}
		}
	}

	// -------------------------------------------------------------------------------------------------
	// Restrições para setar lb de hip e ub de hfp de cada dia-faseDoDia do professor
	
	for(auto itProf = mapProfDiaFaseHilbHfub.begin(); itProf != mapProfDiaFaseHilbHfub.end(); ++itProf)
	{
		ProfessorHeur *prof = itProf->first;

		auto itProfHiHf = varsProfDiaFaseHiHf.find(prof);
		if ( itProfHiHf == varsProfDiaFaseHiHf.end() )
		{
			stringstream ss;
			ss << "var hip-hfp nao encontrada para prof " << prof->getId();
			HeuristicaNuno::excepcao("MIPAlocarProfs::criarRestProfHiHf_", ss.str());
		}

		for(auto itDia = itProf->second.begin(); itDia != itProf->second.end(); ++itDia)
		{
			int dia = itDia->first;

			auto itDiaHiHf = itProfHiHf->second.find(dia);
			if ( itDiaHiHf == itProfHiHf->second.end() )
			{
				stringstream ss;
				ss << "var hip-hfp nao encontrada para dia " << dia;
				HeuristicaNuno::excepcao("MIPAlocarProfs::criarRestProfHiHf_", ss.str());
			}

			for(auto itFase = itDia->second.begin(); itFase != itDia->second.end(); ++itFase)
			{
				int faseDoDia = itFase->first;
				auto itFaseHiHf = itDiaHiHf->second.find(faseDoDia);
				if ( itFaseHiHf == itDiaHiHf->second.end() )
				{
					stringstream ss;
					ss << "var hip-hfp nao encontrada para fase " << faseDoDia;
					HeuristicaNuno::excepcao("MIPAlocarProfs::criarRestProfHiHf_", ss.str());
				}
				
				// Procura vars hip e hfp
				int colHi = -1, colHf = -1;
				colHi = itFaseHiHf->second.first;
				colHf = itFaseHiHf->second.second;

				if ((colHi < 0) || (colHi >= lp_->getNumCols()))
				{
					HeuristicaNuno::logMsgInt("bad col idx: ", colHi, 1);
					HeuristicaNuno::excepcao("MIPAlocarProfs::criarRestProfHiHf_", "index de coluna hip invalido!");
				}
				if ((colHf < 0) || (colHf >= lp_->getNumCols()))
				{
					HeuristicaNuno::logMsgInt("bad col idx: ", colHf, 1);
					HeuristicaNuno::excepcao("MIPAlocarProfs::criarRestProfHiHf_", "index de coluna hfp invalido!");
				}

				criarRestProfHiLB_( prof, dia, faseDoDia, colHi, itFase->second.first );
				criarRestProfHfUB_( prof, dia, faseDoDia, colHf, itFase->second.second );
			}
		}
	}


	// -------------------------------------------------------------------------------------------------
	// Calcula os valores que serão usados como rhs das restrições de gap (a seguir)

	map< int, map< int, int > > somaTempoIntervDiaFase;
	
	for ( auto itDia = mapDiasFases.begin(); itDia != mapDiasFases.end(); itDia++ )
	{
		for ( auto itFase = itDia->second.begin(); itFase != itDia->second.end(); itFase++ )
		{
			somaTempoIntervDiaFase[itDia->first][*itFase] = 
				CentroDados::getProblemData()->getCalendariosMaxSomaInterv( itDia->first, *itFase );
		}
	}

	// -------------------------------------------------------------------------------------------------
	// Restrições para impedir gap nos horários de mesma fase do dia do professor
	
	for(auto itProf = mapProfDiaFase.begin(); itProf != mapProfDiaFase.end(); ++itProf)
	{
		ProfessorHeur *prof = itProf->first;
		auto itProfHiHf = varsProfDiaFaseHiHf.find(prof);

		for(auto itDia = itProf->second.begin(); itDia != itProf->second.end(); ++itDia)
		{
			int dia = itDia->first;
			auto itDiaHiHf = itProfHiHf->second.find(dia);

			for(auto itFase = itDia->second.begin(); itFase != itDia->second.end(); ++itFase)
			{
				int faseDoDia = itFase->first;
				auto itFaseHiHf = itDiaHiHf->second.find(faseDoDia);
				
				// Procura vars hip e hfp
				int colHi = -1, colHf = -1;
				colHi = itFaseHiHf->second.first;
				colHf = itFaseHiHf->second.second;

				// Right hand side é (-) o máximo de tempo ocioso permitido dentro de uma mesma fase f do dia t
				int delta = somaTempoIntervDiaFase[dia][faseDoDia];
				int rhs = - delta;

				criarRestProfGapMTN_( prof, dia, faseDoDia, rhs, colHi, colHf, itFase->second );
			}
		}
	}
}

void MIPAlocarProfs::criarRestProfHiUB_( ProfessorHeur* const prof, const int dia, const int fase,
	const int colHi, map<DateTime, set< pair<int,double> >> const &mapDtiVars )
{
	/*
		Restrição que limita superiormente a variável hip.

		Para todo professor p, todo dia t, todo fase do dia f, todo DateTime dt:

		hip_{p,t,f} <= m(dt) + M*( 1 - sum[x t.q. dti==dt] x_{p,t,h} )

		aonde:
		m(dt)		= datetime dt em número de minutos. Por ex: m(7:30) = 7*60 + 30
		x_{p,t,h}	= variável binária indicando se o prof p dá aula no dia t iniciando no horário h
		hip_{p,t,f}	= variável inteira indicando o primeiro horário (valor em número de minutos) 
					  da fase f do dia t em que prof p dá aula.
		
		"x t.q. dti==dt" significa as variáveis x tais que o DateTime de fim da aula dti é igual ao dt da restrição
	*/

	CentroDados::printTest("criarRestProfHi_", "criando rest ub hi");

	for(auto itDateTime = mapDtiVars.begin(); itDateTime != mapDtiVars.end(); ++itDateTime)
	{					
		DateTime dti = itDateTime->first;
		int dtMin = dti.getDateMinutes();

		double bigM = 9999999;

		MIPAlocarProfCons cons;
		cons.reset();
		cons.setType(MIPAlocarProfCons::C_PROF_HOR_INIC_UB);
		cons.setProfessor(prof);
		cons.setDia(dia);
		cons.setFaseDoDia(fase);
		cons.setDateTime(dti);

		OPT_ROW rowHi (OPT_ROW::ROWSENSE::LESS, 0.0, (char*) cons.toString().c_str() );

		// Insere vars ProfTurma
		for ( auto itVar = itDateTime->second.begin(); itVar != itDateTime->second.end(); itVar++ )
		{
			rowHi.insert(itVar->first,itVar->second);

			bigM = itVar->second;
		}

		// Corrige rhs
		double rhs = dtMin + bigM;
		rowHi.setRhs(rhs);	

		// Insere var hip
		rowHi.insert(colHi,1.0);
					
		addRow_(rowHi);					
	}
}

void MIPAlocarProfs::criarRestProfHiLB_( ProfessorHeur* const prof, const int dia, const int fase,
	const int colHi, map<DateTime, set< pair<int,double> >> const &mapDtiVars )
{
	/*
		Restrição que limita inferiormente a variável hip.

		Para todo professor p, todo dia t, todo fase do dia f, todo DateTime dt:

		hip_{p,t,f} >= m(dt) * ( 1 - sum[x t.q. dti<dt] x_{p,t,dti} )

		aonde:
		m(dt) = datetime dt em número de minutos. Por ex: m(7:30) = 7*60 + 30
		x_{p,t,dti}	= variável binária indicando se o prof p dá aula no dia t iniciando no DateTime dti
		hip_{p,t,f}	= variável inteira indicando o primeiro horário (valor em número de minutos) 
					  da fase f do dia t em que prof p dá aula.
		
		"x t.q. dti<dt" significa as variáveis x tais que o DateTime dti de inicio da aula é menor ao dt da restrição
	*/
	
	CentroDados::printTest("criarRestProfHiLB_", "criando rest lb hf");

	for(auto itDateTime = mapDtiVars.begin(); itDateTime != mapDtiVars.end(); ++itDateTime)
	{					
		DateTime dti= itDateTime->first;
		int dtMin = dti.getDateMinutes();

		MIPAlocarProfCons cons;
		cons.reset();
		cons.setType(MIPAlocarProfCons::C_PROF_HOR_INIC_LB);
		cons.setProfessor(prof);
		cons.setDia(dia);
		cons.setFaseDoDia(fase);
		cons.setDateTime(dti);

		int rhs = dtMin;
		OPT_ROW rowHi (OPT_ROW::ROWSENSE::GREATER, rhs, (char*) cons.toString().c_str());

		// Insere vars ProfTurma
		for ( auto itVar = itDateTime->second.begin(); itVar != itDateTime->second.end(); itVar++ )
		{
			rowHi.insert(itVar->first,itVar->second);

			if ( itVar->second != rhs )
				HeuristicaNuno::excepcao("MIPAlocarProfs::criarRestProfHiLB_()", "Coef de x diferente do rhs.");
		}

		// Insere var hfi
		rowHi.insert(colHi,1.0);
					
		addRow_(rowHi);					
	}
}

void MIPAlocarProfs::criarRestProfHfLB_( ProfessorHeur* const prof, const int dia, const int fase,
	const int colHf, map<DateTime, set< pair<int,double> >> const &mapDtfVars )
{
	/*
		Para todo professor p, todo dia t, todo fase do dia f, todo DateTime dt:

		hfp_{p,t,f} >= sum[x t.q. dtf==dt] m(dt) * x_{p,t,h}

		aonde:
		m(dt) = datetime dt em número de minutos. Por ex: m(7:30) = 7*60 + 30
		x_{p,t,h}	= variável binária indicando se o prof p dá aula no dia t iniciando no horário h
		hfp_{p,t,f}	= variável inteira indicando o último horário (valor em número de minutos) 
					  da fase f do dia t em que prof p dá aula.
		
		"x t.q. dtf==dt" significa as variáveis x tais que o DateTime de fim da aula dtf é igual ao dt da restrição
	*/
	
	CentroDados::printTest("criarRestProfHfLB_", "criando rest lb hf");

	for(auto itDateTime = mapDtfVars.begin(); itDateTime != mapDtfVars.end(); ++itDateTime)
	{					
		DateTime dtf= itDateTime->first;
		int dtMin = dtf.getDateMinutes();

		MIPAlocarProfCons cons;
		cons.reset();
		cons.setType(MIPAlocarProfCons::C_PROF_HOR_FIN_LB);
		cons.setProfessor(prof);
		cons.setDia(dia);
		cons.setFaseDoDia(fase);
		cons.setDateTime(dtf);

		int rhs = 0.0;
		OPT_ROW rowHf (OPT_ROW::ROWSENSE::GREATER, rhs, (char*) cons.toString().c_str());

		// Insere vars ProfTurma
		for ( auto itVar = itDateTime->second.begin(); itVar != itDateTime->second.end(); itVar++ )
		{
			rowHf.insert(itVar->first,itVar->second);										
		}
										
		// Insere var hfp
		rowHf.insert(colHf,1.0);
					
		addRow_(rowHf);					
	}
}

void MIPAlocarProfs::criarRestProfHfUB_( ProfessorHeur* const prof, const int dia, const int fase,
	const int colHf, map<DateTime, set< pair<int,double> >> const &mapDtfVars )
{
	/*
		Para todo professor p, todo dia t, todo fase do dia f, todo DateTime dt:

		hfp_{p,t,f} <= m(dt) + M*( sum[x t.q. dtf>dt] x_{p,t,h} )

		aonde:
		m(dt)		= datetime dt em número de minutos. Por ex: m(7:30) = 7*60 + 30
		x_{p,t,h}	= variável binária indicando se o prof p dá aula no dia t que inicia no horario h
		hfp_{p,t,f}	= variável inteira indicando o último horário (valor em número de minutos) 
					  da fase f do dia t em que prof p dá aula.
		
		"x t.q. dtf>dt" significa as variáveis x tais que o DateTime dtf de fim da aula é maior que o dt da restrição
	*/

	for(auto itDateTime = mapDtfVars.begin(); itDateTime != mapDtfVars.end(); ++itDateTime)
	{					
		DateTime dtf = itDateTime->first;
		int dtMax = dtf.getDateMinutes();
		double rhs = dtMax;

		MIPAlocarProfCons cons;
		cons.reset();
		cons.setType(MIPAlocarProfCons::C_PROF_HOR_FIN_UB);
		cons.setProfessor(prof);
		cons.setDia(dia);
		cons.setFaseDoDia(fase);
		cons.setDateTime(dtf);

		OPT_ROW rowHf (OPT_ROW::ROWSENSE::LESS, rhs, (char*) cons.toString().c_str());

		// Insere vars ProfTurma
		for ( auto itVar = itDateTime->second.begin(); itVar != itDateTime->second.end(); itVar++ )
		{
			rowHf.insert(itVar->first,itVar->second);
		}
		
		// Insere var hfp
		rowHf.insert(colHf,1.0);
					
		addRow_(rowHf);					
	}

}

void MIPAlocarProfs::criarRestProfGapMTN_( ProfessorHeur* const prof, const int dia, const int fase,
	const int rhs, const int colHi, const int colHf, set< pair<int,double> > const &varsColCoef )
{
	/*
		Para todo professor p, todo dia t, todo fase do dia f:

		sum[x t.q. h e Hf] tempo_{x} * x_{p,t,h} + delta_{f,t} >= hfp_{p,t,f} - hip_{p,t,f}

		aonde:
		duracao_{x}	= tempo de duração (min) da aula representada por x
		delta_{f,t} = máximo de tempo ocioso permitido dentro de uma mesma fase f do dia t,
					  entre a primeira e a ultima aula do professor na fase. Conta, basicamente,
					  o tempo do intervalo entre aulas.
		x_{p,t,h}	= variável binária indicando se o prof p dá aula no dia t que inicia no horário h
		hfp_{p,t,f}	= variável inteira indicando o último horário (valor em número de minutos) 
					  da fase f do dia t em que prof p dá aula.
		hip_{p,t,f}	= variável inteira indicando o primeiro horário (valor em número de minutos) 
					  da fase f do dia t em que prof p dá aula.

		"x t.q. h e Hf" significa as variáveis x tais que o horário h de inicio da aula 
			pertence aos horários do turno f
	*/
	
	MIPAlocarProfCons cons;
	cons.reset();
	cons.setType(MIPAlocarProfCons::C_PROF_GAP);
	cons.setProfessor(prof);
	cons.setDia(dia);
	cons.setFaseDoDia(fase);

	OPT_ROW row (OPT_ROW::ROWSENSE::GREATER, rhs, (char*) cons.toString().c_str());

	for(auto itVars = varsColCoef.begin(); itVars != varsColCoef.end(); ++itVars)
	{					
		int col = itVars->first;
		double coef = itVars->second;
		
		// Insere vars ProfTurma
		row.insert(col,coef);				
	}		

	// Insere var hip
	row.insert(colHi,1.0);
	// Insere var hfp
	row.insert(colHf,-1.0);

	addRow_(row);
}

// ------------------------------------------------------------------------------------

// pega na solução herdada da heurística e carrega-a para o MIP
void MIPAlocarProfs::definirMIPStart_(void)
{
	const int nrCols = lp_->getNumCols();
	// inicializar estruturas
	int* indices_ = new int [nrCols]();
	for(int idx = 0; idx < nrCols; ++idx)
		indices_[idx] = idx;

	double* values_ = new double [nrCols]();
	for(int idx = 0; idx < nrCols; ++idx)
		values_[idx] = 0;

	for(auto itTurmas = turmasProfs_.begin(); itTurmas != turmasProfs_.end(); ++itTurmas)
	{
		TurmaHeur* const turma = itTurmas->first;
		ProfessorHeur* const prof = itTurmas->second;

		auto itVarTurma = varsTurmaProf.find(turma);
		if(itVarTurma == varsTurmaProf.end())
		{
			//HeuristicaNuno::warning("MIPAlocarProfs::definirMIPStart_", "Vars da turma nao encontradas");
			continue;
		}
		auto itVar = itVarTurma->second.find(prof);
		if(itVar == itVarTurma->second.end())
		{
			//HeuristicaNuno::warning("MIPAlocarProfs::definirMIPStart_", "Var prof-turma nao encontrada");
			continue;
		}

		values_[itVar->second] = 1.0;
	}

	// partial fix
	addMIPStartParcial(indices_, values_, nrCols);

	delete indices_;
	delete values_;
}
// MIP start com todas as turmas com professor virtual único
void MIPAlocarProfs::definirMIPStartAllVirtual_(void)
{
	const int nrCols = lp_->getNumCols();
	// inicializar estruturas
	int* indices_ = new int [nrCols]();
	for(int idx = 0; idx < nrCols; ++idx)
		indices_[idx] = idx;

	double* values_ = new double [nrCols]();
	for(int idx = 0; idx < nrCols; ++idx)
		values_[idx] = 0;

	// get professor virtual único
	auto itVirtual = solucao_->professoresHeur.find(ParametrosHeuristica::profVirtualId);
	if(itVirtual == solucao_->professoresHeur.end())
		HeuristicaNuno::excepcao("MIPAlocarProfs::definirMIPStartAllVirtual_", "Professor virtual unico nao encontrado.");

	ProfessorHeur* const profVirtualUnico = itVirtual->second;

	// todas as turmas com prof virtual
	for(auto it = varsTurmaProf.begin(); it != varsTurmaProf.end(); ++it)
	{
		auto itV = it->second.find(profVirtualUnico);
		if(itV == it->second.end())
			HeuristicaNuno::excepcao("MIPAlocarProfs::definirMIPStartAllVirtual_", "Var prof turma de professor virtual nao encontrada.");

		values_[itV->second] = 1.0;
	}

	// partial fix
	addMIPStartParcial(indices_, values_, nrCols);

	delete indices_;
	delete values_;
}

// verificar a % de profs reais que teve a chAnterior * perc % satisfeita
void MIPAlocarProfs::checkChAnterior_(void)
{
	int nrProfsCh = 0;
	int nrProfsChMin = 0;
	for(auto it = solucao_->professoresHeur.cbegin(); it != solucao_->professoresHeur.cend(); ++it)
	{
		ProfessorHeur* const professor = it->second;
		if(professor->ehVirtual())
			continue;

		int chAnt = professor->getProfessor()->getChAnterior();
		if(chAnt <= 0)
			continue;

		nrProfsCh++;
		int chMin = (int)ceil(chAnt * ParametrosHeuristica::percChAnterior);
		if(professor->getNrCreditosAlocados() >= chMin)
			nrProfsChMin++;
	}

	HeuristicaNuno::logMsgInt("nr profs ch anterior: ", nrProfsCh, 1);
	HeuristicaNuno::logMsgInt("nr profs ch anterior satisfeita (c/ relax): ", nrProfsChMin, 1);
}

// verificar se a proibição de gaps no horários de mesma fase do dia do prof foi respeitada
void MIPAlocarProfs::checkGapsProf_(void)
{
	if ( CentroDados::getProblemData()->parametros->proibirProfGapMTN !=
		 ParametrosPlanejamento::ConstraintLevel::Strong)
		return;

	// ---------------------------------------------------------------------------------
	// Agrupa os horários alocados aos professores na solução

	unordered_map<ProfessorHeur*, unordered_map<int, unordered_map<int, set< pair<DateTime,DateTime> > > > > solProfDiaFaseDt;

	// Campus
	for(auto itCampus = solucao_->ofertasDisciplina_.begin(); itCampus != solucao_->ofertasDisciplina_.end(); ++itCampus)
	{
		//Disciplina
		for(auto itDisc = itCampus->second.begin(); itDisc != itCampus->second.end(); ++itDisc)
		{
			// OfertaDisciplina
			OfertaDisciplina* const oferta = itDisc->second;
			unordered_set<TurmaHeur*> turmas;
			oferta->getTurmas(turmas);

			for(auto itTurma = turmas.begin(); itTurma != turmas.end(); itTurma++)
			{
				TurmaHeur* turma = *itTurma;

				ProfessorHeur* prof = turma->getProfessor();
				if ( prof==nullptr )
				{
					HeuristicaNuno::excepcao("void MIPAlocarProfs::checkGapsProf_(void)", "Turma sem professor alocado.");
					continue;
				}

				if ( prof->ehVirtualUnico() )
				{
					continue;
				}
				if ( prof->ehVirtual() && ! ParametrosHeuristica::proibirGapProfVirt )
				{
					continue;
				}

				unordered_map<int, AulaHeur*> aulas;
				turma->getAulas(aulas);

				unordered_map<int, unordered_map<int, set< pair<DateTime,DateTime> > > >
					*solDiaFaseDt = & solProfDiaFaseDt[prof];

				for ( auto itAula = aulas.begin(); itAula != aulas.end(); itAula++ )
				{
					int dia = itAula->first;
					AulaHeur * aula = itAula->second;
					
					DateTime dti;
					aula->getPrimeiroHor(dti);
					DateTime dtf;
					aula->getLastHor(dtf);

					int fase = CentroDados::getFaseDoDia(dti);

					(*solDiaFaseDt)[dia][fase].insert( make_pair(dti,dtf) );
				}
			}
		}
	}
	
	// ---------------------------------------------------------------------------------
	// Verifica se há gaps em mesma fase do dia para todos os professores da solução

	for(auto itProf = solProfDiaFaseDt.begin(); itProf != solProfDiaFaseDt.end(); ++itProf)
	{
		for(auto itDia = itProf->second.begin(); itDia != itProf->second.end(); ++itDia)
		{
			for(auto itFase = itDia->second.begin(); itFase != itDia->second.end(); ++itFase)
			{
				int maxInterv = CentroDados::getProblemData()->getCalendariosMaxInterv(itDia->first,itFase->first);

				for(auto itParDt = itFase->second.begin(); itParDt != itFase->second.end(); ++itParDt)
				{
					auto itNext = std::next(itParDt);
					if ( itNext == itFase->second.end() ) continue;
					
					DateTime dtf = itParDt->second;
					DateTime dti = itNext->first;

					int gap = (dti - dtf).getDateMinutes();
					
					if ( gap > maxInterv )
					{
						stringstream msg;
						msg << "Gap de " << gap << " mins encontrado no dia " << itDia->first << " fase "
							<< itFase->first << " do prof " << itProf->first->getId();
						CentroDados::printError("void MIPAlocarProfs::checkGapsProf_(void)", msg.str());
					}
				}
			}
		}
	}
}