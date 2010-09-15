#ifndef PROBLEMSOLUTION_H
#define PROBLEMSOLUTION_H

#include <ostream>
#include "GGroup.h"
#include "ProblemData.h"

//#include "Oferecimento.h"
#include "AtendimentoCampus.h"

using namespace std;

//Stores output data
class ProblemSolution
{
public:
   //Constructor
   ProblemSolution();

   //Destructor
   ~ProblemSolution() {}

   //==================================================
   // SET METHODS 
   //==================================================

   /**
   ToDo:
   All set methods of the private attributes should be defined here
   */

   //==================================================
   // GET METHODS 
   //==================================================

   /**
   ToDo:
   All get methods of the private attributes should be defined here
   */

//private:

   /**
   ToDo:
   All objects that define the problem output should be declared here
   **/

   //GGroup<Oferecimento*> oferecimentos;
   
   GGroup<AtendimentoCampus*> atendimento_campus;

};

std::ostream& operator << (std::ostream& out, ProblemSolution& solution );

#endif
