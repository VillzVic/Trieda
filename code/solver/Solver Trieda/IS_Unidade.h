#pragma once

#include "Unidade.h"

#include "IS_Sala.h"

class IS_Unidade : public OFBase
{
public:
   IS_Unidade(Unidade * _unidade);

   IS_Unidade(IS_Unidade const & is_unidade);

   virtual ~IS_Unidade(void);

   Unidade const * unidade;

   GGroup<IS_Sala*> salas;
};
