#pragma once
#include "ofbase.h"

#include "AtendimentoUnidadeSolucao.h"
#include "AtendimentoCampus.h"

class AtendimentoCampusSolucao :
   public OFBase
{
public:
   AtendimentoCampusSolucao( void );

   AtendimentoCampusSolucao( AtendimentoCampus & );

   ~AtendimentoCampusSolucao( void );
   virtual void le_arvore( ItemAtendimentoCampusSolucao & );

   GGroup< AtendimentoUnidadeSolucao * > atendimentosUnidades;

   virtual bool operator < ( AtendimentoCampusSolucao & right )
   { 
      return ( campusId < right.getCampusId() );
   }

   virtual bool operator == ( AtendimentoCampusSolucao & right )
   { 
	   return ( campusId == right.getCampusId() );
   }

   void setCampusId( int v ) { campusId = v; }
   void setCampusCodigo( std::string s ) { campusCodigo = s; }

   int getCampusId() { return campusId; }
   std::string getCampusCodigo() { return campusCodigo; }

private:
   int campusId;
   std::string campusCodigo;
};
