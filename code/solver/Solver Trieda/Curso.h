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

   GGroup<int> area_ids;
   GGroup<Curriculo*> curriculos;
   TipoCurso* tipo_curso;

   std::pair<int,double> regra_min_mestres;
   std::pair<int,double> regra_min_doutores;

   void setCodigo(std::string s) { codigo = s; }
   void setTipoId(int v) { tipo_id = v; }
   void setNumPeriodos(int v) { num_periodos = v; }
   void setQtdMaxProfDisc(int v) { qtd_max_prof_disc = v; }
   void setMaisDeUma(bool v) { mais_de_uma = v; }

   std::string getCodigo() const { return codigo; }
   int getTipoId() const { return tipo_id; }
   int getNumPeriodos() const { return num_periodos; }
   int getQtdMaxProfDisc() const { return qtd_max_prof_disc; }
   bool getMaisDeUma() const { return mais_de_uma; }

private:
   std::string codigo;
   int tipo_id;
   int num_periodos;
   int qtd_max_prof_disc;
   bool mais_de_uma;
};
