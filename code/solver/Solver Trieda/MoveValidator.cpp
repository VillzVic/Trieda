#include "MoveValidator.h"

MoveValidator::MoveValidator( ProblemData * pD )
: problem_data( pD )
{
}

MoveValidator::~MoveValidator()
{
}

//bool MoveValidator::isValid(Aula & aX, Aula & aY)
//{
//   return true;
//}

bool MoveValidator::canSwapSchedule( Aula & aX, Aula & aY, SolucaoOperacional & solOp ) const
{
   std::vector<HorarioAula*> novosHorariosAX;
   std::vector<HorarioAula*> novosHorariosAY;

   std::map< Aula *, std::pair< Professor *, std::vector< HorarioAula * > >, LessPtr<Aula> >::const_iterator
      itAula = solOp.blocoAulas.find(&aX);

   if(itAula != solOp.blocoAulas.end())
      novosHorariosAY = itAula->second.second;
   else
   { std::cout << "Erro em <bool MoveValidator::canSwapSchedule( Aula & aX, Aula & aY, SolucaoOperacional & solOp ) const>.\n"; exit(1); }

   itAula = solOp.blocoAulas.find(&aY);

   if(itAula != solOp.blocoAulas.end())
      novosHorariosAY = itAula->second.second;
   else
   { std::cout << "Erro em <bool MoveValidator::canSwapSchedule( Aula & aX, Aula & aY, SolucaoOperacional & solOp ) const>.\n"; exit(1); }

   return ( !checkBlockConflict( aX, novosHorariosAX, solOp )
      && !checkBlockConflict( aY, novosHorariosAY, solOp )
      && checkClassAndLessonDisponibility( aX, novosHorariosAX, solOp )
      && checkClassAndLessonDisponibility( aY, novosHorariosAY, solOp ) );
}

bool MoveValidator::checkBlockConflict( Aula & aula, std::vector<HorarioAula*> & novosHorariosAula, SolucaoOperacional & solOp ) const
{
   /* Nesse método, apenas verificamos se os novos horários para a aula conflitam com algum horário
   de outra aula que pertença ao mesmo bloco. Porém, pode acontecer o caso em que os novos horários
   não pertençam à lista de horarios letivos da disciplina em questão. Esse check é tratado no método 
   <checkClassAndLessonDisponibility>. Portanto, não há necessidade de fazer esse check aqui. */

   // Procurando o(s) bloco(s) curricular(es) ao(s) qual(is) a aula em questao pertence.
   std::map< Aula *, GGroup< BlocoCurricular *, LessPtr< BlocoCurricular > > >::iterator
      itAulaBlocosCurriculares = problem_data->aulaBlocosCurriculares.find( &aula );

   if ( itAulaBlocosCurriculares == problem_data->aulaBlocosCurriculares.end() )
   {
      std::cout << "Na funcao <SolucaoOperacional::checkConflitoBlocoCurricular>"
         << "alguma aula nao foi encontrada." << std::endl;

      exit(1);
   }

   // Para cada Bloco Curricular ao qual a aula pertence
   ITERA_GGROUP_LESSPTR( itBlocoCurric, itAulaBlocosCurriculares->second, BlocoCurricular )
   {
      // Procurando a(s) aula(s) do bloco curricular selecionado.
      std::map< BlocoCurricular *, std::map< int /*dia*/, GGroup< Aula *, LessPtr< Aula > > > >::iterator
         itBlocoCurricularAulas = problem_data->blocoCurricularDiaAulas.find( *itBlocoCurric );

      if ( itBlocoCurricularAulas == problem_data->blocoCurricularDiaAulas.end() )
      {
         std::cout << "Na funcao <SolucaoOperacional::checkConflitoBlocoCurricular>"
            << "algum bloco nao foi encontrado." << std::endl;

         exit(1);
      }

	  // Procurando as aulas do bloco em selecionado que possuam o dia da aula em questao.
      std::map< int /*dia*/, GGroup< Aula *, LessPtr< Aula > > >::iterator 
         itBlocoCurricularAulasDia = itBlocoCurricularAulas->second.find( aula.getDiaSemana() );

      if ( itBlocoCurricularAulasDia == itBlocoCurricularAulas->second.end() )
      {
         std::cout << "Na funcao <SolucaoOperacional::checkConflitoBlocoCurricular>"
            << "algum dia nao foi encontrado." << std::endl;

         exit(1);
      }

      /* Para todas as aulas de um dado dia do bloco curricular em questão, salvo a aula corrente,
      verificar se há conflito de horário. */
      ITERA_GGROUP_LESSPTR( itAula, itBlocoCurricularAulasDia->second, Aula )
      {
         /* Somente se não for igual a Aula em questão E se a aula selecionada para análise 
         já estiver alocada a algum horário. A segunda condição é necessária para a criação 
         de uma solução inicial. */
         if ( **itAula != aula )
         {
            // Buscando, na solução, uma referência para a aula em questão.
            std::map< Aula *, std::pair< Professor *, std::vector< HorarioAula * > >, LessPtr<Aula> >::iterator 
               itAula = solOp.blocoAulas.find(itAula->first);

            if(!itAula->second.second.empty())
            {
               std::vector<HorarioAula*>::iterator
                  itNovosHorariosAula = novosHorariosAula.begin();

               /* Para cada novo horário da aula em questão, checo se ele conflita com  algum horário da
               aula selecionada do bloco curricular. */
               for (; itNovosHorariosAula != novosHorariosAula.end(); ++itNovosHorariosAula)
               {
                  std::vector<HorarioAula*>::iterator 
                     itHorarioAula = itAula->second.second.begin();               

                  // Para cada horário alocado da aula selecionada de algum bloco curricular.
                  for(; (itHorarioAula != itAula->second.second.end()); ++itHorarioAula)
                  {
                     // Se conflitar o horario, o movimento é inviável.
                     if( **itNovosHorariosAula == **itHorarioAula )
                     {
                        return true;
                     }
                  }
               }
            }
         }
      }
   }

   return false;
}

//TRIEDA-889
bool MoveValidator::checkClassAndLessonDisponibility( Aula & aula, std::vector<HorarioAula*> & novosHorariosAula, SolucaoOperacional & solOp ) const
{
   /*
   Nesse método, são verificadas duas condições:

   1 - Se a disciplina possui os novos horários em sua lista de horários letivos.
   2 - Se a sala de aula possui os novos horários em sua lista de horários letivos.

   Qdo as duas condições acimas são satisfeitas, o método conclui que há disponibilidade dos novos horários
   entre a sala e a aula.

   Mas ATENÇÃO, esse método não se preocupa se os horários estão ocupados, ou não. Apenas verifica se eles existem
   para a disciplina e sala em um determinado dia letivo.

   Uma estrutura auxiliar foi construída no ProblemData. Dado um par <Disciplina,Sala> tal estrutura retorna um
   map de HorarioAula, separados por dia, comuns entre a disciplina e a sala. Assim, somente serão considerados 
   horários comuns entre a disc e a sala. Isso evita a checagem separada da disponibilidade da sala e da disciplina.
   */

   /*Obtendo as listas de horários para cada dia letivo comum entre uma sala e uma disciplina (aula).*/
   std::map< std::pair< int /*idDisc*/, int /*idSala*/ >, 
      std::map< int /*Dias*/, GGroup< HorarioAula *, LessPtr< HorarioAula > > > >::iterator

      it_Disc_Salas_Dias_Horarios_Aula = problem_data->disc_Salas_Dias_HorariosAula.find(
      std::make_pair( aula.getDisciplina()->getId(), aula.getSala()->getId() ) );

   // Se encontrar uma relação entre a disciplina e a sala.
   if ( it_Disc_Salas_Dias_Horarios_Aula != problem_data->disc_Salas_Dias_HorariosAula.end() )
   {
      int dia = aula.getDiaSemana();

      std::map< int /*Dias*/, GGroup< HorarioAula *, LessPtr< HorarioAula > > >::iterator
         it_Disc_Salas_DIA_Horarios_Aula = it_Disc_Salas_Dias_Horarios_Aula->second.find(dia);

      // Se encontrar o dia letivo em questão.
      if( it_Disc_Salas_DIA_Horarios_Aula != it_Disc_Salas_Dias_Horarios_Aula->second.end())
      {
         std::vector<HorarioAula*>::iterator
            itNovosHorariosAula = novosHorariosAula.begin();

         // Para cada novo horário.
         for(; ( itNovosHorariosAula != novosHorariosAula.end() ); ++itNovosHorariosAula )
         {
            if( it_Disc_Salas_DIA_Horarios_Aula->second.find(*itNovosHorariosAula) == 
               it_Disc_Salas_DIA_Horarios_Aula->second.end() )
            {
               /* Nao encontrou algum HorarioAula candidato a ser um novo HorarioAula para a disciplina da 
               aula em questão. */
               return false;
            }
         }
      }
      else
      {
         /* Não existe relação entre a disciplina dessa aula e a sala à qual a aula está sendo alocada 
         para o dia em questão. */
         return false;
      }
   }
   else
   {
      // Não existe relação entre a disciplina dessa aula e a sala à qual a aula está sendo alocada
      return false;
   }

   return true;
}

//bool MoveValidator::canShiftSchedule(Aula & aula, std::vector< std::pair< Professor*, Horario* > > blocoHorariosVagos, SolucaoOperacional & solucaoOperacional) const
//bool MoveValidator::canShiftSchedule(Aula & aula, std::vector< std::pair< Professor*, Horario* > > blocoHorariosVagos) const
//{
//   SolucaoOperacional * solOp;
//
//  // Verificando se os possíveis novos horarios estão desocupados (NULL) na solução.
//
//   Professor & professor = *blocoHorariosVagos.begin()->first;
//
//   GGroup<int> horarioAulaId;
//
//   for(unsigned h = 0; h < blocoHorariosVagos.size(); ++h)
//      horarioAulaId.add(blocoHorariosVagos.at(h).second->getHorarioAulaId());
//
//   int diaSemana = aula.getDiaSemana();
//
//   /*
//   Receber como parametro dessa funcao a solucao operacional em questao.
//   Dai, com o dia, os ids dos horarios aula e do professor em maos (ja listei todos acima),
//   basta verificar se estao todos NULL.
//   */
//
//   return ((!checkBlockConflict(aula,blocoHorariosVagos)) && checkClassDisponibility(aula,blocoHorariosVagos));
//}
