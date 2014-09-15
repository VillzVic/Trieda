#ifndef _VALIDATE_SOLUTION_H_
#define _VALIDATE_SOLUTION_H_

#include <iostream>
#include <sstream>

#include "ProblemSolution.h"
#include "ProblemData.h"
#include "AtendimentoBase.h"

class ValidateSolutionOp
{
public:
   ValidateSolutionOp( void );
   virtual ~ValidateSolutionOp( void  );

   bool checkSolution( const ProblemData *, const ProblemSolution * );

private:
   bool checkRestricaoAlocacaoAulas( const ProblemData *, const ProblemSolution * );
   bool checkRestricaoProfessorHorario( const ProblemData *, const ProblemSolution * );
   bool checkRestricaoSalaHorario( const ProblemData *, const ProblemSolution * );
   bool checkRestricaoBlocoHorario( const ProblemData *, const ProblemSolution * );
   bool checkRestricaoFixProfDiscSalaDiaHorario( const ProblemData *, const ProblemSolution * );
   bool checkRestricaoFixProfDiscDiaHor( const ProblemData *, const ProblemSolution * );
   bool checkRestricaoDisciplinaMesmoHorario( const ProblemData *, const ProblemSolution * );
   bool checkRestricaoDeslocamentoViavel( const ProblemData *, const ProblemSolution * );
   bool checkRestricaoDeslocamentoProfessor( const ProblemData *, const ProblemSolution * );

   // Métodos utilitários
   bool deslocamentoViavel( const ProblemData *,
      const AtendimentoBase *, const AtendimentoBase * );
};

#endif