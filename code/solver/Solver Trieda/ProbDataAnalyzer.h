#ifndef _PROB_DATA_ANALYZER_
#define _PROB_DATA_ANALYZER_

#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include <vector>
#include <list>
#include <string>

class ProblemData;
class Campus;
class Unidade;
class Sala;
class TurnoIES;
class HorarioAula;
class Professor;
class Oferta;
class Disciplina;
class Aluno;
class AlunoDemanda;
class Calendario;
class DateTime;

using std::string;


/*
 *	ProbDataAnalyzer: classe abstrata usada para analise dos dados de entrada (ProblemData)
 */
class ProbDataAnalyzer
{
public:
	ProbDataAnalyzer(void);
	~ProbDataAnalyzer(void);

	virtual void foo(void) = 0;	// classe abstrata

	// Set Problem Data (evitar que pointer seja mudado por engano)
	static void setProblemData(ProblemData* const probData);
	

   // -------------------------------------------------------------------
   // Ordenacao de prioridades para ESCOLA

   static void getProfsIntersecMapeados(Disciplina* const disciplina, std::set<Calendario*> const &calends,
		std::set<TurnoIES*> const &turnos, std::unordered_map<TurnoIES*, std::unordered_map<Calendario*, 
			std::unordered_map<Professor*, std::map<int, std::set<DateTime> >>>> &profsIntersec);
   static void getProfsIntersecMapeados(Disciplina* const disciplina, std::unordered_map<TurnoIES*, std::unordered_map<Calendario*, 
			std::unordered_map<Professor*, std::map<int, std::set<DateTime> >>>> &profsIntersec);

   static void getDemandasMapeadas(std::map<string, std::unordered_map<Disciplina*, std::unordered_map<TurnoIES*, 
			std::unordered_map<Calendario*, std::set<AlunoDemanda*>> >>> &mapDiscTurnoCalend);
   static void getDemandasIntersecDisponib(
	std::map<string, std::unordered_map<Disciplina*, std::unordered_map<TurnoIES*, 
		std::unordered_map<Calendario*, std::set<AlunoDemanda*>> >>> const & mapDiscTurnoCalend,
	std::unordered_map<Disciplina*, std::map<Professor*, 
		std::pair< std::map<int, std::set<DateTime>>, std::set<AlunoDemanda*> > >> &mapDiscIntersec,
	std::unordered_map<Professor*, std::map<int, std::map<DateTime, std::unordered_set<Disciplina*>>>> &mapProfDiaDiscIntersec);
   static void orderIntersecDisponibDemandas(
	std::map<string, std::unordered_map<Disciplina*, std::unordered_map<TurnoIES*, 
		std::unordered_map<Calendario*, std::set<AlunoDemanda*>> >>> const &mapDiscDemandaTurnoCalend,
	std::unordered_map<Disciplina*, std::map<Professor*, 
		std::pair< std::map<int, std::set<DateTime>>, std::set<AlunoDemanda*> > >> const &mapDiscIntersec,
	std::map<int, std::unordered_set<Disciplina*>> &orderDemandas);
   static void getAlunosDisponMapeados(
	   std::map<Aluno*, std::map<int, std::map<DateTime, std::map<string,Disciplina*>>>> & mapAlunoDispon,
	   std::map<Aluno*, std::map<int, std::map<DateTime, std::set<Professor*>>>> &mapAlunoDiaDtiProfsDispon);
   static void getUnidadesDisponMapeados(
	std::map<Unidade*, std::map<int, std::map<DateTime, std::map<string,Disciplina*>>>> & mapUnidadesDispon,
	std::map<Unidade*, std::map<string, Aluno*>> & mapUnidadesTurmasDispon,
	std::map<Unidade*, std::map<int, std::map<DateTime, std::set<Professor*>>>> & mapUnidadesDiaDtiProfsDispon); 

   static void getCompartilhProfUnid(
		std::unordered_map<Professor*, std::map<int, std::map<DateTime, std::unordered_set<Disciplina*>>>> const &mapProfDiaDiscIntersec);
   static void getMapProfUnids(
		std::unordered_map<Professor*, std::map<int, std::map<DateTime, std::unordered_set<Disciplina*>>>> const &mapProfDiaDiscIntersec,
		std::unordered_map<Professor*, std::unordered_set<int>> &mapProfUnidsId);
   static void getMapUnidProfs(
		std::unordered_map<Professor*, std::unordered_set<int>> const & mapProfUnidsId,
		std::unordered_map<int, std::unordered_set<Professor*>> & mapUnidIdProfs);
   static void calculaProfsComumParUnid(
	    std::unordered_map<Professor*, std::unordered_set<int>> const & mapProfUnidsId,
		std::unordered_map<int, std::unordered_map<int, std::unordered_set<Professor*>>> &parUnidProfsComuns);
   static void calculaUnidsComumParProf(
		std::unordered_map<int, std::unordered_set<Professor*>> const & mapUnidIdProfs,
		std::unordered_map<Professor*, std::unordered_map<Professor*, std::unordered_set<int>>> &parProfUnidsComuns);
   static int getMaxNrProfsComuns(
		std::unordered_map<int, std::unordered_map<int, std::unordered_set<Professor*>>> const &parUnidProfsComuns);
   static int getNrProfsNaUnid(
		std::unordered_map<int, std::unordered_set<Professor*>> const & mapUnidIdProfs, int unidId);
   static void printGraphviz(
	   std::unordered_map<int, std::unordered_set<Professor*>> const & mapUnidIdProfs,
		std::unordered_map<int, std::unordered_map<int, std::unordered_set<Professor*>>> const &parUnidProfsComuns);
   static void printGraphviz(
		std::unordered_map<Professor*, std::unordered_map<Professor*, std::unordered_set<int>>> const &parProfUnidsComuns);
   static int getColorIdx(int maximum, int value);

   static void printOrderedIntersecDisponibDemandas(
	std::map<int, std::unordered_set<Disciplina*>> const &orderDemandas);	
   static void printFullIntersecDisponibDemandas(
	std::map<string, std::unordered_map<Disciplina*, std::unordered_map<TurnoIES*, 
		std::unordered_map<Calendario*, std::set<AlunoDemanda*>> >>> const & mapDiscTurnoCalend,
	std::unordered_map<Disciplina*, std::map<Professor*, 
		std::pair< std::map<int, std::set<DateTime>>, std::set<AlunoDemanda*> > >> const &mapDiscIntersec);	
   static void printProfDiaDisponibDiscs(
	std::unordered_map<Professor*, std::map<int, std::map<DateTime, std::unordered_set<Disciplina*>>>> const &mapProfDiaDiscIntersec);
   static void printAlunosDisponMapeados(
	std::map<Aluno*, std::map<int, std::map<DateTime, std::map<string,Disciplina*>>>> const & mapAlunoDispon,
	std::map<Aluno*, std::map<int, std::map<DateTime, std::set<Professor*>>>> const &mapAlunoDiaDtiProfsDispon);
   static void printUnidadesDisponMapeados(
	std::map<Unidade*, std::map<int, std::map<DateTime, std::map<string,Disciplina*>>>> const & mapUnidadesDispon,
	std::map<Unidade*, std::map<string, Aluno*>> const & mapUnidadesTurmasDispon,
	std::map<Unidade*, std::map<int, std::map<DateTime, std::set<Professor*>>>> const &mapUnidadesDiaDtiProfsDispon); 

   static void estatisticasDemandasEscola();

   // -------------------------------------------------------------------


private:

	// Dados do Problema: acessiveis de qualquer lugar
	static ProblemData* problemData_;

	
};

#endif