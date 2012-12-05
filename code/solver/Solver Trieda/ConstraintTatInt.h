#ifndef _CONSTRAINT_TAT_INT_H_
#define _CONSTRAINT_TAT_INT_H_

#ifndef WIN32
#include <map>
#else
#include <hash_map>
#endif

#include "ProblemData.h"

/**
* Class which defines a contraint in the LP.
*/
class ConstraintTatInt
{
public:
   /** All constraint types */
   enum ConstraintType
   {
      C_ERROR = 0,
	  C_SALA_HORARIO = 2,							// Restricao 1.2.3
	  C_UNICO_ATEND_TURMA_DISC_DIA = 3,				// Restricao 1.2.4
	  C_DEMANDA_DISC_ALUNO = 4,						// Restricao 1.2.5	  
	  C_TURMA_DISC_DIAS_CONSEC = 5,					// Restricao 1.2.6
      C_MIN_ALUNOS_TURMA = 6,						// Restricao 1.2.7
	  C_DIVISAO_CREDITO = 7,						// Restricao 1.2.8
	  C_COMBINACAO_DIVISAO_CREDITO = 8,				// Restricao 1.2.9
	  C_VAR_ZC = 9,									// Restricao 1.2.10
	  C_DISC_INCOMPATIVEIS = 10,					// Restricao 1.2.11
	  C_ASSOCIA_V_X = 11,
	  C_ALUNO_VARIAS_UNIDADES_DIA = 13,				// Restricao 1.2.14
	  C_ALUNO_DISC_PRATICA_TEORICA = 16,			// Restricao 1.2.17
	  C_DISC_PRATICA_TEORICA = 17,					// Restricao 1.2.17
	  C_ALUNO_HORARIO = 18,							// Restricao 1.2.12	  
	  C_MIN_DIAS_ALUNO = 19,
	  C_MAX_DIAS_ALUNO = 20,
	  C_ALUNOS_CURSOS_INCOMP = 21,
	  C_PROIBE_COMPARTILHAMENTO = 22,
	  C_SALA_UNICA = 23,
	  C_SALA_TURMA = 24,
	  C_ASSOCIA_S_V = 25,
	  C_ABRE_TURMAS_EM_SEQUENCIA = 26,
	  C_ALUNO_CURSO = 27,
	  C_ALUNO_PRIORIDADES_DEMANDA = 28,
	  C_FORMANDOS1 = 29,
	  C_FORMANDOS2 = 30,
	  C_DEMANDA_EQUIV_ALUNO = 31,
	  C_ALUNO_DISC_PRATICA_TEORICA_EQUIV = 32
   };

   /** Default constructor. */
   ConstraintTatInt();

   /** Copy constructor. */
   ConstraintTatInt( const ConstraintTatInt & );

   /** Destructor. */
   virtual ~ConstraintTatInt();

   /** Assign operator. */
   ConstraintTatInt & operator = ( const ConstraintTatInt & );

   /** Less operator. */
   bool operator < ( const ConstraintTatInt & ) const;

   /** Equals operator. */
   bool operator == ( const ConstraintTatInt & ) const;

   /**
   * Returns a string containing an unique name for the constraint.
   * @return a string containing an unique name for the constraint.
   */
   std::string toString();

   //==================================================
   // GET METHODS 
   //==================================================
   // Return constraint type
   ConstraintType getType() const { return type; }

   Campus * getCampus() const { return cp; }
   Unidade * getUnidade() const { return u; }
   Sala * getSala() const { return s; }
   ConjuntoSala * getSubCjtSala() const { return tps; }
   int getTurma() const { return i; }
   Curso * getCurso() const { return c; }
   Curso * getCursoIncompat() const { return c_incompat; }
   BlocoCurricular * getBloco() const { return b; }
   Disciplina* getDisciplina() const { return d; }
   int getSubBloco() const { return j; }
   int getDia() const { return t; }
   Oferta * getOferta() const { return o; }
   Calendario* getSemanaLetiva() const { return sl; }
   std::pair<Curso*, Curso*> getParCursos() const{ return parCursos;}
   ConjuntoSala * getSubCjtSalaCompart() const { return cjtSalaCompart; }
   std::pair<Oferta*, Oferta*> getParOfertas() const { return parOfts; }
   HorarioAula* getHorarioAula () const { return h; } 
   std::pair<Disciplina*, Disciplina*> getParDisciplinas () const { return parDiscs; }
   Aluno* getAluno () const { return aluno; }
   int getTurma1() const { return turma1; }   
   int getTurma2() const { return turma2; }
   Disciplina* getDisciplina1() const { return disc1; }   
   Disciplina* getDisciplina2() const { return disc2; }
   HorarioAula* getHorarioAulaInicial() const { return horarioAulaI; }
   HorarioAula* getHorarioAulaFinal() const { return horarioAulaF; }

   //==================================================
   // SET METHODS 
   //==================================================
   // Reset variables values
   void reset();

   // Set constraint type
   void setType( ConstraintType t ) { type = t; }

   void setCampus( Campus * cpp ) { cp = cpp; }
   void setUnidade( Unidade * uu ) {  u = uu; }
   void setSala( Sala * ss ) {  s = ss; }
   void setSubCjtSala( ConjuntoSala * tpss ) { tps = tpss; }
   void setTurma( int ii ) { i = ii; }
   void setCurso( Curso * cc ) { c = cc; }
   void setCursoIncompat( Curso * cc ) { c_incompat = cc; }
   void setBloco( BlocoCurricular * bb ) {  b = bb; } 
   void setDisciplina( Disciplina * dd ) {  d = dd; }
   void setSubBloco( int jj ) { j = jj; }   
   void setDia( int tt ) {  t = tt; }
   void setOferta( Oferta * oferta ) { o = oferta; }
   void setSemanaLetiva( Calendario* calend ){ sl = calend; }
   void setParCursos( std::pair<Curso*, Curso*> par ){ parCursos = par;}
   void setSubCjtSalaCompart( ConjuntoSala *s ){ cjtSalaCompart = s; }
   void setParOfertas( std::pair<Oferta*, Oferta*> ofts ){ parOfts = ofts; }
   void setHorarioAula ( HorarioAula * ha ) { h = ha; }
   void setParDisciplinas ( std::pair<Disciplina*, Disciplina*> par ) { parDiscs = par; }
   void setAluno ( Aluno *a ) { aluno = a; }
   void setTurma1( int i1 ) { turma1 = i1; }   
   void setTurma2( int i2 ) { turma2 = i2; }
   void setDisciplina1( Disciplina * d1 ) {  disc1 = d1; }
   void setDisciplina2( Disciplina * d2 ) {  disc2 = d2; }
   void setHorarioAulaInicial( HorarioAula* h ) { horarioAulaI = h; }
   void setHorarioAulaFinal( HorarioAula* h ) { horarioAulaF = h; }

private:

   /** Attribute which defines the constraint type of the instance. */
   ConstraintType type;

   Campus *cp;
   Unidade* u;
   Sala* s;
   ConjuntoSala* cjtSalaCompart;

   ConjuntoSala* tps;

   int i; // Turma
   Curso* c;

   Curso* c_incompat;

   std::pair<Curso*, Curso*> parCursos;

   std::pair<Disciplina*, Disciplina*> parDiscs;

   BlocoCurricular* b;
   Disciplina* d;

   int j; // subbloco

   int t; // dia

   Oferta * o; // oferta

   Calendario* sl; // semana letiva (calendario)

   std::pair<Oferta*, Oferta*> parOfts; // par de ofertas (restri��o 1.2.36)

   HorarioAula* h;
   
   Aluno* aluno;

   int turma1;          // i1
   Disciplina *disc1;   // d1
   int turma2;          // i2
   Disciplina *disc2;   // d2

   HorarioAula* horarioAulaI;
   HorarioAula* horarioAulaF;
};

/**
//* Type definition for the hash object.
*/
typedef std::map< ConstraintTatInt, int > ConstraintTatIntHash;

#endif