#ifndef _SOLUCAO_TATICO_INICIAl_H_
#define _SOLUCAO_TATICO_INICIAl_H_

#include <vector>
#include <map>
#include <set>

#include "GGroup.h"

class VariablePre;
class VariableTatInt;
class VariableTatico;
class Disciplina;
class AlunoDemanda;
class Aula;
class Sala;
class Aluno;
class Campus;

class SolucaoTaticoInicial
{
public:
	SolucaoTaticoInicial();
	~SolucaoTaticoInicial();

   bool existeSolInicial();

   void addAula( int campusId, Disciplina* disciplina, int turma, Aula* aula );

   void addAlunoDem( Campus *campus, Disciplina* disciplina, int turma, AlunoDemanda* alunoDemanda, bool fixar );

   void addAlunosDem( Campus *campus, Disciplina* disciplina, int turma, GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> alunosDemanda, bool fixar );

   void addAlunoTurma( int campusId, Disciplina* disciplina, int turma, Aluno* aluno, bool fixar );

   void removeAula( int campusId, Disciplina* disciplina, int turma, Aula *aula );
   
   std::map< int /*campusId*/, std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
	   LessPtr<Disciplina> > >* getPtMapAulas();

   std::map< int /*campusId*/, std::map< Disciplina*, std::map< int /*turma*/, GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> >,
	   LessPtr<Disciplina> > >* getPtMapTurmasAlsDem();

   GGroup<Aula*, LessPtr<Aula>> getAulas( int campusId, Disciplina* disciplina, int turma );

   Aula* getAula( int campusId, Disciplina* disciplina, int turma, int diaSemana, Sala* sala );

   GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>>* getAlunosDemanda( int campusId, Disciplina* disciplina, int turma );

   int getSolNumTurmas( int campusId, Disciplina *disciplina );

   Sala* getSala( int campusId, Disciplina* disciplina, int turma, bool &fixar );

   int getTurma( Aluno* aluno, int campusId, Disciplina *disciplina, bool &fixar );

   bool existeTurma( int campusId, Disciplina *disciplina, int turma, bool &fixar );

   void addInSol( VariablePre v );
   void addInSol( VariableTatInt v );
   void removeFromSol( VariablePre v );
   void removeFromSol( VariableTatInt v );

   bool isInSol( VariablePre v );
   bool isInSol( VariableTatInt v );
   bool isInSol( VariableTatico v );
   bool criar( VariableTatico v, bool &fixar );
   bool criar( VariablePre x, bool &fixar );

   VariableTatInt inToVariableTatInt( VariableTatico v );

   int getNroAlunosDemandaAtendidos( int cpId ) { return nroAlunosDemandaAtendidos[cpId]; }
   int getNroTurmasAbertas( int cpId ) { return nroTurmasAbertas[cpId]; }
   void calculaNroTurmasAbertas( int campusId );

   void confereCompletudeAulas();
   void confereCorretudeAlocacoes();
   void confereSolucao();
   void imprimeAulas( std::string fileName );
   void imprimeSolucaoX( std::string fileName );
   void imprimeSolucaoS( std::string fileName );
   void imprimeSolucaoNaoAtendimentos( std::string fileName );

private:
      
   std::map< int /*campusId*/, std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
	   LessPtr<Disciplina> > > mapAulas;

   std::map< Aluno*, std::map< int /*campusId*/, std::map< Disciplina*, std::pair< int/*turma*/, bool /*fixar*/ >, 
	   LessPtr<Disciplina> > > , LessPtr<Aluno> > mapAluno_CampusTurmaDisc;

   std::map< int /*campusId*/, std::map< Disciplina*, std::map< int /*turma*/, GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> >,
	   LessPtr<Disciplina> > > mapCampusDiscTurma_AlunosDemanda;

   std::set< VariablePre > preSol_S;
   std::set< VariablePre > preSol_O;
   
   std::set< VariableTatInt > tatSol_S;
   std::set< VariableTatInt > tatSol_X;

   std::map< int /*campusId*/, int > nroAlunosDemandaAtendidos;
   std::map< int /*campusId*/, int > nroTurmasAbertas;

};

#endif
