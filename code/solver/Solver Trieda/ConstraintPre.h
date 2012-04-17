#ifndef _CONSTRAINTPRE_H_
#define _CONSTRAINTPRE_H_

#ifndef WIN32
#include <map>
#else
#include <hash_map>
#endif

#include "ProblemData.h"

/**
* Class which defines a contraint in the LP.
*/
class ConstraintPre
{
public:
   /** All constraint types */
   enum ConstraintType
   {
      C_ERROR = 0,
      C_PRE_CARGA_HORARIA			= 1,			// Restricao 1.1
	  C_PRE_MAX_CRED_SALA			= 2,			// Restricao 1.2
	  C_PRE_VAR_O					= 3,			// Restricao 1.3
	  C_EVITA_MUDANCA_DE_SALA		= 4,			// Restricao 1.4
	  C_PRE_CAP_ALOC_DEM_DISC		= 5,			// Restricao 1.5
	  C_ALUNO_OFT_DISC				= 6,			// Restricao 1.6
	  C_CAP_SALA					= 7,			// Restricao 1.7
	  C_PRE_ALUNOS_CURSOS_INCOMP	= 8,			// Restricao 1.8
	  C_PRE_PROIBE_COMPARTILHAMENTO	= 9,			// Restricao 1.9
	  C_PRE_ATIVA_Z					= 10,			// Restricao 1.10
	  C_PRE_EVITA_TURMA_DISC_CAMP_D		= 11,		// Restricao 1.11
	  C_PRE_LIMITA_ABERTURA_TURMAS		= 12,		// Restricao 1.12
	  C_PRE_ABRE_TURMAS_EM_SEQUENCIA	= 13,		// Restricao 1.13
	  C_PRE_TURMA_MESMA_DISC_SALA_DIF = 14,			// Restricao 1.14
	  C_PRE_LIM_SUP_CREDS_SALA		= 15,			// Restricao 1.15
	  C_PRE_ATIVA_C					= 16,			// Restricao 1.16

	  // Restrição só usada para o modelo Tatico (sem aluno):

	  C_PRE_FIXA_NAO_COMPARTILHAMENTO = 17,			// Restricao 1.17

	  // Restrição só usada para o modelo Tatico-Aluno:

	  C_ATENDIMENTO_ALUNO = 18,						// Restricao 1.18
	  C_ALUNO_UNICA_TURMA_DISC = 19,				// Restricao 1.19
	  C_ALUNO_DISC_PRATICA_TEORICA = 20,			// Restricao 1.20
	  C_ALUNO_PRIORIDADES_DEMANDA = 21				// Restricao 1.21

   };

   /** Default constructor. */
   ConstraintPre();

   /** Copy constructor. */
   ConstraintPre( const ConstraintPre & );

   /** Destructor. */
   virtual ~ConstraintPre();

   /** Assign operator. */
   ConstraintPre & operator = ( const ConstraintPre & );

   /** Less operator. */
   bool operator < ( const ConstraintPre & ) const;

   /** Equals operator. */
   bool operator == ( const ConstraintPre & ) const;

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
   Oferta * getOferta() const { return o; }
   Calendario* getSemanaLetiva() const { return sl; }
   std::pair<Curso*, Curso*> getParCursos() const{ return parCursos;}
   ConjuntoSala * getSubCjtSalaCompart() const { return cjtSalaCompart; }
   std::pair<Oferta*, Oferta*> getParOfertas() const { return parOfts; }
   Aluno* getAluno() const { return aluno; } 

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
   void setOferta( Oferta * oferta ) { o = oferta; }
   void setSemanaLetiva( Calendario* calend ){ sl = calend; }
   void setParCursos( std::pair<Curso*, Curso*> par ){ parCursos = par;}
   void setSubCjtSalaCompart( ConjuntoSala *s ){ cjtSalaCompart = s; }
   void setParOfertas( std::pair<Oferta*, Oferta*> ofts ){ parOfts = ofts; }
   void setAluno( Aluno* a ){ aluno = a; }

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

   BlocoCurricular* b;
   Disciplina* d;

   int j; // subbloco

   Oferta * o; // oferta

   Calendario* sl; // semana letiva (calendario)

   std::pair<Oferta*, Oferta*> parOfts; // par de ofertas (restrição 1.2.36)

   Aluno* aluno;

};

/**
//* Type definition for the hash object.
*/
typedef std::map< ConstraintPre, int > ConstraintPreHash;


#endif
