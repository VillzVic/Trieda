#pragma once

#include "Unidade.h"

#include "IS_Sala.h"

//#include "IS_CjtSala.h"

#include "ConjuntoSala.h"

class IS_Unidade : public OFBase
{
public:
   IS_Unidade(Unidade * _unidade);

   IS_Unidade(IS_Unidade const & is_unidade);

   virtual ~IS_Unidade(void);

   //Unidade const * unidade;
   Unidade * unidade;

   GGroup<IS_Sala*> salas;

   //GGroup<IS_CjtSala*> cjt_Salas;

   //GGroup<ConjuntoSala*> * conjutoSalas;


};
