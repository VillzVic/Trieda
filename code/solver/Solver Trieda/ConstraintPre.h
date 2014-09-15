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

	  // Restrição só usada para o modelo Tatico BlocoCurr (sem aluno):

	  C_PRE_FIXA_NAO_COMPARTILHAMENTO = 17,			// Restricao 1.17
	  C_PRE_TURMA_MESMA_DISC_OFT_SALA_DIF = 18,

	  // Restrição só usada para o modelo Tatico-Aluno:

	  C_ATENDIMENTO_ALUNO = 19,						// Restricao 1.18
	  C_ALUNO_UNICA_TURMA_DISC = 20,				// Restricao 1.19
	  C_ALUNO_DISC_PRATICA_TEORICA = 21,			// Restricao 1.20
	  C_ALUNO_PRIORIDADES_DEMANDA = 22,				// Restricao 1.21

	  C_PRE_EVITA_SOBREPOS_SALA_DIA_ALUNO = 23,
	  C_PRE_ATIVA_VAR_COMPART_TURMA = 24,
	  C_PRE_MAX_CREDS_ALUNO_DIA = 25,

	  C_PRE_DISTR_ALUNOS = 26,
      C_PRE_SOMA_CRED_SALA = 27,
      C_PRE_MAX_CREDS_ALUNO_SEMANA = 28,
	  C_PRE_ALUNO_SALA = 29,
	  C_PRE_FORMANDOS1 = 30,
	  C_PRE_FORMANDOS2 = 31,
	  C_PRE_TURMA_CALENDARIOS_INTERSECAO = 32,
	  C_PRE_SET_V_1 = 33,
	  C_PRE_SET_V_2 = 34,
	  C_PRE_ALUNO_EM_TURMA_ABERTA = 35,
	  C_PRE_NRO_ALUNOS_POR_TURMA = 36,
	  C_PRE_DISC_PRATICA_TEORICA_OFERTA = 37,
	  C_PRE_ALOC_DEM_DISC_OFERTA = 38,
     C_PRE_ALOC_TURMA_SAB = 39,
     C_PRE_MAX_CRED_SAB = 40,
	 C_PRE_FORCA_MIN_ATEND = 41,
	 C_ALUNO_DISC_PRATICA_TEORICA_1x1 = 42,
	 C_ALUNO_DISC_PRATICA_TEORICA_1xN = 43,
	 C_ALUNOS_MESMA_TURMA_PRAT = 44,
	 C_PRE_ALOC_MIN_ALUNO = 45,
	 C_PRE_FOLGA_OCUPACAO_SALA = 46,
	 C_PRE_MAX_CRED_SALA_TURNO = 47,
	 C_PRE_SET_TURNO_POR_TURMA = 48,
	 C_PRE_LIMITA_TURNO_POR_TURMA = 49,
	 C_PRE_TURNO_VIAVEL_POR_TURMA = 50,
	 C_PRE_MAX_CRED_ALUNO_TURNO = 51,
	 C_PRE_TEMPO_MAX_ALUNO_SAB = 52,
	 C_PRE_TEMPO_MAX_ALUNO_TURNO = 53,
	 C_PRE_ALUNOS_TURNO = 54,
	 C_PRE_USA_DISC = 55,
	 C_PRE_UNID_PERIODO = 56,
	 C_PRE_USA_UNID_PERIODO = 57,
	 C_PRE_USA_SALA_PERIODO = 58,
	 C_PRE_SALA_PERIODO = 59,
	 C_PRE_DISTRIB_ALUNOS = 60,
	 C_PRE_DISPONIB_PROF = 61
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
   std::pair<Calendario*, Calendario*> getParCalendarios() const{ return parSl;}
   std::pair<Curso*, Curso*> getParCursos() const{ return parCursos;}
   ConjuntoSala * getSubCjtSalaCompart() const { return cjtSalaCompart; }
   std::pair<Oferta*, Oferta*> getParOfertas() const { return parOfts; }
   Aluno* getAluno() const { return aluno; } 
   int getDia() const { return dia; }    
   int getTurma1() const { return turma1; }   
   int getTurma2() const { return turma2; }
   Disciplina* getDisciplina1() const { return disc1; }   
   Disciplina* getDisciplina2() const { return disc2; }
   std::pair<Aluno*, Aluno*> getParAlunos() const { return parAlunos; }
   int getTurno() const { return turno; }
   TurnoIES* getTurnoIES() const { return turnoIES; }
   std::pair<TurnoIES*,TurnoIES*> getParTurnosIES() const { return parTurnosIES; }
   int getPeriodo() const { return periodo; }

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
   void setParCalendarios( Calendario* calend1, Calendario* calend2 ){ parSl = std::make_pair(calend1,calend2); }
   void setParCursos( std::pair<Curso*, Curso*> par ){ parCursos = par;}
   void setSubCjtSalaCompart( ConjuntoSala *s ){ cjtSalaCompart = s; }
   void setParOfertas( std::pair<Oferta*, Oferta*> ofts ){ parOfts = ofts; }
   void setAluno( Aluno* a ){ aluno = a; }
   void setDia( int t ){ dia = t; }
   void setTurma1( int i1 ) { turma1 = i1; }   
   void setTurma2( int i2 ) { turma2 = i2; }
   void setDisciplina1( Disciplina * d1 ) {  disc1 = d1; }
   void setDisciplina2( Disciplina * d2 ) {  disc2 = d2; }
   void setParAlunos( std::pair<Aluno*, Aluno*> a ){ parAlunos = a; }
   void setTurno( int t ){ turno = t; }
   void setTurnoIES( TurnoIES *tt ){ turnoIES = tt; }
   void setParTurnosIES( TurnoIES *tt1, TurnoIES *tt2 ){ parTurnosIES.first = tt1; parTurnosIES.second = tt2; }
   void setPeriodo( int p ){ periodo = p; }

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
   std::pair<Calendario*, Calendario*> parSl; // semana letiva (calendario)
   std::pair<Oferta*, Oferta*> parOfts; // par de ofertas (restrição 1.2.36)
   Aluno* aluno;
   int dia;
   int turma1;          // i1
   Disciplina *disc1;   // d1
   int turma2;          // i2
   Disciplina *disc2;   // d2
   std::pair<Aluno*, Aluno*> parAlunos;
   int turno;
   std::pair<TurnoIES*, TurnoIES*> parTurnosIES;
   TurnoIES* turnoIES;
   int periodo;
};

/**
//* Type definition for the hash object.
*/
typedef std::map< ConstraintPre, int > ConstraintPreHash;


#endif
