#ifndef HORARIODIA_H_
#define HORARIODIA_H_

#include "ofbase.h"
#include "GGroup.h"

class HorarioAula;

class HorarioDia
{
public:
   HorarioDia();
   ~HorarioDia();

   HorarioDia(const HorarioDia &right);

   bool operator < ( const HorarioDia & right );

   int getId() { return id; }
   void setId(int i) { id = i; }

   int getHorarioAulaId() { return idHorarioAula; }
   void setHorarioAulaId(int i) { idHorarioAula = i; }

   HorarioAula *getHorarioAula() { return horarioAula; }
   void setHorarioAula(HorarioAula* ha) { horarioAula = ha; }

   int getDia() { return dia; }
   void setDia(int d) { dia = d; }

private:
   int id;
   int dia;
   int idHorarioAula;
   HorarioAula *horarioAula;
};

#endif