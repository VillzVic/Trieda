#ifndef _CONSTRAINT_ESCOLA_H_
#define _CONSTRAINT_ESCOLA_H_

#ifndef WIN32
#include <map>
#else
#include <hash_map>
#endif

#include "ProblemData.h"

/**
* Class which defines a contraint in the LP.
*/
class ConstraintMIPUnico
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
	  C_DEMANDA_EQUIV_ALUNO = 31,
	  C_ALUNO_DISC_PRATICA_TEORICA_EQUIV = 32,
	  C_ATIVA_Z = 33,
	  C_TURMA_COM_MESMOS_ALUNOS_POR_AULA = 34,
	  C_DISC_DIA_HOR_PROF = 35,
	  C_ALUNO_DISC_PRATICA_TEORICA_1x1 = 36,
	  C_AULA_PT_SEQUENCIAL = 37,
	  C_ALUNO_DISC_PRATICA_TEORICA_1xN = 38,
	  C_ALUNOS_MESMA_TURMA_PRAT = 39,
	  C_ALOC_MIN_ALUNO = 40,
	  C_FOLGA_OCUPACAO_SALA = 41,
	  
	  C_PROF_MIN_DESCANSO = 49,
	  C_MAX_ALUNOS_TURMA = 50,

	  C_PROF_DIA_HOR						= 51,
	  C_PROF_AULA							= 52,
	  C_PROF_AULA_SUM						= 53,
	  C_PROF_TURMA							= 54,
	  C_PROF_UNICO							= 55,
     C_GAPS_PROFESSORES_I_F = 56,
     C_GAPS_PROFESSORES_I = 57,
     C_GAPS_PROFESSORES_F = 58,
     C_GAPS_ALUNOS_I_F = 59,
     C_GAPS_ALUNOS_I = 60,
     C_GAPS_ALUNOS_F = 61,

      C_PROF_HOR_INIC_LB = 62,
	  C_PROF_HOR_INIC_UB = 63,
      C_PROF_HOR_FIN_LB = 64,
	  C_PROF_HOR_FIN_UB = 65,
	  C_PROF_GAP = 66,
	  
      C_ALUNO_HOR_INIC_LB = 67,
	  C_ALUNO_HOR_INIC_UB = 68,
      C_ALUNO_HOR_FIN_LB = 69,
	  C_ALUNO_HOR_FIN_UB = 70,
	  C_ALUNO_GAP = 71,

	  C_ALUNO_MIN_CREDS_DIA = 72,
	  C_TEMPO_DESLOC_PROF = 73,
	  C_NR_DESLOC_PROF = 74,
	  C_CARGA_HOR_ANTERIOR = 75
   };

   /** Default constructor. */
   ConstraintMIPUnico();

   /** Copy constructor. */
   ConstraintMIPUnico( const ConstraintMIPUnico & );

   /** Destructor. */
   virtual ~ConstraintMIPUnico();

   /** Assign operator. */
   ConstraintMIPUnico & operator = ( const ConstraintMIPUnico & );

   /** Less operator. */
   bool operator < ( const ConstraintMIPUnico & ) const;

   /** Equals operator. */
   bool operator == ( const ConstraintMIPUnico & ) const;

   /**
   * Returns a string containing an unique name for the constraint.
   * @return a string containing an unique name for the constraint.
   */
   std::string toString( int etapa );

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
   DateTime getDateTimeInicial() const { return dateTimeI; }
   DateTime getDateTimeFinal () const { return dateTimeF; }
   std::pair<Aluno*, Aluno*> getParAlunos() const { return parAlunos; }
   int getPeriodo() const { return periodo; }
   Professor* getProfessor() const { return professor_; }
   int getFaseDoDia() const { return faseDoDia_; }

   Unidade * getUnidOrig() const { return u_orig; }
   Unidade * getUnidDest() const { return u_dest; }
   Unidade * getUnidAtual() const { return u_atual; }
      
   HorarioAula* getHorarioAulaOrig() const { return h_orig; }
   HorarioAula* getHorarioAulaDest() const { return h_dest; }
   HorarioAula* getHorarioAulaAtual() const { return h_atual; }

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
   void setDateTimeInicial (DateTime di) { dateTimeI = di; }
   void setDateTimeFinal (DateTime df) { dateTimeF = df; }
   void setParAlunos( std::pair<Aluno*, Aluno*> a ){ parAlunos = a; }
   void setPeriodo( int value ){ periodo = value; }
   void setProfessor( Professor* p ) { professor_ = p; }
   void setFaseDoDia( int value ){ faseDoDia_ = value; }

   void setUnidadeOrig( Unidade * uu ) {  u_orig = uu; }
   void setUnidadeDest( Unidade * uu ) {  u_dest = uu; }
   void setUnidadeAtual( Unidade * uu ) {  u_atual = uu; }

   void setHorarioAulaOrig( HorarioAula * hh ) {  h_orig = hh; }
   void setHorarioAulaDest( HorarioAula * hh ) {  h_dest = hh; }
   void setHorarioAulaAtual( HorarioAula * hh ) {  h_atual = hh; }
   
   bool compLessHorarioAula( HorarioAula* h1, HorarioAula* h2 ) const;

private:

   /** Attribute which defines the constraint type of the instance. */
   ConstraintType type;

   Campus *cp;
   Unidade* u;
   Sala* s;
   ConjuntoSala* cjtSalaCompart;
   ConjuntoSala* tps;
   Unidade *u_orig;
   Unidade *u_dest;
   Unidade *u_atual;
   
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

   std::pair<Oferta*, Oferta*> parOfts; // par de ofertas (restrição 1.2.36)

   HorarioAula* h;
   
   HorarioAula* h_orig;
   HorarioAula* h_dest;
   HorarioAula* h_atual;
   
   Aluno* aluno;

   int turma1;          // i1
   Disciplina *disc1;   // d1
   int turma2;          // i2
   Disciplina *disc2;   // d2

   HorarioAula* horarioAulaI;
   HorarioAula* horarioAulaF;

   DateTime dateTimeI;
   DateTime dateTimeF;

   int periodo;

   std::pair<Aluno*, Aluno*> parAlunos;

   Professor *professor_;
   
   int faseDoDia_;
};

/**
//* Type definition for the hash object.
*/
typedef std::map< ConstraintMIPUnico, int > ConstraintMIPUnicoHash;

#endif
