#ifndef VARIABLE_ESCOLA_H
#define VARIABLE_ESCOLA_H

#ifndef WIN32
#include <map>
#else
#include <hash_map>
#endif

#include "ProblemData.h"

// Variables
class VariableMIPUnico 
{
public:
   // All variable types
   enum VariableType
   {
      V_ERROR,
	  V_ALUNO_CREDITOS,						// v_{a,i,d,u,s,hi,hf,t}
	  V_CREDITOS,							// x_{i,d,u,s,hi,hf,t}
      V_OFERECIMENTO,						// o_{i,d,u,s}
      V_ALOCA_ALUNO_TURMA_DISC,				// s_{i,d,a}
	  V_DIAS_CONSECUTIVOS,					// c_{i,d,t}
      V_SLACK_DEMANDA_ALUNO,				// fd_{d,a}
	  V_TURMA_ATEND_CURSO,					// b_{i,d,c}
      V_SLACK_ABERT_SEQ_TURMA,				// ft_{i,d,cp}
      
      V_COMBINACAO_DIVISAO_CREDITO,			// m{i,d,k,cp}
      V_SLACK_COMBINACAO_DIVISAO_CREDITO_M,	// fkm{i,d,t,cp}
      V_SLACK_COMBINACAO_DIVISAO_CREDITO_P,	// fkp{i,d,t,cp}
	  V_ABERTURA_COMPATIVEL,				// zc_{d,t}
	  V_SLACK_COMPARTILHAMENTO,				// fc_{i,d,c,c'}
	  V_ALUNO_DIA,							// du_{a,t}
	  V_SLACK_PRIOR_INF,					// fpi_{a,cp}
	  V_SLACK_PRIOR_SUP,					// fps_{a,cp}
	  V_ABERTURA,							// z_{i,d,cp}
	  V_ALUNOS_MESMA_TURMA_PRAT,			// ss_{dp,a1,a2}
	  V_FOLGA_ALUNO_MIN_ATEND1,				// fmd1_{a}
	  V_FOLGA_ALUNO_MIN_ATEND2,				// fmd2_{a}
	  V_FOLGA_ALUNO_MIN_ATEND3,				// fmd3_{a}
	  V_SALA,								// u_{s}

	  V_PROF_TURMA,							// y_{p,i,d,cp}
	  V_PROF_AULA,							// k_{p,i,d,u,t,h}
	  V_PROF_UNID,							// uu_{p,t,u}
	  V_PROF_DESLOC,						// desloc_{p,t,u1,u2}

	  V_HI_PROFESSORES,						// hip_{p,t,f}
	  V_HF_PROFESSORES,						// hfp_{p,t,f}
	  V_HI_ALUNOS,							// hia_{a,t}
	  V_HF_ALUNOS,							// hfa_{a,t}
	  V_FOLGA_GAP_ALUNOS,					// fagap_{a,t}
	  V_FOLGA_GAP_PROF,						// fpgap_{p,t}
	  V_PROF_FASE_DIA_USADA,				// ptf_{p,t,f}
	  V_PROF_DIA_USADO,						// pt_{p,t}
	  
	  V_FOLGA_MIN_CRED_DIA_ALUNO,			// fcad_{a,t}
	  V_F_CARGA_HOR_ANT_PROF,				// fch_{p}

	  V_LONGO_DIA_ALUNO,					// l_{a,t}		// marretado
	  V_FOLGA_MIN_CRED_DIA_ALUNO_MARRETA,	// fcadm_{a,t}  // marretado

	  V_INICIO_ALUNOS,						// inicio_{a,t,h}
	  V_FIM_ALUNOS							// fim_{a,t,h}
	  	  
   };

   //Constructors
   VariableMIPUnico();
   VariableMIPUnico( const VariableMIPUnico & );

   //Destructor
   virtual ~VariableMIPUnico();

   //==================================================
   // GET METHODS 
   //==================================================
   //Return variable type
   VariableType getType() const { return type; }

   // Return value
   double getValue() const { return value; }

   Campus * getCampus() const { return cp; }

   Unidade * getUnidade() const { return u; }   
   Unidade * getUnidade1() const { return u1; }
   Unidade * getUnidade2() const { return u2; }

   Sala * getSala() const { return s; }

   ConjuntoSala * getSubCjtSala() const { return tps; }

   int getTurma() const { return i; }

   Curso * getCurso() const { return c; }

   Curso * getCursoIncompat() const { return c_incompat; }

   BlocoCurricular * getBloco() const { return b; }

   Disciplina * getDisciplina() const { return d; }

   int getSubBloco() const { return j; }

   int getDia() const { return t; }

   Oferta * getOferta() const { return o; }

   int getK() const { return k; }
   
   std::pair<Curso*, Curso*> getParCursos() const{ return parCursos;}
   
   std::pair<Oferta*, Oferta*> getParOfertas() const{ return parOft;}
   
   HorarioAula* getHorarioAulaInicial() const { return horarioAulaI; }

   HorarioAula* getHorarioAulaFinal() const { return horarioAulaF; }

   Aluno* getAluno() const { return aluno; }

   int getTurma1() const { return turma1; }
   
   int getTurma2() const { return turma2; }

   Disciplina* getDisciplina1() const { return disc1; }
   
   Disciplina* getDisciplina2() const { return disc2; }
   
   DateTime getDateTimeInicial() const { return dateTimeI; }

   DateTime getDateTimeFinal () const { return dateTimeF; }

   AlunoDemanda* getAlunoDemanda () const { return alunoDemanda; }
         
   std::pair<Aluno*, Aluno*> getParAlunos() const { return parAlunos; }

   int getPeriodo() const { return periodo; }
   
   Professor* getProfessor () const { return professor_; }
         
   int getFaseDoDia() const { return faseDoDia_; }

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

   void setUnidade1( Unidade * uu ) {  u1 = uu; }  
   void setUnidade2( Unidade * uu ) {  u2 = uu; }

   void setSala( Sala * ss ) {  s = ss; }

   void setSubCjtSala( ConjuntoSala * tpss )  { tps = tpss; }

   void setTurma( int ii ) { i = ii; }

   void setCurso( Curso * cc ) { c = cc; }

   void setCursoIncompat( Curso * cc ) { c_incompat = cc; }

   void setBloco( BlocoCurricular * bb ) {  b = bb; } 

   void setDisciplina( Disciplina * dd ) {  d = dd; }

   void setSubBloco( int jj ) { j = jj; }   

   void setDia( int tt ) {  t = tt; }

   void setOferta( Oferta * oferta ) { o = oferta; }

   void setK( int kk ) { k = kk; }
   
   void setParCursos( std::pair<Curso*, Curso*> par ){ parCursos = par;}
     
   void setParOfertas( Oferta* oft1, Oferta* oft2 ){ parOft.first = oft1; parOft.second = oft2;}
   
   void setHorarioAulaInicial( HorarioAula* h ) { horarioAulaI = h; }

   void setHorarioAulaFinal( HorarioAula* h ) { horarioAulaF = h; }

   void setAluno( Aluno *a ) { aluno = a; }
   
   void setTurma1( int i1 ) { turma1 = i1; }
   
   void setTurma2( int i2 ) { turma2 = i2; }

   void setDisciplina1( Disciplina * d1 ) {  disc1 = d1; }

   void setDisciplina2( Disciplina * d2 ) {  disc2 = d2; }

   void setDateTimeInicial (DateTime di) { dateTimeI = di; }

   void setDateTimeFinal (DateTime df) { dateTimeF = df; }
   
   void setAlunoDemanda (AlunoDemanda* aldem) { alunoDemanda = aldem; }
   
   void setParAlunos( Aluno* aluno1, Aluno* aluno2 ){ parAlunos.first = aluno1; parAlunos.second = aluno2;}

   void setPeriodo( int value ){ periodo = value; }

   void setProfessor( Professor* p ){ professor_ = p; }
   
   void setFaseDoDia( int value ){ faseDoDia_ = value; }
      


   //==================================================
   // OPERATORS 
   //==================================================
   // Assignment 
   VariableMIPUnico & operator = ( const VariableMIPUnico & );
   // Less 
   bool operator < ( const VariableMIPUnico & ) const;
   // Equals 
   bool operator == ( const VariableMIPUnico & ) const;

   // Variable name
   std::string toString() const;

private:
   VariableType type;
   double value;
   Campus * cp;
   Unidade * u;
   Unidade * u1;
   Unidade * u2;
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
   int t; // dia
   Oferta * o; // oferta
   int k; // combinação de divisão de créditos
   HorarioAula* horarioAulaI;
   HorarioAula* horarioAulaF;
   Aluno *aluno;
   
   DateTime dateTimeI;
   DateTime dateTimeF;

   int turma1;          // i1
   Disciplina *disc1;   // d1
   int turma2;          // i2
   Disciplina *disc2;   // d2

   AlunoDemanda* alunoDemanda;
   std::pair<Aluno*, Aluno*> parAlunos;

   int periodo;

   Professor *professor_;

   int faseDoDia_;
};


/**
//* Type definition for the hash object.
*/
typedef std::map< VariableMIPUnico, int > VariableMIPUnicoHash;


#endif 
