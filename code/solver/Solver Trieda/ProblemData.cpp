#include "ProblemData.h"

#include <iostream>
#include <fstream>

// Macro baseada na ITERA_SEQ para facilitar a leitura dos dados no
// ProblemData
#define LE_SEQ(ggroup,addr,type) \
   for (Grupo##type::type##_iterator __##type##_it = \
   (addr).type().begin(); __##type##_it != (addr).type().end(); \
   ++ __##type##_it) { \
   type *__obj_##type = new type; \
   __obj_##type->le_arvore(*__##type##_it); \
   ggroup.add(__obj_##type); } 

ProblemData::ProblemData()
{
   EQUIV_TRANSITIVIDADE = true;

   this->atendimentosTatico = NULL;
}

ProblemData::~ProblemData()
{

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

void ProblemData::le_arvore( TriedaInput & raiz )
{
   this->calendarios.clear();
   LE_SEQ( this->calendarios, raiz.calendarios(), Calendario );

   ITERA_GGROUP_LESSPTR( it_calendario,
      this->calendarios, Calendario )
   {
      Calendario * calendario = ( *it_calendario );

      ITERA_GGROUP_LESSPTR( it_turno, calendario->turnos, Turno )
      {
	      this->todos_turnos.add( *it_turno );
      }
   }

   ITERA_SEQ( it_campi, raiz.campi(), Campus )
   {
      Campus * campus = new Campus;
      campus->le_arvore( *it_campi );
      this->campi.add( campus );
   }

   ITERA_SEQ( it_tsalas, raiz.tiposSala(), TipoSala )
   {
      TipoSala * tipo_sala = new TipoSala;
      tipo_sala->le_arvore( *it_tsalas );
      this->tipos_sala.add( tipo_sala );
   }

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
   LE_SEQ( this->alunosDemandaTotal, raiz.alunosDemanda(), AlunoDemanda );
   
   this->parametros = new ParametrosPlanejamento;
   this->parametros->le_arvore( raiz.parametrosPlanejamento() );

   ITERA_SEQ( it_oferta,
      raiz.ofertaCursosCampi(), OfertaCurso )
   {
      Oferta * oferta = new Oferta;
      oferta->le_arvore( *it_oferta );
      this->ofertas.add( oferta );
   }

   if ( this->parametros->otimizarPor == "ALUNO" )
   {
	   // Inicializa com zero o numero de prioridades de cada campus
	   ITERA_GGROUP_LESSPTR( it_campi, this->campi, Campus )
	   {
		   this->nPrioridadesDemanda[ it_campi->getId() ] = 0;   
	   }

	   // Filtrando alunosDemanda: referência somente para os alunosDemanda que tiverem prioridade 1
	   ITERA_GGROUP_LESSPTR( itAlDem, this->alunosDemandaTotal, AlunoDemanda )
	   {
		   int demId = itAlDem->getDemandaId();
		   int campusId = retornaCampusId( demId );
		   int p = itAlDem->getPrioridade();

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
	   
	   // Filtrando demanda: referencia somente para as que possuirem alunoDemanda associado
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



   // Se a tag existir ( mesmo que esteja em branco ) no xml de entrada
   if ( raiz.atendimentosTatico().present() )
   {
      this->atendimentosTatico = new GGroup< AtendimentoCampusSolucao *, LessPtr< AtendimentoCampusSolucao > > ();

      for ( unsigned int i = 0;
		    i < raiz.atendimentosTatico().get().AtendimentoCampus().size(); i++ )
      {
         ItemAtendimentoCampusSolucao * it_atendimento
            = &( raiz.atendimentosTatico().get().AtendimentoCampus().at( i ) );

         AtendimentoCampusSolucao * item = new AtendimentoCampusSolucao();
         item->le_arvore( ( *it_atendimento ) );
         this->atendimentosTatico->add( item );
      }
   }

   TriedaInput::fixacoes_type & list_fixacoes = raiz.fixacoes();
   LE_SEQ( this->fixacoes, list_fixacoes, Fixacao );

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

   // Primeiro caso : executar o
   // solver apenas com a entrada do tático
   bool primeiroCaso = ( this->parametros->modo_otimizacao == "TATICO" );

   // Segundo caso  : executar o solver com
   // a saída do tático e a entrada do operacional
   bool segundoCaso  = ( this->parametros->modo_otimizacao == "OPERACIONAL"
							&& raiz.atendimentosTatico().present() == true );

   // Terceiro caso : executar o solver apenas
   // com a entrada do operacional (sem saída do tático)
   bool terceiroCaso = ( this->parametros->modo_otimizacao == "OPERACIONAL"
							&& raiz.atendimentosTatico().present() == false );

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
			if ( it_oferta->curriculo->getPeriodo( disciplina ) > 0 )
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

	/*for (; it_map != this->compat_cursos.end(); it_map++ )
	{
		int id1 = it_map->first.first->getId();
		int id2 = it_map->first.second->getId();

		if (   ( id1 == curso1->getId() && id2 == curso2->getId() )
			|| ( id2 == curso1->getId() && id1 == curso2->getId() ) )
		{
			if ( it_map->second == true )
			{
				return true;
			}
		}
	}

	return false;*/
}

// Dada uma disciplina, e seu par curso/curriculo,
// esse método retorna-se a oferta dessa disciplina
Oferta * ProblemData::retornaOfertaDiscilpina(
	Curso * curso, Curriculo * curriculo, Disciplina * disciplina )
{
	Oferta * oferta = NULL;
	Demanda * demanda = NULL;

	ITERA_GGROUP_LESSPTR( it_demanda, this->demandas, Demanda )
	{
		demanda = ( *it_demanda );

		if ( demanda->oferta->curso->getId() == curso->getId()
			&& demanda->oferta->curriculo->getId() == curriculo->getId()
			&& demanda->disciplina->getId() == disciplina->getId() )
		{
			oferta = demanda->oferta;
			break;
		}
	}

	return oferta;
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

	/*ITERA_GGROUP_LESSPTR( it_disc, this->disciplinas, Disciplina )
	{
		if ( it_disc->getId() == id)
		{
			d = ( *it_disc );
			return d;
		}
	}
	return d;*/
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
			if ( it_oferta->curriculo->getPeriodo( disciplina ) > 0 )
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

GGroup< Demanda *, LessPtr< Demanda > >
   ProblemData::retornaDemandaDisciplinasSubstituidas( Disciplina * disciplina )
{
	GGroup< Demanda *, LessPtr< Demanda > > demandas_disciplina;

	ITERA_GGROUP_LESSPTR( it_demanda, this->demandas, Demanda )
	{
		if ( it_demanda->disciplina->getId() == disciplina->getId() )
		{
			demandas_disciplina.add( *it_demanda );
		}
	}

	return demandas_disciplina;
}

Demanda * ProblemData::buscaDemanda(
  Curso * curso, Disciplina * disciplina )
{
   Demanda * demanda = NULL;

   ITERA_GGROUP_LESSPTR( it_demanda, this->demandas, Demanda )
   {
      if ( it_demanda->disciplina->getId() == disciplina->getId()
         && cursosCompativeis( curso, it_demanda->oferta->curso ) )
      {
         demanda = ( *it_demanda );
         break;
      }
   }

	return demanda;
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

bool ProblemData::aulaAtendeCurso( Aula * aula, Curso * curso )
{
   ITERA_GGROUP_LESSPTR( it_aula, this->aulas, Aula )
   {
      ITERA_GGROUP_LESSPTR( it_oferta, it_aula->ofertas, Oferta )
      {
         if ( it_oferta->curso == curso )
         {
            return true;
         }
      }
   }

   return false;
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

// Método relacionado com a issue TRIEDA-1054
bool ProblemData::verificaDisponibilidadeDisciplinaHorario(
   Disciplina * disciplina, HorarioAula * horario_aula )
{
   int idCalendarioDisc = -1;

   ITERA_GGROUP_LESSPTR( it_curso, this->cursos, Curso )
   {
      Curso * curso = ( *it_curso );

      if ( idCalendarioDisc >= 0 )
      {
         break;
      }

      ITERA_GGROUP_LESSPTR( it_curriculo, curso->curriculos, Curriculo )
      {
         Curriculo * curriculo = ( *it_curriculo );
         
         if ( idCalendarioDisc >= 0 )
         {
            break;
         }

         map < Disciplina*, int, LessPtr< Disciplina > >::iterator it_disc_periodo
			 = curriculo->disciplinas_periodo.find(disciplina);

         if (it_disc_periodo != curriculo->disciplinas_periodo.end())
         {
				idCalendarioDisc = disciplina->getCalendario()->getId();
         }
      }
   }

   if ( idCalendarioDisc < 0 )
   {
      /*
      std::cerr << "A disciplina " << disciplina->getCodigo()
                << " nao esta associada a nenhums matriz curricular. "
                << std::endl;

      exit( 1 );
      */
   }

   bool result = ( idCalendarioDisc >= 0 && horario_aula != NULL
      && horario_aula->getCalendario() != NULL
      && horario_aula->getCalendario()->getId() == idCalendarioDisc );

   return result;
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
      demanda = ( *it_demanda );

      if ( demanda == NULL )
      {
         continue;
      }

      if ( demanda->getOfertaId() == id_oferta
         && demanda->getDisciplinaId() == id_disciplina )
      {
         break;
      }
   }

   return demanda;
}

Turno * ProblemData::findTurno( int turno_id )
{
   ITERA_GGROUP_LESSPTR( it_turno, this->todos_turnos, Turno )
   {
      Turno * turno = ( *it_turno );

      if ( turno != NULL && turno->getId() == turno_id )
      {
         return turno;
      }
   }

   return NULL;
}

HorarioAula * ProblemData::findHorarioAula( int id_horario_aula )
{
   ITERA_GGROUP_LESSPTR( it_horario_dia, this->horariosDia, HorarioDia )
   {
      HorarioAula * horario_aula = it_horario_dia->getHorarioAula();

      if ( horario_aula != NULL
         && horario_aula->getId() == id_horario_aula )
      {
         return horario_aula;
      }
   }

   return NULL;
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
   GGroup< Professor *, LessPtr< Professor > > professores = this->getProfessores();

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );

      if ( professor->getId() == id )
      {
         return professor;
      }
   }

   return NULL;
}

int ProblemData::totalHorariosTurnoAula( Aula * aula )
{
   int totalHorarios = 0;

   Oferta * oferta = ( aula->ofertas.size() == 0 ? NULL : *aula->ofertas.begin() );

   if ( oferta != NULL )
   {
      Turno * turno = this->findTurno( oferta->getTurnoId() );

      if ( turno != NULL )
      {
         totalHorarios = turno->horarios_aula.size();
      }
   }

   return totalHorarios;
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

Sala * ProblemData::findSala( int id )
{
   GGroup< Sala *, LessPtr< Sala > > salas = this->getSalas();

   ITERA_GGROUP_LESSPTR( it_sala, salas, Sala )
   {
      Sala * sala = ( *it_sala );

      if ( sala->getId() == id )
      {
         return sala;
      }
   }

   return NULL;
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

	for ( ; itMap != this->mapDiscSubstituidaPor.end(); itMap++ )
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
		
	std::map< Aluno*, GGroup< Trio< int, int, Disciplina* > >, LessPtr< Aluno > >::iterator
		itMap = mapAluno_CampusTurmaDisc.find( aluno );
	
	if ( itMap != mapAluno_CampusTurmaDisc.end() )
	{
		GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > > atendimentos = itMap->second;
	
		GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >::iterator itAtend = atendimentos.begin();
		for ( ; itAtend != atendimentos.end(); itAtend++ )
		{
			if ( (*itAtend).third == disc )	
			{
				turma = (*itAtend).second;
				return turma;
			}
		}
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
	
	ITERA_GGROUP_LESSPTR( itHd, horariosDia, HorarioDia )
	{
		if ( itHd->getDia() == dia &&
			 itHd->getHorarioAula() == ha )
		{
			return *itHd;
		}
	}

	return NULL;
}

AlunoDemanda* ProblemData::procuraAlunoDemanda( int discId, int alunoId )
{
	ITERA_GGROUP_LESSPTR( itAlDemanda, alunosDemandaTotal, AlunoDemanda )
	{
		if ( itAlDemanda->demanda->getDisciplinaId() == discId &&
			 itAlDemanda->getAlunoId() == alunoId )
		{
			return *itAlDemanda;
		}
	}

	return NULL;
}

/*
	Atualiza alunosDemanda e demandas, retirando o que não foi atendido
	de prioridade p-1 e acrescentando o que há de demanda de prioridade p
*/
void ProblemData::atualizaDemandas( int novaPrioridade, int campusId )
{	
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
			AlunoDemanda * ad = procuraAlunoDemanda( - discId, alunoId );

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
	GGroup< Demanda*, LessPtr<Demanda> > remover;
	GGroup< Demanda*, LessPtr<Demanda> > todas;
	ITERA_GGROUP_LESSPTR( itDem, this->demandas, Demanda )
	{
		if ( itDem->getQuantidade() == 0 )
		{
			remover.add( *itDem );
		}
		todas.add( *itDem );
	}

	this->demandas.clear();

	ITERA_GGROUP_LESSPTR( itDem, todas, Demanda )
	{
		GGroup< Demanda*, LessPtr<Demanda> >::iterator itRemover = remover.find( *itDem );
		if ( itRemover == remover.end() )
		{
			this->demandas.add( *itDem );
		}
	}
	
	// -----------------------------------------------------
	// NOVAS DEMANDAS de novaPrioridade

	// Filtrando alunosDemanda: somente os alunosDemanda que tiverem a prioridade procurada
	// FILTRAR: NOVAS PRIORIDADES SOMENTE PARA OS ALUNOS QUE NÃO TIVERAM TOTAL ATENDIMENTO ANTERIORMENTE!
	ITERA_GGROUP_LESSPTR( itAlDem, this->alunosDemandaTotal, AlunoDemanda )
	{
		int alunoId = itAlDem->getAlunoId();

		if ( itAlDem->getPrioridade() == novaPrioridade && 
			 itAlDem->demanda->oferta->getCampusId() == campusId )
		{
			// Só insere o alunoDemanda novo se existir folga de demanda anterior para o aluno correspondente
			ITERA_GGROUP_LESSPTR( itSlack, this->listSlackDemandaAluno, AlunoDemanda )
			{
				if ( itSlack->getAlunoId() == alunoId )
				{
					this->alunosDemanda.add( *itAlDem );
					Aluno *a = retornaAluno( alunoId );
					a->demandas.add( *itAlDem );
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
	ITERA_GGROUP_LESSPTR( itDem, this->demandasTotal, Demanda )
	{
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

      demanda->disciplina->setMaxDemanda(
         std::max( demanda->disciplina->getMaxDemanda(), dem ) );

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

/*
	Retorna o numero de alunos da ofertaId alocados na turma e discId.
*/
int ProblemData::atendeTurmaDiscOferta( int turma, int discId, int ofertaId )
{
	int campusId = this->refOfertas[ofertaId]->getCampusId();
	Disciplina* disciplina = this->refDisciplinas[discId];

	Trio< int, int, Disciplina* > trio;
	trio.set( campusId, turma, disciplina);

	std::map< Trio< int, int, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator itMap = 
		this->mapCampusTurmaDisc_AlunosDemanda.find( trio );
	
	if ( itMap == this->mapCampusTurmaDisc_AlunosDemanda.end() )
	{
		return 0;
	}

	int n = 0;

	GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > at_alunosDemanda = itMap->second;

	ITERA_GGROUP_LESSPTR( itAlDem, at_alunosDemanda, AlunoDemanda )
	{
		if ( itAlDem->demanda->getOfertaId() == ofertaId )
		{
			n++;
		}
	}

	return n;
}

/*
	Pesquisa em demanda, ou seja, considerando a prioridade atual.
*/
bool ProblemData::haDemandaDiscNoCampus( int disciplina, int campusId )
{	
	ITERA_GGROUP_LESSPTR ( itDem, this->demandas, Demanda )
	{
		if ( (*itDem)->getDisciplinaId() == disciplina &&
			 (*itDem)->oferta->getCampusId() == campusId )
		{
			return true;
		}
	}

	return false;
}


bool ProblemData::haDemandaDiscNoCampus( int disciplina, int campusId, int prioridade )
{	
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

GGroup<Aluno*> ProblemData::alunosEmComum( int turma1, Disciplina* disc1, int turma2, Disciplina* disc2, Campus* campus )
{
	GGroup<Aluno*> alunosEmComum;

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

bool ProblemData::possuiAlunosEmComum( int turma1, Disciplina* disc1, int turma2, Disciplina* disc2, Campus* campus )
{
	Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio1;
	trio1.set( campus->getId(), turma1, disc1 );
	Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio2;
	trio2.set( campus->getId(), turma2, disc2 );

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

		if ( aluno->getOferta()->getCampusId() != campusId )
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
			Disciplina *d = ad->demanda->disciplina;
		
			itCjtAlunos = auxCjtAlunos.begin();

			for ( ; itCjtAlunos != auxCjtAlunos.end(); itCjtAlunos++ )
			{
				bool EXISTE_NO_CJT_ATUAL = false;

				GGroup< Aluno *, LessPtr< Aluno > > grupoAlunos = (*itCjtAlunos).second;
				ITERA_GGROUP_LESSPTR( itAlunoInt, grupoAlunos, Aluno )
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
					GGroup< Aluno*, LessPtr< Aluno > > listaAlunos = auxCjtAlunos[id];
					ITERA_GGROUP_LESSPTR ( itAl, listaAlunos, Aluno)
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
	
		GGroup< Aluno *, LessPtr< Aluno > > grupoAlunos = itCjtAlunos->second;
		ITERA_GGROUP_LESSPTR( itAl, grupoAlunos, Aluno )
		{
			Aluno *a = *itAl;
			
			ITERA_GGROUP_LESSPTR( itAlDem, a->demandas, AlunoDemanda )
			{			
				AlunoDemanda *alDem = *itAlDem;
				Demanda *dem = alDem->demanda;
				Disciplina *disciplina = dem->disciplina;

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



   ITERA_GGROUP_LESSPTR( it_disciplina, this->disciplinas, Disciplina )
   {
      Disciplina* disciplina = ( *it_disciplina );

	  if ( this->cp_discs[ campusId ].find( disciplina->getId() ) ==
		   this->cp_discs[ campusId ].end() )
	  {
		  continue;
 	  }

	  #pragma region Equivalencias
	  if ( ( this->mapDiscSubstituidaPor.find( disciplina ) !=
			 this->mapDiscSubstituidaPor.end() ) &&
			!this->ehSubstituta( disciplina ) )
	  {
		  continue;
	  }
	  #pragma endregion
	  
	  if ( ! this->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
	  {
		  continue;
	  }

	  map< Disciplina *, int /* cjtAlunosId */, LessPtr< Disciplina > >::iterator 
		  itMapDiscCjt = this->cjtDisciplinas.find( disciplina );

	  if ( itMapDiscCjt == this->cjtDisciplinas.end() )
	  {
		  std::cout<<"\nAtencao em cria_preRestricao_abre_turmas_em_sequencia: disciplina "
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


int ProblemData::haDemandaDiscNoCjtAlunosPorOferta( int discId, int oftId, int cjtAlunosId )
{	
	GGroup< Demanda*, LessPtr< Demanda > > demandasCjtAlunos;

	map< int, GGroup< Demanda *, LessPtr< Demanda > > >::iterator itMap = this->cjtDemandas.find( cjtAlunosId );
	
	if ( itMap == this->cjtDemandas.end() )
	{
		std::cout << "\nConjunto de alunos " << cjtAlunosId << " nao encontrado.\n";
		return false;
	}
	else
	{
		demandasCjtAlunos = itMap->second;
	}

	// Calculando P_{d,o}
	int qtdDem = 0;

	ITERA_GGROUP_LESSPTR( itDem, demandasCjtAlunos, Demanda )
	{
		if ( itDem->getDisciplinaId() == discId && 
			 itDem->getOfertaId() == oftId )
		{
			qtdDem += itDem->getQuantidade();
		}
	}

	return qtdDem;
}

int ProblemData::haDemandaDiscNoCjtAlunosPorCurso( int discId, int cursoId, int cjtAlunosId )
{	
	GGroup< Demanda*, LessPtr< Demanda > > demandasCjtAlunos;

	map< int, GGroup< Demanda *, LessPtr< Demanda > > >::iterator itMap = this->cjtDemandas.find( cjtAlunosId );
	
	if ( itMap == this->cjtDemandas.end() )
	{
		std::cout << "\nConjunto de alunos " << cjtAlunosId << " nao encontrado.\n";
		return false;
	}
	else
	{
		demandasCjtAlunos = itMap->second;
	}

	// Calculando P_{d,o}
	int qtdDem = 0;

	ITERA_GGROUP_LESSPTR( itDem, demandasCjtAlunos, Demanda )
	{
		if ( itDem->getDisciplinaId() == discId && 
			itDem->oferta->getCursoId() == cursoId )
		{
			qtdDem += itDem->getQuantidade();
		}
	}

	return qtdDem;
}


bool ProblemData::haDemandaP2DiscNoCampus( int campusId, int P_ATUAL, Disciplina* disciplina )
{
	ITERA_GGROUP_LESSPTR( itAlDem, this->alunosDemanda, AlunoDemanda )
	{
		if ( itAlDem->demanda->getDisciplinaId() == disciplina->getId() && 
		 	 itAlDem->getPrioridade() == P_ATUAL && 
			 itAlDem->demanda->oferta->getCampusId() == campusId )
		{
			return true;
		}
	}
	return false;
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

		GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > > gad = this->cjtAlunoDemanda[id];
		ITERA_GGROUP_LESSPTR( itAlDem, gad, AlunoDemanda )
		{
			if ( itAlDem->demanda->getDisciplinaId() >= 0 )
				nAlDem++;
		}
		GGroup< Demanda *, LessPtr< Demanda > > gd = this->cjtDemandas[id];
		ITERA_GGROUP_LESSPTR( itDem, gd, Demanda )
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
	
	std::cout << "\nTotal de alunos: " << totalAlunos;
	std::cout << "\nTotal de demandas: " << totalDemandas;
	std::cout << "\nTotal de alunosDemanda: " << totalAlunoDemanda;
	std::cout << "\nTotal de demandas sem divisao de pratica: " << totalDemandasSP;
	std::cout << "\nTotal de alunosDemanda sem divisao de pratica: " << totalAlunoDemandaSP;

}


// Verifica se disciplina, sala e professor possuem o horario no dia
bool ProblemData::verificaDisponibilidadeHorario( HorarioAula *horarioAula, int dia, Sala *sala, Professor *prof, Disciplina* disc )
{
	bool achouNaSala = false;
	ITERA_GGROUP( itHorDia, sala->horariosDia, HorarioDia )
	{
		if ( (*itHorDia)->getHorarioAula() == horarioAula &&
			(*itHorDia)->getDia() == dia )
		{
			achouNaSala = true;
			break;
		}
	}
	if ( !achouNaSala )
	{
		return false;
	}

	bool achouNaDisc = false;
	ITERA_GGROUP( itHorDia, disc->horariosDia, HorarioDia )
	{
		if ( (*itHorDia)->getHorarioAula() == horarioAula &&
			(*itHorDia)->getDia() == dia )
		{
			achouNaDisc = true;
			break;
		}
	}
	if ( !achouNaDisc )
	{
		return false;
	}

	if ( prof != NULL )
	{
		bool achouNoProf = false;
		ITERA_GGROUP_LESSPTR( itHorDia, prof->horariosDia, HorarioDia )
		{
			if ( (*itHorDia)->getHorarioAula() == horarioAula &&
				(*itHorDia)->getDia() == dia )
			{
				achouNoProf = true;
				break;
			}
		}
		if ( !achouNoProf )
		{
			return false;
		}
	}

	return true;

}


void ProblemData::insereAlunoEmTurma( Aluno* aluno, Trio< int /*campusId*/, int /*turma*/, Disciplina*> trio, std::map<int/*dia*/, double> diasNCreds )
{
	AlunoDemanda *alDem = aluno->getAlunoDemanda( trio.third->getId() );
	if ( alDem != NULL )
	{
		this->mapAluno_CampusTurmaDisc[aluno].add( trio );
		this->mapCampusTurmaDisc_AlunosDemanda[trio].add( alDem );

	}
		
	Calendario *sl = trio.third->getCalendario();
	
	std::map<int, double>::iterator itMap = diasNCreds.begin();
	for( ; itMap != diasNCreds.end(); itMap++ )
	{
		int dia = (*itMap).first;
		double value = (*itMap).second;
		aluno->addNCredsAlocados( sl, dia, value );
	}
}

void ProblemData::removeAlunoDeTurma( Aluno* aluno, Trio< int /*campusId*/, int /*turma*/, Disciplina*> trio, std::map<int, double> diasNCreds )
{
	AlunoDemanda *alDem = aluno->getAlunoDemanda( trio.third->getId() );
	if ( alDem != NULL )
	{	
		this->mapAluno_CampusTurmaDisc[aluno].remove( trio );
		this->mapCampusTurmaDisc_AlunosDemanda[trio].remove( alDem );
	}
	else
	{
		std::cout<<"\nError: AlDem nao encontrado! Aluno "<<aluno->getAlunoId()<<" Disc"<<trio.third->getId()<<endl;
	}

	Calendario *sl = trio.third->getCalendario();
	
	std::map<int, double>::iterator itMap = diasNCreds.begin();
	for( ; itMap != diasNCreds.end(); itMap++ )
	{
		int dia = (*itMap).first;
		double value = - (*itMap).second;
		aluno->addNCredsAlocados( sl, dia, value );
	}
}


double ProblemData::cargaHorariaNaoAtendidaPorPrioridade( int prior, int alunoId )
{
	double cargaHorariaNaoAtendida = 0.0;
	ITERA_GGROUP_LESSPTR( itAlDemanda, this->listSlackDemandaAluno, AlunoDemanda )
	{
		if ( itAlDemanda->getPrioridade() != prior )
			continue;

		if ( itAlDemanda->getAlunoId() == alunoId )
		{
			int nCreds = itAlDemanda->demanda->disciplina->getTotalCreditos();
			int duracaoCred = itAlDemanda->demanda->disciplina->getTempoCredSemanaLetiva();

			cargaHorariaNaoAtendida += nCreds*duracaoCred;
		}
	}
	return cargaHorariaNaoAtendida;
}

double ProblemData::cargaHorariaRequeridaPorPrioridade( int prior, Aluno* aluno )
{
	double cargaHorariaP2 = 0.0;
	ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
	{
		Disciplina *disciplina = itAlDemanda->demanda->disciplina;
		if ( itAlDemanda->getPrioridade() == prior )
		{
			int nCreds = itAlDemanda->demanda->disciplina->getTotalCreditos();
			int duracaoCred = itAlDemanda->demanda->disciplina->getTempoCredSemanaLetiva();
			cargaHorariaP2 += nCreds*duracaoCred;
		}
	}
	return cargaHorariaP2;
}


void ProblemData::imprimeAlocacaoAlunos( int campusId, int prioridade, int cjtAlunosId, bool heuristica )
{
	stringstream ssCp;
	stringstream ssP;
	stringstream ssCjt;

	ssCp << campusId;
	ssP << prioridade;	
	ssCjt << cjtAlunosId;
		
	// Alunos ------------------------------------------------------------

	ofstream alunosFile;
	std::string alunosFilename( "AlocacaoAlunos_Cp" );    
	alunosFilename += ssCp.str();
	if ( heuristica )
		alunosFilename += "_PH";
	else
		alunosFilename += "_P";
	alunosFilename += ssP.str();
	alunosFilename += "_Cjt"; 
	alunosFilename += ssCjt.str();
    alunosFilename += ".txt";
		
	alunosFile.open(alunosFilename, ios::out);
	if (!alunosFile)
	{
		cerr << "Can't open output file " << alunosFilename << endl;
		exit(1);
	}

	std::map< Aluno*, GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >, LessPtr< Aluno > >::iterator
		itMapAlunos = this->mapAluno_CampusTurmaDisc.begin();
	
	for ( ; itMapAlunos != this->mapAluno_CampusTurmaDisc.end() ; itMapAlunos++ )
	{
		if ( itMapAlunos->first->getOferta()->getCampusId() != campusId )
			continue;

		int alunoId = itMapAlunos->first->getAlunoId();

		alunosFile << "\n\nAluno " << alunoId << ": ";

		GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >::iterator itGGroup = itMapAlunos->second.begin();
		for ( ; itGGroup != itMapAlunos->second.end() ; itGGroup++ )
		{
			int turma = (*itGGroup).second;
			int disc = (*itGGroup).third->getId();

			alunosFile << "i" << turma << "_Disc" << disc << "; ";	
		}
	}

	alunosFile.close();

	// Turmas ------------------------------------------------------------
	
	ofstream turmasFile;
	std::string turmasFilename( "AlocacaoTurmas_Cp" );
	turmasFilename += ssCp.str();
	if ( heuristica )
		turmasFilename += "_PH";
	else
		turmasFilename += "_P";
	turmasFilename += ssP.str();
	turmasFilename += "_Cjt"; 
	turmasFilename += ssCjt.str();
    turmasFilename += ".txt";
	
	turmasFile.open(turmasFilename, ios::out);
	if (!turmasFile)
	{
		std::cout << "Error: Can't open output file " << turmasFilename << endl;
		return;
	}
	
	std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
		itMapTurmas = this->mapCampusTurmaDisc_AlunosDemanda.begin();

	for ( ; itMapTurmas != this->mapCampusTurmaDisc_AlunosDemanda.end() ; itMapTurmas++ )
	{		
		int cp = itMapTurmas->first.first;
		int turma = itMapTurmas->first.second;
		int disc = itMapTurmas->first.third->getId();

		if ( cp != campusId )
			continue;

		turmasFile << "\n\ni" << turma << "_Disc" << disc << ": ";

		GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > >::iterator itGGroup = itMapTurmas->second.begin();
		for ( ; itGGroup != itMapTurmas->second.end() ; itGGroup++ )
		{
			int alunoId = itGGroup->getAlunoId();
			turmasFile << "Aluno " << alunoId << "; ";
						
		}	
	}
	
	turmasFile.close();
}
