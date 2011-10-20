#ifndef _ATENDIMENTO_CAMPUS_SOLUCAO_H_
#define _ATENDIMENTO_CAMPUS_SOLUCAO_H_

#include "ofbase.h"
#include "AtendimentoUnidadeSolucao.h"
#include "AtendimentoCampus.h"

class AtendimentoCampusSolucao :
   public OFBase
{
public:
   AtendimentoCampusSolucao( void );
   AtendimentoCampusSolucao( AtendimentoCampus & );
   virtual ~AtendimentoCampusSolucao( void );

   virtual void le_arvore( ItemAtendimentoCampusSolucao & );

   GGroup< AtendimentoUnidadeSolucao * > atendimentosUnidades;

   virtual bool operator < ( AtendimentoCampusSolucao & right ) const
   { 
      return ( this->campusId < right.getCampusId() );
   }

   virtual bool operator == ( AtendimentoCampusSolucao & right ) const
   { 
	   return ( this->campusId == right.getCampusId() );
   }

   void setCampusId( int v ) { this->campusId = v; }
   void setCampusCodigo( std::string s ) { this->campusCodigo = s; }

   int getCampusId() const { return this->campusId; }
   std::string getCampusCodigo() const { return this->campusCodigo; }

private:
   int campusId;
   std::string campusCodigo;
};

#endif
