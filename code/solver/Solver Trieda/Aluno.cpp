#include "Aluno.h"
#include "CentroDados.h"
#include "ProblemData.h"

std::ostream & operator << (
   std::ostream & out, Aluno & aluno )
{
   out << "<Aluno>" << std::endl;
      
   out << "<alunoId>" << aluno.getAlunoId()
       << "</alunoId>" << std::endl;

   out << "<nomeAluno>" << aluno.getNomeAluno()
	    << "</nomeAluno>" << std::endl;
   
   //out << "<ofertaId>" << aluno.getOfertaId()
   //    << "</ofertaId>" << std::endl;

   out << "<Demandas>" << std::endl;
   ITERA_GGROUP_LESSPTR( itAlDemandas, aluno.demandas, AlunoDemanda )
   {
	   out << "<demandaId>" << itAlDemandas->demanda->getId()
			<< "</demandaId>" << std::endl;
   }  
   out << "</Demandas>" << std::endl;
      
   out << "</AlunoDemanda>" << std::endl;

   return out;
}
 
Aluno::Aluno( void )
	: nrCredsReqP1(0), nrCredsMedioDia_(-1)
{
   this->setAlunoId( -1 );
   this->oferta = NULL;
   this->ofertaId = -1;
   this->nCredsAlocados.clear();
   this->formando = false;
   this->calouro = false;
   this->cargaHorariaOrigRequeridaP1 = 0;
   this->nroCredsOrigRequeridosP1 = 0;
   this->nCredsJaAlocados = 0;
   this->nroDiscsOrigRequeridosP1 = 0;
   this->prioridadeDoAluno = 0;
}

Aluno::Aluno( int id, std::string nome, bool formando, Oferta* oft )
	: nrCredsReqP1(0), nrCredsMedioDia_(-1)
{
   this->setAlunoId( id );
   this->setNomeAluno( nome );
   this->formando = formando;
   this->calouro = false;
   this->oferta = oft;
   this->ofertaId = oft->getId();
   this->nCredsAlocados.clear();
   this->cargaHorariaOrigRequeridaP1 = 0;
   this->nroCredsOrigRequeridosP1 = 0;
   this->nCredsJaAlocados = 0;
   this->nroDiscsOrigRequeridosP1 = 0;
   this->prioridadeDoAluno = 0;
}

Aluno::~Aluno( void )
{
}

void Aluno::le_arvore( ItemAluno & elem )
{
   this->setAlunoId( elem.alunoId() );
   this->setNomeAluno( elem.nomeAluno() );
   this->setFormando( elem.formando() );
   this->setPrioridadeDoAluno( elem.prioridade() );
}

void Aluno::setNrMedioCredsDia()
{
	double nrCredsPorDia = 0;
	for ( auto itAlDem = demandas.begin(); itAlDem != demandas.end(); itAlDem++ )
	{
		Demanda *demanda = itAlDem->demanda;

		int nrCreds = demanda->disciplina->getTotalCreditos();
		int nrDias = demanda->disciplina->diasLetivos.size();

		double mediaPorDia = (double)nrCreds / (double)nrDias;
		nrCredsPorDia += mediaPorDia;
	}

	nrCredsMedioDia_ = (int) nrCredsPorDia;
}

int Aluno::getNrMedioCredsDia()
{ 
	if (nrCredsMedioDia_==-1)
		setNrMedioCredsDia();
	return nrCredsMedioDia_; 
}

double Aluno::getReceita( Disciplina *disciplina ) 
{ 
	AlunoDemanda *al = this->getAlunoDemandaEquiv( disciplina );
	if ( al == NULL )
	{ 
		std::cout<<"\nErro: AlunoDemanda nao encontrado para disciplina " 
			<< disciplina->getId() << " e aluno " << this->getAlunoId();
		return 0;
	}
	return al->demanda->oferta->getReceita(); 
}
 
bool Aluno::demandaDisciplina( int idDisc )
{
	ITERA_GGROUP_LESSPTR( itAlunoDemanda, this->demandas, AlunoDemanda )
	{
		if ( (*itAlunoDemanda)->demanda->getDisciplinaId() == idDisc )
		{
			return true;
		}
	}
	return false;
}

AlunoDemanda* Aluno::getAlunoDemanda( int disciplinaId )
{
	ITERA_GGROUP_LESSPTR( itAlunoDemanda, this->demandas, AlunoDemanda )
	{
		if ( (*itAlunoDemanda)->demanda->getDisciplinaId() == disciplinaId )
		{
			return (*itAlunoDemanda);
		}
	}
	return NULL;
}

AlunoDemanda* Aluno::getAlunoDemandaEquiv( Disciplina *disciplina )
{
	ITERA_GGROUP_LESSPTR( itAlunoDemanda, this->demandas, AlunoDemanda )
	{
		if ( (*itAlunoDemanda)->demanda->getDisciplinaId() == disciplina->getId() )
		{
			return (*itAlunoDemanda);
		}
	}
	ITERA_GGROUP_LESSPTR( itAlunoDemanda, this->demandas, AlunoDemanda )
	{
		if ( CentroDados::getProblemData()->alocacaoEquivViavel( (*itAlunoDemanda)->demanda, disciplina ) )
		//if ( (*itAlunoDemanda)->demanda->disciplina->discEquivSubstitutas.find( disciplina )!=
		//	 (*itAlunoDemanda)->demanda->disciplina->discEquivSubstitutas.end() )
		{
			AlunoDemanda* alDemTeor = (*itAlunoDemanda);
			int idOrig = alDemTeor->demanda->disciplina->getId();
			
			if ( disciplina->getId() > 0)
				return alDemTeor;
			else
			{
				AlunoDemanda* alDemPrat = getAlunoDemanda( -idOrig );
				if ( alDemPrat!=NULL )
					return alDemPrat;
				else
					return alDemTeor;
			}
		}
	}
	return NULL;
}

// olha para a estrutura demandasP2
AlunoDemanda* Aluno::getAlunoDemandaEquivP2( Disciplina *disciplina )
{
	for(auto itAlunoDemanda = demandasP2.cbegin(); itAlunoDemanda != demandasP2.cend(); ++itAlunoDemanda )
	{
		if ( (*itAlunoDemanda)->demanda->getDisciplinaId() == disciplina->getId() )
		{
			return (*itAlunoDemanda);
		}
	}
	for(auto itAlunoDemanda = demandasP2.cbegin(); itAlunoDemanda != demandasP2.cend(); ++itAlunoDemanda )
	{
		if ( (*itAlunoDemanda)->demanda->discIdSubstitutasPossiveis.find( disciplina->getId() )!=
			 (*itAlunoDemanda)->demanda->discIdSubstitutasPossiveis.end() )
		{
			AlunoDemanda* alDemTeor = (*itAlunoDemanda);
			int idOrig = alDemTeor->demanda->disciplina->getId();

			if ( disciplina->getId() > 0)
				return alDemTeor;
			else
			{
				AlunoDemanda* alDemPrat = getAlunoDemanda( -idOrig );
				if ( alDemPrat!=NULL )
					return alDemPrat;
				else
					return alDemTeor;
			}
		}
	}

	return nullptr;
}
 
/*
	Retorna o numero maximo de creditos possivel, dado um dia da semana (dia), 	
	um Calendario (sl) e tipo de combinação de creditos das semanas letivas (id).
*/
int Aluno::getNroMaxCredCombinaSL( int k, Calendario *c, int dia )
{
	int n = 0;

	if ( dia < 0 || dia > 7 )
	{
		std::cerr<<"Erro em Aluno::getNroCredCombinaSL(): dia invalido.";
		return 0;
	}

	Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
	t.set( dia, k, c );

	std::map< Trio< int/*dia*/, int /*k_id*/, Calendario* /*sl*/ >, int/*nroCreds*/ >::iterator 
		it = combinaCredSL.find( t );

	if ( it != combinaCredSL.end() )
	{
		n = it->second;
	}
	
	return n;
}


double Aluno::getNroCreditosJaAlocados( Calendario* c, int dia )
{
	double tempo = 0.0;

	std::map< Calendario*, std::map< int /*dia*/, double /*nCreds*/> >::iterator
		itMap1 = nCredsAlocados.find( c );
	if ( itMap1 != nCredsAlocados.end() )
	{
		std::map< int /*dia*/, double /*nCreds*/>::iterator
			itMap2 = itMap1->second.find( dia );
		if ( itMap2 != itMap1->second.end() )
			tempo += itMap2->second;
	}
	return tempo;
}

bool Aluno::totalmenteAtendido()
{
	if ( this->getNroCreditosJaAlocados() >= this->getNroCredsOrigRequeridosP1() )
		return true;
	return false;
}

int Aluno::getNroCreditosNaoAtendidos()
{
	return this->getNroCredsOrigRequeridosP1() - this->getNroCreditosJaAlocados();
}
	

/*
	Retorna todos os calendarios associados às demandas do aluno.
	Atenção: em geral, o aluno é alocado somente em disciplinas com
	o mesmo calendário de seu curriculo. Porém, exceções ocorrem em casos
	de equivalência. Nesses casos, um aluno pode ser associado a um
	calendário diferente desde que este tenha horários em comum com o
	calendário de seu curriculo.
*/
GGroup< Calendario*, LessPtr<Calendario> > Aluno::retornaSemanasLetivas()
{
	GGroup< Calendario*, LessPtr<Calendario> > calendarios;
	
	Calendario *slAluno = this->oferta->curriculo->calendario;
	calendarios.add( slAluno );
	
	// Insere possiveis demais calendarios necessarios
	ITERA_GGROUP_LESSPTR( itAlunoDemanda, this->demandas, AlunoDemanda )
	{
		if ( (*itAlunoDemanda)->demanda->disciplina->getCalendarios().find( slAluno ) ==
			 (*itAlunoDemanda)->demanda->disciplina->getCalendarios().end() )
		{
			// seleciona somente os calendarios de maior interseção de horarios com o calendario do curriculo do aluno
			GGroup< Calendario*, LessPtr<Calendario> > aux_calend;
			int max=0;

			GGroup< Calendario*, LessPtr<Calendario> > calendsDisc = (*itAlunoDemanda)->demanda->disciplina->getCalendarios();
			ITERA_GGROUP_LESSPTR( itCalend, calendsDisc, Calendario )
			{
				int n=0;
				std::map< int, GGroup<HorarioAula*, LessPtr<HorarioAula>> > dia_hors_comum =
					itCalend->retornaDiaHorariosEmComum( (*itAlunoDemanda)->demanda->disciplina->getCalendarios() );
				
				std::map< int, GGroup<HorarioAula*, LessPtr<HorarioAula>> >::iterator itMapDiaHors = dia_hors_comum.begin();
				for( ; itMapDiaHors != dia_hors_comum.end(); itMapDiaHors++ )
					n += itMapDiaHors->second.size();
				
				if ( n>max || (n==max && n!=0) )
				{
					if (n>max)
					{
						max=n;
						aux_calend.clear();
					}
					aux_calend.add( *itCalend );
				}
			}

			ITERA_GGROUP_LESSPTR( itCalend, aux_calend, Calendario )
				calendarios.add( *itCalend );		
		}
	}

	return calendarios;
}

/*	-------------------------------------------------------------------------------------------
	FUNÇÕES PARA CRIAR A REGRA DE COMBINAÇÃO DOS CREDITOS DO ALUNO
*/


void Aluno::setCombinaCredSL( int dia, int k, Calendario* sl , int n )
{
	Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
	t.first = dia;
	t.second = k;
	t.third = sl;
	combinaCredSL[t] = n;
}

void Aluno::removeCombinaCredSL( Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t )
{
	combinaCredSL.erase(t);
}

// atencao para a ordem: i refere-se a sl1 & j refere-se a sl2
bool Aluno::combinaCredSL_eh_dominado( int i, Calendario *sl1, int j, Calendario *sl2, int dia )
{
	std::map< Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ >, int/*nroCreds*/ >::iterator it_map = combinaCredSL.begin();

	for ( ; it_map != combinaCredSL.end(); it_map++  )
	{
		if ( it_map->first.first == dia &&
			 it_map->first.third == sl1 &&
			 it_map->second > i )
		{
			int k = it_map->first.second;

			Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
			t.set(dia, k, sl2);

			if ( combinaCredSL[t] >= j )
			{
					return true;
			}
		}
		else if ( it_map->first.first == dia &&
			 it_map->first.third == sl1 &&
			 it_map->second == i )
		{
			int k = it_map->first.second;

			Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
			t.set(dia, k, sl2);

			if ( combinaCredSL[t] > j )
			{
					return true;
			}
		}
	}

	return false;
}

// atencao para a ordem: i refere-se a sl1 & j refere-se a sl2
bool Aluno::combinaCredSL_domina( int i, Calendario *sl1, int j, Calendario *sl2, int dia )
{
	std::map< Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ >, int/*nroCreds*/ >::iterator it_map = combinaCredSL.begin();

	for ( ; it_map != combinaCredSL.end(); it_map++  )
	{
		if ( it_map->first.first == dia &&
			 it_map->first.third == sl1 &&
			 it_map->second < i )
		{
			int k = it_map->first.second;
			Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
			t.set(dia, k, sl2);

			if ( combinaCredSL[t] <= j )
			{
				return true;
			}
		}
		else
		{
			if ( it_map->first.first == dia &&
				 it_map->first.third == sl1 &&
				 it_map->second == i )
			{
				int k = it_map->first.second;
				Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
				t.set(dia, k, sl2);

				if ( combinaCredSL[t] < j )
				{
					return true;
				}
			}
		}
	}

	return false;
}


// atencao para a ordem: i refere-se a sl1 & j refere-se a sl2
bool Aluno::combinaCredSL_eh_repetido( int i, Calendario *sl1, int j, Calendario *sl2, int dia )
{
	std::map< Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ >, int/*nroCreds*/ >::iterator it_map = combinaCredSL.begin();

	for ( ; it_map != combinaCredSL.end(); it_map++  )
	{
		if ( it_map->first.first == dia &&
			 it_map->first.third == sl1 &&
			 it_map->second == i )
		{
			int k = it_map->first.second;
			Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t;
			t.set(dia, k, sl2);

			if ( combinaCredSL[t] == j )
			{
				return true;
			}
		}
	}

	return false;
}

std::map< Trio<int, int, Calendario*>, int > Aluno::retornaCombinaCredSL_Dominados( int dia )
{
	std::map< Trio<int, int, Calendario*>, int > dominados;

	std::map< Trio< int, int, Calendario* >, int >::iterator it1_map = combinaCredSL.begin();
	for ( ; it1_map != combinaCredSL.end(); it1_map++  )
	{
		if ( it1_map->first.first == dia )
		{
			Calendario *sl1 = it1_map->first.third;
			int n1 = it1_map->second;
			int k = it1_map->first.second;

			std::map< Trio< int, int, Calendario* >, int >::iterator it2_map = combinaCredSL.begin();
			for ( ; it2_map != combinaCredSL.end(); it2_map++  )
			{
				if ( it2_map != it1_map &&
					 it2_map->first.first == dia &&
					 it2_map->first.second == k )
				{
					Calendario *sl2 = it2_map->first.third;
					int n2 = it2_map->second;
					
					if ( combinaCredSL_eh_dominado( n1, sl1, n2, sl2, dia ) )
					{
						dominados.insert( *it1_map );
						dominados.insert( *it2_map );
					}

				}
			}
		}
	}

	return dominados;
}

//	-------------------------------------------------------------------------------------------

 bool Aluno::sobrepoeHorarioDiaOcupado( HorarioDia *hd )
 {
	 std::map< int /*dia*/, std::map< DateTime, HorarioDia*> >::iterator
		 itDia = horariosDiaOcupados.find( hd->getDia() );
	 if ( itDia != horariosDiaOcupados.end() )
	 {
		 std::map< DateTime, HorarioDia*> *horsDia = & itDia->second;
		 std::map< DateTime, HorarioDia*>::iterator itDt = horsDia->begin();
		 for( ; itDt != horsDia->end(); itDt++ )
		 {
			 HorarioAula *horAulaOcup = itDt->second->getHorarioAula();

			 if ( horAulaOcup->getInicio() > hd->getHorarioAula()->getFinal() )
				 return false;

			 if ( horAulaOcup->sobrepoe( *hd->getHorarioAula() ) )
				 return true;
		 }
	 }
	 return false;
 }

 bool Aluno::sobrepoeHorarioDiaOcupado( HorarioAula *ha, int dia )
 {
	 std::map< int /*dia*/, std::map< DateTime, HorarioDia*> >::iterator
		 itDia = horariosDiaOcupados.find( dia );
	 if ( itDia != horariosDiaOcupados.end() )
	 {
		 std::map< DateTime, HorarioDia*> *horsDia = & itDia->second;
		 std::map< DateTime, HorarioDia*>::iterator itDt = horsDia->begin();
		 for( ; itDt != horsDia->end(); itDt++ )
		 {
			 HorarioAula *horAulaOcup = itDt->second->getHorarioAula();
			 
			 if ( horAulaOcup->getInicio() > ha->getFinal() )
				 return false;

			 if ( horAulaOcup->sobrepoe( *ha ) )
				 return true;
		 }
	 }
	 return false;
 }

void Aluno::addHorarioDiaOcupado( HorarioDia* hd )
{
	horariosDiaOcupados[ hd->getDia() ];
	std::map< DateTime, HorarioDia*> *hors = & horariosDiaOcupados[ hd->getDia() ];
	if ( hors->find( hd->getHorarioAula()->getInicio() ) == hors->end() )
	{
		(*hors)[ hd->getHorarioAula()->getInicio() ] = hd;
		nCredsJaAlocados++;
	}
}

 void Aluno::addHorarioDiaOcupado( GGroup<HorarioDia*> hds )
 { 
	 ITERA_GGROUP( it, hds, HorarioDia )
	 {
		 int dia = it->getDia();
		 std::map< DateTime, HorarioDia*> *mapDt = & horariosDiaOcupados[ dia ];
		 DateTime dt = it->getHorarioAula()->getInicio();

		 if ( mapDt->find( dt ) == mapDt->end() )
		 {
			(*mapDt)[ dt ] = *it;
			nCredsJaAlocados++;
		 }		 
	 }
 }
  
void Aluno::removeHorarioDiaOcupado( HorarioDia* hd )
{ 
	std::map< int /*dia*/, std::map< DateTime, HorarioDia*> >::iterator
		itDia = horariosDiaOcupados.find( hd->getDia() );
	if ( itDia != horariosDiaOcupados.end() )
	{
		std::map< DateTime, HorarioDia*>::iterator 
			itDt = itDia->second.find( hd->getHorarioAula()->getInicio() );
		if ( itDt != itDia->second.end() )
		{
			itDia->second.erase( hd->getHorarioAula()->getInicio() );
			nCredsJaAlocados--;
		}
	}	   
}

 bool Aluno::sobrepoeAulaJaAlocada( HorarioAula *hi, HorarioAula *hf, int dia )
 {
	Calendario *calendario = hi->getCalendario();
	int nCreds = calendario->retornaNroCreditosEntreHorarios( hi, hf );
	HorarioAula *ha = hi;

	bool fim=false;
	for ( int i = 1; i <= nCreds; i++ )
	{
		if ( this->sobrepoeHorarioDiaOcupado( ha, dia ) )
			return true;

		if ( ha == hf )
			fim=true;
		ha = calendario->getProximoHorario( ha );
	}
	
	if (!fim) 
		std::cout<<"\nErro em Aluno::sobrepoeAulaJaAlocada: nro de creditos nao corresponde ao intervalo hi-hf"
		<<"\nhi = "<<hi->getId()<<" hf = "<<hf->getId()<<" dia = "<<dia<<" nCreds = "<<nCreds<<endl;

	return false;
 }

 void Aluno::defineSeEhCalouro()
 {
	 bool calourinho = true;

	ITERA_GGROUP_LESSPTR( itAlunoDemanda, this->demandas, AlunoDemanda )
	{
		Curriculo *curr = (*itAlunoDemanda)->demanda->oferta->curriculo;
		Disciplina *disciplina = (*itAlunoDemanda)->demanda->disciplina;

		int periodo = curr->getPeriodo( disciplina );
		if ( periodo != 1 )
		{
			calourinho = false;
			break;
		}
	}	 
	
	if ( this->demandas.size() == 0 )
		calourinho = false;

	this->setCalouro( calourinho );
 }

 std::string Aluno::getHorariosOcupadosStr( int dia )
 { 
	 std::stringstream ss;
	 ss.clear();
	 std::map< int /*dia*/, std::map< DateTime, HorarioDia*> >::iterator
		 itDia = horariosDiaOcupados.find( dia );
	 if ( itDia != horariosDiaOcupados.end() )
	 {
		 std::map< DateTime, HorarioDia*>::iterator itMapDt = itDia->second.begin();
		 for ( ; itMapDt != itDia->second.end(); itMapDt++ )
		 {
			 DateTime dti = itMapDt->second->getHorarioAula()->getInicio();
			 DateTime dtf = itMapDt->second->getHorarioAula()->getFinal();

			 ss << dti.getHour() << ":" << dti.getMinute()
				 << " - "
				 << dtf.getHour() << ":" << dtf.getMinute();
			 ss << " || ";
		 }
	 }
	 return ss.str();
 }
 
