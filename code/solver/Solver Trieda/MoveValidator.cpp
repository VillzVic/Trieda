#include "MoveValidator.h"

MoveValidator::MoveValidator(ProblemData * pD) : problem_data(pD)
{
}

MoveValidator::~MoveValidator()
{
}

bool MoveValidator::canSwapSchedule(Aula & aX, Aula & aY) const
{
   std::vector< std::pair< Professor *, Horario * > > novosHorariosAX;
   std::vector< std::pair< Professor *, Horario * > > novosHorariosAY;

   novosHorariosAX = aY.bloco_aula;
   novosHorariosAY = aX.bloco_aula;

   return ( !checkBlockConflict(aX, novosHorariosAX)
      && !checkBlockConflict(aY, novosHorariosAY)
      && checkClassDisponibility(aX, novosHorariosAX)
      && checkClassDisponibility(aY, novosHorariosAY) );
}

bool MoveValidator::checkBlockConflict( Aula & aula, std::vector< std::pair< Professor *, Horario * > > & novosHorariosAula ) const
{
   std::map<Aula *, GGroup<BlocoCurricular*,LessPtr<BlocoCurricular> > >::iterator
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
                  ++itHorariosAula)
               {
                  // Se conflitar, o movimento � invi�vel.
                  if(    (*(itNovosHorariosAula->first) == *(itHorariosAula->first))
                     && (*(itNovosHorariosAula->second) == *(itHorariosAula->second)) )
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
   Aula & aula, 
   std::vector< std::pair< Professor *, Horario * > > & novosHorariosAula) const
{
   /* Verificando se os poss�veis novos hor�rios da aula s�o compat�veis com a sala a qual
   a aula est� alocada. */

   // Obtendo as listas de hor�rios para cada dia letivo comum entre uma sala e uma disciplina (aula).
   std::map<std::pair< int /*idDisc*/, int /*idSala*/ >, 
      std::map< int /*Dias*/, GGroup<Horario*, LessPtr<Horario> > > >::iterator
      it_Disc_Salas_Dias_Horarios_Aula = problem_data->disc_Salas_Dias_Horarios.find(
      std::make_pair(aula.getDisciplina()->getId(),aula.getSala()->getId()));

   if(it_Disc_Salas_Dias_Horarios_Aula != problem_data->disc_Salas_Dias_Horarios.end())
   {
      std::vector< std::pair< Professor *, Horario * > >::iterator 
         itNovosHorariosAula = novosHorariosAula.begin();

      // Para cada hor�rio
      for(; (itNovosHorariosAula != novosHorariosAula.end()); ++itNovosHorariosAula)
      {

         std::map< int /*Dias*/, GGroup<Horario*, LessPtr<Horario> > >::iterator
            it_Dias_Horarios_Aula = it_Disc_Salas_Dias_Horarios_Aula->second.find(aula.getDiaSemana());

         // Se encontrei o dia procurado para as duas aulas
         if(it_Dias_Horarios_Aula != it_Disc_Salas_Dias_Horarios_Aula->second.end())
         {
            // Verificando os hor�rios.
            if(it_Dias_Horarios_Aula->second.find(itNovosHorariosAula->second) == it_Dias_Horarios_Aula->second.end())
            {
               return false;
            }
         }
         else
         {
            std::cout << "ERRO 1 em <>\n";
            exit(1);
         }

         //if(aX.getSala()->horarios_disponiveis.find(itNovosHorariosAX->second) == 
         //   aX.getSala()->horarios_disponiveis.end() 
         //   ||
         //   aY.getSala()->horarios_disponiveis.find(itNovosHorariosAY->second) == 
         //   aY.getSala()->horarios_disponiveis.end() )
         //{
         //   return false;
         //}

      }
   }
   else
   {
      std::cout << "ERRO 2 em <>\n";
      exit(1);
   }

   return true;
}