#ifndef _NS_SHIFT_H_
#define _NS_SHIFT_H_

#include "NS.hpp"

#include "MoveShift.h"

#include "MoveShiftValidator.h"

#include "ProblemData.h"

class NSShift : public NS
{
public:
   
   NSShift(ProblemData & problemData, bool keepFeasibility = false);

   virtual ~NSShift();

   Move & move(SolucaoOperacional & solOp);

   void print();

private:

   bool keepFeasibility;

   ProblemData & problemData;
   
   std::pair<Aula*, std::pair<Professor*,std::vector<HorarioAula*> > > pickSomeClassAndNewSchedule(SolucaoOperacional & solOp);

   std::pair<Aula*, std::pair<Professor*,std::vector<HorarioAula*> > > pickSomeClassAndNewScheduleWithouChecks(SolucaoOperacional & solOp);

   MoveShiftValidator * moveValidator;
};

#endif /*_NS_SHIFT_H_*/