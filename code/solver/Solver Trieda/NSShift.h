#ifndef _NS_SHIFT_H_
#define _NS_SHIFT_H_

#include "NS.hpp"
#include "MoveShift.h"
#include "MoveShiftValidator.h"
#include "ProblemData.h"

// TRIEDA-924
class NSShift
   : public NS
{
public:
   NSShift( ProblemData &, bool = false );
   virtual ~NSShift();

   Move & move( SolucaoOperacional & );
   void print();

private:
   bool keepFeasibility;
   ProblemData & problemData;
   
   std::pair< Aula *, std::pair< Professor * , std::vector< HorarioAula * > > >
      pickSomeClassAndNewSchedule( SolucaoOperacional & );

   std::pair< Aula *, std::pair< Professor * , std::vector< HorarioAula * > > >
      pickSomeClassAndNewScheduleWithouChecks( SolucaoOperacional & );

   MoveShiftValidator * moveValidator;
};

#endif