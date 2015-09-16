#ifndef _PROBLEM_SOLUTION_H_
#define _PROBLEM_SOLUTION_H_

#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "GGroup.h"
#include "ProblemData.h"

#include "AtendimentoCampus.h"
#include "RestricaoViolada.h"
#include "ProfessorVirtualOutput.h"
#include "Aula.h"
#include "Professor.h"

using namespace std;

class NaoAtendimento;
class AlunoSolution;

// Stores output data
class ProblemSolution
{
public:
	ProblemSolution( bool = true );
    virtual ~ProblemSolution(); 

    void resetProblemSolution();
    AtendimentoCampus* getAddAtendCampus(int id_cp);

    RestricaoVioladaGroup * getFolgas() const { return this->folgas; }
    GGroup< AtendimentoCampus * > * atendimento_campus;

    GGroup< ProfessorVirtualOutput * > * professores_virtuais;
    GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > > * alunosDemanda;
    GGroup< NaoAtendimento *, LessPtr< NaoAtendimento > > * nao_atendimentos;
    bool modoOtmTatico;
    int getIdAtendimentos() { return this->idsAtendimentos++; }

    void setCenarioId( int value ) { this->cenarioId = value; }
    int getCenarioId() const { return this->cenarioId; }

    ProfessorVirtualOutput* getProfVirtualOutput( int id );
   
	void getMapsDaSolucao(
		unordered_map<Sala*, unordered_map<Disciplina*, unordered_map<int, 
			std::pair<Professor*, unordered_set<Aluno*>> >>> & solTurmaProfAlunos,
		unordered_map<Campus*, unordered_map<Disciplina*, unordered_map<int,
			unordered_map<int, set<DateTime>> >>> & solCpDiscTurmaDiaDti,
		bool somenteFixado = true );

    void constroiMapsDaSolucao();
	void preencheQuantChProfs();
    void clearMapsDaSolucao();
	void gatherIndicadores();

    map< Sala*, vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > >, LessPtr<Sala> >
	   procuraCombinacaoLivreEmSalas( Disciplina *disciplina, TurnoIES* turno, int campusId );

    vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > >
	   procuraCombinacaoLivreNaSala( Disciplina *disciplina, TurnoIES* turno, Sala* sala );

    void procuraOpcoesSemChoque( 
		const map< int /*opcao*/, map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > > &sem_choques, 
		const map< TurnoIES*, GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>>, LessPtr<TurnoIES> > &mapAlsDemNaoAtend, 
		map< int /*opção*/, GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> > &mapOpcaoAlunosSemChoque );

    void verificaPossivelNovaTurma( NaoAtendimento *naoAtend, Disciplina *disc, AlunoDemanda *ad, int campusId,
	   const map< TurnoIES*, GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>>, LessPtr<TurnoIES> > &mapAlsDemNaoAtend );

    void verificaNrDiscSimultVirtual();
    void verificaNaoAtendimentosTaticos();
    void verificaUsoDeProfsVirtuais();
    void computaMotivos( bool motivoNaoAtend, bool motivoUsoPV );
    void imprimeIndicadores();

    int retornaTurmaDiscAluno( AlunoDemanda* alunoDemanda, bool teorica );

   
    /* Referencia o objeto AlunoSolution correpondente ao aluno. Caso ainda não exista, cria.
     * Retorna true se o objeto foi criado, ou false caso já existia.
    **/
    bool getAlunoSolution( Aluno* aluno, AlunoSolution *& alunoSolution );

    /* Retorna pointer para o AlunoSolution do aluno procurado.
	   Caso não exista, retorna nullptr.
    **/
    AlunoSolution* getAlunoSolution( Aluno* aluno );

   
	// -------------------------------
	// Get totais de alunos-demandas atendidos e não atendidos
    int getNroTotalAlunoDemandaP1() const { return nroTotalAlunoDemandaP1; }
	int getNroAlunoDemAtendP1() const { return nroAlunoDemAtendP1; }
	int getNroAlunoDemNaoAtendP1() const { return nroAlunoDemNaoAtendP1; }
	int getNroAlunoDemAtendP2() const { return nroAlunoDemAtendP2; }
	int getNroAlunoDemAtendP1P2() const { return nroAlunoDemAtendP1P2; }
	int getNroTotalAlunoDemFormandosP1() const { return nroTotalAlunoDemFormandosP1; }
	int getNroTotalAlunoDemCalourosP1() const { return nroTotalAlunoDemCalourosP1; }
	int getNroTotalAlunoDemFormandosAtendP1() const { return nroTotalAlunoDemFormandosAtendP1; }
	int getNroTotalAlunoDemCalourosAtendP1() const { return nroTotalAlunoDemCalourosAtendP1; }
	
	// -------------------------------
	// Set totais de alunos-demandas atendidos e não atendidos
    void setNroTotalAlunoDemandaP1( int n ) { nroTotalAlunoDemandaP1 = n; }
	void setNroAlunoDemAtendP1( int n ) { nroAlunoDemAtendP1 = n; }
	void setNroAlunoDemNaoAtendP1( int n ) { nroAlunoDemNaoAtendP1 = n; }
	void setNroAlunoDemAtendP2( int n ) { nroAlunoDemAtendP2 = n; }
	void setNroAlunoDemAtendP1P2( int n ) { nroAlunoDemAtendP1P2 = n; }
	void setNroTotalAlunoDemFormandosP1( int n ) { nroTotalAlunoDemFormandosP1 = n; }
	void setNroTotalAlunoDemCalourosP1( int n ) { nroTotalAlunoDemCalourosP1 = n; }
	void setNroTotalAlunoDemFormandosAtendP1( int n ) { nroTotalAlunoDemFormandosAtendP1 = n; }
	void setNroTotalAlunoDemCalourosAtendP1( int n ) { nroTotalAlunoDemCalourosAtendP1 = n; }

   // Imprime maps da solução
   void imprimeMapsDaSolucao();
   void imprimeMapSolDiscTurmaDiaAula();
   void imprimeMapSolAlunoDiaDiscAulas();
   void imprimeMapAlunoDiscTurmaCp();
   void imprimeMapSalaDiaHorariosVagos();
   void imprimeMapSolTurmaProfVirtualDiaAula();
   void imprimeMapSolProfRealDiaHorarios();
   void imprimeQuantChProfs();
   void imprimeNrDiscSimultVirtual();
   
   // ler solução de um arquivo
   static ProblemSolution* lerSolucao(char* const filePath, bool modoTatico = false);
   

private:
   
	RestricaoVioladaGroup * folgas;
	int idsAtendimentos;
	int cenarioId;
	
	// -------------------------------
	// Totais de alunos-demandas atendidos e não atendidos
	int nroTotalAlunoDemandaP1;
	int nroAlunoDemAtendP1;
	int nroAlunoDemNaoAtendP1;
	int nroAlunoDemAtendP2;
	int nroAlunoDemAtendP1P2;
	int nroTotalAlunoDemFormandosP1;
	int nroTotalAlunoDemCalourosP1;
	int nroTotalAlunoDemFormandosAtendP1;
	int nroTotalAlunoDemCalourosAtendP1;
	
	int nrMaxDiscSimult_;
    map<int, unordered_set<Disciplina*>> mapNrDiscSimult;

	// -------------------------------
	// Map de aulas da solução por turma
	map< int /*campusId*/, map< Disciplina*, map< int /*turma*/, std::pair< std::map<int /*dia*/, Aula*>,
		GGroup<int /*alDem*/> > >, LessPtr<Disciplina> > > mapSolDiscTurmaDiaAula_;

	map< int /*campusId*/, map< Disciplina*, map< int /*turma*/, GGroup<Curso*> >, 
		LessPtr<Disciplina> > > mapSolTurmaCursos_;

	// -------------------------------
	// Maps de aulas da solução por aluno
		
	map< Aluno*, AlunoSolution*, LessPtr<Aluno> > mapAlunoSolution_;
	
	// -------------------------------
	// Map de aulas da solução por sala
	map< Sala*, map<int /*dia*/, std::map<DateTime /*dti*/, std::map<DateTime /*dtf*/, 
		GGroup<HorarioAula*, LessPtr<HorarioAula>> > > >, LessPtr<Sala> > mapSalaDiaHorariosVagos_;

	// -------------------------------
	// Maps de aulas da solução com professor
	map< int /*campusId*/, map< Disciplina*, map< int /*turma*/, map< ProfessorVirtualOutput*, map< int /*dia*/, Aula* >, 
		LessPtr<ProfessorVirtualOutput> > >, LessPtr<Disciplina> > > mapSolTurmaProfVirtualDiaAula_;
	
	map<int/*campusId*/, map<Disciplina*, map<int/*turma*/, int/*pvId*/>>> mapCpDiscTurmaPVId_;

	std::map< Professor*, std::map<int/*dia*/, vector<HorarioDia*>>, LessPtr<Professor> > mapSolProfRealDiaHorarios_;

	std::map<int/*idProf*/, pair<int/*carga horaria alocada*/, int /*nro creds alocados*/>> quantChProfs_;

   // ler solucao de um arquivo
   static ProblemSolution* lerTexto(char* const buffer);
};

std::ostream & operator << ( std::ostream &, ProblemSolution & );

std::istream& operator >> ( std::istream &file, ProblemSolution* const &ptrProbSolution);

#endif
