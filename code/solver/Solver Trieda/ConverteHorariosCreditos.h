#ifndef _CONVERTEHORARIOSCREDITOS_H_
#define _CONVERTEHORARIOSCREDITOS_H_

#include "ofbase.h"

class ConverteHorariosCreditos :
   public OFBase
{
public:
   ConverteHorariosCreditos(void);
   ~ConverteHorariosCreditos(void);

   // M�todos get
   int getTurno() { return turno; }
   int getDiaSemana() { return dia_semana; }

   // M�todos set
   void setTurno(int t) { turno = t; }
   void setDiaSemana(int ds) { dia_semana = ds; }

   GGroup<int> horarios;

   virtual bool operator < (ConverteHorariosCreditos & right) 
   { 
      return 
         ((turno < right.getTurno()) &&
		 (dia_semana < right.getDiaSemana()));
   }

   virtual bool operator == (ConverteHorariosCreditos & right)
   { 
      return (turno == right.getTurno() &&
			  dia_semana == right.getDiaSemana());
   }

private:
   int turno;
   int dia_semana;
};

#endif