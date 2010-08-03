#include "ProblemData.h"
#include <iostream>

ProblemData::ProblemData()
{
   /*
   ToDo:
   */
}

ProblemData::~ProblemData()
{
   /*
   ToDo:
   */
}

void ProblemData::le_arvore(Trieda& raiz)
{
   std::cout << "PData" << std::endl;
   calendario = new Calendario();
   calendario->le_arvore(raiz.calendario());

}
