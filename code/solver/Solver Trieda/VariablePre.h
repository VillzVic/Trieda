#ifndef VARIABLEPRE_H
#define VARIABLEPRE_H

#ifndef WIN32
#include <map>
#else
#include <hash_map>
#endif

#include "ProblemData.h"

// Variables
class VariablePre 
{
public:
   // All variable types
   enum VariableType
   {
      V_ERROR = 0,
      V_PRE_CREDITOS = 1,							// x_{i,d,u,s}
      V_PRE_OFERECIMENTO = 2,						// o_{i,d,s}
      V_PRE_ABERTURA = 3,							// z_{i,d,cp}
      V_PRE_ALUNOS = 4,							    // a_{i,d,s,oft}
      V_PRE_ALOC_ALUNO = 5,				            // b_{i,d,c}
      V_PRE_SLACK_ALOC_ALUNOS_CURSO_INCOMPAT = 6,   // bs_{i,d,c1,c2}
      V_PRE_SLACK_DEMANDA = 7,						// fd_{d,oft} ou fd_{d,a}, depende se a otimização é por bloco ou por aluno
	  V_PRE_SLACK_COMPARTILHAMENTO = 8,				// fc_{i,d,c1,c2}	  
	  V_PRE_LIM_SUP_CREDS_SALA = 9,					// Hs_{cp}

	  V_PRE_ALOCA_ALUNO_TURMA_DISC = 10,			// s_{a,i,d,cp}
	  V_PRE_SLACK_PRIOR_INF = 11,					// fpi_{a}
	  V_PRE_SLACK_PRIOR_SUP = 12,					// fps_{a}
	  V_PRE_FOLGA_ABRE_TURMA_SEQUENCIAL = 13,		// ft_{i,d}
	  V_PRE_TURMAS_COMPART = 14,					// w_{i,d,i',d'}
	  V_PRE_FOLGA_DISTR_ALUNOS = 15,				// fda_{}
      V_PRE_CRED_SALA_F1 = 16,						// xcs1_{s}
      V_PRE_CRED_SALA_F2 = 17,						// xcs2_{s}
      V_PRE_CRED_SALA_F3 = 18,						// xcs3_{s}
      V_PRE_CRED_SALA_F4 = 19,						// xcs4_{s}
	  V_PRE_SLACK_DISC_SALA = 20,					// fs_{d,s}
	  V_PRE_ALUNO_SALA = 21,						// as_{a,s}

	  V_PRE_SLACK_SALA = 22,						// fs_{d,s,oft}
	  V_PRE_ALOC_ALUNO_OFT = 23,					// c_{i,d,s,oft}
	  V_PRE_FORMANDOS_NA_TURMA = 24,				// f_{i,d,cp}
	  V_PRE_TURMA_CALENDARIO = 25,					// v_{i,d,sl,s}
	  V_PRE_NRO_ALUNOS = 26,						// n_{i,d}
	  V_PRE_NRO_ALUNOS_OFT = 27,					// no_{i,d,oft}
	  V_PRE_SLACK_DEMANDA_OFERTA = 28,				// fd_{d,oft}
	  V_PRE_TURMA_SAB = 29,							// sab_{i,d,s}
	  V_PRE_ALUNOS_MESMA_TURMA_PRAT = 30,			// ss_{a1,a2,dp}	  
	  V_PRE_FOLGA_OCUPA_SALA = 32,					// fos_{i,d,cp}
	  V_PRE_OFERECIMENTO_TURNO = 33,				// q_{i,d,s,g}
	  V_PRE_USA_DISCIPLINA = 34,					// k_{d}
	  V_PRE_TURMA_TURNOIES = 35,					// v_{i,d,tt,s}
	  V_PRE_FOLGA_ALUNO_MIN_ATEND1 = 36,				// fmd1_{a}
	  V_PRE_FOLGA_ALUNO_MIN_ATEND2 = 37,				// fmd2_{a}
	  V_PRE_FOLGA_ALUNO_MIN_ATEND3 = 38,				// fmd3_{a}

	  V_PRE_UNID_PERIODO = 39,						// uu_{u,oft,p}
	  V_PRE_SALA_PERIODO = 40,						// us_{s,oft,p}
	  V_PRE_FOLGA_SALA_PERIODO = 41,				// fus_{oft,p}
	  V_PRE_FOLGA_UNID_PERIODO = 42,				// fuu_{oft,p}
	  V_PRE_DISTRIB_ALUNOS = 43,					// da_{cp,d}
	  V_PRE_SLACK_PROF = 44							// fp_{d,g}
   };

   //Constructors
   VariablePre();
   VariablePre( const VariablePre & );

   //Destructor
   virtual ~VariablePre();

   //==================================================
   // GET METHODS 
   //==================================================
   //Return variable type
   VariableType getType() const { return type; }

   // Return value
   double getValue() const { return value; }

   Campus * getCampus() const { return cp; }

   Unidade * getUnidade() const { return u; }

   Sala * getSala() const { return s; }

   ConjuntoSala * getSubCjtSala() const { return tps; }

   int getTurma() const { return i; }

   Curso * getCurso() const { return c; }

   Curso * getCursoIncompat() const { return c_incompat; }

   BlocoCurricular * getBloco() const { return b; }

   Disciplina * getDisciplina() const { return d; }

   int getSubBloco() const { return j; }

   Oferta * getOferta() const { return o; }

   std::pair<Curso*, Curso*> getParCursos() const{ return parCursos;}
   
   std::pair<Oferta*, Oferta*> getParOfertas() const{ return parOft;}
   
   Aluno* getAluno() const { return aluno; }

   int getTurma1() const { return turma1; }
   
   int getTurma2() const { return turma2; }

   Disciplina* getDisciplina1() const { return disc1; }
   
   Disciplina* getDisciplina2() const { return disc2; }

   Calendario* getCalendario() const { return calendario; }

   std::pair<Aluno*, Aluno*> getParAlunos() const { return parAlunos; }

   int getTurno() const { return turno; }
   
   TurnoIES* getTurnoIES() const { return turnoIES; }
   
   int getPeriodo() const { return periodo; }

   

   //==================================================
   // SET METHODS 
   //==================================================
   // Reset variables values
   void reset();

   // Set variable type
   void setType( VariableType t ) { type = t; }

   // Set value
   void setValue( double v ) { value = v; }

   void setCampus( Campus * cpp ) { cp = cpp; }

   void setUnidade( Unidade * uu ) {  u = uu; }

   void setSala( Sala * ss ) {  s = ss; }

   void setSubCjtSala( ConjuntoSala * tpss )  { tps = tpss; }

   void setTurma( int ii ) { i = ii; }

   void setCurso( Curso * cc ) { c = cc; }

   void setCursoIncompat( Curso * cc ) { c_incompat = cc; }

   void setBloco( BlocoCurricular * bb ) {  b = bb; } 

   void setDisciplina( Disciplina * dd ) {  d = dd; }

   void setSubBloco( int jj ) { j = jj; }   

   void setOferta( Oferta * oferta ) { o = oferta; }

   void setParCursos( std::pair<Curso*, Curso*> par ){ parCursos = par;}
     
   void setParOfertas( Oferta* oft1, Oferta* oft2 ){ parOft.first = oft1; parOft.second = oft2;}

   void setAluno( Aluno * a ) { aluno = a; }

   void setTurma1( int i1 ) { turma1 = i1; }
   
   void setTurma2( int i2 ) { turma2 = i2; }

   void setDisciplina1( Disciplina * d1 ) {  disc1 = d1; }

   void setDisciplina2( Disciplina * d2 ) {  disc2 = d2; }

   void setCalendario( Calendario * sl ) {  calendario = sl; }

   void setParAlunos( Aluno* aluno1, Aluno* aluno2 ){ parAlunos.first = aluno1; parAlunos.second = aluno2;}

   void setTurno( int t ){ turno = t; }
   
   void setTurnoIES( TurnoIES* tt ){ turnoIES = tt;}
   
   void setPeriodo( int p ){ periodo = p;}

   

   //==================================================
   // OPERATORS 
   //==================================================
   // Assignment 
   VariablePre & operator = ( const VariablePre & );
   // Less 
   bool operator < ( const VariablePre & ) const;
   // Equals 
   bool operator == ( const VariablePre & ) const;

   // Variable name
   std::string toString();

private:
   VariableType type;
   double value;
   Campus * cp;
   Unidade * u;
   Sala * s;
   ConjuntoSala * tps;
   int i; // Turma
   Curso * c;
   Curso * c_incompat;
   std::pair<Curso*, Curso*> parCursos;
   std::pair<Oferta*, Oferta*> parOft;
   BlocoCurricular * b;
   Disciplina * d;
   int j; // subbloco
   Oferta * o; // oferta
   Aluno *aluno;
   Calendario *calendario;
   int periodo;

   std::pair<Aluno*, Aluno*> parAlunos;
      
   int turma1;          // i1
   Disciplina *disc1;   // d1
   int turma2;          // i2
   Disciplina *disc2;   // d2

   int turno;			// g
   TurnoIES* turnoIES;	// tt
};


/**
//* Type definition for the hash object.
*/
typedef std::map< VariablePre, int > VariablePreHash;


#endif 
