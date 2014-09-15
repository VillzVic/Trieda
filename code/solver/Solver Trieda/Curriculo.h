#ifndef _CURRICULO_H_
#define _CURRICULO_H_

#include "ofbase.h"
#include "GGroup.h"
#include "Disciplina.h"
#include "Calendario.h"
#include <map>
#include <set>
#include <unordered_map>

using namespace std;

//struct GradeDoPeriodo;

class Curriculo :
   public OFBase
{
public:
   Curriculo( void );
   virtual ~Curriculo( void );

   Calendario * calendario;

   virtual void le_arvore( ItemCurriculo & );
   
   map < Disciplina*, int, LessPtr< Disciplina > > disciplinas_periodo;
   map< int, GGroup< Calendario*, LessPtr<Calendario> > > semanasLetivas;
   GGroup< int > periodos;

   map< int, map< int, set<int> > > disciplinasId_correquisitos; // periodo -> discId  ->  <discIdCorreq>
   //map< int, GradeDoPeriodo > grades;

   void setCodigo( std::string s ) { codigo = s; }
   void setSemanaLetivaId( int value ) { semanaLetivaId = value; }

   std::string getCodigo() const { return codigo; }
   int getSemanaLetivaId() const { return semanaLetivaId; }
   int getMaxCreds( int dia );

   int getPeriodo( Disciplina *d );
   int getPeriodoEquiv( Disciplina *d );   

   void refDisciplinaPeriodo( GGroup< Disciplina *, LessPtr< Disciplina > > );

   bool possuiDisciplina( Disciplina *d );
   
   GGroup< Calendario*, LessPtr<Calendario> > retornaSemanasLetivasNoPeriodo( int periodo );

   bool possuiDisciplinaComoEquiv( int deq_id );
   void addIdDiscSubstitutas( int newId ){ ids_discs_substitutas.add(newId); }
   bool possuiDiscEmPeriodo( int periodo );

   bool possuiCorrequisito( int disciplinaId );
   bool possuiCorrequisito( int disciplinaId, int periodo );
   void getCorrequisitos( int disciplinaId, set<int> &correquisitos );
   void getCorrequisitos( int disciplinaId, int periodo, set<int> &correquisitos );
   
private:
   GGroup< std::pair< int, int > > ids_disciplinas_periodo;
   std::string codigo;
   int semanaLetivaId;
   map< int, int > mapMaxCreds;
    
   // Armazena os ids das disciplinas que foram usadas como substitutas para alunos do curriculo.
   // Logo, essa estrutura só tem a chance de ser preenchida após a rodada com equivalencias
   GGroup<int> ids_discs_substitutas;

   std::map< int /*tuplaId*/, GGroup< int/*discId*/ > > ids_discs_correquisito;
};



// GradeDoPeriodo

/*
struct GradeDoPeriodo
{
	const int periodo;
	const int curriculoId;

	// DisciplinaId -> disciplinasId de correquisitos
    map< int, set<int> > disciplina_correquisitos;

	// construtor
	GradeDoPeriodo(int per, int curr) 
		: periodo(per), curriculoId(curr) { } 

	virtual bool operator ==(GradeDoPeriodo const &other) const 
	{ 
		return ( !(*this < other) && !(other < *this) ); 
	}

	virtual bool operator <(GradeDoPeriodo const &other) const 
	{
		if (periodo < other.periodo)
			return true;
		if (periodo > other.periodo)
			return false;
		
		if (curriculoId < other.curriculoId)
			return true; 
		if (curriculoId > other.curriculoId)
			return false;

		return false;
	}
};
*/

//namespace std
//{
//	template<>
//	struct hash<GradeDoPeriodo>
//	{
//		size_t operator() (GradeDoPeriodo const &grade) const
//		{
//			return 1103*grade.periodo + grade.curriculoId;
//		}
//	};
//
//	template<>
//	struct equal_to<GradeDoPeriodo>
//	{
//		bool operator() (GradeDoPeriodo const &first, GradeDoPeriodo const &second) const
//		{
//			return first == second;
//		}
//	};
//}

#endif