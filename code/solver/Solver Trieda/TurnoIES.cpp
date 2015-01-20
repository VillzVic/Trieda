#include "TurnoIES.h"
#include "Calendario.h"


TurnoIES::TurnoIES( void )
	: nrHorariosSemana(0)
{
}

TurnoIES::~TurnoIES( void )
{}

void TurnoIES::addHorarioAula( HorarioAula* h )
{ 
	horarios_aula.add(h);
	
	ITERA_GGROUP_N_PT( itDia, h->dias_semana, int )
	{
		mapDiaDateTime[*itDia][h->getInicio()].insert(h);
	}
}

/*
  Dado um dia especifico, retorna o numero de horarios de aula disponiveis.
*/
int TurnoIES::getNroDeHorariosAula(int dia) const
{
	auto finder = mapDiaDateTime.find(dia);
	if (finder != mapDiaDateTime.end())
		return finder->second.size();
	return 0;
}

// Procura o horarioDia nos horarios do turno, ou um igual exceto pelo id.
bool TurnoIES::possuiHorarioDiaOuCorrespondente( HorarioAula *h, int dia ) const
{	
	auto finderDia = mapDiaDateTime.find(dia);
	if (finderDia != mapDiaDateTime.end())
	{
		auto finderDti = finderDia->second.find(h->getInicio());
		if (finderDti != finderDia->second.end())
		{
			for ( auto itHor=finderDti->second.cbegin(); itHor!=finderDti->second.cend(); itHor++ )
			{
				if ((*itHor)->getTempoAula() == h->getTempoAula())
					return true;
			}
		}		
	}
	return false;
}

// Procura o horarioDia nos horarios do turno
bool TurnoIES::possuiHorarioDia( int dia, DateTime dti, DateTime dtf ) const
{	
	auto finderDia = mapDiaDateTime.find(dia);
	if (finderDia != mapDiaDateTime.end())
	{
		auto finderDti = finderDia->second.find(dti);
		if (finderDti != finderDia->second.end())
		{
			for ( auto itHor=finderDti->second.cbegin(); itHor!=finderDti->second.cend(); itHor++ )
			{
				if ((*itHor)->getFinal() == dtf)
					return true;
			}
		}		
	}
	return false;
}

// Procura o horarioDia nos horarios do turno, ou um igual exceto pelo id.
GGroup<HorarioAula*,LessPtr<HorarioAula>> TurnoIES::retornaHorarioDiaOuCorrespondente( HorarioAula *h, int dia ) const
{	
	GGroup<HorarioAula*,LessPtr<HorarioAula>> ggroup;

	auto finderDia = mapDiaDateTime.find(dia);
	if (finderDia != mapDiaDateTime.end())
	{
		auto finderDti = finderDia->second.find(h->getInicio());
		if (finderDti != finderDia->second.end())
		{
			for ( auto itHor=finderDti->second.cbegin(); itHor!=finderDti->second.cend(); itHor++ )
			{
				if ((*itHor)->getTempoAula() == h->getTempoAula())
					ggroup.add( (*itHor) );
			}
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
}

// Procura o horarioDia no calendario, ou um igual exceto pelo id.
HorarioAula* TurnoIES::getHorarioDiaOuCorrespondente(Calendario* const calendario, HorarioAula* const h, int dia)
{	
	return getHorarioDiaOuCorrespondente(calendario, h->getInicio(), dia);
}

HorarioAula* TurnoIES::getHorarioDiaOuCorrespondente(Calendario* const calendario, DateTime dti, int dia)
{
	auto itMapDia = mapDiaDateTime.find(dia);
	if ( itMapDia != mapDiaDateTime.end() )
	{
		auto itMapDateTime = itMapDia->second.find(dti);
		if ( itMapDateTime != itMapDia->second.end() )
		{ 		
			for (auto itHor = itMapDateTime->second.cbegin(); itHor != itMapDateTime->second.cend(); itHor++)
			{
				if ( (*itHor)->getCalendario()->getId() == calendario->getId() )
					return *itHor;
			}
		}		
	}
	return nullptr;
}

// Procura o horarioDia no calendario, ou um igual exceto pelo id.
bool TurnoIES::possuiHorarioDiaOuCorrespondente(Calendario* const calendario, DateTime dti, int dia)
{	
	return (getHorarioDiaOuCorrespondente(calendario, dti, dia) != nullptr);
}

// retorna horarios disponiveis no dia
void TurnoIES::retornaHorariosDisponiveisNoDia(int dia, GGroup<HorarioAula*, LessPtr<HorarioAula>> &horarios) const
{
	auto finderDia = mapDiaDateTime.find(dia);
	if (finderDia != mapDiaDateTime.end())
	{
		for ( auto itDti=finderDia->second.cbegin(); itDti!=finderDia->second.cend(); itDti++ )
		{
			for ( auto itHor=itDti->second.cbegin(); itHor!=itDti->second.cend(); itHor++ )
			{
				horarios.add(*itHor);
			}
		}		
	}
}

// retorna horarios disponiveis no dia
void TurnoIES::retornaHorariosDisponiveisNoDia(int dia, std::map<DateTime,std::set<HorarioAula*>> *horarios)
{
	horarios=nullptr;
	auto finderDia = mapDiaDateTime.find(dia);
	if (finderDia != mapDiaDateTime.end())
	{
		horarios = &finderDia->second;
	}
}