#pragma once
#include "ofbase.h"
#include "AreaTitulacao.h"
#include "Curriculo.h"
#include "TipoCurso.h"


class Curso :
   public OFBase
{
public:
   Curso(void);
   ~Curso(void);
   virtual void le_arvore(ItemCurso& elem);

//private:
   int id;
   std::string codigo;
   TipoCurso* tipo;
   int num_periodos;
   int qtd_min_doutores;
   int qtd_min_mestres;
   int qtd_max_prof_disc;
   AreaTitulacao* area_titulacao;
   GGroup<Curriculo*> curriculos;

};
