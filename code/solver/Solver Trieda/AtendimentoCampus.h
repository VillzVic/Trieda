#ifndef _ATENDIMENTO_CAMPUS_H_
#define _ATENDIMENTO_CAMPUS_H_

#include "ofbase.h"
#include "AtendimentoUnidade.h"
#include "Campus.h"

class AtendimentoCampus:
   public OFBase
{
public:
   AtendimentoCampus();
   
   virtual ~AtendimentoCampus();

   std::string campus_id;
   Campus * campus;

   GGroup< AtendimentoUnidade * > * atendimentos_unidades;
};

std::ostream& operator << ( std::ostream &, AtendimentoCampus & );

#endif