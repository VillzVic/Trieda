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
   Checando se a qtd de horários a serem alocados são iguais (em quantidade) aos horários da aula em questão.
   */

   /* Obtendo referência para o(s) bloco(s) de aulas aos quais pertece a aula selecionada acima. */
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
   Checando se os horários pertencem ao mesmo dia.

   NÃO TEM COMO FAZER ISSO. NA VERDADE, ESSA VERIFICAÇÃO É DESNECESSÁRIA. POIS COMO A NOSSA SOLUÇÃO
   É UMA MATRIZ COMPLETA, ACABA QUE TODOS OS DIAS POSSUEM TODOS OS HORÁRIOS. A DIFERENÃ É QUE ALGUNS
   HORÁRIOS PODEM ESTAR ALOCADOS PARA AULAS VIRTUAIS. ASSIM, NÃO HÁ COMO ALOCAR AULA PARA TAIS HORÁRIOS.
   PORTANTO, AO ESCOLHER UM HORÁRIO, O DIA NÃO IMPORTA. SEMPRE EXISTIRÁ O HORÁRIO DESSEJADO PARA O DIA, COM
   A POSSIBILIDADE DE ESTAR VAGO (NULL), ALOCADO (PARA ALGUMA AULA) OU INDISPONÍVEL (AULA VIRTUAL).
   */

   // -------------------------------------------------------------------------------------------
   /* Checando se a aula é real */

   if(aula.eVirtual())
      return false;

   // -------------------------------------------------------------------------------------------
   // Checando se o professor corrente é o mesmo que o novo professor.
   if(professor.getId() == itBlocoAulas->second.first->getId())
   {
      /* Se, pelo menos, um dos novos horários for comum aos horários antigos da aula em questão deve-se
      retornar false. Ou seja, o shift não será realizado. */

      std::vector<HorarioAula*>::iterator
         itNovosHorariosAula = novosHorariosAula.begin();

      /* Para cada novo horário da aula em questão, checo se ele conflita com  algum horário da
      aula selecionada do bloco curricular. */
      for (; itNovosHorariosAula != novosHorariosAula.end(); ++itNovosHorariosAula)
      {
         std::vector<HorarioAula*>::const_iterator 
            itHorarioAula = itBlocoAulas->second.second.begin();               

         // Para cada horário alocado da aula selecionada de algum bloco curricular.
         for(; (itHorarioAula != itBlocoAulas->second.second.end()); ++itHorarioAula)
         {
            // Se conflitar o horario, o movimento é inviável.
            if( **itNovosHorariosAula == **itHorarioAula )
            {
               return false;
            }
         }
      }
   }

   // -------------------------------------------------------------------------------------------
   // Verificando se os possíveis novos horarios estão desocupados (NULL) na solução.

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

   // Se chegou até aqui, é pq todos os horários estão livres.

   //return ((!checkBlockConflict(aula,novosHorariosAula,solOp)) && checkClassAndLessonDisponibility(aula,novosHorariosAula,solOp));
   return (checkClassAndLessonDisponibility(aula,novosHorariosAula,solOp));
}

bool MoveShiftValidator::isValid(Aula & aula, Professor & prof, std::vector<HorarioAula*> novosHorariosAula, SolucaoOperacional & solOp)
{
   return ((canShiftSchedule(aula,prof,novosHorariosAula,solOp)) && !(aula.getDisciplina()->eLab()));
}