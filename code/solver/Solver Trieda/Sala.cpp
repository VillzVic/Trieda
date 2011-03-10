#include "Sala.h"

Sala::Sala(void)
{
}

Sala::~Sala(void)
{
}

void Sala::le_arvore(ItemSala& elem)
{
	id = elem.id();
	codigo = elem.codigo();
	andar = elem.andar();
	numero = elem.numero();
	tipo_sala_id = elem.tipoSalaId();
	capacidade = elem.capacidade();

	if(elem.creditosDispon_veis() != NULL && elem.horariosDisponiveis() == NULL) {
		// le creditos disponiveis
      ITERA_SEQ(it_cred,elem.creditosDispon_veis().get(),CreditoDisponivel) {
			CreditoDisponivel* credito_disp = new CreditoDisponivel();
			credito_disp->le_arvore(*it_cred);
			creditos_disponiveis.add(credito_disp);
		}
	  std::cout << "TATICO" << std::endl;
	}
	else if(elem.creditosDispon_veis() == NULL && elem.horariosDisponiveis() != NULL) {
		// le horarios disponiveis
		ITERA_SEQ(it_hora,elem.horariosDisponiveis().get(),Horario) {
			Horario* horario = new Horario();
			horario->le_arvore(*it_hora);
			horarios_disponiveis.add(horario);
		}
		std::cout << "OPERACIONAL" << std::endl;
	}
	else
	// ERRO no XML de entrada
	{
		std::cout << "WARNING!!! input inválido para os campos:\n"
				  << "'horariosDisponiveis' e/ou 'creditosDisponiveis'"
				  << std::endl;
		getchar();
	}
	ITERA_NSEQ(it_disc,elem.disciplinasAssociadas(),id,Identificador) {
		disciplinas_associadas.add(*it_disc);
	}
}

int Sala::max_creds(int dia)
{
   int creds = 0;

   ITERA_GGROUP(it_creds,creditos_disponiveis,CreditoDisponivel) {
      if (it_creds->dia_semana == dia) {
         creds += it_creds->max_creditos;
      }
   }
   return creds;
}
