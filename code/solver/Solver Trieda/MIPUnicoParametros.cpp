#include "MIPUnicoParametros.h"

#include "ParametrosPlanejamento.h"
#include "CentroDados.h"

// -----------------------------------------------------------------------------------------------

MIPUnicoParametros::MIPUnicoParametros(void)
{
}

MIPUnicoParametros::~MIPUnicoParametros(void)
{
}

// -----------------------------------------------------------------------------------------------
// Parâmetros

// Gurobi
const int MIPUnicoParametros::timeLimitMaxAtend = 3600*4;
const int MIPUnicoParametros::timeLimitMaxAtendSemMelhora = 3600;
const int MIPUnicoParametros::timeLimitMinProfVirt = 3600*3;
const int MIPUnicoParametros::timeLimitMinProfVirtSemMelhora = 3600*2;
const int MIPUnicoParametros::timeLimitMinTurmas = 3600*1.5;
const int MIPUnicoParametros::timeLimitMinTurmasSemMelhora = 2700;
const int MIPUnicoParametros::timeLimitMinGapProf = 3600;
const int MIPUnicoParametros::timeLimitMinGapProfSemMelhora = 1800;
const int MIPUnicoParametros::timeLimitGeneral= 3600;
const int MIPUnicoParametros::timeLimitGeneralSemMelhora = 1800;
const int MIPUnicoParametros::timeLimitMinDeslocProf_= 3600*2;
const int MIPUnicoParametros::timeLimitMinDeslocProfSemMelhora_ = 3600;
const int MIPUnicoParametros::timeLimitMinFaseDiaProf_ = 3600;
const int MIPUnicoParametros::timeLimitMinFaseDiaProfSemMelhora_ = 1800;



// Disciplinas
const int MIPUnicoParametros::consideraDivCredDisc = ParametrosPlanejamento::Weak;

// Professores
bool MIPUnicoParametros::permiteCriarPV = true;
const bool MIPUnicoParametros::limitarNrUnidsProfDia_ = false;
const int MIPUnicoParametros::MaxUnidProfDia_ = 2;
const bool MIPUnicoParametros::filtroPVHorCompl_ = true;
const bool MIPUnicoParametros::minimizarCustoProf = false;
const bool MIPUnicoParametros::limitar1DeslocSoUnidLonge_ = true;
const bool MIPUnicoParametros::limitarDeslocUnidLongeSemana_ = true;
const int MIPUnicoParametros::maxDeslocUnidLongeSemana_ = 2;
const int MIPUnicoParametros::maxTempoDeslocCurto_ = 50;
const bool MIPUnicoParametros::minimizarProfFaseDoDiaUsada_ = true;
const bool MIPUnicoParametros::minimizarProfDiaUsado_ = true;
int MIPUnicoParametros::priorProfLevel_ = 0;
const MIPUnicoParametros::PRIOR_PROF_TYPE MIPUnicoParametros::priorProfImportante_ = MIPUnicoParametros::PriorTypeOff;
const bool MIPUnicoParametros::minimizarGapProfEntreFases_ = true;
const int MIPUnicoParametros::MaxGapEntreFase_ = 170;
const int MIPUnicoParametros::minCredDispFaseMinGapProf_ = 3;

const int MIPUnicoParametros::allPriorProfLevels_ = 0;
const bool MIPUnicoParametros::fixarSolucaoProfPrior1_ = true;

// Geral
const bool MIPUnicoParametros::goalProgramming_ = false;

// Alunos
const int MIPUnicoParametros::desvioMinCredDiaAluno = 2;
const int MIPUnicoParametros::considerarMinCredDiaAluno = ParametrosPlanejamento::Off;
const bool MIPUnicoParametros::ignorarGapAlunoContraTurno_ = true;

// Peso de variaveis
const double MIPUnicoParametros::pesoDivCred = 0.01;			// multiplica uma var inteira
const double MIPUnicoParametros::pesoFD = 100;					// multiplica uma var binaria
const int MIPUnicoParametros::pesoGapAluno = 3;					// multiplica uma var inteira
const int MIPUnicoParametros::pesoMinCredDiaAluno = 500;		// multiplica uma var inteira
const double MIPUnicoParametros::pesoGapProf = 0.0001;			// multiplica uma var inteira
const double MIPUnicoParametros::pesoCredPV = 10;				// multiplica uma var binaria
const double MIPUnicoParametros::pesoDeslocProf = 0.01;			// multiplica uma var inteira
const double MIPUnicoParametros::pesoCHAntProf = 0.0001;		// multiplica uma var inteira
const double MIPUnicoParametros::pesoProfDia = 0.01;			// multiplica uma var binaria
const double MIPUnicoParametros::pesoProfFaseDia = 0.01;		// multiplica uma var binaria
const double MIPUnicoParametros::pesoProfUnidDia = 0.01;		// multiplica uma var binaria


std::string MIPUnicoParametros::getGoalToString(int type)
{
	std::string solName = "";

	switch (type)
	{
		case (MIPUnicoParametros::MIP_GARANTE_SOL):
			solName = "GaranteSol";
			break;
		case (MIPUnicoParametros::MIP_MAX_ATEND):
			solName = "MaxAtend";
			break;
		case (MIPUnicoParametros::MIP_MIN_VIRT):
			solName = "MinVirt";
			break;
		case (MIPUnicoParametros::MIP_MIN_TURMAS_COMPART):
			solName = "MinTurmasCompart";
			break;
		case (MIPUnicoParametros::MIP_MIN_DESLOC_PROF):
			solName = "MinDeslocProf";
			break;
		case (MIPUnicoParametros::MIP_MIN_FASE_DIA_PROF):
			solName = "MinFaseDiaProf";
			break;			
		case (MIPUnicoParametros::MIP_MIN_GAP_PROF):
			solName = "MinGapProf";
			break;
		case (MIPUnicoParametros::MIP_MARRETA):
			solName = "Marreta";
			break;
		default:
			if (type != MIPUnicoParametros::MIP_GENERAL)
				CentroDados::printError("MIPUnicoParametros::getGoalToString()","Etapa de tipo nao identificado.");
			break;
	}
	return solName;
}