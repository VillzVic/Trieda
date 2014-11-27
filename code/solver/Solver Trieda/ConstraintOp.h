#ifndef _CONSTRAINT_OP_H_
#define _CONSTRAINT_OP_H_

#include <map>

#include "Aula.h"
#include "Professor.h"
#include "HorarioDia.h"
#include "ProblemData.h"

/**
* Class which defines a contraint in the LP.
*/
class ConstraintOp
{
public:
   /** All constraint types */
   enum ConstraintOpType
   {
      C_ERROR = 0,
      C_SALA_HORARIO = 1,
      C_PROFESSOR_HORARIO = 2,
      C_BLOCO_HORARIO = 3,
      C_ALOC_AULA = 4,
      C_PROF_DISC = 5,
      C_PROF_DISC_UNI = 6,
      C_FIX_PROF_DISC_SALA_DIA_HOR = 7,
      C_FIX_PROF_DISC_DIA_HOR = 8,
      C_FIX_PROF_DISC = 9,
      C_FIX_PROF_DISC_SALA = 10,
      C_FIX_PROF_SALA = 11,
      C_DISC_HORARIO = 12,
      C_DISC_HORARIO_UNICO = 13,
      C_MIN_MEST_CURSO = 14,
      C_MIN_DOUT_CURSO = 15,
	  C_CARGA_HOR_MIN_PROF = 17,
      C_CARGA_HOR_MIN_PROF_SEMANA = 18,
      C_CARGA_HOR_MAX_PROF_SEMANA = 19,
      C_CUSTO_CORPO_DOCENTE = 21,
      C_DISC_PROF_CURSO1 = 22,
	  C_DISC_PROF_CURSO2 = 23,
	  C_MAX_DISC_PROF_CURSO = 24,
      C_AVALIACAO_CORPO_DOCENTE = 25,	  
      C_TEMPO_DESLOC_PROF = 26,
      C_DESLOC_VIAVEL = 27,
      C_ULTIMA_PRIMEIRA_AULA_PROF = 28,
      C_GAPS_PROFESSORES = 29,
	  C_BLOCO_HORARIO_DISC = 30,
	  C_PROF_HORARIO_MULTIUNID = 31,
	  C_ALUNO_HORARIO = 32,
	  C_GAPS_PROFESSORES_I = 33,
	  C_GAPS_PROFESSORES_F = 34,
	  C_GAPS_PROFESSORES_I_F = 35,
	  C_CALCULA_NRO_PROFS_CURSO = 36,
	  C_MAX_NAO_MEST_CURSO = 37,
	  C_MAX_NAO_DOUT_CURSO = 38,
	  C_ESTIMA_NRO_PROFS_VIRTUAIS_CURSO = 39,
	  C_ESTIMA_NRO_PROFS_VIRTUAIS_MEST_CURSO = 40,
	  C_ESTIMA_NRO_PROFS_VIRTUAIS_DOUT_CURSO = 41,
	  C_ESTIMA_NRO_PROFS_VIRTUAIS_GERAIS_CURSO = 42,
	  C_SOMA_NRO_PROFS_VIRTUAIS_CURSO = 43,
      C_ALOC_PROF_CURSO1 = 44,
	  C_ALOC_PROF_CURSO2 = 45,
	  C_CALCULA_NRO_PROF_VIRT_MEST_CURSO = 46,
	  C_CALCULA_NRO_PROF_VIRT_DOUT_CURSO = 47,
	  C_CALCULA_NRO_PROF_VIRT_GERAIS_CURSO = 48,
	  C_CALCULA_NRO_PROF_VIRT_CURSO = 49,
	  C_USA_PROF_VIRT = 50,
	  C_PROF_DISC_PT_UNICO = 51,
	  C_AULA_PT_SEQUENCIAL = 52,
	  C_MAX_DIAS_SEMANA_PROF = 53,
      C_DIAS_PROF_MINISTRA_AULA_MIN = 54,
      C_DIAS_PROF_MINISTRA_AULA_MAX = 55,
	  C_MIN_CREDS_DIARIOS_PROF = 56,
	  C_PROF_ATEND_COMPLETO = 57,
	  C_ALOC_MIN_ALUNO = 58,
	  C_PROF_MIN_DESCANSO = 59,
	  C_GARANTE_MIN_PROFS_CURSO = 60,
	  C_MAX_DISC_PROF_OFERTA_PERIODO = 61,
	  C_DISC_PROF_OFERTA1 = 62,
	  C_DISC_PROF_OFERTA2 = 63,
	  C_ESTIMA_NRO_PROFS_VIRTUAIS_CONTRATO_CURSO = 64,
	  C_MAX_SEM_CONTRATO_CURSO = 65,
	  C_CALCULA_NRO_PROF_VIRT_CONTRATO_CURSO = 66,
	  C_MIN_CONTRATO_CURSO = 67,
	  
      C_PROF_HOR_INIC_LB = 68,
	  C_PROF_HOR_INIC_UB = 69,
      C_PROF_HOR_FIN_LB = 70,
	  C_PROF_HOR_FIN_UB = 71,
	  C_PROF_GAP = 72,

	  C_NR_DESLOC_PROF = 73
   };

   /** Default constructor. */
   ConstraintOp();

   /** Copy constructor. */
   ConstraintOp( const ConstraintOp & );

   /** Destructor. */
   virtual ~ConstraintOp();

   /** Assign operator. */
   ConstraintOp & operator = ( const ConstraintOp & );

   /** Less operator. */
   bool operator < ( const ConstraintOp & ) const;

   /** Equals operator. */
   bool operator == ( const ConstraintOp & ) const;

   /**
   * Returns a string containing an unique name for the constraint.
   * @return a string containing an unique name for the constraint.
   */
   std::string toString();

   //==================================================
   // GET METHODS 
   //==================================================
   // Return constraint type
   ConstraintOpType getType() const { return type; }

   Curso * getCurso() const { return this->curso; }
   Sala * getSala() const { return s; }
   BlocoCurricular * getBloco() const { return b; }
   Professor * getProfessor() const { return professor; } 
   Aula * getAula() const { return aula; } 
   int getSubBloco() const { return j; }
   int getDia() const { return t; }
   HorarioDia * getHorario() const { return h; }
   HorarioAula * getHorarioAula() const { return horarioAula; }
   Disciplina * getDisciplina() const { return disciplina; }
   int getTurma() const { return turma; }
   HorarioAula * getH1() const { return h1; }
   HorarioAula * getH2() const { return h2; }
   HorarioDia * getHorarioDiaD() const { return this->horarioDiaD; }
   HorarioDia * getHorarioDiaD1() const { return this->horarioDiaD1; }
   std::pair<Aula*, HorarioAula*> getPar1AulaHor() const { return this->par1_aula_hor; };
   std::pair<Aula*, HorarioAula*> getPar2AulaHor() const { return this->par2_aula_hor; };
   Campus* getCampus() const { return this->campus;}
   Unidade* getUnidade() const { return this->unidade;}
   int getDuracaoAula() const { return this->duracaoAula; }
   Aluno * getAluno() const { return aluno; } 
   Oferta* getOferta() const { return oferta; }
   int getPeriodo () const { return periodo; }
   TipoContrato* getContrato () const { return contrato; }   
   int getTurmaT() const { return turmaT; }
   int getTurmaP() const { return turmaP; }
   Campus* getCampusT() const { return cpT; }
   Campus* getCampusP() const { return cpP; }
   DateTime * getDateTimeInicial() const { return dti; }
   DateTime * getDateTimeFinal() const { return dtf; }
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
   void setType( ConstraintOpType t ) { type = t; }
   void setCurso( Curso * c ) { this->curso = c; }
   void setSala( Sala * ss ) {  s = ss; }
   void setBloco( BlocoCurricular * bb ) {  b = bb; } 
   void setAula( Aula * a ) {  aula = a; }
   void setProfessor( Professor * p ) { professor = p; }
   void setSubBloco( int jj ) { j = jj; }   
   void setDia( int tt ) {  t = tt; }
   void setHorario( HorarioDia * hh ) { h = hh; }
   void setHorarioAula( HorarioAula * hh ) { horarioAula = hh; }
   void setDisciplina ( Disciplina * aD ) { disciplina = aD; }
   void setTurma( int aT ) { turma = aT; }
   void setH1 ( HorarioAula * aH1 ) { h1 = aH1; }
   void setH2 ( HorarioAula * aH2 ) { h2 = aH2; }
   void setHorarioDiaD ( HorarioDia * hD ) { this->horarioDiaD = hD; }
   void setHorarioDiaD1 ( HorarioDia * hD ) { this->horarioDiaD1 = hD; }
   void setPar1AulaHor( Aula* a1, HorarioAula* h1 );
   void setPar2AulaHor( Aula* a2, HorarioAula* h2 );
   void setCampus( Campus* cp ) { this->campus = cp;}
   void setUnidade( Unidade* unid ) { this->unidade = unid;}
   void setDuracaoAula( int duracao ) { this->duracaoAula = duracao; }
   void setAluno ( Aluno *a ) { aluno = a; }
   void setOferta ( Oferta *o ) { oferta = o; }
   void setPeriodo ( int per ) { periodo = per; }
   void setContrato ( TipoContrato *c ) { contrato = c; }      
   void setTurmaT( int i ) { turmaT = i; }
   void setTurmaP( int i ) { turmaP = i; }
   void setCampusT( Campus* t ) { cpT = t; }
   void setCampusP( Campus* p ) { cpP = p; }
   void setDateTimeInicial( DateTime *dt ) { dti = dt; }
   void setDateTimeFinal( DateTime *dt ) { dtf = dt; }
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
   ConstraintOpType type;

   Curso * curso;
   Sala * s;
   HorarioDia * h; // horario
   HorarioAula * horarioAula;
   Aula * aula;
   Professor * professor;
   BlocoCurricular * b;
   Disciplina * disciplina;
   std::pair<Aula*, HorarioAula*> par1_aula_hor;
   std::pair<Aula*, HorarioAula*> par2_aula_hor;
   int turma;
   int j; // subbloco
   int t; // dia
   Campus* campus;
   Unidade* unidade;
   Aluno* aluno;
   Oferta *oferta;
   int periodo;
   TipoContrato *contrato;

   Unidade *u_orig;
   Unidade *u_dest;
   Unidade *u_atual;
   
   HorarioAula* h_orig;
   HorarioAula* h_dest;
   HorarioAula* h_atual;
   
   // Horários de aula utilizados no modelo operacional,
   // no critério de minimização de gaps nos horários dos professores
   HorarioAula * h1;
   HorarioAula * h2;

   // Horarios-Dia utilizados no modelo operacional,
   // no critério de última aula do dia D e primeira aula do dia D + 1
   HorarioDia * horarioDiaD;
   HorarioDia * horarioDiaD1;

   int duracaoAula; // Usada na restrição "criaRestricaoProfHorarioMultiCampi"

   int turmaT;
   int turmaP;
   Campus* cpT;
   Campus* cpP;
   
   DateTime* dti;
   DateTime* dtf;

   int faseDoDia_;
};

/**
//* Type definition for the hash object.
*/
typedef std::map< ConstraintOp, int > ConstraintOpHash;

#endif
