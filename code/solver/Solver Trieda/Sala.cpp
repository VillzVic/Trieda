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
// TRUE  -> XML de entrada possui a sa�da do t�tico
// FALSE -> XML de entrada N�O possui a sa�da do t�tico
// 'modo_operacao' = TATICO ou OPERACIONAL
void Sala::construirCreditosHorarios(ItemSala& elem, std::string modo_operacao, bool possuiOutputTatico)
{
	// Primeiro caso : executar o solver apenas com a entrada do t�tico
	bool primeiroCaso = ( modo_operacao == "TATICO" );

	// Segundo caso  : executar o solver com a sa�da do t�tico e a entrada do operacional
	bool segundoCaso  = ( modo_operacao == "OPERACIONAL" && possuiOutputTatico == true );

	// Terceiro caso : executar o solver apenas com a entrada do operacional (sem sa�da do t�tico)
	bool terceiroCaso = ( modo_operacao == "OPERACIONAL" && possuiOutputTatico == false );

	if( primeiroCaso )
	{
		// L� creditos disponiveis
		if ( elem.creditosDispon_veis().present() )
		{
			ITERA_SEQ(it_cred, elem.creditosDispon_veis().get(), CreditoDisponivel)
			{
				CreditoDisponivel * credito_disp = new CreditoDisponivel();
				credito_disp->le_arvore( *it_cred );
				creditos_disponiveis.add( credito_disp );
			}
		}
	}
	else if( segundoCaso )
	{
		// L� horarios disponiveis
		if ( elem.horariosDisponiveis().present() )
		{
			ITERA_SEQ(it_hora, elem.horariosDisponiveis().get(), Horario)
			{
				Horario * horario = new Horario();
				horario->le_arvore( *it_hora );
				horarios_disponiveis.add( horario );
			}
		}
	}
	else if( terceiroCaso )
	{
		if ( elem.horariosDisponiveis().present() )
		{
			// L� horarios disponiveis
			ITERA_SEQ(it_hora, elem.horariosDisponiveis().get(), Horario)
			{
				Horario * horario = new Horario();
				horario->le_arvore( *it_hora );
				horarios_disponiveis.add( horario );
			}

			// A partir dos 'hor�rios dispon�veis' (input operacional),
			// devemos construir a estrutura de 'cr�ditos dispon�veis'
			GGroup< CreditoDisponivel * > creditosDisponiveis
				= converteHorariosParaCreditos();

			ITERA_GGROUP(it_cred, creditosDisponiveis, CreditoDisponivel)
			{
				CreditoDisponivel * credito_disp = new CreditoDisponivel();

				credito_disp->setTurnoId( it_cred->getTurnoId() );
				credito_disp->setDiaSemana( it_cred->getDiaSemana() );
				credito_disp->setMaxCreditos( it_cred->getMaxCreditos() );

				creditos_disponiveis.add( credito_disp );
			}
		}
	}
}

GGroup< CreditoDisponivel * > Sala::converteHorariosParaCreditos()
{
	// Objeto de retorno, contendo os cr�ditos criados
	// a partir dos hor�rios contidos no XML de entrada
	GGroup< CreditoDisponivel * > creditosDisponiveis;

	GGroup< ConverteHorariosCreditos * > groupHorariosCreditos;
	ITERA_GGROUP(it_horario, horarios_disponiveis, Horario)
	{
		GGroup< int >::iterator it_dia_semana = it_horario->dias_semana.begin();
		for (; it_dia_semana != it_horario->dias_semana.end();
			   it_dia_semana++)
		{
            int diaaaa = *(it_dia_semana);

			ConverteHorariosCreditos * item = new ConverteHorariosCreditos();
			item->setTurno( it_horario->getTurnoId() );
			item->setDiaSemana( *it_dia_semana );

			bool found = false;
			ITERA_GGROUP(it, groupHorariosCreditos, ConverteHorariosCreditos)
			{
			   if((it->getDiaSemana() == *it_dia_semana)
					&& (it->getTurno() == it_horario->getTurnoId()))
			   {
			      // Inserir mais um hor�rio em 'groupHorariosCreditos'
			      it->horarios.add( it_horario->getHorarioAulaId() );

			      found = true;
			      break;
			   }
			}

			if( !found )
			{
			   // Adicionar novo item na lista
			   // 'groupHorariosCreditos' e inserir o hor�rio				
			   groupHorariosCreditos.add( item );
			   item->horarios.add( it_horario->getHorarioAulaId() );
			}
		}
	}

	// Com a lista de objetos 'ConverteHorariosCreditos'
	// preenchida, construimos agora os cr�tidos correspondentes
	ITERA_GGROUP(it_converte, groupHorariosCreditos, ConverteHorariosCreditos)
	{
		CreditoDisponivel * credito = new CreditoDisponivel();

		credito->setTurnoId( it_converte->getTurno() );
		credito->setDiaSemana( it_converte->getDiaSemana() );
		credito->setMaxCreditos( it_converte->horarios.size() );

		creditosDisponiveis.add( credito );
	}

	return creditosDisponiveis;
}

int Sala::max_creds(int dia)
{
   int creds = 0;
   ITERA_GGROUP(it_creds, creditos_disponiveis, CreditoDisponivel)
   {
      if ( it_creds->getDiaSemana() == dia )
	  {
         creds += it_creds->getMaxCreditos();
      }
   }

   return creds;
}
