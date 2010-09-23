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
   std::string codigo;
   std::string nome;

   GGroup<Unidade*> unidades;
   GGroup<Professor*> professores;
   GGroup<Horario*> horarios;

   int total_salas;
   int maior_sala;

};