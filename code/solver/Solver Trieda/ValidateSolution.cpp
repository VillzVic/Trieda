#include "ValidateSolution.h"

ValidateSolutionOp::ValidateSolutionOp( void )
{

}

ValidateSolutionOp::~ValidateSolutionOp( void )
{

}

bool ValidateSolutionOp::checkSolution(
   const ProblemData * pData, const ProblemSolution * sol )
{
   if ( !checkRestricaoAlocacaoAulas( pData, sol ) )
   {
      return false;
   }

   if ( !checkRestricaoProfessorHorario( pData, sol ) )
   {
      return false;
   }

   if ( !checkRestricaoSalaHorario( pData, sol ) )
   {
      return false;
   }

   if ( !checkRestricaoBlocoHorario( pData, sol ) )
   {
      return false;
   }

   if ( !checkRestricaoFixProfDiscSalaDiaHorario( pData, sol ) )
   {
      return false;
   }

   if ( !checkRestricaoFixProfDiscDiaHor( pData, sol ) )
   {
      return false;
   }

   /*
   if ( !checkRestricaoDisciplinaMesmoHorario( pData, sol ) )
   {
      return false;
   }
   */

   if ( !checkRestricaoDeslocamentoViavel( pData, sol ) )
   {
      return false;
   }

   if ( !checkRestricaoDeslocamentoProfessor( pData, sol ) )
   {
      return false;
   }

   return true;
}

bool ValidateSolutionOp::checkRestricaoProfessorHorario(
   const ProblemData * pData, const ProblemSolution * sol )
{
   bool result = true;

   // Armazenando todos os professores
   GGroup< Professor *, LessPtr< Professor > > professores
      = pData->getProfessores();

   // Relaciona cada professor com os dias e
   // hor�rios que eles possuem aulas alocadas na solu��o
   std::map< Professor *, GGroup< AtendimentoBase >,
      LessPtr< Professor > > mapProfessorDiaHorario;

   // Percorrendo a solu��o procurando por conflitos
   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );

      ITERA_GGROUP( it_at_campi,
         ( *sol->atendimento_campus ), AtendimentoCampus )
      {
         Campus * campus = it_at_campi->campus;

         ITERA_GGROUP( it_at_unidade,
            ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
         {
            Unidade * unidade = it_at_unidade->unidade;

            ITERA_GGROUP( it_at_sala,
               ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
            {
               Sala * sala = it_at_sala->sala;

               ITERA_GGROUP( it_at_dia,
                  ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
               {
                  int dia_semana = it_at_dia->getDiaSemana();

                  ITERA_GGROUP( it_at_turno,
                     ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
                  {
                     Turno * turno = it_at_turno->turno;

                     ITERA_GGROUP( it_at_horario,
                        ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                     {
                        if ( it_at_horario->getProfessorId() != professor->getId() )
                        {
                           continue;
                        }

                        HorarioAula * horario_aula = it_at_horario->horario_aula;

                        AtendimentoBase atendimento;

                        atendimento.professor = professor;
                        atendimento.dia_semana = dia_semana;
                        atendimento.horario_aula = horario_aula;

                        std::map< Professor *, GGroup< AtendimentoBase >,
                           LessPtr< Professor > >::iterator
                           it_find = mapProfessorDiaHorario.find( professor );

                        // J� temos esse dia/hor�rio ocupados para o professor em quest�o
                        if ( it_find != mapProfessorDiaHorario.end() )
                        {
                           GGroup< AtendimentoBase > atendimentosProf = it_find->second;

                           bool horarioOcupado = ( atendimentosProf.find( atendimento ) == atendimentosProf.end() );

                           if ( horarioOcupado )
                           {
                              #ifdef DEBUG
                              std::cout << "\nTentativa de alocar o professor " << professor->getId()
                                        << " no dia " << dia_semana
                                        << " e horario " << horario_aula->getId()
                                        << " em mais de uma aula." << std::endl << std::endl;
                              #endif

                              // A solu��o n�o � v�lida
                              result = false;
                           }
                           else
                           {
                              it_find->second.add( atendimento );
                           }
                        }
                        else
                        {
                           mapProfessorDiaHorario[ professor ].add( atendimento );
                        }
                     } // Hor�rio de aula
                  } // Turno
               } // Dia da semana
            } // Sala
         } // Unidade
      } // Campus
   } // Profesor

   return result;
}

bool ValidateSolutionOp::checkRestricaoSalaHorario(
   const ProblemData * pData, const ProblemSolution * sol )
{
   // Relaciona cada sala com os dias e hor�rios que elas possuem aulas alocadas na solu��o
   std::map< std::string, bool > mapSalaDiaHorario;

   ITERA_GGROUP( it_at_campi,
      ( *sol->atendimento_campus ), AtendimentoCampus )
   {
      Campus * campus = it_at_campi->campus;

      ITERA_GGROUP( it_at_unidade,
         ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
      {
         Unidade * unidade = it_at_unidade->unidade;

         ITERA_GGROUP( it_at_sala,
            ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
         {
            Sala * sala = it_at_sala->sala;

            ITERA_GGROUP( it_at_dia,
               ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
            {
               int dia_semana = it_at_dia->getDiaSemana();

               ITERA_GGROUP( it_at_turno,
                  ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
               {
                  Turno * turno = it_at_turno->turno;;

                  ITERA_GGROUP( it_at_horario,
                     ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                  {
                     HorarioAula * horario_aula = it_at_horario->horario_aula;

                     std::string key = "";
                     std::stringstream out;

                     out << sala->getId() << "-"
                         << dia_semana << "-"
                         << horario_aula->getId();

                     key += out.str();

                     std::map< std::string, bool >::iterator
                        it_find = mapSalaDiaHorario.find( key );

                     // J� temos esse dia/hor�rio ocupados para a sala em quest�o
                     if ( it_find != mapSalaDiaHorario.end()
                        && it_find->second == true )
                     {
                        #ifdef DEBUG
                        std::cout << "\nTentativa de alocar mais de uma "
                                  << "aula na sala " << sala->getId()
                                  << ", no dia " << dia_semana
                                  << " e horario " << horario_aula->getId()
                                  << std::endl << std::endl;
                        #endif

                        return false;
                     }
                     else
                     {
                        // Informamos que esse dia/hor�rio est�o ocupados para essa slaa
                        mapSalaDiaHorario[ key ] = true;
                     }
                  } // Hor�rio de aula
               } // Turno
            } // Dia da semana
         } // Sala
      } // Unidade
   } // Campus

   // A solu��o � v�lida
   return true;
}

bool ValidateSolutionOp::checkRestricaoBlocoHorario(
   const ProblemData * pData, const ProblemSolution * sol )
{
   // Relaciona cada sala com os dias e hor�rios que elas possuem aulas alocadas na solu��o
   std::map< std::string, Disciplina * > mapBlocoDiaHorario;

   ITERA_GGROUP_LESSPTR( it_bloco, pData->blocos, BlocoCurricular )
   {
      BlocoCurricular * bloco_curricular = ( *it_bloco );

      ITERA_GGROUP( it_at_campi,
         ( *sol->atendimento_campus ), AtendimentoCampus )
      {
         Campus * campus = it_at_campi->campus;

         ITERA_GGROUP( it_at_unidade,
            ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
         {
            Unidade * unidade = it_at_unidade->unidade;

            ITERA_GGROUP( it_at_sala,
               ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
            {
               Sala * sala = it_at_sala->sala;

               ITERA_GGROUP( it_at_dia,
                  ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
               {
                  int dia_semana = it_at_dia->getDiaSemana();

                  ITERA_GGROUP( it_at_turno,
                     ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
                  {
                     Turno * turno = it_at_turno->turno;

                     ITERA_GGROUP( it_at_horario,
                        ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                     {
                        HorarioAula * horario_aula = it_at_horario->horario_aula;

                        ITERA_GGROUP( it_at_oferta,
                           ( *it_at_horario->atendimentos_ofertas ), AtendimentoOferta )
                        {
                           int id_oferta = atoi( it_at_oferta->getOfertaCursoCampiId().c_str() );

                           Oferta * oferta = pData->findOferta( id_oferta );
                           Disciplina * disciplina = it_at_oferta->disciplina;
                           int turma = it_at_oferta->getTurma();

                           // Procuramos o per�odo no qual a disciplina
                           // � ministrada para esse bloco curricular
                           int periodoDisciplina = -1;

                           GGroup< std::pair< int, Disciplina * > >::iterator
                              it_disc_periodo = bloco_curricular->curriculo->disciplinas_periodo.begin();

                           for (; it_disc_periodo != bloco_curricular->curriculo->disciplinas_periodo.end();
                                  it_disc_periodo++ )
                           {
                              if ( disciplina->getId() == ( *it_disc_periodo ).second->getId() )
                              {
                                 periodoDisciplina = ( *it_disc_periodo ).first;
                                 break;
                              }
                           }
                           ////

                           if ( bloco_curricular->campus->getId() != oferta->campus->getId()
                              || bloco_curricular->curriculo->getId() != oferta->curriculo->getId()
                              || bloco_curricular->curso->getId() != oferta->curso->getId() 
                              || bloco_curricular->getPeriodo() != periodoDisciplina )
                           {
                              continue;
                           }

                           std::string key = "";
                           std::stringstream out;

                           // key = campus + curso + curriculo + periodo + dia + hor�rio
                           out << "-Campus: " << oferta->campus->getId() << "\n"
                               << "-Curso: " << oferta->curso->getId() << "\n"
                               << "-Curriculo: " << oferta->curriculo->getId() << "\n"
                               << "-Periodo: " << periodoDisciplina << "\n"
                               << "-Turma: " << turma << "\n"
                               << "-Dia: " << dia_semana << "\n"
                               << "-Horario: " << horario_aula->getId();

                           key += out.str();
   
                           std::map< std::string, Disciplina * >::iterator
                              it_find = mapBlocoDiaHorario.find( key );

                           // J� temos esse dia/hor�rio ocupados para o bloco curricular em quest�o
                           if ( it_find != mapBlocoDiaHorario.end()
                              && it_find->second->getId() != disciplina->getId() )
                           {
                              #ifdef DEBUG
                              std::cout << "\nTentativa de alocar mais \nde uma "
                                        << "aula ao bloco curricular \n\n" << key
                                        << std::endl << std::endl;

                              std::cout << "Disciplinas : "
                                        << it_find->second->getId() 
                                        << " e " << disciplina->getId()
                                        << std::endl << std::endl;
                              #endif;

                              return false;
                           }
                           else
                           {
                              // Informamos que esse dia/hor�rio est�o ocupados para essa slaa
                              mapBlocoDiaHorario[ key ] = disciplina;
                           }
                        }
                     } // Hor�rio de aula
                  } // Turno
               } // Dia da semana
            } // Sala
         } // Unidade
      } // Campus
   } // Bloco Curricular

   // A solu��o � v�lida
   return true;
}

bool ValidateSolutionOp::checkRestricaoFixProfDiscSalaDiaHorario(
   const ProblemData * pData, const ProblemSolution * sol )
{
   // Caso n�o exista nenhuma fixa��o do tipo
   // 'ProfDiscSalaDiaHorario' n�o � necess�rio verificar a solu��o
   if ( pData->fixacoes_Prof_Disc_Sala_Dia_Horario.size() == 0 )
   {
      return true;
   }

   // Armazena todas as combina��es ocorridas de aloca��o de aula
   // para todos os professores, disciplinas, salas, dias e hor�rios
   std::map< std::string, bool > mapProfDiscSalaDiaHorario;

   ITERA_GGROUP( it_at_campi,
      ( *sol->atendimento_campus ), AtendimentoCampus )
   {
      Campus * campus = it_at_campi->campus;

      ITERA_GGROUP( it_at_unidade,
         ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
      {
         Unidade * unidade = it_at_unidade->unidade;

         ITERA_GGROUP( it_at_sala,
            ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
         {
            Sala * sala = it_at_sala->sala;

            ITERA_GGROUP( it_at_dia,
               ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
            {
               int dia_semana = it_at_dia->getDiaSemana();

               ITERA_GGROUP( it_at_turno,
                  ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
               {
                  Turno * turno = it_at_turno->turno;

                  ITERA_GGROUP( it_at_horario,
                     ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                  {
                     Professor * professor = it_at_horario->professor;
                     HorarioAula * horario_aula = it_at_horario->horario_aula;

                     ITERA_GGROUP( it_at_oferta,
                        ( *it_at_horario->atendimentos_ofertas ), AtendimentoOferta )
                     {
                        int id_oferta = atoi( it_at_oferta->getOfertaCursoCampiId().c_str() );
                        Oferta * oferta = pData->findOferta( id_oferta );

                        std::string key = "";
                        std::stringstream out;

                        out << professor->getId() << "-"
                            << it_at_oferta->getDisciplinaId() << "-"
                            << sala->getId() << "-"
                            << dia_semana << "-"
                            << horario_aula->getId();

                        key += out.str();

                        // Armazenamos os dados do atendimento
                        mapProfDiscSalaDiaHorario[ key ] = true;
                     }

                  } // Hor�rio de aula
               } // Turno
            } // Dia da semana
         } // Sala
      } // Unidade
   } // Campus

   // Dado que agora sabemos quais foram as combina��es de prof/disc/sala/dia/hor�rio
   // atribu�das na solu��o, podemos ent�o verificar se as fixa��es foram todas atendidas
   ITERA_GGROUP_LESSPTR( it_fix,
      pData->fixacoes_Prof_Disc_Sala_Dia_Horario, Fixacao )
   {
      Fixacao * fixacao = ( *it_fix );

      Professor * professor = fixacao->professor;
      Disciplina * disciplina = fixacao->disciplina;
      Sala * sala = fixacao->sala;
      int dia_semana = fixacao->getDiaSemana();
      HorarioAula * horario_aula = fixacao->horario_aula;

      bool encontrou = false;

      std::map< std::string, bool >::iterator
         it_map = mapProfDiscSalaDiaHorario.begin();

      for (; it_map != mapProfDiscSalaDiaHorario.end();
             it_map++ )
      {
         std::string key_map = it_map->first;
         std::string key_fixacao = "";
         std::stringstream out;

         out << professor->getId() << "-"
             << disciplina->getId() << "-"
             << sala->getId() << "-"
             << dia_semana << "-"
             << horario_aula->getId();

         key_fixacao += out.str();

         if ( strcmp( key_map.c_str(), key_fixacao.c_str() ) == 0 )
         {
            encontrou = true;
            break;
         }
      }

      if ( !encontrou )
      {
         return false;
      }
   }
   
   // A solu��o � v�lida
   return true;
}

bool ValidateSolutionOp::checkRestricaoFixProfDiscDiaHor(
   const ProblemData * pData, const ProblemSolution * sol )
{
   // Caso n�o exista nenhuma fixa��o do tipo
   // 'ProfDiscDiaHor' n�o � necess�rio verificar a solu��o
   if ( pData->fixacoes_Prof_Disc_Dia_Horario.size() == 0 )
   {
      return true;
   }

   // Armazena todas as combina��es ocorridas de aloca��o de aula
   // para todos os professores, disciplinas, salas, dias e hor�rios
   std::map< std::string, bool > mapProfDiscDiaHorario;

   ITERA_GGROUP( it_at_campi,
      ( *sol->atendimento_campus ), AtendimentoCampus )
   {
      Campus * campus = it_at_campi->campus;

      ITERA_GGROUP( it_at_unidade,
         ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
      {
         Unidade * unidade = it_at_unidade->unidade;

         ITERA_GGROUP( it_at_sala,
            ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
         {
            Sala * sala = it_at_sala->sala;

            ITERA_GGROUP( it_at_dia,
               ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
            {
               int dia_semana = it_at_dia->getDiaSemana();

               ITERA_GGROUP( it_at_turno,
                  ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
               {
                  Turno * turno = it_at_turno->turno;

                  ITERA_GGROUP( it_at_horario,
                     ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                  {
                     Professor * professor = it_at_horario->professor;

                     HorarioAula * horario_aula = it_at_horario->horario_aula;

                     ITERA_GGROUP( it_at_oferta,
                        ( *it_at_horario->atendimentos_ofertas ), AtendimentoOferta )
                     {
                        int id_oferta = atoi( it_at_oferta->getOfertaCursoCampiId().c_str() );
                        Oferta * oferta = pData->findOferta( id_oferta );

                        std::string key = "";
                        std::stringstream out;

                        out << professor->getId() << "-"
                            << it_at_oferta->getDisciplinaId() << "-"
                            << dia_semana << "-"
                            << horario_aula->getId();

                        key += out.str();

                        // Armazenamos os dados do atendimento
                        mapProfDiscDiaHorario[ key ] = true;
                     }

                  } // Hor�rio de aula
               } // Turno
            } // Dia da semana
         } // Sala
      } // Unidade
   } // Campus

   // Dado que agora sabemos quais foram as combina��es de prof/disc/sala/dia/hor�rio
   // atribu�das na solu��o, podemos ent�o verificar se as fixa��es foram todas atendidas
   ITERA_GGROUP_LESSPTR( it_fix,
      pData->fixacoes_Prof_Disc_Dia_Horario, Fixacao )
   {
      Fixacao * fixacao = ( *it_fix );

      Professor * professor = fixacao->professor;
      Disciplina * disciplina = fixacao->disciplina;
      Sala * sala = fixacao->sala;
      int dia_semana = fixacao->getDiaSemana();
      HorarioAula * horario_aula = fixacao->horario_aula;

      bool encontrou = false;

      std::map< std::string, bool >::iterator
         it_map = mapProfDiscDiaHorario.begin();

      for (; it_map != mapProfDiscDiaHorario.end();
             it_map++ )
      {
         std::string key_map = it_map->first;
         std::string key_fixacao = "";
         std::stringstream out;

         out << professor->getId() << "-"
             << disciplina->getId() << "-"
             << dia_semana << "-"
             << horario_aula->getId();

         key_fixacao += out.str();

         if ( strcmp( key_map.c_str(), key_fixacao.c_str() ) == 0 )
         {
            encontrou = true;
            break;
         }
      }

      if ( !encontrou )
      {
         return false;
      }
   }
   
   // A solu��o � v�lida
   return true;
}

bool ValidateSolutionOp::checkRestricaoDeslocamentoProfessor(
   const ProblemData * pData, const ProblemSolution * sol  )
{
   // N�o foram cadastradas as informa��es
   // de deslocamento entre campus e unidades
   if ( pData->tempo_campi.size() == 0
      && pData->tempo_unidades.size() == 0 )
   {
      return true;
   }

   // Armazenando todos os professores
   GGroup< Professor *, LessPtr< Professor > > professores = pData->getProfessores();

   // Para cada par professor/dia, armazena os atendimentos correspondentes
   std::map< std::pair< Professor *, int > ,
      GGroup< AtendimentoBase *, LessPtr< AtendimentoBase > > > mapProfessorAtendimentos;

   // Percorrendo a solu��o procurando por conflitos
   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );

      ITERA_GGROUP( it_at_campi,
         ( *sol->atendimento_campus ), AtendimentoCampus )
      {
         Campus * campus = it_at_campi->campus;

         ITERA_GGROUP( it_at_unidade,
            ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
         {
            Unidade * unidade = it_at_unidade->unidade;

            ITERA_GGROUP( it_at_sala,
               ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
            {
               Sala * sala = it_at_sala->sala;

               ITERA_GGROUP( it_at_dia,
                  ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
               {
                  int dia_semana = it_at_dia->getDiaSemana();

                  ITERA_GGROUP( it_at_turno,
                     ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
                  {
                     Turno * turno = it_at_turno->turno;

                     ITERA_GGROUP( it_at_horario,
                        ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                     {
                        HorarioAula * horario_aula = it_at_horario->horario_aula;

                        if ( it_at_horario->getProfessorId() != professor->getId() )
                        {
                           continue;
                        }

                        AtendimentoBase * atendimento = new AtendimentoBase();

                        atendimento->professor = professor;
                        atendimento->campus = campus;
                        atendimento->unidade = unidade;
                        atendimento->sala = sala;
                        atendimento->turno = turno;
                        atendimento->horario_aula = horario_aula;

                        std::pair< Professor *, int > professor_dia
                           = std::make_pair( professor, dia_semana );

                        mapProfessorAtendimentos[ professor_dia ].add( atendimento );

                     } // Hor�rio de aula
                  } // Turno
               } // Dia da semana
            } // Sala
         } // Unidade
      } // Campus
   } // Profesor

   // Inicialmente, a solu��o � v�lida
   bool result = true;

   // Procura por deslocamentos invi�veis na solu��o
   std::map< std::pair< Professor *, int >,
      GGroup< AtendimentoBase *, LessPtr< AtendimentoBase > > >::iterator
      it_map = mapProfessorAtendimentos.begin();

   for (; it_map != mapProfessorAtendimentos.end() && result;
          it_map++ )
   {
      Professor * professor = it_map->first.first;
      int dia_semana = it_map->first.second;
      GGroup< AtendimentoBase *, LessPtr< AtendimentoBase > > atendimentosProf = it_map->second;

      std::vector< AtendimentoBase * > atProfVector;

      ITERA_GGROUP_LESSPTR( it_at, atendimentosProf, AtendimentoBase )
      {
         AtendimentoBase * at = ( *it_at );
         atProfVector.push_back( at );
      }

      // TODO -- ordenar 'atProfVector' por horario

      for ( int i = 0; i < (int)( atProfVector.size() - 1 ); i++ ) 
      {
         AtendimentoBase * at1 = atProfVector.at( i );
         AtendimentoBase * at2 = atProfVector.at( i + 1 );

         bool deslocViavel = deslocamentoViavel( pData, at1, at2 );

         if ( !deslocViavel )
         {
            // A solu��o n�o � v�lida
            result = false;
            break;
         }
      }
   }
   
   // Desalocando os objetos "AtendimentoBase"
   it_map = mapProfessorAtendimentos.begin();

   for (; it_map != mapProfessorAtendimentos.end();
          it_map++ )
   {
      Professor * professor = it_map->first.first;
      int dia_semana = it_map->first.second;
      GGroup< AtendimentoBase *, LessPtr< AtendimentoBase > > atendimentosProf = it_map->second;

      atendimentosProf.deleteElements();
      atendimentosProf.clear();
   }

   return result;
}

bool ValidateSolutionOp::deslocamentoViavel( const ProblemData * pData,
   const AtendimentoBase * at1, const AtendimentoBase * at2 )
{
   Turno * turno = at1->turno;
   HorarioAula * horario_aula1 = at1->horario_aula;
   HorarioAula * horario_aula2 = at2->horario_aula;

   DateTime dt1 = horario_aula1->getInicio();
   dt1.addMinutes( turno->getTempoAula() );

   DateTime dt2 = horario_aula2->getInicio();
   DateTime diff = dt2 - dt1;

   int minutosDisponiveis = diff.getDateMinutes();

   GGroup< Deslocamento * >::iterator it_desloc
      = pData->tempo_unidades.begin();

   int minutosNecessarios = -1;

   for (; it_desloc != pData->tempo_unidades.end();
          it_desloc++ )
   {
      int id_unidade1 = it_desloc->getOrigemId();
      int id_unidade2 = it_desloc->getDestinoId();

      Unidade * unidade1 = pData->refUnidade.find( id_unidade1 )->second;
      Unidade * unidade2 = pData->refUnidade.find( id_unidade2 )->second;

      if ( ( id_unidade1 == unidade1->getId() && id_unidade2 == unidade2->getId() )
         || ( id_unidade2 == unidade1->getId() && id_unidade1 == unidade2->getId() ) )
      {
         minutosNecessarios = it_desloc->getTempo();
         break;
      }
   }

   return ( minutosNecessarios <= minutosDisponiveis );
}

bool ValidateSolutionOp::checkRestricaoDeslocamentoViavel(
   const ProblemData * pData, const ProblemSolution * sol )
{
   if ( pData->tempo_campi.size() == 0
      && pData->tempo_unidades.size() == 0 )
   {
      return true;
   }

   // Para cada par bloco/dia, armazena os atendimentos correspondentes
   std::map< std::pair< BlocoCurricular *, int > ,
      GGroup< AtendimentoBase *, LessPtr< AtendimentoBase > > > mapBlocoAtendimentos;

   ITERA_GGROUP_LESSPTR( it_bloco,
      pData->blocos, BlocoCurricular )
   {
      BlocoCurricular * bloco_curricular = ( *it_bloco );

      ITERA_GGROUP( it_at_campi,
         ( *sol->atendimento_campus ), AtendimentoCampus )
      {
         Campus * campus = it_at_campi->campus;

         ITERA_GGROUP( it_at_unidade,
            ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
         {
            Unidade * unidade = it_at_unidade->unidade;

            ITERA_GGROUP( it_at_sala,
               ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
            {
               Sala * sala = it_at_sala->sala;

               ITERA_GGROUP( it_at_dia,
                  ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
               {
                  int dia_semana = it_at_dia->getDiaSemana();

                  ITERA_GGROUP( it_at_turno,
                     ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
                  {
                     Turno * turno = it_at_turno->turno;

                     ITERA_GGROUP( it_at_horario,
                        ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                     {
                        HorarioAula * horario_aula = it_at_horario->horario_aula;

                        ITERA_GGROUP( it_at_oferta,
                           ( *it_at_horario->atendimentos_ofertas ), AtendimentoOferta )
                        {
                           int id_oferta = atoi( it_at_oferta->getOfertaCursoCampiId().c_str() );

                           Oferta * oferta = pData->findOferta( id_oferta );
                           Disciplina * disciplina = it_at_oferta->disciplina;

                           // Procuramos o per�odo no qual a disciplina
                           // � ministrada para esse bloco curricular
                           int periodoDisciplina = -1;

                           GGroup< std::pair< int, Disciplina * > >::iterator
                              it_disc_periodo = bloco_curricular->curriculo->disciplinas_periodo.begin();

                           for (; it_disc_periodo != bloco_curricular->curriculo->disciplinas_periodo.end();
                                  it_disc_periodo++ )
                           {
                              if ( disciplina->getId() == ( *it_disc_periodo ).second->getId() )
                              {
                                 periodoDisciplina = ( *it_disc_periodo ).first;
                                 break;
                              }
                           }
                           ////

                           if ( bloco_curricular->campus->getId() != oferta->campus->getId()
                              || bloco_curricular->curriculo->getId() != oferta->curriculo->getId()
                              || bloco_curricular->curso->getId() != oferta->curso->getId() 
                              || bloco_curricular->getPeriodo() != periodoDisciplina )
                           {
                              continue;
                           }

                           Professor * professor = it_at_horario->professor;

                           AtendimentoBase * atendimento = new AtendimentoBase();

                           atendimento->professor = professor;
                           atendimento->campus = campus;
                           atendimento->unidade = unidade;
                           atendimento->sala = sala;
                           atendimento->turno = turno;
                           atendimento->horario_aula = horario_aula;

                           std::pair< BlocoCurricular *, int > bloco_dia
                              = std::make_pair( bloco_curricular, dia_semana );

                           mapBlocoAtendimentos[ bloco_dia ].add( atendimento );
                        }
                     } // Hor�rio de aula
                  } // Turno
               } // Dia da semana
            } // Sala
         } // Unidade
      } // Campus
   } // Bloco Curricular

   // Inicialmente, a solu��o � v�lida
   bool result = true;

   // Procura por deslocamentos invi�veis na solu��o
   std::map< std::pair< BlocoCurricular *, int >,
      GGroup< AtendimentoBase *, LessPtr< AtendimentoBase > > >::iterator
      it_map = mapBlocoAtendimentos.begin();

   for (; it_map != mapBlocoAtendimentos.end() && result;
          it_map++ )
   {
      BlocoCurricular * bloco_curricular = it_map->first.first;
      int dia_semana = it_map->first.second;
      GGroup< AtendimentoBase *,
         LessPtr< AtendimentoBase > > atendimentosBloco = it_map->second;

      std::vector< AtendimentoBase * > atBlocoVector;

      ITERA_GGROUP_LESSPTR( it_at,
         atendimentosBloco, AtendimentoBase )
      {
         AtendimentoBase * at = ( *it_at );
         atBlocoVector.push_back( at );
      }

      // TODO -- ordenar 'atBlocoVector' por hor�rio

      for ( int i = 0; i < (int)( atBlocoVector.size() - 1 ); i++ ) 
      {
         AtendimentoBase * at1 = atBlocoVector.at( i );
         AtendimentoBase * at2 = atBlocoVector.at( i + 1 );

         bool deslocViavel = deslocamentoViavel( pData, at1, at2 );

         if ( !deslocViavel )
         {
            // A solu��o n�o � v�lida
            result = false;
            break;
         }
      }
   }

   // Desalocando os objetos "AtendimentoBase"
   it_map = mapBlocoAtendimentos.begin();

   for (; it_map != mapBlocoAtendimentos.end();
          it_map++ )
   {
      BlocoCurricular * bloco_curricular = it_map->first.first;
      int dia_semana = it_map->first.second;
      GGroup< AtendimentoBase *,
         LessPtr< AtendimentoBase > > atendimentosBloco = it_map->second;

      atendimentosBloco.deleteElements();
      atendimentosBloco.clear();
   }

   return result;
}

bool ValidateSolutionOp::checkRestricaoDisciplinaMesmoHorario(
   const ProblemData * pData, const ProblemSolution * sol )
{
   // Relaciona cada disciplina com os
   // hor�rios nos quais est�o alocadas na solu��o
   std::map< Disciplina *, GGroup< HorarioAula *,
      LessPtr< HorarioAula > > > mapDisciplinaHorarioAula;

   std::map< Disciplina *, GGroup< int > > mapDisciplinaDiasLetivos;

   ITERA_GGROUP( it_at_campi,
      ( *sol->atendimento_campus ), AtendimentoCampus )
   {
      Campus * campus = it_at_campi->campus;

      ITERA_GGROUP( it_at_unidade,
         ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
      {
         Unidade * unidade = it_at_unidade->unidade;

         ITERA_GGROUP( it_at_sala,
            ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
         {
            Sala * sala = it_at_sala->sala;

            ITERA_GGROUP( it_at_dia,
               ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
            {
               int dia_semana = it_at_dia->getDiaSemana();

               ITERA_GGROUP( it_at_turno,
                  ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
               {
                  Turno * turno = it_at_turno->turno;

                  ITERA_GGROUP( it_at_horario,
                     ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                  {
                     HorarioAula * horario_aula = it_at_horario->horario_aula;

                     ITERA_GGROUP( it_at_oferta,
                        ( *it_at_horario->atendimentos_ofertas ), AtendimentoOferta )
                     {
                        Disciplina * disciplina = it_at_oferta->disciplina;

                        mapDisciplinaHorarioAula[ disciplina ].add( horario_aula );
                        mapDisciplinaDiasLetivos[ disciplina ].add( dia_semana );
                     }
                  } // Hor�rio de aula
               } // Turno
            } // Dia da semana
         } // Sala
      } // Unidade
   } // Campus

   // O objetivo aqui � verificar se houve alguma aloca��o
   // de disciplina que n�o obedeceu a regra de utiliza��o
   // dos mesmos hor�rios de aula em dias distintos.
   std::map< Disciplina *, GGroup< HorarioAula *,
      LessPtr< HorarioAula > > >::iterator it_map = mapDisciplinaHorarioAula.begin();

   for (; it_map != mapDisciplinaHorarioAula.end();
          it_map++ )
   {
      Disciplina * disciplina = it_map->first;

      int creditos_disciplina = disciplina->getTotalCreditos();
      int horarios_alocados = it_map->second.size();
      double dias_letivos_disciplina = mapDisciplinaDiasLetivos[ disciplina ].size();

      // SE Hor�rios > ( Cr�ditos / Dias Letivos ), ENT�O a regra foi violada
      if ( horarios_alocados > ( creditos_disciplina / dias_letivos_disciplina ) )
      {
         std::cout << "\nA disciplina " << disciplina->getId()
                   << " nao obedece a alocacao \nde de mesmos "
                   << "horarios em dias letivos distintos." << std::endl;

         return false;
      }
   }

   // A solu��o � v�lida
   return true;
}

bool ValidateSolutionOp::checkRestricaoAlocacaoAulas(
   const ProblemData * pData, const ProblemSolution * sol )
{
   // Para cada par professor/dia, armazena os atendimentos correspondentes
   std::map< Aula * , GGroup< AtendimentoBase *,
      LessPtr< AtendimentoBase > >, LessPtr< Aula > > mapAulaAtendimentos;

   ITERA_GGROUP( it_at_campi,
      ( *sol->atendimento_campus ), AtendimentoCampus )
   {
      Campus * campus = it_at_campi->campus;

      ITERA_GGROUP( it_at_unidade,
         ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
      {
         Unidade * unidade = it_at_unidade->unidade;

         ITERA_GGROUP( it_at_sala,
            ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
         {
            Sala * sala = it_at_sala->sala;

            ITERA_GGROUP( it_at_dia,
               ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
            {
               int dia_semana = it_at_dia->getDiaSemana();

               ITERA_GGROUP( it_at_turno,
                  ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
               {
                  // Turno * turno = pData->findTurno( it_at_turno->getTurnoId() );
                  Turno * turno = it_at_turno->turno;

                  ITERA_GGROUP( it_at_horario,
                     ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                  {
                     HorarioAula * horario_aula = it_at_horario->horario_aula;
                     Professor * professor = it_at_horario->professor;

                     ITERA_GGROUP( it_oferta,
                        ( *it_at_horario->atendimentos_ofertas ), AtendimentoOferta )
                     {
                        int turma = it_oferta->getTurma();
                        Disciplina * disciplina = it_oferta->disciplina;

                        AtendimentoBase * atendimento = new AtendimentoBase();

                        atendimento->professor = professor;
                        atendimento->turno = turno;
                        atendimento->disciplina = disciplina;
                        atendimento->campus = campus;
                        atendimento->unidade = unidade;
                        atendimento->sala = sala;
                        atendimento->dia_semana = dia_semana;
                        atendimento->turma = turma;
                        atendimento->quantidade = it_oferta->getQuantidade();
                        atendimento->horario_aula = horario_aula;

                        Aula * aula = new Aula();

                        aula->setDisciplina( disciplina );
                        aula->setSala( sala );
                        aula->setDiaSemana( dia_semana );
                        aula->setTurma( turma );
                        aula->setQuantidade( it_oferta->getQuantidade() );

                        mapAulaAtendimentos[ aula ].add( atendimento );
                     } // Oferta
                  } // Hor�rio de aula
               } // Turno
            } // Dia da semana
         } // Sala
      } // Unidade
   } // Campus

   int aulasNaoAtendidas = 0;

   // Aulas que devem ser atendidas
   ITERA_GGROUP_LESSPTR( it_aula, pData->aulas, Aula )
   {
      Aula * aula = ( *it_aula );
      bool encontrou = false;

      // Aulas que foram atendidas na solu��o
      std::map< Aula *, GGroup< AtendimentoBase *, LessPtr< AtendimentoBase > >,
         LessPtr< Aula > >::iterator it_map = mapAulaAtendimentos.begin();

      for (; it_map != mapAulaAtendimentos.end();
             it_map++ )
      {
         Aula * aulaAtendida = it_map->first;
         GGroup< AtendimentoBase *,
            LessPtr< AtendimentoBase > > atendimentosAula = it_map->second;

         int creditosAtendidos = atendimentosAula.size();

         if ( aula->getDiaSemana() == aulaAtendida->getDiaSemana()
            && aula->getDisciplina() == aulaAtendida->getDisciplina()
            && aula->getQuantidade() == aulaAtendida->getQuantidade()
            && aula->getSala() == aulaAtendida->getSala()
            && aula->getTurma() == aulaAtendida->getTurma()
            && aula->getTotalCreditos() == creditosAtendidos )
         {
            encontrou = true;
            break;
         }
      }

      if ( !encontrou )
      {
         aula->toString();
         aulasNaoAtendidas++;
      }
   }
   
   // Desalocando os objetos "AtendimentoBase"
   std::map< Aula *, GGroup< AtendimentoBase *, LessPtr< AtendimentoBase > >,
      LessPtr< Aula > >::iterator it_map = mapAulaAtendimentos.begin();

   for (; it_map != mapAulaAtendimentos.end();
      it_map++ )
   {
      Aula * aulaAtendida = it_map->first;
      GGroup< AtendimentoBase *,
         LessPtr< AtendimentoBase > > atendimentosAula = it_map->second;

      atendimentosAula.deleteElements();
      atendimentosAula.clear();
   }

   if ( aulasNaoAtendidas != 0 )
   {
      std::cout << "\nUm total de " << aulasNaoAtendidas
                << " aulas nao foram atendidas na solucao."
                << std::endl << std::endl;

      return false;
   }

   // A solu��o � v�lida
   return true;
}
