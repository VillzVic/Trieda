#include "MoveShiftValidator.h"

MoveShiftValidator::MoveShiftValidator(ProblemData * pD) : MoveValidator(pD)
{
}

MoveShiftValidator::~MoveShiftValidator()
{
}

bool MoveShiftValidator::canShiftSchedule(Aula & aula, Professor & professor, std::vector<HorarioAula*> novosHorariosAula, SolucaoOperacional & solOp) const
{
   return false;
}

bool MoveShiftValidator::isValid(Aula & aula, Professor & prof, std::vector<HorarioAula*> novosHorariosAula, SolucaoOperacional & solOp)
{
   return ((canShiftSchedule(aula,prof,novosHorariosAula,solOp)) && !(aula.getDisciplina()->eLab()));
}

cItClassesBlocks MoveShiftValidator::getClassesBlocks(Aula & aula, SolucaoOperacional & solOp)
{
   /* Obtendo referência para o(s) bloco(s) de aulas aos quais pertece a aula selecionada acima. */
   std::map< Aula *, std::pair< Professor *, std::vector< HorarioAula * > >, LessPtr<Aula> >::const_iterator
      itBlocoAulas = solOp.blocoAulas.find(&aula);

   if(itBlocoAulas == solOp.blocoAulas.end())
   {
      std::cout << "Nao encontrei a aula desejada na estrutura de blocoAula de uma dada solucao. ERRO.\n";
      exit(1);
   }

   return itBlocoAulas;
}

bool MoveShiftValidator::totalSchedules(cItClassesBlocks & itBlocks, std::vector<HorarioAula*> & newSchedule)
{
   /* Checando se a qtd de horários a serem alocados são iguais (em quantidade) aos horários da aula em questão. */
   return (itBlocks->second.second.size() != newSchedule.size());
}

bool MoveShiftValidator::sameTeachers(Professor & teacher, cItClassesBlocks & itBlocks)
{
   return (teacher.getId() == itBlocks->second.first->getId());
}

bool MoveShiftValidator::conflictScheduleTeachers(Professor & teacher, cItClassesBlocks & itBlocks, std::vector<HorarioAula*> & newSchedule)
{
   // Checando se o professor corrente é o mesmo que o novo professor.
   if(sameTeachers(teacher,itBlocks))
   {
      /* Se, pelo menos, um dos novos horários for comum aos horários antigos da aula em questão deve-se
      retornar false. Ou seja, o shift não será realizado. */

      std::vector<HorarioAula*>::iterator
         itNovosHorariosAula = newSchedule.begin();

      /* Para cada novo horário da aula em questão, checo se ele conflita com  algum horário da
      aula selecionada do bloco curricular. */
      for (; itNovosHorariosAula != newSchedule.end(); ++itNovosHorariosAula)
      {
         std::vector<HorarioAula*>::const_iterator 
            itHorarioAula = itBlocks->second.second.begin();               

         // Para cada horário alocado da aula selecionada de algum bloco curricular.
         for(; (itHorarioAula != itBlocks->second.second.end()); ++itHorarioAula)
         {
            // Se conflitar o horario, o movimento é inviável.
            if( **itNovosHorariosAula == **itHorarioAula )
            { return true; }
         }
      }
   }

   return false;
}

bool MoveShiftValidator::newScheduleFree(Aula & aula, Professor & teacher, std::vector<HorarioAula*> & newSchedule, SolucaoOperacional & solOp)
{
   // Verificando se os possíveis novos horarios estão desocupados (NULL) na solução.
   for(unsigned h = 0; h < newSchedule.size(); ++h)
   {
      std::vector< HorarioAula * >::iterator itHorariosAulaOrd = problem_data->horarios_aula_ordenados.begin();

      int indice = 0;
      for(; itHorariosAulaOrd != problem_data->horarios_aula_ordenados.end(); ++itHorariosAulaOrd, ++indice)
      {
         if( (*itHorariosAulaOrd)->getId() == newSchedule.at(h)->getId()) { break; }
      }

      if(itHorariosAulaOrd == problem_data->horarios_aula_ordenados.end())
      {
         std::cout << "ERRO em <bool MoveShiftValidator::canShiftSchedule(Aula & aula, std::vector< std::pair< Professor*, Horario* > > blocoHorariosVagos) const>. "
            << "Indice de horario nao encontrado." << std::endl;
         exit(1);
      }

      int diaSemana = aula.getDiaSemana();

      int colHorarioDia = (diaSemana-1)*solOp.getTotalHorarios() + indice;

      if(solOp.getMatrizAulas()->at(teacher.getIdOperacional())->at(colHorarioDia) != NULL)
      { return false; }
   }

   return true;

}

bool MoveShiftValidator::isValidV2(Aula & aula, Professor & teacher, std::vector<HorarioAula*> newSchedule, SolucaoOperacional & solOp, std::vector<std::string> checksToDo)
{
   cItClassesBlocks itBlocks = getClassesBlocks(aula,solOp);

   while(!checksToDo.empty())
   {    
      std::string cmd = (*(checksToDo.begin()));

      if(cmd == "totalSchedules")
      { if(!totalSchedules(itBlocks,newSchedule)) { return false; } }
      else if(cmd == "virtualClass")
      { if(aula.eVirtual()) { return false; } }
      else if(cmd == "avoidSameTeachers")
      { if(sameTeachers(teacher,itBlocks)) { return false; } }
      else if(cmd == "conflictScheduleTeachers")
      { if(conflictScheduleTeachers(teacher,itBlocks,newSchedule)) { return false; } }
      else if(cmd == "newScheduleFree")
      { if(!newScheduleFree(aula,teacher,newSchedule,solOp)) { return false; } }
      else if(cmd == "checkBlockConflict")
      { if(checkBlockConflict(aula,newSchedule,solOp)) { return false; } }
      else if(cmd == "checkClassAndLessonDisponibility")
      { if(!checkClassAndLessonDisponibility(aula,newSchedule,solOp)) { return false; } }
      // add <else if> to others checks.
      else 
      {
         std::cout << "CMD NAO ENCONTRADO EM MOVESHIFTVALIDATOR" << std::endl;
         exit(1);
      }

      checksToDo.erase(checksToDo.begin());
   }

   return true;
}