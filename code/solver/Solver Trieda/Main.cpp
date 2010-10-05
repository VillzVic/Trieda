// OptFramework.cpp : Defines the entry point for the console application.
//

#include <fstream>
#include <cstdio>
#include <exception>
#include "ProblemSolution.h"
#include "ProblemData.h"
#include "ProblemDataLoader.h"
#include "SolverMIP.h"
#include "ErrorHandler.h"

// >>>
GGroup<int/*ids de campus existentes*/> * AtendimentoCampus::campi_id = new GGroup<int>;

GGroup<int/*ids de unidades existentes*/> * AtendimentoUnidade::__ids_cadastrados = new GGroup<int>;
GGroup<int/*ids de salas existentes*/> * AtendimentoSala::__ids_cadastrados = new GGroup<int>;
GGroup<int/*ids dos dias existentes*/> * AtendimentoDiaSemana::__ids_cadastrados = new GGroup<int>;
// <<<


// >>>
/*
Disciplina& Disciplina::operator= (const Disciplina& d)
{
if (this == &d) return *this;   // self assignment

Disciplina *dd = new Disciplina();

dd->setId(d.getId());

dd->codigo = d.codigo;
dd->nome = d.nome;
dd->cred_teoricos = d.cred_teoricos;
dd->cred_praticos = d.cred_praticos;
dd->e_lab = d.e_lab;
dd->max_alunos_t = d.max_alunos_t;
dd->max_alunos_p = d.max_alunos_p;
dd->tipo_disciplina_id = d.tipo_disciplina_id;
dd->nivel_dificuldade_id = d.nivel_dificuldade_id;

//DivisaoCreditos* divisao_creditos;
dd->divisao_creditos = new DivisaoCreditos();
dd->divisao_creditos->setId(d.divisao_creditos->getId());
dd->divisao_creditos->creditos = d.divisao_creditos->creditos;
for(int i=0;i<8;i++) {
dd->divisao_creditos->dia[i] = d.divisao_creditos->dia[i];		
}

GGroup<int>::iterator it_eq = d.equivalentes.begin();

for(unsigned i=0;i<d.equivalentes.size();i++) {
dd->equivalentes.add(*it_eq);
it_eq++;
}

GGroup<int>::iterator it_inc = d.incompativeis.begin();

for(unsigned i=0;i<d.incompativeis.size();i++) {
dd->incompativeis.add(*it_inc);
it_inc++;
}

//>>> Copying HORARIO
ITERA_GGROUP(it_hr,d.horarios,Horario) {
Horario *h =  new Horario;
h->setId(it_hr->getId());

//>>> >>> Copying DiaSemana
GGroup<int>::iterator it_dia = it_hr->dias_semana.begin();
for(unsigned dia =0;dia<it_hr->dias_semana.size();dia++) {
h->dias_semana.add(*it_dia);
it_dia++;
}
// <<< <<<

h->horarioAulaId = it_hr->horarioAulaId;

h->turnoId = it_hr->turnoId;

// >>> >>> Copying TURNO
Turno *tur = new Turno();

tur->setId(it_hr->turno->getId());
tur->nome = it_hr->turno->nome;
tur->tempoAula = it_hr->turno->tempoAula;

// >>> >>> >>> Copying HorariosAula
HorarioAula *hr_aula;
ITERA_GGROUP(it_hr_aula,tur->horarios_aula,HorarioAula) {
//HorarioAula *hr_aula = new HorarioAula();
hr_aula = new HorarioAula();
hr_aula->setId(it_hr_aula->getId());
hr_aula->inicio = it_hr_aula->inicio;

GGroup<int>::iterator it_dia_sem = it_hr_aula->diasSemana.begin();
for(unsigned dia =0;dia<it_hr_aula->diasSemana.size();dia++) {
hr_aula->diasSemana.add(*it_dia_sem);
it_dia_sem++;
}
//tur->horarios_aula = hr_aula;
}
tur->horarios_aula.add(hr_aula);
// <<< <<< <<<
h->turno = tur;
// <<< <<<
dd->horarios.add(h);
}
// <<<

dd->tipo_disciplina->setId(d.tipo_disciplina->getId());
dd->tipo_disciplina->nome = d.tipo_disciplina->nome;

dd->nivel_dificuldade->setId(d.nivel_dificuldade->getId());
dd->nivel_dificuldade->nome = d.nivel_dificuldade->nome;

dd->demanda_total = d.demanda_total;
dd->max_demanda = d.max_demanda;
dd->num_turmas = d.num_turmas;
dd->min_creds = d.min_creds;
dd->max_creds = d.max_creds;

std::cout << "Disciplina::Operator= finalized\n";

return *dd;
}
// <<<

*/

#ifndef PATH_SEPARATOR
#ifdef WIN32
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif
#endif

int main(int argc, char** argv)
{
   char path[1024];
   char inputFile[1024];
   char tempOutput[1024];
   char outputFile[1024];
   bool error;
   std::ofstream file;

   ProblemDataLoader* dataLoader;
   ProblemData *data = new ProblemData();
   ProblemSolution* solution = new ProblemSolution();
   Solver* solver;

   //Initializations
   path[0] = '\0';
   inputFile[0] = '\0';
   tempOutput[0] = '\0';
   outputFile[0] = '\0';
   error = false;

   //Check command line
   if( argc <= 2 )
   {
      if ( argc == 2 && strcmp(argv[1],"-version") == 0 )
      {
         printf("%s_%s\n",__DATE__,__TIME__);
         return 0; 
      }
      else
      {
         printf("Invalid parameters in command line.\n");
         printf("Usage: solver <instanceID> <path>\n");
         return 0;
      }
   }

   //Read path
   strcat(path,argv[2]);
   strcat(path,PATH_SEPARATOR);

   //Input file name
   strcat(inputFile,path);
   strcat(inputFile,"input");
   strcat(inputFile,argv[1]);

   //Temporary output file name
   strcat(tempOutput,path);
   strcat(tempOutput,"partialSolution.xml");
   std::string tempOutputFile = tempOutput;

   //Output file name
   strcat(outputFile,path);
   strcat(outputFile,"output");
   strcat(outputFile,argv[1]);
   strcat(outputFile,"F");

   if( argc > 3 )
   {
      //Read other parameters
   }

#ifndef DEBUG
   try
   {
#endif
      //Load data
      dataLoader = new ProblemDataLoader(inputFile, data);   
      dataLoader->load();
      delete dataLoader;

      // solve the problem
      solver = new SolverMIP(data);
      solver->solve();
      solver->getSolution(solution);
      delete solver;

#ifndef DEBUG
   }
   catch( std::exception& e )
   {
      if( ErrorHandler::getErrorMessages().size() == 0 )
         ErrorHandler::addErrorMessage( UNEXPECTED_ERROR, "Ocorreu um erro interno no resolvedor.", "main.cpp", true );
      printf("\n\nERROR: %s\n",e.what());
      error = true;
   }
#endif

   //Write output
   try
   {
      remove(tempOutput);

      file.open(tempOutput);
      file << *solution;
      file.close();

      remove(outputFile);
      rename(tempOutput,outputFile);
   }
   catch( std::exception& e )
   {
      printf("\n\nAn error occurred during creation of output file.\n",e.what());
      printf("ERROR: %s\n",e.what()); 
      error = true;
   }

   if( error )
      return 0;
   return 1;
}

