#include "ProblemDataLoader.h"
#include "ProblemSolution.h"
#include "TRIEDA-InputXSD.h"
#include <iostream>

using namespace std;

ProblemDataLoader::ProblemDataLoader( char *inputFile, ProblemData* data )
{
   this->inputFile = inputFile;
   this->problemData  = data;
}

ProblemDataLoader::~ProblemDataLoader()
{
}

void ProblemDataLoader::load()
{
   std::cout << "Loading file..." << std::endl;
   root = std::auto_ptr<TriedaInput>(TriedaInput_(inputFile, xml_schema::flags::dont_validate));
   std::cout << "Extracting data..." << std::endl;
   problemData->le_arvore(*root);

   std::cout << "Some preprocessing..." << std::endl;
   /* processamento */
   /* cria blocos curriculares */
   ITERA_GGROUP(it_curso,problemData->cursos,Curso)
   {
      ITERA_GGROUP(it_curr,it_curso->curriculos,Curriculo)
      {
         GGroup<DisciplinaPeriodo>::iterator it_dp = 
            it_curr->disciplinas_periodo.begin();
         for(;it_dp != it_curr->disciplinas_periodo.end(); ++it_dp)
         {
            DisciplinaPeriodo dp = *it_dp;
            int p = dp.first;
            Disciplina* d = dp.second;
            BlocoCurricular* b = new BlocoCurricular;
            b->curso = *it_curso;
            b->periodo = p;
            GGroup<BlocoCurricular*>::iterator it_bc = 
               problemData->blocos.find(b);
            if (it_bc != problemData->blocos.end())
            {
               delete b;
               b = *it_bc;
            }
            b->disciplinas.add(d);
         }
      }
   }
}
