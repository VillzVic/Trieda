#ifndef _MIP_ALOC_H_
#define _MIP_ALOC_H_

#include <unordered_set>
#include <unordered_map>
#include <set>
#include "../HorarioAula.h"
#include "TurmaHeur.h"
#include "ProfessorHeur.h"
#include "Node.h"

#ifdef SOLVER_CPLEX
#include "opt_cplex.h"
#endif

#ifdef SOLVER_GUROBI
#include "opt_gurobi.h"
#endif

using namespace std;

class TurmasIncHorarioDia;
class ConjUnidades;
struct hashHorsDia;
struct equalHorsDia;

typedef unordered_map<const ConjUnidades*, unordered_map<pair<int, HorarioAula*>, TurmasIncHorarioDia* , hashHorsDia, equalHorsDia>> turmasIncUnidHorDia;


//----------- Nodes usados na gera��o de cliques de incompatibilidade ------



//---------------------------------------------------------------------------

class MIPAloc
{
	friend class SolucaoHeur;
public:
	// n�o chamar directamente
	MIPAloc(int tipoMip, std::string nome, SolucaoHeur* const solucao_);
	virtual ~MIPAloc(void);

	// fazer aloca��o
	virtual void alocar(void);

protected:
	std::string nome_;
	SolucaoHeur* solucao_;
	const int tipoMIP_;					// 0 - alocar alunos/salas, 1 - alocar profs

	// Modelo LP
	#ifdef SOLVER_CPLEX 
	   OPT_CPLEX* lp_;
	#endif
	#ifdef SOLVER_GUROBI 
	   OPT_GUROBI* lp_;
	#endif

	// n�mero de vari�veis
	int nrVars_;
	// solu��o final
	double* solFinal_;

	// [MAIN METHODS]

	// Construir o modelo
	virtual void buildLP_(void);
	// Set par�metros resolvedor matem�tico
	virtual void setParametrosLP_(void);
	// Resolver problema
	virtual bool solveLP_(void);
	// Carregar solu��o
	virtual void carregarSolucao(void);

	#pragma region  [CRIAR VARI�VEIS]

	// criar vari�veis do modelo
	virtual void criarVariaveis_(void);
	// adicionar vari�vel bin�ria ao modelo. retorna n�mero da coluna.
	int addBinaryVarLP_(double coef, const char* nome);
	// adicionar variavel inteira ao modelo. retorna n�mero da coluna.
	int addIntVarLP_(double coef, int lb, int ub, const char* nome);
	// adicionar variavel linear ao modelo. retorna numero da coluna.
	int addLinVarLP_(double coef, int ub, char* nome);

	#pragma endregion

	#pragma region  [CRIAR RESTRI��ES]

	// turmas incompativeis por horarios
	unordered_map<TurmaHeur*, unordered_set<TurmaHeur*>> turmasIncompHorarios_;
	unordered_map<TurmaHeur*, unordered_set<TurmaHeur*>> turmasCompatHorarios_;
	// turmas incompativeis por deslocamentos
	unordered_map<TurmaHeur*, unordered_set<TurmaHeur*>> turmasIncompativeis_;
	unordered_map<TurmaHeur*, unordered_set<TurmaHeur*>> turmasCompativeis_;

	// criar restri��es do modelo
	virtual void criarRestricoes_(void);
	// NOVO: criar turmas incompativeis recorrendo � gera��o de cliques de incompatibilidade!
	void criarRestrTurmasIncompatCliques_(unordered_map<TurmaHeur*, int> const &turmas, bool deslocavel,
											bool considerarMesmaTurmaOft = true);
	// NOVO: criar turmas incompativeis olhando para os horarios! S� N�O DESLOC�VEL (i.e. Sala)
	void criarRestrTurmasIncompatHorarios_(unordered_map<TurmaHeur*, int> const &turmas, bool deslocavel, bool interjornada = false);

	// adicionar restri��o
	void addRow_(OPT_ROW &row);

	#pragma endregion

	#pragma region [UTIL]

	// verifica se duas turmas s�o incompat�veis
	bool saoIncompativeis_(TurmaHeur* const turmaUm, TurmaHeur* const turmaDois, bool deslocavel);
	// turmas incompativeis por disciplina equivalente?
	bool turmasOfertaIncompativel_(TurmaHeur* const primTurma, TurmaHeur* const secTurma);
	// verifica se incompatibilidade j� foi registada. retorna -1 se incompat�vel, 1 se compat�vel, 0 se n�o souber
	int compatibilidadeJaRegistada_(TurmaHeur* const turmaUm, TurmaHeur* const turmaDois, bool deslocavel);
	int compHorsJaReg_(TurmaHeur* const turmaUm, TurmaHeur* const turmaDois);
	int compDeslocJaReg_(TurmaHeur* const turmaUm, TurmaHeur* const turmaDois);
	// registar compatibilidade
	void registarCompatibilidade_(TurmaHeur* const primTurma, TurmaHeur* const secTurma, bool deslocavel);
	void registarCompHors_(TurmaHeur* const primTurma, TurmaHeur* const secTurma);
	void registarCompDesloc_(TurmaHeur* const primTurma, TurmaHeur* const secTurma);
	// registar compatibilidade
	void registarIncompatibilidade_(TurmaHeur* const primTurma, TurmaHeur* const secTurma, bool deslocavel);
	void registarIncompHors_(TurmaHeur* const primTurma, TurmaHeur* const secTurma);
	void registarIncompDesloc_(TurmaHeur* const primTurma, TurmaHeur* const secTurma);

	#pragma endregion


	#pragma region [Calculador de Cliques para Incompatibilidade de Turmas (algoritmo de Bron�Kerbosch)]

	//  calcular cliques maximos de incompatibilidade com base nas turmas em quest�o
	void gerarMaxCliques(unordered_map<TurmaHeur*, int> const &turmasCols, bool deslocavel, vector<unordered_set<int>*> &cliques,
						bool considerarMmTipoOft = true);

	// criar nodes turmas (ordem crescente de grau)
	void criarNodesTurmas(unordered_map<TurmaHeur*, int> const &turmasCols, bool deslocavel, set<Node*, compNodes> &nodes,
							bool considerarMmTipoOft); 


	#pragma endregion

	#pragma region [Outro gerador de cliques de turmas com incompatibilidade]

	// retorna grupos de turmas incompativeis, baseado na sua sobreposi��o a um horario num dia
	static void getTurmasIncHorarios(unordered_map<TurmaHeur*, int> const &turmasCols, vector<unordered_set<int>> &turmasIncompativeis,
									bool deslocavel, bool interjornada);

	// criar objectos TurmasTemHorarioDia. Deslocamento: OFF
	static void criarTurmasTemHorDia(unordered_set<Calendario*> const &calendarios, map<int, vector<TurmasIncHorarioDia*>> &turmasTemHorarios);

	// juntar turmas que cont�m um determinado horario
	static void checkContemHorDia(unordered_map<TurmaHeur*, int> const &turmasCols, map<int, vector<TurmasIncHorarioDia*>> const &turmasTemHorarios);

	// verificar horarios incompativeis dentro do dia e passar restricoes incompativeis
	static void addGruposTurmasIncompativeis(map<int, vector<TurmasIncHorarioDia*>> const &turmasTemHorarios, vector<unordered_set<int>> &turmasIncompativeis, 
											bool deslocavel, bool interjornada);

	// check add horarios incompativeis
	static bool checkAddHorariosIncomp(TurmasIncHorarioDia* const first, int diaFst, TurmasIncHorarioDia* const second, 
										int diaScd, bool deslocavel, bool interjornada, unordered_set<int> &setCols);

	#pragma endregion

	// add MIP start parcial
	void addMIPStartParcial(int* indices, double* values, const int size);
};


struct equalHorsDia
{
	bool operator() (pair<int, HorarioAula*> const &first, pair<int, HorarioAula*> const &second) const
	{
		if(first.first != second.first)
			return false;

		return first.second->inicioFimIguais(second.second);
	}
};

struct hashHorsDia
{
	size_t operator() (pair<int, HorarioAula*> const &par) const
	{
		size_t hash = par.first + 100*(par.second->getInicioTimeMin());
		return hash;
	}
};


#endif
