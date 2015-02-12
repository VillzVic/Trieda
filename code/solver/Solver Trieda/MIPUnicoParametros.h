#ifndef _MIPUNICO_PARAMETROS_H_
#define _MIPUNICO_PARAMETROS_H_

#include <string>

class MIPUnicoParametros
{
public:
	MIPUnicoParametros(void);
	~MIPUnicoParametros(void);

	virtual void foo () = 0;	// abstract
	
	// Constantes e parâmetros

	enum PRIOR_PROF_TYPE
	{
		PriorTypeOff,	// Sem prior de profs	= um único modelo contendo indiferentemente todos os professores reais
		PriorType1,	// Prior 1 > Prior 2	= etapa de maximizar demanda é única, com um único modelo contendo ambas prioridades
		PriorType2,	// Prior 1 >> Prior 2	= todas as etapas são duplas (exceto marreta), com um único modelo contendo ambas prioridades
		PriorType3	// Prior 1 >> Prior 2	= todas as etapas são duplas (exceto marreta), com um modelo para cada prioridade
	};
	
   enum OutPutFileType
   {
	  MIP_GENERAL,
	  MIP_GARANTE_SOL,
	  MIP_MAX_ATEND,
	  MIP_MIN_VIRT,
	  MIP_MIN_TURMAS_COMPART,
	  MIP_MIN_FASE_DIA_PROF,
	  MIP_MIN_DESLOC_PROF,
	  MIP_MIN_GAP_PROF,
	  MIP_MARRETA
   };
	    
	// Gurobi
	static const int timeLimitMaxAtend;
	static const int timeLimitMaxAtendSemMelhora;
	static const int timeLimitMinProfVirt;
	static const int timeLimitMinProfVirtSemMelhora;
	static const int timeLimitMinTurmas;
	static const int timeLimitMinTurmasSemMelhora;
	static const int timeLimitMinGapProf;
	static const int timeLimitMinGapProfSemMelhora;
	static const int timeLimitGeneral;
	static const int timeLimitGeneralSemMelhora;
	static const int timeLimitMinDeslocProf_;
	static const int timeLimitMinDeslocProfSemMelhora_;
	static const int timeLimitMinFaseDiaProf_;
	static const int timeLimitMinFaseDiaProfSemMelhora_;

	// Disciplinas
	static const int consideraDivCredDisc;
	static const double pesoDivCred;

	// Professores	
	static bool permiteCriarPV;
	static const bool limitarNrUnidsProfDia_;
	static const int MaxUnidProfDia_;
	static const bool filtroPVHorCompl_;
	static const bool minimizarCustoProf;
	static const double pesoGapProf;
	static const double pesoCredPV;
	static const double pesoDeslocProf;
	static const double pesoCHAntProf;
	static const bool limitar1DeslocSoUnidLonge_;
	static const bool limitarDeslocUnidLongeSemana_;
	static const int maxDeslocUnidLongeSemana_;	
	static const int maxTempoDeslocCurto_;
	static const bool minimizarProfFaseDoDiaUsada_;
	static const bool minimizarProfDiaUsado_;
	static int priorProfLevel_;
	static const PRIOR_PROF_TYPE priorProfImportante_;
	static const bool minimizarGapProfEntreFases_;
	static const int MaxGapEntreFase_;
	static const int minCredDispFaseMinGapProf_;

	static const bool fixarSolucaoProfPrior1_;

	static const int allPriorProfLevels_;

	// Alunos
	static const double pesoFD;
	static const int pesoGapAluno;
	static const int pesoMinCredDiaAluno;
	static const int desvioMinCredDiaAluno;		// desvio máximo do nr médio de créditos por dia do aluno, sem que haja penalização 
	static const int considerarMinCredDiaAluno;
	static const bool ignorarGapAlunoContraTurno_;

	
    static std::string getOutPutFileTypeToString(int type);
	
};

#endif