#pragma once
#include "ofbase.h"
#include "Sala.h"
#include "HorarioDisponivel.h"
#include "Curriculo.h"
#include "Demanda.h"

class Unidade :
   public OFBase
{
public:
   Unidade(void);
   ~Unidade(void);
   GGroup<Sala*> salas;
//private:
   std::string codigo;
   std::string endereco;
   int num_med_salas;
   int custo_med_cred;

   GGroup<HorarioDisponivel*> horarios;
   GGroup<Demanda*> demandas; 
//   GGroup<UnidadeCurriculo*> curriculos; TODO
//   GGroup<Deslocamento*> deslocamento; TODO
public:
   virtual void le_arvore(ItemUnidade& elem);
};
