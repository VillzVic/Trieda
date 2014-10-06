#ifndef _UTIL_HEUR_H_
#define _UTIL_HEUR_H_

#include "GGroup.h"
#include <stdlib.h> 
#include <stddef.h>
#include <set>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <vector>
#include <tuple>
#include <array>
#include <fstream>
#include <boost\functional\hash.hpp>

using namespace std;

// Use to convert bytes to MB
#define DIV 1048576

class AulaHeur;
class HorarioAula;
class ProfessorHeur;
class SolucaoHeur;
class AlunoHeur;
class TurmaHeur;
class DadosHeuristica;
class TurmaPotencial;
class HorarioAula;
class TurnoIES;
class DateTime;
class TurmaPotencial;
class SalaHeur;
class AlunoDemanda;
class Calendario;
class Disciplina;
class Aluno;
class Sala;
class Professor;
class Curso;
class OfertaDisciplina;

// Alias
typedef GGroup<HorarioAula*, LessPtr<HorarioAula>> gGroupHorarios;


class UtilHeur
{
public:
	UtilHeur(void);
	~UtilHeur(void);

	virtual void foo() = 0;	// virtual function to make this an abstract class

	static int idAulas;

	// calendarios / turnos / horarios
	static bool calendarioAbrange(Calendario* const calendario, int dia, HorarioAula* const horario);
	static bool calendarioAbrange(Calendario* const calendario, int dia, set<HorarioAula*> const &horarios);
	static bool turnoAbrange(TurnoIES* const turno, int dia, HorarioAula* const horario);
	static bool horariosAbrangem(gGroupHorarios const &horarios, int dia, HorarioAula* const horario);
	// verifica se o calendario tem algum horario do turno
	static bool calendarioContemAnyHorarioTurno(Calendario* const calendario, TurnoIES* const turno);

	// guarda as aulas possiveis para cada calendario, dia, nr creditos
	// [calendario -> [dia -> [nr creds -> [aulas.ids]]]
	static unordered_map<Calendario*, unordered_map<int, unordered_map<int, vector<AulaHeur*>>>> aulasCalendario;
	static void initAulasCalendario(void);
	static void getAulasCalDiaCreds(Calendario* const calendario, int dia, int nrCreds, vector<AulaHeur*> &aulas);

	// check formando / co-req
	static void temFormandoCoReq(unordered_set<AlunoDemanda*> const &setAlunos, bool &temFormando, bool &temCoReq);
	static void temFormandoCoReq(OfertaDisciplina* const &ofertaDisc, unordered_set<AlunoHeur*> const &setAlunos, bool &temFormando, bool &temCoReq);

	static const std::string currentDateTime();
	static const std::string horarioAulaToString(HorarioAula const &horario);
	// calcula o intervalo entre dois horários em minutos! se se intersectam retorna -1
	static int intervaloHorarios(const HorarioAula* const anterior, const HorarioAula* const posterior);
	// total créditos da divisão
	static int totalCreds(std::vector<std::pair<int, int>> const &divisao);

	// get id de aula
	static AulaHeur* saveAula(AulaHeur const &aula);
	static AulaHeur* getAula(int id);
	static int nrAulasReg(void) { return aulasHeurId_.size(); }

	// devolve novas aulas com nova unidade
	static void aulasUnidade(unordered_map<int, AulaHeur*> const &aulas, int unidadeId, unordered_map<int, AulaHeur*> &newAulas);
	static void aulasCampusUnid(vector<AulaHeur*> const &aulas, int campusId, int unidadeId, vector<AulaHeur*> &newAulas);

	// imprimir memória a ser usada actualmente pelo processo
	static void printMemoryUsage(void);
	// imprime uma divisão semanal de créditos
	static void printDivisao(std::vector< std::pair< int, int> > const &divisao, int lvl, bool total = false);
	static void printDivisao(unordered_map<int, int> const &divisao, int lvl, bool total = false);
	// imprime array
	static void printArray(int* arr, int size, int lvl);

	// get valor de demanda
	static double getValDemPrior(int prioridade);

	// dia anterior / posterior
	static int diaAnterior(int dia) { if(dia == 2) { return 8; } else return (dia-1); }
	static int diaPosterior(int dia) { if(dia == 8) { return 2; } else return (dia+1); }

	// retorna as horas do dia como um double
	static double horasDouble(DateTime const &time);
	// imprime as horas do dia
	static std::string strHoras(DateTime const &time);
	static void printHoras(DateTime const &time, int lvl);

	// string to char*
	static char* stringToChar(std::string str);

	// numero de combinacoes de horarios de uma divisao num calendario
	static long nrCombHorarios(Calendario* const calendario, std::vector<std::pair<int, int>> &divisao);
	// numero de combinacoes de horarios de uma divisao num turno
	static long nrCombHorarios(TurnoIES* const turno, std::vector<std::pair<int, int>> &divisao);
	// numero de combinacoes possiveis aulas
	static int nrCombAulas(const unordered_map<int, std::vector<AulaHeur*>> &possiveisAulas);

	#pragma region [VERIFICAÇÃO]

	// verifica se a aula tem creditos suficientes
	static void checkCreds(AulaHeur* const aula, int creditos, std::string metodo);
	// verifica se id é negativo
	static void checkAulaId(int id, std::string metodo);
	// verificar se não há turmas potenciais
	static bool turmasPotenciaisVazia(unordered_map<int, set<TurmaPotencial*>> const &turmasPotenciais);

	// unir sets turmas potenciais
	static void unirSetsTurmasPot(set<const TurmaPotencial* const> const &set1, set<const TurmaPotencial* const> const &set2, set<const TurmaPotencial* const> &final);

	// check divisão disciplina
	static bool checkDivisaoDisc(std::vector<std::pair<int, int>> const &divisao, Disciplina* const disciplina);
	// check aulas possíveis
	static void checkAulasPoss(std::vector<std::pair<int, int>> const &divisao, unordered_map<int, std::vector<AulaHeur*>> const &aulas);
	// nr creditos total das aulas
	static int nrCredsAulas( unordered_map< int, AulaHeur* > const &aulas);

	#pragma endregion

	#pragma region [ABRIDOR TURMAS]

	// com base no calendário e na divisão selecciona as aulas possiveis
	static bool getAulasPossiveis(Calendario * const calendario, std::vector< std::pair< int, int> > const &divisao, int campusId, int unidadeId,
							unordered_map<int, std::vector<AulaHeur*>> &aulas);
	// retorna as aulas com 'nrCreditos' possiveis num determinado dia do calendario
	static bool getAulasPossiveisDia(int dia, int nrCreditos, Calendario * const calendario, int campusId, int unidadeId, 
								std::vector<AulaHeur*>& result);
	// com base no turno e na divisão selecciona as aulas possiveis
	static bool getAulasPossiveis(TurnoIES * const turno, int calendarioId, std::vector< std::pair< int, int> > const &divisao, int campusId, int unidadeId,
							unordered_map<int, std::vector<AulaHeur*>> &aulas);
	// retorna as aulas com 'nrCreditos' possiveis num determinado dia do calendario
	static bool getAulasPossiveisDia(int dia, int nrCreditos, TurnoIES * const turno, int calendarioId, int campusId, int unidadeId, 
								std::vector<AulaHeur*>& result);

	
	// função auxiliar usada para conseguir obter as várias combinações de aulas possiveis para um calendário e uma divisão
	static bool incrementArray(int arrayComb[9], const unordered_map<int, std::vector<AulaHeur*>> &possiveisAulas);
	// retorna um conjunto de aulas para um calendário e uma divisão
	static void getAulasFromArray(int arrayComb[9], const unordered_map<int, std::vector<AulaHeur*>> &possiveisAulas, unordered_map<int, AulaHeur*> &result);

	// analisa todas as combinações de créditos e obtem o minimo de maximo de creditos por dia.
	static void credsPossiveisDias(vector<vector<pair<int,int>>> const &divisoesCreditos, unordered_map<int, unordered_set<int>> &credsPossiveisDias);

	template<typename T, typename H>
	static void intersectSets(unordered_set<T,H> const &setUm, unordered_set<T, H> const &setDois, unordered_set<T, H> &result)
	{
		// procurar usando o menor
		if( setDois.size() < setUm.size() )	
			return intersectSets<T, H>(setDois, setUm, result);

		result.clear();

		for(auto it = setUm.cbegin(); it != setUm.cend(); ++it)
		{
			if(setDois.find(*it) != setDois.end())
				result.insert(*it);
		}
	}

	// intersecta sets
	template<typename T>
	static void intersectSets(unordered_set<T, std::less<T>> const &setUm, unordered_set<T, std::less<T>> const &setDois, 
							unordered_set<T, std::less<T>> &result)
	{
		intersectSets<T, std::less<T>>(setUm, setDois, result);
	}

	// une sets
	template<typename T, typename H>
	static void unionSets(unordered_set<T, H> const &setUm, unordered_set<T, H> const &setDois, unordered_set<T, H> &result)
	{
		result.clear();
		result.insert(setUm.begin(), setUm.end());
		result.insert(setDois.begin(), setDois.end());
	}

	// intersecta sets de alunos
	static void intersectSetsAlunos(OfertaDisciplina* const ofertaDisc, unordered_set<AlunoHeur*> const &setUm, 
									unordered_set<AlunoHeur*> const &setDois,  unordered_set<AlunoHeur*> &result, 
									bool &temFormando, bool &temCoReq);

	#pragma endregion

	// retorna um conjunto de alunosheur associado a um set de alunodemanda
	static void getSetAlunosFromSetAlunoDemanda(set<AlunoDemanda *> const &alunosDemanda, unordered_set<AlunoHeur*> &alunos);
	// retorna um conjunto de alunosheur ordenado associado a um set de alunodemanda
	static void getSetAlunosFromSetAlunoDemanda(set<AlunoDemanda *> const &alunosDemanda, set<AlunoHeur*> &alunos);

	// print nr creditos e nr divisoes
	static void printCredsNrDivs(Disciplina* const disciplina, int nrDiv, int lvl);

	// verifica se um calendario abrange outro, i.e. se tem todos os horarios do outro (inicio e fim igual)
	static bool calendarioDomina(Calendario* const calendario, Calendario* const dominado);

	// procura a demanda de um aluno quando ela não é encontrada em alunosDemandaTotal
	static AlunoDemanda* findDemanda(Aluno* const aluno, Disciplina* const disciplina);

	// get disciplinas de um curso
	static void getDisciplinasCurso(Curso* const curso, unordered_set<Disciplina*> &disciplinas, bool equiv = false);

	// insere um elemento num vetor numa posição randomica
	template<typename T>
	static void insertRandomIndex(vector<T> &vetor, T const &obj)
	{
		if(vetor.size() > 0)
		{
			int rdIdx = rand() % vetor.size();
			auto it = next(vetor.begin(), rdIdx);
			vetor.insert(it, obj);
		}
		else
			vetor.push_back(obj);
	}

private:
	// mapa para guardar aulas (diminuir uso de memória)
	static unordered_map<int, AulaHeur> aulasHeurId_;
	static unordered_map<size_t, int> hashesToIdAulasReg_;

	// mapa para guardar dominancias de calendarios
	static unordered_map<int, unordered_set<int>> dominanciaCalendarios_;	// [calendario.id -> [dominados.id]]
	// mapa para relacionar calendarios com turnos contidos nele
	static unordered_map<int, unordered_set<int>> calendarioTurnos_;		// [calendario.id -> [turno.id]]
	// mapa para relacionar calendarios com turnos nao contidos nele
	static unordered_map<int, unordered_set<int>> calTurnosNot_;			// [calendario.id -> [turno.id]]

	// acelerar verificacao de horarios. turno -> dia -> horarioaula.id
	static unordered_map<int, unordered_map<int, unordered_set<int>>> turnosAbrangem;
	static unordered_map<int, unordered_map<int, unordered_set<int>>> turnosNaoAbrangem;
	// acelerar verificacao de horarios. calendario.id -> dia -> horarioaula.id
	static unordered_map<int, unordered_map<int, unordered_set<int>>> calendariosAbrangem;
	static unordered_map<int, unordered_map<int, unordered_set<int>>> calendariosNaoAbrangem;

	// (<0) -> nao abrange / (>0) -> abrange / (==0) unknown
	static int checkRegAbrange(int id, int dia, HorarioAula* const horario,
								unordered_map<int, unordered_map<int, unordered_set<int>>> const &registoAbrange,
								unordered_map<int, unordered_map<int, unordered_set<int>>> const &registoNaoAbrange);
	static void regAbrangencia(int id, int dia, HorarioAula* const horario, 
							unordered_map<int, unordered_map<int, unordered_set<int>>> &registo);

	// retorna as aulas possiveis para um número de creditos e um conjunto de horarios
	static bool getAulasPossiveisDiaHors(int dia, int nrCreditos, gGroupHorarios &horariosDia, int calendarioId, int campusId, int unidadeId, 
								std::vector<AulaHeur*>& result);

	// add aula. retorna id
	static AulaHeur* addAula_(AulaHeur const &aula, size_t hash);
	
};

#pragma region [ALIASES de ESTRUTURAS]

typedef GGroup< Aluno *, LessPtr< Aluno > > gGroupAlunos;
typedef GGroup< Sala *, LessPtr< Sala > > gGroupSalas;
typedef GGroup< Professor *, LessPtr< Professor > > gGroupProfs;
typedef GGroup< Calendario*, LessPtr<Calendario> >	gGroupCalendarios;
typedef std::map< Professor *, GGroup< Disciplina *,LessPtr< Disciplina > >, LessPtr< Professor > > mapProfDisc;

#pragma endregion

#pragma region [HASHERS]

template<class T> 
struct hasher
{
	size_t operator()(T* const &n) const 
	{ 
		return boost::hash_value(n);
	}
	size_t operator()(T &obj) const 
	{ 
		return boost::hash_value(obj);
	}
};

#pragma endregion


#endif

