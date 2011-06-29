#include "IS_Bloco_Curricular.h"

IS_Bloco_Curricular::IS_Bloco_Curricular(BlocoCurricular * blc)
{
   bloco = blc;

   demanda_Total = 0;

   map<Disciplina*,Demanda*>::iterator 
      it_Disciplina_Demanda = bloco->disciplina_Demanda.begin();

   for(;it_Disciplina_Demanda != bloco->disciplina_Demanda.end(); ++it_Disciplina_Demanda)
   {
      disciplina_Dem_Nao_Atendida[it_Disciplina_Demanda->first] = it_Disciplina_Demanda->second->getQuantidade();

      demanda_Total += it_Disciplina_Demanda->second->getQuantidade();
   }

   //atendido = false;
}

IS_Bloco_Curricular::IS_Bloco_Curricular(IS_Bloco_Curricular const & is_Bloco_Curric)
{
   cerr << "COPY CONSTRUCTOR OF <IS_Bloco_Curricular> NOT IMPLEMENTED YET" << endl;
   exit(1);
}

IS_Bloco_Curricular::~IS_Bloco_Curricular(void)
{
}

bool IS_Bloco_Curricular::operator < (IS_Bloco_Curricular const & right)
{
   return bloco->getId() < right.bloco->getId();
}

bool IS_Bloco_Curricular::operator == (IS_Bloco_Curricular const & right)
{
   return bloco->getId() == right.bloco->getId();
}
