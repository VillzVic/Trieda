#include "SolutionLoader.h"

#include <string>
#include <cstdlib>
#include <iostream>

#include "ProblemData.h"
#include "ProblemSolution.h"
#include "Variable.h"
#include "Constraint.h"
#include "ErrorHandler.h"

SolutionLoader::SolutionLoader( ProblemData * aProblemData, ProblemSolution * aProblemSolution )
{
   problemData = aProblemData;
   problemSolution = aProblemSolution;
}

SolutionLoader::~SolutionLoader()
{

}

void SolutionLoader::setFolgas( Variable * v )
{
   std::string restricaoStr;
   std::string auxUnidade;

   char buffer[100];
   if ( v == NULL )
   {
      return;
   }

   RestricaoViolada * restricaoViolada = new RestricaoViolada();
   switch ( v->getType() )
   {
	   case Variable::V_SLACK_DIST_CRED_DIA_SUPERIOR:
		   restricaoStr = "Fixa��o da distribui��o de cr�ditos por dia (fcp_{";
         sprintf(buffer, "%d", v->getDisciplina()->getId() );
         restricaoStr += buffer;
         restricaoStr += ",";
         sprintf(buffer, "%d", v->getDia());
         restricaoStr += buffer;
         restricaoStr += "})";
         restricaoViolada->setRestricao(restricaoStr);
         restricaoViolada->setUnidade("x");
         restricaoViolada->setValor(v->getValue());
         problemSolution->getFolgas()->add(restricaoViolada);
		   break;
	   case Variable::V_SLACK_DIST_CRED_DIA_INFERIOR:
         restricaoStr = "Fixa��o da distribui��o de cr�ditos por dia (fcm_{";
         sprintf(buffer, "%d", v->getDisciplina()->getId());
         restricaoStr += buffer;
         restricaoStr += ",";
         sprintf(buffer, "%d", v->getDia());
         restricaoStr += buffer;
         restricaoStr += "})";
         restricaoViolada->setRestricao(restricaoStr);
		   restricaoViolada->setUnidade("x");
		   restricaoViolada->setValor(v->getValue());
		   problemSolution->getFolgas()->add(restricaoViolada);
		   break;
	   case Variable::V_SLACK_ALOC_ALUNOS_CURSO_INCOMPAT:
         restricaoStr = "N�o permitir que alunos de cursos incompativeis compartilhem turmas (bs_{";
         sprintf(buffer, "%d", v->getDia());
         restricaoStr += buffer;
         restricaoStr += ",";
         sprintf(buffer, "%d", v->getDisciplina()->getId());
         restricaoStr += buffer;
         restricaoStr += ",";
         sprintf(buffer, "%d", v->getCurso()->getId());
         restricaoStr += buffer;
         restricaoStr += ",";
		 sprintf(buffer, "%d", v->getCursoIncompat()->getId());
         restricaoStr += buffer;
         restricaoStr += ",";
		 sprintf(buffer, "%d", v->getCampus()->getId());
         restricaoStr += buffer;
         restricaoStr += "})";
         restricaoViolada->setRestricao(restricaoStr);
		   restricaoViolada->setUnidade("b");
		   restricaoViolada->setValor(v->getValue());
		   problemSolution->getFolgas()->add(restricaoViolada);
		  break;
	   case Variable::V_SLACK_COMPARTILHAMENTO:
         restricaoStr = "N�o permitir que alunos de cursos diferentes compartilhem turmas (fc_{";
         sprintf(buffer, "%d", v->getDia());
         restricaoStr += buffer;
         restricaoStr += ",";
         sprintf(buffer, "%d", v->getDisciplina()->getId());
         restricaoStr += buffer;
         restricaoStr += ",";
		 sprintf(buffer, "%d", v->getParCursos().first->getId());
         restricaoStr += buffer;
         restricaoStr += ",";
		 sprintf(buffer, "%d", v->getParCursos().second->getId());
         restricaoStr += buffer;
		 restricaoStr += ",";
         sprintf(buffer, "%d", v->getCampus()->getId());
         restricaoStr += buffer;
         restricaoStr += "})";
         restricaoViolada->setRestricao(restricaoStr);
		   restricaoViolada->setUnidade("b");
		   restricaoViolada->setValor(v->getValue());
		   problemSolution->getFolgas()->add(restricaoViolada);
		  break;
	   case Variable::V_SLACK_DEMANDA:
		  restricaoStr = "Capacidade alocada tem que permitir atender demanda da disciplina (fd_{";
        sprintf(buffer, "%d", v->getDisciplina()->getId());
        restricaoStr += buffer;
        restricaoStr += ",";
        sprintf(buffer, "%d", v->getOferta()->getId());
        restricaoStr += buffer;
        restricaoStr += "})";
        restricaoViolada->setRestricao(restricaoStr);
        restricaoViolada->setUnidade("a");
        restricaoViolada->setValor(v->getValue());
		  problemSolution->getFolgas()->add(restricaoViolada);
		  break;
	   case Variable::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M: 
      case Variable::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P: 
		   restricaoStr = "Regra de divis�o de cr�ditos (fk_{";
         sprintf(buffer, "%d", v->getTurma());
         restricaoStr += buffer;
         restricaoStr += ",";
         sprintf(buffer, "%d", v->getDisciplina()->getId());
         restricaoStr += buffer;
         restricaoStr += ",";
         sprintf(buffer, "%d", v->getK());
         restricaoStr += buffer;
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
