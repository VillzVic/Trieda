#include "ProblemData.h"

#include <iostream>
#include <fstream>
#include <iomanip>

#include "Indicadores.h"
#include "CentroDados.h"


bool DEBUGANDO = false;


// Macro baseada na ITERA_SEQ para facilitar a leitura dos dados no
// ProblemData
#define LE_SEQ(ggroup,addr,type) \
   for (Grupo##type::type##_iterator __##type##_it = \
   (addr).type().begin(); __##type##_it != (addr).type().end(); \
   ++ __##type##_it) { \
   type *__obj_##type = new type; \
   __obj_##type->le_arvore(*__##type##_it); \
   ggroup.add(__obj_##type); } 


bool less_than( DateTime left, DateTime right )
{
	// Ignora o dia dos DateTimes

	if ( left.getHour() < right.getHour() )
	{
		return true;
	}
	if ( left.getHour() == right.getHour() &&
		 left.getMinute() < right.getMinute() )
	{
		return true;
	}
	return false;
}


ProblemData::ProblemData( char inputName[1024], int inputId )
{
	inputFileName = inputName;
		
	this->setInputId( inputId );
   
	EQUIV_TRANSITIVIDADE = false;

   this->atendimentosTatico = NULL;

//#ifndef HEURISTICA
//   this->solTaticoInicial = new SolucaoTaticoInicial();
//#endif
	  
   // TODO: isso não deveria ser fixo aqui. Tem que ser geral, só para o problemData.
   // Quando passar a vir definido pelo cliente, mudar isso!!! Na Sala também tem getTurno()!!
      
   DateTime tarde;
   tarde.setHour(12);
   tarde.setMinute(30);
   this->setInicioTarde( tarde );

#ifdef UNIRITTER
   DateTime noite;
   noite.setHour(17);
   noite.setMinute(0);
   this->setInicioNoite( noite );
#elif defined KROTON
   DateTime noite;
   noite.setHour(17);
   noite.setMinute(40);
   this->setInicioNoite( noite );
#elif defined UNIT
   DateTime noite;
   noite.setHour(18);
   noite.setMinute(0);
   this->setInicioNoite( noite );
#endif

   nroTurnos=3;
}

ProblemData::~ProblemData()
{
	turnosIES.deleteElements();
    calendarios.deleteElements();
    tipos_sala.deleteElements();
    tipos_contrato.deleteElements();
    tipos_titulacao.deleteElements();
    areas_titulacao.deleteElements();
    tipos_disciplina.deleteElements();
    niveis_dificuldade.deleteElements();

	cout << "\n deleting tipos_curso..."; fflush(0);
    tipos_curso.deleteElements();
	
	cout << "\n deleting regras_div..."; fflush(0);
    regras_div.deleteElements();
	
	cout << "\n deleting tempo_campi..."; fflush(0);
    tempo_campi.deleteElements();
	
	cout << "\n deleting tempo_unidades..."; fflush(0);
    tempo_unidades.deleteElements();
	
	cout << "\n deleting disciplinas..."; fflush(0);
    disciplinas.deleteElements();
	
	cout << "\n deleting cursos..."; fflush(0);
    cursos.deleteElements();

	cout << "\n deleting demandas..."; fflush(0);
    demandas.deleteElements();
	
	cout << "\n deleting ofertas..."; fflush(0);
    ofertas.deleteElements();
	
	cout << "\n deleting parametros..."; fflush(0);
    if(parametros) delete parametros;
	
	cout << "\n deleting fixacoes..."; fflush(0);
    fixacoes.deleteElements();
	
	cout << "\n deleting alunosDemanda..."; fflush(0);
    alunosDemanda.deleteElements();
	
	cout << "\n deleting alunos..."; fflush(0);
    alunos.deleteElements();
	
	cout << "\n deleting profsVirtuais..."; fflush(0);
    profsVirtuais.deleteElements(); // 1 por tipo de titulação
	
	cout << "\n deleting equivalencias..."; fflush(0);
    equivalencias.deleteElements(); // só contém as teoricas
	
	cout << "\n deleting horariosDia..."; fflush(0);
	horariosDia.deleteElements();

	cout << "\n clearHorarioAula..."; fflush(0);
	clearHorarioAula();
	
	cout << "\n deleting campi..."; fflush(0);
    campi.deleteElements();
}

void ProblemData::clearHorarioAula()
{
	for ( auto it = refHorarioAula.begin(); it != refHorarioAula.end(); )
	{
		HorarioAula *h = it->second;
		refHorarioAula.erase(it);
		if (h) delete h;
	}
}

std::string ProblemData::inputIdToString()
{  
	std::string s;
	int id = this->getInputId();
	if ( id != 0 )
	{
		stringstream ss;
		ss << id;
		s += "_"; 
		s += "id"; 
		s += ss.str();
	}
	return s;
}

string ProblemData::getDiaEmString( int dia, bool comGenero )
{
	string str;
	string genero;

	switch (dia)
	{
	case 2:
		str = "segunda-feira";
		genero = "a ";
		break;
	case 3:
		str = "terca-feira";
		genero = "a ";
		break;
	case 4:
		str = "quarta-feira";
		genero = "a ";
		break;
	case 5:
		str = "quinta-feira";
		genero = "a ";
		break;
	case 6:
		str = "sexta-feira";
		genero = "a ";
		break;
	case 7:
		str = "sabado";
		genero = "o ";
		break;
	case 8:
		str = "domingo";
		genero = "o ";
		break;
	default:
		str = "error-dia";
		genero = "";
		break;
	}
	
	if ( comGenero )
		return (genero + str);
	else
		return str;
}

void ProblemData::defineFasesDosTurnos()
{	

	// Calcula as fases (inicio e fim) para cada Calendario

	std::map< Calendario*, GGroup<DateTime> > mapCalendDateTime;
	std::map< Calendario*, std::vector< std::pair<DateTime,DateTime> > > map_Calend_FasesDtIf;

	ITERA_GGROUP_LESSPTR( itCalend, calendarios, Calendario )
	{
		Calendario *c = *itCalend;

		ITERA_GGROUP_LESSPTR( itTurnoIES, c->turnosIES, TurnoIES )
		{
			ITERA_GGROUP_LESSPTR( itHor, itTurnoIES->horarios_aula, HorarioAula )
			{
				if ( c->possuiHorario( *itHor ) )
				{
					mapCalendDateTime[ c ].add( itHor->getInicio() );
				}
			}
		}

		bool turnoDif;
		int turnoIESant;

		int tempo = c->getTempoAula();
		DateTime dt_ant;
		dt_ant.setHour(0);
		dt_ant.setMinute(0);

		auto finderC = mapCalendDateTime.find(c);
		if (finderC != mapCalendDateTime.end())
		{
			GGroup<DateTime> ggroup = finderC->second;

			ITERA_GGROUP_N_PT( itDT, ggroup, DateTime )
			{
				int turnoIESatual = c->getTurnoIES( *itDT );
				if ( itDT == ggroup.begin() ) turnoIESant = turnoIESatual;

				turnoDif = ( turnoIESant != turnoIESatual ? true : false );

				DateTime dt = *itDT;
				DateTime dif = dt - dt_ant;
				int delta = dif.getHour()*60 + dif.getMinute();
				if ( ( delta>=2*tempo ) || ( delta>=tempo && turnoDif ) )
				{		
					// Intervalo significativo => provável mudança de fase

					int n = map_Calend_FasesDtIf[c].size();
					if ( n >= 1 )
					{					
						map_Calend_FasesDtIf[c][n-1].second = dt_ant;
					}

					std::pair<DateTime,DateTime> parDT;
					parDT.first = dt;
					map_Calend_FasesDtIf[c].push_back( parDT );
				}
				dt_ant = dt;
				turnoIESant = turnoIESatual;
			}
			map_Calend_FasesDtIf[c].rbegin()->second = dt_ant;
		}
	}

	bool debugging = true;
	// Define os horarios das 3 fases possíveis existentes: Manha, Tarde, Noite
	
	DateTime hrMin;
	hrMin.setHour(0);
	hrMin.setMinute(0);
	DateTime hrMax;
	hrMax.setHour(24);
	hrMax.setMinute(0);

	std::pair<DateTime,DateTime> fase1( hrMax, hrMin );
	std::pair<DateTime,DateTime> fase2( hrMax, hrMin );
	std::pair<DateTime,DateTime> fase3( hrMax, hrMin );
	DateTime hr12;
	hr12.setHour(12);
	hr12.setMinute(0);
	DateTime hr18;
	hr18.setHour(18);
	hr18.setMinute(0);

	std::map< Calendario*, std::vector< std::pair<DateTime,DateTime> > >::iterator
		itMapCalendFases = map_Calend_FasesDtIf.begin();
	for ( ; itMapCalendFases != map_Calend_FasesDtIf.end(); itMapCalendFases++ )
	{
		Calendario *c = itMapCalendFases->first;

		if ( itMapCalendFases->second.size() > 3 )
			std::cout<<"\nATENCAO: Mais de 3 fases na semana letiva!? Verificar.";

		for ( int i = 0; i < itMapCalendFases->second.size(); i++ )
		{
			DateTime dti = itMapCalendFases->second[i].first;
			DateTime dtf = itMapCalendFases->second[i].second;
			
			DateTime diff12 = ( dtf < hr12 ? hr12-dtf : dtf-hr12 );
			int deltaf12 = diff12.getHour()*60 + diff12.getMinute();

			DateTime diff18 = ( dtf < hr18 ? hr18-dtf : dtf-hr18 );
			int deltaf18 = diff18.getHour()*60 + diff18.getMinute();
			
			DateTime difi12 = ( dti < hr12 ? hr12-dti : dti-hr12 );
			int deltai12 = difi12.getHour()*60 + difi12.getMinute();

			DateTime difi18 = ( dti < hr18 ? hr18-dti : dti-hr18 );
			int deltai18 = difi18.getHour()*60 + difi18.getMinute();
			
			if ( dti < hr12 && deltaf12 < 100 )								// Manhã
			{
				if ( fase1.first > dti ) fase1.first = dti;
				if ( fase1.second < dtf ) fase1.second = dtf;
				
				if ( debugging ) std::cout<<"\nManha: i=" << dti << " f=" << dtf;
				this->fasesDosTurnos[ Manha ] = fase1;
			}
			else if ( dti < hr12 && deltai12 > 100 && deltaf18 < 100 )		// Manhã-Tarde
			{
				if ( fase1.first > dti ) fase1.first = dti;
				if ( fase2.second < dtf ) fase2.second = dtf;
				
				if ( debugging ) std::cout<<"\nManhaTarde: i=" << dti << " f=" << dtf;
				this->fasesDosTurnos[ Manha ] = fase1;
				this->fasesDosTurnos[ Tarde ] = fase2;
			}
			else if ( dti < hr18 && deltaf18 < 100  )						// Tarde
			{
				if ( fase2.first > dti ) fase2.first = dti;
				if ( fase2.second < dtf ) fase2.second = dtf;
				
				if ( debugging ) std::cout<<"\nTarde: i=" << dti << " f=" << dtf;
				this->fasesDosTurnos[ Tarde ] = fase2;
			}
			else if ( dti < hr18 && deltai12 < 100 && dtf > hr18 && deltaf18 > 100  )	// Tarde-Noite
			{
				if ( fase2.first > dti ) fase2.first = dti;
				if ( fase3.second < dtf ) fase3.second = dtf;
				
				if ( debugging ) std::cout<<"\nTardeNoite: i=" << dti << " f=" << dtf;
				this->fasesDosTurnos[ Tarde ] = fase2;
				this->fasesDosTurnos[ Noite ] = fase3;
			}
			else if ( dtf > hr18 && deltai18 < 100 )						// Noite
			{
				if ( fase3.first > dti ) fase3.first = dti;
				if ( fase3.second < dtf ) fase3.second = dtf;				
				
				if ( debugging ) std::cout<<"\nNoite: i=" << dti << " f=" << dtf;
				this->fasesDosTurnos[ Noite ] = fase3;
			}
			else if ( dti < hr12 && deltai12 > 100 && dtf > hr18 && deltaf18 > 100 )		// Integral
			{
				if ( fase1.first > dti ) fase1.first = dti;
				if ( fase3.second < dtf ) fase3.second = dtf;				
				
				if ( debugging ) std::cout<<"\nIntegral: i=" << dti << " f=" << dtf;
				this->fasesDosTurnos[ Manha ] = fase1;
				this->fasesDosTurnos[ Tarde ] = fase2;
				this->fasesDosTurnos[ Noite ] = fase3;
			}
		}
	}
	
	if ( debugging ) std::cout<<"\n\nFase 1: i=" << fase1.first << " f=" << fase1.second;
	if ( debugging ) std::cout<<"\nFase 2: i=" << fase2.first << " f=" << fase2.second;
	if ( debugging ) std::cout<<"\nFase 3: i=" << fase3.first << " f=" << fase3.second << std::endl;

	if ( fase2.first < fase1.second )
		std::cout<<"\nERRO: Inicio da fase 2 esta ocorrendo antes do fim da fase 1.";
	if ( fase3.first < fase2.second )
		std::cout<<"\nERRO: Inicio da fase 3 esta ocorrendo antes do fim da fase 2.";

	if ( this->fasesDosTurnos.find( Tarde ) != this->fasesDosTurnos.end() )
		this->setInicioTarde( fase2.first );	

	if ( this->fasesDosTurnos.find( Noite ) != this->fasesDosTurnos.end() )
		this->setInicioNoite( fase3.first );
	
}

void ProblemData::copiaFasesDosTurnosParaSalas()
{
	ITERA_GGROUP_LESSPTR( itCp, campi, Campus )
	{
		ITERA_GGROUP_LESSPTR( itUnid, itCp->unidades, Unidade )
		{
			ITERA_GGROUP_LESSPTR( itSala, itUnid->salas, Sala )
			{
				itSala->setFasesTurnos( this->getInicioTarde(), this->getInicioNoite() );
			}		
		}		
	}
}

int ProblemData::getHorarioDiaIdx( HorarioDia * horarioDia )
{
   if ( horarioDia == NULL )
   {
      return -1;
   }

   return ( horarioDia->getDia() * ( maxHorariosDif + 1 ) + horarioDia->getHorarioAulaId() );
}

int ProblemData::getHorarioDiaIdx( int dia, int horarioId )
{
   return ( dia * ( maxHorariosDif + 1 ) + horarioId );
}

bool ProblemData::getPairDateTime(int horId, std::pair<DateTime*,int> & result) const
{
	auto finder = horarioAulaDateTime.find(horId);
	if ( finder != horarioAulaDateTime.end() )
	{
		result = finder->second;
		return true;
	}
	return false;
}

DateTime* ProblemData::getDateTimeInicial( DateTime dt )
{
	// fim
	DateTime *newFinalDt = new DateTime( dt );
    GGroup<DateTime*,LessPtr<DateTime> >::iterator itDtf = this->horariosInicioValidos.find(newFinalDt);
	delete newFinalDt;

    if ( itDtf != this->horariosInicioValidos.end() )
    {	    
		return *itDtf;
    }
	return nullptr;
}

DateTime* ProblemData::getDateTimeFinal( DateTime dt )
{
	// fim
	DateTime *newFinalDt = new DateTime( dt );
    GGroup<DateTime*,LessPtr<DateTime> >::iterator itDtf = this->horariosTerminoValidos.find(newFinalDt);
	delete newFinalDt;

    if ( itDtf != this->horariosTerminoValidos.end() )
    {	    
		return *itDtf;
    }
	return nullptr;
}

// Retorna todos os DateTime que são anteriores à dtf
GGroup<DateTime*, LessPtr<DateTime> > ProblemData::getDtiAnteriores( DateTime dtf )
{
	GGroup<DateTime*, LessPtr<DateTime> > dtAnteriores;

    GGroup<DateTime*,LessPtr<DateTime> >::iterator itDti = this->horariosInicioValidos.begin();
	while ( itDti != this->horariosInicioValidos.end() && itDti->earlierTime(dtf) )
	{
		dtAnteriores.add( *itDti );
		itDti++;
	}

	return dtAnteriores;
}

void ProblemData::setDateTimesSobrepostos()
{
	// Preenche inicialmente com todos os DateTimes iniciais existentes
	ITERA_GGROUP_LESSPTR( itCalend, this->calendarios, Calendario )
	{
		ITERA_GGROUP_LESSPTR( itHorAula, itCalend->horarios_aula, HorarioAula )
		{		
			HorarioAula *ha = *itHorAula;
						  
			std::pair<DateTime,DateTime> par( ha->getInicio(), ha->getFinal() );

			mapDtiSobrepoeDtiDtf[ ha->getInicio() ].add( par );
		}
	}

	// Preenche com todos os pares de DateTimes sobrepostos existentes
	std::map<DateTime, GGroup< std::pair<DateTime,DateTime> > >::iterator
		itMapDt = mapDtiSobrepoeDtiDtf.begin();
	for ( ; itMapDt != mapDtiSobrepoeDtiDtf.end(); itMapDt++ )
	{
		DateTime dt = itMapDt->first;
				
		std::map<DateTime, GGroup< std::pair<DateTime,DateTime> > >::iterator
			itDt2 = mapDtiSobrepoeDtiDtf.begin();
		for( ; itDt2 != mapDtiSobrepoeDtiDtf.end() ; itDt2++ )
		{
			// Só inicios menores que dt
			if ( itDt2->first > dt )
				break;

			GGroup< std::pair<DateTime,DateTime> > *paresDtiDtf = & itDt2->second;
			GGroup< std::pair<DateTime,DateTime> >::iterator
				itPares = paresDtiDtf->begin();
			for( ; itPares != paresDtiDtf->end(); itPares++ )
			{
				DateTime dti = (*itPares).first;
				DateTime dtf = (*itPares).second;

				if ( dti <= dt && dt < dtf )
				{
					mapDtiSobrepoeDtiDtf[ dt ].add( *itPares );
				}
			}
		}
	}
}

GGroup< std::pair<DateTime,DateTime> >* ProblemData::getDateTimesSobrepostos( DateTime dt )
{
	GGroup< std::pair<DateTime,DateTime> > *ggroup;

	map<DateTime, GGroup< std::pair<DateTime,DateTime> > >::iterator
		itDt = this->mapDtiSobrepoeDtiDtf.find( dt );
	if ( itDt != this->mapDtiSobrepoeDtiDtf.end() )
		ggroup = & itDt->second;

	return ggroup;
}

TipoTitulacao* ProblemData::retornaTipoTitulacaoMinimo()
{
	TipoTitulacao* tit = NULL;
	int minTit = 9999999;
	ITERA_GGROUP_LESSPTR( itTit, tipos_titulacao, TipoTitulacao )
	{
		if ( itTit->getTitulacao() < minTit )
		{
			tit = *itTit;
			minTit = tit->getTitulacao();
		}
	}
	return tit;
}

TipoTitulacao* ProblemData::retornaTitulacao( int tit )
{
	TipoTitulacao *titulacao = new TipoTitulacao( tit );
	GGroup< TipoTitulacao *, LessPtr< TipoTitulacao > >::iterator
		itTit = this->tipos_titulacao.find( titulacao );
	delete titulacao;
	titulacao = nullptr;
	if ( itTit != this->tipos_titulacao.end() )
	{
		titulacao = *itTit;
	}

	return titulacao;
}

TipoTitulacao* ProblemData::retornaTitulacaoPorId( int tit_id )
{
	TipoTitulacao *titulacao = NULL;
	GGroup< TipoTitulacao *, LessPtr< TipoTitulacao > >::iterator
		itTit = this->tipos_titulacao.begin();
	for ( ; itTit != this->tipos_titulacao.end(); itTit++ )
	{
		if ( itTit->getId() == tit_id )
		{
			titulacao = *itTit;
			break;
		}
	}
	return titulacao;
}

TipoContrato* ProblemData::retornaTipoContratoMinimo()
{
	TipoContrato* contr = NULL;
	int minContr = 9999999;
	ITERA_GGROUP_LESSPTR( itContr, tipos_contrato, TipoContrato )
	{
		if ( itContr->getContrato() < minContr )
		{
			contr = *itContr;
			minContr = contr->getContrato();
		}
	}
	return contr;
}

TipoContrato* ProblemData::retornaTipoContrato( int id )
{
	TipoContrato *contrato = new TipoContrato( id );
	GGroup< TipoContrato *, LessPtr< TipoContrato > >::iterator
		itContr = this->tipos_contrato.find( contrato );
	delete contrato;
	contrato = nullptr;
	if ( itContr != this->tipos_contrato.end() )
	{
		contrato = *itContr;
	}

	return contrato;
}

TipoContrato* ProblemData::retornaTipoContratoPorId( int contr_id )
{
	TipoContrato *contrato = NULL;
	GGroup< TipoContrato *, LessPtr< TipoContrato > >::iterator
		itContr = this->tipos_contrato.begin();
	for ( ; itContr != this->tipos_contrato.end(); itContr++ )
	{
		if ( itContr->getId() == contr_id )
		{
			contrato = *itContr;
			break;
		}
	}
	return contrato;
}

bool ProblemData::temAtendTatInicialFixado( int alDemId )
{ 
	ITERA_GGROUP_LESSPTR( itCp, (*this->atendimentosTatico), AtendimentoCampusSolucao )
	{
		ITERA_GGROUP( itUnid, itCp->atendimentosUnidades, AtendimentoUnidadeSolucao )
		{
			ITERA_GGROUP( itSala, itUnid->atendimentosSalas, AtendimentoSalaSolucao )
			{
				ITERA_GGROUP( itDia, itSala->atendimentosDiasSemana, AtendimentoDiaSemanaSolucao )
				{
					ITERA_GGROUP( itTat, itDia->atendimentosTatico, AtendimentoTaticoSolucao )
					{
						// TODO: AlunoDemanda ainda não tem tag propria e individual de fixação
						if ( itTat->getFixaAbertura() )
						{
							AtendimentoOfertaSolucao *atendOft = itTat->atendimento_oferta;
							ITERA_GGROUP_N_PT( itAlDem, atendOft->alunosDemandasAtendidas, int )
							{
								if ( *itAlDem == alDemId )
								{
									return true;
								}
							}
						}
					}				
				}			
			}			
		}
	}
	return false;
}

void ProblemData::le_turnosIES( TriedaInput & raiz )
{
   ITERA_SEQ( it_calend, raiz.calendarios(), Calendario )
   {
	   int calendarioId = it_calend->id();

	   ITERA_SEQ( it_turno, it_calend->turnos(), TurnoIES )
	   {
	 	  int turnoIESId = it_turno->id();
		  TurnoIES *turnoIES = new TurnoIES();
		  turnoIES->setId( turnoIESId );

		  GGroup< TurnoIES *, LessPtr< TurnoIES> >::iterator 
			  itTurnoIES = this->turnosIES.find( turnoIES );
		  if ( itTurnoIES == this->turnosIES.end() )
		  {
			  turnoIES->setNome( it_turno->nome() );
			  this->turnosIES.add( turnoIES );
		  }
		  else
		  {
			  delete turnoIES;
			  turnoIES = *itTurnoIES;
		  } 

		  // Procura o objeto Calendario
		  Calendario *calendario = new Calendario();
		  calendario->setId( calendarioId );
		  GGroup< Calendario *, LessPtr< Calendario > >::iterator 
			  itCalend = this->calendarios.find( calendario );
		  if ( itCalend == this->calendarios.end() )
		  {
			  std::cout<<"\nErro, calendario " << calendarioId << " nao criado.";
		  }
		  else
		  {
			delete calendario;
			calendario = *itCalend;
		  }

		  // Adiciona horarios ao turno -----
		  ITERA_GGROUP_LESSPTR( it_horarios, calendario->horarios_aula, HorarioAula )
		  {
			HorarioAula * horario = *it_horarios;
			if ( horario->getTurnoIESId() == turnoIESId )
				turnoIES->addHorarioAula( horario );		
		  }
		  // --------------------------------

		  calendario->addTurnoIES( turnoIES );
	   }
   }

}

void ProblemData::le_arvore( TriedaInput & raiz )
{
   this->setCenarioId( 0 );//raiz.cenarioId() );
   this->calendarios.clear();
   LE_SEQ( this->calendarios, raiz.calendarios(), Calendario );
	  
   this->le_turnosIES( raiz );

   LE_SEQ( this->campi, raiz.campi(), Campus );
   LE_SEQ( this->tipos_sala, raiz.tiposSala(), TipoSala );   
   LE_SEQ( this->tipos_sala, raiz.tiposSala(), TipoSala );
   LE_SEQ( this->tipos_contrato, raiz.tiposContrato(), TipoContrato );
   LE_SEQ( this->tipos_titulacao, raiz.tiposTitulacao(), TipoTitulacao );
   LE_SEQ( this->areas_titulacao, raiz.areasTitulacao(), AreaTitulacao );
   LE_SEQ( this->tipos_disciplina, raiz.tiposDisciplina(), TipoDisciplina );
   LE_SEQ( this->niveis_dificuldade, raiz.niveisDificuldade(), NivelDificuldade );
   LE_SEQ( this->tipos_curso, raiz.tiposCurso(), TipoCurso );
   LE_SEQ( this->regras_div, raiz.regrasDivisaoCredito(), DivisaoCreditos );
   LE_SEQ( this->tempo_campi, raiz.temposDeslocamentosCampi(), Deslocamento );
   LE_SEQ( this->tempo_unidades, raiz.temposDeslocamentosUnidades(), Deslocamento );
   LE_SEQ( this->disciplinas, raiz.disciplinas(), Disciplina );
   LE_SEQ( this->cursos, raiz.cursos(), Curso );
   LE_SEQ( this->demandasTotal, raiz.demandas(), Demanda );
   LE_SEQ( this->equivalencias, raiz.equivalencias(), Equivalencia );

   TriedaInput::fixacoes_type & list_fixacoes = raiz.fixacoes();
   LE_SEQ( this->fixacoes, list_fixacoes, Fixacao );
   
   this->parametros = new ParametrosPlanejamento;
   this->parametros->le_arvore( raiz.parametrosPlanejamento() );
   
   refDiscEquivalencias();
   checkParamUsaEquivalencia();

   this->addHorariosAnalogosDisciplina();

   leOfertas(raiz);
      
   setModoOtimizacao();

   leAtendimentoTatico(raiz);

   leDemandas(raiz);

   criaPerfilProfVirtual();

   refTitContProfReal();

   preencheHorSalas(raiz);

}

void ProblemData::refDiscEquivalencias()
{
   ITERA_GGROUP_LESSPTR( itEquiv, this->equivalencias, Equivalencia )
   {
	   int disc1 = itEquiv->getDisciplinaEliminaId();
	   int disc2 = itEquiv->getDisciplinaCursouId();
	   int idEquiv = itEquiv->getId();

	   this->mapParDisc_EquivId[ std::make_pair( disc1, disc2 ) ] = idEquiv;

	   // ---------------------------------------------------
	   // Adiciona a equivalencia nos objetos Disciplina correspondentes

		Disciplina *disciplina1 = new Disciplina();
		disciplina1->setId( disc1 );
		Disciplina *disciplina2 = new Disciplina();
		disciplina2->setId( disc2 );

		GGroup< Disciplina *, LessPtr< Disciplina > >::iterator itDisc1 =
			this->disciplinas.find( disciplina1 );
		GGroup< Disciplina *, LessPtr< Disciplina > >::iterator itDisc2 =
			this->disciplinas.find( disciplina2 );

		delete disciplina1;
		delete disciplina2;

		if ( itDisc1 != this->disciplinas.end() && itDisc2 != this->disciplinas.end() )
		{
			disciplina1 = *itDisc1;
			disciplina2 = *itDisc2;
			disciplina1->addDiscSubstituta( disciplina2 );
			disciplina2->addDiscSubstituida( disciplina1 );
		}
		else
		{
			stringstream msg;
			msg << "Equivalencia refenciando disciplinas " << disc1 << " e " << disc2 << " que nao foram encontradas.";
			CentroDados::printError( "void ProblemData::le_arvore( TriedaInput & raiz )", msg.str() );
		}
	   // ---------------------------------------------------
   }
}

void ProblemData::checkParamUsaEquivalencia()
{
	if (this->parametros->considerar_equivalencia_por_aluno ||
		this->parametros->considerar_equivalencia)
	{
		if (this->equivalencias.size() == 0)
		{
			this->parametros->considerar_equivalencia_por_aluno = false;
			this->parametros->considerar_equivalencia = false;
		}
	}
}

void ProblemData::leOfertas(TriedaInput & raiz)
{
   ITERA_SEQ( it_oferta,
      raiz.ofertaCursosCampi(), OfertaCurso )
   {
      Oferta * oferta = new Oferta;
      oferta->le_arvore( *it_oferta );
      this->ofertas.add( oferta );
   }	
}

void ProblemData::setModoOtimizacao()
{
   if ( this->parametros->modo_otimizacao == "TATICO" )
	   this->setModoOtimizacao( ProblemData::TATICO );
   else if ( this->parametros->modo_otimizacao == "OPERACIONAL" && this->atendimentosTatico != NULL )
	   this->setModoOtimizacao( ProblemData::OPERACIONAL_COM_TAT );
   else if ( this->parametros->modo_otimizacao == "OPERACIONAL" && this->atendimentosTatico == NULL  )
	   this->setModoOtimizacao( ProblemData::OPERACIONAL_SEM_TAT );	
}

void ProblemData::leAtendimentoTatico(TriedaInput & raiz)
{   
   // Se a tag existir ( mesmo que esteja em branco ) no xml de entrada
   if ( raiz.atendimentosTatico().present() )
   {
      this->atendimentosTatico = new GGroup< AtendimentoCampusSolucao *, LessPtr< AtendimentoCampusSolucao > > ();

      for ( unsigned int i = 0;
		    i < raiz.atendimentosTatico().get().AtendimentoCampus().size(); i++ )
      {
         ItemAtendimentoCampusSolucao it_atendimento
            = ( raiz.atendimentosTatico().get().AtendimentoCampus().at( i ) );

         AtendimentoCampusSolucao * item = new AtendimentoCampusSolucao();
         item->le_arvore( (it_atendimento) );
         this->atendimentosTatico->add( item );
      }
   }
}

void ProblemData::leDemandas(TriedaInput &raiz)
{
   if ( this->parametros->otimizarPor == "ALUNO" )
   {
	   LE_SEQ( this->alunosDemandaTotal, raiz.alunosDemanda(), AlunoDemanda );
	   LE_SEQ( this->alunos, raiz.alunos(), Aluno );

	   // Inicializa com zero o numero de prioridades de cada campus
	   ITERA_GGROUP_LESSPTR( it_campi, this->campi, Campus )
	   {
		   this->nPrioridadesDemanda[ it_campi->getId() ] = 0;   
	   }

	   // Filtrando alunosDemanda: referência somente para os alunosDemanda que tiverem prioridade 1
	   ITERA_GGROUP_LESSPTR( itAlDem, this->alunosDemandaTotal, AlunoDemanda )
	   {
		   int alDemId = itAlDem->getId();
		   int demId = itAlDem->getDemandaId();
		   int campusId = retornaCampusId( demId );
		   int p = itAlDem->getPrioridade();

		   if ( p > 1 )
		   {
			   if ( this->parametros->modo_otimizacao == "TATICO" && this->atendimentosTatico != NULL )
			   {
				   /* Verifica se o AlunoDemanda está fixado em solução tática inicial. Se estiver,
					  modifica sua prioridade para 1. */
				   if ( this->temAtendTatInicialFixado(alDemId) )
				   {
					   itAlDem->setPrioridade(1);
					   p = 1;
				   }
			   }
		   }
   
		   if ( p == 1 )
		   {
			   this->alunosDemanda.add( *itAlDem );	
		   }

		   // Contabiliza quantos niveis de prioridade existem
		   if ( p > this->nPrioridadesDemanda[ campusId ] )
		   {
			   this->nPrioridadesDemanda[ campusId ] = p;
		   }
	   }
	   
	   if ( !this->parametros->min_alunos_abertura_turmas )
	   {
		   // Removendo alunosDemanda replicados, caso existam

		   GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > > remover;
		   ITERA_GGROUP_LESSPTR( itAlDemTotal, this->alunosDemandaTotal, AlunoDemanda )
		   {
			   int demId = itAlDemTotal->getDemandaId();
			   int alunoId = itAlDemTotal->getAlunoId();
			   int p = itAlDemTotal->getPrioridade();

			   if ( p > 1 )
			   {
				   ITERA_GGROUP_LESSPTR( itAlDem, this->alunosDemanda, AlunoDemanda ) // Só p1
				   {
					   if ( (*itAlDem)->getDemandaId() == demId && (*itAlDem)->getAlunoId() == alunoId ) 
						  remover.add( *itAlDemTotal );
				   }
			   }
		   }
		   ITERA_GGROUP_LESSPTR( itAlDem, remover, AlunoDemanda )
		   {
			   this->alunosDemandaTotal.remove( *itAlDem );
			   std::cout<<"\nAlunoDemanda replicado removido definitivamente: id " << (*itAlDem)->getId();
		   }
		   ITERA_GGROUP_LESSPTR( itAlDemTotal, this->alunosDemandaTotal, AlunoDemanda )
		   {
			   if ( remover.find(*itAlDemTotal) != remover.end() ) std::cout<<"\nERRO, alunoDemanda nao removido!!!\n";
		   }
		   std::cout<<endl;
	   }

	   // Filtrando demanda: referência somente para as que possuirem alunoDemanda associado
	   // e ajustando a quantidade associada à demanda.
	   ITERA_GGROUP_LESSPTR( itDem, this->demandasTotal, Demanda )
	   {
		   int id = itDem->getId();
		   int n = 0;

		   ITERA_GGROUP_LESSPTR( itAlDem, this->alunosDemanda, AlunoDemanda )
		   {
			   if ( itAlDem->getDemandaId() == id )
			   {
					n++;
			   }
		   }
		   if ( n )
		   {
			   itDem->setQuantidade( n );
			   this->demandas.add( *itDem );
		   }
	   }
   }
   else // BLOCOCURRICULAR
   {
	   ITERA_GGROUP_LESSPTR( itAlDem, this->alunosDemandaTotal, AlunoDemanda )
	   {
		   this->alunosDemanda.add( *itAlDem );	
	   }
	   ITERA_GGROUP_LESSPTR( itDem, this->demandasTotal, Demanda )
	   {
		   this->demandas.add( *itDem );
	   }
	   ITERA_GGROUP_LESSPTR( it_campi, this->campi, Campus )
	   {
		   this->nPrioridadesDemanda[ it_campi->getId() ] = 1;
	   }
   }
}

void ProblemData::criaPerfilProfVirtual()
{

   // Primeiro caso : executar o
   // solver apenas com a entrada do táticob
   bool primeiroCaso = ( this->getModoOtimizacao() == ProblemData::TATICO );

   // Segundo caso  : executar o solver com
   // a saída do tático e a entrada do operacional
   bool segundoCaso = ( this->getModoOtimizacao() == ProblemData::OPERACIONAL_COM_TAT );
   
   // Terceiro caso : executar o solver apenas
   // com a entrada do operacional (sem saída do tático)
   bool terceiroCaso = ( this->getModoOtimizacao() == ProblemData::OPERACIONAL_SEM_TAT );

   // Informa o modo de otimização que será execuado
   if ( primeiroCaso )
   {
	   std::cout << "TATICO" << std::endl << std::endl;
   }
   else if ( segundoCaso )
   {
	   std::cout << "OPERACIONAL COM OUTPUT TATICO"
                << std::endl << std::endl;
   }
   else if ( terceiroCaso )
   {
	   std::cout << "OPERACIONAL SEM OUTPUT TATICO"
                << std::endl << std::endl;
   }
	else
	{
		// ERRO no XML de entrada
		std::cout << "ERROR!!! input inválido para os campos:"
				    << "\n'horariosDisponiveis' e/ou 'creditosDisponiveis'"
				    << "\n\nSaindo." << std::endl << std::endl;

		exit( 1 );
	}

    // cria professores virtuais com diferentes titulações
    if ( segundoCaso || terceiroCaso )
	{	
		// Titulações mínimas
		bool haCursoComMinMestre = false;
		bool haCursoComMinDout = false;
		ITERA_GGROUP_LESSPTR( itCurso, this->cursos, Curso )
		{
			if ( itCurso->regra_min_mestres.second > 0 )
				haCursoComMinMestre = true;
			if ( itCurso->regra_min_doutores.second > 0 )
				haCursoComMinDout = true;
			if ( haCursoComMinMestre && haCursoComMinDout ) break;
		}

		TipoTitulacao *titMin = retornaTipoTitulacaoMinimo();
		if (titMin==NULL)
			std::cout<<"\nErro: nenhum TipoTitulacao encontrado.\n";

		GGroup<int> titulMinId;
		titulMinId.add( titMin->getTitulacao() );	
		if ( this->parametros->min_mestres && haCursoComMinMestre )
			titulMinId.add( TipoTitulacao::Mestre );
		if ( this->parametros->min_doutores && haCursoComMinDout )
			titulMinId.add( TipoTitulacao::Doutor );


		// Tipos de contrato mínimos
		bool haCursoComMinInt = false;
		bool haCursoComMinParc = false;
		ITERA_GGROUP_LESSPTR( itCurso, this->cursos, Curso )
		{
			if ( itCurso->getMinTempoIntegral() > 0 )
				haCursoComMinInt = true;
			if ( itCurso->getMinTempoIntegralParcial() > 0 )
				haCursoComMinParc = true;
			if ( haCursoComMinInt && haCursoComMinParc ) break;
		}

		
		TipoContrato *contrMin = retornaTipoContratoMinimo();
		if (contrMin==NULL)
			std::cout<<"\nErro: nenhum TipoContrato encontrado.\n";

		GGroup<int> contratosId;
		contratosId.add( contrMin->getContrato() );
		if ( this->parametros->minContratoIntegral && haCursoComMinInt )
			contratosId.add( TipoContrato::Integral );
		if ( this->parametros->minContratoParcial && haCursoComMinParc )
			contratosId.add( TipoContrato::Parcial );

		// Cria professores virtuais com titulações e contratos mínimos
		int i=1;
		ITERA_GGROUP_LESSPTR( itTitul, tipos_titulacao, TipoTitulacao )
		{
			TipoTitulacao *titulacao = (*itTitul);

			if ( titulMinId.find( titulacao->getTitulacao() ) != titulMinId.end() )
			{
				ITERA_GGROUP_LESSPTR( itContr, tipos_contrato, TipoContrato )
				{
					TipoContrato *contrato = (*itContr);

					if ( contratosId.find( contrato->getContrato() ) != contratosId.end() )
					{
						Professor *pv = new Professor(true);
						pv->setTitulacaoId( titulacao->getId() );
						pv->titulacao = titulacao;
						pv->setTipoContratoId( contrato->getId() );
						pv->tipo_contrato = contrato;
						pv->setId(-i);
						std::string nome = pv->getNome();
						stringstream ss; ss << -i;
						nome += ss.str();
						pv->setNome( nome );

						this->profsVirtuais.add( pv );
						i++;
					}
				}
			}
		}
	}

}

void ProblemData::refTitContProfReal()
{
   // Referencia titulação e contrato dos professores reais
   ITERA_GGROUP_LESSPTR( it_campi, this->campi, Campus )
   {
	   ITERA_GGROUP_LESSPTR( it_prof, it_campi->professores, Professor )
      {
		  int titid = it_prof->getTitulacaoId();
		  TipoTitulacao *tipotit = this->retornaTitulacaoPorId( titid );
		  it_prof->titulacao = tipotit;

		  int contrid = it_prof->getTipoContratoId();
		  TipoContrato *tipocontr = this->retornaTipoContratoPorId( contrid );
		  it_prof->tipo_contrato = tipocontr;
      }
   }

}

void ProblemData::preencheHorSalas(TriedaInput &raiz)
{
   // Monta um 'map' para recuperar cada 'ItemSala'
   // do campus a partir de seu respectivo id de sala
   std::map< int, ItemSala * > mapItemSala;

   ITERA_SEQ( it_campi, raiz.campi(), Campus )
   {
      ITERA_SEQ( it_unidade, it_campi->unidades(), Unidade )
      {
         ITERA_SEQ( it_sala, it_unidade->salas(), Sala )
         {
            mapItemSala[ it_sala->id() ] = &( ( *it_sala ) );
         }
      }
   }

   // Prencher os horários e/ou créditos das salas
   ITERA_GGROUP_LESSPTR( it_campi, this->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_unidade, it_campi->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( it_sala, it_unidade->salas, Sala )
         {
			 std::map< int, ItemSala * >::iterator it
				 = mapItemSala.find( it_sala->getId() );

            if ( it != mapItemSala.end() )
            {
               // Objeto de entrada do XML
               ItemSala * elem = it->second;

               it_sala->construirCreditosHorarios(
					( *elem ), this->parametros->modo_otimizacao,
					raiz.atendimentosTatico().present() );
            }
         }
      }
   }

}

void ProblemData::setFaseDoDiaDosHorariosAula()
{
	std::map< int, int > mapHorAulaIdFase;

	ITERA_GGROUP_LESSPTR( itCalend, this->calendarios, Calendario )
	{
		ITERA_GGROUP_LESSPTR( itHorAula, itCalend->horarios_aula, HorarioAula )
		{
			DateTime dt = itHorAula->getInicio();
			int fase = this->getFaseDoDia( dt );
			itHorAula->setFaseDoDia( fase );

			mapHorAulaIdFase[ itHorAula->getId() ] = fase;
		}
	}
	//ITERA_GGROUP_LESSPTR( itDisc, this->disciplinas, Disciplina )
	//{
	//	ITERA_GGROUP_LESSPTR( itHor, itDisc->horarios, Horario )
	//	{
	//		int fase = mapHorAulaIdFase[ itHor->getHorarioAulaId() ];
	//		itHor->horario_aula->setFaseDoDia( fase );
	//	}
	//}
	//ITERA_GGROUP_LESSPTR( itCampi, this->campi, Campus )
	//{		
	//	for( GGroup< Horario * >::iterator itHor = itCampi->horarios.begin();
	//		itHor != itCampi->horarios.end() ; itHor++ )
	//	{
	//		int fase = mapHorAulaIdFase[ itHor->getHorarioAulaId() ];
	//		itHor->horario_aula->setFaseDoDia( fase );
	//	}

	//	ITERA_GGROUP_LESSPTR( itUnid, itCampi->unidades, Unidade )
	//	{			
	//		for( GGroup< Horario * >::iterator itHor = itUnid->horarios.begin(); 
	//			itHor != itUnid->horarios.end() ; itHor++ )
	//		{
	//			int fase = mapHorAulaIdFase[ itHor->getHorarioAulaId() ];
	//			itHor->horario_aula->setFaseDoDia( fase );
	//		}

	//		ITERA_GGROUP_LESSPTR( itSala, itUnid->salas, Sala )
	//		{				
	//			for( GGroup< Horario * >::iterator itHor = itSala->horarios_disponiveis.begin();
	//				itHor != itUnid->horarios.end() ; itHor++ )
	//			{
	//				int fase = mapHorAulaIdFase[ itHor->getHorarioAulaId() ];	
	//				itHor->horario_aula->setFaseDoDia( fase );
	//			}
	//		}
	//	}

	//	ITERA_GGROUP_LESSPTR( itProf, itCampi->professores, Professor )
	//	{
	//		for( GGroup< Horario * >::iterator itHor = itProf->horarios.begin();
	//				itHor != itProf->horarios.end() ; itHor++ )
	//		{
	//			int fase = mapHorAulaIdFase[ itHor->getHorarioAulaId() ];
	//			itHor->horario_aula->setFaseDoDia( fase );
	//		}
	//	}
	//}

}

void ProblemData::addHorariosAnalogosDisciplina()
{
	std::map< int, Calendario* > mapHorAulaIdCalend;
	std::map< std::pair< DateTime, DateTime >, GGroup<HorarioAula*,LessPtr<HorarioAula>> > mapCalendDtiDtf;	
	std::map< int, std::pair< DateTime, DateTime > > mapCalendHorAulaDt;

	ITERA_GGROUP_LESSPTR( itCalend, this->calendarios, Calendario )
	{
		double tempoAula = itCalend->getTempoAula();

		ITERA_GGROUP_LESSPTR( itHorAula, itCalend->horarios_aula, HorarioAula )
		{
			DateTime dti = itHorAula->getInicio();
			DateTime dtf = itHorAula->getInicio();
			dtf.addMinutes( tempoAula );
			mapCalendDtiDtf[ std::make_pair( dti, dtf ) ].add( *itHorAula );
			mapCalendHorAulaDt[ itHorAula->getId() ] = std::make_pair( dti, dtf );
			mapHorAulaIdCalend[ itHorAula->getId() ] = *itCalend;
		}
	}

	ITERA_GGROUP_LESSPTR( itDisc, this->disciplinas, Disciplina )
	{		
		std::map< std::pair< DateTime, DateTime >, GGroup<Horario*,LessPtr<Horario>> > mapDiscDtiDtf;	

		ITERA_GGROUP_LESSPTR( itHor, itDisc->horarios, Horario )
		{
			std::pair< DateTime, DateTime > dit_dtf = mapCalendHorAulaDt[ itHor->getHorarioAulaId() ];
			mapDiscDtiDtf[ dit_dtf ].add( *itHor );
			
			itDisc->addCalendariosOriginais( mapHorAulaIdCalend[ itHor->getHorarioAulaId() ] );		 
		}

		std::map< std::pair< DateTime, DateTime >, GGroup<Horario*,LessPtr<Horario>> >::iterator
			itMapDiscDtiDtf = mapDiscDtiDtf.begin();
		for ( ; itMapDiscDtiDtf != mapDiscDtiDtf.end(); itMapDiscDtiDtf++ )
		{
			std::pair< DateTime, DateTime > dti_dtf = itMapDiscDtiDtf->first;
			GGroup<Horario*,LessPtr<Horario>> *ggroupDiscHor = & itMapDiscDtiDtf->second;

			Horario *h_disc = * (*ggroupDiscHor).begin();

			GGroup<HorarioAula*,LessPtr<HorarioAula>> *ggroupCalendHorAulas = & mapCalendDtiDtf[ dti_dtf ];

			// Para cada horario-aula analogo existente
			ITERA_GGROUP_LESSPTR( itCalendHorAula, (*ggroupCalendHorAulas), HorarioAula )
			{
				int idNovo = (*itCalendHorAula)->getId();

				// Se nao pertencer à disciplina, adiciona-o
				bool found=false;
				ITERA_GGROUP_LESSPTR( itDiscHor, (*ggroupDiscHor), Horario )
				{
					if ( itDiscHor->getHorarioAulaId() == idNovo )
						found = true;
				}
				if ( !found )
				{
					// Cria Horario analogo à (*itCalendHorAula) e adiciona na disciplina
					// Copia dias de qq horario em ggroupDiscHorAulas, mas seta id de (*itCalendHorAula)

					Horario * horario = new Horario();
					ITERA_GGROUP_N_PT( it_dia, h_disc->dias_semana, int )
					{
						if ( itCalendHorAula->dias_semana.find( *it_dia ) !=
							 itCalendHorAula->dias_semana.end() )
							horario->dias_semana.add( *it_dia );
					}
					horario->setHorarioAulaId( idNovo );
					horario->setTurnoIESId( itCalendHorAula->getTurnoIESId() );

					itDisc->horarios.add( horario );
					itDisc->addTurnoIES( horario->getTurnoIESId(), horario );
				}
			}
		}
	}
}

/*
	Preenche o atributo ´cursosComp_disc´ com todos os pares de cursos
	compativeis (c1,c2) e suas disciplinas em comum.
*/
void ProblemData::preencheCursosCompDisc()
{
	ITERA_GGROUP_LESSPTR( it_disc, this->disciplinas, Disciplina )
    {
		Disciplina *disciplina = *it_disc;

		GGroup< Curso *, LessPtr< Curso > > cursosRelacionados;

		ITERA_GGROUP_LESSPTR( it_oferta, this->ofertas, Oferta )
		{		
			if ( it_oferta->curriculo->getPeriodo( disciplina ) >= 0 )
			{
				cursosRelacionados.add( it_oferta->curso );
			}
		}

		if ( cursosRelacionados.size() > 1 )
		{
			// relaciona os cursos dois a dois (sem simetria)
			ITERA_GGROUP_LESSPTR( it1_curso, cursosRelacionados, Curso )
			{
				GGroup< Curso *, LessPtr<Curso> >::iterator it2_curso = it1_curso;
				it2_curso++;
				for ( ; it2_curso != cursosRelacionados.end(); it2_curso++ )
				{
					if ( cursosCompativeis( *it1_curso, *it2_curso ) )
						insereDisciplinaEmCursosComp( std::make_pair(*it1_curso, *it2_curso), disciplina->getId() );
						
				}
			}
		}

		cursosRelacionados.clear();
	}
}

/*
	Dado um par de cursos compativeis pc (independente da ordem),
	insere idDisc no vetor de ids de disciplinas em comum dos dois cursos.
	Se nem o par pc nem o seu inverso existir, insere-o.
*/
void ProblemData::insereDisciplinaEmCursosComp( std::pair<Curso*, Curso*> pc, int idDisc )
{
	std::map< std::pair<Curso*, Curso*>, std::vector<int> >::iterator it_parCurso_disc = this->cursosComp_disc.find( pc );

    if ( it_parCurso_disc != cursosComp_disc.end() )
    {
		it_parCurso_disc->second.push_back( idDisc );
	}
    else
	{
		std::pair<Curso*, Curso*> pc_equiv = std::make_pair(pc.second, pc.first);

		it_parCurso_disc = this->cursosComp_disc.find( pc_equiv );
		if ( it_parCurso_disc != cursosComp_disc.end() )
		{
			it_parCurso_disc->second.push_back( idDisc );
		}
		else
		{
			std::vector<int> novaDisc(1,idDisc);
			cursosComp_disc[pc] = novaDisc;
		}
	}
}


bool ProblemData::cursosCompativeis( Curso * curso1, Curso * curso2 )
{
	std::map< std::pair< Curso *, Curso * >, bool >::iterator
		it_map = this->compat_cursos.find(make_pair(curso1, curso2));

	if(it_map != this->compat_cursos.end() && it_map->second) 
		return true;

	it_map = this->compat_cursos.find(make_pair(curso2, curso1));

	if(it_map != this->compat_cursos.end() && it_map->second) 
		return true;

	return false;
}

// Dado o id de uma disciplina, retorna a referencia para a disciplina.
// Se nao existir a disciplina de id procurado, retorna null.
Disciplina* ProblemData::retornaDisciplina( int id )
{
	Disciplina * d = new Disciplina();
	d->setId(id);

	GGroup< Disciplina *, LessPtr< Disciplina > >::iterator it = this->disciplinas.find(d);
	delete d;

	if(it != this->disciplinas.end())
		return *it;
	else
		return NULL;
}


/*
	Preenche o atributo ´oftsComp_disc´ com todos os pares de ofertas
	compativeis (oft1,oft2) e suas disciplinas em comum.
*/
void ProblemData::preencheOftsCompDisc()
{
	ITERA_GGROUP_LESSPTR( it_disc, this->disciplinas, Disciplina )
    {
		Disciplina *disciplina = *it_disc;

		GGroup< Oferta *, LessPtr< Oferta > > oftsRelacionadas;

		ITERA_GGROUP_LESSPTR( it_oferta, this->ofertas, Oferta )
		{		
			if ( it_oferta->curriculo->getPeriodo( disciplina ) >= 0 )
			{
				oftsRelacionadas.add( *it_oferta);
			}
		}

		if ( oftsRelacionadas.size() > 1 )
		{
			// relaciona as ofertas 2 a 2 (sem simetria)
			ITERA_GGROUP_LESSPTR( it1_Oft, oftsRelacionadas, Oferta )
			{
				ITERA_GGROUP_INIC_LESSPTR( it2_Oft, it1_Oft, oftsRelacionadas, Oferta )
				{
					if ( it2_Oft->getId() == it1_Oft->getId() ) continue;

					if ( it2_Oft->campus->getId() != it1_Oft->campus->getId() ) continue;

					if ( cursosCompativeis( it1_Oft->curso, it2_Oft->curso ) )
						insereDisciplinaEmOftsComp( std::make_pair(*it1_Oft, *it2_Oft), disciplina->getId() );
						
				}
			}
		}
		oftsRelacionadas.clear();
	}
}
   
/*
	Dado um par de ofertas compativeis po (independente da ordem),
	insere idDisc no vetor de ids de disciplinas em comum das duas ofertas.
	Se nem o par po nem o seu inverso existir, insere-o.
*/
void ProblemData::insereDisciplinaEmOftsComp( std::pair<Oferta*, Oferta*> po, int idDisc )
{
	std::map< std::pair< Oferta*, Oferta* >, std::vector<int> >::iterator it_parOft_disc = this->oftsComp_disc.find( po );

    if ( it_parOft_disc != oftsComp_disc.end() )
    {
		it_parOft_disc->second.push_back( idDisc );
	}
    else
	{
		std::pair<Oferta*, Oferta*> po_equiv = std::make_pair(po.second, po.first);

		it_parOft_disc = this->oftsComp_disc.find( po_equiv );
		if ( it_parOft_disc != oftsComp_disc.end() )
		{
			it_parOft_disc->second.push_back( idDisc );
		}
		else
		{
			std::vector<int> novaDisc(1,idDisc);
			oftsComp_disc[po] = novaDisc;
		}
	}
}


// Retorna todos os pares curso/curriculo
// onde a disciplina informada está incluída
GGroup< std::pair< Curso *, Curriculo * > >
	ProblemData::retornaCursosCurriculosDisciplina( Disciplina * disciplina_equivalente )
{
	GGroup< std::pair< Curso *, Curriculo * > > cursos_curriculos;

	ITERA_GGROUP_LESSPTR( it_curso, this->cursos, Curso )
	{
		ITERA_GGROUP_LESSPTR( it_curriculo, it_curso->curriculos, Curriculo )
		{
			if((*it_curriculo)->possuiDisciplina(disciplina_equivalente))
			{
				cursos_curriculos.add( std::make_pair( ( *it_curso ), ( *it_curriculo ) ) );
			}
		}
	}

   return cursos_curriculos;
}

GGroup< Demanda*, LessPtr< Demanda > > ProblemData::buscaTodasDemandas( Curso *c , Disciplina *d, Campus *cp )
{
   GGroup< Demanda*, LessPtr< Demanda > > demandas;

   ITERA_GGROUP_LESSPTR( it_demanda, this->demandas, Demanda )
   {
      if ( it_demanda->disciplina->getId() == d->getId() &&
		   it_demanda->oferta->getCursoId() == c->getId() &&
		   it_demanda->oferta->getCampusId() == cp->getId() )
      {
		  demandas.add( *it_demanda );
      }
   }

	return demandas;
}

Disciplina * ProblemData::retornaDisciplinaSubstituta(
  Curso * curso, Curriculo * curriculo, Disciplina * disciplina )
{
   std::map< std::pair< Curso *, Curriculo * >,
      std::map< Disciplina *, GGroup< Disciplina *, LessPtr< Disciplina > >, LessPtr< Disciplina > > >::iterator
      it_map = this->mapGroupDisciplinasSubstituidas.find(make_pair(curso, curriculo));

   if( it_map != this->mapGroupDisciplinasSubstituidas.end())
   {
      std::map< Disciplina *, GGroup< Disciplina *, LessPtr< Disciplina > >, LessPtr< Disciplina > >::iterator
         it_disciplinas = it_map->second.begin();

      for (; it_disciplinas != it_map->second.end(); it_disciplinas++ )
      {
         Disciplina * disciplina_substituta = it_disciplinas->first;
		 GGroup< Disciplina *, LessPtr< Disciplina > >::iterator itD = it_disciplinas->second.find(disciplina);
		 if(itD != it_disciplinas->second.end())
			 return disciplina_substituta;
         /*ITERA_GGROUP_LESSPTR( it_disc_equi, it_disciplinas->second, Disciplina )
         {
            Disciplina * disciplina_equivalente = ( *it_disc_equi );

            if ( disciplina_equivalente->getId() == disciplina->getId() )
            {
               return disciplina_substituta;
            }
         }*/
      }      
   }

	/*std::map< std::pair< Curso *, Curriculo * >,
		std::map< Disciplina *, Disciplina * > >::iterator
		it_map = this->map_CursoCurriculo_DiscSubst.find(make_pair(curso, curriculo));

	if( it_map != this->map_CursoCurriculo_DiscSubst.end())
	{
		std::map< Disciplina *, Disciplina * >::iterator it_map2 = it_map->second.find(disciplina);
		if(it_map2 != it_map->second.end())
			return it_map2->second;
		else
			return NULL;
	}*/

	return NULL;
}

int ProblemData::creditosFixadosDisciplinaDia(
	Disciplina * disciplina, int dia_semana, ConjuntoSala * conjunto_sala )
{
	int creditos_fixados = 0;
	if ( disciplina != NULL && dia_semana >= 0 )
	{
		// Inicialmente, consideramos o número de créditos fixados entre
		// o par 'disciplina/dia da semana', sem levar em consideração a sala
		std::pair< Disciplina *, int > disciplina_dia
			= std::make_pair( disciplina, dia_semana );

		creditos_fixados = this->map_Discicplina_DiaSemana_CreditosFixados[ disciplina_dia ];

		//-------------------------------------------------------------------------------------
		ITERA_GGROUP_LESSPTR( it_fixacao, this->fixacoes, Fixacao )
		{
			// Procura pela fixação de 'disciplina/sala/dia da semana' atual
			if ( it_fixacao->disciplina != NULL
				&& it_fixacao->disciplina->getId() == disciplina->getId()
				&& it_fixacao->getDiaSemana() == dia_semana
				&& it_fixacao->sala != NULL )
			{
				// Caso exista uma fixaçao do par 'disciplina/dia da semana' informados
				// com alguma sala, devo verificar se essa sala está no conjunto de salas
				// informados. Caso não esteja, o total de créditos fixados nesse caso é zerado
				if ( conjunto_sala->salas.find( it_fixacao->sala->getId() )
						== conjunto_sala->salas.end() )
				{
					creditos_fixados = 0;
				}
			}
		}
		//-------------------------------------------------------------------------------------
	}

	return creditos_fixados;
}

GGroup< Curso*, LessPtr<Curso> > ProblemData::cursosAtendidosNaTurma( int turma, Disciplina *disciplina, Campus *campus )
{
	GGroup< Curso*, LessPtr<Curso> > cursosAtendidos;

   ITERA_GGROUP_LESSPTR( it_aula, this->aulas, Aula )
   {
	   Aula *aula = *it_aula;

	   if ( aula->getTurma() == turma &&
		    aula->getDisciplina() == disciplina &&
			aula->getUnidade()->getIdCampus() == campus->getId() )
	   {
			ITERA_GGROUP_LESSPTR( it_oferta, it_aula->ofertas, Oferta )
			{
				cursosAtendidos.add( it_oferta->curso );
			}
			break;
	   }
   }

   return cursosAtendidos;
}

GGroup< Oferta*, LessPtr<Oferta> > ProblemData::ofertasAtendidasNaTurma( int turma, Disciplina *disciplina, Campus *campus )
{
	GGroup< Oferta*, LessPtr<Oferta> > ofertasAtendidas;

   ITERA_GGROUP_LESSPTR( it_aula, this->aulas, Aula )
   {
	   Aula *aula = *it_aula;

	   if ( aula->getTurma() == turma &&
		    aula->getDisciplina() == disciplina &&
			aula->getUnidade()->getIdCampus() == campus->getId() )
	   {
			ITERA_GGROUP_LESSPTR( it_oferta, it_aula->ofertas, Oferta )
			{
				ofertasAtendidas.add( *it_oferta );
			}
			break;
	   }
   }

   return ofertasAtendidas;
}

bool ProblemData::aulaAtendeCurso( int turma, Disciplina *disciplina, Campus *campus, Curso * curso )
{
   ITERA_GGROUP_LESSPTR( it_aula, this->aulas, Aula )
   {
	   Aula *aula = *it_aula;

	   if ( aula->getTurma() == turma &&
		    aula->getDisciplina() == disciplina &&
			aula->getUnidade()->getIdCampus() == campus->getId() )
	   {
			ITERA_GGROUP_LESSPTR( it_oferta, it_aula->ofertas, Oferta )
			{
				if ( it_oferta->curso->getId() == curso->getId() )
				{
					return true;
				}
			}
			return false;
	   }
   }

   return false;
}


int ProblemData::getNroDiscsAtendidasNoCurso( Curso *curso )
{
	GGroup<Disciplina*> discsAtendNoCurso;
	
   ITERA_GGROUP_LESSPTR( it_aula, this->aulas, Aula )
   {
	    Aula *aula = *it_aula;
		
		if ( aula->atendeAoCurso( curso->getId() ) )
		{
			int id = aula->getDisciplina()->getId();
			Disciplina *d = this->refDisciplinas[ abs(id) ];
			
			if ( discsAtendNoCurso.find( d ) == discsAtendNoCurso.end() )
			{
				discsAtendNoCurso.add( d );			// só entram as teóricas, evitando duplicações
			}
		}	   
   }

   int n = (int) discsAtendNoCurso.size();

   return n;
}


int ProblemData::calculaTempoEntreCampusUnidades(
   Campus * campus_atual, Campus * campus_anterior,
   Unidade * unidade_atual, Unidade * unidade_anterior )
{
   int distancia = 0;

   // As aulas são realizadas em campus diferentes
   if ( campus_atual->getId() != campus_anterior->getId() )
   {
      GGroup< Deslocamento *, LessPtr< Deslocamento > >::iterator it_tempo_campi
         = this->tempo_campi.begin();

      for (; it_tempo_campi != this->tempo_campi.end();
			    it_tempo_campi++ )
      {
         if ( it_tempo_campi->getOrigemId() == campus_anterior->getId()
				&& it_tempo_campi->getDestinoId() == campus_atual->getId() )
         {
            distancia = it_tempo_campi->getTempo();
         }
      }
   }
   // As aulas são realizadas em unidades diferentes
   else if ( unidade_atual->getId() != unidade_anterior->getId() )
   {
      GGroup< Deslocamento *, LessPtr< Deslocamento > >::iterator it_tempo_unidade
         = this->tempo_unidades.begin();

      for (; it_tempo_unidade != this->tempo_unidades.end();
			 it_tempo_unidade++)
      {
         if ( it_tempo_unidade->getOrigemId() == unidade_anterior->getId()
				&& it_tempo_unidade->getDestinoId() == unidade_atual->getId() )
         {
            distancia = it_tempo_unidade->getTempo();
         }
      }
   }

   return distancia;
}

int ProblemData::minutosIntervalo( DateTime dt1, DateTime dt2 )
{
   DateTime back = ( dt1 - dt2 );
   int minutes = ( back.getHour() * 60 + back.getMinute() );
   return minutes;
}


bool ProblemData::verificaUltimaPrimeiraAulas(
   HorarioDia * h1, HorarioDia * h2 )
{
   if ( h1 == NULL || h2 == NULL )
   {
      return false;
   }

   if ( h1->getHorarioAula() == NULL
      || h2->getHorarioAula() == NULL )
   {
      return false;
   }

   if ( h1->getHorarioAula()->getCalendario() == NULL
      || h2->getHorarioAula()->getCalendario() == NULL )
   {
      return false;
   }

   if ( h1->getDia() <= 0
      || h2->getDia() <= 0 )
   {
      return false;
   }

   if ( abs( h1->getDia() - h2->getDia() ) == 1
      && h1->getHorarioAula()->getCalendario() == h2->getHorarioAula()->getCalendario() )
   {
      Calendario * calendario = h1->getHorarioAula()->getCalendario();

      if ( this->mapCalendarioHorariosAulaOrdenados.find( calendario )
         != this->mapCalendarioHorariosAulaOrdenados.end()
         && this->mapCalendarioHorariosAulaOrdenados[ calendario ].size() > 0 )
      {
         HorarioAula * primeiroHorario = this->mapCalendarioHorariosAulaOrdenados[ calendario ][ 0 ];

         HorarioAula * ultimoHorario = this->mapCalendarioHorariosAulaOrdenados
            [ calendario ][ this->mapCalendarioHorariosAulaOrdenados[ calendario ].size() - 1 ];

         if ( primeiroHorario != NULL && ultimoHorario != NULL )
         {
            if ( ( h1->getHorarioAula() == primeiroHorario && h2->getHorarioAula() == ultimoHorario )
               || ( h2->getHorarioAula() == primeiroHorario && h1->getHorarioAula() == ultimoHorario ) )
            {
               return true;
            }
         }
      }
   }

   return false;
}

bool ProblemData::verificaPrimeiraAulas(
	HorarioDia * h )
{
	if ( h == NULL )
	{
		return false;
	}

	if ( h->getHorarioAula() == NULL )
	{
		return false;
	}

	if ( h->getHorarioAula()->getCalendario() == NULL )
	{
		return false;
	}

	if ( h->getDia() <= 0 )
	{
		return false;
	}

	Calendario * calendario = h->getHorarioAula()->getCalendario();

	if ( this->mapCalendarioHorariosAulaOrdenados.find( calendario )
		!= this->mapCalendarioHorariosAulaOrdenados.end()
		&& this->mapCalendarioHorariosAulaOrdenados[ calendario ].size() > 0 )
	{
		HorarioAula * primeiroHorario = this->mapCalendarioHorariosAulaOrdenados[ calendario ][ 0 ];

		if ( primeiroHorario != NULL )
		{
			if ( h->getHorarioAula() == primeiroHorario )
			{
				return true;
			}
		}
	}

	return false;
}

bool ProblemData::verificaUltimaAulas(
	HorarioDia * h )
{
	if ( h == NULL )
	{
		return false;
	}

	if ( h->getHorarioAula() == NULL )
	{
		return false;
	}

	if ( h->getHorarioAula()->getCalendario() == NULL )
	{
		return false;
	}

	if ( h->getDia() <= 0 )
	{
		return false;
	}

	Calendario * calendario = h->getHorarioAula()->getCalendario();

	if ( this->mapCalendarioHorariosAulaOrdenados.find( calendario )
		!= this->mapCalendarioHorariosAulaOrdenados.end()
		&& this->mapCalendarioHorariosAulaOrdenados[ calendario ].size() > 0 )
	{

		HorarioAula * ultimoHorario = this->mapCalendarioHorariosAulaOrdenados
			[ calendario ][ this->mapCalendarioHorariosAulaOrdenados[ calendario ].size() - 1 ];

		if ( ultimoHorario != NULL )
		{
			if ( h->getHorarioAula() == ultimoHorario ) 
			{
				return true;
			}
		}

	}

	return false;
}


Demanda * ProblemData::buscaDemanda( int id_oferta, int id_disciplina )
{
   Demanda * demanda = NULL;

   ITERA_GGROUP_LESSPTR( it_demanda,
     this->demandas, Demanda )
   {
      if ( it_demanda->getOfertaId() == id_oferta
         && it_demanda->getDisciplinaId() == id_disciplina )
      {
		 demanda = ( *it_demanda );
         break;
      }
   }

   return demanda;
}

GGroup< Professor *, LessPtr< Professor > > ProblemData::getProfessores() const
{
   // Armazenando todos os professores
   GGroup< Professor *, LessPtr< Professor > > professores;

   ITERA_GGROUP_LESSPTR( it_campi, this->campi, Campus )
   {
      Campus * campus = ( *it_campi );

      ITERA_GGROUP_LESSPTR( it_prof, it_campi->professores, Professor )
      {
         Professor * professor = ( *it_prof );

         professores.add( professor );
      }
   }

   return professores;
}

Professor * ProblemData::findProfessor( int id )
{
	auto itFinderP = this->refProfessores.find( id );							   
	if ( itFinderP != this->refProfessores.end() );
		return itFinderP->second;


   GGroup< Professor *, LessPtr< Professor > > professores = this->getProfessores();

   Professor *prof = new Professor(false);
   prof->setId(id);

   auto itP = professores.find( prof );

   delete prof;

   if ( itP != professores.end() )
	   return *itP;

   return NULL;
}

Professor * ProblemData::findProfessorVirtual( int id )
{
   Professor *profVirtual = new Professor(true);
   profVirtual->setId(id);

   auto itP = profsVirtuais.find( profVirtual );

   delete profVirtual;

   if ( itP != profsVirtuais.end() )
	   return *itP;

   return nullptr;
}

Oferta * ProblemData::findOferta( int id_oferta ) const
{
   Oferta * oferta = NULL;

   try
   {
      oferta = this->refOfertas.find( id_oferta )->second;
   }
   catch( std::exception ex )
   {
      oferta = NULL;
   }

   return oferta;
}

GGroup< Sala *, LessPtr< Sala > > ProblemData::getSalas() const
{
   GGroup< Sala *, LessPtr< Sala > > salas;

   ITERA_GGROUP_LESSPTR( it_campus, this->campi, Campus )
   {
      Campus * campus = ( *it_campus );

      ITERA_GGROUP_LESSPTR( it_unidade, campus->unidades, Unidade )
      {
         Unidade * unidade = ( *it_unidade );

         ITERA_GGROUP_LESSPTR( it_sala, unidade->salas, Sala )
         {
            Sala * sala = ( *it_sala );
            salas.add( sala );
         }
      }
   }

   return salas;
}

Disciplina * ProblemData::ehSubstitutaDe( Disciplina* disciplina, std::pair< Curso *, Curriculo * > parCursoCurr)
{	
	std::map< std::pair< Curso *, Curriculo * >,
			  std::map< Disciplina *,
			  GGroup< Disciplina *, LessPtr< Disciplina > >, LessPtr< Disciplina > > >::iterator
			  it_CursoCurr_Discs;

	it_CursoCurr_Discs = this->mapGroupDisciplinasSubstituidas.find( parCursoCurr );

	if ( it_CursoCurr_Discs == mapGroupDisciplinasSubstituidas.end() )
		return NULL;

	std::map< Disciplina *, GGroup< Disciplina *, LessPtr< Disciplina > >, LessPtr< Disciplina > > mapDiscs = it_CursoCurr_Discs->second;

	std::map< Disciplina *, GGroup< Disciplina *, LessPtr<Disciplina> >, LessPtr< Disciplina > >::iterator 
		it_Discs = mapDiscs.find( disciplina );

	if ( it_Discs == mapDiscs.end() )
		return NULL;

	Disciplina *discOriginal = *it_Discs->second.begin(); // TODO: vai ter sempre 1 unica disciplina no group, não é? então pode tirar o group!!!

	return discOriginal;
}


// Informa se uma dada disciplina é substituta de alguma outra.
bool ProblemData::ehSubstituta( Disciplina* d )
{
	std::map< Disciplina*, Disciplina*, LessPtr< Disciplina > >::iterator
		itMap = this->mapDiscSubstituidaPor.begin();

	for ( ; itMap != this->mapDiscSubstituidaPor.end(); ++itMap )
	{
		if ( itMap->second == d )
		{
			return true;
		}
	}

	return false;
}

GGroup< HorarioAula *, LessPtr< HorarioAula > > ProblemData::retornaHorariosEmComum( int sala, int disc, int dia )
{
	GGroup< HorarioAula *, LessPtr< HorarioAula > > horarios;

	std::pair< int, int > parDiscSala = std::make_pair( disc, sala );

	std::map< std::pair< int /*idDisc*/, int /*idSala*/ >, 
			  std::map< int /*Dia*/, GGroup< HorarioAula *, LessPtr< HorarioAula > > > >::iterator

	it_Disc_Sala_Dias_HorariosAula = this->disc_Salas_Dias_HorariosAula.find( parDiscSala );

	if ( it_Disc_Sala_Dias_HorariosAula == 
		this->disc_Salas_Dias_HorariosAula.end() )
	{
		return horarios; // NULL
	}

	std::map< int /*Dia*/, GGroup< HorarioAula *, LessPtr< HorarioAula > > >::iterator
		it_Dia_HorarioAula = it_Disc_Sala_Dias_HorariosAula->second.find( dia );
					
	if ( it_Dia_HorarioAula == it_Disc_Sala_Dias_HorariosAula->second.end() )
	{
		return horarios; // NULL
	}

	return it_Dia_HorarioAula->second;
}


// Dado o id de uma unidade, retorna referencia para o campus correspondente
Campus* ProblemData::retornaCampus( int unidId /*unidId*/ )
{
   ITERA_GGROUP_LESSPTR( it_campus, this->campi, Campus )
   {
      Campus * campus = ( *it_campus );
	  
	  ITERA_GGROUP_LESSPTR( it_unid, campus->unidades, Unidade )
	  {
		  if ( ( *it_unid )->getId() == unidId )
		  {
			  return campus;
		  }
	  }
   }

   return NULL;
}


int ProblemData::retornaTurmaDiscAluno( Aluno* aluno, Disciplina* disc )
{
	int turma = -1;

	if ( aluno == NULL )
	{
		std::cout << "\nERRO: aluno NULL em ProblemData::retornaTurmaDiscAluno( Aluno* aluno, Disciplina* disc )"; fflush(NULL);
		return turma;
	}
	if ( disc == NULL )
	{
		std::cout << "\nERRO: disciplina NULL em ProblemData::retornaTurmaDiscAluno( Aluno* aluno, Disciplina* disc )"; fflush(NULL);
		return turma;
	}

	bool bd = false;
	if ( aluno->getAlunoId() == 49689 && disc->getId() == -16005 ){
		std::cout << "\nProcurando turma do aluno " << aluno->getAlunoId() << ", disc " << disc->getId(); fflush(NULL);
		bd = true;
	}

	std::map< Aluno*, GGroup< Trio< int, int, Disciplina* > >, LessPtr< Aluno > >::iterator
		itMap = mapAluno_CampusTurmaDisc.find( aluno );
	
	if ( itMap != mapAluno_CampusTurmaDisc.end() )
	{
		if ( bd ){
			std::cout<<"\n\tAluno encontrado\tTurmas:"; fflush(NULL);
		}

		GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > > *atendimentos = & (itMap->second);
	
		GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >::iterator itAtend = (*atendimentos).begin();
		for ( ; itAtend != (*atendimentos).end(); itAtend++ )
		{
			if ( bd ){
				std::cout<<" i" << (*itAtend).second << ",d" <<(*itAtend).third->getId(); fflush(NULL);
			}
		
			if ( (*itAtend).third == disc )	
			{
				turma = (*itAtend).second;
				return turma;
			}
		}
	}
	
	if ( bd ){
		std::cout<<"\nTurma = " << turma; fflush(NULL);
	}


    return turma;
}

// Dado o id de um aluno, retorna referencia para o aluno correspondente
Aluno* ProblemData::retornaAluno( int id )
{
	ITERA_GGROUP_LESSPTR( it_aluno, alunos, Aluno )
	{
		if ( it_aluno->getAlunoId() == id )
			return *it_aluno;
	}
	return NULL;
}

// Dado o id de um aluno, retorna referencia para o aluno correspondente
AlunoDemanda* ProblemData::retornaAlunoDemanda( int idAlunoDem )
{
	ITERA_GGROUP_LESSPTR( it_aluno_dem, alunosDemandaTotal, AlunoDemanda )
	{
		if ( it_aluno_dem->getId() == idAlunoDem )
			return *it_aluno_dem;
	}
	return NULL;
}


HorarioDia* ProblemData::getHorarioDiaCorrespondente( HorarioAula *ha, int dia )
{	
	HorarioDia * auxHD = new HorarioDia();

	auxHD->setDia( dia );
	auxHD->setHorarioAula( ha );
	auxHD->setHorarioAulaId( ha->getId() );

	GGroup< HorarioDia *, LessPtr< HorarioDia > >::iterator
		itHorarioDia = this->horariosDia.find( auxHD );
	
	delete auxHD;

	if ( itHorarioDia == this->horariosDia.end() )
	{
		return NULL;
	}
	return *itHorarioDia;
}


HorarioDia* ProblemData::getHorarioDiaCorrespondente( int horarioAulaId, int dia )
{	
	ITERA_GGROUP_LESSPTR( itHd, horariosDia, HorarioDia )
	{
		if ( itHd->getDia() == dia &&
			itHd->getHorarioAula()->getId() == horarioAulaId )
		{
			return *itHd;
		}
	}

	return NULL;
}

// Retorna o maior id das demandas já cadastradas
int ProblemData::retornaMaiorIdDemandas()
{
   int id_demanda = -1;

   ITERA_GGROUP_LESSPTR( it_demanda, this->demandasTotal, Demanda )
   {
      if ( it_demanda->getId() > id_demanda )
      {
         id_demanda = it_demanda->getId();
      }
   }

   return id_demanda;
}

// Retorna o maior id das demandas já cadastradas
int ProblemData::retornaMaiorIdAlunoDemandas()
{
   int alDemId = -1;

   ITERA_GGROUP_LESSPTR( it_alunoDemanda, this->alunosDemandaTotal, AlunoDemanda )
   {
      if ( it_alunoDemanda->getId() > alDemId )
      {
         alDemId = it_alunoDemanda->getId();
      }
   }

   return alDemId;
}

AlunoDemanda* ProblemData::procuraAlunoDemanda( int discId, int alunoId, int prioridade )
{
	ITERA_GGROUP_LESSPTR( itAlDemanda, alunosDemandaTotal, AlunoDemanda )
	{
		if ( itAlDemanda->demanda->getDisciplinaId() == discId &&
			 itAlDemanda->getAlunoId() == alunoId &&
			 itAlDemanda->getPrioridade() == prioridade )
		{
			return *itAlDemanda;
		}
	}

	return NULL;
}

AlunoDemanda* ProblemData::procuraAlunoDemOrigEquiv( Disciplina* disc, Aluno *aluno, int prioridade )
{
	ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
	{
		if ( itAlDemanda->demanda->disciplina == disc &&
			 itAlDemanda->getPrioridade() <= prioridade )
		{
			return *itAlDemanda;
		}
	}

	// not found at first level, search deeper

	Disciplina *disciplina=NULL;
	if ( disc->getId() < 0 ) disciplina = this->refDisciplinas[ -disc->getId() ];
	else disciplina = disc;

	ITERA_GGROUP_LESSPTR( itDiscOrig, disciplina->discEquivalentes, Disciplina ) // itera as possiveis substituídas (originais)
	{
		int origId = (*itDiscOrig)->getId();

		AlunoDemanda *alDem = aluno->getAlunoDemanda( origId );
		if ( retornaTurmaDiscAluno( aluno, *itDiscOrig ) == -1 &&
			 alDem != NULL &&
			 alDem->getPrioridade() <= prioridade )
		{			
			if ( disc->getId() < 0 && origId > 0 )
			{
				if ( this->refDisciplinas.find(-origId) != this->refDisciplinas.end() )
				{
					AlunoDemanda *alDemPratica = aluno->getAlunoDemanda( -origId );
					return alDemPratica;
				}
			}
			return alDem;
		}
	}


	std::cout<<"\nAtencao: ProblemData::procuraAlunoDemOrigEquiv: Demanda nao encontrada. Aluno "
		<< aluno->getAlunoId() << " Disc " << disc->getId() << endl;

	return NULL;
}

AlunoDemanda* ProblemData::procuraAlunoDemEquiv( Disciplina* disc, Aluno *aluno, int prioridade )
{
	ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
	{
		if ( itAlDemanda->demanda->disciplina == disc &&
			 itAlDemanda->getPrioridade() <= prioridade )
		{
			return *itAlDemanda;
		}
	}

	// not found at first level, search deeper

	Disciplina *disciplina=NULL;
	if ( disc->getId() < 0 ) disciplina = this->refDisciplinas[ -disc->getId() ];
	else disciplina = disc;

	ITERA_GGROUP_LESSPTR( itDiscOrig, disciplina->discEquivSubstitutas, Disciplina ) // itera as possiveis SUBSTITUTAS
	{
		int origId = (*itDiscOrig)->getId();

		AlunoDemanda *alDem = aluno->getAlunoDemanda( origId );
		if ( retornaTurmaDiscAluno( aluno, *itDiscOrig ) == -1 &&
			 alDem != NULL &&
			 alDem->getPrioridade() <= prioridade )
		{			
			if ( disc->getId() < 0 && origId > 0 )
			{
				if ( this->refDisciplinas.find(-origId) != this->refDisciplinas.end() )
				{
					AlunoDemanda *alDemPratica = aluno->getAlunoDemanda( -origId );
					return alDemPratica;
				}
			}
			return alDem;
		}
	}


	std::cout<<"\nAtencao: ProblemData::procuraAlunoDemEquiv: Demanda nao encontrada. Aluno "
		<< aluno->getAlunoId() << " Disc " << disc->getId() << endl;

	return NULL;
}

std::string ProblemData::getEquivFileName( int campusId, int prioridade )
{
	std::string solName;
	solName += "Equivalencias";
	solName += this->inputIdToString();
	   
   if ( campusId != 0 )
   {	   
		 stringstream ss;
		 ss << campusId;
		 solName += "_Cp"; 
		 solName += ss.str();
   }
   if ( prioridade != 0 )
   {
		stringstream ss;
		ss << prioridade;
		solName += "_P"; 
		solName += ss.str();   		
   }
   solName += ".txt";
      
   return solName;
}

#ifdef BUILD_COM_SOL_INICIAL
AlunoDemanda* ProblemData::atualizaAlunoDemandaEquivSolInicial( int turma, Disciplina* disciplina, int campusId, Aluno *aluno, int prioridade )
{
	int idAlDemanda = this->retornaMaiorIdAlunoDemandas();
	bool achou=false;

	AlunoDemanda *alDem = NULL;

	ITERA_GGROUP_LESSPTR ( itAlDem, aluno->demandas, AlunoDemanda ) // procura AlunoDemanda original
	{
		// TODO: E SE O ORIGINAL ERA DE P2? NÃO VAI ACHAR? ARRUMAR ISSO

		alDem = (*itAlDem);
		Demanda *demandaOriginal;

		Disciplina *discOrig = alDem->demanda->disciplina;

		bool fixarInit = false;
		int turmaAluno = this->getSolTaticoInicial()->getTurma( aluno, campusId, discOrig, fixarInit );
		
		bool OK=false;

		if ( ( turmaAluno != -1 ) && // aluno alocado recentemente
			 ( disciplina->getId() < 0 && discOrig->getId() == -disciplina->getId() ) &&
			 ( alDem->demandaOriginal != NULL ) )  // teorica de 'disciplina' ja foi trocada
		{
			demandaOriginal = alDem->demandaOriginal;
			discOrig = alDem->demandaOriginal->disciplina;
			OK=true;
		}
		else 
		if ( ( turmaAluno == -1 ) && // aluno não alocado
			 ( discOrig->discEquivSubstitutas.find( disciplina ) !=
				discOrig->discEquivSubstitutas.end() ) ) // 'disciplina' é substituta de 'discOrig'
		{
			if ( this->alocacaoEquivViavel( alDem->demanda, disciplina ) )
			{
				demandaOriginal = alDem->demanda;
				OK=true;
			}else std::cout<<"Isso pode ser um erro! A nao ser que existam demandas do aluno para disc equiv entre si";
				// isso só não será erro se a discEquiv tiver outra equivalencia, valida, com uma outra demanda do aluno.
				// estou pondo esse alerta por precaução, ja que estamos mudando muita coisa relacionada a isso.
		}

		if ( OK )
		{			
			if (!fixarInit)
			{
				std::cout<<"Estranho, acho que nao era esperado atualizar equiv para solucao inicial nao fixada.";
			}
			
			achou=true;

			//aluno->getOferta()->curriculo->addIdDiscSubstitutas( disciplina->getId() );
										
			int cjtAlunoId = this->retornaCjtAlunosId( aluno );
			if ( cjtAlunoId==0 ) std::cout<<"\nERRO: nao achei cjtAlunoId, muito estranho!\n";

			bool existePraticaNova = this->refDisciplinas.find(-disciplina->getId()) != this->refDisciplinas.end();
			bool existePraticaAntiga = this->refDisciplinas.find(-discOrig->getId()) != this->refDisciplinas.end();
			int caso = ( !existePraticaAntiga && !existePraticaNova ? 1 : 
						( ( !existePraticaAntiga && existePraticaNova ? 2 :
						( existePraticaAntiga && !existePraticaNova ? 3 : 
						( existePraticaAntiga && existePraticaNova ? 4 : 0 ) ) ) ) );
						
			switch ( caso )
			{
				case 1: // (T => T)
					break;
				case 2: // (T => PT)								
					// CRIA ALUNODEMANDA PARA DISCIPLINA PRATICA

					// Será necessário trocar o alunoDemanda inserido no map de turmas/alunoDemanda,
					// inserindo o correto da disc pratica. À frente.
								
					break;
				case 3: // (PT => T)
					// REMOVE ALUNODEMANDA DE DISCIPLINA PRATICA
					if (1)
					{
						AlunoDemanda *alunoDemandaPraticaOrig = aluno->getAlunoDemanda( - abs( discOrig->getId() ) );

						this->removeAlunoDemanda( alunoDemandaPraticaOrig );
					}
					break;
				case 4: // (PT => PT)
					// ASSOCIA ALUNODEMANDA DE DISCIPLINA PRATICA ANTIGA COM PRATICA NOVA
					if ( disciplina->getId() < 0 ) // pratica
					{
						alDem = aluno->getAlunoDemanda( - abs( discOrig->getId() ) );
						discOrig = alDem->demanda->disciplina;

						int turmaAluno = this->retornaTurmaDiscAluno( aluno, discOrig );	
						if ( turmaAluno != -1 )
							std::cout<<"\nErro! como o aluno esta alocado na pratica se ainda nao o troquei??\n";

						// Reduz qtd demanda pratica original
						Demanda *demPraticaOriginal = alDem->demanda;
						demPraticaOriginal->setQuantidade( demPraticaOriginal->getQuantidade() - 1 );
					}
					break;
				default: break;
			}

			// Debita 1 unidade na quantidade de alunos para a disc de credito teorico original							
			if ( disciplina->getId() > 0 ) 
			{
				alDem->demanda->setQuantidade( alDem->demanda->getQuantidade() - 1 );							
			}

			// Modificar referência de demanda
			// -----------------------------------------------------------------
			//							DEMANDAS
												
			Oferta *oft = alDem->demanda->oferta;
			Demanda *dem = this->buscaDemanda( oft->getId(), disciplina->getId() );

			if ( dem == NULL )
			{
				// Cria nova Demanda
				int id = this->retornaMaiorIdDemandas() + 1;
				dem = new Demanda();
				dem->setId( id );
				dem->setOfertaId( oft->getId() );
				dem->setDisciplinaId( disciplina->getId() );
				dem->setQuantidade( 0 );
				dem->oferta = oft;
				dem->disciplina = disciplina;

				this->demandasTotal.add( dem );
				this->demandas.add( dem );

				this->ofertasDisc[ disciplina->getId() ].add( oft );														
				this->cjtDemandas[cjtAlunoId].add( dem );
			}

			// Cria AlunoDemanda para disciplina pratica
			if ( caso==2 && disciplina->getId() < 0 )
			{						
				idAlDemanda++;
				AlunoDemanda *novo_aluno_demanda_p = new AlunoDemanda( idAlDemanda, aluno->getAlunoId(), alDem->getPrioridade(), demandaOriginal, alDem->getExigeEquivalenciaForcada() );
				novo_aluno_demanda_p->setAluno(aluno);
				novo_aluno_demanda_p->setADemParTeorPrat( alDem );		// par de AlunoDemanda teórico-prático
				alDem->setADemParTeorPrat( novo_aluno_demanda_p);		// par de AlunoDemanda teórico-prático

				this->alunosDemandaTotal.add( novo_aluno_demanda_p );
				this->alunosDemanda.add( novo_aluno_demanda_p );
				aluno->demandas.add( novo_aluno_demanda_p );
				this->cjtAlunoDemanda[cjtAlunoId].add( novo_aluno_demanda_p );
							
				alDem = novo_aluno_demanda_p;
			}

			// Liga AlunoDemanda à Demanda
			alDem->demandaOriginal = alDem->demanda;														
			alDem->demanda = dem;
			alDem->setDemandaId( dem->getId() );
			dem->setQuantidade( dem->getQuantidade() + 1 );

			this->listSlackDemandaAluno.remove( alDem );
			this->mapDemandaAlunos[ alDem->demandaOriginal ].remove( alDem );
			if ( this->mapDemandaAlunos[ alDem->demandaOriginal ].size() == 0 )
				this->mapDemandaAlunos.erase( alDem->demandaOriginal );
			this->mapDemandaAlunos[ dem ].add( alDem );

			if ( this->cjtDisciplinas.find(disciplina) == this->cjtDisciplinas.end() )
				this->cjtDisciplinas[disciplina] = cjtAlunoId;

			// -----------------------------------------------------------------

			break;
		}
	}
					
	if (!achou)
	{
		if (  prioridade==1 || 
			( prioridade==2 && this->procuraAlunoDemanda(disciplina->getId(), aluno->getAlunoId(), 1)==NULL ) )
		{
			std::cout<<"\nErro! Nao achei AlunoDemanda original nem substituto. Aluno"
			<<aluno->getAlunoId()<<" Disc"<<disciplina->getId()<<". Rodada P"<<prioridade<<"\n";
			ITERA_GGROUP_LESSPTR ( itAlDemanda, aluno->demandas, AlunoDemanda )
				std::cout<<" (AlDem"<<itAlDemanda->getId()<<",Disc"<<itAlDemanda->demanda->getDisciplinaId()<<")";
		}
		alDem = NULL;
	}

	return alDem;
}
#endif

AlunoDemanda* ProblemData::atualizaAlunoDemandaEquiv( int turma, Disciplina* disciplina, int campusId, Aluno *aluno, int prioridade )
{
	int idAlDemanda = this->retornaMaiorIdAlunoDemandas();
	bool achou=false;

	AlunoDemanda *alDem = NULL;

	ITERA_GGROUP_LESSPTR ( itAlDem, aluno->demandas, AlunoDemanda ) // procura AlunoDemanda original
	{
		alDem = (*itAlDem);
		Demanda *demandaOriginal;

		Disciplina *discOrig = alDem->demanda->disciplina;
		int turmaAluno = this->retornaTurmaDiscAluno( aluno, discOrig );	

		bool OK=false;

		if ( ( turmaAluno != -1 ) && // aluno alocado recentemente
			 ( disciplina->getId() < 0 && discOrig->getId() == -disciplina->getId() ) &&
			 ( alDem->demandaOriginal != NULL ) )  // teorica de 'disciplina' ja foi trocada
		{
			demandaOriginal = alDem->demandaOriginal;
			discOrig = alDem->demandaOriginal->disciplina;
			OK=true;
		}
		else 
		if ( ( turmaAluno == -1 ) && // aluno não alocado
			 ( discOrig->discEquivSubstitutas.find( disciplina ) !=
				discOrig->discEquivSubstitutas.end() ) ) // 'disciplina' é substituta de 'discOrig'
		{
			if ( this->alocacaoEquivViavel( alDem->demanda, disciplina ) )
			{
				demandaOriginal = alDem->demanda;
				OK=true;
			}else std::cout<<"Isso pode ser um erro! A nao ser que existam demandas do aluno para disc equiv entre si";
				// isso só não será erro se a discEquiv tiver outra equivalencia, valida, com uma outra demanda do aluno.
				// estou pondo esse alerta por precaução, ja que estamos mudando muita coisa relacionada a isso.
		}

		if ( OK )
		{
			achou=true;

			//aluno->getOferta()->curriculo->addIdDiscSubstitutas( disciplina->getId() );
										
			int cjtAlunoId = this->retornaCjtAlunosId( aluno );
			if ( cjtAlunoId==0 ) std::cout<<"\nERRO: nao achei cjtAlunoId, muito estranho!\n";

			bool existePraticaNova = this->refDisciplinas.find(-disciplina->getId()) != this->refDisciplinas.end();
			bool existePraticaAntiga = this->refDisciplinas.find(-discOrig->getId()) != this->refDisciplinas.end();
			int caso = ( !existePraticaAntiga && !existePraticaNova ? 1 : 
						( ( !existePraticaAntiga && existePraticaNova ? 2 :
						( existePraticaAntiga && !existePraticaNova ? 3 : 
						( existePraticaAntiga && existePraticaNova ? 4 : 0 ) ) ) ) );
						
			switch ( caso )
			{
				case 1: // (T => T)
					break;
				case 2: // (T => PT)								
					// CRIA ALUNODEMANDA PARA DISCIPLINA PRATICA

					// Será necessário trocar o alunoDemanda inserido no map de turmas/alunoDemanda,
					// inserindo o correto da disc pratica. À frente.
								
					break;
				case 3: // (PT => T)
					// REMOVE ALUNODEMANDA DE DISCIPLINA PRATICA
					if (1)
					{
						AlunoDemanda *alunoDemandaPraticaOrig = aluno->getAlunoDemanda( - abs( discOrig->getId() ) );

						this->removeAlunoDemanda( alunoDemandaPraticaOrig );
					}
					break;
				case 4: // (PT => PT)
					// ASSOCIA ALUNODEMANDA DE DISCIPLINA PRATICA ANTIGA COM PRATICA NOVA
					if ( disciplina->getId() < 0 ) // pratica
					{
						alDem = aluno->getAlunoDemanda( - abs( discOrig->getId() ) );
						discOrig = alDem->demanda->disciplina;

						int turmaAluno = this->retornaTurmaDiscAluno( aluno, discOrig );	
						if ( turmaAluno != -1 )
							std::cout<<"\nErro! como o aluno esta alocado na pratica se ainda nao o troquei??\n";

						// Reduz qtd demanda pratica original
						Demanda *demPraticaOriginal = alDem->demanda;
						demPraticaOriginal->setQuantidade( demPraticaOriginal->getQuantidade() - 1 );
					}
					break;
				default: break;
			}

			// Debita 1 unidade na quantidade de alunos para a disc de credito teorico original							
			if ( disciplina->getId() > 0 ) 
			{
				alDem->demanda->setQuantidade( alDem->demanda->getQuantidade() - 1 );							
			}

			// Modificar referência de demanda
			// -----------------------------------------------------------------
			//							DEMANDAS
												
			Oferta *oft = alDem->demanda->oferta;
			Demanda *dem = this->buscaDemanda( oft->getId(), disciplina->getId() );

			if ( dem == NULL )
			{
				// Cria nova Demanda
				int id = this->retornaMaiorIdDemandas() + 1;
				dem = new Demanda();
				dem->setId( id );
				dem->setOfertaId( oft->getId() );
				dem->setDisciplinaId( disciplina->getId() );
				dem->setQuantidade( 0 );
				dem->oferta = oft;
				dem->disciplina = disciplina;

				this->demandasTotal.add( dem );
				this->demandas.add( dem );

				this->ofertasDisc[ disciplina->getId() ].add( oft );														
				this->cjtDemandas[cjtAlunoId].add( dem );
			}

			// Cria AlunoDemanda para disciplina pratica
			if ( caso==2 && disciplina->getId() < 0 )
			{						
				idAlDemanda++;
				AlunoDemanda *novo_aluno_demanda_p = new AlunoDemanda( idAlDemanda, aluno->getAlunoId(), alDem->getPrioridade(), demandaOriginal, alDem->getExigeEquivalenciaForcada() );
				novo_aluno_demanda_p->setAluno(aluno);
				novo_aluno_demanda_p->setADemParTeorPrat( alDem );		// par de AlunoDemanda teórico-prático
				alDem->setADemParTeorPrat( novo_aluno_demanda_p);		// par de AlunoDemanda teórico-prático

				this->alunosDemandaTotal.add( novo_aluno_demanda_p );
				this->alunosDemanda.add( novo_aluno_demanda_p );
				aluno->demandas.add( novo_aluno_demanda_p );
				this->cjtAlunoDemanda[cjtAlunoId].add( novo_aluno_demanda_p );
							
				// reinsere corretamente o alunoDemanda pratico nos maps!
				Trio< int /*campusId*/, int /*turma*/, Disciplina*> trio;
				trio.set(campusId, turma, disciplina);
				this->mapAluno_CampusTurmaDisc[aluno].add( trio );

				AlunoDemanda* alDemTeorico=NULL;							
				GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > *group;

				if ( this->mapCampusTurmaDisc_AlunosDemanda.find(trio) !=
						this->mapCampusTurmaDisc_AlunosDemanda.end() )
					group = & this->mapCampusTurmaDisc_AlunosDemanda[trio];

				ITERA_GGROUP_LESSPTR( it, *group, AlunoDemanda )
				{
					if ( it->getAlunoId() == aluno->getAlunoId() )
					{
						alDemTeorico = *it;
						if ( it->demanda->getDisciplinaId() == disciplina->getId() )
						{
							std::cout<<"\nAtencao! Estranho! achei alunodemanda com disc"<<disciplina->getId()
								<<" aluno"<<aluno->getAlunoId()<<" para caso 2 (T=>PT)." 
								<<"Achei que isso so seria possivel apos a troca de demandas que estou fazendo agora.";
						}
						break;
					}
				}
				if ( alDemTeorico==NULL ) 
				{
					std::cout<<"ERRO BRUTO!"; continue;
				}
				if ( alDemTeorico->demanda->getDisciplinaId() != abs(disciplina->getId()) ) // ainda nao troquei a teorica substituta
				if ( alDemTeorico->demanda->disciplina->discEquivSubstitutas.find( disciplina ) ==
						alDemTeorico->demanda->disciplina->discEquivSubstitutas.end() ) // entao alDemTeorico->demanda->disciplina deve ser a original teorica
				{
					if ( alDemTeorico->demanda->disciplina->getId() != discOrig->getId() )
						std::cout<<"\nErro1: origId"<<discOrig->getId();
					std::cout<<"\nErro, a disc"<<alDemTeorico->demanda->getDisciplinaId()<<" nao eh a teorica correta"
					<<" nem substituida da disc"<<disciplina->getId()<<"\n\tsubstitutas:";
					ITERA_GGROUP_LESSPTR( it, alDemTeorico->demanda->disciplina->discEquivSubstitutas, Disciplina )
						std::cout<<" "<<it->getId();
				}

				if ( alDemTeorico!=NULL )
					this->mapCampusTurmaDisc_AlunosDemanda[trio].remove( alDemTeorico );							
				this->mapCampusTurmaDisc_AlunosDemanda[trio].add( novo_aluno_demanda_p );

				alDem = novo_aluno_demanda_p;
			}

			// Liga AlunoDemanda à Demanda
			alDem->demandaOriginal = alDem->demanda;														
			alDem->demanda = dem;
			alDem->setDemandaId( dem->getId() );
			dem->setQuantidade( dem->getQuantidade() + 1 );

			this->listSlackDemandaAluno.remove( alDem );
			this->mapDemandaAlunos[ alDem->demandaOriginal ].remove( alDem );
			if ( this->mapDemandaAlunos[ alDem->demandaOriginal ].size() == 0 )
				this->mapDemandaAlunos.erase( alDem->demandaOriginal );
			this->mapDemandaAlunos[ dem ].add( alDem );

			if ( this->cjtDisciplinas.find(disciplina) == this->cjtDisciplinas.end() )
				this->cjtDisciplinas[disciplina] = cjtAlunoId;

			// -----------------------------------------------------------------

			break;
		}
	}
					
	if (!achou)
	{
		if (  prioridade==1 || 
			( prioridade==2 && this->procuraAlunoDemanda(disciplina->getId(), aluno->getAlunoId(), 1)==NULL ) )
		{
			std::cout<<"\nErro! Nao achei AlunoDemanda original nem substituto. Aluno"
			<<aluno->getAlunoId()<<" Disc"<<disciplina->getId()<<". Rodada P"<<prioridade<<"\n";
			ITERA_GGROUP_LESSPTR ( itAlDemanda, aluno->demandas, AlunoDemanda )
				std::cout<<" (AlDem"<<itAlDemanda->getId()<<",Disc"<<itAlDemanda->demanda->getDisciplinaId()<<")";
		}
		alDem = NULL;
	}

	return alDem;
}

/*
	Atualiza alunosDemanda e demandas, retirando o que não foi atendido
	de prioridade p-1 e acrescentando o que há de demanda de prioridade p
*/
void ProblemData::atualizaDemandas( int novaPrioridade, int campusId )
{	
	std::cout << "\nAtualizacao de demandas de prioridade " << novaPrioridade <<"..."; fflush(0);

	int velhaPrioridade = novaPrioridade - 1;

	mapDemandaAlunos.clear();

	// -----------------------------------------------------
	// NAO-ATENDIMENTO de velhaPrioridade
	
	// Retira de alunosDemanda e de cada aluno todos os AlunosDemanda de velhaPrioridade que não foram atendidos
	ITERA_GGROUP_LESSPTR( itAlDem, this->listSlackDemandaAluno, AlunoDemanda )
	{
		AlunoDemanda *ad = *itAlDem;

		if ( ad->getPrioridade() != velhaPrioridade )
		{
			continue;
		}

		this->alunosDemanda.remove( ad );
		int q = ad->demanda->getQuantidade();
		ad->demanda->setQuantidade( --q ); // decrementa 1 unidade de atendimento

		Aluno *a = retornaAluno( ad->getAlunoId() );
		a->demandas.remove( ad );
		
	}
	
	// Retira de alunosDemanda e de cada aluno todos os AlunosDemanda de velhaPrioridade que não foram TOTALMENTE atendidos
	ITERA_GGROUP_LESSPTR( itAlDem, this->alunosDemanda, AlunoDemanda )
	{
		int discId = itAlDem->demanda->getDisciplinaId();
		int alunoId = itAlDem->getAlunoId();

		// Se existe a disciplina pratica ou teorica correspondente
		if ( this->refDisciplinas.find( - discId ) !=
			 this->refDisciplinas.end() )
		{	
			AlunoDemanda * ad = procuraAlunoDemanda( - discId, alunoId, itAlDem->getPrioridade() );

			// Se não houve o atendimento completo de itAlDem (ad=NULL),
			// então este deve ser retirado de alunosDemanda
			if ( ad == NULL )
			{		
				Aluno *a = retornaAluno( alunoId );
				a->demandas.remove( *itAlDem );

				int q = itAlDem->demanda->getQuantidade();
				itAlDem->demanda->setQuantidade( --q );
				this->alunosDemanda.remove( *itAlDem );
		
				itAlDem = this->alunosDemanda.begin();
			}
		}
	}
	
	// Retira de demandas todas as Demanda que não possuem mais alunos associados (quantidade = 0)
	GGroup< Demanda *, LessPtr< Demanda > >::iterator itDem = this->demandas.begin();
	while( itDem != this->demandas.end() )
	{
		if ( itDem->getQuantidade() == 0 )
		{
			itDem = this->demandas.remove( (*itDem) );
		}
		else itDem++;
	}
	
	// -----------------------------------------------------
	// NOVAS DEMANDAS de novaPrioridade
	// Filtrando alunosDemanda: somente os alunosDemanda que tiverem a prioridade procurada
	// FILTRAR: NOVAS PRIORIDADES SOMENTE PARA OS ALUNOS QUE NÃO TIVERAM TOTAL ATENDIMENTO ANTERIORMENTE!
	ITERA_GGROUP_LESSPTR( itAlDem, this->alunosDemandaTotal, AlunoDemanda )
	{
		int alunoId = (*itAlDem)->getAlunoId();
		Aluno *aluno = this->retornaAluno(alunoId);
		if (aluno==NULL){
			std::cout<<"\nATENCAO: ALUNO NAO ENCONTRADO. ID="<<alunoId; fflush(NULL);	continue;
		}
		// Pula AlunoDemanda possivelmente repetido
		bool repetido=false;
		ITERA_GGROUP_LESSPTR( itAlDem2, aluno->demandas, AlunoDemanda )
		{
			if ( (*itAlDem2)->demanda->getDisciplinaId() == (*itAlDem)->demanda->getDisciplinaId() )
			{
				repetido=true;
				break;
			}

			if ( (*itAlDem2)->demandaOriginal != NULL ) // CASO DE SUBSTITUIÇÃO POR EQUIVALENCIA
			if ( (*itAlDem2)->demandaOriginal->getDisciplinaId() == (*itAlDem)->demanda->getDisciplinaId() )
			{
				repetido=true;
				break;
			}
		}
		if (repetido) continue;

		if ( itAlDem->getPrioridade() == novaPrioridade && 
			 itAlDem->demanda->oferta->getCampusId() == campusId )
		{
			// Só insere o alunoDemanda novo se existir folga de demanda anterior para o aluno correspondente
			ITERA_GGROUP_LESSPTR( itSlack, this->listSlackDemandaAluno, AlunoDemanda )
			{
				if ( (*itSlack)->getAlunoId() == alunoId )
				{
					this->alunosDemanda.add( *itAlDem );					
					aluno->demandas.add( *itAlDem );
					break;
				}
			}
		}
	}

	// Atualiza total de turmas existentes referentes aos alunos-demanda atuais existentes
	this->totalTurmas_AlDem = 0;
	ITERA_GGROUP_LESSPTR( it_aldem, this->alunosDemanda, AlunoDemanda )
	{
		this->totalTurmas_AlDem += it_aldem->demanda->disciplina->getNumTurmas();
	}

	// Filtrando as demandas
	this->preencheMapDisciplinaAlunosDemanda( this->parametros->considerar_equivalencia_por_aluno );
	
	ITERA_GGROUP_LESSPTR( itDem, this->demandasTotal, Demanda )
	{
		Disciplina *disciplina = (*itDem)->disciplina;
		int campusId = itDem->oferta->getCampusId();
		int demandaId = itDem->getId();
		int n = 0;

		ITERA_GGROUP_LESSPTR( itAlDem, this->alunosDemanda, AlunoDemanda )
		{
			if ( itAlDem->getDemandaId() == demandaId )
			{
				n++;
				this->mapDemandaAlunos[ *itDem ].add( *itAlDem );
			}
		}
		if ( n )
		{
			itDem->setQuantidade( n );
			this->demandas.add( *itDem );
		}
	}

	calculaDemandas();
	
	std::cout << "  atualizadas!\n"; fflush(0);
}

// Dado o id de uma demanda, retorna o campus correspondente.
// Função necessário quando as referências ainda não foram setadas.
int ProblemData::retornaCampusId( int demandaId )
{
	int id = -1;
   
	ITERA_GGROUP_LESSPTR( it_dem, this->demandasTotal, Demanda ) 
    {		
		if ( it_dem->getId() == demandaId )
		{
			int oftId = it_dem->getOfertaId();

			ITERA_GGROUP_LESSPTR( it_oft, this->ofertas, Oferta ) 
			{		
				if ( it_oft->getId() == oftId )
				{
					id = it_oft->getCampusId();

					return id;
				}
			}
		}
	}

	return id;
}

void ProblemData::calculaDemandas()
{
   Demanda * demanda = NULL;
   Campus * campus = NULL;
   Curso * curso = NULL;

   this->map_campus_curso_demanda.clear();
   this->demandas_campus.clear();

   ITERA_GGROUP_LESSPTR( it_demanda, this->demandas, Demanda )
   {
      demanda = ( *it_demanda );

      campus = demanda->oferta->campus;
      curso = demanda->oferta->curso;

      //-------------------------------------------------------------------
      // Relaciona a 'Demanda' atual a seus respectivos 'Campus' e 'Curso'
      std::pair< Campus *, Curso * > campus_curso
         = std::make_pair( campus, curso );

      GGroup< Demanda *, LessPtr< Demanda > > * lista_demandas
         = &( this->map_campus_curso_demanda[ campus_curso ] );

      lista_demandas->add( demanda );
      //-------------------------------------------------------------------

      //-------------------------------------------------------------------
      int dem = demanda->getQuantidade();

      demanda->disciplina->setMaxDemanda( (demanda->disciplina->getMaxDemanda() > dem) ? demanda->disciplina->getMaxDemanda() : dem );

      demanda->disciplina->adicionaDemandaTotal( dem );
      //-------------------------------------------------------------------

      //-------------------------------------------------------------------
      // Armazenando a demanda total de cada Campus
      std::pair< int, int > demanda_campus
         = std::make_pair( demanda->disciplina->getId(),
           demanda->oferta->campus->getId() );

      // Inicializa com zero caso ainda não exista;
      if ( this->demandas_campus.find( demanda_campus ) ==
           this->demandas_campus.end() )
      {
         this->demandas_campus[ demanda_campus ] = 0;
      }

      this->demandas_campus[ demanda_campus ] += ( dem );
      //-------------------------------------------------------------------
   }
}

bool ProblemData::atendeCursoTurmaDisc( int turma, Disciplina* disc, int campusId, int cursoId )
{
	Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio;
	trio.set( campusId, turma, disc );

	std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> > >::iterator 
		itMap = this->mapCampusTurmaDisc_AlunosDemanda.find( trio );
	if ( itMap != this->mapCampusTurmaDisc_AlunosDemanda.end() )
	{
		GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > ggroupAlunosDem = itMap->second;
		ITERA_GGROUP_LESSPTR( it, ggroupAlunosDem, AlunoDemanda )
		{
			if ( it->demanda->oferta->getCursoId() == cursoId )
				return true;
		}
	}
	return false;
}

/*
	Pesquisa em demanda, ou seja, considerando no maximo a prioridade atual.
*/
bool ProblemData::haDemandaDiscNoCampus( int disciplina, int campusId )
{	
	Disciplina *disc = this->refDisciplinas[disciplina];

	std::map< int, std::map< Disciplina*, std::map< int, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> >, LessPtr<Disciplina> > >::iterator
		it1 = mapDisciplinaAlunosDemanda.find( campusId );
	if ( it1 != mapDisciplinaAlunosDemanda.end() )
	{
		std::map< Disciplina*, std::map< int, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> >, LessPtr<Disciplina> > *map2 = & it1->second;
		std::map< Disciplina*, std::map< int, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> >, LessPtr<Disciplina> >::iterator
			it2 = map2->find( disc );
		if ( it2 != map2->end() )
		{
			if ( it2->second.size() > 0 )
				return true;			
		}		
	}

	/*
	ITERA_GGROUP_LESSPTR ( itDem, this->demandas, Demanda )
	{
		if ( (*itDem)->getDisciplinaId() == disciplina &&
			 (*itDem)->oferta->getCampusId() == campusId &&
			 (*itDem)->getQuantidade() > 0 )
		{
			return true;
		}
	}*/

	return false;
}

/*
	Pesquisa em demanda de cada aluno, ou seja, considerando no MAXIMO a prioridade atual.
*/
GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> ProblemData::retornaDemandasDiscNoCampus( int disciplinaId, int campusId, int prioridade )
{	
	GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> alunosDemanda;

	Disciplina *disc = this->refDisciplinas[disciplinaId];

	std::map< int, std::map< Disciplina*, std::map< int, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> >, LessPtr<Disciplina> > >::iterator
		it1 = mapDisciplinaAlunosDemanda.find( campusId );
	if ( it1 != mapDisciplinaAlunosDemanda.end() )
	{
		std::map< Disciplina*, std::map< int, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> >, LessPtr<Disciplina> > *map2 = &it1->second;
		std::map< Disciplina*, std::map< int, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> >, LessPtr<Disciplina> >::iterator
			it2 = map2->find( disc );
		if ( it2 != map2->end() )
		{
			std::map< int, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> > *map3 = &it2->second;
			std::map< int, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> >::iterator it3 = map3->begin();
			for ( ; it3 != map3->end(); it3++ )
			{
				if ( it3->first > prioridade ) continue;

				alunosDemanda.add( it3->second );	
			}			
		}		
	}
	
	/*
	ITERA_GGROUP_LESSPTR ( itAl, this->alunos, Aluno )
	{
		if ( (*itAl)->getOferta()->getCampusId() != campusId )
			continue;

		ITERA_GGROUP_LESSPTR ( itAlDem, (*itAl)->demandas, AlunoDemanda )
		{
			if ( (*itAlDem)->demanda->getDisciplinaId() == disciplinaId &&				
				(*itAlDem)->getPrioridade() <= prioridade )
			{
				alunosDemanda.add( *itAlDem );
			}
		}
	}*/

	return alunosDemanda;
}

/*
	Pesquisa em demanda de cada aluno, ou seja, considerando no maximo a prioridade atual, olhando também pelas
	disciplinas equivalentes mesmo se o aluno estiver alocado na disciplina original da demanda corrente.
*/
GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> ProblemData::retornaMaxDemandasDiscNoCampus_EquivTotal( Disciplina* disciplina, int campusId, int prioridade, bool permiteRealocacao )
{	
	GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> alunosDemanda;

	ITERA_GGROUP_LESSPTR ( itAl, this->alunos, Aluno )
	{
		Aluno *aluno = *itAl;
		
		ITERA_GGROUP_LESSPTR ( itAlDem, aluno->demandas, AlunoDemanda )
		{
			AlunoDemanda *alunoDemanda = *itAlDem;

			if ( alunoDemanda->getPrioridade() > prioridade )
				continue;
			if ( alunoDemanda->getCampus()->getId() != campusId )
				continue;

			Disciplina *d = alunoDemanda->demanda->disciplina;
			
			int turmaAluno = this->retornaTurmaDiscAluno( aluno, d );
			bool alocado=false;
			if ( turmaAluno != -1 ) alocado=true;

			if ( d != disciplina ) // procura disciplinaId nas equivalencias
			{				
				if ( (alocado && permiteRealocacao) || (!alocado) )
				{
					// Se a disciplina d já não for uma substituta da original. Ou seja, não permite transitividade entre equivalências
					if ( alunoDemanda->demandaOriginal == NULL )
					if ( this->alocacaoEquivViavel( alunoDemanda->demanda, disciplina ) )
						alunosDemanda.add( *itAlDem );
				}
			}			
			else
			{
				bool jaTrocou = (alunoDemanda->demandaOriginal != nullptr);

				if ( !alunoDemanda->getExigeEquivalenciaForcada() || jaTrocou )
					alunosDemanda.add( alunoDemanda );
			}
		}
	}

	return alunosDemanda;
}

bool ProblemData::haDemandasDiscNoCampus_Equiv( int disciplinaId, int campusId, int prioridade )
{		
	ITERA_GGROUP_LESSPTR ( itAl, this->alunos, Aluno )
	{
		Aluno *aluno = *itAl;
		
		ITERA_GGROUP_LESSPTR ( itAlDem, aluno->demandas, AlunoDemanda )
		{
			if ( (*itAlDem)->getPrioridade() > prioridade )
				continue;
			if ( (*itAlDem)->getCampus()->getId() != campusId )
				continue;

			Disciplina *d = (*itAlDem)->demanda->disciplina;
			int turmaAluno = this->retornaTurmaDiscAluno( aluno, d );				
			if ( turmaAluno == -1 && d->getId()!=disciplinaId ) // aluno não alocado, procura disciplinaId nas equivalencias
			{
				if ( aluno->getCargaHorariaOrigRequeridaP1() -
					 this->cargaHorariaJaAtendida( aluno ) <= 0 )
					 continue;

				ITERA_GGROUP_LESSPTR ( itDiscEq, d->discEquivSubstitutas, Disciplina )
				{
					if ( (*itDiscEq)->getId() == disciplinaId )
					if ( this->alocacaoEquivViavel( itAlDem->demanda, *itDiscEq ) )
					{
						return true;						
					}
				}
			}
			else if ( turmaAluno != -1 && d->getId() == disciplinaId )
			{
				return true;
			}
		}
	}

	return false;
}

bool ProblemData::haDemandaDiscNoCampus( int disciplina, int campusId, int prioridade )
{	
	Disciplina *disc = this->refDisciplinas[disciplina];

	std::map< int, std::map< Disciplina*, std::map< int, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> >, LessPtr<Disciplina> > >::iterator
		it1 = mapDisciplinaAlunosDemanda.find( campusId );
	if ( it1 != mapDisciplinaAlunosDemanda.end() )
	{
		std::map< Disciplina*, std::map< int, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> >, LessPtr<Disciplina> > *map2 = & it1->second;
		std::map< Disciplina*, std::map< int, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> >, LessPtr<Disciplina> >::iterator
			it2 = map2->find( disc );
		if ( it2 != map2->end() )
		{
			std::map< int, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> > *map3 = &it2->second;
			std::map< int, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> >::iterator it3 = map3->find(prioridade);
			if ( it3 != map3->end() )
			{
				GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> *alunosdemanda = & it3->second;
				if ( alunosdemanda->size() > 0 )
					return true;
				else return false;
			}
			else return false;
		}
		else return false;
	}
	else return false;

	/*
	ITERA_GGROUP_LESSPTR ( itAlDem, this->alunosDemanda, AlunoDemanda )
	{
		if ( (*itAlDem)->demanda->getDisciplinaId() == disciplina &&
			 (*itAlDem)->demanda->oferta->getCampusId() == campusId &&
			 (*itAlDem)->getPrioridade() == prioridade )
		{
			return true;
		}
	}

	return false;
	*/
}


int ProblemData::existeTurmaDiscCampus( int turma, int discId, int campusId )
{
	Disciplina *disciplina = this->refDisciplinas[ discId ];

	Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio;
	trio.set( campusId, turma, disciplina );

	std::map< Trio< int, int, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
		itMap = mapCampusTurmaDisc_AlunosDemanda.find( trio );

	if ( itMap != mapCampusTurmaDisc_AlunosDemanda.end() )
	{
		return itMap->second.size();
	}

	return 0;
}

GGroup<Aluno*, LessPtr<Aluno>> ProblemData::alunosEmComum( int turma1, Disciplina* disc1, int turma2, Disciplina* disc2, Campus* campus )
{
	GGroup<Aluno*, LessPtr<Aluno>> alunosEmComum;

	Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio1;
	trio1.set( campus->getId(), turma1, disc1 );
	Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio2;
	trio2.set( campus->getId(), turma2, disc2 );

	// Acha os alunoDemanda da primeira turma
	GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > alunos1;

	std::map< Trio< int, int, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator 
		itMap1 = mapCampusTurmaDisc_AlunosDemanda.find( trio1 );
	if ( itMap1 != mapCampusTurmaDisc_AlunosDemanda.end() )
		alunos1 = itMap1->second;

	// Acha os alunoDemanda da segunda turma
	GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > alunos2;

	std::map< Trio< int, int, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator 
		itMap2 = mapCampusTurmaDisc_AlunosDemanda.find( trio2 );
	if ( itMap2 != mapCampusTurmaDisc_AlunosDemanda.end() )
		alunos2 = itMap2->second;

	// Acha os alunos em comum
	ITERA_GGROUP_LESSPTR( itAlDem1, alunos1, AlunoDemanda )
	{
		int aluno1Id = (*itAlDem1)->getAlunoId();

		ITERA_GGROUP_LESSPTR( itAlDem2, alunos2, AlunoDemanda )
		{
			int aluno2Id = (*itAlDem2)->getAlunoId();

			if ( aluno1Id == aluno2Id )
			{
				Aluno* aluno = this->retornaAluno( aluno1Id );
				alunosEmComum.add( aluno );
			}
		}			
	}

	return alunosEmComum;
}

bool ProblemData::possuiAlunosEmComum( int turma1, Disciplina* disc1, Campus* campus1, int turma2, Disciplina* disc2, Campus* campus2 )
{
	Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio1;
	trio1.set( campus1->getId(), turma1, disc1 );
	Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio2;
	trio2.set( campus2->getId(), turma2, disc2 );

	// Acha os alunoDemanda da primeira turma
	GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > alunos1;

	std::map< Trio< int, int, Disciplina* >, GGroup< AlunoDemanda* , LessPtr< AlunoDemanda >> >::iterator 
		itMap1 = mapCampusTurmaDisc_AlunosDemanda.find( trio1 );
	if ( itMap1 != mapCampusTurmaDisc_AlunosDemanda.end() )
		alunos1 = itMap1->second;

	// Acha os alunoDemanda da segunda turma
	GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > alunos2;

	std::map< Trio< int, int, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator 
		itMap2 = mapCampusTurmaDisc_AlunosDemanda.find( trio2 );
	if ( itMap2 != mapCampusTurmaDisc_AlunosDemanda.end() )
		alunos2 = itMap2->second;

	// Acha os alunos em comum
	ITERA_GGROUP_LESSPTR( itAlDem1, alunos1, AlunoDemanda )
	{
		int aluno1Id = (*itAlDem1)->getAlunoId();

		ITERA_GGROUP_LESSPTR( itAlDem2, alunos2, AlunoDemanda )
		{
			int aluno2Id = (*itAlDem2)->getAlunoId();

			if ( aluno1Id == aluno2Id )
			{
				return true;
			}
		}			
	}

	return false;
}


// Dadas duas disciplinas, retorna os dias disponiveis em comum entre elas
GGroup<int> ProblemData::diasComunsEntreDisciplinas( Disciplina *disciplina1, Disciplina *disciplina2 )
{
	GGroup<int> dias;

	GGroup< int >::iterator itDia1 = disciplina1->diasLetivos.begin();
	for ( ; itDia1 != disciplina1->diasLetivos.end(); itDia1++ )
	{
		int dia = *itDia1;

		GGroup<int>::iterator itDia2 = disciplina2->diasLetivos.begin();
		for ( ; itDia2 != disciplina2->diasLetivos.end(); itDia2++ )
		{
			if ( dia == *itDia2 )
				dias.add( dia );
		}
	}
		
	return dias;
}


/*
	Preenche os maps cjtAlunos, cjtDemandas, cjtAlunoDemanda e cjtDisciplinas
*/
void ProblemData::criaCjtAlunos( int campusId, int prioridade, bool FIXAR_P1 )
{	
	this->cjtAlunos.clear();		
	this->cjtDemandas.clear();
	this->cjtAlunoDemanda.clear();
	this->cjtDisciplinas.clear();

	int biggestId = 0;
	
	map< int, GGroup< Aluno *, LessPtr< Aluno > > > auxCjtAlunos;
	map< int /*idCjtAlunos*/ , int /*qtd de AlunoDemanda*/> map_CjtAlunosId_SizeDemanda;


	// ---------------------------------------------------------------------
	// Preenche auxCtAlunos

	ITERA_GGROUP_LESSPTR( itAluno, this->alunos, Aluno )
	{
		Aluno * aluno = *itAluno;

		if ( !aluno->hasCampusId(campusId) )
		{
			continue;
		}

		bool EXISTE_CJT = false;
		
		// armazena os ids dos grupos com os quais o aluno tem interseção de demanda
		// se houver mais de 1 grupo, todos eles têm que ser fundidos
		GGroup< int > gruposId;

		map< int, GGroup< Aluno *, LessPtr< Aluno > > >::iterator itCjtAlunos;

		// Procura se já existe grupo com demanda de alguma disciplina do aluno
		ITERA_GGROUP_LESSPTR( itAlDem, aluno->demandas, AlunoDemanda )
		{			
			AlunoDemanda *ad = *itAlDem;
			Disciplina *disciplina = ad->demanda->disciplina;
			GGroup<Disciplina*, LessPtr<Disciplina>> disciplinasPorAlDem;				
			disciplinasPorAlDem.add( disciplina );

			#ifdef EQUIVALENCIA_DESDE_O_INICIO
			if ( this->parametros->considerar_equivalencia_por_aluno )
			{
				ITERA_GGROUP_LESSPTR( itDisc, disciplina->discEquivSubstitutas, Disciplina )
				{
					if ( this->alocacaoEquivViavel( ad->demanda, *itDisc ) )
					{
						disciplinasPorAlDem.add( *itDisc );
					}
				}
			}
			#endif

			ITERA_GGROUP_LESSPTR( itDisc, disciplinasPorAlDem, Disciplina )
			{
				Disciplina *d = *itDisc;			

				for ( itCjtAlunos = auxCjtAlunos.begin(); itCjtAlunos != auxCjtAlunos.end(); itCjtAlunos++ )
				{
					bool EXISTE_NO_CJT_ATUAL = false;

					GGroup< Aluno *, LessPtr< Aluno > > *grupoAlunos = & (*itCjtAlunos).second;
					ITERA_GGROUP_LESSPTR( itAlunoInt, *grupoAlunos, Aluno )
					{
						ITERA_GGROUP_LESSPTR( itAlDemInt, itAlunoInt->demandas, AlunoDemanda )
						{
							if ( true)//itAlDemInt->demanda->disciplina == d )
							{
								EXISTE_CJT = true;
								EXISTE_NO_CJT_ATUAL = true;
								gruposId.add( (*itCjtAlunos).first );
								break;
							}
						}
						if (EXISTE_NO_CJT_ATUAL)
							break;
					}
				}
			}
		}

		if ( EXISTE_CJT )
		{
			if ( gruposId.size() == 1 )
			{
				int id = *gruposId.begin();
				auxCjtAlunos[id].add( aluno );
				map_CjtAlunosId_SizeDemanda[ id ] += aluno->demandas.size();
			}
			else
			{
				int nroAlDem = 0;

				GGroup< Aluno*, LessPtr< Aluno > > uniao;
				ITERA_GGROUP_N_PT( itId, gruposId, int )
				{
					int id = *itId;
					GGroup< Aluno*, LessPtr< Aluno > > *listaAlunos = & auxCjtAlunos[id];
					ITERA_GGROUP_LESSPTR ( itAl, *listaAlunos, Aluno)
					{						
						uniao.add( *itAl );
						nroAlDem += itAl->demandas.size();
					}					
				}
				uniao.add( aluno );

				// Deleta cada grupo
				ITERA_GGROUP_N_PT( itId, gruposId, int )
				{
					auxCjtAlunos.erase( *itId );
					map_CjtAlunosId_SizeDemanda.erase( *itId );
				}

				// Insere a uniao de todos os grupos deletados
				biggestId++;
				auxCjtAlunos[biggestId] = uniao;
				map_CjtAlunosId_SizeDemanda[ biggestId ] = nroAlDem;
			}
		}
		else if ( aluno->demandas.size() != 0 )
		{
			biggestId++;
			GGroup< Aluno *, LessPtr< Aluno > > novoGrupo;
			novoGrupo.add( aluno );
			auxCjtAlunos[biggestId] = novoGrupo;
			map_CjtAlunosId_SizeDemanda[ biggestId ] = aluno->demandas.size();
		}
	}

	// ---------------------------------------------------------------------
	// Preenche cjtAlunos em ordem decrescente de quantidade de AlunoDemanda
	bool AGRUPAR_CJS_PEQUENOS = true;	
	double PERC_MIN = 0.4;

	if ( prioridade > 1 && FIXAR_P1 )
	{
		PERC_MIN = 1.0;
		AGRUPAR_CJS_PEQUENOS = true;
	}

	int ID = 0;
	while ( map_CjtAlunosId_SizeDemanda.size() != 0 )
	{
		int cjtAlunoId = 0;
		int maiorQtdAlDem = 0;

		// Seleciona o id (cjtAlunoId) do proximo cjt de alunos a ser inserido em cjtAlunos		
		map< int, int >::iterator itMap = map_CjtAlunosId_SizeDemanda.begin();
		for ( ; itMap != map_CjtAlunosId_SizeDemanda.end(); itMap++ )
		{
			int id = itMap->first;
			int qtdAlDem = itMap->second;

			if ( maiorQtdAlDem < qtdAlDem )
			{
				maiorQtdAlDem = qtdAlDem;
				cjtAlunoId = id;
			}
		}
		
		ID++;
		this->cjtAlunos[ ID ] = auxCjtAlunos[cjtAlunoId];
		
		map_CjtAlunosId_SizeDemanda.erase( cjtAlunoId );
		auxCjtAlunos.erase( cjtAlunoId );

		if ( AGRUPAR_CJS_PEQUENOS )
		{
			// Agrupa os conjuntos com menos que PERC_MIN da qtd de AlunosDemanda
			if ( maiorQtdAlDem < PERC_MIN * this->getQtdAlunoDemandaAtualPorCampus(campusId) )
			{
				map< int, GGroup< Aluno *, LessPtr< Aluno > > >::iterator itMap = auxCjtAlunos.begin();
				for ( ; itMap != auxCjtAlunos.end(); itMap++ )
				{
					ITERA_GGROUP_LESSPTR( itAluno, itMap->second, Aluno )
					{
						Aluno *a = *itAluno;
						this->cjtAlunos[ ID ].add( a );
					}
				}
				map_CjtAlunosId_SizeDemanda.clear();
				auxCjtAlunos.clear();
			}
		}

	}

	// ---------------------------------------------------------------------
	// Preenche cjtDemandas, cjtAlunoDemanda e cjtDisciplinas

	map< int, GGroup< Aluno *, LessPtr< Aluno > > >::iterator 
			itCjtAlunos = this->cjtAlunos.begin();

	for ( ; itCjtAlunos != this->cjtAlunos.end(); itCjtAlunos++ )
	{
		int id = itCjtAlunos->first;
	
		GGroup< Aluno *, LessPtr< Aluno > > *grupoAlunos = & itCjtAlunos->second;
		ITERA_GGROUP_LESSPTR( itAl, *grupoAlunos, Aluno )
		{
			Aluno *a = *itAl;
			
			ITERA_GGROUP_LESSPTR( itAlDem, a->demandas, AlunoDemanda )
			{
				AlunoDemanda *alDem = *itAlDem;
				Demanda *dem = alDem->demanda;
				Disciplina *disciplina = alDem->demanda->disciplina;
				GGroup<Disciplina*, LessPtr<Disciplina>> disciplinasPorAlDem;				
				disciplinasPorAlDem.add( disciplina );

				#ifdef EQUIVALENCIA_DESDE_O_INICIO
				if ( this->parametros->considerar_equivalencia_por_aluno )
				{
					ITERA_GGROUP_LESSPTR( itDisc, disciplina->discEquivSubstitutas, Disciplina )
					{
						if ( this->alocacaoEquivViavel( alDem->demanda, *itDisc ) )
						{
							disciplinasPorAlDem.add( *itDisc );
						}
					}
				}
				#endif

				ITERA_GGROUP_LESSPTR( itDisc, disciplinasPorAlDem, Disciplina )
				{
					Disciplina *disciplina = *itDisc;

					this->cjtDemandas[id].add( dem );
					this->cjtAlunoDemanda[id].add( alDem );
		
					if ( this->cjtDisciplinas.find( disciplina ) != this->cjtDisciplinas.end() )
					{
						int idFound = this->cjtDisciplinas[ disciplina ];
						if ( idFound != id )
						{
							std::cout << "\nErro em ProblemData::criaCjtAlunos( int campusId )."
							<< "\nDisciplina " << disciplina->getId() << "ja associada ao cjt de alunos " << idFound
							<< "\nNao eh possivel associa-la ao cjt de alunos " << id;
						}
					}
					else
					{
						this->cjtDisciplinas[ disciplina ] = id;
					}
				}
			}
		}
	}



   ITERA_GGROUP_LESSPTR( it_disciplina, this->disciplinas, Disciplina )
   {
      Disciplina* disciplina = ( *it_disciplina );

	  if ( this->cp_discs[ campusId ].find( disciplina->getId() ) ==
		   this->cp_discs[ campusId ].end() )
	  {
		  continue;
 	  }

	  if ( ! this->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
	  {
		  continue;
	  }

	  map< Disciplina *, int /* cjtAlunosId */, LessPtr< Disciplina > >::iterator 
		  itMapDiscCjt = this->cjtDisciplinas.find( disciplina );

	  if ( itMapDiscCjt == this->cjtDisciplinas.end() )
	  {
		  std::cout<<"\nAtencao em criaCjtAlunos: disciplina "
						<<disciplina->getId() <<" nao pertence a nenhum conjunto\n";
	  }

   }



}

int ProblemData::retornaCjtAlunosId( int discId )
{	
	int cjtAlunosId;
	
	Disciplina *disciplina = this->refDisciplinas[ discId ];

	map< Disciplina *, int /* cjtAlunosId */, LessPtr< Disciplina > >::iterator itMap = this->cjtDisciplinas.find( disciplina );
	
	if ( itMap == this->cjtDisciplinas.end() )
	{
		//std::cout << "\nAtencao em ProblemData::retornaCjtAlunosId( int discId ):";
		//std::cout << "\nDisciplina " << discId << " nao encontrada.\n";
		return 0;
	}
	else
	{
		cjtAlunosId = itMap->second;
		return cjtAlunosId;
	}

	return 0;
}

int ProblemData::retornaCjtAlunosId( Aluno* aluno )
{	
	int cjtAlunosId;
	
	map< int /* cjtAlunosId */, GGroup< Aluno* , LessPtr< Aluno > > >::iterator 
		itMapAluno = this->cjtAlunos.begin();
	
	for ( ; itMapAluno != this->cjtAlunos.end(); itMapAluno++ )
	{
		cjtAlunosId = itMapAluno->first;

		ITERA_GGROUP_LESSPTR( itAluno, itMapAluno->second, Aluno )
		{
			Aluno *a = *itAluno;
			if ( a->getAlunoId() == aluno->getAlunoId() )
			{
				return cjtAlunosId;
			}
		}
	}

	if ( aluno->demandas.size() != 0 )
	{
		std::cout << "\nErro em int ProblemData::retornaCjtAlunosId( Aluno* aluno )."
				  << "\nAluno " << aluno->getAlunoId() << "nao encontrado.\n";
	}
	return 0;
}

int ProblemData::haDemandaDiscNoCurso( int discId, int cursoId )
{	
	// Calculando P_{d,o}
	int qtdDem = 0;

	ITERA_GGROUP_LESSPTR( itDem, demandas, Demanda )
	{
		if ( itDem->getDisciplinaId() == discId && 
			itDem->oferta->getCursoId() == cursoId )
		{
			qtdDem += itDem->getQuantidade();
		}
	}

	return qtdDem;
}

bool ProblemData::haDemandaDiscNoCursoEquiv( Disciplina *disciplina, int cursoId )
{	
	ITERA_GGROUP_LESSPTR( itDem, demandas, Demanda )
	{
		if ( itDem->oferta->getCursoId() != cursoId )
		{
			continue;
		}

		if ( itDem->getDisciplinaId() == disciplina->getId() &&
			 itDem->oferta->getCursoId() == cursoId )
		{
			return true;
		}
		if ( itDem->disciplina->discEquivSubstitutas.find( disciplina ) !=
				itDem->disciplina->discEquivSubstitutas.end() )
		{
			if ( this->alocacaoEquivViavel( *itDem, disciplina ) )	
				return true;
		}
	}


	//ITERA_GGROUP_LESSPTR( itAluno, this->alunos, Aluno )
	//{
	//	Aluno *aluno = *itAluno;
	//	
	//	ITERA_GGROUP_LESSPTR( it1AlDemanda, aluno->demandas, AlunoDemanda )
	//	{
	//		if ( it1AlDemanda->getCurso()->getId() != cursoId )
	//		{
	//			continue;
	//		}

	//		Disciplina *disc = it1AlDemanda->demanda->disciplina;
	//		if ( disc == disciplina )
	//		{				
	//			return true;
	//		}
	//		else
	//		{
	//			int turma = this->retornaTurmaDiscAluno( aluno, disc );
	//			if ( turma == -1 )
	//			{
	//				if ( disc->discEquivSubstitutas.find( disciplina ) !=
	//					 disc->discEquivSubstitutas.end() )
	//				{
	//					if ( this->alocacaoEquivViavel( it1AlDemanda->demanda, disciplina ) )	
	//						return true;
	//				}
	//			}
	//		}
	//			
	//	}
	//}

	return false;
}

int ProblemData::maxDemandaDiscNoCursoEquiv( Disciplina *disciplina, int cursoId )
{	
	int n = 0;

	ITERA_GGROUP_LESSPTR( itDem, demandas, Demanda )
	{
		if ( itDem->oferta->getCursoId() != cursoId )
		{
			continue;
		}

		if ( itDem->getDisciplinaId() == disciplina->getId() )
		{
			n += itDem->getQuantidade();
		}
		else if ( itDem->disciplina->discEquivSubstitutas.find( disciplina ) !=
				itDem->disciplina->discEquivSubstitutas.end() )
		{
			if ( this->alocacaoEquivViavel( *itDem, disciplina ) )	
				n += itDem->getQuantidade();
		}
	}



	//ITERA_GGROUP_LESSPTR( itAluno, this->alunos, Aluno )
	//{
	//	Aluno *aluno = *itAluno;

	//	ITERA_GGROUP_LESSPTR( it1AlDemanda, aluno->demandas, AlunoDemanda )
	//	{
	//		if ( it1AlDemanda->getCurso()->getId() != cursoId )
	//		{
	//			continue;
	//		}

	//		Disciplina *disc = it1AlDemanda->demanda->disciplina;
	//		if ( disc == disciplina )
	//		{
	//			n++;
	//		}
	//		else
	//		{
	//			int turma = this->retornaTurmaDiscAluno( aluno, disc );
	//			if ( turma == -1 )
	//			{
	//				if ( disc->discEquivSubstitutas.find( disciplina ) !=
	//					 disc->discEquivSubstitutas.end() )
	//				{
	//					if ( this->alocacaoEquivViavel( it1AlDemanda->demanda, disciplina ) )
	//						n++;
	//				}
	//			}
	//		}
	//			
	//	}
	//}

	return n;
}

int ProblemData::getQtdAlunoDemandaAtualPorCampus( int campusId )
{
	int n = 0;
	ITERA_GGROUP_LESSPTR( itAlDem, this->alunosDemanda, AlunoDemanda )
	{
		if ( itAlDem->demanda->oferta->getCampusId() == campusId )
		{
			n++;
		}
	}
	return n;
}


void ProblemData::imprimeCjtAlunos( int campusId )
{	
	#ifndef PRINT_LOGS
		return;
	#endif

	int totalAlunos = 0, totalDemandas = 0, totalAlunoDemanda = 0;
	int totalDemandasSP = 0, totalAlunoDemandaSP = 0;

	std::cout << "\nNumero de conjuntos: " << this->cjtAlunos.size();

	map< int, GGroup< Aluno *, LessPtr< Aluno > > >::iterator
		itMapCjtAlunos = this->cjtAlunos.begin();
			
	for ( ; itMapCjtAlunos != this->cjtAlunos.end(); itMapCjtAlunos++ )
	{
		int id = itMapCjtAlunos->first;
		int nroAlunos = itMapCjtAlunos->second.size();

		// --------------------------------------------
		// CONSIDERANDO PRATICAS SEPARADAS
		std::cout << "\nConjunto " << id
					<< ": nro de alunos = " << nroAlunos
					<< ", nro de alunosDemanda = " << this->cjtAlunoDemanda[id].size()
					<< ", nro de demandas = " << this->cjtDemandas[id].size();
			
		totalDemandas += this->cjtDemandas[id].size();
		totalAlunoDemanda += this->cjtAlunoDemanda[id].size();

		// --------------------------------------------
		// SEM CONSIDERAR PRATICAS
		int nAlDem = 0, nDem = 0;

		GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > > *gad = &this->cjtAlunoDemanda[id];
		ITERA_GGROUP_LESSPTR( itAlDem, *gad, AlunoDemanda )
		{
			if ( itAlDem->demanda->getDisciplinaId() >= 0 )
				nAlDem++;
		}
		GGroup< Demanda *, LessPtr< Demanda > > *gd = &this->cjtDemandas[id];
		ITERA_GGROUP_LESSPTR( itDem, *gd, Demanda )
		{
			if ( itDem->getDisciplinaId() >= 0 )
				nDem++;
		}

		std::cout << ", nro de alunosDemanda sem divisao de pratica = " << nAlDem
				  << ", nro de demandas sem divisao de pratica = " << nDem;

		totalDemandasSP += nDem;
		totalAlunoDemandaSP += nAlDem;
		// --------------------------------------------

		totalAlunos += nroAlunos;
	}
	
#ifdef BUILD_COM_SOL_INICIAL
	setTotalOrigAlunoDemanda( campusId, totalAlunoDemanda );
#endif

	std::cout << "\nTotal de alunos: " << totalAlunos;
	std::cout << "\nTotal de demandas: " << totalDemandas;
	std::cout << "\nTotal de alunosDemanda: " << totalAlunoDemanda;
	std::cout << "\nTotal de demandas sem divisao de pratica: " << totalDemandasSP;
	std::cout << "\nTotal de alunosDemanda sem divisao de pratica: " << totalAlunoDemandaSP;

}


// Verifica se disciplina, sala e professor possuem o horario no dia
bool ProblemData::verificaDisponibilidadeHorario( HorarioAula *horarioAula, int dia, Sala *sala, Professor *prof, Disciplina* disc )
{
	bool debugando = false;
	if ( dia == 7 &&
		sala->getId() == 4420 &&
		disc->getId() == 20927 )
	{
		debugando = true;
		std::cout<<"\n\n---verificaDisponibilidadeHorario";
		std::cout << "\nHor " << horarioAula->getId();
		if ( prof != NULL ) std::cout << "   Prof " << prof->getId() << endl;
	}

	bool achouNaSala = false;
	for( auto itHorDia = sala->horariosDia.begin(); itHorDia != sala->horariosDia.end() ; ++itHorDia )
	{
		if ( (*itHorDia)->getHorarioAula()->getInicio() == horarioAula->getInicio() &&
			(*itHorDia)->getHorarioAula()->getFinal() == horarioAula->getFinal() &&
			(*itHorDia)->getDia() == dia )
		{
			achouNaSala = true;
			break;
		}
	}
	if ( !achouNaSala )
	{
		if ( debugando ) std::cout << "\nNao achou na sala";
		return false;
	}

	bool achouNaDisc = true; /*false;
	Calendario *calendario = horarioAula->getCalendario();
	GGroup< HorarioDia*, LessPtr<HorarioDia> > listaHorariosDia = disc->getHorariosDia(calendario);
	ITERA_GGROUP_LESSPTR( itHorDia, listaHorariosDia, HorarioDia )
	{
		if ((*itHorDia)->getHorarioAula()->getInicio() == horarioAula->getInicio() &&
			(*itHorDia)->getHorarioAula()->getFinal() == horarioAula->getFinal() &&
			(*itHorDia)->getDia() == dia )
		{
			achouNaDisc = true;
			break;
		}
	}
	if ( !achouNaDisc )
	{
		if ( debugando ) std::cout << "\nNao achou na disc";
		return false;
	}*/

	if ( prof != NULL )
	{
		bool achouNoProf = false;
		ITERA_GGROUP_LESSPTR( itHorDia, prof->horariosDia, HorarioDia )
		{
			if ( (*itHorDia)->getHorarioAula()->getInicio() == horarioAula->getInicio() &&
				(*itHorDia)->getHorarioAula()->getFinal() == horarioAula->getFinal() &&
				(*itHorDia)->getDia() == dia )
			{
				achouNoProf = true;
				break;
			}
		}
		if ( !achouNoProf )
		{
			if ( debugando ) std::cout << "\nNao achou no prof";
			return false;
		}
	}

	return true;

}

/*
	Usado para a heuristica do modelo Tatico Com Horarios
*/
void ProblemData::insereAlunoEmTurma( AlunoDemanda* alunoDemanda, Trio< int /*campusId*/, int /*turma*/, Disciplina*> trio, GGroup<HorarioDia*> horariosDias )
{
	if ( alunoDemanda != NULL )
	{
		Aluno *aluno = this->retornaAluno( alunoDemanda->getAlunoId() );
		
		this->mapAluno_CampusTurmaDisc[aluno].add( trio );
		this->mapCampusTurmaDisc_AlunosDemanda[trio].add( alunoDemanda );

		ITERA_GGROUP( itHorDia, horariosDias, HorarioDia )
		{
			aluno->addHorarioDiaOcupado( *itHorDia );
		}
	}	
	else std::cout<<"\nAtencao, erro em insereAlunoEmTurma: alunoDemanda NULL\n";
}

double ProblemData::cargaHorariaNaoAtendidaPorPrioridade( int prior, int alunoId )
{
	Aluno *aluno = this->retornaAluno(alunoId);

	if ( prior == 1 )
		return aluno->getCargaHorariaOrigRequeridaP1() - cargaHorariaJaAtendida( aluno );

	return ( cargaHorariaOriginalRequeridaPorPrioridade( prior, aluno ) - cargaHorariaJaAtendida( aluno ) );
}

double ProblemData::cargaHorariaOriginalRequeridaPorPrioridade( int prior, Aluno* aluno )
{
	if ( prior == 1 )
		return aluno->getCargaHorariaOrigRequeridaP1();

	double cargaHorariaP2 = 0.0;
	ITERA_GGROUP_LESSPTR( itAlDemanda, alunosDemandaTotal, AlunoDemanda )
	{
		if ( (*itAlDemanda)->getAlunoId() != aluno->getAlunoId() )
			continue;
				
		if ( (*itAlDemanda)->getPrioridade() == prior )
		{
			Disciplina *disciplina=NULL;
			if ( (*itAlDemanda)->demandaOriginal != NULL )
				disciplina = (*itAlDemanda)->demandaOriginal->disciplina;
			else
				disciplina = (*itAlDemanda)->demanda->disciplina;

			int nCreds = disciplina->getTotalCreditos();
			int duracaoCred = disciplina->getTempoCredSemanaLetiva();
			cargaHorariaP2 += nCreds*duracaoCred;
		}
	}
	return cargaHorariaP2;
}

double ProblemData::cargaHorariaAtualRequeridaPorPrioridade( int prior, Aluno* aluno )
{
	double cargaHorariaP2 = 0.0;
	ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
	{		
		if ( (*itAlDemanda)->getPrioridade() == prior )
		{
			Disciplina *disciplina = (*itAlDemanda)->demanda->disciplina;
			int nCreds = disciplina->getTotalCreditos();
			int duracaoCred = disciplina->getTempoCredSemanaLetiva();
			cargaHorariaP2 += nCreds*duracaoCred;
		}
	}
	return cargaHorariaP2;
}

double ProblemData::cargaHorariaJaAtendida( Aluno* aluno )
{
	double cargaHoraria = 0.0;
	
	GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >
		*atendAluno = &this->mapAluno_CampusTurmaDisc[aluno];

	GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >::iterator
		itAtends = atendAluno->begin();
	for ( ; itAtends != atendAluno->end(); itAtends++ )
	{
		Disciplina *disciplina = (*itAtends).third;
		int nCreds = disciplina->getTotalCreditos();
		int duracaoCred = disciplina->getTempoCredSemanaLetiva();
		cargaHoraria += nCreds*duracaoCred;	
	}
	return cargaHoraria;
}


std::string ProblemData::getAlocacaoAlunosFileName( int campusId, int prioridade, int cjtAlunosId, bool heuristica, int r, string MIP )
{
    std::string solName;
	solName += "AlocacaoAlunos";
	solName += this->inputIdToString();
	
  // if ( tatico == 0 )
   //{
		 solName += MIP; 
//   }
   if ( campusId != 0 )
   {	   
		 stringstream ss;
		 ss << campusId;
		 solName += "_Cp"; 
		 solName += ss.str();
   }
   if ( prioridade != 0 )
   {
		stringstream ss;
		ss << prioridade;
		if ( heuristica ) solName += "_PH";
		else solName += "_P";
		solName += ss.str();   		
   }
   if ( cjtAlunosId != 0 )
   {
	    stringstream ss;
		ss << cjtAlunosId;		
		solName += "_Cjt"; 
		solName += ss.str();   		
   } 
   if ( r != 0 )
   {
		stringstream ss;
		ss << r;
		solName += "_R"; 
		solName += ss.str();   		
   } 
/*   if ( tatico != 0 )
   {
		stringstream ss;
		ss << tatico;
		solName += "_T"; 
		solName += ss.str();   		
   }   */     
   solName += ".txt";
      
   return solName;
}

std::string ProblemData::getAlocacaoTurmasFileName( int campusId, int prioridade, int cjtAlunosId, bool heuristica, int r, string MIP )
{
	std::string solName;
	solName += "AlocacaoTurmas";
	solName += this->inputIdToString();
	
   //if ( tatico == 0 )
   //{
		 solName += MIP; 
   //}
   if ( campusId != 0 )
   {	   
		 stringstream ss;
		 ss << campusId;
		 solName += "_Cp"; 
		 solName += ss.str();
   }
   if ( prioridade != 0 )
   {
		stringstream ss;
		ss << prioridade;
		if ( heuristica ) solName += "_PH";
		else solName += "_P";
		solName += ss.str();   		
   }
   if ( cjtAlunosId != 0 )
   {
	    stringstream ss;
		ss << cjtAlunosId;		
		solName += "_Cjt"; 
		solName += ss.str();   		
   } 
   if ( r != 0 )
   {
		stringstream ss;
		ss << r;
		solName += "_R"; 
		solName += ss.str();   		
   } 
/*   if ( tatico != 0 )
   {
		stringstream ss;
		ss << tatico;
		solName += "_T"; 
		solName += ss.str();   		
   }   */     
   solName += ".txt";
      
   return solName;
}

std::string ProblemData::getAlocacoesFileName()
{
   std::string solName( "Alocacoes" );
   solName += "_input";
   solName += this->getInputFileName();
   solName += this->inputIdToString();
   solName += ".txt";
      
   return solName;
}


std::string ProblemData::getDiscNTurmasFileName( int campusId, int prioridade, int cjtAlunosId, bool heuristica, int r, int tatico )
{
   std::string solName;
	solName += "DiscNTurmas";
	solName += this->inputIdToString();
	

   if ( tatico == 0 )
   {
		 solName += "Pre"; 
   }
   if ( campusId != 0 )
   {	   
		 stringstream ss;
		 ss << campusId;
		 solName += "_Cp"; 
		 solName += ss.str();
   }
   if ( prioridade != 0 )
   {
		stringstream ss;
		ss << prioridade;
		if ( heuristica ) solName += "_PH";
		else solName += "_P";
		solName += ss.str();   		
   }
   if ( cjtAlunosId != 0 )
   {
	    stringstream ss;
		ss << cjtAlunosId;		
		solName += "_Cjt"; 
		solName += ss.str();   		
   } 
   if ( r != 0 )
   {
		stringstream ss;
		ss << r;
		solName += "_R"; 
		solName += ss.str();   		
   } 
   if ( tatico != 0 )
   {
		stringstream ss;
		ss << tatico;
		solName += "_T"; 
		solName += ss.str();   		
   }        
   solName += ".txt";
      
   return solName;
}

std::string ProblemData::getUtilizacaoSalasFileName( int campusId, int prioridade, int cjtAlunosId, bool heuristica, int r, string MIP )
{
   std::string solName;
	solName += "CreditosSala";
	solName += this->inputIdToString();
	

  // if ( tatico == 0 )
   //{
		 solName += MIP; 
   //}
   if ( campusId != 0 )
   {	   
		 stringstream ss;
		 ss << campusId;
		 solName += "_Cp"; 
		 solName += ss.str();
   }
   if ( prioridade != 0 )
   {
		stringstream ss;
		ss << prioridade;
		if ( heuristica ) solName += "_PH";
		else solName += "_P";
		solName += ss.str();   		
   }
   if ( cjtAlunosId != 0 )
   {
	    stringstream ss;
		ss << cjtAlunosId;		
		solName += "_Cjt"; 
		solName += ss.str();   		
   } 
   if ( r != 0 )
   {
		stringstream ss;
		ss << r;
		solName += "_R"; 
		solName += ss.str();   		
   } 
/*   if ( tatico != 0 )
   {
		stringstream ss;
		ss << tatico;
		solName += "_T"; 
		solName += ss.str();   		
   }     */   
   solName += ".txt";
      
   return solName;
}


void ProblemData::imprimeAlocacaoAlunos( int campusId, int prioridade, int cjtAlunosId, bool heuristica, int r, string MIP, int runtime )
{
	#ifndef PRINT_LOGS
		return;
	#endif

	cout << "Imprimindo alocacao de alunos..." << endl;

	int totalAtendimentos=0;
	int totalAtendimentosSemDivPT=0;
	long int totalCreditos=0, totalCreditosPagos=0;
		
	// Alunos ------------------------------------------------------------
	ofstream alunosFile;
	std::string alunosFilename( this->getAlocacaoAlunosFileName( campusId, prioridade, cjtAlunosId, heuristica, r, MIP ) );	
	alunosFile.open( alunosFilename, ios::out );
	if (!alunosFile)
	{
		cerr << "Error: Can't open output file " << alunosFilename << endl;
		return;
	}

	std::set< std::pair<double, Aluno*> > minAtendAlunoViolado;

	std::map< Aluno*, GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >, LessPtr< Aluno > >::iterator
		itMapAlunos = this->mapAluno_CampusTurmaDisc.begin();	
	for ( ; itMapAlunos != this->mapAluno_CampusTurmaDisc.end() ; itMapAlunos++ )
	{
		Aluno *aluno = itMapAlunos->first;

		if ( ! aluno->hasCampusId(campusId) )
			continue;

		int alunoId = aluno->getAlunoId();
		int nCredsAluno = 0;
		alunosFile << "\n\nAluno " << alunoId << ": ";

		GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >::iterator itGGroup = itMapAlunos->second.begin();
		for ( ; itGGroup != itMapAlunos->second.end() ; itGGroup++ )
		{
			int campus = (*itGGroup).first;
			int turma = (*itGGroup).second;
			int disc = (*itGGroup).third->getId();
			
			if ( campus!=campusId )	continue;

			alunosFile << "i" << turma << "_Disc" << disc << "; ";
			
			nCredsAluno += (*itGGroup).third->getTotalCreditos();
			totalCreditos += (*itGGroup).third->getTotalCreditos();
			totalAtendimentos++;
			if ( disc > 0 ) totalAtendimentosSemDivPT++;
		}

		double perc = (double) nCredsAluno / aluno->getNroCredsOrigRequeridosP1();
		alunosFile << "\n% Atendido: " << perc*100;

		if ( this->parametros->considerarMinPercAtendAluno )
		if ( perc < this->parametros->minAtendPercPorAluno )
		{
			minAtendAlunoViolado.insert( std::make_pair( perc, aluno ) );
		}
	}
	
	if ( this->parametros->considerarMinPercAtendAluno )
	{
		if ( minAtendAlunoViolado.size() > 0 )
			alunosFile << "\n\nPercentual minimo de atendimento por aluno violado ("
					   << this->parametros->minAtendPercPorAluno << "%):\n";

		std::set< std::pair<double, Aluno*> >::iterator 
			itViola = minAtendAlunoViolado.begin();	
		for ( ; itViola != minAtendAlunoViolado.end() ; itViola++ )
		{
			alunosFile << "\nAluno " << itViola->second->getAlunoId() << "\t" << itViola->first * 100 << "%";
		}
	}

	alunosFile << "\n\nTotal de AlunosDemanda atendidos: " << totalAtendimentos;
	alunosFile << "\nTotal de AlunosDemanda sem divisao PT atendidos: " << totalAtendimentosSemDivPT;
	alunosFile << "\nTotal de creditos pagos: " << totalCreditos;

	alunosFile.close();

	// Turmas ------------------------------------------------------------
	
	ofstream turmasFile;	
	std::string turmasFilename( this->getAlocacaoTurmasFileName( campusId, prioridade, cjtAlunosId, heuristica, r, MIP ) );	
	turmasFile.open(turmasFilename, ios::out);
	if (!turmasFile)
	{
		std::cout << "Error: Can't open output file " << turmasFilename << endl;
		return;
	}
	
	totalAtendimentos=0;
	totalAtendimentosSemDivPT=0;
	int totalAtendimentosSemDivPT_P1=0;
	int totalAtendimentosSemDivPT_P2=0;
	int totalCredsSemDivPT_P1=0;
	int totalCredsSemDivPT_P2=0;
	totalCreditos=0;
	totalCreditosPagos=0;
	double totalTurmas=0;
	double totalReceita=0;
	double totalCusto=0;
	double custo = this->refCampus[campusId]->getCusto();

	std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
		itMapTurmas = this->mapCampusTurmaDisc_AlunosDemanda.begin();

	for ( ; itMapTurmas != this->mapCampusTurmaDisc_AlunosDemanda.end() ; itMapTurmas++ )
	{		
		int cp = itMapTurmas->first.first;
		int turma = itMapTurmas->first.second;
		int disc = itMapTurmas->first.third->getId();
		int nCredsDisc = itMapTurmas->first.third->getTotalCreditos();

		if ( cp != campusId )
			continue;

		totalCreditosPagos += nCredsDisc;		
		totalCreditos += itMapTurmas->second.size() * nCredsDisc;
		totalCusto += nCredsDisc * custo;

		if ( disc > 0 ) totalTurmas++;

		turmasFile << "\n\ni" << turma << "_Disc" << disc << ": (" << itMapTurmas->second.size() << " alunos) = ";

		GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > >::iterator itGGroup = itMapTurmas->second.begin();
		for ( ; itGGroup != itMapTurmas->second.end() ; itGGroup++ )
		{
			int alunoId = itGGroup->getAlunoId();
			turmasFile << "Aluno " << alunoId << "; ";
			
			totalReceita += nCredsDisc * itGGroup->getOferta()->getReceita();
			
			totalAtendimentos++;	
			if ( disc > 0 ) 
			{
				totalAtendimentosSemDivPT++;
				if ( itGGroup->getPrioridade() == 1 )
				{
					totalAtendimentosSemDivPT_P1++;
				}
				else 
				{
					totalAtendimentosSemDivPT_P2++;
				}
			}

			if ( itGGroup->getPrioridade() == 1 )
			{
				totalCredsSemDivPT_P1 += nCredsDisc;
			}
			else 
			{
				totalCredsSemDivPT_P2 += nCredsDisc;
			}
		}	
	}
	
	turmasFile << "\n\nCom divisao PT:";
	turmasFile << "\n\nTotal de AlunosDemanda atendidos: " << totalAtendimentos;
	turmasFile << "\nTotal de turmas: " << this->mapCampusTurmaDisc_AlunosDemanda.size();

	turmasFile << "\n\nSem divisao PT:";
	turmasFile << "\nTotal de AlunosDemanda atendidos: " << totalAtendimentosSemDivPT;
	turmasFile << "\nTotal de AlunosDemanda P1 atendidos: " << totalAtendimentosSemDivPT_P1;
	turmasFile << "\nTotal de AlunosDemanda P2 atendidos: " << totalAtendimentosSemDivPT_P2;
	turmasFile << "\nTotal de creditos P1 x nro alunos atendidos: " << totalCredsSemDivPT_P1;
	turmasFile << "\nTotal de creditos P2 x nro alunos atendidos: " << totalCredsSemDivPT_P2;
	turmasFile << "\nTotal de creditos x nro alunos atendidos: " << totalCreditos;
	turmasFile << "\nTotal de creditos pagos: " << totalCreditosPagos;
	turmasFile << "\nTotal de turmas: " << totalTurmas;
	turmasFile << "\nCusto Total: " << totalCusto;
	turmasFile << "\nReceita Total: " << totalReceita;
	turmasFile << "\nCusto / Receita: " << 100*totalCusto/totalReceita << "%";

	turmasFile.close();

	// Geral -------------------------------------------------------------------------------------------

	imprimeAlocacoesGerais( campusId, prioridade, r, MIP, totalAtendimentosSemDivPT_P1, totalAtendimentosSemDivPT_P2,
					totalCreditos, totalCreditosPagos, totalTurmas, totalCusto, totalReceita, runtime );
}

void ProblemData::imprimeAlocacoesGerais( int campusId, int prioridade, int rodada, string MIP, int totalAtendimentosSemDivPT_P1,
	int totalAtendimentosSemDivPT_P2, int totalCreditos, int totalCreditosPagos, int totalTurmas, int totalCusto, int totalReceita, int runtime )
{
	// Geral -------------------------------------------------------------------------------------------
		
   int hours = runtime / 3600;						// h
   int min = (int) ( (int) runtime % 3600 ) / 60;	// min
   std::stringstream runtimess;
   runtimess << hours << "h" << min;

	std::cout << "\nImprimindo alocacoes gerais..."<< endl;
	
	stringstream rowName;
	rowName << "Cp" << campusId << "_P" << prioridade << "_R" << rodada << "_" << MIP;

	Indicadores::printTaticoIndicadores( rowName.str(), totalAtendimentosSemDivPT_P1, totalAtendimentosSemDivPT_P2,
			totalCreditos, totalCreditosPagos, totalTurmas, totalCusto, totalReceita, runtimess.str() );

}

void ProblemData::imprimeDiscNTurmas( int campusId, int prioridade, int cjtAlunosId, bool heuristica, int r, int tatico )
{
	std::cout<<"\nImprimeDiscNTurmas...\n"; fflush(NULL);

	int totalAtendimentos=0;
	int totalAtendimentosSemDivPT=0;
	long int totalCargaHoraria=0;

	std::map<Disciplina*, std::pair<int/*nro de turmas abertas*/,int/*total de alunos atendidos*/>, LessPtr<Disciplina>> mapDiscNTurmasAbertas;

	std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
		itMapTurmas = this->mapCampusTurmaDisc_AlunosDemanda.begin();
	for ( ; itMapTurmas != this->mapCampusTurmaDisc_AlunosDemanda.end() ; itMapTurmas++ )
	{		
		int cp = itMapTurmas->first.first;
		int turma = itMapTurmas->first.second;
		int disc = itMapTurmas->first.third->getId();
		
		if ( cp != campusId )
			continue;
		
		if ( mapDiscNTurmasAbertas.find( itMapTurmas->first.third ) == mapDiscNTurmasAbertas.end() )
		{
			mapDiscNTurmasAbertas[itMapTurmas->first.third].first = 1;
			mapDiscNTurmasAbertas[itMapTurmas->first.third].second = itMapTurmas->second.size();
		}
		else
		{
			mapDiscNTurmasAbertas[itMapTurmas->first.third].first++;
			mapDiscNTurmasAbertas[itMapTurmas->first.third].second += itMapTurmas->second.size();
		}
	}
		
	// Disciplina / Numero de Turmas / Demandas ------------------------------------------------------------
	

	bool equiv = ( (tatico == -3) ? true : false );
	
	this->mapDiscDif.clear();

	std::map< int, int > mapDiscTotalDemanda;
	std::map< int, int > mapDiscNroAtend;
	std::map< int, int > mapDiscTurmasAbertas;
	std::map< int, int > mapDiscTotalTurmas;

	int totalTurmasAbertas=0;

	std::map< int, std::map< Disciplina*, std::map< int, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> >, LessPtr<Disciplina> > >::iterator
		it1 = mapDisciplinaAlunosDemanda.find( campusId );
	if ( it1 != mapDisciplinaAlunosDemanda.end() )
	{
		std::map< Disciplina*, std::map< int, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> >, LessPtr<Disciplina> > map2 = it1->second;
		std::map< Disciplina*, std::map< int, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> >, LessPtr<Disciplina> >::iterator
			it2 = map2.begin();
		for ( ; it2 != map2.end(); it2++ )
		{
			Disciplina *disciplina = it2->first;
			int demanda=0;
			std::map< int, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> > map3 = it2->second;
			std::map< int, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> >::iterator it3 = map3.begin();
			for ( ; it3 != map3.end(); it3++ )
			{				
				if ( it3->first <= prioridade )
					demanda += it3->second.size();				
			}

			int discId = disciplina->getId();			
			int totalTurmas = disciplina->getNumTurmas();
			int turmas = 0;
			int nAlunosAtendidos = 0;					

			std::map< Disciplina*, std::pair<int/*nro de turmas abertas*/,int>, LessPtr< Disciplina > >::iterator
				itMapDiscNTurmas = mapDiscNTurmasAbertas.find( disciplina );
			if ( itMapDiscNTurmas != mapDiscNTurmasAbertas.end() )
			{
				turmas = itMapDiscNTurmas->second.first;
				nAlunosAtendidos = itMapDiscNTurmas->second.second;		
			}
	
			mapDiscTurmasAbertas[discId] = turmas;
			mapDiscNroAtend[discId] = nAlunosAtendidos;
			mapDiscTotalDemanda[discId] = demanda;
			mapDiscTotalTurmas[discId] = totalTurmas;

			std::pair<int/*difTurmas*/, int/*nroNaoAtend*/> par( totalTurmas - turmas, demanda - nAlunosAtendidos );

			this->mapDiscDif[discId] = par;
			totalTurmasAbertas += turmas;
		}		
	}

	#ifndef PRINT_LOGS
		return;
	#endif

	ofstream discTurmasFile;	
	std::string discTurmasFileName( this->getDiscNTurmasFileName( campusId, prioridade, cjtAlunosId, heuristica, r, tatico ) );	
	discTurmasFile.open(discTurmasFileName, ios::out);
	if (!discTurmasFile)
	{
		std::cout << "Error: Can't open output file " << discTurmasFileName << endl;
		return;
	}
		
	discTurmasFile << "Total de turmas abertas = " << totalTurmasAbertas;
	
	int soma=0, somaSemDivisaoPT=0;
	
	// difTurmas = 0 e nroNaoAtend > 0
	discTurmasFile << "\n\n\nDisciplinas com TURMAS ESGOTADAS e NAO-ATENDIMENTO:";
	std::map<int /*discId*/, std::pair<int/*difTurmas*/, int/*nroNaoAtend*/>>::iterator
		itMapDiscNaoAtend = this->mapDiscDif.begin();
	for ( ; itMapDiscNaoAtend != this->mapDiscDif.end() ; itMapDiscNaoAtend++ )
	{		
		int discId = itMapDiscNaoAtend->first;
		std::pair<int/*difTurmas*/, int/*nroNaoAtend*/> par = itMapDiscNaoAtend->second;
		int difTurmas = par.first;
		int naoAtend = par.second;

		if ( difTurmas == 0 && naoAtend != 0 )
		{
			discTurmasFile << "\nDisc" << discId << ", Turmas Abertas:" 
				<< mapDiscTurmasAbertas[discId] << ", Total de Turmas: " << mapDiscTotalTurmas[discId]
				<< ", Atendido: " << mapDiscNroAtend[discId] << ", Total de AlunoDemandas:" << mapDiscTotalDemanda[discId];
			discTurmasFile << "\t=> NAO Atendido: " << naoAtend;
			soma += naoAtend;
			if ( discId > 0 ) somaSemDivisaoPT += naoAtend;		
		}
	}	
	discTurmasFile << "\n\nSoma de casos com TURMAS ESGOTADAS e NAO-ATENDIMENTO:";
	discTurmasFile << "\nCom divisao PT = " << soma;
	discTurmasFile << "\nSem divisao PT = " << somaSemDivisaoPT;		
	discTurmasFile << "\n\n======================================================================================";

	soma=0;
	somaSemDivisaoPT=0;

	// difTurmas > 0 e nroNaoAtend > 0	
	discTurmasFile << "\n\nDisciplinas com TURMAS SOBRANDO e NAO-ATENDIMENTO:";
		itMapDiscNaoAtend = this->mapDiscDif.begin();
	for ( ; itMapDiscNaoAtend != this->mapDiscDif.end() ; itMapDiscNaoAtend++ )
	{		
		int discId = itMapDiscNaoAtend->first;
		std::pair<int/*difTurmas*/, int/*nroNaoAtend*/> par = itMapDiscNaoAtend->second;
		int difTurmas = par.first;
		int naoAtend = par.second;
			
		if ( difTurmas > 0 && naoAtend != 0 )
		{
			discTurmasFile << "\nDisc" << discId << ", Turmas Abertas:" 
				<< mapDiscTurmasAbertas[discId] << ", Total de Turmas: " << mapDiscTotalTurmas[discId]
				<< ", Atendido: " << mapDiscNroAtend[discId] << ", Total de AlunoDemandas:" << mapDiscTotalDemanda[discId];
			discTurmasFile << "\t=> NAO Atendido: " << naoAtend;
			soma += naoAtend;
			if ( discId > 0 ) somaSemDivisaoPT += naoAtend;				
		}
	}
	discTurmasFile << "\n\nSoma de casos com TURMAS SOBRANDO e NAO-ATENDIMENTO:";
	discTurmasFile << "\nCom divisao PT = " << soma;
	discTurmasFile << "\nSem divisao PT = " << somaSemDivisaoPT;	
	discTurmasFile << "\n\n======================================================================================";

	soma=0;
	somaSemDivisaoPT=0;

	// difTurmas > 0 e nroNaoAtend = 0
	discTurmasFile << "\n\nDisciplinas com TURMAS SOBRANDO e TOTAL ATENDIMENTO:";
		itMapDiscNaoAtend = this->mapDiscDif.begin();
	for ( ; itMapDiscNaoAtend != this->mapDiscDif.end() ; itMapDiscNaoAtend++ )
	{		
		int discId = itMapDiscNaoAtend->first;
		std::pair<int/*difTurmas*/, int/*nroNaoAtend*/> par = itMapDiscNaoAtend->second;
		int difTurmas = par.first;
		int naoAtend = par.second;
			
		if ( difTurmas > 0 && naoAtend == 0 )
		{
			discTurmasFile << "\nDisc" << discId << ", Turmas Abertas:" 
				<< mapDiscTurmasAbertas[discId] << ", Total de Turmas: " << mapDiscTotalTurmas[discId]
				<< ", Atendido: " << mapDiscNroAtend[discId] << ", Total de AlunoDemandas:" << mapDiscTotalDemanda[discId];
			discTurmasFile << "\t=> NAO Atendido: " << naoAtend;
			soma += naoAtend;
			if ( discId > 0 ) somaSemDivisaoPT += naoAtend;		
		}
	}
	discTurmasFile << "\n\nSoma de casos com TURMAS SOBRANDO e TOTAL ATENDIMENTO:";
	discTurmasFile << "\nCom divisao PT = " << soma;
	discTurmasFile << "\nSem divisao PT = " << somaSemDivisaoPT;	
	discTurmasFile << "\n\n======================================================================================";

	soma=0;
	somaSemDivisaoPT=0;

	// difTurmas = 0 e nroNaoAtend = 0
	discTurmasFile << "\n\nDisciplinas com TURMAS ESGOTADAS e TOTAL ATENDIMENTO:";
		itMapDiscNaoAtend = this->mapDiscDif.begin();
	for ( ; itMapDiscNaoAtend != this->mapDiscDif.end() ; itMapDiscNaoAtend++ )
	{		
		int discId = itMapDiscNaoAtend->first;
		std::pair<int/*difTurmas*/, int/*nroNaoAtend*/> par = itMapDiscNaoAtend->second;
		int difTurmas = par.first;
		int naoAtend = par.second;
			
		if ( difTurmas == 0 && naoAtend == 0 )
		{
			discTurmasFile << "\nDisc" << discId << ", Turmas Abertas:" 
				<< mapDiscTurmasAbertas[discId] << ", Total de Turmas: " << mapDiscTotalTurmas[discId]
				<< ", Atendido: " << mapDiscNroAtend[discId] << ", Total de AlunoDemandas:" << mapDiscTotalDemanda[discId];
			discTurmasFile << "\t=> NAO Atendido: " << naoAtend;
			soma += naoAtend;
			if ( discId > 0 ) somaSemDivisaoPT += naoAtend;		
		}
	}
	discTurmasFile << "\n\nSoma de casos com TURMAS ESGOTADAS e TOTAL ATENDIMENTO:";
	discTurmasFile << "\nCom divisao PT = " << soma;
	discTurmasFile << "\nSem divisao PT = " << somaSemDivisaoPT;

	discTurmasFile.close();

}
   
bool ProblemData::haAlunoFormandoNaoAlocado( Disciplina *disciplina, int cpId, int prioridadeAtual )
{
	ITERA_GGROUP_LESSPTR( itAluno, this->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		if ( ! aluno->ehFormando() )
			continue;

		ITERA_GGROUP_LESSPTR( it1AlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *d = it1AlDemanda->demanda->disciplina;			
			
			if ( it1AlDemanda->getCampus()->getId() != cpId )
				continue;

			if ( it1AlDemanda->getPrioridade() <= prioridadeAtual )
			if ( *d == *disciplina )
			{
				if ( this->retornaTurmaDiscAluno( aluno, d ) == -1 )
					return true;
			}
		}
	}

	return false;
}

bool ProblemData::haAlunoFormandoNaoAlocadoEquiv( Disciplina *disciplina, int cpId, int prioridadeAtual )
{
	ITERA_GGROUP_LESSPTR( itAluno, this->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;
		
		if ( ! aluno->ehFormando() )
			continue;

		ITERA_GGROUP_LESSPTR( it1AlDemanda, aluno->demandas, AlunoDemanda )
		{			
			if ( it1AlDemanda->getCampus()->getId() != cpId )
				continue;

			Disciplina *d = it1AlDemanda->demanda->disciplina;			

			int turma = this->retornaTurmaDiscAluno( aluno, d );

			if ( it1AlDemanda->getPrioridade() <= prioridadeAtual )
			if ( *d == *disciplina && turma == -1 )
			{	
				return true;
			}
			else if ( *d != *disciplina && turma == -1 )
			{
				if ( d->discEquivSubstitutas.find( disciplina ) !=
					 d->discEquivSubstitutas.end() )
				{
					if ( this->alocacaoEquivViavel( it1AlDemanda->demanda, disciplina ) )
						return true;
				}				
			}
		}
	}

	return false;
}

int ProblemData::getNroFolgasDeAtendimento( int prioridade, Disciplina *disciplina, int campusId )
{
	int n = 0;
	GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> alDem = this->retornaDemandasDiscNoCampus( disciplina->getId(), campusId, prioridade );

	ITERA_GGROUP_LESSPTR( itAlDem, alDem, AlunoDemanda )
	{
		int id = (*itAlDem)->getAlunoId();
		Aluno *aluno = this->retornaAluno( id );

		if ( this->retornaTurmaDiscAluno( aluno, disciplina ) == -1 )
			n++;
	}

	return n;
}

Disciplina* ProblemData::getDisciplinaTeorPrat( Disciplina *disciplina )
{
	// Se existir a disciplina teorica/pratica correspondente
	int discId2 = - disciplina->getId();
	std::map< int, Disciplina* >::iterator itMapDisc = this->refDisciplinas.find( discId2 );
	if ( itMapDisc != this->refDisciplinas.end() )				
	{
		return itMapDisc->second;
	}
	return NULL;
}

bool ProblemData::violaMinTurma( int campusId )
{
	bool viola=false;

	if ( !this->parametros->min_alunos_abertura_turmas )
		return false;

	std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
		itMapTurmas = this->mapCampusTurmaDisc_AlunosDemanda.begin();

	for ( ; itMapTurmas != this->mapCampusTurmaDisc_AlunosDemanda.end() ; itMapTurmas++ )
	{		
		int cp = itMapTurmas->first.first;
		int turma = itMapTurmas->first.second;
		int disc = itMapTurmas->first.third->getId();
		Disciplina *disciplina = itMapTurmas->first.third;

		if ( cp != campusId )
			continue;

		int size = itMapTurmas->second.size();
		
		if ( disciplina->eLab() && size >= this->parametros->min_alunos_abertura_turmas_praticas_value )
			continue;
		
		if ( !disciplina->eLab() && size >= this->parametros->min_alunos_abertura_turmas_value )
			continue;

		if ( !this->parametros->violar_min_alunos_turmas_formandos )
		{
			viola=true;
			std::cout<<"\nErro:Turma "<<turma<<" Disc "<<disc
				 <<" possui somente "<<size<<" alunos"; fflush(NULL); continue;
		}
		bool possuiFormando=false;
		GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > *alunosDemanda = &itMapTurmas->second;
		ITERA_GGROUP_LESSPTR( itAlDem, *alunosDemanda, AlunoDemanda )
		{
			Aluno *aluno = this->retornaAluno( (*itAlDem)->getAlunoId() );
			if ( aluno->ehFormando() )
			{
				possuiFormando=true; break;
			}
		}

		if ( !possuiFormando )
		{   viola=true;
			std::cout<<"\nErro:Turma "<<turma<<" Disc "<<disc
				<<" possui somente "<<size<<" alunos e sem formandos"; fflush(NULL); 
		}
	}

	return viola;
}
   
void ProblemData::imprimeFormandos()
{
	if ( this->parametros->otimizarPor != "ALUNO" )
		return;

	#ifndef PRINT_LOGS
		return;
	#endif

	ofstream formandosFile;
	std::string formandosFilename( "alunosFormandos" );
	formandosFilename += this->inputFileName;
	formandosFilename += ".txt";
	formandosFile.open(formandosFilename, ios::out);
	if (!formandosFile)
	{
		cerr << "Error: Can't open output file " << formandosFilename << endl;
		return;
	}

	int totalAlunos = 0, totalFormandos = 0;
	
	ITERA_GGROUP_LESSPTR( itAluno, alunos, Aluno )
	{		
		if ( itAluno->ehFormando() )
		{
			formandosFile << "\nAluno " << itAluno->getAlunoId() << "  " << itAluno->getNomeAluno();
			totalFormandos++;
		}
				
		totalAlunos++;
	}
	
	formandosFile << "\n\nTotal de alunos: " << totalAlunos;
	formandosFile << "\nTotal de formandos: " << totalFormandos;

	formandosFile.close();
}

void ProblemData::imprimeCombinacaoCredsDisciplinas()
{
	#ifndef PRINT_LOGS
		return;
	#endif

	ofstream divCredFile;
	std::string divFilename( "divCredsDisciplinas_input" );
	divFilename += this->inputFileName;
	divFilename += ".txt";
	divCredFile.open(divFilename, ios::out);
	if (!divCredFile)
	{
		cerr << "Error: Can't open output file " << divFilename << endl;
		return;
	}

    ITERA_GGROUP_LESSPTR( itDisc, this->disciplinas, Disciplina )
    {
	   Disciplina *disciplina = *itDisc;

	   if ( disciplina->divisao_creditos.size() != 0 )
	   {
		   divCredFile << "\n\nDisciplina " << disciplina->getId() << ":";
	
		   for ( int k = 0; k < (int)disciplina->combinacao_divisao_creditos.size(); k++ )
		   {	
			   divCredFile << "\nk = " << k << ":  ";
	           ITERA_GGROUP_N_PT( itDiasLetDisc, disciplina->diasLetivos, int )
		       {
				   int dia = *itDiasLetDisc;
				   
				   // N{d,k,t}
				   int numCreditos = disciplina->getNroCredsRegraDiv(k,dia);  // N{d,k,t}
				   divCredFile << dia << "->" << numCreditos << "  ";
			   }
		   }
	   }
	   else divCredFile << "\n\nDisciplina " << disciplina->getId() << ":  sem regra de divisao de creditos.";
    }

	divCredFile.close();
}


void ProblemData::confereExcessoP2( int campusId )
{
	std::cout<<"\nConferindo excesso de p2...\n";

	if ( ! this->parametros->utilizarDemandasP2 )
		return;

	ofstream excessosP2File;
	std::string excessosP2Filename( "excessosP2_" );
	excessosP2Filename += this->inputFileName;
	excessosP2Filename += ".txt";
	excessosP2File.open(excessosP2Filename, ios::app);
	if (!excessosP2File)
	{
		cerr << "Error: Can't open output file " << excessosP2Filename << endl;
		return;
	}

	excessosP2File <<"-----------------------------------------------------------------------------------------\n";
	excessosP2File <<"Aluno / Id \t\t\t Carga horaria semanal excedida (em minutos)\n";

	ITERA_GGROUP_LESSPTR( itAluno, alunos, Aluno )
	{		
		Aluno *a = (*itAluno);
		if ( a->hasCampusId( campusId ) )
		{
			double chr_p1 = a->getCargaHorariaOrigRequeridaP1();
			double cha = this->cargaHorariaJaAtendida( a );		
			double excesso = cha - chr_p1;
			if ( excesso )
			{
				excessosP2File <<"\n" << a->getNomeAluno() << " / " << a->getAlunoId() << "\t\t\t\t" << excesso;
			}
		}
	}

	excessosP2File.close();
}

void ProblemData::imprimeResumoDemandasPorAluno()
{
	if ( this->parametros->otimizarPor != "ALUNO" )
		return;
	
	#ifndef PRINT_LOGS
		return;
	#endif

	map< Aluno*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Aluno> > mapAlunoAlsDem;

	ITERA_GGROUP_LESSPTR( itAlunoDemanda, alunosDemandaTotal, AlunoDemanda )
	{
		mapAlunoAlsDem[ itAlunoDemanda->getAluno() ].add( *itAlunoDemanda );
	}

	ofstream demandasFile;
	std::string demandasFilename( "demandasAlunos_" );
	demandasFilename += this->inputFileName;
	demandasFilename += ".txt";
	demandasFile.open(demandasFilename, ios::out);
	if (!demandasFile)
	{
		cerr << "Error: Can't open output file " << demandasFilename << endl;
		return;
	}
	demandasFile <<"Demandas por aluno:";
		
	for ( auto itAluno = mapAlunoAlsDem.begin(); itAluno != mapAlunoAlsDem.end(); itAluno++ )
	{
		Aluno *a = itAluno->first;
		
		double tempoTotal=0.0;

		demandasFile <<"\n\n" << a->getNomeAluno() << "\t" << a->getAlunoId();
		
		ITERA_GGROUP_LESSPTR( itAlunoDemanda, a->demandas, AlunoDemanda )
		{
			demandasFile << "\nDiscId " << (*itAlunoDemanda)->demanda->getDisciplinaId() 
				<< ", AlunoDemandaId " << (*itAlunoDemanda)->getId();

			if ( (*itAlunoDemanda)->getExigeEquivalenciaForcada() )
				demandasFile << " (equivForcada)";
			
			demandasFile << ", DemandaId " << (*itAlunoDemanda)->getDemandaId()
				<< ", Prioridade " << (*itAlunoDemanda)->getPrioridade()
				<< ", " << (*itAlunoDemanda)->demanda->disciplina->getTotalCreditos() << " creditos";
			
			demandasFile <<"\tOft" << itAlunoDemanda->getOferta()->getId() 
				<< ", Curr" << itAlunoDemanda->getOferta()->getCurriculoId()
				<< ", Calend" << itAlunoDemanda->demanda->getCalendario()->getId()
				<< ", Turno" << itAlunoDemanda->demanda->getTurnoIES()->getId();

			tempoTotal += (*itAlunoDemanda)->demanda->disciplina->getTotalTempo();

			if ( this->parametros->considerar_equivalencia_por_aluno )
			{
				bool firstTime=true;
				ITERA_GGROUP_LESSPTR( itEquiv, this->equivalencias, Equivalencia )
				{
					if ( itEquiv->getDisciplinaEliminaId() == (*itAlunoDemanda)->demanda->getDisciplinaId() )
					{
						Disciplina *discEquiv = this->refDisciplinas[ itEquiv->getDisciplinaCursouId() ];
						if ( this->alocacaoEquivViavel( (*itAlunoDemanda)->demanda, discEquiv ) )
						{
							if (firstTime) demandasFile <<"\tSubstitutas por equiv: ";
							firstTime=false;

							demandasFile <<"DiscId " << itEquiv->getDisciplinaCursouId() << "  ";
					
							if ( a->demandaDisciplina( itEquiv->getDisciplinaCursouId() ) )
							{
								std::cout<<"\nATENCAO: Aluno "<< a->getAlunoId() 
									<< " possui demanda por duas disciplinas equivalencias."
									<< "\nDiscId " << itEquiv->getDisciplinaEliminaId()
									<< " original e discId " << itEquiv->getDisciplinaCursouId() << " substituta";
							}
						}
					}
				}
			}				
		}		
		demandasFile <<"\nTempo total requerido: "<< tempoTotal<< endl;

		ITERA_GGROUP_LESSPTR( itAlunoDemanda, itAluno->second, AlunoDemanda )
		{			
			if ( a->demandas.find( (*itAlunoDemanda) ) != a->demandas.end() )
				continue;

			demandasFile <<"\nDiscId " << (*itAlunoDemanda)->demanda->getDisciplinaId()
				<< ", AlunoDemandaId " << (*itAlunoDemanda)->getId();
			
			if ( (*itAlunoDemanda)->getExigeEquivalenciaForcada() )
				demandasFile << " (equivForcada)";
			
			demandasFile << ", DemandaId " << (*itAlunoDemanda)->getDemandaId()
				<< ", Prioridade " << (*itAlunoDemanda)->getPrioridade()
				<< ", " << (*itAlunoDemanda)->demanda->disciplina->getTotalCreditos() << " creditos";

			if ( this->parametros->considerar_equivalencia_por_aluno &&
				(*itAlunoDemanda)->demanda->disciplina->discEquivSubstitutas.size() > 0 )
			{
				demandasFile <<"\tPossiveis substitutas por equiv: ";
				ITERA_GGROUP_LESSPTR( itDiscEquiv, (*itAlunoDemanda)->demanda->disciplina->discEquivSubstitutas, Disciplina )
					if ( this->alocacaoEquivViavel( itAlunoDemanda->demanda, *itDiscEquiv ) )	
						demandasFile <<"DiscId " << (*itDiscEquiv)->getId() << "  "; 
			}
		}
	}

	demandasFile.close();
}

void ProblemData::imprimeResumoDemandasPorAlunoPosEquiv()
{
	if ( this->parametros->otimizarPor != "ALUNO" )
		return;

	#ifndef PRINT_LOGS
		return;
	#endif

	ofstream demandasFile;
	std::string demandasFilename( "demandasAlunosPosEquiv_" );
	demandasFilename += this->inputFileName;
	demandasFilename += ".txt";
	demandasFile.open(demandasFilename, ios::out);
	if (!demandasFile)
	{
		cerr << "Error: Can't open output file " << demandasFilename << endl;
		return;
	}

	demandasFile <<"Demandas por aluno:";

	ITERA_GGROUP_LESSPTR( itAluno, alunos, Aluno )
	{		
		Aluno *a = (*itAluno);
		
		double tempoTotal=0.0;

		demandasFile <<"\n\n" << a->getNomeAluno() << "\t" << a->getAlunoId();
		
		ITERA_GGROUP_LESSPTR( itAlunoDemanda, a->demandas, AlunoDemanda )
		{
			AlunoDemanda *alDem = (*itAlunoDemanda);

			demandasFile <<"\nDiscId " << alDem->demanda->getDisciplinaId() 
				<< ", AlunoDemandaId " << alDem->getId();
			
			if ( (*itAlunoDemanda)->getExigeEquivalenciaForcada() )
				demandasFile << " (equivForcada)";

			demandasFile << ", DemandaId " << alDem->getDemandaId()
				<< ", Prioridade " << alDem->getPrioridade()
				<< ", " << alDem->demanda->disciplina->getTotalCreditos() << " creditos";

			demandasFile <<"\tOft" << alDem->getOferta()->getId()
				<< ", Curr" << alDem->getOferta()->getCurriculoId()
				<< ", Calend" << alDem->demanda->getCalendario()->getId();

			tempoTotal += (*itAlunoDemanda)->demanda->disciplina->getTotalTempo();
			
			if ( alDem->demandaOriginal!=NULL )
				demandasFile <<"\tDemanda Original: " << alDem->demandaOriginal->getId() << " Disc: " << alDem->demandaOriginal->getDisciplinaId();
			else if ( this->parametros->considerar_equivalencia_por_aluno &&
				      (*itAlunoDemanda)->demanda->disciplina->discEquivSubstitutas.size() > 0 )
			{
				demandasFile <<"\tPossiveis substitutas por equiv: ";
				ITERA_GGROUP_LESSPTR( itDiscEquiv, (*itAlunoDemanda)->demanda->disciplina->discEquivSubstitutas, Disciplina )
					if ( this->alocacaoEquivViavel( itAlunoDemanda->demanda, *itDiscEquiv ) )	
						demandasFile <<"DiscId " << (*itDiscEquiv)->getId() << "  ";
			}

		}		
		demandasFile <<"\nTempo total requerido: "<< tempoTotal<< endl;

		ITERA_GGROUP_LESSPTR( itAlunoDemanda, alunosDemandaTotal, AlunoDemanda )
		{				
			if ( (*itAlunoDemanda)->getAlunoId() != a->getAlunoId() )
				continue;
			
			if ( a->demandas.find( (*itAlunoDemanda) ) != a->demandas.end() )
				continue;

			demandasFile <<"\nDiscId " << (*itAlunoDemanda)->demanda->getDisciplinaId()
				<< ", AlunoDemandaId " << (*itAlunoDemanda)->getId();
			
			if ( (*itAlunoDemanda)->getExigeEquivalenciaForcada() )
				demandasFile << " (equivForcada)";

			demandasFile << ", DemandaId " << (*itAlunoDemanda)->getDemandaId()
				<< ", Prioridade " << (*itAlunoDemanda)->getPrioridade()
				<< ", " << (*itAlunoDemanda)->demanda->disciplina->getTotalCreditos() << " creditos";

			if ( this->parametros->considerar_equivalencia_por_aluno &&
				(*itAlunoDemanda)->demanda->disciplina->discEquivSubstitutas.size() > 0 )
			{
				demandasFile <<"\tPossiveis substitutas por equiv: ";
				ITERA_GGROUP_LESSPTR( itDiscEquiv, (*itAlunoDemanda)->demanda->disciplina->discEquivSubstitutas, Disciplina )	
					if ( this->alocacaoEquivViavel( itAlunoDemanda->demanda, *itDiscEquiv ) )	
						demandasFile <<"DiscId " << (*itDiscEquiv)->getId() << "  "; 
			}
		}
	}

	demandasFile.close();
}

void ProblemData::preencheMapDisciplinaAlunosDemanda( bool EQUIVALENCIA )
{
	this->mapDisciplinaAlunosDemanda.clear();

#ifdef EQUIVALENCIA_DESDE_O_INICIO
	ITERA_GGROUP_LESSPTR( itAlDem, this->alunosDemanda, AlunoDemanda )
	{
		AlunoDemanda *alunoDemanda = (*itAlDem);

		Aluno *aluno = this->retornaAluno( alunoDemanda->getAlunoId() );

		int campusId = alunoDemanda->demanda->oferta->getCampusId();
		Disciplina *disciplina = alunoDemanda->demanda->disciplina;		
		int p = alunoDemanda->getPrioridade();
		Calendario *calendario = alunoDemanda->demanda->getCalendario();
		TurnoIES* turnoIES = alunoDemanda->demanda->getTurnoIES();

		GGroup<Disciplina*, LessPtr<Disciplina>> disciplinasPorAlDem;				
		disciplinasPorAlDem.add( disciplina );
		
		if ( EQUIVALENCIA )
		{
			int turmaAluno = this->retornaTurmaDiscAluno( aluno, disciplina );		
			if ( turmaAluno == -1 && this->parametros->considerar_equivalencia_por_aluno ) // aluno não alocado
			{
				ITERA_GGROUP_LESSPTR( itDisc, disciplina->discEquivSubstitutas, Disciplina )
				{
					if ( this->alocacaoEquivViavel( (*itAlDem)->demanda, *itDisc ) )
						disciplinasPorAlDem.add( *itDisc );
				}
			}
		}

		ITERA_GGROUP_LESSPTR( itDisc, disciplinasPorAlDem, Disciplina )
		{
			this->mapDisciplinaAlunosDemanda[campusId][*itDisc][p].add( alunoDemanda );
		}
	}

#else
	ITERA_GGROUP_LESSPTR( itAlDem, this->alunosDemanda, AlunoDemanda )
	{
		AlunoDemanda *alunoDemanda = (*itAlDem);

		Aluno *aluno = this->retornaAluno( alunoDemanda->getAlunoId() );

		int campusId = alunoDemanda->demanda->oferta->getCampusId();
		Disciplina *disciplina = alunoDemanda->demanda->disciplina;		
		int p = alunoDemanda->getPrioridade();
		Calendario *calendario = aluno->getCalendario();		
		TurnoIES* turnoIES = alunoDemanda->demanda->getTurnoIES();

		this->mapDisciplinaAlunosDemanda[campusId][disciplina][p].add( alunoDemanda );
	}
#endif

}


// Se remover AlunoDemanda no meio do processamento,
// remove referencia para o objeto em todos os maps necessarios
void ProblemData::removeAlunoDemanda( AlunoDemanda *alunoDemanda )
{	
	int campusId = alunoDemanda->demanda->oferta->getCampusId();
	Disciplina *disciplina = alunoDemanda->demanda->disciplina;		
	int p = alunoDemanda->getPrioridade();
	Demanda *demanda = alunoDemanda->demanda;
	Aluno *aluno = this->retornaAluno( alunoDemanda->getAlunoId() );
	int cjtAlunoId = this->retornaCjtAlunosId( aluno );
	if ( cjtAlunoId==0 ) std::cout<<"\nERRO2: nao achei cjtAlunoId, muito estranho!\n";
	
	if ( this->mapDemandaAlunos[demanda].find( alunoDemanda ) != this->mapDemandaAlunos[demanda].end() )
	{
		demanda->setQuantidade( demanda->getQuantidade() - 1 );
		this->mapDemandaAlunos[ demanda ].remove( alunoDemanda );
		if ( this->mapDemandaAlunos[demanda ].size() == 0 )
			this->mapDemandaAlunos.erase( demanda );
	}

	this->listSlackDemandaAluno.remove( alunoDemanda );
	this->mapDisciplinaAlunosDemanda[campusId][disciplina][p].remove( alunoDemanda );
	this->cjtAlunoDemanda[cjtAlunoId].remove( alunoDemanda );				
	aluno->demandas.remove( alunoDemanda );
	this->alunosDemanda.remove( alunoDemanda );
}

GGroup<HorarioDia*, LessPtr<HorarioDia>> ProblemData::retornaHorariosDiaTurnoComum( GGroup<Calendario*,LessPtr<Calendario>> &calends )
{
	GGroup<HorarioDia*, LessPtr<HorarioDia>> horariosDiaTurnosComuns;	

	if ( calends.size() == 0 ) return horariosDiaTurnosComuns;

	std::map< int/*turno*/, std::map< int/*dia*/, std::map< Calendario*, 
		GGroup<HorarioDia*, LessPtr<HorarioDia>>, LessPtr<Calendario> > > > mapTurnoDiaCalend;
	
	ITERA_GGROUP_LESSPTR( itCalend, calends, Calendario )
	{	
		GGroup<int> turnos = itCalend->getTurnos();
		ITERA_GGROUP_N_PT( itTurno, turnos, int )
		{
			int turno = *itTurno;

			std::map< int/*dia*/, std::map< DateTime, HorarioAula* > >::iterator 
				itMapDia = itCalend->mapDiaDateTime.begin();
			for ( ; itMapDia!=itCalend->mapDiaDateTime.end(); itMapDia++ )
			{
				int dia = itMapDia->first;

				std::map< DateTime, HorarioAula* >::iterator 
					itMapDT = itMapDia->second.begin();
				for ( ; itMapDT != itMapDia->second.end(); itMapDT++ )
				{
					if ( this->getFaseDoDia( itMapDT->second->getInicio() ) == turno )
					{
						HorarioDia *hd = this->getHorarioDiaCorrespondente( itMapDT->second, dia );
						mapTurnoDiaCalend[turno][dia][*itCalend].add(hd);					
					}
				}
			}			
		}
	}

	std::map< int/*turno*/, std::map< int/*dia*/, std::map< Calendario*, 
		GGroup<HorarioDia*, LessPtr<HorarioDia>>, LessPtr<Calendario> > > >::iterator
		itMap = mapTurnoDiaCalend.begin();	
	for ( ; itMap != mapTurnoDiaCalend.end(); itMap++ )
	{
		std::map< int/*dia*/, std::map< Calendario*, 
		GGroup<HorarioDia*, LessPtr<HorarioDia>>, LessPtr<Calendario> > > diaCalendSTurno = itMap->second;
		std::map< int/*dia*/, std::map< Calendario*, 
		GGroup<HorarioDia*, LessPtr<HorarioDia>>, LessPtr<Calendario> > >::iterator
			itMapDia = diaCalendSTurno.begin();
		for ( ; itMapDia != diaCalendSTurno.end(); itMapDia++ )
		{
			int dia = itMapDia->first;
			std::map< Calendario*, GGroup<HorarioDia*, LessPtr<HorarioDia>>, LessPtr<Calendario> > 
				mapCalends = itMapDia->second;
			if ( mapCalends.size() == calends.size() ) // TurnoIES presente em todos os calendarios no dia
			{
				std::map< Calendario*, GGroup<HorarioDia*, LessPtr<HorarioDia>>, LessPtr<Calendario> >::iterator
					itMapCalends = mapCalends.begin();
				for ( ; itMapCalends != mapCalends.end(); itMapCalends++ )
				{				
					ITERA_GGROUP_LESSPTR( itHorDia, itMapCalends->second, HorarioDia )
					{	
						horariosDiaTurnosComuns.add(*itHorDia);						
					}
				}
			}
		}
	}
	
	return horariosDiaTurnosComuns;
}

GGroup<HorarioDia*, LessPtr<HorarioDia>> ProblemData::retornaHorariosDiaComuns( GGroup<TurnoIES*,LessPtr<TurnoIES>> &turnosIES )
{
	GGroup<HorarioDia*, LessPtr<HorarioDia>> horariosDiaComuns;

	if ( turnosIES.size() == 0 ) return horariosDiaComuns;

	// Copia horariosDia do primeiro turno da lista
	ITERA_GGROUP_LESSPTR( itHorAula, turnosIES.begin()->horarios_aula, HorarioAula )
	{	
		ITERA_GGROUP_N_PT( itDia, itHorAula->dias_semana, int )
		{
			HorarioDia *hd = this->getHorarioDiaCorrespondente( *itHorAula, *itDia );
			horariosDiaComuns.add(hd);
		}
	}

	// Remove os horariosDia que não são comuns e adiciona os analogos
	ITERA_GGROUP_LESSPTR( itTurnoIES, turnosIES, TurnoIES )
	{
		if ( itTurnoIES == turnosIES.begin() ) continue;
		
		GGroup<HorarioDia*, LessPtr<HorarioDia>> remover;
		GGroup<HorarioDia*, LessPtr<HorarioDia>> adicionar;

		ITERA_GGROUP_LESSPTR( itHorDia, horariosDiaComuns, HorarioDia )
		{		
			GGroup<HorarioAula*, LessPtr<HorarioAula>> 
				has = (*itTurnoIES)->retornaHorarioDiaOuCorrespondente( (*itHorDia)->getHorarioAula(), (*itHorDia)->getDia() );
			if ( has.size() == 0 )
			{
				remover.add(*itHorDia);
			}
			else
			{
				ITERA_GGROUP_LESSPTR( itHorAula, has, HorarioAula )
				{
					adicionar.add( this->getHorarioDiaCorrespondente( (*itHorAula), (*itHorDia)->getDia()) );
				}
			}
		}

		ITERA_GGROUP_LESSPTR( itHorDia, remover, HorarioDia )
			horariosDiaComuns.remove( *itHorDia );

		ITERA_GGROUP_LESSPTR( itHorDia, adicionar, HorarioDia )
			horariosDiaComuns.add( *itHorDia );

		if ( horariosDiaComuns.size()==0 ) break;
	}

	return horariosDiaComuns;
}

void ProblemData::clearMaps( int cpIdAtual, int prioridade )
{		
	// -----------------------------------------------------------------------------------
	// Clear itens of mapAluno_CampusTurmaDisc which are from the current campus
	
	typedef std::map< Aluno*, GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >, 
		LessPtr< Aluno > >::iterator MapAlunoIterator;
	typedef GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >::iterator GGroupTrioIterator;

	GGroup< Aluno*, LessPtr<Aluno> > toRemoveAluno;

	MapAlunoIterator itMapAl = this->mapAluno_CampusTurmaDisc.begin();
	for ( ; itMapAl != this->mapAluno_CampusTurmaDisc.end(); itMapAl++ )
	{				
		GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > > toRemove;

		GGroupTrioIterator itGGroup = itMapAl->second.begin();
		for ( ; itGGroup != itMapAl->second.end(); itGGroup++ )
		{
			int cpId = (*itGGroup).first;
			if ( cpId == cpIdAtual )
			{
				toRemove.add( (*itGGroup) );
			}
		}

		GGroupTrioIterator itRemove = toRemove.begin();
		for ( ; itRemove != toRemove.end(); itRemove++ )
		{
			itMapAl->second.remove( *itRemove );
		}

		if ( itMapAl->second.size() == 0 )
		{
			toRemoveAluno.add( itMapAl->first );
		}	
	}
	
	GGroup< Aluno*, LessPtr<Aluno> >::iterator
		itRemoveAluno = toRemoveAluno.begin();
	for ( ; itRemoveAluno != toRemoveAluno.end(); itRemoveAluno++ )
	{
		this->mapAluno_CampusTurmaDisc.erase( *itRemoveAluno );
	}

	// -----------------------------------------------------------------------------------
	// Clear itens of mapCampusTurmaDisc_AlunosDemanda which are from the current campus
	
	typedef std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >,
		GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator MapTrioIterator;

	GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > > toRemoveTrios;

	MapTrioIterator itTrio = this->mapCampusTurmaDisc_AlunosDemanda.begin();
	for ( ; itTrio != this->mapCampusTurmaDisc_AlunosDemanda.end(); itTrio++ )
	{
		int cpId = itTrio->first.first;
		if ( cpId == cpIdAtual )
		{
			toRemoveTrios.add( itTrio->first );			
		}
	}

	GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >::iterator itRemoveTrio = toRemoveTrios.begin();
	for ( ; itRemoveTrio != toRemoveTrios.end(); itRemoveTrio++ )
	{
		this->mapCampusTurmaDisc_AlunosDemanda.erase( *itRemoveTrio );
	}

}

void ProblemData::preencheMapsTurmasCalendarios()
{
	std::cout << "Preenchendo maps de turmas/calendarios..." << endl;

	mapTurma_Calendarios.clear();
	mapTurma_HorComunsAosCalendarios.clear();

	// Associa quais os calendarios podem ser usados na alocação da turma. Necessário caso haja alunos
    // na turma pertencentes a ofertas de calendarios diferentes
	
	std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
		itMapTurmas = this->mapCampusTurmaDisc_AlunosDemanda.begin();
	for ( ; itMapTurmas != this->mapCampusTurmaDisc_AlunosDemanda.end(); itMapTurmas++ )
	{				
		Disciplina *disciplina = itMapTurmas->first.third;

		Trio< int /*campusId*/, int /*turma*/, int /*discId*/ > trio;
		trio.set( itMapTurmas->first.first, itMapTurmas->first.second, disciplina->getId() );

		GGroup< Calendario*, LessPtr<Calendario> > calendsTrio;
		GGroup< HorarioDia*, LessPtr<HorarioDia> > horDiaTrio;
				
		
		if ( itMapTurmas->first.first == 22 && itMapTurmas->first.second == 0 && disciplina->getId() == 6462 )
		{
			//DEBUGANDO = true;
			//std::cout<<"\n\n\n--------\nPREENCHENDO MAP\n-------------------\nDEBUGANDO = true\n";
		}

		GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > alunosDemanda = itMapTurmas->second;
		ITERA_GGROUP_LESSPTR( itAlDem, alunosDemanda, AlunoDemanda )
		{			
			calendsTrio.add( itAlDem->demanda->getCalendario() ); // Calendario da demanda

			if ( DEBUGANDO )
			{
				std::cout<< "\nAlunoDemanda " << itAlDem->getId()
					<<	" com Demanda"<< itAlDem->demanda->getId() << "com calendario "
					<< itAlDem->demanda->getCalendario()->getId();
			}
		}
				
		GGroup<HorarioDia*, LessPtr<HorarioDia>> horariosDiaComuns = this->retornaHorariosDiaTurnoComum( calendsTrio );
				
		// Adiciona horários dos calendarios da Disciplina que estejam em mesmo turno/dia que os comuns aos alunos.
		GGroup<Calendario*, LessPtr<Calendario>> discCalends = disciplina->getCalendarios();
		ITERA_GGROUP_LESSPTR( itCalend, discCalends, Calendario )
		{
			Calendario * calendario = *itCalend;

			std::map< int/*dia*/, std::map< DateTime, HorarioAula* > > map1 = calendario->mapDiaDateTime;
			std::map< int/*dia*/, std::map< DateTime, HorarioAula* > >::iterator itMap1 = map1.begin();
			for ( ; itMap1 != map1.end(); itMap1++ )
			{
				// Dia do calendario da disciplina
				int dia = itMap1->first;
				std::map< DateTime, HorarioAula* > map2 = itMap1->second;
				std::map< DateTime, HorarioAula* >::iterator itMap2 = map2.begin();
				for ( ; itMap2 != map2.end(); itMap2++ )
				{
					// Horario da disciplina
					HorarioAula *ha = itMap2->second;
					HorarioDia *hd = this->getHorarioDiaCorrespondente( ha, dia );					
					if ( horariosDiaComuns.find( hd ) == horariosDiaComuns.end() )
					{
						ITERA_GGROUP_LESSPTR( ithd, horariosDiaComuns, HorarioDia )
						{
							if ( ithd->getDia() == dia )
							{
								int turnoDisc = getFaseDoDia( itMap2->first );
								int turnoAlunos = getFaseDoDia( ithd->getHorarioAula()->getInicio() );
								if ( turnoDisc == turnoAlunos )
								{
									horariosDiaComuns.add( hd ); break;
								}
							}
						}
					}
				}
			}
		}

		if ( DEBUGANDO )
		{
			std::cout<<"\nHorariosDia:";
			ITERA_GGROUP_LESSPTR( ithd, horariosDiaComuns, HorarioDia )
			{	
				std::cout<< "\nH = " << ithd->getHorarioAulaId() << "   hi = " << ithd->getHorarioAula()->getInicio()
					<<  "  Dia = " << ithd->getDia();
			}

			std::cout<<"\n\nHorariosDia no credito da disciplina:";
			ITERA_GGROUP_LESSPTR( ithd, horariosDiaComuns, HorarioDia )
			{	
				if ( ithd->getHorarioAula()->getTempoAula() == itMapTurmas->first.third->getTempoCredSemanaLetiva() )
				std::cout<< "\nH = " << ithd->getHorarioAulaId() << "   hi = " << ithd->getHorarioAula()->getInicio()
					<<  "  Dia = " << ithd->getDia();
			}
		}

		if ( horariosDiaComuns.size() == 0 && alunosDemanda.size() != 0 )
		{
			std::cout<<"\nErro em ProblemData::preencheMapsTurmasCalendarios(), trio (turma,disciplina,campus) = ("
				<< trio.second <<"," << trio.third <<"," << trio.first << ") sem horario em comum nos calendarios\n";
			std::cout<<"\nCalendarios:";
			ITERA_GGROUP_LESSPTR( itCalend, calendsTrio, Calendario )
			{				
				std::cout<<" Id " << itCalend->getId()<<";";
			}
		}
		
		mapTurma_Calendarios[trio].add( calendsTrio );
		mapTurma_HorComunsAosCalendarios[trio].add( horariosDiaComuns );
	}
}

void ProblemData::preencheMapsTurmasTurnosIES()
{
	std::cout << "Preenchendo maps de turmas/turnosIES..." << endl;

	mapTurma_TurnosIES.clear();
	mapTurma_HorComunsAosTurnosIES.clear();
	
	// Associa quais os turnosIES podem ser usados na alocação da turma. Necessário caso haja alunos
    // na turma pertencentes a ofertas de turnos diferentes
	
	std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
		itMapTurmas = this->mapCampusTurmaDisc_AlunosDemanda.begin();
	for ( ; itMapTurmas != this->mapCampusTurmaDisc_AlunosDemanda.end(); itMapTurmas++ )
	{				
		Disciplina *disciplina = itMapTurmas->first.third;

		Trio< int /*campusId*/, int /*turma*/, int /*discId*/ > trio;
		trio.set( itMapTurmas->first.first, itMapTurmas->first.second, disciplina->getId() );

		GGroup< TurnoIES*, LessPtr<TurnoIES> > turnosIESTrio;
		GGroup< HorarioDia*, LessPtr<HorarioDia> > horDiaTrio;				
		
		if ( itMapTurmas->first.first == 49 && itMapTurmas->first.second == 1 && disciplina->getId() == 13539 )
		{
			//DEBUGANDO = true;
			//std::cout<<"\n\n\n--------\nPREENCHENDO MAP\n-------------------\nDEBUGANDO = true\n";
		}

		GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > alunosDemanda = itMapTurmas->second;
		ITERA_GGROUP_LESSPTR( itAlDem, alunosDemanda, AlunoDemanda )
		{			
			turnosIESTrio.add( itAlDem->demanda->getTurnoIES() ); // TurnoIES da demanda

			if ( DEBUGANDO )
			{
				std::cout<< "\nAlunoDemanda " << itAlDem->getId()
					<<	" com Demanda"<< itAlDem->demanda->getId() << "com turno "
					<< itAlDem->demanda->getTurnoIES()->getId();
			}
		}

		GGroup<HorarioDia*, LessPtr<HorarioDia>> remover;
		GGroup<HorarioDia*, LessPtr<HorarioDia>> horariosDiaComuns = this->retornaHorariosDiaComuns( turnosIESTrio );

		if ( DEBUGANDO )
			std::cout<< "\nHors comuns dos turnos: ";

		ITERA_GGROUP_LESSPTR( ithd, horariosDiaComuns, HorarioDia )
		{
			if ( DEBUGANDO )
			{
				std::cout<< "\nhd " << ithd->getId()
					<<	" "<< ithd->getHorarioAula()->getInicio() << " dia "<< ithd->getDia()  << "com turno "
					<< ithd->getHorarioAula()->getTurnoIESId();
			}

			if ( ! disciplina->possuiHorarioDiaOuAnalogo( ithd->getDia(), ithd->getHorarioAula() ) )
			{
				if ( DEBUGANDO )
					std::cout<< "   disc nao possui! Mas a remocao esta comentada por bambiarra";

				//remover.add( *ithd );
			}
		}

		// Remove horários que não estão na Disciplina		
		ITERA_GGROUP_LESSPTR( itRemove, remover, HorarioDia )
		{
			horariosDiaComuns.remove( *itRemove );
		}


		if ( DEBUGANDO )
		{
			std::cout<<"\nHorariosDia:";
			ITERA_GGROUP_LESSPTR( ithd, horariosDiaComuns, HorarioDia )
			{	
				std::cout<< "\nH = " << ithd->getHorarioAulaId() << "   hi = " << ithd->getHorarioAula()->getInicio()
					<<  "  Dia = " << ithd->getDia();
			}

			std::cout<<"\n\nHorariosDia no credito da disciplina:";
			ITERA_GGROUP_LESSPTR( ithd, horariosDiaComuns, HorarioDia )
			{	
				if ( ithd->getHorarioAula()->getTempoAula() == itMapTurmas->first.third->getTempoCredSemanaLetiva() )
				std::cout<< "\nH = " << ithd->getHorarioAulaId() << "   hi = " << ithd->getHorarioAula()->getInicio()
					<<  "  Dia = " << ithd->getDia();
			}
		}

		if ( horariosDiaComuns.size() == 0 && alunosDemanda.size() != 0 )
		{
			std::cout<<"\nErro em ProblemData::preencheMapsTurmasTurnosIES(), trio (turma,disciplina,campus) = ("
				<< trio.second <<"," << trio.third <<"," << trio.first << ") sem horario em comum nos calendarios\n";
			std::cout<<"\nTurnosIES:";
			ITERA_GGROUP_LESSPTR( itT, turnosIESTrio, TurnoIES )
			{				
				std::cout<<" Id " << itT->getId()<<";";
			}
		}
		
		mapTurma_TurnosIES[trio].add( turnosIESTrio );
		mapTurma_HorComunsAosTurnosIES[trio].add( horariosDiaComuns );
	}
}

void ProblemData::preencheMapParCalendHorariosDiaComuns()
{
	ITERA_GGROUP_LESSPTR( itCalend1, this->calendarios, Calendario )
	{		
		ITERA_GGROUP_LESSPTR( itHorAula, itCalend1->horarios_aula, HorarioAula )
		{
			HorarioAula *ha1 = *itHorAula;

			ITERA_GGROUP_N_PT( itDia, itHorAula->dias_semana, int )
			{
				int dia = *itDia;
				HorarioDia *hd1 = this->getHorarioDiaCorrespondente(ha1, dia);

				ITERA_GGROUP_INIC_LESSPTR( itCalend2, itCalend1, this->calendarios, Calendario )
				{
					HorarioAula *ha2 = (*itCalend2)->possuiHorarioDiaOuCorrespondente( ha1, dia );
					if ( ha2 != NULL )
					{
						HorarioDia *hd2 = this->getHorarioDiaCorrespondente(ha2, dia);
						std::pair<Calendario*,Calendario*> par_calend( *itCalend1,*itCalend2 );						
						map_ParCalend_HorariosDiaComuns[par_calend].add(hd1);
						map_ParCalend_HorariosDiaComuns[par_calend].add(hd2);
					}
				}
			}
		}
	}
}

void ProblemData::imprimeAssociacaoDisciplinaSala()
{
	#ifndef PRINT_LOGS
		return;
	#endif

	ofstream associacaoFile;
	std::string associacaoFilename( "associacaoDiscSalas_" );
	associacaoFilename += this->inputFileName;
	associacaoFilename += ".txt";
	associacaoFile.open(associacaoFilename, ios::out);
	if (!associacaoFile)
	{
		cerr << "Error: Can't open output file " << associacaoFilename << endl;
		return;
	}

	associacaoFile <<"Associacao Disciplinas-Salas:";
	
	std::map< Disciplina *, GGroup< Sala *, LessPtr< Sala > >, LessPtr< Disciplina > >::iterator itDiscSala;

    ITERA_GGROUP_LESSPTR( it_disciplina, this->disciplinas, Disciplina )
    {
		associacaoFile <<"\n\nDisciplina Id = "<< it_disciplina->getId();
		
		GGroup< Sala *, LessPtr< Sala > > *ggroupSalas;
		
		bool found=false;
		itDiscSala = this->discSalas.find(*it_disciplina);
		if ( itDiscSala != this->discSalas.end() )
		{
			ggroupSalas = & itDiscSala->second;
			if ( (*ggroupSalas).size() > 0 ) found = true;
		}

		if (found)
		{
			associacaoFile << "\tNumero de salas: " << ggroupSalas->size() << "\n\t";
			ITERA_GGROUP_LESSPTR( itSala, (*ggroupSalas), Sala )
			{			
				fflush(NULL);
				associacaoFile <<"(SalaId="<< (*itSala)->getId() << ", Codigo="<< (*itSala)->getCodigo();
				if ( (*itSala)->disciplinas_Associadas_AUTOMATICA.find( *it_disciplina ) !=
					 (*itSala)->disciplinas_Associadas_AUTOMATICA.end() )
					 associacaoFile << ", Auto";
				else
					associacaoFile << ", Manual";
				if ( (*itSala)->ehLab() )
					associacaoFile << ", Lab) ; ";
				else
					associacaoFile << ", Sala) ; ";
			}
		}
		else
		{
			fflush(NULL);
			ITERA_GGROUP_LESSPTR( itCampi, this->campi, Campus )
			{
				bool haDemanda = this->haDemandaDiscNoCampus( it_disciplina->getId(), itCampi->getId() );
				if ( haDemanda )
					std::cout<<"\nErro! Disciplina id=" << it_disciplina->getId() << " possui demanda e não esta associada a nenhuma sala!!";
			}
		}
	}
	associacaoFile.close();
}

void ProblemData::constroiHistogramaDiscDemanda()
{
	// histogram structures

	// Calcula as quantidades
	double qtdTotal = 0;
	ITERA_GGROUP_LESSPTR( itDem, this->demandas, Demanda )
	{
		Disciplina *disciplina = itDem->disciplina;
		double qtd = itDem->getQuantidade();
		qtdTotal += qtd;

		if ( this->mapDiscPerc.find(disciplina) != this->mapDiscPerc.end() )
			this->mapDiscPerc[disciplina] += qtd;
		else
			this->mapDiscPerc[disciplina] = qtd;
	}

	// Troca as quantidades por percentual
	std::map< Disciplina*, double, LessPtr<Disciplina> >::iterator itMapDiscPerc = this->mapDiscPerc.begin();
	for( ; itMapDiscPerc != this->mapDiscPerc.end(); itMapDiscPerc++ )
	{
		Disciplina *disciplina = itMapDiscPerc->first;
		int qtd = itMapDiscPerc->second;
		double perc = (qtd*100)/qtdTotal;
		itMapDiscPerc->second = perc;
	}

	std::set< std::pair<double,Disciplina*> > aux_Perc_Disc;

	// Calcula aux_Perc_Disc na ordem crescente de percentual de demanda	
	itMapDiscPerc = this->mapDiscPerc.begin();
	for( ; itMapDiscPerc != this->mapDiscPerc.end(); itMapDiscPerc++ )
	{
		Disciplina *disciplina = itMapDiscPerc->first;		
		double perc = itMapDiscPerc->second;

		std::pair<double,Disciplina*> par (perc, disciplina);		
		aux_Perc_Disc.insert( par );
	}

	// Calcula o percentual acumulado
	double acumulo=0.0;
	typedef std::set< std::pair<double,Disciplina*> >::iterator itAcum;
	
	std::reverse_iterator< itAcum > itEnd ( aux_Perc_Disc.begin() );
	std::reverse_iterator< itAcum > it ( aux_Perc_Disc.end() );

	for( ; it != itEnd; it++ )
	{
		Disciplina *disciplina = (*it).second;
		double perc = (*it).first;		
		acumulo += perc;

		std::pair<double,Disciplina*> par (acumulo, disciplina);		
		this->perc_Disc.add( par );
	}

	// Limpa mapDiscPerc e o preenche com os percentuais acumulados
	this->mapDiscPerc.clear();
	GGroup< std::pair<double,Disciplina*> >::iterator itGGroup = this->perc_Disc.begin();
	for( ; itGGroup != this->perc_Disc.end(); itGGroup++ )
	{
		Disciplina *disciplina = (*itGGroup).second;
		double perc = (*itGGroup).first;		
		
		mapDiscPerc[disciplina] = perc;
	}
}

void ProblemData::preencheMapTurnoSemanasLetivas()
{
	ITERA_GGROUP_LESSPTR( itCalend, this->calendarios, Calendario )
	{
		Calendario *calendario = *itCalend;

		ITERA_GGROUP_LESSPTR( itHorAula, calendario->horarios_aula, HorarioAula )
		{
			if ( itHorAula->getInicio() < inicio_tarde )
			{
				this->mapTurnoSemanasLetivas[ Manha ].add( calendario );
				calendario->addTurno( Manha, itHorAula->dias_semana );
			//	this->turnos.add( Manha );
			}
			else if ( itHorAula->getInicio() < inicio_noite )
			{
				this->mapTurnoSemanasLetivas[ Tarde ].add( calendario );
				calendario->addTurno( Tarde, itHorAula->dias_semana );
			//	this->turnos.add( Tarde );
			}
			else
			{
				this->mapTurnoSemanasLetivas[ Noite ].add( calendario );
				calendario->addTurno( Noite, itHorAula->dias_semana );
			//	this->turnos.add( Noite );
			}
		}	
	}
}


void ProblemData::preencheMapTurnoDisciplinas()
{
	ITERA_GGROUP_LESSPTR( itDisc, this->disciplinas, Disciplina )
	{
		Disciplina *disciplina = *itDisc;

		ITERA_GGROUP_LESSPTR( itHor, disciplina->horarios, Horario )
		{
			if ( itHor->horario_aula->getInicio() < inicio_tarde )
			{
				disciplina->addTurno( Manha, *itHor );
			}
			else if ( itHor->horario_aula->getInicio() < inicio_noite )
			{
				disciplina->addTurno( Tarde, *itHor );
			}
			else
			{
				disciplina->addTurno( Noite, *itHor );
			}
		}	
	}
}

int ProblemData::getFaseDoDia( DateTime dt )
{ 
	if ( dt < inicio_tarde )
	{
		return Manha;
	}
	else if ( dt < inicio_noite )
	{
		return Tarde;
	}
	else
	{
		return Noite;
	}
}

DateTime ProblemData::getFimDaFase( int fase ) const
{ 
	DateTime dt;
	dt.setHour(24);
	dt.setMinute(59);
	auto itFinder = fasesDosTurnos.find(fase);
	if ( itFinder != fasesDosTurnos.end() )
	{
		dt = itFinder->second.second;
	}
	return dt;
}

bool ProblemData::haTurnoComumViavel( Disciplina *disciplina, TurnoIES *turnoIES )
{
	GGroup< Horario*, LessPtr<Horario> > 
		horariosNoTurnoIES = disciplina->getHorariosOuCorrespondentes(turnoIES);

	map< int /*faseDoTurno*/, map< int /*dia*/, GGroup<DateTime> > > mapTurnoDiaDT;

	int nCreds = disciplina->getTotalCreditos();
	int nHors = 0;

	ITERA_GGROUP_LESSPTR( itHor, horariosNoTurnoIES, Horario )
	{
		DateTime dt = itHor->horario_aula->getInicio();
		int fase = this->getFaseDoDia( dt );
		ITERA_GGROUP_N_PT( itDia, itHor->dias_semana, int )
	    {
			mapTurnoDiaDT[ fase ][ *itDia ].add( dt );
		}
	}

	map< int, map< int, GGroup<DateTime> > >::iterator itMapFase = mapTurnoDiaDT.begin();
	for ( ; itMapFase != mapTurnoDiaDT.end(); itMapFase++ )
	{
		int fase = itMapFase->first;

		int n = 0;
		map< int, GGroup<DateTime> > mapDia = itMapFase->second;
		map< int, GGroup<DateTime> >::iterator itMapDia = mapDia.begin();
		for ( ; itMapDia != mapDia.end(); itMapDia++ )
		{
			n += itMapDia->second.size();
		}

		if ( n >= nCreds )
			return true;
	}
   
	return false;
}

GGroup<int> ProblemData::getTurnosComunsViaveis( Disciplina *disciplina, TurnoIES *turnoIES )
{
	GGroup<int> turnos; // Fases do dia

	GGroup< Horario*, LessPtr<Horario> > 
		horariosNoTurnoIES = disciplina->getHorariosOuCorrespondentes(turnoIES);

	map< int /*faseDoTurno*/, map< int /*dia*/, GGroup<DateTime> > > mapTurnoDiaDT;

	int nCreds = disciplina->getTotalCreditos();
	int nHors = 0;

	ITERA_GGROUP_LESSPTR( itHor, horariosNoTurnoIES, Horario )
	{
		DateTime dt = itHor->horario_aula->getInicio();
		int fase = this->getFaseDoDia( dt );
		ITERA_GGROUP_N_PT( itDia, itHor->dias_semana, int )
	    {
			mapTurnoDiaDT[ fase ][ *itDia ].add( dt );
		}
	}

	map< int, map< int, GGroup<DateTime> > >::iterator itMapFase = mapTurnoDiaDT.begin();
	for ( ; itMapFase != mapTurnoDiaDT.end(); itMapFase++ )
	{
		int fase = itMapFase->first;

		int n = 0;
		map< int, GGroup<DateTime> > mapDia = itMapFase->second;
		map< int, GGroup<DateTime> >::iterator itMapDia = mapDia.begin();
		for ( ; itMapDia != mapDia.end(); itMapDia++ )
		{
			n += itMapDia->second.size();
		}

		if ( n >= nCreds )
			turnos.add(fase);
	}
   
	return turnos;
}


bool ProblemData::alocacaoEquivViavel( Demanda *demanda, Disciplina *disciplinaEquiv )
{
	// Procura se já calculou
	std::pair< Demanda*, Disciplina* > pair(demanda, disciplinaEquiv);
	std::map< std::pair< Demanda*, Disciplina* >, bool >::iterator it = mapEquivViavelDemandaDisc.find(pair);
	if ( it != mapEquivViavelDemandaDisc.end() )
	{
		return it->second;
	}

	// Calcula
	if ( this->ehSubstituivel( demanda->disciplina->getId(), disciplinaEquiv->getId(), demanda->oferta->curso ) )
	{
		if ( this->haTurnoComumViavel( disciplinaEquiv, demanda->getTurnoIES() ) )
		{
			mapEquivViavelDemandaDisc[pair] = true;
			return true;
		}
	}
			
	mapEquivViavelDemandaDisc[pair] = false;
	return false;
}

GGroup<int> ProblemData::alocacaoEquivViavelTurnosIES( Demanda *demanda, Disciplina *disciplinaEquiv )
{
	GGroup<int> turnosViaveis;

	if ( this->ehSubstituivel( demanda->disciplina->getId(), disciplinaEquiv->getId(), demanda->oferta->curso ) )
	{
		turnosViaveis = this->getTurnosComunsViaveis( disciplinaEquiv, demanda->getTurnoIES() );
	}

	return turnosViaveis;
}

void ProblemData::imprimeUtilizacaoSala( int campusId, int prioridade, int cjtAlunosId, bool heuristica, int r, string MIP )
{
	#ifndef PRINT_LOGS
		return;
	#endif
	std::cout<<"\nImprime utilizacao das salas...\n"; fflush(NULL);

	ofstream utilizaSalasFile;
	std::string solFilenameCreditosSala( this->getUtilizacaoSalasFileName( campusId, prioridade, cjtAlunosId, heuristica, r, MIP ) );
	
	utilizaSalasFile.open( solFilenameCreditosSala, ios::out );

   if ( utilizaSalasFile )
   {
	   utilizaSalasFile << "CjtSala\tTempo Usado\n";
	   for(map<ConjuntoSala *, int, LessPtr<ConjuntoSala>>::iterator itCS = this->mapCreditosSalas.begin();
		   itCS != this->mapCreditosSalas.end();
		   itCS++)
	   {
		   utilizaSalasFile << itCS->first->getId() << "\t=\t" << itCS->second
			   << " (MAX = " << itCS->first->minLimiteTempoPermitidoNaSemana( this->mapDiscSubstituidaPor ) << ")\n";
	   }
	   

	   utilizaSalasFile << "\n\nDetalhado:";
	   for(map<ConjuntoSala *, int, LessPtr<ConjuntoSala>>::iterator itCS = this->mapCreditosSalas.begin();
		   itCS != this->mapCreditosSalas.end();
		   itCS++)
	   {
		   utilizaSalasFile << "\n\nCjtSala " << itCS->first->getId();
		   utilizaSalasFile << "   Sala " << itCS->first->salas.begin()->first;
		   utilizaSalasFile << "   Codigo " << itCS->first->salas.begin()->second->getCodigo();
		   utilizaSalasFile << "   Numero " << itCS->first->salas.begin()->second->getNumero();
		   utilizaSalasFile << "   Tipo " << itCS->first->salas.begin()->second->getTipoSalaId();
		   Sala *sala = itCS->first->salas.begin()->second;
		   
		   int dia = 0;
		   GGroup< HorarioDia*, LessPtr<HorarioDia> > alocacoes = sala->getHorariosDiaOcupados();
		   ITERA_GGROUP_LESSPTR( it, alocacoes, HorarioDia )
		   {
			   if ( dia != it->getDia() ) utilizaSalasFile << "\nDia " << it->getDia() << ":";

			   dia = it->getDia();
			   utilizaSalasFile << "  id" << it->getHorarioAulaId() << "=" 
				   << it->getHorarioAula()->getInicio().getHour() 
				   << ":" << it->getHorarioAula()->getInicio().getMinute();
		   }		   
	   }

	   utilizaSalasFile.close();
   }
}

int ProblemData::getNumTurmasJaAbertas( Disciplina *disciplina, int campusId )
{
	int n=0;
	for ( int i=1; i <= disciplina->getNumTurmas(); i++ )
	{
		if ( this->existeTurmaDiscCampus( i, disciplina->getId(), campusId ) )
			n++;
	}
	return n;
}

void ProblemData::preencheDiscSubstitutaPossivelPorDemanda()
{
	ITERA_GGROUP_LESSPTR( itDem, this->demandasTotal, Demanda )
	{
		Demanda *demanda = *itDem;
		Disciplina *discOrig = demanda->disciplina;

		ITERA_GGROUP_LESSPTR( itDiscEquiv, discOrig->discEquivSubstitutas, Disciplina )
		{
			Disciplina *discEquiv = *itDiscEquiv;

			if ( this->alocacaoEquivViavel( demanda, discEquiv ) )
				demanda->addSubstitutaPossivel( discEquiv );
		}
	}
}

bool ProblemData::ehSubstituivel( int disciplina, int disciplinaEquiv, Curso *curso )
{
	int oldD = abs( disciplina );
	int newD = abs( disciplinaEquiv );

	if ( oldD == newD ) return true;

	std::pair<int, int> parDisc( oldD, newD );
	std::map< std::pair< int/*oldDisc*/, int/*newDisc*/ >, int /*equiv*/>::iterator
		itMap = this->mapParDisc_EquivId.find( parDisc );
	if ( itMap!= this->mapParDisc_EquivId.end() )
	{
		int equivId = itMap->second;

		bool geral=false;
		Equivalencia* equiv = new Equivalencia();
		equiv->setId(equivId);
		GGroup< Equivalencia*, LessPtr< Equivalencia > >::iterator
			itEquiv = this->equivalencias.find(equiv);
		delete equiv;
		if ( itEquiv != this->equivalencias.end() )
		{
			equiv = *itEquiv;
			geral = equiv->getGeral();								
		}
		if (!geral)
		{
			if ( curso->equiv_ids.find(equivId) ==
				 curso->equiv_ids.end() )
			{
				return false;
			}
         else
            return true;
		}
		else return true;
	}
	return false;
}

AlunoDemanda* ProblemData::getAlunoDemandaEquiv( Aluno* aluno, Disciplina *discProcurada )
{
	ITERA_GGROUP_LESSPTR( itAlDem, aluno->demandas, AlunoDemanda )
	{
		if ( (*itAlDem)->demanda->getDisciplinaId() == discProcurada->getId() )
		{
			return (*itAlDem);
		}
	}
	ITERA_GGROUP_LESSPTR ( itAlDem, aluno->demandas, AlunoDemanda )
	{
		Disciplina *d = (*itAlDem)->demanda->disciplina;			
		if ( d != discProcurada ) // procura nas equivalencias
		{				
			// Se a disciplina d já não for uma substituta da original. Ou seja, não permite transitividade entre equivalências
			if ( (*itAlDem)->demandaOriginal == NULL )
			if ( this->alocacaoEquivViavel( (*itAlDem)->demanda, discProcurada ) )
			{
				AlunoDemanda* alDemTeor = (*itAlDem);
				int idOrig = alDemTeor->demanda->disciplina->getId();

				if ( discProcurada->getId() > 0)
					return alDemTeor;
				else
				{
					AlunoDemanda* alDemPrat = aluno->getAlunoDemanda( -idOrig );
					if ( alDemPrat!=NULL )
						return alDemPrat;
					else
						return alDemTeor;
				}
			}
		}			
		else if ( d == discProcurada )
		{
			return *itAlDem;
		}
	}

	return NULL;
}

void ProblemData::addMapAula( int campusId, int turma, Disciplina* disciplina, Aula* aula )
{
	this->mapAulas[campusId][turma][disciplina].add( aula );
}

GGroup<Aula*, LessPtr<Aula>> ProblemData::getMapAulas( int campusId, int turma, Disciplina* disciplina )
{
	return this->mapAulas[campusId][turma][disciplina];
}

#ifdef BUILD_COM_SOL_INICIAL

int ProblemData::getPercAtendAlunosInicialFixado( int cpId )
{ 
	if ( ! existeSolTaticoInicial() )
		return 0;

	int nroAlunosAtend = getSolTaticoInicial()->getNroAlunosDemandaAtendidos(cpId);
	int nroAlunosOrig = getTotalOrigAlunoDemanda(cpId);
	if ( nroAlunosOrig > 0 )
		return (int) ( ( nroAlunosAtend * 100 ) / nroAlunosOrig ); 

	return 0;
}
int ProblemData::getPercAtendTurmasInicialFixado( int cpId )
{ 
	if ( ! existeSolTaticoInicial() )
		return 0;

	int nroTurmasAbertas = getSolTaticoInicial()->getNroTurmasAbertas(cpId);
	int nroTurmasEst = getNumTurmasTotalEstimados(cpId);
	if ( nroTurmasEst > 0 )
		return (int) ( ( nroTurmasAbertas * 100 ) / nroTurmasEst ); 

	return 0;
}


int ProblemData::getPercAtendInicialFixado( int cpId )
{ 
	int percTurmas = getPercAtendTurmasInicialFixado(cpId);
	int percAlunos = getPercAtendAlunosInicialFixado(cpId);
	return max( percTurmas, percAlunos );
}

bool ProblemData::passarPorPreModeloETatico( int cpId )
{ 
	int nroAlunosOrig = getTotalOrigAlunoDemanda(cpId);
	bool problemaGrande = (nroAlunosOrig>1000? true:false );
	
	if ( this->getPercAtendInicialFixado(cpId) < 70 && problemaGrande )
		return true;
	else
		return false;
}


int ProblemData::estimaNroVarsV( int campusId )
{ 
	int nVars = 0;

	// Estima numero de variaveis do tipo v_{a,i,d,s,t,hi,hf} a serem criadas
	// no Tatico Integrado que abre turmas, a fim de atender demandas
	// ainda não atendidas.
	ITERA_GGROUP_LESSPTR( itAlDem, this->listSlackDemandaAluno, AlunoDemanda )
	{
		Disciplina *disciplina = itAlDem->demanda->disciplina;

		int nTurmasAbertas = this->getNumTurmasJaAbertas( disciplina, campusId );

		int nSalas = disciplina->cjtSalasAssociados.size();
		int nTurmas = disciplina->getNumTurmas() - nTurmasAbertas;
		if ( nTurmas < 0 )
		{
			nTurmas = 0;
			std::cout<<"Erro, abertas mais turmas do que o total! Como??";
		}
		int nHorsHiHf = disciplina->getTotalDeInicioTerminoValidos();

		nVars += nSalas*nTurmas*nHorsHiHf;
	}

	return nVars;
}

bool ProblemData::pularParaTaticoIntegrado( int campusId )
{
	// Retorna true se a estimativa para o numero de variaveis no Tatico Integrado
	// for baixa o suficiente, indicando que o nro de combinações não explodirá. 

	int estima = this->estimaNroVarsV( campusId );
	std::cout << "\nEstimativa para total de variaveis v para novas alocacoes: " << estima << std::endl;
	if ( estima < 4000000 )
		return true;
	return false;
}

#endif

void ProblemData::confereCorretudeAlocacoes()
{
	std::cout << "\nConfere corretude de alocacoes";

   auto itMapTrio = this->mapCampusTurmaDisc_AlunosDemanda.begin();
   for ( ; itMapTrio != this->mapCampusTurmaDisc_AlunosDemanda.end(); itMapTrio++ )
   {
	   Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio = itMapTrio->first;

	   Disciplina *discAlocada = trio.third;

	   ITERA_GGROUP_LESSPTR( itAlDem, itMapTrio->second, AlunoDemanda )
	   {
 		    bool usouEquiv = abs(itAlDem->demanda->disciplina->getId()) != abs( discAlocada->getId() );

			if ( itAlDem->getExigeEquivalenciaForcada() && !usouEquiv )
			{
				if ( itAlDem->demandaOriginal == nullptr )
				{
					// Equivalencia NÃO usada							
					if ( itAlDem->demanda->disciplina->discEquivSubstitutas.size() > 0 )
					{
						stringstream msg;
						msg << "Equivalencia forcada nao respeitada. Aluno " 
							<< itAlDem->getAlunoId() << " e Disciplina " << discAlocada->getId();
						CentroDados::printError( "void ProblemData::confereCorretudeAlocacoes()", msg.str() );
					}
					else
					{
						stringstream msg;
						msg << "Disciplina sem equivalentes encontradas. Equivalencia forcada nao respeitada. Aluno " 
							<< itAlDem->getAlunoId() << " e Disciplina " << discAlocada->getId();
						CentroDados::printError( "void ProblemData::confereCorretudeAlocacoes()", msg.str() );							   
					}
				}
				else
				{
					// Equivalencia usada
					if ( ! this->alocacaoEquivViavel( itAlDem->demandaOriginal, discAlocada ) )
					{
						stringstream msg;
						msg << "Equivalencia forcada nao respeitada. Aluno " 
							<< itAlDem->getAlunoId() << " e Disciplina " << discAlocada->getId();
						CentroDados::printError( "void ProblemData::confereCorretudeAlocacoes()", msg.str() );
					}
				}
			}
				   
			if ( usouEquiv )
			{
				// Equivalencia usada
				if ( itAlDem->demandaOriginal != nullptr )
				if ( ! this->alocacaoEquivViavel( itAlDem->demandaOriginal, discAlocada ) )
				{
					stringstream msg;
					msg << "Equivalencia inviavel usada. Aluno " 
						<< itAlDem->getAlunoId() << ", Curso " << itAlDem->demandaOriginal->oferta->getCursoId()
						<< " e Disciplina " << discAlocada->getId();
					CentroDados::printError( "void ProblemData::confereCorretudeAlocacoes()", msg.str() );
				}
			}
	   }
   }
}

void ProblemData::confereDadosDeEquivalenciaForcada()
{
	std::map< Disciplina*, std::map< bool, GGroup<AlunoDemanda*, 
		LessPtr<AlunoDemanda>> >, LessPtr<Disciplina> > mapDiscExigeEquivForcada;
	
	int i=1;
	ITERA_GGROUP_LESSPTR( itAlDem, this->alunosDemandaTotal, AlunoDemanda )
	{
		bool exigeEquivForcada = itAlDem->getExigeEquivalenciaForcada();

		if ( itAlDem->demanda==nullptr )
		{
			std::cout << "\nErro, aluno-demanda " << itAlDem->getId() << " sem referencia para demanda " << itAlDem->getDemandaId();
			fflush(0);
			continue;
		}		
		if ( itAlDem->demanda->disciplina==nullptr )
		{
			std::cout << "\nErro, demanda " << itAlDem->demanda->getId() << " sem referencia para disciplina " << itAlDem->demanda->getDisciplinaId();
			fflush(0);
			continue;
		}
		
		mapDiscExigeEquivForcada[ itAlDem->demanda->disciplina ][ exigeEquivForcada ].add( *itAlDem );
		
		i++;
	}
	
	auto itMapDisc = mapDiscExigeEquivForcada.begin();
	for ( ; itMapDisc != mapDiscExigeEquivForcada.end(); itMapDisc++ )
	{
		bool exigeTrueAndFalse = ( itMapDisc->second.size() > 1 ? true : false );

		if ( exigeTrueAndFalse )
		{
			auto itMapExige = itMapDisc->second.begin();
			for ( ; itMapExige != itMapDisc->second.end(); itMapExige++ )
			{
				bool exige = itMapExige->first;

				auto itAlDem = itMapExige->second.begin();
				for ( ; itAlDem != itMapExige->second.end(); itAlDem++ )
				{
					if ( (! exige) && (! itAlDem->getAluno()->ehFormando() ) )
					{
						stringstream msg;
						msg << "O aluno " << itAlDem->getAluno()->getAlunoId()
							<< " nao eh formando e nao exige equivalencia forcada para a disciplina "
							<< itMapDisc->first->getId() << ", no entanto ha casos com essa exigencia.";
						CentroDados::printWarning( "void ProblemData::confereDadosDeEquivalenciaForcada()", msg.str() );
					}
					else if ( (exige) && (itAlDem->getAluno()->ehFormando() ) )
					{
						stringstream msg;
						msg << "O aluno " << itAlDem->getAluno()->getAlunoId()
							<< " eh formando e exige equivalencia forcada para a disciplina "
							<< itMapDisc->first->getId() << ".";
						CentroDados::printWarning( "void ProblemData::confereDadosDeEquivalenciaForcada()", msg.str() );					
					}
				}
			}
		}
	}
}

int ProblemData::getNroProfPorDisc( Disciplina *disciplina )
{	
	int nroProfs = 0;
	auto itDisc = mapDiscProfsHabilit.find( disciplina );
	if ( itDisc != mapDiscProfsHabilit.end() )
		nroProfs = itDisc->second.size();
    return nroProfs;
}

void ProblemData::calculaCalendarioSomaInterv()
{
	ITERA_GGROUP_LESSPTR( itCalend, this->calendarios, Calendario )
	{
		itCalend->calculaDiaFaseSomaInterv();
		itCalend->calculaDiaSomaInterv();
	}
}

int ProblemData::getCalendariosMaxSomaInterv( int dia, int fase )
{
	int maxSoma=0;
	ITERA_GGROUP_LESSPTR( itCalend, this->calendarios, Calendario )
	{
		int soma = itCalend->getSomaInterv( dia, fase );
		maxSoma = max( maxSoma, soma );
	}
	return maxSoma;
}

int ProblemData::getCalendariosMaxSomaInterv( int dia )
{
	int maxSoma=0;
	ITERA_GGROUP_LESSPTR( itCalend, this->calendarios, Calendario )
	{
		int soma = itCalend->getSomaInterv( dia );
		maxSoma = max( maxSoma, soma );
	}
	return maxSoma;
}

int ProblemData::getCalendariosMaxInterv( int dia, int fase )
{
	int maxInterv=0;
	ITERA_GGROUP_LESSPTR( itCalend, this->calendarios, Calendario )
	{
		int interv = itCalend->getMaxInterv( dia, fase );
		maxInterv = max( maxInterv, interv );
	}
	return maxInterv;
}