#ifndef _TURNO_H_
#define _TURNO_H_

#include "ofbase.h"
#include "GGroup.h"
#include "HorarioAula.h"

class Turno :
   public OFBase
{
public:
   Turno(void);
   ~Turno(void);

   virtual void le_arvore(ItemTurno& elem);

   GGroup<HorarioAula*> horarios_aula;

   void setNome(std::string s) { nome = s; }   
   void setTempoAula(int value) { tempo_aula = value; }

   std::string getNome() const { return nome; }
   int getTempoAula() const { return tempo_aula; }

private:
   std::string nome;
   int tempo_aula;
};

#endif // _TURNO_H_