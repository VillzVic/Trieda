#include "Unidade.h"

Unidade::Unidade(void)
{
}

Unidade::~Unidade(void)
{
}

void Unidade::le_arvore(ItemUnidade& elem)
{
   id = elem.id();
   codigo = elem.codigo();
   endereco = elem.endereco();
   num_med_salas = elem.numMedSalas();
   custo_med_cred = elem.custoMedCred();
   ITERA_SEQ(it_salas, elem.salas(), Sala) {
      Sala* sala = new Sala();
      sala->le_arvore(*it_salas);
      salas.add(sala);
   }
   ITERA_SEQ(it_hora,elem.horarios(),HorarioDisponivel) {
      HorarioDisponivel* horario = new HorarioDisponivel();
      horario->le_arvore(*it_hora);
      horarios.add(horario);
   }
}
