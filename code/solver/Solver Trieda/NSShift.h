#ifndef _NS_SHIFT_H_
#define _NS_SHIFT_H_

#include "NS.hpp"

#include "MoveShift.h"

#include "MoveShiftValidator.h"

#include "ProblemData.h"

class NSShift : public NS
{
private:
   ProblemData & problemData;

   //std::pair<Aula*, std::vector< std::pair< Professor*, Horario* > > > pickSomeClassAndNewSchedule( const SolucaoOperacional & s );
   bool pickSomeClassAndNewSchedule( const SolucaoOperacional & s ); // So para poder compilar !!!!

   MoveShiftValidator * moveValidator;

public:

   NSShift(ProblemData & problemData);

   virtual ~NSShift();

   Move & move(const SolucaoOperacional & s);

   void print();
};

#endif /*_NS_SHIFT_H_*/