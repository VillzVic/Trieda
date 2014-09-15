#include "ParametrosHeuristica.h"
#include "HeuristicaNuno.h"
#include "../ProblemData.h"
#include "../ParametrosPlanejamento.h"
#include "../Professor.h"

Professor* ParametrosHeuristica::professorVirtual = new Professor(true);

ParametrosHeuristica::ParametrosHeuristica(void)
{
}

ParametrosHeuristica::~ParametrosHeuristica(void)
{
}

// setup dados
void ParametrosHeuristica::setup(ProblemData* const problemData)
{
	// set id professor virtual �nico
	ParametrosHeuristica::professorVirtual->setId(ParametrosHeuristica::profVirtualId);

	// set relacoes das pr�ticas
	ParametrosHeuristica::setRelacPraticas(problemData);
	// set percChAnterior
	ParametrosHeuristica::percChAnterior = (1 - (problemData->parametros->perc_max_reducao_CHP/100));
	if(!problemData->parametros->evitar_reducao_carga_horaria_prof)
		ParametrosHeuristica::percChAnterior = 0;

	// desligar m�nimo de alunos interno
	if(ParametrosHeuristica::desligarMinAlunosInterno)
	{
		ParametrosHeuristica::minAlunosTurma = 1;
		ParametrosHeuristica::limMinAlunosTurma = 1;
	}
}

// Vers�o r�pida da heur�stica (sem MIP)
const bool ParametrosHeuristica::versaoFast = false;

// Local Search
const bool ParametrosHeuristica::localSearch = false;
const int ParametrosHeuristica::timeLocalSearch = 1200;	// segundos

// rela��o pr�ticas
ParametrosHeuristica::abrePraticas ParametrosHeuristica::relacPraticas = ParametrosHeuristica::abrePraticas::N_N;
// set relac praticas
void ParametrosHeuristica::setRelacPraticas(ProblemData* const data)
{
	if(data->parametros->discPratTeorMxN)
	{
		ParametrosHeuristica::relacPraticas = ParametrosHeuristica::abrePraticas::N_N;
		HeuristicaNuno::logMsg("[PARAM] Rela��o teoricas x praticas: MxN", 0);
	}
	else if(data->parametros->discPratTeor1xN)
	{
		ParametrosHeuristica::relacPraticas = ParametrosHeuristica::abrePraticas::UM_N;
		HeuristicaNuno::logMsg("[PARAM] Rela��o teoricas x praticas: 1xN", 0);
	}
	else if(data->parametros->discPratTeor1x1)
	{
		ParametrosHeuristica::relacPraticas = ParametrosHeuristica::abrePraticas::UM_UM;
		HeuristicaNuno::logMsg("[PARAM] Rela��o teoricas x praticas: 1x1", 0);
	}
}

// desligar m�nimo de alunos interno	[Default: true]
bool ParametrosHeuristica::desligarMinAlunosInterno = true;
// reduzir calendarios. remover dos calendarios da disciplina aqueles que s�o abrangidos por outros
const bool ParametrosHeuristica::reduzirCalendarios = true;
// indica se gerar de novo todas as combina��es de cr�ditos ou n�o
const bool ParametrosHeuristica::gerarAllCombsCreds = false;


// id prof virtual
const int ParametrosHeuristica::profVirtualId = -999999;

// -------------------------------- ABRIDOR TURMAS --------------------------------------------
const int ParametrosHeuristica::maxCredsExcedentes = 2;
const int ParametrosHeuristica::maxIntervAulas = 35;
int ParametrosHeuristica::minAlunosTurma = 8; // Minimo alunos turma (quando n�o definido!)
int ParametrosHeuristica::limMinAlunosTurma = 5;

int ParametrosHeuristica::getMinAlunos(bool usaLab)
{
	int min = ParametrosHeuristica::minAlunosTurma;

	// turma pratica
	if ( usaLab )
	{
		// considerando minimo de alunos pratico
		if( HeuristicaNuno::probData->parametros->min_alunos_abertura_turmas_praticas )
			min = HeuristicaNuno::probData->parametros->min_alunos_abertura_turmas_praticas_value;
		// n�o considerando minimo de alunos pratico
		else
			min = ParametrosHeuristica::limMinAlunosTurma;
	}
	// turma teorica, considerando minimo de alunos teorico
	else if ( !usaLab && HeuristicaNuno::probData->parametros->min_alunos_abertura_turmas )
	{
		min = HeuristicaNuno::probData->parametros->min_alunos_abertura_turmas_value;
	}
	
	return min;
}
bool ParametrosHeuristica::temMins(void)
{
	if(getMinAlunos(true) != 1)
		return true;

	if(getMinAlunos(false) != 1)
		return true;

	return false;
}


// Flexibiliza��o da regra de minimo de n�mero de alunos por turma, para abrir algumas turmas a mais pelo abridor.
double ParametrosHeuristica::relaxMinAlunosTurma = 0.40;				// % do m�nimo de alunos considerada

const double ParametrosHeuristica::minCapacidadeUsoSala = 0.0;		// teste
const double ParametrosHeuristica::desvioMaximoValorTurma = 0.00;

// tools
const bool ParametrosHeuristica::heurRealocAlunos = true;
const bool ParametrosHeuristica::dominanciaTurmas = true;
// limitar nr de turmas de uma disciplina ao mesmo tempo
const bool ParametrosHeuristica::limTurmasSimultaneas = true;
const int ParametrosHeuristica::slackTurmasSimult = 3;

const int ParametrosHeuristica::maxTurmasPotenciais = 100;
// indicador de demandas activo
const bool ParametrosHeuristica::indicDemSalas = true;

// m�ximo de itera��es na abertura de turmas final
const int ParametrosHeuristica::maxIterAbrirTurmasFinal = 5;

// -------------------------------- MIP ALOC --------------------------------------------
int ParametrosHeuristica::timeLimitMIP = 3600;
const double ParametrosHeuristica::heurFreqMIP = -1;

const double ParametrosHeuristica::realocarMipGapTolerance = 0.0005;	// 0.05%

// -------------------------------- MIP ALOC ALUNOS --------------------------------------------
const bool ParametrosHeuristica::alocarP2 = true;
const bool ParametrosHeuristica::fecharTurmasCarregadas = true;

bool ParametrosHeuristica::mipRealocSalas = true;

const double ParametrosHeuristica::coefDemAtendP1 = 1.0;
const double ParametrosHeuristica::coefDemAtendP2 = 0.001;
const double ParametrosHeuristica::coefEquivalenciaMIP = 0.99;
const double ParametrosHeuristica::pesoExtraFormando = 1.1;
const double ParametrosHeuristica::pesoExtraCalouro = 1.1;
const double ParametrosHeuristica::pesoExtraCarregado = 20.0;
const double ParametrosHeuristica::coefAbrirTurmaMIP = 0.0;	// -0.1;
const double ParametrosHeuristica::coefAbrirTurmaMIPCreds = -7;

// coef da vari�vel de folga m�nimo de alunos por turma (s� em modo Improve Solu��o !)
const double ParametrosHeuristica::coefPenalViolaMinAlunos = -0.5;

// minimo imposto no MIPAlocarAlunos para o r�cio (cr�ditos aluno / cr�ditos prof)
double ParametrosHeuristica::minProdutividadeCred = 0.0;	// default

// --------------------------------------- MIP ALOC PROFS --------------------------------------------
const bool ParametrosHeuristica::considerarChAnterior = true;
const bool ParametrosHeuristica::profsVirtuaisIndiv = true;
// custo deve ser negativo!!
const double ParametrosHeuristica::coefProfReal = 0;
const double ParametrosHeuristica::coefProfVirtual = -50;
const double ParametrosHeuristica::coefProfVirtualUnico = -100000;
const double ParametrosHeuristica::coefHireProfVirtual = -10;

const bool ParametrosHeuristica::minChForte = false;
const double ParametrosHeuristica::penalCredMinChSemana = -2;
const double ParametrosHeuristica::coefProfDia = -0.1;
const double ParametrosHeuristica::penalCredChAnterior = -1;
double ParametrosHeuristica::percChAnterior = 0.80;				// default
const bool ParametrosHeuristica::proibirGapProfVirt = false;

// Destrui��o da solu��o final para testar carregamento [deve tar estar FALSE no funcionamento normal]
const bool ParametrosHeuristica::finalDestroy = false;