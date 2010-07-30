#ifndef PROBLEMDATA_H
#define PROBLEMDATA_H

#include "GGroup.h"
#include "Curso.h"
#include "Unidade.h"
#include "Disciplina.h"
#include "DivisaoCreditos.h"
#include "Professor.h"
#include "Unidade.h"
#include "Calendario.h"

//Stores input data
class ProblemData
{
public:
   //Constructor for initial state
   ProblemData();

   //Destructor
   ~ProblemData();

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

private:

   /**
   ToDo:
   All objects that define the problem input should be declared here
   **/
   GGroup<Unidade*> unidades;
   GGroup<Curso*> curso;
   GGroup<Professor*> professores;
   Calendario* calendario;
   GGroup<Disciplina*> disciplinas;
   GGroup<DivisaoCreditos*> regrasCredito;
};

#endif
