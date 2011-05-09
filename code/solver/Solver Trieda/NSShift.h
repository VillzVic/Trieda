#ifndef _NS_SHIFT_H_
#define _NS_SHIFT_H_

#include "NS.hpp"

#include "MoveShift.h"

#include "MoveShiftValidator.h"

#include "ProblemData.h"

class NSShift : public NS
{
public:
   //NSShift(ProblemData & problemData);
   NSShift(ProblemData & problemData, SolucaoOperacional & solOp);

   virtual ~NSShift();

   Move & move(const SolucaoOperacional & s);

   void print();

private:
   ProblemData & problemData;
   
   SolucaoOperacional & solOp;

   //std::pair<Aula*, std::vector< std::pair< Professor*, Horario* > > > pickSomeClassAndNewSchedule( const SolucaoOperacional & s );
   //bool pickSomeClassAndNewSchedule( const SolucaoOperacional & s ); // So para poder compilar !!!!
   std::pair<Aula*, std::vector< std::pair< Professor*, Horario* > > > pickSomeClassAndNewSchedule();

   MoveShiftValidator * moveValidator;
};

#endif /*_NS_SHIFT_H_*/