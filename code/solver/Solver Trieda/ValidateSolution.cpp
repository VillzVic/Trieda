#include "ValidateSolution.h"

// Método utilizado para ordenas os atendimentos de acordo com o horário de aula,
// considerando que todos esses atendimentos pertencem ao mesmo dia da semana
int sortAtendimentosBase( AtendimentoBase * at1, AtendimentoBase * at2 )
{
   if ( at1->getHorarioAula() < at2->getHorarioAula() )
   {
      return -1;
   }
   else if ( at2->getHorarioAula() < at1->getHorarioAula() )
   {
      return 1;
   }

   return 0;
}

ValidateSolutionOp::ValidateSolutionOp( ProblemData * data  )
{
   this->pData = data;
   this->solution = NULL;
}

ValidateSolutionOp::~ValidateSolutionOp()
{
   this->pData = NULL;
   this->solution = NULL;
}

bool ValidateSolutionOp::checkSolution( ProblemSolution *  sol )
{
   this->setSolution( sol );

   if ( !checkRestricaoProfessorHorario() )
   {
      return false;
   }

   if ( !checkRestricaoSalaHorario() )
   {
      return false;
   }

   if ( !checkRestricaoBlocoHorario() )
   {
      return false;
   }

   if ( !checkRestricaoFixProfDiscSalaDiaHorario() )
   {
      return false;
   }

   /*
   if ( !checkRestricaoFixProfDiscDiaHor() )
   {
      return false;
   }
   */

   if ( !checkRestricaoDisciplinaMesmoHorario() )
   {
      return false;
   }

   if ( !checkRestricaoDeslocamentoViavel() )
   {
      return false;
   }

   if ( !checkRestricaoDeslocamentoProfessor() )
   {
      return false;
   }

   return true;
}

bool ValidateSolutionOp::checkRestricaoProfessorHorario()
{
   // Armazenando todos os professores
   GGroup< Professor *, LessPtr< Professor > > professores = pData->getProfessores();

   // Relaciona cada professor com os dias e horários que eles possuem aulas alocadas na solução
   std::map< std::string, bool > mapProfessorDiaHorario;

   // Percorrendo a solução procurando por conflitos
   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );

      ITERA_GGROUP( it_at_campi, ( *solution->atendimento_campus ), AtendimentoCampus )
      {
         Campus * campus = pData->refCampus[ it_at_campi->getId() ];

         ITERA_GGROUP( it_at_unidade, ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
         {
            Unidade * unidade = pData->refUnidade[ it_at_unidade->getId() ];

            ITERA_GGROUP( it_at_sala, ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
            {
               Sala * sala = pData->refSala[ it_at_sala->getId() ];

               ITERA_GGROUP( it_at_dia, ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
               {
                  int dia_semana = it_at_dia->getDiaSemana();

                  ITERA_GGROUP( it_at_turno, ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
                  {
                     Turno * turno = pData->findTurno( it_at_turno->getTurnoId() );

                     ITERA_GGROUP( it_at_horario, ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                     {
                        HorarioAula * horario_aula = pData->findHorarioAula( it_at_horario->getHorarioAulaId() );

                        if ( it_at_horario->getProfessorId() != professor->getId() )
                        {
                           continue;
                        }

                        std::string key = "";
                        std::stringstream out;

                        out << professor->getId() << "-"
                            << dia_semana << "-"
                            << horario_aula->getId();

                        key += out.str();

                        std::map< std::string, bool >::iterator
                           it_find = mapProfessorDiaHorario.find( key );

                        // Já temos esse dia/horário ocupados para o professor em questão
                        if ( it_find != mapProfessorDiaHorario.end() && it_find->second == true )
                        {
                           std::cout << "\nTentativa de alocar o professor " << professor->getId()
                                     << " no dia " << dia_semana
                                     << " e horario " << horario_aula->getId()
                                     << " em mais de uma aula." << std::endl << std::endl;

                           return false;
                        }
                        else
                        {
                           // Informamos que esse dia/horário estão ocupados para esse professor
                           mapProfessorDiaHorario[ key ] = true;
                        }
                     } // Horário de aula
                  } // Turno
               } // Dia da semana
            } // Sala
         } // Unidade
      } // Campus
   } // Profesor

   // A solução é válida
   return true;
}

bool ValidateSolutionOp::checkRestricaoSalaHorario()
{
   // Relaciona cada sala com os dias e horários que elas possuem aulas alocadas na solução
   std::map< std::string, bool > mapSalaDiaHorario;

   ITERA_GGROUP( it_at_campi, ( *solution->atendimento_campus ), AtendimentoCampus )
   {
      Campus * campus = pData->refCampus[ it_at_campi->getId() ];

      ITERA_GGROUP( it_at_unidade, ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
      {
         Unidade * unidade = pData->refUnidade[ it_at_unidade->getId() ];

         ITERA_GGROUP( it_at_sala, ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
         {
            Sala * sala = pData->refSala[ it_at_sala->getId() ];

            ITERA_GGROUP( it_at_dia, ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
            {
               int dia_semana = it_at_dia->getDiaSemana();

               ITERA_GGROUP( it_at_turno, ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
               {
                  Turno * turno = pData->findTurno( it_at_turno->getTurnoId() );

                  ITERA_GGROUP( it_at_horario, ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                  {
                     HorarioAula * horario_aula = pData->findHorarioAula(
                        it_at_horario->getHorarioAulaId() );

                     std::string key = "";
                     std::stringstream out;

                     out << sala->getId() << "-"
                         << dia_semana << "-"
                         << horario_aula->getId();

                     key += out.str();

                     std::map< std::string, bool >::iterator
                        it_find = mapSalaDiaHorario.find( key );

                     // Já temos esse dia/horário ocupados para a sala em questão
                     if ( it_find != mapSalaDiaHorario.end()
                        && it_find->second == true )
                     {
                        std::cout << "\nTentativa de alocar mais de uma "
                                  << "aula na sala " << sala->getId()
                                  << ", no dia " << dia_semana
                                  << " e horario " << horario_aula->getId()
                                  << std::endl << std::endl;

                        return false;
                     }
                     else
                     {
                        // Informamos que esse dia/horário estão ocupados para essa slaa
                        mapSalaDiaHorario[ key ] = true;
                     }
                  } // Horário de aula
               } // Turno
            } // Dia da semana
         } // Sala
      } // Unidade
   } // Campus

   // A solução é válida
   return true;
}

bool ValidateSolutionOp::checkRestricaoBlocoHorario()
{
   // Relaciona cada sala com os dias e horários que elas possuem aulas alocadas na solução
   std::map< std::string, Disciplina * > mapBlocoDiaHorario;

   ITERA_GGROUP_LESSPTR( it_bloco, pData->blocos, BlocoCurricular )
   {
      BlocoCurricular * bloco_curricular = ( *it_bloco );

      ITERA_GGROUP( it_at_campi, ( *solution->atendimento_campus ), AtendimentoCampus )
      {
         Campus * campus = pData->refCampus[ it_at_campi->getId() ];

         ITERA_GGROUP( it_at_unidade, ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
         {
            Unidade * unidade = pData->refUnidade[ it_at_unidade->getId() ];

            ITERA_GGROUP( it_at_sala, ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
            {
               Sala * sala = pData->refSala[ it_at_sala->getId() ];

               ITERA_GGROUP( it_at_dia, ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
               {
                  int dia_semana = it_at_dia->getDiaSemana();

                  ITERA_GGROUP( it_at_turno, ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
                  {
                     Turno * turno = pData->findTurno( it_at_turno->getTurnoId() );

                     ITERA_GGROUP( it_at_horario, ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                     {
                        HorarioAula * horario_aula = pData->findHorarioAula( it_at_horario->getHorarioAulaId() );

                        ITERA_GGROUP( it_at_oferta, ( *it_at_horario->atendimentos_ofertas ), AtendimentoOferta )
                        {
                           int id_oferta = atoi( it_at_oferta->getOfertaCursoCampiId().c_str() );

                           Oferta * oferta = pData->refOfertas[ id_oferta ];
                           Disciplina * disciplina = pData->refDisciplinas[ it_at_oferta->getDisciplinaId() ];
                           int turma = it_at_oferta->getTurma();

                           // Procuramos o período no qual a disciplina é ministrada a esse bloco curricular
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

                           // key = campus + curso + curriculo + periodo + dia + horário
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

                           // Já temos esse dia/horário ocupados para o bloco curricular em questão
                           if ( it_find != mapBlocoDiaHorario.end()
                              && it_find->second->getId() != disciplina->getId() )
                           {
                              std::cout << "\nTentativa de alocar mais \nde uma "
                                        << "aula ao bloco curricular \n\n" << key
                                        << std::endl << std::endl;

                              std::cout << "Disciplinas : "
                                        << it_find->second->getId() 
                                        << " e " << disciplina->getId()
                                        << std::endl << std::endl;

                              return false;
                           }
                           else
                           {
                              // Informamos que esse dia/horário estão ocupados para essa slaa
                              mapBlocoDiaHorario[ key ] = disciplina;
                           }
                        }
                     } // Horário de aula
                  } // Turno
               } // Dia da semana
            } // Sala
         } // Unidade
      } // Campus
   } // Bloco Curricular

   // A solução é válida
   return true;
}

bool ValidateSolutionOp::checkRestricaoFixProfDiscSalaDiaHorario()
{
   // Caso não exista nenhuma fixação do tipo
   // 'ProfDiscSalaDiaHorario' não é necessário verificar a solução
   if ( pData->fixacoes_Prof_Disc_Sala_Dia_Horario.size() == 0 )
   {
      return true;
   }

   // Armazena todas as combinações ocorridas de alocação de aula
   // para todos os professores, disciplinas, salas, dias e horários
   std::map< std::string, bool > mapProfDiscSalaDiaHorario;

   ITERA_GGROUP( it_at_campi, ( *solution->atendimento_campus ), AtendimentoCampus )
   {
      Campus * campus = pData->refCampus[ it_at_campi->getId() ];

      ITERA_GGROUP( it_at_unidade, ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
      {
         Unidade * unidade = pData->refUnidade[ it_at_unidade->getId() ];

         ITERA_GGROUP( it_at_sala, ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
         {
            Sala * sala = pData->refSala[ it_at_sala->getId() ];

            ITERA_GGROUP( it_at_dia, ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
            {
               int dia_semana = it_at_dia->getDiaSemana();

               ITERA_GGROUP( it_at_turno, ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
               {
                  Turno * turno = pData->findTurno( it_at_turno->getTurnoId() );

                  ITERA_GGROUP( it_at_horario, ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                  {
                     Professor * professor = pData->findProfessor( it_at_horario->getProfessorId() );

                     HorarioAula * horario_aula = pData->findHorarioAula(
                        it_at_horario->getHorarioAulaId() );

                     ITERA_GGROUP( it_at_oferta, ( *it_at_horario->atendimentos_ofertas ), AtendimentoOferta )
                     {
                        int id_oferta = atoi( it_at_oferta->getOfertaCursoCampiId().c_str() );
                        Oferta * oferta = pData->refOfertas[ id_oferta ];

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

                  } // Horário de aula
               } // Turno
            } // Dia da semana
         } // Sala
      } // Unidade
   } // Campus

   // Dado que agora sabemos quais foram as combinações de prof/disc/sala/dia/horário
   // atribuídas na solução, podemos então verificar se as fixações foram todas atendidas
   ITERA_GGROUP_LESSPTR( it_fix, pData->fixacoes_Prof_Disc_Sala_Dia_Horario, Fixacao )
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
   
   // A solução é válida
   return true;
}

bool ValidateSolutionOp::checkRestricaoFixProfDiscDiaHor()
{
   // Caso não exista nenhuma fixação do tipo
   // 'ProfDiscDiaHor' não é necessário verificar a solução
   if ( pData->fixacoes_Prof_Disc_Dia_Horario.size() == 0 )
   {
      return true;
   }

   // Armazena todas as combinações ocorridas de alocação de aula
   // para todos os professores, disciplinas, salas, dias e horários
   std::map< std::string, bool > mapProfDiscDiaHorario;

   ITERA_GGROUP( it_at_campi, ( *solution->atendimento_campus ), AtendimentoCampus )
   {
      Campus * campus = pData->refCampus[ it_at_campi->getId() ];

      ITERA_GGROUP( it_at_unidade, ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
      {
         Unidade * unidade = pData->refUnidade[ it_at_unidade->getId() ];

         ITERA_GGROUP( it_at_sala, ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
         {
            Sala * sala = pData->refSala[ it_at_sala->getId() ];

            ITERA_GGROUP( it_at_dia, ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
            {
               int dia_semana = it_at_dia->getDiaSemana();

               ITERA_GGROUP( it_at_turno, ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
               {
                  Turno * turno = pData->findTurno( it_at_turno->getTurnoId() );

                  ITERA_GGROUP( it_at_horario, ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                  {
                     Professor * professor = pData->findProfessor( it_at_horario->getProfessorId() );

                     HorarioAula * horario_aula = pData->findHorarioAula(
                        it_at_horario->getHorarioAulaId() );

                     ITERA_GGROUP( it_at_oferta, ( *it_at_horario->atendimentos_ofertas ), AtendimentoOferta )
                     {
                        int id_oferta = atoi( it_at_oferta->getOfertaCursoCampiId().c_str() );
                        Oferta * oferta = pData->refOfertas[ id_oferta ];

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

                  } // Horário de aula
               } // Turno
            } // Dia da semana
         } // Sala
      } // Unidade
   } // Campus

   // Dado que agora sabemos quais foram as combinações de prof/disc/sala/dia/horário
   // atribuídas na solução, podemos então verificar se as fixações foram todas atendidas
   ITERA_GGROUP_LESSPTR( it_fix, pData->fixacoes_Prof_Disc_Dia_Horario, Fixacao )
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
   
   // A solução é válida
   return true;
}

bool ValidateSolutionOp::checkRestricaoDeslocamentoProfessor()
{
   // Não foram cadastradas as informações
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

   // Percorrendo a solução procurando por conflitos
   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );

      ITERA_GGROUP( it_at_campi, ( *solution->atendimento_campus ), AtendimentoCampus )
      {
         Campus * campus = pData->refCampus[ it_at_campi->getId() ];

         ITERA_GGROUP( it_at_unidade, ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
         {
            Unidade * unidade = pData->refUnidade[ it_at_unidade->getId() ];

            ITERA_GGROUP( it_at_sala, ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
            {
               Sala * sala = pData->refSala[ it_at_sala->getId() ];

               ITERA_GGROUP( it_at_dia, ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
               {
                  int dia_semana = it_at_dia->getDiaSemana();

                  ITERA_GGROUP( it_at_turno, ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
                  {
                     Turno * turno = pData->findTurno( it_at_turno->getTurnoId() );

                     ITERA_GGROUP( it_at_horario, ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                     {
                        HorarioAula * horario_aula = pData->findHorarioAula( it_at_horario->getHorarioAulaId() );

                        if ( it_at_horario->getProfessorId() != professor->getId() )
                        {
                           continue;
                        }

                        AtendimentoBase * atendimento = new AtendimentoBase();

                        atendimento->setProfessor( professor );
                        atendimento->setCampus( campus );
                        atendimento->setUnidade( unidade );
                        atendimento->setSala( sala );
                        atendimento->setTurno( turno );
                        atendimento->setHorarioAula( horario_aula );

                        std::pair< Professor *, int > professor_dia
                           = std::make_pair( professor, dia_semana );

                        mapProfessorAtendimentos[ professor_dia ].add( atendimento );

                     } // Horário de aula
                  } // Turno
               } // Dia da semana
            } // Sala
         } // Unidade
      } // Campus
   } // Profesor

   // Procura por deslocamentos inviáveis na solução
   std::map< std::pair< Professor *, int >,
      GGroup< AtendimentoBase *, LessPtr< AtendimentoBase > > >::iterator
      it_map = mapProfessorAtendimentos.begin();

   for (; it_map != mapProfessorAtendimentos.end();
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

      std::sort( atProfVector.begin(),
         atProfVector.end(), sortAtendimentosBase );

      for ( int i = 0; i < (int)( atProfVector.size() - 1 ); i++ ) 
      {
         AtendimentoBase * at1 = atProfVector.at( i );
         AtendimentoBase * at2 = atProfVector.at( i + 1 );

         bool deslocViavel = deslocamentoViavel( at1, at2 );

         if ( !deslocViavel )
         {
            return false;
         }
      }
   }

   // A solução é válida
   return true;
}

bool ValidateSolutionOp::deslocamentoViavel(
   AtendimentoBase * at1, AtendimentoBase * at2 )
{
   DateTime dt1 = at1->getHorarioAula()->getInicio();
   dt1.addMinutes( at1->getTurno()->getTempoAula() );

   DateTime dt2 = at2->getHorarioAula()->getInicio();
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

      if ( ( id_unidade1 == at1->getUnidade()->getId() && id_unidade2 == at2->getUnidade()->getId() )
         || ( id_unidade2 == at1->getUnidade()->getId() && id_unidade1 == at2->getUnidade()->getId() ) )
      {
         minutosNecessarios = it_desloc->getTempo();
         break;
      }
   }

   return ( minutosNecessarios <= minutosDisponiveis );
}

bool ValidateSolutionOp::checkRestricaoDeslocamentoViavel()
{
   if ( pData->tempo_campi.size() == 0
      && pData->tempo_unidades.size() == 0 )
   {
      return true;
   }

   // Para cada par bloco/dia, armazena os atendimentos correspondentes
   std::map< std::pair< BlocoCurricular *, int > ,
      GGroup< AtendimentoBase *, LessPtr< AtendimentoBase > > > mapBlocoAtendimentos;

   ITERA_GGROUP_LESSPTR( it_bloco, pData->blocos, BlocoCurricular )
   {
      BlocoCurricular * bloco_curricular = ( *it_bloco );

      ITERA_GGROUP( it_at_campi, ( *solution->atendimento_campus ), AtendimentoCampus )
      {
         Campus * campus = pData->refCampus[ it_at_campi->getId() ];

         ITERA_GGROUP( it_at_unidade, ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
         {
            Unidade * unidade = pData->refUnidade[ it_at_unidade->getId() ];

            ITERA_GGROUP( it_at_sala, ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
            {
               Sala * sala = pData->refSala[ it_at_sala->getId() ];

               ITERA_GGROUP( it_at_dia, ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
               {
                  int dia_semana = it_at_dia->getDiaSemana();

                  ITERA_GGROUP( it_at_turno, ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
                  {
                     Turno * turno = pData->findTurno( it_at_turno->getTurnoId() );

                     ITERA_GGROUP( it_at_horario, ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                     {
                        HorarioAula * horario_aula = pData->findHorarioAula( it_at_horario->getHorarioAulaId() );

                        ITERA_GGROUP( it_at_oferta, ( *it_at_horario->atendimentos_ofertas ), AtendimentoOferta )
                        {
                           int id_oferta = atoi( it_at_oferta->getOfertaCursoCampiId().c_str() );

                           Oferta * oferta = pData->refOfertas[ id_oferta ];
                           Disciplina * disciplina = pData->refDisciplinas[ it_at_oferta->getDisciplinaId() ];

                           // Procuramos o período no qual a disciplina é ministrada a esse bloco curricular
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

                           AtendimentoBase * atendimento = new AtendimentoBase();

                           Professor * professor = pData->findProfessor(
                              it_at_horario->getProfessorId() );

                           atendimento->setProfessor( professor );
                           atendimento->setCampus( campus );
                           atendimento->setUnidade( unidade );
                           atendimento->setSala( sala );
                           atendimento->setTurno( turno );
                           atendimento->setHorarioAula( horario_aula );

                           std::pair< BlocoCurricular *, int > bloco_dia
                              = std::make_pair( bloco_curricular, dia_semana );

                           mapBlocoAtendimentos[ bloco_dia ].add( atendimento );
                        }
                     } // Horário de aula
                  } // Turno
               } // Dia da semana
            } // Sala
         } // Unidade
      } // Campus
   } // Bloco Curricular

   // Procura por deslocamentos inviáveis na solução
   std::map< std::pair< BlocoCurricular *, int >,
      GGroup< AtendimentoBase *, LessPtr< AtendimentoBase > > >::iterator
      it_map = mapBlocoAtendimentos.begin();

   for (; it_map != mapBlocoAtendimentos.end();
          it_map++ )
   {
      BlocoCurricular * bloco_curricular = it_map->first.first;
      int dia_semana = it_map->first.second;
      GGroup< AtendimentoBase *, LessPtr< AtendimentoBase > > atendimentosBloco = it_map->second;

      std::vector< AtendimentoBase * > atBlocoVector;

      ITERA_GGROUP_LESSPTR( it_at, atendimentosBloco, AtendimentoBase )
      {
         AtendimentoBase * at = ( *it_at );
         atBlocoVector.push_back( at );
      }

      std::sort( atBlocoVector.begin(),
         atBlocoVector.end(), sortAtendimentosBase );

      for ( int i = 0; i < (int)( atBlocoVector.size() - 1 ); i++ ) 
      {
         AtendimentoBase * at1 = atBlocoVector.at( i );
         AtendimentoBase * at2 = atBlocoVector.at( i + 1 );

         bool deslocViavel = deslocamentoViavel( at1, at2 );

         if ( !deslocViavel )
         {
            return false;
         }
      }
   }

   // A solução é válida
   return true;
}

bool ValidateSolutionOp::checkRestricaoDisciplinaMesmoHorario()
{
   // Relaciona cada disciplina com os horários nos quais estão alocadas na solução
   std::map< Disciplina *, GGroup< HorarioAula *, LessPtr< HorarioAula > > > mapDisciplinaHorarioAula;

   std::map< Disciplina *, GGroup< int > > mapDisciplinaDiasLetivos;

   ITERA_GGROUP( it_at_campi, ( *solution->atendimento_campus ), AtendimentoCampus )
   {
      Campus * campus = pData->refCampus[ it_at_campi->getId() ];

      ITERA_GGROUP( it_at_unidade, ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
      {
         Unidade * unidade = pData->refUnidade[ it_at_unidade->getId() ];

         ITERA_GGROUP( it_at_sala, ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
         {
            Sala * sala = pData->refSala[ it_at_sala->getId() ];

            ITERA_GGROUP( it_at_dia, ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
            {
               int dia_semana = it_at_dia->getDiaSemana();

               ITERA_GGROUP( it_at_turno, ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
               {
                  Turno * turno = pData->findTurno( it_at_turno->getTurnoId() );

                  ITERA_GGROUP( it_at_horario,
                     ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                  {
                     HorarioAula * horario_aula = pData->findHorarioAula(
                        it_at_horario->getHorarioAulaId() );

                     ITERA_GGROUP( it_at_oferta,
                        ( *it_at_horario->atendimentos_ofertas ), AtendimentoOferta )
                     {
                        Disciplina * disciplina = pData->refDisciplinas
                           [ it_at_oferta->getDisciplinaId() ];

                        mapDisciplinaHorarioAula[ disciplina ].add( horario_aula );

                        mapDisciplinaDiasLetivos[ disciplina ].add( dia_semana );
                     }
                  } // Horário de aula
               } // Turno
            } // Dia da semana
         } // Sala
      } // Unidade
   } // Campus

   // O objetivo aqui é verificar se houve alguma alocação
   // de disciplina que não obedeceu a regra de utilização
   // dos mesmos horários de aula em dias distintos.
   std::map< Disciplina *, GGroup< HorarioAula *,
      LessPtr< HorarioAula > > >::iterator it_map = mapDisciplinaHorarioAula.begin();

   for (; it_map != mapDisciplinaHorarioAula.end();
          it_map++ )
   {
      Disciplina * disciplina = it_map->first;

      int creditos_disciplina = disciplina->getTotalCreditos();
      int horarios_alocados = it_map->second.size();
      double dias_letivos_disciplina = mapDisciplinaDiasLetivos[ disciplina ].size();

      // SE Horários > ( Créditos / Dias Letivos ), ENTÃO a regra foi violada
      if ( horarios_alocados > ( creditos_disciplina / dias_letivos_disciplina ) )
      {
         std::cout << "\nA disciplina " << disciplina->getId()
                   << " nao obedece a alocacao \nde de mesmos "
                   << "horarios em dias letivos distintos." << std::endl;

         return false;
      }
   }

   // A solução é válida
   return true;
}
