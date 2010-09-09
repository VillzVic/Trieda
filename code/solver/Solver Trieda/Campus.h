#pragma once
#include "ofbase.h"

#include "Unidade.h"
#include "Professor.h"
#include "Horario.h"

class Campus:
   public OFBase
{
public:
   Campus(void);
   ~Campus(void);
   
   virtual void le_arvore(ItemCampus& raiz);

//private:
   int id;
   std::string codigo;
   std::string nome;

   GGroup<Unidade*> unidades;
   GGroup<Professor*> professores;
   GGroup<Horario*> horarios;

};