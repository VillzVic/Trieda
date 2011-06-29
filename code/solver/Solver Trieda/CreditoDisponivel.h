#ifndef _CREDITO_DISPONIVEL_H_
#define _CREDITO_DISPONIVEL_H_

#include "ofbase.h"
#include "Turno.h"

class CreditoDisponivel :
   public OFBase
{
public:
   CreditoDisponivel(void);
   virtual ~CreditoDisponivel(void);

   void le_arvore( ItemCreditoDisponivel & );

   Turno * turno;

   void setTurnoId(int value) { turno_id = value; }
   void setDiaSemana(int value) { dia_semana = value; }
   void setMaxCreditos(int value) { max_creditos = value; }

   int getTurnoId() const { return turno_id; }
   int getDiaSemana() const { return dia_semana; }
   int getMaxCreditos() const { return max_creditos; }

private:
   int turno_id;
   int dia_semana;
   int max_creditos;
};

#endif
