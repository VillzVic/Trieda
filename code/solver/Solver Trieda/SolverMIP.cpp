#include "opt_cplex.h"
#include "ProblemData.h"
#include "ProblemSolution.h"
#include "SolverMIP.h"

SolverMIP::SolverMIP(ProblemData *aProblemData)
:Solver(aProblemData)
{
   alpha = beta = gamma = delta = lambda = 1.0;
   try
   {
	   lp = new OPT_CPLEX;
   }
   catch(...)
   {
   }
   
   solVars.clear();
}

SolverMIP::~SolverMIP()
{
   int i;

   if (lp != NULL) delete lp;

   for (i=0; i < (int)solVars.size(); i++)
   {
      if ( solVars[i] != NULL )
         delete solVars[i];
   }
   solVars.clear();
}

int SolverMIP::solve()
{
   int varNum = 0;
   int constNum = 0;

   lp->createLP("Solver Trieda", OPTSENSE_MAXIMIZE, PROB_LP);

#ifdef DEBUG
   printf("Creating LP...\n");
#endif

   /* Variable creation */

#ifdef DEBUG
   printf("Total of Variables: %i\n\n",varNum);
#endif

   /* Constraint creation */

#ifdef DEBUG
   printf("Total of Constraints: %i\n",constNum);
#endif

#ifdef DEBUG
   lp->writeProbLP("Solver Trieda");
#endif

   int status = lp->optimize(METHOD_PRIMAL);

   return status;
}


void SolverMIP::getSolution(ProblemSolution *problemSolution)
{
   double *xSol = NULL;
   VariableHash::iterator vit;

   xSol = new double[lp->getNumCols()];
   lp->getX(xSol);

   vit = vHash.begin();

#ifdef DEBUG
   FILE *fout = fopen("solucao.txt","wt");
#endif

   while (vit != vHash.end())
   {
      Variable *v = new Variable(vit->first);
      int col = vit->second;

      v->setValue( xSol[col] );

      if ( v->getValue() > 0.00001 )
      {
#ifdef DEBUG
         char auxName[100];
         lp->getColName(auxName,col,100);
         fprintf(fout,"%s = %f\n",auxName,v->getValue());
#endif
         /**
         ToDo:
         */
      }

      vit++;
   }

   // Fill the solution

#ifdef DEBUG
   if ( fout )
      fclose(fout);
#endif

   if ( xSol )
      delete[] xSol;
}


int SolverMIP::cria_variaveis(void)
{
   int num_vars = 0;
   num_vars += cria_variavel_creditos();
   return num_vars;
}

int SolverMIP::cria_variavel_creditos(void)
{
   int num_vars = 0;

   ITERA_GGROUP(it_unidades,problemData->unidades,Unidade) 
   {
      ITERA_GGROUP(it_salas,it_unidades->salas,Sala) 
      {
         ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
         {
            ITERA_GGROUP(it_turma,it_disc->turmas,Turma)
            {
               for(int dia=0;dia<7;dia++)
               {
                  Variable v;
                  v.reset();
                  v.setType(Variable::V_CREDITOS);
                  v.setUnidade(*it_unidades);
                  v.setSala(*it_salas);
                  v.setDisciplina(*it_disc);
                  v.setTurma(*it_turma);
                  v.setDia(dia);
                  if (vHash.find(v) == vHash.end())
                  {
                     vHash[v] = lp->getNumCols();

                     OPT_COL col(OPT_COL::VAR_INTEGRAL,0.0,0.0,24.0,
                        (char*)v.toString().c_str());

                     lp->newCol(col);

                     num_vars += 1;
                  }
               }
            }
         }
      }
   }

   return num_vars;
}

int SolverMIP::cria_variavel_oferecimentos(void)
{
   int num_vars = 0;

   ITERA_GGROUP(it_unidades,problemData->unidades,Unidade) 
   {
      ITERA_GGROUP(it_salas,it_unidades->salas,Sala) 
      {
         ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
         {
            ITERA_GGROUP(it_turma,it_disc->turmas,Turma)
            {
               for(int dia=0;dia<7;dia++)
               {
                  Variable v;
                  v.reset();
                  v.setType(Variable::V_OFERECIMENTO);
                  v.setTurma(*it_turma);
                  v.setDisciplina(*it_disc);
                  v.setUnidade(*it_unidades);
                  v.setSala(*it_salas);
                  v.setDia(dia);
                  if (vHash.find(v) == vHash.end())
                  {
                     vHash[v] = lp->getNumCols();

                     OPT_COL col(OPT_COL::VAR_BINARY,0.0,0.0,1.0,
                        (char*)v.toString().c_str());

                     lp->newCol(col);

                     num_vars += 1;
                  }
               }
            }
         }
      }
   }


   return num_vars;
}

int SolverMIP::cria_variavel_abertura(void)
{
   int num_vars = 0;
   ITERA_GGROUP(it_unidades,problemData->unidades,Unidade)
   {
      ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
      {
         int max_demanda = -1;
         int demanda_unidade = 0;
         ITERA_GGROUP(it_dem,it_disc->demandas,Demanda) 
         {
            max_demanda = std::max(max_demanda,it_dem->quantidade);
            if (it_dem->unidade == *it_unidades)
               demanda_unidade += it_dem->quantidade;
         }
         ITERA_GGROUP(it_turma,it_disc->turmas,Turma)
         {
            Variable v;
            v.reset();
            v.setType(Variable::V_ABERTURA);
            v.setTurma(*it_turma);
            v.setDisciplina(*it_disc);
            v.setUnidade(*it_unidades);

            double ratiodem = (max_demanda - demanda_unidade)/
                              max_demanda;
            double coeff = alpha + gamma*ratiodem;

            if (vHash.find(v) == vHash.end())
            {
               vHash[v] = lp->getNumCols();

               OPT_COL col(OPT_COL::VAR_BINARY,coeff,0.0,1.0,
                  (char*)v.toString().c_str());

               lp->newCol(col);

               num_vars += 1;
            }
         }
      }
   }
   return num_vars;
}

int SolverMIP::cria_variavel_alunos(void)
{
   int num_vars = 0;
   ITERA_GGROUP(it_unidades,problemData->unidades,Unidade)
   {
      ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
      {
         ITERA_GGROUP(it_turma,it_disc->turmas,Turma)
         {
            Variable v;
            v.reset();
            v.setType(Variable::V_ALUNOS);
            v.setTurma(*it_turma);
            v.setDisciplina(*it_disc);
            v.setUnidade(*it_unidades);

            if (vHash.find(v) == vHash.end())
            {
               vHash[v] = lp->getNumCols();

               OPT_COL col(OPT_COL::VAR_INTEGRAL,0.0,0.0,1000.0,
                  (char*)v.toString().c_str());

               lp->newCol(col);

               num_vars += 1;
            }
         }
      }
   }
   return num_vars;
}

int SolverMIP::cria_variavel_consecutivos(void)
{
   int num_vars = 0;
   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
   {
      ITERA_GGROUP(it_turma,it_disc->turmas,Turma)
      {
         for(int dia=1;dia<7;dia++) 
         {
            Variable v;
            v.reset();
            v.setType(Variable::V_DIAS_CONSECUTIVOS);
            v.setTurma(*it_turma);
            v.setDisciplina(*it_disc);
            v.setDia(dia);

            if (vHash.find(v) == vHash.end())
            {
               vHash[v] = lp->getNumCols();

               OPT_COL col(OPT_COL::VAR_BINARY,delta,0.0,1.0,
                  (char*)v.toString().c_str());

               lp->newCol(col);

               num_vars += 1;
            }
         }
      }
   }
   return num_vars;
}
