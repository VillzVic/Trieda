#include "Sala.h"
#include "ConverteHorariosCreditos.h"

Sala::Sala(void)
{
}

Sala::~Sala(void)
{
}

void Sala::le_arvore(ItemSala& elem)
{
	this->setId( elem.id() );
	codigo = elem.codigo();
	andar = elem.andar();
	numero = elem.numero();
	tipo_sala_id = elem.tipoSalaId();
	capacidade = elem.capacidade();

	ITERA_NSEQ(it_disc,elem.disciplinasAssociadas(), id, Identificador)
	{
		disciplinas_associadas.add(*it_disc);
	}
}

// 'possuiOutputTatico'
// TRUE  -> XML de entrada possui a saída do tático
// FALSE -> XML de entrada NÃO possui a saída do tático
// 'modo_operacao' = TATICO ou OPERACIONAL
void Sala::construirCreditosHorarios(ItemSala& elem, std::string modo_operacao, bool possuiOutputTatico)
{
	// Primeiro caso : executar o solver apenas com a entrada do tático
	bool primeiroCaso = ( modo_operacao == "TATICO" );

	// Segundo caso  : executar o solver com a saída do tático e a entrada do operacional
	bool segundoCaso  = ( modo_operacao == "OPERACIONAL" && possuiOutputTatico == true );

	// Terceiro caso : executar o solver apenas com a entrada do operacional (sem saída do tático)
	bool terceiroCaso = ( modo_operacao == "OPERACIONAL" && possuiOutputTatico == false );

	if(primeiroCaso)
	{
		// Lê creditos disponiveis
		if (elem.creditosDispon_veis().present())
		{
			ITERA_SEQ(it_cred, elem.creditosDispon_veis().get(), CreditoDisponivel)
			{
				CreditoDisponivel* credito_disp = new CreditoDisponivel();
				credito_disp->le_arvore(*it_cred);
				creditos_disponiveis.add(credito_disp);
			}
		}

		std::cout << "TATICO" << std::endl;
	}
	else if(segundoCaso)
	{
		// Lê horarios disponiveis
		if (elem.horariosDisponiveis().present())
		{
			ITERA_SEQ(it_hora, elem.horariosDisponiveis().get(), Horario)
			{
				Horario* horario = new Horario();
				horario->le_arvore(*it_hora);
				horarios_disponiveis.add(horario);
				delete horario;
			}
		}

		std::cout << "OPERACIONAL COM OUTPUT TATICO" << std::endl;
	}
	else if(terceiroCaso)
	{
		if (elem.horariosDisponiveis().present())
		{
			// Lê horarios disponiveis
			ITERA_SEQ(it_hora, elem.horariosDisponiveis().get(), Horario)
			{
				Horario* horario = new Horario();
				horario->le_arvore(*it_hora);
				horarios_disponiveis.add(horario);
				delete horario;
			}

			// A partir dos 'horários disponíveis' (input operacional),
			// devemos construir a estrutura de 'créditos disponíveis'
			GGroup<CreditoDisponivel*> creditosDisponiveis
				= converteHorariosParaCreditos();

			ITERA_GGROUP(it_cred, creditosDisponiveis, CreditoDisponivel)
			{
				CreditoDisponivel* credito_disp = new CreditoDisponivel();

				credito_disp->turno_id = it_cred->turno_id;
				credito_disp->dia_semana = it_cred->dia_semana;
				credito_disp->max_creditos = it_cred->max_creditos;

				creditos_disponiveis.add(credito_disp);
				delete credito_disp;
			}
		}

		std::cout << "OPERACIONAL SEM OUTPUT TATICO" << std::endl;
	}
	else
	{
		// ERRO no XML de entrada
		std::cout << "WARNING!!! input inválido para os campos:\n"
				  << "'horariosDisponiveis' e/ou 'creditosDisponiveis'"
				  << std::endl;
	}
}

GGroup<CreditoDisponivel*> Sala::converteHorariosParaCreditos()
{
   //using 

	// Objeto de retorno, contendo os créditos criados
	// a partir dos horários contidos no XML de entrada
	GGroup<CreditoDisponivel*> creditosDisponiveis;

	GGroup<ConverteHorariosCreditos*> groupHorariosCreditos;
	ITERA_GGROUP(it_horario, horarios_disponiveis, Horario)
	{
		GGroup<int>::iterator it_dia_semana = it_horario->dias_semana.begin();
		for (; it_dia_semana != it_horario->dias_semana.end(); it_dia_semana++)
		{
            int diaaaa = *it_dia_semana;

			ConverteHorariosCreditos* item = new ConverteHorariosCreditos();
			item->setTurno( it_horario->getTurnoId() );
			item->setDiaSemana( *it_dia_semana );

			bool found = false;
			ITERA_GGROUP(it,groupHorariosCreditos,ConverteHorariosCreditos)
			{
			   if((it->getDiaSemana() == *it_dia_semana) && (it->getTurno() == it_horario->getTurnoId()))
			   {
			      // Inserir mais um horário em 'groupHorariosCreditos'
			      it->horarios.add( it_horario->getHorarioAulaId() );

			      found = true;
			      break;
			   }
			}

			if(!found)
			{
			   // Adicionar novo item na lista 'groupHorariosCreditos' e inserir o horário				
			   groupHorariosCreditos.add(item);
			   item->horarios.add( it_horario->getHorarioAulaId() );
			}
		}
	}

	// Com a lista de objetos 'ConverteHorariosCreditos'
	// preenchida, construimos agora os crétidos correspondentes
	ITERA_GGROUP(it_converte, groupHorariosCreditos, ConverteHorariosCreditos)
	{
		CreditoDisponivel* credito = new CreditoDisponivel();

		credito->turno_id = it_converte->getTurno();
		credito->dia_semana = it_converte->getDiaSemana();
		credito->max_creditos = it_converte->horarios.size();

		creditosDisponiveis.add( credito );
	}

	return creditosDisponiveis;
}

int Sala::max_creds(int dia)
{
   int creds = 0;
   ITERA_GGROUP(it_creds, creditos_disponiveis, CreditoDisponivel)
   {
      if (it_creds->dia_semana == dia)
	  {
         creds += it_creds->max_creditos;
      }
   }

   return creds;
}
