#ifndef _CURRICULO_H_
#define _CURRICULO_H_

#include "ofbase.h"
#include "GGroup.h"

typedef std::pair<int/*periodo*/,int/*disciplina_id*/> DisciplinaPeriodo;

class Curriculo :
   public OFBase
{
public:
   Curriculo();
   ~Curriculo(void);
   virtual void le_arvore(ItemCurriculo& elem);

   GGroup<DisciplinaPeriodo> disciplinas_periodo;

   void setCodigo(std::string s) { codigo = s; }
   std::string getCodigo() const { return codigo; }

private:
   std::string codigo;
};

#endif