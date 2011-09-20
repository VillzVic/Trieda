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

   GGroup< AtendimentoUnidade * > * atendimentos_unidades;
   Campus * campus;

   void setCampusId( std::string s ) { this->campus_id = s; }
   std::string getCampusId() const { return this->campus_id; }

private:
   std::string campus_id;
};

std::ostream& operator << ( std::ostream &, AtendimentoCampus & );

#endif
