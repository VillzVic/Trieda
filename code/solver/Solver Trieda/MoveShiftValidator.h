#ifndef _MOVE_SHIFT_VALIDATOR_HPP_
#define _MOVE_SHIFT_VALIDATOR_HPP_

#include "MoveValidator.h"

typedef std::map< Aula *, std::pair< Professor *, std::vector< HorarioAula * > >, LessPtr<Aula> >::const_iterator cItClassesBlocks;

class MoveShiftValidator : public MoveValidator
{
public:
   
   MoveShiftValidator(ProblemData * pD);

   virtual ~MoveShiftValidator();

   bool canShiftSchedule(Aula & aula, Professor & prof, std::vector<HorarioAula*> novosHorariosAula, SolucaoOperacional & solOp) const;

   bool isValid(Aula & aula, Professor & prof, std::vector<HorarioAula*> novosHorariosAula, SolucaoOperacional & solOp);

   cItClassesBlocks getClassesBlocks(Aula & aula, SolucaoOperacional & solOp);

   bool totalSchedules(cItClassesBlocks & itBlocks, std::vector<HorarioAula*> & newSchedule);

   bool sameTeachers(Professor & teacher, cItClassesBlocks & itBlocks);

   bool conflictScheduleTeachers(Professor & teacher, cItClassesBlocks & itBlocks, std::vector<HorarioAula*> & newSchedule);

   bool newScheduleFree(Aula & aula, Professor & teacher, std::vector<HorarioAula*> & newSchedule, SolucaoOperacional & solOp);

   bool isValidV2(Aula & aula, Professor & teacher, std::vector<HorarioAula*> newSchedule, SolucaoOperacional & solOp, std::vector<std::string> checksToDo);

};

#endif /*_MOVE_SHIFT_VALIDATOR_HPP_*/