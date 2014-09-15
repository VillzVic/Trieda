#ifndef VARIABLE_TATICO_H
#define VARIABLE_TATICO_H

#ifndef WIN32
#include <map>
#else
#include <hash_map>
#endif

#include "ProblemData.h"

// Variables
class VariableTatico 
{
public:
   // All variable types
   enum VariableType
   {
      V_ERROR = 0,
      V_CREDITOS = 1,							// x_{i,d,u,s,hi,hf,t}
      V_ABERTURA = 2,							// z_{i,d,cp}
      V_DIAS_CONSECUTIVOS = 5,					// c_{i,d,t,cp}
      V_SLACK_DEMANDA = 6,						// fd_{i,d,cp}
	  V_MIN_CRED_SEMANA = 8,					// h_{a}
      V_MAX_CRED_SEMANA = 9,					// H_{a}
      V_SLACK_DIST_CRED_DIA_SUPERIOR = 11,		// fcp_{i,d,s,t}
      V_SLACK_DIST_CRED_DIA_INFERIOR = 12,		// fcm_{i,d,s,t}
      
      V_COMBINACAO_DIVISAO_CREDITO = 13,		// m{i,d,k}
      V_SLACK_COMBINACAO_DIVISAO_CREDITO_M = 14,// fkm{i,d,t}
      V_SLACK_COMBINACAO_DIVISAO_CREDITO_P = 15,// fkp{i,d,t}
	  V_ABERTURA_COMPATIVEL = 16,				// zc_{d,t}
	  V_ALUNO_UNID_DIA = 17,					// y_{a,u,t}
	  V_ALUNO_VARIAS_UNID_DIA = 18,				// w_{a,t}
	  V_SLACK_ALUNO_VARIAS_UNID_DIA = 19,		// fu_{i1,d1,i2,d2,t,cp}
	  V_SLACK_SLACKDEMANDA_PT = 20,				// ffd_{i1,-d,i2,d,cp}
	  V_ALUNO_DIA = 21,							// du_{a,t}
	  V_DESALOCA_ALUNO = 22,					// fa_{i,d,a}
	  V_DESALOCA_ALUNO_DIA = 23,				// fad_{i,d,a,t}
	  V_FORMANDOS_NA_TURMA = 24,				// f_{i,d,cp}
	  V_FOLGA_HOR_PROF = 25,					// fp_{d,t,h}
      V_SALA_TURMA = 26,						// y_{d,i,s}
	  V_FOLGA_ALUNO_MIN_ATEND1 = 27,			// fmd1_{a}
	  V_FOLGA_ALUNO_MIN_ATEND2 = 28,			// fmd2_{a}
	  V_FOLGA_ALUNO_MIN_ATEND3 = 29				// fmd3_{a}
   };

   //Constructors
   VariableTatico();
   VariableTatico( const VariableTatico & );

   //Destructor
   virtual ~VariableTatico();

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

   int getDia() const { return t; }

   Oferta * getOferta() const { return o; }

   int getK() const { return k; }
   
   std::pair<Curso*, Curso*> getParCursos() const{ return parCursos;}
   
   std::pair<Oferta*, Oferta*> getParOfertas() const{ return parOft;}
   
   HorarioAula* getHorarioAulaInicial() const { return horarioAulaI; }

   HorarioAula* getHorarioAulaFinal() const { return horarioAulaF; }

   DateTime* getDateTimeInicial() const { return dateTimeI; }

   DateTime* getDateTimeFinal () const { return dateTimeF; }

   Aluno* getAluno() const { return aluno; }

   int getTurma1() const { return turma1; }
   
   int getTurma2() const { return turma2; }

   Disciplina* getDisciplina1() const { return disc1; }
   
   Disciplina* getDisciplina2() const { return disc2; }

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

   void setDia( int tt ) {  t = tt; }

   void setOferta( Oferta * oferta ) { o = oferta; }

   void setK( int kk ) { k = kk; }
   
   void setParCursos( std::pair<Curso*, Curso*> par ){ parCursos = par;}
     
   void setParOfertas( Oferta* oft1, Oferta* oft2 ){ parOft.first = oft1; parOft.second = oft2;}
   
   void setHorarioAulaInicial( HorarioAula* h ) { horarioAulaI = h; }

   void setHorarioAulaFinal( HorarioAula* h ) { horarioAulaF = h; }

   void setDateTimeInicial (DateTime *di) { dateTimeI = di; }

   void setDateTimeFinal (DateTime *df) { dateTimeF = df; }

   void setAluno( Aluno *a ) { aluno = a; }
   
   void setTurma1( int i1 ) { turma1 = i1; }
   
   void setTurma2( int i2 ) { turma2 = i2; }

   void setDisciplina1( Disciplina * d1 ) {  disc1 = d1; }

   void setDisciplina2( Disciplina * d2 ) {  disc2 = d2; }

   //==================================================
   // OPERATORS 
   //==================================================
   // Assignment 
   VariableTatico & operator = ( const VariableTatico & );
   // Less 
   bool operator < ( const VariableTatico & ) const;
   // Equals 
   bool operator == ( const VariableTatico & ) const;

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
   int t; // dia
   Oferta * o; // oferta
   int k; // combinação de divisão de créditos
   HorarioAula* horarioAulaI;
   HorarioAula* horarioAulaF;
   Aluno *aluno;
   DateTime *dateTimeI;
   DateTime *dateTimeF;

   int turma1;          // i1
   Disciplina *disc1;   // d1
   int turma2;          // i2
   Disciplina *disc2;   // d2

};


/**
//* Type definition for the hash object.
*/
typedef std::map< VariableTatico, int > VariableTaticoHash;


#endif 
