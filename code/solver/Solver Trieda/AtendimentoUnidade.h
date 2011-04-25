#ifndef _ATENDIMENTO_UNIDADE_H_
#define _ATENDIMENTO_UNIDADE_H_

#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"
#include "AtendimentoSala.h"
#include "Unidade.h"

class AtendimentoUnidade:
   public OFBase
{
public:
   AtendimentoUnidade(void);
   virtual ~AtendimentoUnidade(void);

   GGroup< AtendimentoSala * > * atendimentos_salas;
   Unidade * unidade;

   void setUnidadeId(std::string value) { unidade_id = value; }
   std::string getUnidadeId() { return unidade_id; }

private:
   std::string unidade_id;
};

std::ostream& operator << (std::ostream& out, AtendimentoUnidade& unidade);

#endif