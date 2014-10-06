#include "Professor.h"

Professor::Professor( bool eVirtual )
	: is_virtual(eVirtual), nroCredsCadastroDisc_(0)
{
    this->cpf = "";
	this->nome = "";
	this->tipo_contrato_id = 0;
	this->ch_min = 0;
	this->ch_max = 10000;
	this->ch_anterior = 0;
	this->titulacao_id = 0;
	this->area_id = 0;
	this->valor_credito = 0;

   this->tipo_contrato = NULL;
   this->titulacao = NULL;
   this->area = NULL;

	this->id_operacional = -1;
	this->max_dias_semana = 5;
	this->min_creds_diarios = 1;
	this->curso = NULL;

	// Parametro utilizado na função de
	// prioridade para o modelo operacional.
	this->custoDispProf = 0;

	if ( this->is_virtual )
	{
      this->cpf = "000.000.000-00";
      this->nome = "PROFESSOR VIRTUAL";
      this->tipo_contrato_id = 1;
      this->ch_min = 0;
      this->valor_credito = 500;
	  this->titulacao_id = TipoTitulacao::Licenciado;

	   // 7 dias na semana, 4 creditos por dia
      this->ch_max = 40; 
	}
}

Professor::~Professor( void )
{

}

void Professor::le_arvore( ItemProfessor & elem )
{
	this->setId( elem.id() );

	if ( elem.areaTitulacaoId().present() )
	{
		this->area_id = elem.areaTitulacaoId().get();
	}

	this->tipo_contrato_id = elem.tipoContratoId();
	this->titulacao_id = elem.titulacaoId();

	this->cpf = elem.cpf();
	this->nome = elem.nome();
	this->ch_min = elem.chMin();
	this->ch_max = elem.chMax();
	this->ch_anterior = elem.credAnterior();
	this->valor_credito = elem.valorCred();

	int id_magisterio = 1;
	this->max_dias_semana = elem.maxDiasSemana();
	this->min_creds_diarios = elem.minCredsDiarios();

	ITERA_SEQ( it_mag, elem.disciplinas(), ProfessorDisciplina )
	{
		Magisterio * m = new Magisterio();

		m->setId( id_magisterio );
		id_magisterio++;

		m->le_arvore( *it_mag );
		this->magisterio.add( m );
	}

	ITERA_SEQ( it_h, elem.horariosDisponiveis(), Horario )
	{
		Horario * h = new Horario();
		h->le_arvore( *it_h );
		this->horarios.add( h );
	}
}


void Professor::addDisponibilidade( int dia, DateTime dti, DateTime dtf )
{
	std::map<DateTime,GGroup<DateTime>> empty1;
	GGroup<DateTime> empty2;

	auto itDia = mapDiaDtiDtf.find(dia);
	if (itDia == mapDiaDtiDtf.end())
		itDia = mapDiaDtiDtf.insert( pair<int, std::map<DateTime,GGroup<DateTime>>> (dia, empty1) ).first;

	auto itDti = itDia->second.find(dti);
	if (itDti == itDia->second.end())
	{
		itDti = itDia->second.insert( pair<DateTime,GGroup<DateTime>> (dti, empty2) ).first;
		nroCredsCadastroDisc_++;
	}

	itDti->second.add(dtf);

}

HorarioAula* Professor::getPrimeiroHorarioDisponivelDia( int dia )
{
	HorarioAula *hi;
	bool ACHEI = false;

	// Acha um horario inicial qualquer no dia
	ITERA_GGROUP_LESSPTR( it_h, horariosDia, HorarioDia )
	{
		HorarioDia * h = *it_h;
		if ( h->getDia() == dia )
		{
			hi = h->getHorarioAula();
			ACHEI = true;
			break;
		}
	}

	if ( !ACHEI )
	{
		std::cout<<"\nATENCAO em getPrimeiroHorarioDisponivelDia: nenhum horario disponivel no dia foi encontrado.\n";
		return NULL;
	}

	// Seleciona o menor horario existente no dia
	ITERA_GGROUP_LESSPTR( it_h, horariosDia, HorarioDia )
	{
		HorarioDia * h = *it_h;
		if ( h->getDia() == dia )
		{
			if ( h->getHorarioAula()->getInicio() < hi->getInicio() )
				hi = h->getHorarioAula();
		}
	}

	return hi;
}

HorarioAula* Professor::getUltimoHorarioDisponivelDia( int dia )
{
	HorarioAula *hf;
	bool ACHEI = false;

	// Acha um horario inicial qualquer no dia
	ITERA_GGROUP_LESSPTR( it_h, horariosDia, HorarioDia )
	{
		HorarioDia * h = *it_h;
		if ( h->getDia() == dia )
		{
			hf = h->getHorarioAula();
			ACHEI = true;
			break;
		}
	}

	if ( !ACHEI )
	{
		std::cout<<"\nATENCAO em getUltimoHorarioDisponivelDia: nenhum horario disponivel no dia foi encontrado.\n";
		return NULL;
	}

	// Seleciona o maior horario existente no dia
	ITERA_GGROUP_LESSPTR( it_h, horariosDia, HorarioDia )
	{
		HorarioDia * h = *it_h;
		if ( h->getDia() == dia )
		{
			if ( h->getHorarioAula()->getInicio() > hf->getInicio() )
				hf = h->getHorarioAula();
		}
	}

	return hf;
}

 bool Professor::possuiMagisterioEm( Disciplina* disciplina )
 {
	ITERA_GGROUP_LESSPTR( itMagist, magisterio, Magisterio )
	{
		if ( itMagist->disciplina == disciplina )
			return true;
	}
	return false;
 }


GGroup< HorarioDia*,LessPtr<HorarioDia> > Professor::getHorariosAnterioresDisponivelDia( DateTime inicio, int dia )
{
	GGroup< HorarioDia*,LessPtr<HorarioDia> > horariosAnteriores;

	// ignora o dia do DateTime inicio!

	int hour = inicio.getHour();
	int min = inicio.getMinute();

	ITERA_GGROUP_LESSPTR( it_h, horariosDia, HorarioDia )
	{
		HorarioDia * h = *it_h;
		if ( h->getDia() == dia )
		{
			int profHour = h->getHorarioAula()->getInicio().getHour();
			
			if ( profHour < hour )
			{
				horariosAnteriores.add( h );
			}
			else if ( profHour == hour )
			{
				int profMin = h->getHorarioAula()->getInicio().getMinute();

				if ( profMin < min )
				{
					horariosAnteriores.add( h );
				}
			}
		}
	}

	return horariosAnteriores;
}

GGroup< HorarioDia*,LessPtr<HorarioDia> > Professor::getHorariosPosterioresDisponivelDia( DateTime inicio, int dia )
{
	GGroup< HorarioDia*,LessPtr<HorarioDia> > horariosPosteriores;

	// ignora o dia do DateTime inicio!

	int hour = inicio.getHour();
	int min = inicio.getMinute();

	ITERA_GGROUP_LESSPTR( it_h, horariosDia, HorarioDia )
	{
		HorarioDia * h = *it_h;
		if ( h->getDia() == dia )
		{
			int profHour = h->getHorarioAula()->getInicio().getHour();
			
			if ( profHour > hour )
			{
				horariosPosteriores.add( h );
			}
			else if ( profHour == hour )
			{
				int profMin = h->getHorarioAula()->getInicio().getMinute();

				if ( profMin > min )
				{
					horariosPosteriores.add( h );
				}
			}
		}
	}

	return horariosPosteriores;
}

bool Professor::possuiHorariosNoDia( HorarioAula *const hi, HorarioAula *const hf, int dia ) const
{		
	auto itDia = mapDiaDtiDtf.find( dia );

	 if ( itDia != mapDiaDtiDtf.end() )
	 {
		 HorarioAula *h = hi;

		 Calendario *calendario = h->getCalendario();
		 
		 while ( h!=NULL )
		 {
			 DateTime dti = h->getInicio();
			 DateTime dtf = h->getFinal();

			 auto *mapDti = &itDia->second;			 
			 auto itHi = mapDti->find( dti );

			 if ( itHi != mapDti->end() )
			 {				 
				 auto itHf = itHi->second.find( dtf );

				 if ( itHf != itHi->second.end() )
				 {					 
					 if ( h->inicioFimIguais(*hf) )
						 return true;

					 h = calendario->getProximoHorario( h );
				 }
				 else return false;
			 }
			 else return false;
		 }
	 }

	 return false;
}