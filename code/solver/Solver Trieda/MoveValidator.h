#ifndef _MOVE_VALIDATOR_HPP_
#define _MOVE_VALIDATOR_HPP_

#include "input.h"

#include "ProblemData.h"

#include "Aula.h"

// -----------------------------
// Para evitar uma dependência circular entre SolucaoOperacional e ProblemData
//#include "SolucaoOperacional.h"
//class SolucaoOperacional;
// -----------------------------

class MoveValidator
{
public:

   MoveValidator(ProblemData * pD);

   //MoveValidator(SolucaoOperacional * solOp) {};

   virtual ~MoveValidator();

   //virtual bool isValid(Aula & aX, Aula & aY);

//protected:

   // ToDo : Passar o metodo abaixo para a classe MoveSwapValidator(Cria-la antes. Ela servirá apenas para armazenar esse método.).
   bool canSwapSchedule(Aula & aX, Aula & aY) const;

   bool checkBlockConflict( Aula & a, std::vector< std::pair< Professor *, Horario * > > & s ) const;

   bool checkClassDisponibility(
      Aula & a, 
      std::vector< std::pair< Professor *, Horario * > > & s) const;

protected:
   
   //bool canShiftSchedule(Aula & aula, std::vector< std::pair< Professor*, Horario* > > blocoHorariosVagos) const;

//private:

   ProblemData * problem_data;
};

#endif /*_MOVE_VALIDATOR_HPP_*/