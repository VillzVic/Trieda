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
      V_PRE_CREDITOS = 1,							// x_{i,d,s}
      V_PRE_OFERECIMENTO = 2,						// o_{i,d,s}
      V_PRE_ABERTURA = 3,							// z_{i,d,cp}
      V_PRE_ALUNOS = 4,							    // a_{i,d,s,oft}
      V_PRE_ALOC_ALUNO = 5,				            // b_{i,d,c}
      V_PRE_MIN_CRED_SEMANA = 6,					// h_{bi}
      V_PRE_MAX_CRED_SEMANA = 7,					// H_{bi}
      V_PRE_SLACK_ALOC_ALUNOS_CURSO_INCOMPAT = 8,   // bs_{i,d,c1,c2}
      V_PRE_SLACK_DEMANDA = 9,						// fd_{d,o}
	  V_PRE_SLACK_COMPARTILHAMENTO = 10,			// fc_{i,d,c1,c2}
	  V_PRE_SLACK_SALA = 11,						// fs_{d,s}
	  V_PRE_LIM_SUP_CREDS_SALA = 12,				// Hs_{cp}
	  V_PRE_ALOC_ALUNO_OFT = 13						// c_{i,d,s,oft}
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
};


/**
//* Type definition for the hash object.
*/
typedef std::map< VariablePre, int > VariablePreHash;


#endif 