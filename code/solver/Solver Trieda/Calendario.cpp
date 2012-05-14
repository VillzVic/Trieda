#include "Calendario.h"

#include <iostream>

Calendario::Calendario( void )
{

}

Calendario::~Calendario( void )
{

}

void Calendario::le_arvore( ItemCalendario & elem ) 
{
   this->setId( elem.id() );
   this->setCodigo( elem.codigo() );
   this->setTempoAula( elem.tempoAula() );

   ITERA_SEQ( it_turno, elem.turnos(), Turno )
   {
      Turno * turno = new Turno();
      turno->le_arvore( *it_turno );
      turnos.add( turno );
   }
}

/*
  Dado um dia especifico, retorna o numero de horarios de aula disponiveis no dia da semana letiva em questao
  (ou numero de creditos).
*/
int Calendario::getNroDeHorariosAula(int dia)
{
	int nHorariosNoDia = 0;
	ITERA_GGROUP_LESSPTR( it, turnos, Turno )
	{
		nHorariosNoDia += it->getNroDeHorariosAula(dia);
	}
	return nHorariosNoDia;
}

/*
	Dado um horarioAula h, retorna o proximo horarioAula do turno do calendario.
*/
HorarioAula * Calendario::getProximoHorario( HorarioAula *h )
{	
	HorarioAula * proximoHor = NULL;

	DateTime menorInicio = h->getFinal();
	menorInicio.addMinutes(1840); // adiciona 24hs no dateTime, só para garantir que será um valor suficientemente grande

	DateTime fim = h->getFinal();

	ITERA_GGROUP_LESSPTR( itTurno, turnos, Turno )
	{
		if ( itTurno->horarios_aula.find( h ) != itTurno->horarios_aula.end() )
		{						
			ITERA_GGROUP_LESSPTR( itHorAula, itTurno->horarios_aula, HorarioAula )
			{
				if ( itHorAula->getInicio() == fim )
				{
					return *itHorAula;
				}
				if ( itHorAula->getInicio() > fim &&
					 itHorAula->getInicio() < menorInicio )
				{
					proximoHor = *itHorAula;
					menorInicio = proximoHor->getInicio();
				}
			}
		}
	}

	return proximoHor;
}

/*
	Dados dois horarios-aula de inicio e fim, retorna quantos creditos
	estão delimitados por eles. Se hi = hf, temos 1 credito.
	Se hf não for encontrado após hi, retorna 0.
*/
int Calendario::retornaNroCreditosEntreHorarios( HorarioAula *hi, HorarioAula *hf )
{
	if ( !possuiHorario( hi ) ||
		 !possuiHorario( hf ) )
	{
		std::cout<<"\nAtencao em Calendario::retornaNroCreditosEntreHorarios(): \n";
		std::cout<<"Horario hf ou hi nao encontrado no calendario.\n";
		
		return 0;
	}
	
	if ( *hi > *hf )
	{
		std::cout<<"\nAtencao em Calendario::retornaNroCreditosEntreHorarios(): ";
		std::cout<<"hi > hf\n";
		
		return 0;
	}

	int n = 1;

	HorarioAula *h = hi;
	
	while ( h != NULL && *h != *hf )
	{
		h = getProximoHorario( h );
		n++;	
	}

	if ( h == NULL )
	{
		std::cout<<"\nAtencao em Calendario::retornaNroCreditosEntreHorarios(): \n";
		std::cout<<"O horario hf nao existe apos o hi informado.\n";

		return 0;
	}

	return n;
}


bool Calendario::possuiHorario( HorarioAula *h )
{
	ITERA_GGROUP_LESSPTR( itTurno, turnos, Turno )
	{
		if ( itTurno->horarios_aula.find( h ) != itTurno->horarios_aula.end() )
		{
			return true;
		}
	}

	return false;
}


bool Calendario::intervaloEntreHorarios( HorarioAula *hi, HorarioAula *hf )
{
	int n = retornaNroCreditosEntreHorarios(hi, hf);

	HorarioAula *h1 = hi;
	HorarioAula *h2;

	for ( int i = 1; i < n; i++ )
	{
		h2 = getProximoHorario( h1 );

		DateTime dtf = h1->getFinal();	
		DateTime dti = h2->getInicio();
		
		if ( dtf < dti )
			return true;

		h1 = h2;
	}

	return false;
}

 
int Calendario::getTempoTotal( int dia )
{
	return ( tempo_aula * this->getNroDeHorariosAula(dia) ); 
}

GGroup<HorarioAula*> Calendario::retornaHorariosDisponiveisNoDia( int dia )
{
	GGroup<HorarioAula*> horarios;

	ITERA_GGROUP_LESSPTR( itTurno, turnos, Turno )
	{
		ITERA_GGROUP_LESSPTR( itHor, itTurno->horarios_aula, HorarioAula )
		{		
			if ( itHor->dias_semana.find( dia ) !=  itHor->dias_semana.end() )
			{
				horarios.add( *itHor );
			}
		}
	}

	return horarios;
}