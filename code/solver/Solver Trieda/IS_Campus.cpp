#include "IS_Campus.h"

IS_Campus::IS_Campus(void)
{
}

IS_Campus::IS_Campus(IS_Campus const & is_campus)
{
   campus = is_campus.campus;

   GGroup<IS_Unidade*>::iterator it_unidade = is_campus.unidades.begin();

   for(; it_unidade != is_campus.unidades.end(); it_unidade++)
   {
      unidades.add(*it_unidade);
   }
}

IS_Campus::~IS_Campus(void)
{
}