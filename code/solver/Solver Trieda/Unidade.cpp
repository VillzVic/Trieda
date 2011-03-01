#include "Unidade.h"

Unidade::Unidade(void)
{
   maiorSala = 0;
}

Unidade::~Unidade(void)
{
	delete sala;
	delete horario;
}

void Unidade::le_arvore(ItemUnidade& elem)
{
   id = elem.id();
   codigo = elem.codigo();
   nome = elem.nome();
   ITERA_SEQ(it_salas, elem.salas(), Sala) {
      sala = new Sala();
      sala->le_arvore(*it_salas);
      salas.add(sala);
   }
   ITERA_SEQ(it_hora,elem.horariosDisponiveis(),Horario) {
      horario = new Horario();
      horario->le_arvore(*it_hora);
      horarios.add(horario);
   }
}
