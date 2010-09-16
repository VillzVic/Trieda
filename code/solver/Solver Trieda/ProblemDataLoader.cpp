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
            int disc_id = dp.second;
            Disciplina* d = new Disciplina;
            d->id = disc_id;
            if (problemData->disciplinas.find(d) != 
                problemData->disciplinas.end())
                d = *problemData->disciplinas.find(d);
            
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
template<class T> 
void ProblemDataLoader::find_and_set(int id, 
                                     GGroup<T*> haystack, 
                                     T*& needle) 
{
   T* finder = new T;
   finder->id = id;
   GGroup<T*>::iterator it_g = 
      haystack.find(finder);
   if (it_g != haystack.end()) 
      needle = *it_g;
   delete finder;
}

void ProblemDataLoader::gera_refs() {
   ITERA_GGROUP(it_campi,problemData->campi,Campus) {
      ITERA_GGROUP(it_unidades,it_campi->unidades,Unidade) {
         ITERA_GGROUP(it_horario,it_unidades->horarios,Horario) {
            find_and_set(it_horario->turnoId,
               problemData->calendario->turnos,
               it_horario->turno);
            find_and_set(it_horario->horarioAulaId,
               it_horario->turno->horarios_aula,
               it_horario->horario_aula);
         }
         ITERA_GGROUP(it_salas,it_unidades->salas,Sala) {
            find_and_set(it_salas->tipo_sala_id,
               problemData->tipos_sala,
               it_salas->tipo_sala);
            ITERA_GGROUP(it_horario,it_salas->horarios_disponiveis,Horario)
            {
               find_and_set(it_horario->turnoId,
                  problemData->calendario->turnos,
                  it_horario->turno);
               find_and_set(it_horario->horarioAulaId,
                  it_horario->turno->horarios_aula,
                  it_horario->horario_aula);
            }
            ITERA_GGROUP(it_credito,it_salas->creditos_disponiveis,
                         CreditoDisponivel) 
            {
               find_and_set(it_credito->turno_id,
                  problemData->calendario->turnos,
                  it_credito->turno);
            }
            /* Disciplinas associadas ? 
            TODO (ou não) */
         }
         // next: professor 
      }
   }
}
