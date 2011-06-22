#ifndef _MOVE_VALIDATOR_HPP_
#define _MOVE_VALIDATOR_HPP_

#include "ProblemData.h"
#include "Aula.h"
#include "SolucaoOperacional.h"

class MoveValidator
{
public:

   MoveValidator( ProblemData * );
   virtual ~MoveValidator();

   // ToDo : Passar o metodo abaixo para a classe MoveSwapValidator(Cria-la antes. Ela servirá apenas para armazenar esse método.).
   // PASSAR TB A SOLUCAO PARA PODER EXTRAIR OS HORARIOS DE CADA AULA.
   bool canSwapSchedule( Aula &, Aula &, SolucaoOperacional & ) const;

   bool checkBlockConflict( 
      Aula &,
      std::vector< HorarioAula * > &,
      SolucaoOperacional & ) const;

   bool checkClassAndLessonDisponibility(
      Aula &,
      std::vector< HorarioAula * > &,
      SolucaoOperacional & ) const;

protected:
   ProblemData * problem_data;
};

#endif
