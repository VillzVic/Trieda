#include <iostream>
#include <fstream>

#include "Calendario.h"
#include "CentroDados.h"


Calendario::Calendario( void )
{

}

Calendario::~Calendario( void )
{}

void Calendario::le_arvore( ItemCalendario & elem ) 
{
   this->setId( elem.id() );
   this->setCodigo( elem.codigo() );
   this->setTempoAula( elem.tempoAula() );

   ITERA_SEQ( it_turno, elem.turnos(), TurnoIES )
   {     
	  // Le arvore do turno -----
	  ItemTurno elemTurno = *it_turno;
	  ITERA_SEQ( it_horarios, elemTurno.HorariosAula(), HorarioAula )
	  {
		HorarioAula * horario = new HorarioAula();
		horario->le_arvore( *it_horarios );
		horario->setTurnoIESId( elemTurno.id() );

		this->horarios_aula.add( horario );
		
		ITERA_GGROUP_N_PT( itDia, horario->dias_semana, int )
		{
			this->mapDiaDateTime[*itDia][horario->getInicio()] = horario;
		}
	  }
	  // ------------------------
   }

   if ( elem.permiteIntervaloEmAula().present() )
	   this->RESTRINGE_AULA_COM_INTERVALO = ! elem.permiteIntervaloEmAula();
   else
	   this->RESTRINGE_AULA_COM_INTERVALO = true;

   calculaProximosHorarioAula();

#ifdef KROTON
  // if ( this->getTempoAula() == 75 ) // TODO
  //	RESTRINGE_AULA_COM_INTERVALO = false;	
#endif

}

/*
  Dado um dia especifico, retorna o numero de horarios de aula disponiveis no dia da semana letiva em questao
  (ou numero de creditos).
*/
int Calendario::getNroDeHorariosAula(int dia)
{
	int nHorariosNoTurnoDoDia = 0;
	std::map< int/*dia*/, std::map< DateTime, HorarioAula* > >::iterator
		itMapDia = mapDiaDateTime.find(dia);
	if( itMapDia != mapDiaDateTime.end() )
	{
		nHorariosNoTurnoDoDia += (int) itMapDia->second.size();		
	}

	return nHorariosNoTurnoDoDia;
}

/*
	Dado um horarioAula h, retorna o proximo horarioAula do turno do calendario.
*/

void Calendario::calculaProximosHorarioAula()
{
#ifdef PRINT_LOGS
	std::ofstream outFile;
	std::stringstream ssName;
	ssName << "proximoHorarioCalendarios.txt";
	outFile.open( ssName.str(), std::ios::app );

	outFile << "------------------------\n";
	outFile << "Calendario " << this->getId() << std::endl << std::endl;

#endif
	ITERA_GGROUP_LESSPTR( itHorarioAula, this->horarios_aula, HorarioAula )
	{
		HorarioAula *h = *itHorarioAula;

		HorarioAula * proximoHor = NULL;
		DateTime menorInicio = h->getInicio();
		menorInicio.addMinutes(1840*2); // adiciona 48hs no dateTime, só para garantir que será um valor suficientemente grande

		DateTime fim = h->getInicio();
		fim.addMinutes( (int) this->getTempoAula() ); // Fim da aula

		ITERA_GGROUP_LESSPTR( itHorAula, this->horarios_aula, HorarioAula )
		{
			if ( itHorAula->getInicio() == fim )
			{
				if ( itHorAula->getTurnoIESId() == h->getTurnoIESId() )
				{
					 proximoHor = *itHorAula;
					 break;
				}
				else
				{
					proximoHor = *itHorAula;	// tenta mais, para ver se tem hor do mesmo turno
					menorInicio = proximoHor->getInicio();
				}
			}
			if ( itHorAula->getInicio() > fim &&
				 itHorAula->getInicio() <= menorInicio )
			{
				if ( itHorAula->getInicio() < menorInicio )
				{
					proximoHor = *itHorAula;
					menorInicio = proximoHor->getInicio();
				}
				else
				{
					// prefere pegar o mesmo turno
					if ( itHorAula->getTurnoIESId() == h->getTurnoIESId() &&
						 proximoHor->getTurnoIESId() != h->getTurnoIESId() )
					{
						proximoHor = *itHorAula;
						menorInicio = proximoHor->getInicio();
					}
				}
			}
		}

#ifdef PRINT_LOGS
		if (outFile )
		{
			outFile << h->getId() << "(" << h->getInicio() << ")  ->  ";
			if ( proximoHor !=NULL ) 
				outFile << proximoHor->getId() << "(" << proximoHor->getInicio() << ")\n" ;
			else 
				outFile << "NULL\n";
		}
#endif

		mapProximoHorarioAula[h->getId()] = proximoHor;
	}

#ifdef PRINT_LOGS
	if ( outFile )
	{
		outFile.close();
	}
#endif
}


/*
	Dado um horarioAula h, retorna o proximo horarioAula do turno do calendario.
*/
HorarioAula * Calendario::getProximoHorario( HorarioAula *h ) const
{	
	HorarioAula * proximoHor = NULL;

	auto it = mapProximoHorarioAula.find( h->getId() );
	if ( it != mapProximoHorarioAula.end() )
	{
		proximoHor = it->second;
	}
	else
	{
		std::cout<<"\nErro. HorarioAula nao encontrado no calendario:\nh = " 
			<< h->getId() << ", " << h->getInicio() << ", calend de h: " << h->getCalendario()->getId();
		fflush(NULL);
	}

	return proximoHor;
}



/*
	Dado um horarioAula h, retorna o horarioAula anterior do calendario.
*/
HorarioAula * Calendario::getHorarioAnterior( HorarioAula *h )
{	
	HorarioAula * horAnterior = NULL;

	if ( this->horarios_aula.find( h ) != this->horarios_aula.end() )
	{						
		ITERA_GGROUP_LESSPTR( itHorAula, this->horarios_aula, HorarioAula )
		{
			if ( this->getProximoHorario( *itHorAula ) == h )
			{
				horAnterior = *itHorAula;
				break;
			}
		}
	}

	if ( horAnterior != NULL )
	if ( horAnterior->getFinal().getDateMinutes() + h->getTempoAula() <= h->getInicio().getDateMinutes() )	
	{
		// Gap grande entre os horarios
		return NULL;
	}

	return horAnterior;
}


/*
	Dados dois horarios-aula de inicio e fim, retorna quantos creditos
	estão delimitados por eles. Se hi = hf, temos 1 credito.
	Se hf não for encontrado após hi, retorna 0.
*/
int Calendario::retornaNroCreditosEntreHorarios( HorarioAula *hi, HorarioAula *hf )
{
	if ( hi == NULL || hf == NULL ) return 0;

	std::pair<HorarioAula*, HorarioAula*> parHorarios( hi, hf );
	if ( this->horarios_nroCreds.find( parHorarios ) != this->horarios_nroCreds.end() )
	{
		return horarios_nroCreds[ parHorarios ];
	}

	if ( !possuiHorario( hi ) ||
		 !possuiHorario( hf ) )
	{
		std::stringstream msg;
		msg <<"Horario hf ou hi nao encontrado no calendario.\n";		
		CentroDados::printError("Calendario::retornaNroCreditosEntreHorarios()",msg.str());
		return 0;
	}	
	if ( *hi > *hf && !hi->inicioFimIguais(*hf) )
	{
		std::stringstream msg;
		msg << "hi > hf\n" <<"hi = "<<hi->getInicio() <<"\thf = "<<hf->getInicio();
		CentroDados::printError("Calendario::retornaNroCreditosEntreHorarios()",msg.str());
		return 0;
	}
	else if ( hi->inicioFimIguais(*hf) )
		return 1;


	int n = 1;
	HorarioAula *h = hi;	
	while ( h != NULL && !h->inicioFimIguais(*hf) )
	{
		h = getProximoHorario( h );
		n++;	
	}

	if ( h == NULL )
	{
		std::cout<<"\nAtencao em Calendario::retornaNroCreditosEntreHorarios( HorarioAula *hi, HorarioAula *hf ): \n";
		std::cout<<"O horario hf nao existe apos o hi informado.\n";

		return 0;
	}

	horarios_nroCreds[ parHorarios ] = n;

	return n;
}


bool Calendario::possuiHorario( HorarioAula *h )
{
	if ( this->horarios_aula.find( h ) != this->horarios_aula.end() )
	{
		return true;
	}

	return false;
}


bool Calendario::possuiHorarioDia( HorarioAula *h, int dia )
{
	GGroup< HorarioAula *, LessPtr< HorarioAula > >::iterator
		it = this->horarios_aula.find( h );
	if ( it != this->horarios_aula.end() )
	{
		if ( it->dias_semana.find( dia ) != it->dias_semana.end() )
			return true;
		else
			return false;
	}
	return false;
}

// Procura o horarioDia no calendario, ou um igual exceto pelo id.
HorarioAula* Calendario::possuiHorarioDiaOuCorrespondente( HorarioAula *h, int dia )
{		
	if ( this->getTempoAula() != h->getCalendario()->getTempoAula() ) 
	{
		return false;
	}

	std::map< int/*dia*/, std::map< DateTime, HorarioAula* > >::iterator itMapDia = this->mapDiaDateTime.find(dia);
	if ( itMapDia != this->mapDiaDateTime.end() )
	{
		std::map< DateTime, HorarioAula* >::iterator itMapDateTime = itMapDia->second.find( h->getInicio() );
		if ( itMapDateTime != itMapDia->second.end() )
		{ 
			return itMapDateTime->second;
		}
	}
	return NULL;
}

// Procura os horarioDias entre hi e hf no calendario, ou iguais exceto pelo id.
// Usado quando hi e hf são de calendarios diferentes de this
bool Calendario::possuiHorarioDiaOuCorrespondente( HorarioAula *hi, HorarioAula *hf, int dia )
{
	if ( hi->getCalendario()->getId() != hf->getCalendario()->getId() )
	{
		std::cout<<"\nErro, calendarios diferentes. So deveria ser par (hi, hf) com mesmo calendario!";
		return false;
	}

	if ( hi->getCalendario()->getId() == this->getId() )
	{
		return true;
	}
	
	if ( this->getTempoAula() != hi->getCalendario()->getTempoAula() ) 
	{
		return false;
	}
	
	Calendario *sl = hi->getCalendario();
	int n = sl->retornaNroCreditosEntreHorarios(hi, hf);

	HorarioAula* thisHi=NULL;
	HorarioAula* thisHf=NULL;

	bool hiFound=false;
	bool hfFound=false;
	
	std::map< int/*dia*/, std::map< DateTime, HorarioAula* > >::iterator itMapDia = this->mapDiaDateTime.find(dia);
	if ( itMapDia != this->mapDiaDateTime.end() )
	{
		// Procura hi
		std::map< DateTime, HorarioAula* >::iterator itMapDateTime = itMapDia->second.find( hi->getInicio() );
		if ( itMapDateTime != itMapDia->second.end() )
		{
			thisHi = itMapDateTime->second;	hiFound=true;
			
			int k = 1;	
			HorarioAula *thisH = thisHi;
			HorarioAula *h = hi;

			// Continua (já que o map é ordenado), procurando hf			
			for ( ; itMapDateTime != itMapDia->second.end(); itMapDateTime++ )
			{ 
				thisH = itMapDateTime->second;
				DateTime thisDateTime = itMapDateTime->first;

				if( h==NULL ) return false; 

				if ( thisH->inicioFimIguais( *h ) ) 
				{
					if ( k==n )
					{
						if ( thisDateTime == hf->getInicio() )
							return true;

						std::cout<<"\nAtencao! Isso deveria ocorrer?? n="<<n<<", thisH="<<thisH->getId()<<", hi="<<hi->getId()<<", hf="<<hf->getId()<<", dia="<<dia;
						return false;
					}
					
					h = sl->getProximoHorario( h );
					k++;
				}
				else return false;				
			}
		}
	}

	return false;

	//if ( thisHi==NULL || thisHf==NULL ) return false;

	//HorarioAula *thisH = thisHi;
	//HorarioAula *h = hi;


	//for ( int i = 1; i < n; i++ )
	//{
	//	if( thisH==NULL || h==NULL ) return false; 

	//	if ( thisH->inicioFimIguais( *h ) && 
	//		 thisH->dias_semana.find( dia ) != thisH->dias_semana.end() ) 
	//	{
	//		h = sl->getProximoHorario( h );
	//		thisH = this->getProximoHorario( thisH );		
	//	}
	//	else return false;
	//}
	//
	//return true;
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

 
double Calendario::getTempoTotal( int dia )
{
	return ( tempo_aula * this->getNroDeHorariosAula(dia) ); 
}

GGroup<HorarioAula*, LessPtr<HorarioAula>> Calendario::retornaHorariosDisponiveisNoDia( int dia )
{
	GGroup<HorarioAula*, LessPtr<HorarioAula>> horarios;
	
	std::map< int/*dia*/, std::map< DateTime, HorarioAula* > >::iterator itMapDia = this->mapDiaDateTime.find(dia);
	if ( itMapDia != this->mapDiaDateTime.end() )
	{
		std::map< DateTime, HorarioAula* >::iterator itMapDateTime = itMapDia->second.begin();
		for ( ; itMapDateTime != itMapDia->second.end(); itMapDateTime++ )
		{ 
			horarios.add( itMapDateTime->second );
		}
	}

	return horarios;
}

/*
	HorarioAula *h	: horarioAula inicial
	int nCreds		: numero não-negativo de creditos a serem acrescentados à h
	Return 			: horarioAula que está nCreds à frente de h

	Exemplo:
	- se nCreds = 0, retorna o proprio horario inicial
	- se nCreds = 1, retorna o horario seguinte ao horario inicial
	...
*/
HorarioAula* Calendario::getHorarioMaisNCreds( HorarioAula *h, int nCreds )
{	
	HorarioAula *hf = h;

	for ( int i=1; i<=nCreds; i++ )
	{
		if ( hf == NULL ) return NULL;

		hf = this->getProximoHorario( hf );
	}

	return hf;
}


/*
	Dado um grupo de calendarios, retorna um map contendo, para cada dia, a lista de HorariosAula
	que são comuns em todos os calendarios (incluindo o calendario this).
*/
std::map< int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > Calendario::retornaDiaHorariosEmComum( GGroup<Calendario*,LessPtr<Calendario>> calendarios )
{
	std::map< int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > horariosDiaComuns;

	// Copia horariosDia do primeiro calendario da lista
	ITERA_GGROUP_LESSPTR( itHorAula, this->horarios_aula, HorarioAula )
	{
		ITERA_GGROUP_N_PT( itDia, itHorAula->dias_semana, int )
		{
			horariosDiaComuns[*itDia].add(*itHorAula);
		}
	}

	// Remove os horariosDia que não são comuns
	ITERA_GGROUP_LESSPTR( itCalend, calendarios, Calendario )
	{		
		std::map< int, GGroup<HorarioAula*, LessPtr<HorarioAula>> > remover;
		std::map< int, GGroup<HorarioAula*, LessPtr<HorarioAula>> >::iterator itMap = horariosDiaComuns.begin();
		for( ; itMap != horariosDiaComuns.end(); itMap++ )
		{			
			int dia = (*itMap).first;
			ITERA_GGROUP_LESSPTR( itHa, (*itMap).second, HorarioAula )
			{
				HorarioAula *ha = *itHa;
				if ( ! (*itCalend)->possuiHorarioDia( ha, dia ) )
				{
					remover[dia].add( ha );
				}
			}
		}
				
		for( itMap = remover.begin(); itMap != remover.end(); itMap++ )
		{			
			int dia = (*itMap).first;
			ITERA_GGROUP_LESSPTR( itHa, (*itMap).second, HorarioAula )
			{
				HorarioAula *ha = *itHa;
				horariosDiaComuns[dia].remove( ha );				
			}
		}

		if ( horariosDiaComuns.size()==0 ) break;
	}

	return horariosDiaComuns;
}


bool Calendario::possuiTurno( int t )
{
	if ( this->mapTurnos.find(t) != this->mapTurnos.end() )
		return true;
	else
		return false;
}

bool Calendario::possuiTurno( int t , int dia )
{
	std::map<int, GGroup<int>>::iterator itMap = this->mapTurnos.find(t);
	if ( itMap != this->mapTurnos.end() )
	{
		GGroup<int> dias = itMap->second;
		if ( dias.find(dia) != dias.end() )
			return true;
	}
	
	return false;
}

GGroup<int> Calendario::dias( int t ) 
{
	GGroup<int> d;

	std::map<int, GGroup<int>>::iterator it = mapTurnos.find(t);
	if ( it != mapTurnos.end() ) 
		d = it->second;

	return d;
}

int Calendario::getTurnoIES( DateTime dt )
{
	ITERA_GGROUP_LESSPTR( itHorAula, horarios_aula, HorarioAula )
	{
		if ( itHorAula->getInicio() == dt )
		{
			return itHorAula->getTurnoIESId();
		}
	}

	std::cout<<"\nErro, turno nao encontrado!! DateTime " << dt << " Calendario " << this->getId(); fflush(NULL);
	return -1;
}

void Calendario::calculaDiaFaseSomaInterv()
{
	for ( auto itDia = this->mapDiaDateTime.begin(); itDia != this->mapDiaDateTime.end(); itDia++ )
	{
		int dia = itDia->first;

		int faseAnt = -1, fase = 0;
		int soma = 0;
		DateTime dtAnt;
		int maxInterv = 0;

		for ( auto itDt = itDia->second.begin(); itDt != itDia->second.end(); itDt++ )
		{
			DateTime dt = itDt->first;
			
			fase = CentroDados::getFaseDoDia(dt);

			if ( faseAnt == fase )
			{
				int interv = (dt - dtAnt).getDateMinutes() - (int) getTempoAula();
				soma += interv;
				maxInterv = max( maxInterv, interv );
			}
			
			bool fechaFase=false;
			if ( faseAnt != fase && faseAnt>0 )									// finaliza nova fase
				fechaFase=true;
			if ( auto itRev = (++itDia->second.rend())->first == itDt->first )	// finaliza ultima fase
				fechaFase=true;

			if (fechaFase)
			{
				mapDiaFaseSomaInterv[dia][faseAnt] = soma;
				soma = 0;
				mapDiaFaseMaxInterv[dia][fase] = maxInterv;
				maxInterv = 0;
			}

			dtAnt = dt;
			faseAnt = fase;
		}
	}
	
}

void Calendario::calculaDiaSomaInterv()
{
	for ( auto itDia = this->mapDiaDateTime.begin(); itDia != this->mapDiaDateTime.end(); itDia++ )
	{
		auto *mapDtHor = &itDia->second;
				
		int dia = itDia->first;
		int soma = 0;		
		DateTime dtAnt;

		for ( auto itDt = mapDtHor->begin(); itDt != mapDtHor->end(); itDt++ )
		{
			DateTime dt = itDt->first;
			
			if ( itDt != mapDtHor->begin() )
			{
				int interv = (dt - dtAnt).getDateMinutes() - (int) getTempoAula();
				soma += interv;
			}

			dtAnt = dt;
		}

		mapDiaSomaInterv[dia] = soma;
	}
	
}

int Calendario::getSomaInterv( int dia, int fase )
{
	int soma = 0;
	auto itDia = this->mapDiaFaseSomaInterv.find(dia);
	if ( itDia != this->mapDiaFaseSomaInterv.end() )
	{
		auto itFase = itDia->second.find(fase);
		if ( itFase != itDia->second.end() )
		{
			soma = itFase->second;
		}
	}
	return soma;
}

int Calendario::getSomaInterv( int dia )
{
	int soma = 0;
	auto itDia = this->mapDiaSomaInterv.find(dia);
	if ( itDia != this->mapDiaSomaInterv.end() )
	{
		soma = itDia->second;
	}
	return soma;
}

int Calendario::getMaxInterv( int dia, int fase )
{
	int interv = 0;
	auto itDia = this->mapDiaFaseMaxInterv.find(dia);
	if ( itDia != this->mapDiaFaseMaxInterv.end() )
	{
		auto itFase = itDia->second.find(fase);
		if ( itFase != itDia->second.end() )
		{
			interv = itFase->second;
		}
	}
	return interv;
}