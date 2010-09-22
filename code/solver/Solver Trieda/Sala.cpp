#include "Sala.h"

Sala::Sala(void)
{
}

Sala::~Sala(void)
{
}

void Sala::le_arvore(ItemSala& elem)
{
   bool tatico = true; //fixme!!!

   id = elem.id();
   codigo = elem.codigo();
   andar = elem.andar();
   numero = elem.numero();
   tipo_sala_id = elem.tipoSalaId();
   capacidade = elem.capacidade();
   if(tatico) {
      ITERA_SEQ(it_cred,elem.creditosDispon_veis().get(),CreditoDisponivel) {
         CreditoDisponivel* credito_disp = new CreditoDisponivel;
         credito_disp->le_arvore(*it_cred);
         creditos_disponiveis.add(credito_disp);
      }
   }
   else {
      // le horarios disponiveis 
   }
   ITERA_NSEQ(it_disc,elem.disciplinasAssociadas(),id,Identificador) {
      disciplinas_associadas.add(*it_disc);
   }
}
int Sala::max_creds(int dia) {
   int creds = 0;
   ITERA_GGROUP(it_creds,creditos_disponiveis,CreditoDisponivel) {
      if (it_creds->dia_semana == dia) {
         creds += it_creds->max_creditos;
      }
   }
   return creds;
}
