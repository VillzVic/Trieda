#include "MoveShiftValidator.h"

MoveShiftValidator::MoveShiftValidator(ProblemData * pD) : MoveValidator(pD)
{
}

MoveShiftValidator::~MoveShiftValidator()
{
}

bool MoveShiftValidator::canShiftSchedule(Aula & aula, Professor & professor, std::vector<HorarioAula*> novosHorariosAula, SolucaoOperacional & solOp) const
{
   // -------------------------------------------------------------------------------------------
   /*
   Checando se a qtd de hor�rios a serem alocados s�o iguais (em quantidade) aos hor�rios da aula em quest�o.
   */

   /* Obtendo refer�ncia para o(s) bloco(s) de aulas aos quais pertece a aula selecionada acima. */
   std::map< Aula *, std::pair< Professor *, std::vector< HorarioAula * > >, LessPtr<Aula> >::const_iterator
      itBlocoAulas = solOp.blocoAulas.find(&aula);

   if(itBlocoAulas == solOp.blocoAulas.end())
   {
      std::cout << "Nao encontrei a aula desejada na estrutura de blocoAula de uma dada solucao. ERRO.\n";
      exit(1);
   }

   if(itBlocoAulas->second.second.size() != novosHorariosAula.size())
      return false;

   // -------------------------------------------------------------------------------------------
   /*
   Checando se os hor�rios pertencem ao mesmo dia.

   N�O TEM COMO FAZER ISSO. NA VERDADE, ESSA VERIFICA��O � DESNECESS�RIA. POIS COMO A NOSSA SOLU��O
   � UMA MATRIZ COMPLETA, ACABA QUE TODOS OS DIAS POSSUEM TODOS OS HOR�RIOS. A DIFEREN� � QUE ALGUNS
   HOR�RIOS PODEM ESTAR ALOCADOS PARA AULAS VIRTUAIS. ASSIM, N�O H� COMO ALOCAR AULA PARA TAIS HOR�RIOS.
   PORTANTO, AO ESCOLHER UM HOR�RIO, O DIA N�O IMPORTA. SEMPRE EXISTIR� O HOR�RIO DESSEJADO PARA O DIA, COM
   A POSSIBILIDADE DE ESTAR VAGO (NULL), ALOCADO (PARA ALGUMA AULA) OU INDISPON�VEL (AULA VIRTUAL).
   */

   // -------------------------------------------------------------------------------------------
   /* Checando se a aula � real */

   if(aula.eVirtual())
      return false;

   // -------------------------------------------------------------------------------------------
   // Checando se o professor corrente � o mesmo que o novo professor.
   if(professor.getId() == itBlocoAulas->second.first->getId())
   {
      /* Se, pelo menos, um dos novos hor�rios for comum aos hor�rios antigos da aula em quest�o deve-se
      retornar false. Ou seja, o shift n�o ser� realizado. */

      std::vector<HorarioAula*>::iterator
         itNovosHorariosAula = novosHorariosAula.begin();

      /* Para cada novo hor�rio da aula em quest�o, checo se ele conflita com  algum hor�rio da
      aula selecionada do bloco curricular. */
      for (; itNovosHorariosAula != novosHorariosAula.end(); ++itNovosHorariosAula)
      {
         std::vector<HorarioAula*>::const_iterator 
            itHorarioAula = itBlocoAulas->second.second.begin();               

         // Para cada hor�rio alocado da aula selecionada de algum bloco curricular.
         for(; (itHorarioAula != itBlocoAulas->second.second.end()); ++itHorarioAula)
         {
            // Se conflitar o horario, o movimento � invi�vel.
            if( **itNovosHorariosAula == **itHorarioAula )
            {
               return false;
            }
         }
      }
   }

   // -------------------------------------------------------------------------------------------
   // Verificando se os poss�veis novos horarios est�o desocupados (NULL) na solu��o.

   for(unsigned h = 0; h < novosHorariosAula.size(); ++h)
   {
      std::vector< HorarioAula * >::iterator itHorariosAulaOrd = problem_data->horarios_aula_ordenados.begin();

      int indice = 0;
      for(; itHorariosAulaOrd != problem_data->horarios_aula_ordenados.end(); ++itHorariosAulaOrd, ++indice)
      {
         if( (*itHorariosAulaOrd)->getId() == novosHorariosAula.at(h)->getId())
            break;
      }

      if(itHorariosAulaOrd == problem_data->horarios_aula_ordenados.end())
      {
         std::cout << "ERRO em <bool MoveShiftValidator::canShiftSchedule(Aula & aula, std::vector< std::pair< Professor*, Horario* > > blocoHorariosVagos) const>. "
            << "Indice de horario nao encontrado." << std::endl;
         exit(1);
      }

      int diaSemana = aula.getDiaSemana();

      int colHorarioDia = (diaSemana-1)*solOp.getTotalHorarios() + indice;

      if(solOp.getMatrizAulas()->at(professor.getIdOperacional())->at(colHorarioDia) != NULL)
      {
         return false;
      }
   }

   // -------------------------------------------------------------------------------------------

   //std::cout << "\tCheck HORARIOS VAGOS SOLUCAO - OK\n";

   // Se chegou at� aqui, � pq todos os hor�rios est�o livres.

   //return ((!checkBlockConflict(aula,novosHorariosAula,solOp)) && checkClassAndLessonDisponibility(aula,novosHorariosAula,solOp));
   return (checkClassAndLessonDisponibility(aula,novosHorariosAula,solOp));
}

bool MoveShiftValidator::isValid(Aula & aula, Professor & prof, std::vector<HorarioAula*> novosHorariosAula, SolucaoOperacional & solOp)
{
   return ((canShiftSchedule(aula,prof,novosHorariosAula,solOp)) && !(aula.getDisciplina()->eLab()));
}