#ifndef _MOVE_VALIDATOR_HPP_
#define _MOVE_VALIDATOR_HPP_

#include "input.h"

#include "ProblemData.h"

#include "Aula.h"

// -----------------------------
// Para evitar uma dependência circular entre SolucaoOperacional e ProblemData
#include "SolucaoOperacional.h"
//class SolucaoOperacional;
// -----------------------------

class MoveValidator
{
public:

   MoveValidator(ProblemData * pD);

   virtual ~MoveValidator();

   //virtual bool isValid(Aula & aX, Aula & aY);

//protected:

   // ToDo : Passar o metodo abaixo para a classe MoveSwapValidator(Cria-la antes. Ela servirá apenas para armazenar esse método.).
   // PASSAR TB A SOLUCAO PARA PODER EXTRAIR OS HORARIOS DE CADA AULA.
   bool canSwapSchedule(Aula & aX, Aula & aY, SolucaoOperacional & solOp) const;

   bool checkBlockConflict( 
      Aula & a,
      std::vector<HorarioAula*> & novosHorariosAula,
      SolucaoOperacional & solOp ) const;

   bool checkClassAndLessonDisponibility(
      Aula & a,
      std::vector<HorarioAula*> & novosHorariosAula,
      SolucaoOperacional & solOp ) const;

protected:
   
   ProblemData * problem_data;
};

#endif /*_MOVE_VALIDATOR_HPP_*/