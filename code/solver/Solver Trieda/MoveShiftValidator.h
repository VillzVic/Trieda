#ifndef _MOVE_SHIFT_VALIDATOR_HPP_
#define _MOVE_SHIFT_VALIDATOR_HPP_

#include "MoveValidator.h"

typedef std::map< Aula *, std::pair< Professor *, std::vector< HorarioAula * > >, LessPtr<Aula> >::const_iterator cItClassesBlocks;

/*
TRIEDA-955 e TRIEDA-957

Foi realizada uma estruturação da classe abaixo de modo que as fixações pudessem ser tratadas.
Falta adicionar o check de fixação para atender as issues acima e, posteriormente, mover alguns métodos
dessa classe para a sua super classe (MoveValidator). Assim, bastará apenas adicionar o check de fixação
nas estruturas de swap.
*/
class MoveShiftValidator : public MoveValidator
{
public:
   
   MoveShiftValidator(ProblemData * pD);

   virtual ~MoveShiftValidator();

   bool canShiftSchedule(Aula & aula, Professor & prof, std::vector<HorarioAula*> novosHorariosAula, SolucaoOperacional & solOp, std::vector<std::string> checksToDo);

protected:

   cItClassesBlocks getClassesBlocks(Aula & aula, SolucaoOperacional & solOp);

   bool totalSchedules(cItClassesBlocks & itBlocks, std::vector<HorarioAula*> & newSchedule);

   bool sameTeachers(Professor & teacher, cItClassesBlocks & itBlocks);

   bool conflictScheduleTeachers(Professor & teacher, cItClassesBlocks & itBlocks, std::vector<HorarioAula*> & newSchedule);

   bool newScheduleFree(Aula & aula, Professor & teacher, std::vector<HorarioAula*> & newSchedule, SolucaoOperacional & solOp);

};

#endif /*_MOVE_SHIFT_VALIDATOR_HPP_*/