#pragma once
#include "ofbase.h"
#include "TipoCurso.h"
#include "Curriculo.h"

class Curso :
   public OFBase
{
public:
   Curso(void);
   ~Curso(void);
   virtual void le_arvore(ItemCurso& elem);

//private:
   std::string codigo;
   int tipo_id;
   int num_periodos;
   int qtd_max_prof_disc;
   bool mais_de_uma;
   std::pair<int,double> regra_min_mestres;
   std::pair<int,double> regra_min_doutores;

   GGroup<int> area_ids;
   GGroup<Curriculo*> curriculos;
   TipoCurso* tipo_curso;
};
