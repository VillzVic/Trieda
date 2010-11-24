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

   int totalSalas;
   int maiorSala;

   /* Armazena os dias letivos para um determinado campus */
   GGroup<int> diasLetivos;

public:

   // =========== METODOS SET

   // =========== METODOS GET

   // ToDo : implementar os métodos básicos para todos os membros da classe.

};