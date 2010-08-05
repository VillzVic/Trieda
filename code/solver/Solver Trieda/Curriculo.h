#pragma once
#include "ofbase.h"

class Curso;
#include "Curso.h"
class Disciplina;
#include "Disciplina.h"

typedef std::pair<int,Disciplina*> DisciplinaPeriodo;

class Curriculo :
   public OFBase
{
public:
   Curriculo(Curso* curso);
   ~Curriculo(void);
   virtual void le_arvore(ItemCurriculo& elem);

//private:

   Curso* curso; //curriculo precisa de reflexão
   std::string codigo;
   std::string descricao;
   GGroup<DisciplinaPeriodo> disciplinas_periodo;
};
