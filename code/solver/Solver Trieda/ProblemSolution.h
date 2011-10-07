#ifndef _PROBLEM_SOLUTION_H_
#define _PROBLEM_SOLUTION_H_

#include <ostream>

#include "GGroup.h"
#include "ProblemData.h"
#include "SolucaoOperacional.h"
#include "AtendimentoCampus.h"
#include "RestricaoViolada.h"
#include "ProfessorVirtualOutput.h"
#include "Aula.h"

// Stores output data
class ProblemSolution
{
public:
   ProblemSolution( bool = true );
   virtual ~ProblemSolution(); 

   RestricaoVioladaGroup * getFolgas() const { return this->folgas; }
   GGroup< AtendimentoCampus * > * atendimento_campus;
   SolucaoOperacional * solucao_operacional;
   GGroup< ProfessorVirtualOutput * > * professores_virtuais;
   GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > > * alunosDemanda;
   bool modoOtmTatico;
   int getIdAtendimentos() { return this->idsAtendimentos++; }

private:
   RestricaoVioladaGroup * folgas;
   int idsAtendimentos;

   // Armazena todas as salas (sala de aula ou lab) em que uma
   // disciplina pode ser oferecida. Pode ser que uma disciplina
   // seja associada à duas salas com capacidades diferentes.
   // Quando o solver alocar essa disciplina a um tipo de sala,
   // a outra sala não deverá constar na lista de salas em que
   // essa disciplina poderá ser alocada. Portanto, a estrutura
   // abaixo, visa armazenar todas as salas em que uma disciplina 
   // pode ser alocada levando em conta o resultado do solver.
   // std::map< int /*Id Disc*/, GGroup< Sala * > > discSalas;
};

std::ostream & operator << ( std::ostream &, ProblemSolution & );

#endif
