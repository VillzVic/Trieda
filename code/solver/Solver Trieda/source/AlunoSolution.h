#ifndef _ALUNO_SOLUTION_H_
#define _ALUNO_SOLUTION_H_

#include <iostream>
#include <map>
#include <vector>

#include "GGroup.h"
#include "Trio.h"

using namespace std;

class AlunoDemanda;
class Aluno;
class Disciplina;
class HorarioAula;
class Aula;
class Campus;
class Demanda;
class DateTime;

// Stores output data
class AlunoSolution
{
public:
   AlunoSolution( Aluno* aluno );
   ~AlunoSolution(); 
      
   void constroiMapsDaSolucao();
   void clearMapsDaSolucao();

  // map< int, GGroup< Trio<int,int,Disciplina*> > > 
  //   procuraChoqueDeHorariosAluno( vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > > &opcoes );
      
   void addNroCreditosJaAlocados( int n ) { nroCredsAtendidos_ += n; }
   void setNroCreditosJaAlocados( int n ) { nroCredsAtendidos_ = n; }
   int getNroCreditosJaAlocados() const { return nroCredsAtendidos_; }

   void addNroDiscsAtendidas( int addNroDiscs ) { nroDiscAtendidas_ += addNroDiscs; }
   void setNroDiscsAtendidas( int n ) { nroDiscAtendidas_ = n; }
   int getNroDiscsAtendidas() const { return nroDiscAtendidas_; }
   
   void addChAtendida( int addCh ) { chAtendida_ += addCh; }
   void setChAtendida( int n ) { chAtendida_ = n; }
   int getChAtendida() const { return chAtendida_; }

   void addDiscIdAtendida( int discId ) { discIdAtendidas_.add( discId ); }
   int getNroDiscAtendidas() const { return discIdAtendidas_.size(); }

   void addTurma( AlunoDemanda *ad, Disciplina* discReal, int turma, Campus* campus );
   int getTurma( AlunoDemanda *ad, bool teorica );

   void addDiaDiscAula( int dia, Disciplina* discReal, Aula *aula );
   
   bool operator < ( const AlunoSolution & as ) const
   {
      if ( this->alunoId_ < as.alunoId_ )
         return true;
      return false;
   }

   bool operator > ( const AlunoSolution & as ) const
   {
      return ( !( *this < as ) && !( as == *this ) );
   }

   bool operator == ( const AlunoSolution & as ) const
   {
      return ( !( *this < as ) && !( as < *this ) );
   }   

   void mapAlDemDiscTurmaCpToStream( std::ostream & out );
   void mapDiaDiscAulasToStream( std::ostream & outFile );
   
   map< DateTime /*dti*/, map< DateTime /*dtf*/, map< Disciplina*, Aula*, 
		LessPtr<Disciplina> > > >* getMapAulasNoDia( int dia );

   void procuraChoqueDeHorariosAluno(
		vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > > &opcoes,
		map< int, GGroup< Trio<int,int,Disciplina*> > > &turmas_choque_horarios );

   void procuraChoqueDeHorariosAluno( int dia, DateTime dti, DateTime dtf, GGroup< Aula* > & aulas_choque );

private:
   	
	// -------------------------------
	// Maps de aulas da solução por aluno
	map< int /*dia*/, map< DateTime /*dti*/, map< DateTime /*dtf*/, map< Disciplina*, Aula*, 
		LessPtr<Disciplina> > > > > mapDiaDiscAulas_;
	
    map< AlunoDemanda*, map< Disciplina*, pair<int/*turma*/, Campus*>,		// AlunoDemanda ORIGINAL; Disciplina tanto teórica quanto prática
		LessPtr<Disciplina> >, LessPtr<AlunoDemanda> > mapAlDemDiscTurmaCp_;

	GGroup< int > discIdAtendidas_;				// <discId> , só originais (teóricos)
	int nroCredsAtendidos_;						// número total de créditos em que o aluno foi atendido
	int nroDiscAtendidas_;						// número total de disciplinas em que o aluno foi atendido
	int chAtendida_;							// carga horária total em que o aluno foi atendido
	
	Aluno *aluno_;
	int alunoId_;
};

#endif
