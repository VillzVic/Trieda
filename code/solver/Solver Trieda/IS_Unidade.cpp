#include "IS_Unidade.h"

IS_Unidade::IS_Unidade(void)
{
}

IS_Unidade::IS_Unidade(IS_Unidade const & is_unidade)
{
   unidade = is_unidade.unidade;

   GGroup<IS_Sala*>::iterator it_sala = is_unidade.salas.begin();

   for(; it_sala != is_unidade.salas.end(); it_sala++)
   {
      salas.add(*it_sala);
   }
}

IS_Unidade::~IS_Unidade(void)
{
}
