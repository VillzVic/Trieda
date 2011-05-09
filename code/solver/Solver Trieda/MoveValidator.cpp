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

bool MoveValidator::canSwapSchedule( Aula & aX, Aula & aY ) const
{
   std::vector< std::pair< Professor *, Horario * > > novosHorariosAX;
   std::vector< std::pair< Professor *, Horario * > > novosHorariosAY;

   novosHorariosAX = aY.bloco_aula;
   novosHorariosAY = aX.bloco_aula;

   return ( !checkBlockConflict( aX, novosHorariosAX )
      && !checkBlockConflict( aY, novosHorariosAY )
      && checkClassDisponibility( aX, novosHorariosAX )
      && checkClassDisponibility( aY, novosHorariosAY ) );
}

bool MoveValidator::checkBlockConflict(
                                       Aula & aula, std::vector< std::pair< Professor *, Horario * > > & novosHorariosAula ) const
{
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
      std::map< BlocoCurricular *, std::map< int /*dia*/, GGroup< Aula *, LessPtr< Aula > > > >::iterator
         itBlocoCurricularAulas = problem_data->blocoCurricularDiaAulas.find( *itBlocoCurric );

      if ( itBlocoCurricularAulas == problem_data->blocoCurricularDiaAulas.end() )
      {
         std::cout << "Na funcao <SolucaoOperacional::checkConflitoBlocoCurricular>"
            << "algum bloco nao foi encontrado." << std::endl;

         exit(1);
      }

      std::map< int /*dia*/, GGroup< Aula *, LessPtr< Aula > > >::iterator 
         itBlocoCurricularDiaAulas = itBlocoCurricularAulas->second.find( aula.getDiaSemana() );
      if ( itBlocoCurricularDiaAulas == itBlocoCurricularAulas->second.end() )
      {
         std::cout << "Na funcao <SolucaoOperacional::checkConflitoBlocoCurricular>"
            << "algum dia nao foi encontrado." << std::endl;

         exit(1);
      }

      // Para todas as aulas de um dado dia do bloco curricular
      // em quest�o, salvo a aula corrente, verificar se h� conflito de hor�rio.
      ITERA_GGROUP_LESSPTR( itAulasBloco, itBlocoCurricularDiaAulas->second, Aula )
      {
         // Somente se n�o for igual a Aula em quest�o E se
         // a aula selecionada para an�lise j� estiver alocada
         // a algum hor�rio. A segunda condi��o � necess�ria
         // para a cria��o de uma solu��o inicial.
         if ( **itAulasBloco != aula && !( itAulasBloco->bloco_aula.empty() ) )
         {
            std::vector< std::pair< Professor *, Horario * > >::iterator 
               itNovosHorariosAula = novosHorariosAula.begin();

            // Para cada novo hor�rio da aula em quest�o,
            // checo se ele conflita com  algum hor�rio da
            // aula selecionada do bloco curricular.
            for (; itNovosHorariosAula != novosHorariosAula.end();
               ++itNovosHorariosAula )
            {
               std::vector< std::pair< Professor *, Horario * > >::iterator 
                  itHorariosAula = itAulasBloco->bloco_aula.begin();

               // Para cada hor�rio da da aula selecionada do bloco curricular
               for(; itHorariosAula != itAulasBloco->bloco_aula.end();
                  ++itHorariosAula )
               {
                  // Se conflitar, o movimento � invi�vel.
                  if(   ( *(itNovosHorariosAula->first) == *(itHorariosAula->first ) )
                     && ( *(itNovosHorariosAula->second) == *(itHorariosAula->second) ) )
                  {
                     return true;
                  }
               }
            }
         }
      }
   }

   return false;
}

bool MoveValidator::checkClassDisponibility(
   Aula & aula, std::vector< std::pair< Professor *, Horario * > > & novosHorariosAula ) const
{
   /*
   Verificando se os poss�veis novos hor�rios da aula s�o compat�veis com a sala a qual a aula est� alocada.
   Verifica-se apenas se a sala possui os horarios demandados. Se eles estao, ou nao, ocupados nao vem ao caso.
   */

   // AQUI, SO EH VERIFICADO SE A SALA POSSUI OS HORARIOS DESEJADOS. SE ELES
   // ESTAO LIVRES OU NAO, NAO INTERESSA AQUI.

   // Obtendo as listas de hor�rios para cada dia
   // letivo comum entre uma sala e uma disciplina (aula).
   std::map< std::pair< int /*idDisc*/, int /*idSala*/ >, 
      std::map< int /*Dias*/, GGroup< HorarioAula *, LessPtr< HorarioAula > > > >::iterator
      it_Disc_Salas_Dias_Horarios_Aula = problem_data->disc_Salas_Dias_HorariosAula.find(
      std::make_pair( aula.getDisciplina()->getId(), aula.getSala()->getId() ) );

   if ( it_Disc_Salas_Dias_Horarios_Aula != problem_data->disc_Salas_Dias_HorariosAula.end() )
   {
      std::vector< std::pair< Professor *, Horario * > >::iterator 
         itNovosHorariosAula = novosHorariosAula.begin();

      // Para cada hor�rio
      for(; ( itNovosHorariosAula != novosHorariosAula.end() );
         ++itNovosHorariosAula )
      {
         std::map< int /*Dias*/, GGroup< HorarioAula *, LessPtr< HorarioAula > > >::iterator
            it_Dias_Horarios_Aula = it_Disc_Salas_Dias_Horarios_Aula->second.find( aula.getDiaSemana() );

         // Se encontrei o dia procurado para a aula
         if ( it_Dias_Horarios_Aula != it_Disc_Salas_Dias_Horarios_Aula->second.end() )
         {
            // Verificando os hor�rios.
            if ( it_Dias_Horarios_Aula->second.find( itNovosHorariosAula->second->horario_aula )
               == it_Dias_Horarios_Aula->second.end() )
            {
			   // Essa sala n�o est� dispon�vel nesse
			   // hor�rio de aula para esse dia da semana
               return false;
            }
         }
         else
         {
			// Essa sala n�o est� dispon�vel em nenhum
			// hor�rio de aula para esse dia da semana
            return false;
         }
      }
   }
   else
   {
	  // N�o existe rela��o entre a disciplina dessa
	  // aula e a sala � qual a aula est� sendo alocada
      return false;
   }

   return true;
}

bool MoveValidator::canShiftSchedule(Aula & aula, std::vector< std::pair< Professor*, Horario* > > blocoHorariosVagos) const
{
  // Verificar aqui tb se alem da sala possuir os horarios, se na solucao em questao os possiveis novos horarios estao desocupados.

   Professor & professor = *blocoHorariosVagos.begin()->first;

   GGroup<int> horarioAulaId;

   for(unsigned h = 0; h < blocoHorariosVagos.size(); ++h)
      horarioAulaId.add(blocoHorariosVagos.at(h).second->getHorarioAulaId());

   int diaSemana = aula.getDiaSemana();

   /*
   Receber como parametro dessa funcao a solucao operacional em questao.
   Dai, com o dia, os ids dos horarios aula e do professor em maos (ja listei todos acima),
   basta verificar se estao todos NULL.
   */

   return ((!checkBlockConflict(aula,blocoHorariosVagos)) && checkClassDisponibility(aula,blocoHorariosVagos));
}