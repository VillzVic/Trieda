#include "Disciplina.h"
#include "CentroDados.h"
#include "Professor.h"


Disciplina::Disciplina(void)
{
   demanda_total = 0;
   max_alunos_t = -1;
   max_alunos_p = -1;

   divisao_creditos.clear();
   tipo_disciplina = NULL;
   nivel_dificuldade = NULL;
   calendarios.clear();
   substituta = NULL;

   max_demanda = -1;
   num_turmas = -1;
   min_creds = -1;
   max_creds = -1;
   nSalasAptas = 0;
   prof_unico_discPT = true;
   aulas_continuas_teor_prat = false;
   tem_par_prat_teor = false;
}

Disciplina::~Disciplina(void)
{
	// ToDo
	//this->clearHors();
	//this->clearDivCreds();
}

void Disciplina::clearHors()
{
	horarios.deleteElements();
	horarios.clear();
}

void Disciplina::clearDivCreds()
{
	divisao_creditos.deleteElements();
	divisao_creditos.clear();
}

void Disciplina::le_arvore( ItemDisciplina & elem )
{
   this->setId( elem.id() );
   codigo = elem.codigo();
   nome = elem.nome();
   cred_teoricos = elem.credTeoricos();
   cred_praticos = elem.credPraticos();
   max_creds = cred_teoricos + cred_praticos;
   e_lab = elem.laboratorio();

   if ( elem.maxAlunosTeorico().present() )
   {
      max_alunos_t = elem.maxAlunosTeorico().get();
   }

   if ( elem.maxAlunosPratico().present() )
   {
      max_alunos_p = elem.maxAlunosPratico().get();
   }

   if ( cred_praticos > 0 && max_alunos_p <= 0 )
   {
	   std::cout<<"\nDisciplina "<< this->getId() << 
		   " com creditos praticos e maximo de alunos igual a"<< max_alunos_p;
	   exit(EXIT_FAILURE);
   }
   if ( cred_teoricos > 0 && max_alunos_t <= 0 )
   {
	   std::cout<<"\nDisciplina "<< this->getId() << 
		   " com creditos praticos e maximo de alunos igual a"<< max_alunos_p;
	   exit(EXIT_FAILURE);
   }
   if ( cred_teoricos > 0 && cred_praticos > 0 && !e_lab )
   if ( max_alunos_p != max_alunos_t )
   {
	   std::cout<<"\nDisciplina "<< this->getId() << 
		   " com creditos praticos e teoricos, sem uso de laboratorio, mas com maximo de alunos diferentes. "
		   << "Usarei o valor teorico, caso esse seja positivo.";
	 //  exit(EXIT_FAILURE);
	   //int max = ( max_alunos_p > max_alunos_t ? max_alunos_p : max_alunos_t );
	   int max = ( max_alunos_t>0 ? max_alunos_t : max_alunos_p );
	   max_alunos_p = max;
	   max_alunos_t = max;
   }

   tipo_disciplina_id = elem.tipoDisciplinaId();
   nivel_dificuldade_id = elem.nivelDificuldadeId();
   prof_unico_discPT = elem.profUnicoCredsTeorPrat();
   aulas_continuas_teor_prat = elem.aulasContinuasTeorPrat();

   clearDivCreds();
   if ( elem.divisaoDeCreditos().present() )
   {
	   DivisaoCreditos * divisao = new DivisaoCreditos();
	   divisao->le_arvore( elem.divisaoDeCreditos().get() );

	   divisao_creditos.add( divisao );
   }

   ITERA_NSEQ( it_contem, elem.disciplinasEquivalentes(), id, Identificador )
   {
      ids_disciplinas_equivalentes.add( *it_contem );
   }

   ITERA_NSEQ( it_inc, elem.disciplinasIncompativeis(), id, Identificador )
   {
      ids_disciplinas_incompativeis.add( *it_inc );
   }

   ITERA_SEQ( it_hora, elem.horariosDisponiveis(), Horario )
   {
      Horario * horario = new Horario();
      horario->le_arvore( *it_hora );
      horarios.add( horario );

	  this->addTurnoIES( horario->getTurnoIESId(), horario );
   }
}

void Disciplina::addHorarioDia( Calendario* sl, HorarioDia* hd )
{ 
	this->calendarios[sl].add( hd );
	this->mapDiaDtiDtf[hd->getDia()][hd->getHorarioAula()->getInicio()].add( hd->getHorarioAula()->getFinal() );
}

int Disciplina::getMenorCapacSala( int campusId ) 
{
	if ( this->menorCapacSala.find( campusId ) != this->menorCapacSala.end()  )
		return this->menorCapacSala[campusId];
	else
		return -1;
}


int Disciplina::getCapacMediaSala( int campusId )
{
	if ( this->capacMediaSala.find( campusId ) != this->capacMediaSala.end()  )
		return this->capacMediaSala[campusId];
	else
		return 0;
}

int Disciplina::getTempoTotalSemana( Calendario* calendario )
{
	if ( this->calendarios.find( calendario ) == this->calendarios.end() )
	{
		std::cout<<"\nErro, calendario nao existe na disciplina.";
		return 0;
	}

    int tempoDisciplina = 0;
    ITERA_GGROUP_N_PT( itD, diasLetivos, int )
    {
       tempoDisciplina += calendario->getTempoTotal(*itD);
    }
    return tempoDisciplina;
}

int Disciplina::getMaxTempoDiscEquivSubstituta()
{
   int tempoMax = 0;
   
   ITERA_GGROUP_LESSPTR( it, discEquivSubstitutas, Disciplina )
   {
	   int t = (*it)->getTotalCreditos() * (*it)->getTempoCredSemanaLetiva();
	   if ( t > tempoMax )
		   tempoMax = t;
   }

   return tempoMax;
}

bool Disciplina::inicioTerminoValidos( HorarioAula *hi, HorarioAula *hf, int dia, GGroup<HorarioAula*, LessPtr<HorarioAula>> horariosPossiveis )
{
	if ( hi->getCalendario()->getId() != hf->getCalendario()->getId() )
		return false;

	if ( hi->getTurnoIESId() != hf->getTurnoIESId() )
	{
	//	std::cout << "\nDisciplina::inicioTerminoValidos(): Caso impedido com turnos diferentes -> hi = " << hi->getId() 
	//		<< ", hf = " << hf->getId() << " , disc " << this->getId();
		return false;
	}

	if ( hi->getFaseDoDia() != hf->getFaseDoDia() )
	{
		return false;
	}

	Calendario *calendario = hi->getCalendario();

	if ( this->calendarios.find( calendario ) == this->calendarios.end() )
		 return false;	

	std::pair<HorarioAula*, HorarioAula*> parHorarios( hi, hf );

	// Procura, caso já tenha sido calculado
	std::map< int /*dia*/, std::map< std::pair<HorarioAula* /*hi*/, HorarioAula* /*hf*/>, GGroup<HorarioAula*, LessPtr<HorarioAula>> > >::iterator
		itMap = this->horarios_hihf_validos.find( dia );
	if ( itMap != this->horarios_hihf_validos.end() )
	{
		if ( itMap->second.find( parHorarios ) != itMap->second.end() )
		{
			// Achou o trio <dia, hi, hf>

			GGroup<HorarioAula*, LessPtr<HorarioAula>> horariosAulaIntermediarios = itMap->second.find( parHorarios )->second;

			if ( horariosAulaIntermediarios.size() == 0 ) return false;

			ITERA_GGROUP_LESSPTR( itHorAula, horariosAulaIntermediarios, HorarioAula )
			{
				if ( horariosPossiveis.find( *itHorAula ) == horariosPossiveis.end() )
					return false;
			}
			return true;
		}	
	}

	// Não foi calculado ainda. Calcula e insere no map horarios_hihf_validos.

	if ( hi->dias_semana.find(dia) == hi->dias_semana.end() )
		return false;
	if ( hf->dias_semana.find(dia) == hf->dias_semana.end() )
		return false;

	if ( *hf < *hi )
	{
		GGroup<HorarioAula*, LessPtr<HorarioAula>> horariosAulaIntermediarios;
		this->horarios_hihf_validos[dia][ parHorarios ] = horariosAulaIntermediarios; // insere vazio: trio não é válido
		return false;
	}

	if ( *hf == *hi )
	{
		// Não permite que uma disciplina com nro par de creditos
		// tenha uma aula com somente 1 credito, a não ser que 
		// esteja determinado por regra de divisão de créditos
		if ( this->getTotalCreditos() % 2 == 0 && 
			 calendario->restringeAulaComIntervalo() )
		{
			bool CRIAR = false;
			std::vector< std::vector< std::pair< int /*dia*/, int /*nCreds*/ > > >::iterator 
				it1 = this->combinacao_divisao_creditos.begin();
			for ( ; it1 != this->combinacao_divisao_creditos.end(); it1++ )
			{
				std::vector< std::pair< int /*dia*/, int /*nCreds*/ > >::iterator
					it2 = (*it1).begin();
				for ( ; it2 != (*it1).end(); it2++ )
				{	
					if ( (*it2).second == 1 )
					{
						CRIAR = true; break;
					}
				}
				if ( CRIAR ) break;										
			}
			if ( !CRIAR )
			{
				GGroup<HorarioAula*, LessPtr<HorarioAula>> horariosAulaIntermediarios;
				this->horarios_hihf_validos[dia][ parHorarios ] = horariosAulaIntermediarios; // insere vazio: trio não é válido
				return false; 
			}
		}
	}

	int delta = calendario->retornaNroCreditosEntreHorarios( hi, hf );
	if ( delta > this->getTotalCreditos() )
	{
		GGroup<HorarioAula*, LessPtr<HorarioAula>> horariosAulaIntermediarios;
		this->horarios_hihf_validos[dia][ parHorarios ] = horariosAulaIntermediarios; // insere vazio: trio não é válido		
		return false;
	}

	if ( delta == 2 &&
		 calendario->intervaloEntreHorarios(hi,hf) && 
		 calendario->restringeAulaComIntervalo() )
	{
		GGroup<HorarioAula*, LessPtr<HorarioAula>> horariosAulaIntermediarios;
		this->horarios_hihf_validos[dia][ parHorarios ] = horariosAulaIntermediarios; // insere vazio: trio não é válido
		return false;
	}

	if ( this->aulasContinuas() &&
		 this->getTotalCreditos() > delta )
	{
		GGroup<HorarioAula*, LessPtr<HorarioAula>> horariosAulaIntermediarios;
		this->horarios_hihf_validos[dia][ parHorarios ] = horariosAulaIntermediarios; // insere vazio: trio não é válido
		return false;
	}

	if ( this->combinacao_divisao_creditos.size() > 0 )
	{
		bool CRIAR = false;
		int nCredHor = calendario->retornaNroCreditosEntreHorarios( hi, hf );
		std::vector< std::vector< std::pair< int /*dia*/, int /*nCreds*/ > > >::iterator 
			it1 = this->combinacao_divisao_creditos.begin();
		for ( ; it1 != this->combinacao_divisao_creditos.end(); it1++ )
		{
			std::vector< std::pair< int /*dia*/, int /*nCreds*/ > >::iterator
				it2 = (*it1).begin();
			for ( ; it2 != (*it1).end(); it2++ )
			{	
				if ( (*it2).second == nCredHor )
				{
					CRIAR = true; break;
				}
			}
			if ( CRIAR ) break;										
		}
		if ( !CRIAR )
		{
			GGroup<HorarioAula*, LessPtr<HorarioAula>> horariosAulaIntermediarios;
			this->horarios_hihf_validos[dia][ parHorarios ] = horariosAulaIntermediarios; // insere vazio: trio não é válido
			return false; 
		}
	}
	else
	{
		if ( delta > 6 )
		{
			std::cout<<"\nAtencao: aulas com mais de 6 horarios seguidos sem regra de divisao de credito"
				<< " estao sendo impedidas por hardcoded! Disc " << this->getId();
			GGroup<HorarioAula*, LessPtr<HorarioAula>> horariosAulaIntermediarios;
			this->horarios_hihf_validos[dia][ parHorarios ] = horariosAulaIntermediarios; // insere vazio: trio não é válido
			return false; 			
		}
	}

	// Verifica se há todos os horários intermediários necessários disponíveis no dia
	GGroup< HorarioAula*, LessPtr<HorarioAula> > horariosDiscNoDia = this->getHorariosDia( calendario, dia ); 
	GGroup<HorarioAula*, LessPtr<HorarioAula>> horariosAulaIntermediarios;
	bool Ok=true;
	HorarioAula *h = hi;
	for ( int i = 1; i <= delta; i++ )
	{
		if( h==NULL ) Ok=false;
				
		if ( horariosDiscNoDia.find( h ) != horariosDiscNoDia.end() ) 
		{
			horariosAulaIntermediarios.add(h);
			h = calendario->getProximoHorario( h );
		}
		else Ok=false;
	}
	if (Ok)
	{
		this->horarios_hihf_validos[dia][ parHorarios ] = horariosAulaIntermediarios; // insere preenchido: trio é válido em geral

		// Verifica se o trio é válido para o conjunto de horariosPossiveis
		ITERA_GGROUP_LESSPTR( itHorAula, horariosAulaIntermediarios, HorarioAula )
		{
			if ( horariosPossiveis.find( *itHorAula ) == horariosPossiveis.end() )
				return false;
		}
		return true;
	}
	else
	{
		horariosAulaIntermediarios.clear();
		this->horarios_hihf_validos[dia][ parHorarios ] = horariosAulaIntermediarios; // insere vazio: trio não é válido
		return false;
	}	
}

bool Disciplina::inicioTerminoValidos( HorarioAula *hi, HorarioAula *hf, int dia )
{

	bool debug=false;
	if ( this->getId() == 16669 && hi->getInicio().getHour() == 15 && hi->getInicio().getMinute() == 20
		&& hf->getInicio().getHour() == 18 && hf->getInicio().getMinute() == 20 )
	{
		//debug=true;
	}

	if (debug) std::cout<<"\nDisciplina::inicioTerminoValidos: disc->getId() == 13747, dia " 
		<< dia << " hi " << hi->getId() << " hf " << hf->getId();

	if ( hi->getCalendario()->getId() != hf->getCalendario()->getId() )
		return false;

	if (debug) std::cout<<"\n1";

	if ( hi->getTurnoIESId() != hf->getTurnoIESId() )
	{
		if (debug)
			std::cout << "\nDisciplina::inicioTerminoValidos(): Caso impedido com turnos diferentes -> hi = " << hi->getId() 
				<< ", hf = " << hf->getId() << " , disc " << this->getId();

		return false;
	}

	if (debug) std::cout<<" 2";
	if ( hi->getFaseDoDia() != hf->getFaseDoDia() )
	{
		return false;
	}
	if (debug) std::cout<<" 3";
	Calendario *calendario = hi->getCalendario();
	
	std::pair<HorarioAula*, HorarioAula*> parHorarios( hi, hf );

	// Procura, caso já tenha sido calculado
	std::map< int /*dia*/, std::map< std::pair<HorarioAula* /*hi*/, HorarioAula* /*hf*/>, bool > >::iterator
		itMap = this->horarios_hihf.find( dia );
	if ( itMap != this->horarios_hihf.end() )
	{
		if ( itMap->second.find( parHorarios ) != itMap->second.end() )
		{
			// Achou o trio <dia, hi, hf>

			bool valido = itMap->second.find( parHorarios )->second;

			if ( !valido ) return false;

			return true;
		}	
	}
	if (debug) std::cout<<" 4";

	// Não foi calculado ainda. Calcula e insere no map horarios_hihf_validos.

	//if ( hi->dias_semana.find(dia) == hi->dias_semana.end() )
	//	return false;
	//if ( hf->dias_semana.find(dia) == hf->dias_semana.end() )
	//	return false;

	if ( *hf < *hi )
	{
		this->horarios_hihf[dia][ parHorarios ] = false; // insere vazio: trio não é válido
		return false;
	}
	if (debug) std::cout<<" 5";

	if ( hf->getInicio() == hi->getInicio() )
	{
		// Não permite que uma disciplina com nro par de creditos
		// tenha uma aula com somente 1 credito, a não ser que 
		// esteja determinado por regra de divisão de créditos
		if ( this->getTotalCreditos() % 2 == 0 && 
			 calendario->restringeAulaComIntervalo() )
		{
			bool CRIAR = false;
			std::vector< std::vector< std::pair< int /*dia*/, int /*nCreds*/ > > >::iterator 
				it1 = this->combinacao_divisao_creditos.begin();
			for ( ; it1 != this->combinacao_divisao_creditos.end(); it1++ )
			{
				std::vector< std::pair< int /*dia*/, int /*nCreds*/ > >::iterator
					it2 = (*it1).begin();
				for ( ; it2 != (*it1).end(); it2++ )
				{	
					if ( (*it2).second == 1 )
					{
						CRIAR = true; break;
					}
				}
				if ( CRIAR ) break;										
			}
			if ( !CRIAR )
			{
				this->horarios_hihf[dia][ parHorarios ] = false; // insere vazio: trio não é válido
				return false; 
			}
		}
	}
	if (debug) std::cout<<" 6";
	int delta = calendario->retornaNroCreditosEntreHorarios( hi, hf );
	if ( delta > this->getTotalCreditos() )
	{
		this->horarios_hihf[dia][ parHorarios ] = false; // insere vazio: trio não é válido		
		return false;
	}
	if (debug) std::cout<<" 7 \tdelta = " << delta;
	if ( delta == 2 &&
		 calendario->intervaloEntreHorarios(hi,hf) && 
		 calendario->restringeAulaComIntervalo() )
	{
		this->horarios_hihf[dia][ parHorarios ] = false; // insere vazio: trio não é válido
		return false;
	}
	if (debug) std::cout<<" 8";
	if ( this->aulasContinuas() &&
		 this->getTotalCreditos() > delta )
	{
		this->horarios_hihf[dia][ parHorarios ] = false; // insere vazio: trio não é válido
		return false;
	}
	if (debug) std::cout<<" 9";
	if ( this->combinacao_divisao_creditos.size() > 0 )
	{
		if (debug) std::cout<<"  9.1";
		bool CRIAR = false;
		std::vector< std::vector< std::pair< int /*dia*/, int /*nCreds*/ > > >::iterator 
			it1 = this->combinacao_divisao_creditos.begin();
		for ( ; it1 != this->combinacao_divisao_creditos.end(); it1++ )
		{
			std::vector< std::pair< int /*dia*/, int /*nCreds*/ > >::iterator
				it2 = (*it1).begin();
			for ( ; it2 != (*it1).end(); it2++ )
			{	
				if ( (*it2).second == delta )
				{
					CRIAR = true; break;
				}
			}
			if ( CRIAR ) break;										
		}
		if ( !CRIAR )
		{
			this->horarios_hihf[dia][ parHorarios ] = false; // insere vazio: trio não é válido
			return false; 
		}
	}
	else
	{
		if (debug) std::cout<<"  9.2";
		if ( delta > 6 )
		{
			std::cout<<"\nAtencao: aulas com mais de 6 horarios seguidos sem regra de divisao de credito"
				<< " estao sendo impedidas por hardcoded! Disc " << this->getId();
			this->horarios_hihf[dia][ parHorarios ] = false; // insere vazio: trio não é válido
			return false; 			
		}
	}
	if (debug) std::cout<<" 10";
	// Verifica se há todos os horários intermediários necessários disponíveis no dia
	
	if ( !this->possuiHorariosNoDia( hi, hf, dia ) )
	{
		this->horarios_hihf[dia][ parHorarios ] = false; // insere vazio: trio não é válido
		return false; 			
	}
	if (debug) std::cout<<" 11 - Valido! Dia" << dia << " hi " << hi->getId() << " hf " << hf->getId();
	this->horarios_hihf[dia][ parHorarios ] = true; // insere preenchido: trio é válido em geral
	return true;
}

bool Disciplina::possuiHorariosNoDia( HorarioAula *hi, HorarioAula *hf, int dia )
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

GGroup< HorarioAula*, LessPtr<HorarioAula> > Disciplina::getHorariosDia( Calendario* sl, int dia )
{
	GGroup< HorarioAula*, LessPtr<HorarioAula> > horariosNoDia;
	GGroup< HorarioDia*, LessPtr<HorarioDia> > horarios = this->getHorariosDia( sl );

	ITERA_GGROUP_LESSPTR( it, horarios, HorarioDia )
	{
		if ( it->getDia() == dia ) horariosNoDia.add( it->getHorarioAula() );
	}
	return horariosNoDia;
}

GGroup< Horario*, LessPtr<Horario> > Disciplina::getHorariosOuCorrespondentes( TurnoIES* turnoIES )
{
	// Retorna os horarios que a disciplina possui disponivel no turnoIES

	GGroup< Horario*, LessPtr<Horario> > horariosNoTurnoIES;

	ITERA_GGROUP_LESSPTR( itHor, this->horarios, Horario )
	{
		ITERA_GGROUP_N_PT( itDia, itHor->dias_semana, int )
		{
			if ( turnoIES->possuiHorarioDiaOuCorrespondente( itHor->horario_aula, *itDia ) )
			{
				horariosNoTurnoIES.add( *itHor );
				break;
			}
		}
	}
	
	return horariosNoTurnoIES;
}

bool Disciplina::possuiHorariosOuCorrespondentesNoTurno( TurnoIES* turnoIES )
{
	GGroup< Horario*, LessPtr<Horario> > horariosNoTurnoIES;

	ITERA_GGROUP_LESSPTR( itHor, this->horarios, Horario )
	{
		ITERA_GGROUP_N_PT( itDia, itHor->dias_semana, int )
		{
			if ( turnoIES->possuiHorarioDiaOuCorrespondente( itHor->horario_aula, *itDia ) )
			{
				return true;
			}
		}
	}
	
	return false;
}

int Disciplina::getNrHorsNoTurno( TurnoIES* turnoIES ) const
{
	int nr = 0;
	for (auto itDia = mapDiaDtiDtf.cbegin(); itDia != mapDiaDtiDtf.cend(); itDia++)
	{
		for ( auto itDti=itDia->second.cbegin(); itDti!=itDia->second.cend(); itDti++ )
		{
			DateTime dti = itDti->first;
			for ( auto itDtf=itDti->second.begin(); itDtf!=itDti->second.end(); itDtf++ )
			{
				DateTime dtf = (*itDtf);
				if (turnoIES->possuiHorarioDia(itDia->first, dti, dtf))
				{
					nr++; break;
				}
			}
		}		
	}
	return nr;
}

int Disciplina::getNumTurmasNaSala( int cjtSalaId )
{
	int n = 0;
	if ( mapSubCjtSalaNumTurmas.find(cjtSalaId) != mapSubCjtSalaNumTurmas.end() )
		n = mapSubCjtSalaNumTurmas[cjtSalaId];
	
	return n;
}

bool Disciplina::possuiHorarioDia( int dia, HorarioAula *hd )
{
	ITERA_GGROUP_LESSPTR( it, horarios, Horario )
	{
		if ( it->getHorarioAulaId() != hd->getId() )
			continue;

		if ( it->dias_semana.find(dia) != it->dias_semana.end() ) 
			return true;
	}
	return false;
}

bool Disciplina::possuiHorarioDiaOuAnalogo( int dia, HorarioAula *hd )
{
	ITERA_GGROUP_LESSPTR( it, horarios, Horario )
	{
		if ( ! it->horario_aula->inicioFimIguais( *hd ) )
			continue;

		if ( it->dias_semana.find(dia) != it->dias_semana.end() ) 
			return true;
	}
	return false;
}

int Disciplina::possuiHorarioNoTurnoDia( int dia, int turno )
{
	int nMax = 0;
	std::map< int, std::map<int, GGroup<Horario*, LessPtr<Horario>>> >::iterator
		it1 = turnos.find(turno); // <turno, dias> que possui horários
	if ( it1 != turnos.end() )
	{
		std::map<int, GGroup<Horario*, LessPtr<Horario>>> map2 = it1->second;
		std::map<int, GGroup<Horario*, LessPtr<Horario>>>::iterator it2 = map2.find(dia);
		if ( it2 != map2.end() ) 
		{
			std::map<int, int> calendId_nHors;
			GGroup<Horario*, LessPtr<Horario>> hors = it2->second;
			ITERA_GGROUP_LESSPTR( itHor, hors, Horario )
			{
				int calend = itHor->horario_aula->getCalendario()->getId();
				if ( calendId_nHors.find(calend) == calendId_nHors.end() )
				{
					calendId_nHors[calend] = 1;
				}
				else calendId_nHors[calend]++;

				if ( nMax < calendId_nHors[calend] ) nMax = calendId_nHors[calend];
			}
		}		
	}

	return nMax;
}

GGroup<int> Disciplina::diasQuePossuiTurno( int t )
{
	GGroup<int> dias;
	ITERA_GGROUP_N_PT( itDia, this->diasLetivos, int )
	{
		if ( possuiHorarioNoTurnoDia( *itDia, t ) > 0 )
		{
			dias.add( *itDia );
		}
	}
	return dias;
}

bool Disciplina::possuiTurno( int t )
{
	if ( this->turnos.find(t) != this->turnos.end() )
		return true;
	else
		return false;
}

void Disciplina::addTurno( int t,  Horario *h )
{	
	ITERA_GGROUP_N_PT( itDia, h->dias_semana, int )
	{
		turnos[t][*itDia].add(h); 		   
	}
}

bool Disciplina::possuiTurnoIES( TurnoIES* turnoIES )
{
	if ( this->turnosIES.find( turnoIES->getId() ) != this->turnosIES.end() )
		return true;
	else
	{
		// correspondente
		if ( possuiHorariosOuCorrespondentesNoTurno( turnoIES ) )
			return true;
	}
	return false;
}
 
void Disciplina::addTurnoIES( int t,  Horario *h )
{	
	ITERA_GGROUP_N_PT( itDia, h->dias_semana, int )
	{
		turnosIES[t][*itDia].add(h); 		   
	}
}
   
int Disciplina::getTotalDeInicioTerminoValidos()
{
	int total = 0;

	std::map< int /*dia*/, std::map< std::pair<HorarioAula* /*hi*/, HorarioAula* /*hf*/>, 
		GGroup<HorarioAula*, LessPtr<HorarioAula>> > >::iterator
		itMap = this->horarios_hihf_validos.begin();
	for ( ; itMap != this->horarios_hihf_validos.end() ; itMap++ )
	{
		total += itMap->second.size();
	}

	return total;
}

void Disciplina::setTurmasPratPorTeor( int nroTurmasPrat )
{
	if ( this->getId() < 0 )
	{
		std::cout << "\nErro: o metodo Disciplina::setTurmasPratPorTeor( int nroTurmasPrat ) "
			<< " eh exclusivo para disciplina teorica.";
		return;
	}

	int turmaP = 1;
	for ( int turmaT = 1; turmaT <= this->getNumTurmas(); turmaT++ )
	{
		for ( int turma = turmaP; turma < turmaP+nroTurmasPrat; turma++ )
		{
			mapTurmaPT_TurmasTP[ turmaT ].add(turma);			
		}
		turmaP += nroTurmasPrat;
	}
}

void Disciplina::setTurmaPratAssociada( int turmaT, int turmaP )
{
	if ( this->getId() < 0 )
	{
		std::cout << "\nErro: o metodo Disciplina::setTurmaPratAssociada( int turmaT, int turmaP ) "
			<< " eh exclusivo para disciplina teorica.";
		return;
	}
	
	mapTurmaPT_TurmasTP[ turmaT ].add(turmaP);
}

void Disciplina::setTurmaTeorAssociada( int turmaP, int turmaT )
{
	if ( this->getId() > 0 )
	{
		std::stringstream msg;
		msg << "Metodo exclusivo para disciplina pratica. Disciplina " << this->getId();
		CentroDados::printError( "GGroup<int> Disciplina::setTurmaTeorAssociada( int turmaP, int turmaT )", msg.str() );
		return;
	}

	mapTurmaPT_TurmasTP[ turmaP ].add(turmaT);			
}

GGroup<int> Disciplina::getTurmasAssociadas( int turma )
{ 
	std::map< int, GGroup<int> >::iterator 
		it = mapTurmaPT_TurmasTP.find( turma );
	if ( it != mapTurmaPT_TurmasTP.end() )
		return it->second;
	
	std::stringstream msg;
	msg << "Turma " << turma << " da disciplina " << this->getId() << " nao existe ou nao possui turmas PT associadas.";
	CentroDados::printWarning( "GGroup<int> Disciplina::getTurmasAssociadas( int turma )", msg.str() );
	
	GGroup<int> ggroup;
	return ggroup;
}

int Disciplina::getNroTurmasAssociadas( int turma )
{ 
	int n = 0;

	std::map< int, GGroup<int> >::iterator 
		it = mapTurmaPT_TurmasTP.find( turma );
	if ( it != mapTurmaPT_TurmasTP.end() )
		n = it->second.size();

	return n;
}

int Disciplina::getNroCredsRegraDiv( int k, int dia )
{
	int num = 0;
	
	if ( this->combinacao_divisao_creditos.size() > k )
	{
		std::vector< std::pair< int /*dia*/, int /*numCreditos*/ > >::iterator
			it = this->combinacao_divisao_creditos[ k ].begin();
		for ( ; it != this->combinacao_divisao_creditos[ k ].end(); it++ )
		{
			if ( it->first == dia )
			{
				num = it->second;
				break;
			}
		}
	}

	return num;
}

bool Disciplina::possuiRegraCred() const
{
	if (combinacao_divisao_creditos.size() > 0 ||
		divisao_creditos.size() > 0)
		return true;
	return false;
}

void Disciplina::addProfHabilit(Professor* p)
{ 
	profsHabilit.insert(p); 
}

bool Disciplina::existeProfRealNoHorarioDia(int dia, HorarioAula* const ha) const
{
	for (auto pit = profsHabilit.cbegin(); pit != profsHabilit.cend(); pit++)
	{
		if ((*pit)->possuiHorariosNoDia(ha, ha, dia))
			return true;
	}
	return false;
}

bool Disciplina::getProfRealNoHorarioDia(int dia, HorarioAula* const ha, std::unordered_set<Professor*> &profs) const
{
	profs.clear();
	for (auto pit = profsHabilit.cbegin(); pit != profsHabilit.cend(); pit++)
	{
		if ((*pit)->possuiHorariosNoDia(ha, ha, dia))
			profs.insert(*pit);
	}
	return (profs.size() > 0);
}

int Disciplina::getNroProfRealHabilit() const
{
	return profsHabilit.size();
}

int Disciplina::getNroProfRealImportHabilit(int importancia, bool ouMenor) const
{
	if (importancia == CentroDados::allPriorProfLevels_)
		return this->profsHabilit.size();

	int nr=0;
	for (auto itProf = this->profsHabilit.cbegin(); itProf != this->profsHabilit.cend(); itProf++)
	{
		if ((*itProf)->getImportancia() == importancia ||
			((*itProf)->getImportancia() < importancia && ouMenor))
			nr++;
	}
	return nr;
}

void Disciplina::getProfsIntersec(std::map<int, std::map<DateTime, std::unordered_set<Professor*> >> &profsIntersec)
{
	for (auto itHor = this->horarios.begin(); itHor != this->horarios.end(); itHor++)
	{
		HorarioAula* const ha = itHor->horario_aula;
				
		for (auto itDia = ha->dias_semana.begin(); itDia != ha->dias_semana.end(); itDia++)
		{
			int dia = *itDia;

			std::unordered_set<Professor*> profsNoDiaHa;
			this->getProfRealNoHorarioDia(dia, ha, profsNoDiaHa);

			for (auto itProf = profsNoDiaHa.begin(); itProf != profsNoDiaHa.end(); itProf++)
			{
				profsIntersec[dia][ha->getInicio()].insert(*itProf);
			}
		}
	}
}

double Disciplina::getTempoCredSemanaLetiva()
{ 
	if (this->calendarios.size() > 0)
	{
		return this->calendarios.begin()->first->getTempoAula(); 
	}
	stringstream ss;
	ss << "Disciplina " << this->getId() << " sem calendarios associados.";
	CentroDados::printWarning("double Disciplina::getTempoCredSemanaLetiva()",ss.str());
	return 0;
}