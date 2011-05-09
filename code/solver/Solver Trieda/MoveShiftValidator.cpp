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
   Checando se a qtd de hor�rios a serem alocados s�o iguais (em quantidade) aos hor�rios da aula em quest�o.
   */

   if(aula.bloco_aula.size() != blocoHorariosVagos.size())
      return false;

   //std::cout << "\tCheck SIZE - OK\n";

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

   //std::cout << "\tCheck VIRTUAL - OK\n";

   // -------------------------------------------------------------------------------------------
   /* Checando se o professor e os hor�rios correntes s�o os mesmos que o novo professor e hor�rios */

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
   // Verificando se os poss�veis novos horarios est�o desocupados (NULL) na solu��o.
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
   Se chegou at� aqui, � pq todos os hor�rios est�o livres.
   */

   return ((!checkBlockConflict(aula,blocoHorariosVagos)) && checkClassDisponibility(aula,blocoHorariosVagos));
}

bool MoveShiftValidator::isValid(Aula & aula, std::vector< std::pair< Professor*, Horario* > > blocoHorariosVagos)
{
	//std::cout << "\n\nWarnning: IMPLEMENTAR O METODO <bool MoveShiftValidator::isValid(Aula & aula, std::vector< std::pair< Professor*, Horario* > > blocoHorariosVagos)>. Retornando sempre false, por padrao.\n\n";
   return ((canShiftSchedule(aula,blocoHorariosVagos)) && !(aula.getDisciplina()->eLab()));
}