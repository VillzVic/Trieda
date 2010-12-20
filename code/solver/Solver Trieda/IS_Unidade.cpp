#include "IS_Unidade.h"

IS_Unidade::IS_Unidade(Unidade * _unidade)
{
   unidade = _unidade;
   this->setId(_unidade->getId());

   //conjutoSalas = &_unidade->conjutoSalas;
}

IS_Unidade::IS_Unidade(IS_Unidade const & is_unidade)
{
   this->setId(is_unidade.getId());

   unidade = is_unidade.unidade;

   GGroup<IS_Sala*>::iterator it_sala = is_unidade.salas.begin();

   for(; it_sala != is_unidade.salas.end(); it_sala++)
   {
      salas.add(*it_sala);
   }

   //conjutoSalas = is_unidade.conjutoSalas;
}

IS_Unidade::~IS_Unidade(void)
{
}
