#include "SolutionLoader.h"

#include <iostream>

#include "ProblemData.h"
#include "ProblemSolution.h"
#include "Variable.h"
#include "Constraint.h"

//#include "Util.h"
#include "ErrorHandler.h"

SolutionLoader::SolutionLoader(ProblemData* aProblemData, ProblemSolution* aProblemSolution)
{
   problemData = aProblemData;
   problemSolution = aProblemSolution;
}

SolutionLoader::~SolutionLoader()
{
}

void SolutionLoader::setFolgas(Variable* v)
{
   std::string restricaoStr;
   std::string auxUnidade;

   char buffer[100];

   if ( v == NULL )
      return;

   RestricaoViolada* restricaoViolada = new RestricaoViolada();

   switch (v->getType())
   {
   case Variable::V_SLACK_DIST_CRED_DIA_SUPERIOR:
	   restricaoStr = "Fixação da distribuição de créditos por dia (fcp_{";
	   restricaoStr += _itoa(v->getDisciplina()->getId(), buffer, 10);
	   restricaoStr += ",";
	   restricaoStr += _itoa(v->getDia(), buffer, 10);
	   restricaoStr += "})";
       restricaoViolada->setRestricao(restricaoStr);
	   restricaoViolada->setUnidade("x");
       restricaoViolada->setValor(v->getValue());
       problemSolution->getFolgas()->add(restricaoViolada);
       break;
   case Variable::V_SLACK_DIST_CRED_DIA_INFERIOR:
	   restricaoStr = "Fixação da distribuição de créditos por dia (fcm_{";
	   restricaoStr += _itoa(v->getDisciplina()->getId(), buffer, 10);
	   restricaoStr += ",";
	   restricaoStr += _itoa(v->getDia(), buffer, 10);
	   restricaoStr += "})";
       restricaoViolada->setRestricao(restricaoStr);
       restricaoViolada->setUnidade("x");
       restricaoViolada->setValor(v->getValue());
       problemSolution->getFolgas()->add(restricaoViolada);
       break;
   case Variable::V_SLACK_ALOC_ALUNOS_CURSO_INCOMPAT: 
	   restricaoStr = "Não permitir que alunos de cursos diferentes compartilhem turmas (bs_{";
	   restricaoStr += _itoa(v->getTurma(), buffer, 10);
	   restricaoStr += ",";
	   restricaoStr += _itoa(v->getDisciplina()->getId(), buffer, 10);
	   restricaoStr += ",";
	   restricaoStr += _itoa(v->getCurso()->getId(), buffer, 10);
	   restricaoStr += ",";
	   restricaoStr += _itoa(v->getCampus()->getId(), buffer, 10);
	   restricaoStr += "})";
       restricaoViolada->setRestricao(restricaoStr);
       restricaoViolada->setUnidade("b");
       restricaoViolada->setValor(v->getValue());
       problemSolution->getFolgas()->add(restricaoViolada);
      break;
   case Variable::V_SLACK_DEMANDA:
	  restricaoStr = "Capacidade alocada tem que permitir atender demanda da disciplina (fd_{";
	  restricaoStr += _itoa(v->getDisciplina()->getId(), buffer, 10);
	  restricaoStr += ",";
	  restricaoStr += _itoa(v->getOferta()->getId(), buffer, 10);
	  restricaoStr += "})";
      restricaoViolada->setRestricao(restricaoStr);
      restricaoViolada->setUnidade("a");
      restricaoViolada->setValor(v->getValue());
      problemSolution->getFolgas()->add(restricaoViolada);
      break;
   case Variable::V_SLACK_COMBINACAO_DIVISAO_CREDITO: 
	   restricaoStr = "Regra de divisão de créditos (fk_{";
	   restricaoStr += _itoa(v->getTurma(), buffer, 10);
	   restricaoStr += ",";
	   restricaoStr += _itoa(v->getDisciplina()->getId(), buffer, 10);
	   restricaoStr += ",";
	   restricaoStr += _itoa(v->getK(), buffer, 10);
	   restricaoStr += "})";
       restricaoViolada->setRestricao(restricaoStr);
       restricaoViolada->setUnidade("x");
       restricaoViolada->setValor(v->getValue());
       problemSolution->getFolgas()->add(restricaoViolada);
       break;
   default:
      delete restricaoViolada;
      return;
   }
}