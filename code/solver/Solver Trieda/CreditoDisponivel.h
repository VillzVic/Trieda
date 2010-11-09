#pragma once
#include "ofbase.h"
#include "Turno.h"

class CreditoDisponivel :
   public OFBase
{
public:
   CreditoDisponivel(void);
   ~CreditoDisponivel(void);

   void le_arvore(ItemCreditoDisponivel& elem);

   int turno_id;
   int dia_semana;
   int max_creditos;

   Turno* turno;

};
