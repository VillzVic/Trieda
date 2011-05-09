#ifndef _MOVE_VALIDATOR_HPP_
#define _MOVE_VALIDATOR_HPP_

#include "input.h"

#include "ProblemData.h"

#include "Aula.h"

class MoveValidator
{
public:

   MoveValidator(ProblemData * pD);

   virtual ~MoveValidator();

   //virtual bool isValid(Aula & aX, Aula & aY);

//protected:

   bool canSwapSchedule(Aula & aX, Aula & aY) const;

   bool checkBlockConflict( Aula & a, std::vector< std::pair< Professor *, Horario * > > & s ) const;

   bool checkClassDisponibility(
      Aula & a, 
      std::vector< std::pair< Professor *, Horario * > > & s) const;

protected:
   
   //bool canShiftSchedule(Aula & aula, Professor & novoProfAula, vector<pair<Professor*,Horario*> > blocoHorariosVagos) const;
   bool canShiftSchedule(Aula & aula, std::vector< std::pair< Professor*, Horario* > > blocoHorariosVagos) const;

private:

   ProblemData * problem_data;
};

#endif /*_MOVE_VALIDATOR_HPP_*/