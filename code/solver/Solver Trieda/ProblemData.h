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
#include "BlocoCurricular.h"
#include <iostream>

//Stores input data
class ProblemData : public OFBase
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
   GGroup<Unidade*>* getUnidades() { return &unidades; }
   /**
   ToDo:
   All get methods of the private attributes should be defined here
   */

//private:

   /**
   ToDo:
   All objects that define the problem input should be declared here
   **/
   GGroup<Unidade*> unidades;
   GGroup<Curso*> cursos;
   GGroup<Professor*> professores;
   Calendario* calendario;
   GGroup<Disciplina*> disciplinas;
   GGroup<DivisaoCreditos*> regras_credito;
   GGroup<BlocoCurricular*> blocos;
public:
   virtual void le_arvore(Trieda& raiz);
};

#endif
