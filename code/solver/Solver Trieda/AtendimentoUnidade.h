#ifndef _ATENDIMENTO_UNIDADE_H_
#define _ATENDIMENTO_UNIDADE_H_

#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"
#include "AtendimentoSala.h"
#include "Unidade.h"

class AtendimentoUnidade :
   public OFBase
{
public:
   AtendimentoUnidade( void );
   virtual ~AtendimentoUnidade( void );

   Unidade * unidade;
   GGroup< AtendimentoSala * > * atendimentos_salas;

   void setCodigoUnidade( std::string value ) { codigo_unidade = value; }
   std::string getCodigoUnidade() const { return codigo_unidade; }

private:
   std::string codigo_unidade;
};

std::ostream & operator << ( std::ostream &, AtendimentoUnidade & );

#endif