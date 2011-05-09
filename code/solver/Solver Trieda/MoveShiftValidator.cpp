#include "MoveShiftValidator.h"

//MoveShiftValidator::MoveShiftValidator(ProblemData * pD) : MoveValidator(pD)
//{
//}

MoveShiftValidator::MoveShiftValidator(ProblemData * pD, SolucaoOperacional & _solOp) : MoveValidator(pD), solOp(_solOp)
{
}

MoveShiftValidator::~MoveShiftValidator()
{
}

bool MoveShiftValidator::canShiftSchedule(Aula & aula, std::vector< std::pair< Professor*, Horario* > > blocoHorariosVagos) const
{
   // -------------------------------------------------------------------------------------------
   /*
   Checando se a qtd de horários a serem alocados são iguais (em quantidade) aos horários da aula em questão.
   */

   if(aula.bloco_aula.size() != blocoHorariosVagos.size())
      return false;

   //std::cout << "\tCheck SIZE - OK\n";

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

   //std::cout << "\tCheck VIRTUAL - OK\n";

   // -------------------------------------------------------------------------------------------
   /* Checando se o professor e os horários correntes são os mesmos que o novo professor e horários */

   // Professor candidato
   Professor & professor = *blocoHorariosVagos.begin()->first;

   if(professor.getId()==aula.bloco_aula.begin()->first->getId())
   {
      GGroup<int> idsHorarioAula;
      
      std::vector< std::pair< Professor*, Horario* > >::iterator 
         itBlocoHorariosVagos = blocoHorariosVagos.begin();

      // Para cada horario novo
      for(; itBlocoHorariosVagos != blocoHorariosVagos.end(); ++itBlocoHorariosVagos)
      { idsHorarioAula.add(itBlocoHorariosVagos->second->getHorarioAulaId()); }

      std::vector< std::pair< Professor*, Horario* > >::iterator 
         itBlocoHorariosAntigos = aula.bloco_aula.begin();

      bool equals = true;

      // Para cada horario antigo
      for(; itBlocoHorariosAntigos != aula.bloco_aula.end(); ++itBlocoHorariosAntigos)
      {
         if(idsHorarioAula.find(itBlocoHorariosAntigos->second->getHorarioAulaId()) == 
            idsHorarioAula.end())
         {
            equals = false;
            break;
         }
      }

      if(equals)
         return false;
   }

   // -------------------------------------------------------------------------------------------
   // Verificando se os possíveis novos horarios estão desocupados (NULL) na solução.
   /**/

   //Professor & professor = *blocoHorariosVagos.begin()->first;

   for(unsigned h = 0; h < blocoHorariosVagos.size(); ++h)
   {
      std::vector< HorarioAula * >::iterator itHorariosAulaOrd = problem_data->horarios_aula_ordenados.begin();

      int indice = 0;
      for(; itHorariosAulaOrd != problem_data->horarios_aula_ordenados.end(); ++itHorariosAulaOrd, ++indice)
      {
         if( (*itHorariosAulaOrd)->getId() == blocoHorariosVagos.at(h).second->getHorarioAulaId())
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
         //std::cout << "\tFALSE\n";
         return false;
      }
   }
   /**/

   // -------------------------------------------------------------------------------------------

   //std::cout << "\tCheck HORARIOS VAGOS SOLUCAO - OK\n";

   /*
   Se chegou até aqui, é pq todos os horários estão livres.
   */

   return ((!checkBlockConflict(aula,blocoHorariosVagos)) && checkClassDisponibility(aula,blocoHorariosVagos));
}

bool MoveShiftValidator::isValid(Aula & aula, std::vector< std::pair< Professor*, Horario* > > blocoHorariosVagos)
{
	//std::cout << "\n\nWarnning: IMPLEMENTAR O METODO <bool MoveShiftValidator::isValid(Aula & aula, std::vector< std::pair< Professor*, Horario* > > blocoHorariosVagos)>. Retornando sempre false, por padrao.\n\n";
   return ((canShiftSchedule(aula,blocoHorariosVagos)) && !(aula.getDisciplina()->eLab()));
}