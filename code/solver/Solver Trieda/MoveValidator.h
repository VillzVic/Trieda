#ifndef _MOVEVALIDATOR_HPP_
#define _MOVEVALIDATOR_HPP_

#include "input.h"

#include "ProblemData.h"

#include "Aula.h"

class MoveValidator
{
public:

   MoveValidator(ProblemData * pD);

   virtual ~MoveValidator();

   virtual bool isValid(Aula & aX, Aula & aY);

//protected:

   bool canSwapSchedule(Aula & aX, Aula & aY) const;

   bool checkBlockConflict( Aula & a, std::vector< std::pair< Professor *, Horario * > > & s ) const;

   bool checkClassDisponibility(
      Aula & a, 
      std::vector< std::pair< Professor *, Horario * > > & s) const;

private:

   ProblemData * problem_data;
};

#endif // _MOVEVALIDATOR_HPP_