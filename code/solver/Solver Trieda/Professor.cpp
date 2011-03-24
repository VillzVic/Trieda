#include "Professor.h"

Professor::Professor(void)
{
	is_virtual = false;
}

Professor::~Professor(void)
{
}

void Professor::le_arvore(ItemProfessor& elem)
{
   this->setId( elem.id() );

   if(elem.areaTitulacaoId().present())
   {
      area_id = elem.areaTitulacaoId().get();
   }

   tipo_contrato_id = elem.tipoContratoId();
   titulacao_id = elem.titulacaoId();

   cpf = elem.cpf();
   nome = elem.nome();
   ch_min = elem.chMin();
   ch_max = elem.chMax();
   ch_anterior = elem.credAnterior();
   valor_credito = elem.valorCred();
   
   ITERA_SEQ(it_mag,elem.disciplinas(),ProfessorDisciplina)
   {
      Magisterio* m = new Magisterio();
      m->le_arvore(*it_mag);
      magisterio.add(m);
   }

   ITERA_SEQ(it_h,elem.horariosDisponiveis(),Horario)
   {
      Horario* h = new Horario();
      h->le_arvore(*it_h);
      horarios.add(h);
   }
}
