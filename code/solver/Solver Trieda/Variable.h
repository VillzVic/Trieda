#ifndef VARIABLE_H
#define VARIABLE_H

#ifndef WIN32
#include <map>
#else
#include <hash_map>
#endif

#include "ProblemData.h"

// Variables
class Variable 
{
public:
   // All variable types
   enum VariableType
   {
      V_ERROR = 0,
      V_CREDITOS = 1,							// x_{idust}
      V_OFERECIMENTO = 2,						// o_{idust}
      V_ABERTURA = 3,							// z_{i,d,cp}
      V_ALUNOS = 4,							   // a_{i,d,o}
      V_ALOC_ALUNO = 5,				         // b_{i,d,c,cp}
      V_N_SUBBLOCOS = 6,				        // w_{bc,t,cp}
      V_DIAS_CONSECUTIVOS = 7,				// c_{idt}
      V_MIN_CRED_SEMANA = 8,					// h_{bc,i}
      V_MAX_CRED_SEMANA = 9,					// H_{bc,i}
      V_ALOC_DISCIPLINA = 10,					// y_{i,d,tps,u}
      V_N_ABERT_TURMA_BLOCO = 11,				// v_{bt}
      V_SLACK_DIST_CRED_DIA_SUPERIOR = 12,	// fcp_{i,d,tps,t}
      V_SLACK_DIST_CRED_DIA_INFERIOR = 13,		// fcm_{i,d,tps,t}
      V_ABERTURA_SUBBLOCO_DE_BLC_DIA_CAMPUS = 14,		   // r_{b,t,cp}

      V_SLACK_ALOC_ALUNOS_CURSO_INCOMPAT = 15,   // bs_{i,d,c,c',cp}
      V_SLACK_DEMANDA = 16, // fd_{d,o}
      V_COMBINACAO_DIVISAO_CREDITO = 17, // m{i,d,k}
      V_SLACK_COMBINACAO_DIVISAO_CREDITO_M = 18, // fkm{i,d,k}
      V_SLACK_COMBINACAO_DIVISAO_CREDITO_P = 19, // fkp{i,d,k}

      V_CREDITOS_MODF = 20, // xm_{d,t}
	  V_ABERTURA_COMPATIVEL = 21, //zc_{d,t}
	  V_ABERTURA_BLOCO_MESMO_TPS = 22, //n_{bc,tps}
	  V_SLACK_ABERTURA_BLOCO_MESMO_TPS = 23, //fn_{bc,tps}
	  V_SLACK_COMPARTILHAMENTO = 24,	// fc_{i,d,c1,c2,cp}
	  V_ALOC_ALUNOS_OFT = 25,	// e_{i,d,oft}
	  V_CREDITOS_OFT = 26,		// q_{i,d,oft,u,s,t}
	  V_CREDITOS_PAR_OFT = 27,	// p_{i,d,oft1,oft2,u,s,t}
	  V_ALOC_ALUNOS_PAR_OFT = 28,  // of_{i,d,oft1,oft2}
	  V_MIN_HOR_DISC_OFT_DIA = 29, // g_{d,oft,t}
	  V_COMBINA_SL_SALA = 30, // cs_{s,t,k}
	  V_COMBINA_SL_BLOCO = 31 // cbc_{cb,t,k}

   };

   //Constructors
   Variable();
   Variable( const Variable & );

   //Destructor
   virtual ~Variable();

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
   
   int getCombinaSL() const { return combinaSL; }

   int getCombinaSLBloco() const { return combinaSLBloco; }

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

   void setCombinaSL( int combinacao ) { combinaSL = combinacao; }

   void setCombinaSLBloco( int combinacao ) { combinaSLBloco = combinacao; }

   //==================================================
   // OPERATORS 
   //==================================================
   // Assignment 
   Variable & operator = ( const Variable & );
   // Less 
   bool operator < ( const Variable & ) const;
   // Equals 
   bool operator == ( const Variable & ) const;

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
   int combinaSL; // combinação de divisão de creditos entre as semanas letivas para uma sala e dia
   int combinaSLBloco; // combinação de divisão de creditos entre as semanas letivas para um bloco curricular e dia
};


/**
//* Type definition for the hash object.
*/
typedef std::map< Variable, int > VariableHash;

//class VariableHasher : public stdext::hash_compare< Variable >
//{
//public:
//   // Less operator
//   bool operator()( const Variable &, const Variable & ) const;
//
//   // Hash value
//   size_t operator()( const Variable & ) const;
//};
//
///**
//* Type definition for the hash object.
//*/
//typedef stdext::hash_map< Variable, int, VariableHasher > VariableHash;

#endif 
