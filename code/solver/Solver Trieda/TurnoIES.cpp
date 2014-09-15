#include "TurnoIES.h"
#include "Calendario.h"


TurnoIES::TurnoIES( void )
	: nrHorariosSemana(0)
{
}

TurnoIES::~TurnoIES( void )
{

}

/*
  Dado um dia especifico, retorna o numero de horarios de aula disponiveis no dia da semana letiva e turno em questao
  (ou numero de creditos).
*/
int TurnoIES::getNroDeHorariosAula(int dia)
{
	int nHorariosNoTurnoDoDia = 0;
	ITERA_GGROUP_LESSPTR( it, horarios_aula, HorarioAula )
	{
		if ( it->horarioDisponivel(dia) ) nHorariosNoTurnoDoDia++;
	}
	return nHorariosNoTurnoDoDia;
}


// Procura o horarioDia nos horarios do turno, ou um igual exceto pelo id.
bool TurnoIES::possuiHorarioDiaOuCorrespondente( HorarioAula *h, int dia )
{	
	ITERA_GGROUP_LESSPTR( itHorAula, horarios_aula, HorarioAula )
	{
		if ( itHorAula->getInicio() == h->getInicio() )
		if ( itHorAula->getTempoAula() == h->getTempoAula() )
		if ( itHorAula->dias_semana.find(dia) != itHorAula->dias_semana.end() )
		{ 
			return true;
		}
	}

	return false;
}


// Procura o horarioDia nos horarios do turno, ou um igual exceto pelo id.
GGroup<HorarioAula*,LessPtr<HorarioAula>> TurnoIES::retornaHorarioDiaOuCorrespondente( HorarioAula *h, int dia )
{	
	GGroup<HorarioAula*,LessPtr<HorarioAula>> ggroup;

	ITERA_GGROUP_LESSPTR( itHorAula, horarios_aula, HorarioAula )
	{
		if ( itHorAula->getInicio() == h->getInicio() )
		if ( itHorAula->getTempoAula() == h->getTempoAula() )
		if ( itHorAula->dias_semana.find(dia) != itHorAula->dias_semana.end() )
		{ 
			ggroup.add( *itHorAula );
		}
	}

	return ggroup;
}


// Verifica se todos os horarioDias entre hi e hf existem (iguais, exceto eventualmente pelo id) no turno.
bool TurnoIES::possuiHorarioDiaOuCorrespondente( HorarioAula *hi, HorarioAula *hf, int dia )
{
	Calendario *calend = hi->getCalendario();
	int nCreds = calend->retornaNroCreditosEntreHorarios( hi, hf );		
							
	bool valid = true;
	HorarioAula *h = hi;
	for ( int j = 1; j <= nCreds; j++ )
	{
		bool possuiHor = this->possuiHorarioDiaOuCorrespondente(h, dia);
		if ( !possuiHor )
			valid = false;
		h = calend->getProximoHorario( h );
	}

	return valid;






	//if ( hi->getCalendario()->getId() != hf->getCalendario()->getId() )
	//{
	//	std::cout << "\nErro em TurnoIES::possuiHorarioDiaOuCorrespondente(), "
	//			  << "calendarios diferentes. So deveria ser par (hi, hf) com mesmo calendario!";
	//	return false;
	//}
	//
	//Calendario *sl = hi->getCalendario();
	//int n = sl->retornaNroCreditosEntreHorarios(hi, hf);

	//HorarioAula* h = hi;
	//bool found = false;

	//for ( int cred = 1; cred <= n; cred++ )
	//{
	//	found = false;
	//	ITERA_GGROUP_LESSPTR( itHorAula, horarios_aula, HorarioAula )
	//	{
	//		if ( itHorAula->getInicio() == h->getInicio() )
	//		if ( itHorAula->getTempoAula() == h->getTempoAula() )
	//		if ( itHorAula->dias_semana.find(dia) != itHorAula->dias_semana.end() )
	//		{
	//			found = true;
	//			break;
	//		}
	//	}
	//	if (!found) break;
	//	if (cred!=n) h = h->getCalendario()->getProximoHorario( h );

	//	if ( h == NULL )
	//		std::cout<<"\nErro! proximo horario null. N="<<n<<", cred="<<cred
	//		<<", hi="<<hi->getId()<<", hf="<<hf->getId()<<", dia="<<dia;
	//}

	//return found;
}

// retorna horarios disponiveis no dia
void TurnoIES::retornaHorariosDisponiveisNoDia( int dia, GGroup<HorarioAula*, LessPtr<HorarioAula>> &horarios) const
{
	ITERA_GGROUP_LESSPTR( it, horarios_aula, HorarioAula )
	{
		if ( it->horarioDisponivel(dia) )
			horarios.add(*it);
	}
}
