#include "ProblemData.h"

#include <iostream>

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
   LE_SEQ( this->demandas, raiz.demandas(), Demanda );
   LE_SEQ( this->alunosDemanda, raiz.alunosDemanda(), AlunoDemanda );

   ITERA_SEQ( it_oferta,
      raiz.ofertaCursosCampi(), OfertaCurso )
   {
      Oferta * oferta = new Oferta;
      oferta->le_arvore( *it_oferta );
      this->ofertas.add( oferta );
   }

   this->parametros = new ParametrosPlanejamento;
   this->parametros->le_arvore( raiz.parametrosPlanejamento() );

   // Se a tag existir ( mesmo que esteja em branco ) no xml de entrada
   if ( raiz.atendimentosTatico().present() )
   {
      this->atendimentosTatico = new GGroup< AtendimentoCampusSolucao * > ();

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
		it_map = this->compat_cursos.begin();

	for (; it_map != this->compat_cursos.end(); it_map++ )
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

	return false;
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
	Disciplina * d = NULL;

	ITERA_GGROUP_LESSPTR( it_disc, this->disciplinas, Disciplina )
	{
		if ( it_disc->getId() == id)
		{
			d = ( *it_disc );
			return d;
		}
	}
	return d;
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
         GGroup< std::pair< int, Disciplina * > >::iterator
            it_disciplina = it_curriculo->disciplinas_periodo.begin();

         for (; it_disciplina != it_curriculo->disciplinas_periodo.end();
                it_disciplina++ )
         {
            if ( ( *it_disciplina ).second->getId() == disciplina_equivalente->getId() )
            {
               cursos_curriculos.add( std::make_pair( ( *it_curso ), ( *it_curriculo ) ) );
            }
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
      std::map< Disciplina *, GGroup< Disciplina *, LessPtr< Disciplina > > > >::iterator
      it_map = this->mapGroupDisciplinasSubstituidas.begin();

   for (; it_map != this->mapGroupDisciplinasSubstituidas.end(); it_map++ )
   {
      std::map< Disciplina *, GGroup< Disciplina *, LessPtr< Disciplina > > >::iterator
         it_disciplinas = it_map->second.begin();

      for (; it_disciplinas != it_map->second.end(); it_disciplinas++ )
      {
         Disciplina * disciplina_substituta = it_disciplinas->first;

         ITERA_GGROUP_LESSPTR( it_disc_equi, it_disciplinas->second, Disciplina )
         {
            Disciplina * disciplina_equivalente = ( *it_disc_equi );

            if ( disciplina_equivalente->getId() == disciplina->getId() )
            {
               return disciplina_substituta;
            }
         }
      }      
   }

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
      GGroup< Deslocamento * >::iterator it_tempo_campi
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
      GGroup< Deslocamento * >::iterator it_tempo_unidade
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

         GGroup< std::pair< int, Disciplina * > >::iterator it_disc_periodo
            = curriculo->disciplinas_periodo.begin();

         for (; it_disc_periodo != curriculo->disciplinas_periodo.end();
                it_disc_periodo++ )
         {
            int periodo = ( *it_disc_periodo ).first;
            Disciplina * d = ( *it_disc_periodo ).second;

            if ( d == disciplina )
            {
               idCalendarioDisc = curriculo->getSemanaLetivaId();
               break;
            }
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