#include "opt_cplex.h"
#include "ProblemData.h"
#include "ProblemSolution.h"
#include "SolverMIP.h"

SolverMIP::SolverMIP(ProblemData *aProblemData)
:Solver(aProblemData)
{
   alpha = beta = gamma = delta = lambda = M = 1.0;
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

   lp->createLP("Solver Trieda", OPTSENSE_MINIMIZE, PROB_MIP);

#ifdef DEBUG
   printf("Creating LP...\n");
#endif

   /* Variable creation */
   varNum = cria_variaveis();

#ifdef DEBUG
   printf("Total of Variables: %i\n\n",varNum);
#endif

   /* Constraint creation */
   constNum = cria_restricoes();

#ifdef DEBUG
   printf("Total of Constraints: %i\n",constNum);
#endif

#ifdef DEBUG
   lp->writeProbLP("Solver Trieda");
#endif

   int status = lp->optimize(METHOD_MIP);

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

   num_vars += cria_variavel_abertura();
   num_vars += cria_variavel_alunos();
   num_vars += cria_variavel_consecutivos();
   num_vars += cria_variavel_creditos();
   num_vars += cria_variavel_max_creds();
   num_vars += cria_variavel_min_creds();
   num_vars += cria_variavel_oferecimentos();
   num_vars += cria_variavel_turma_bloco();

   return num_vars;
}
int SolverMIP::cria_restricoes(void) {
   int restricoes = 0;

   restricoes += cria_restricao_carga();
   restricoes += cria_restricao_max_creditos_sd();
   restricoes += cria_restricao_min_creditos();
   restricoes += cria_restricao_ativacao();
   restricoes += cria_restricao_sobreposicao();
   restricoes += cria_restricao_mesma_unidade();
   restricoes += cria_restricao_max_creditos();
   restricoes += cria_restricao_turmas_bloco();
   restricoes += cria_restricao_cap_demanda();
   restricoes += cria_restricao_cap_sala();
   restricoes += cria_restricao_cap_sala_unidade();
   restricoes += cria_restricao_dias_consecutivos();

   return restricoes;
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
               lp->getNumCols();
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

int SolverMIP::cria_restricao_carga(void)
{
   int restricoes = 0;
   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;
   ITERA_GGROUP(it_unidade,problemData->unidades,Unidade) {
      ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
         ITERA_GGROUP(it_turma,it_disc->turmas,Turma) {

            c.reset();
            c.setType(Constraint::C_CARGA_HORARIA);
            c.setUnidade(*it_unidade);
            c.setDisciplina(*it_disc);
            c.setTurma(*it_turma);

            sprintf( name, "%s", c.toString().c_str() ); 

            if (cHash.find(c) != cHash.end()) continue;

            cHash[ c ] = lp->getNumRows();

            nnz = it_unidade->salas.size() * 7;
            int creditos = it_disc->cred_praticos + 
                           it_disc->cred_teoricos;

            OPT_ROW row( nnz + 1, OPT_ROW::EQUAL , 0, name );
            
            v.reset();
            v.setType(Variable::V_ABERTURA);

            v.setTurma(*it_turma);
            v.setDisciplina(*it_disc);
            v.setUnidade(*it_unidade);

            it_v = vHash.find(v);
            if( it_v != vHash.end() )
            {
               row.insert(it_v->second, -creditos);
            }

            ITERA_GGROUP(it_sala,it_unidade->salas,Sala) {
               for(int dia=0;dia<7;dia++) {

                  v.reset();
                  v.setType(Variable::V_CREDITOS);

                  v.setTurma(*it_turma);
                  v.setDisciplina(*it_disc);
                  v.setUnidade(*it_unidade);
                  v.setSala(*it_sala);
                  v.setDia(dia);

                  it_v = vHash.find(v);
                  if( it_v != vHash.end() )
                  {
                     row.insert(it_v->second, 1.0);
                  }
               }
            }
            lp->addRow(row);
            restricoes++;
         }
      }
   }

   return restricoes;
}

int SolverMIP::cria_restricao_max_creditos_sd(void)
{
   int restricoes = 0;
   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(it_unidade,problemData->unidades,Unidade) 
   {
      ITERA_GGROUP(it_sala,it_unidade->salas,Sala) 
      {
         for(int dia=0;dia<7;dia++) 
         {
            c.reset();
            c.setType(Constraint::C_MAX_CREDITOS_SD);

            c.setUnidade(*it_unidade);
            c.setSala(*it_sala);
            c.setDia(dia);

            sprintf( name, "%s", c.toString().c_str() ); 

            if (cHash.find(c) != cHash.end()) continue;

            cHash[ c ] = lp->getNumRows();

            nnz = 0;
            ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
               nnz += it_disc->turmas.size();

            int max_creds = 0; //TODO, achar max_creds
            OPT_ROW row( nnz, OPT_ROW::LESS , max_creds, name );
              
            ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
               ITERA_GGROUP(it_turma,it_disc->turmas,Turma) {

                  v.reset();
                  v.setType(Variable::V_CREDITOS);

                  v.setTurma(*it_turma);
                  v.setDisciplina(*it_disc);
                  v.setUnidade(*it_unidade);
                  v.setSala(*it_sala);
                  v.setDia(dia);

                  it_v = vHash.find(v);
                  if( it_v != vHash.end() )
                  {
                     row.insert(it_v->second, 1.0);
                  }
               }
            }
            lp->addRow(row);
            restricoes++;

         }
      }
   }
   return restricoes;
}

int SolverMIP::cria_restricao_min_creditos(void)
{
   int restricoes = 0;
   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(it_unidade,problemData->unidades,Unidade) 
   {
      ITERA_GGROUP(it_sala,it_unidade->salas,Sala) 
      {
         for(int dia=0;dia<7;dia++) 
         {
            ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) 
            {
               ITERA_GGROUP(it_turma,it_disc->turmas,Turma) 
               {

                  c.reset();
                  c.setType(Constraint::C_MIN_CREDITOS);

                  c.setUnidade(*it_unidade);
                  c.setSala(*it_sala);
                  c.setDia(dia);
                  c.setDisciplina(*it_disc);
                  c.setTurma(*it_turma);

                  sprintf( name, "%s", c.toString().c_str() ); 

                  if (cHash.find(c) != cHash.end()) continue;

                  cHash[ c ] = lp->getNumRows();

                  nnz = 2;

                  int min_creds = 0; //TODO, achar min_creds
                  
                  OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );
              
                  v.reset();
                  v.setType(Variable::V_OFERECIMENTO);

                  v.setTurma(*it_turma);
                  v.setDisciplina(*it_disc);
                  v.setUnidade(*it_unidade);
                  v.setSala(*it_sala);
                  v.setDia(dia);

                  it_v = vHash.find(v);
                  if( it_v != vHash.end() )
                  {
                     row.insert(it_v->second, min_creds);
                  }

                  v.reset();
                  v.setType(Variable::V_CREDITOS);

                  v.setTurma(*it_turma);
                  v.setDisciplina(*it_disc);
                  v.setUnidade(*it_unidade);
                  v.setSala(*it_sala);
                  v.setDia(dia);

                  it_v = vHash.find(v);
                  if( it_v != vHash.end() )
                  {
                     row.insert(it_v->second, -1.0);
                  }

                  lp->addRow(row);
                  restricoes++;
               }
            }
         }
      }
   }
   return restricoes;
}

int SolverMIP::cria_restricao_ativacao(void)
{
   int restricoes = 0;
   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(it_unidade,problemData->unidades,Unidade) 
   {
      ITERA_GGROUP(it_sala,it_unidade->salas,Sala) 
      {
         for(int dia=0;dia<7;dia++) 
         {
            ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) 
            {
               ITERA_GGROUP(it_turma,it_disc->turmas,Turma) 
               {

                  c.reset();
                  c.setType(Constraint::C_VAR_O);

                  c.setUnidade(*it_unidade);
                  c.setSala(*it_sala);
                  c.setDia(dia);
                  c.setDisciplina(*it_disc);
                  c.setTurma(*it_turma);

                  sprintf( name, "%s", c.toString().c_str() ); 

                  if (cHash.find(c) != cHash.end()) continue;

                  cHash[ c ] = lp->getNumRows();

                  nnz = 2;

                  int creds = it_disc->cred_praticos + 
                              it_disc->cred_teoricos;
                  
                  OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0, name );
              
                  v.reset();
                  v.setType(Variable::V_OFERECIMENTO);

                  v.setTurma(*it_turma);
                  v.setDisciplina(*it_disc);
                  v.setUnidade(*it_unidade);
                  v.setSala(*it_sala);
                  v.setDia(dia);

                  it_v = vHash.find(v);
                  if( it_v != vHash.end() )
                  {
                     row.insert(it_v->second, creds);
                  }

                  v.reset();
                  v.setType(Variable::V_CREDITOS);

                  v.setTurma(*it_turma);
                  v.setDisciplina(*it_disc);
                  v.setUnidade(*it_unidade);
                  v.setSala(*it_sala);
                  v.setDia(dia);

                  it_v = vHash.find(v);
                  if( it_v != vHash.end() )
                  {
                     row.insert(it_v->second, -1.0);
                  }

                  lp->addRow(row);
                  restricoes++;
               }
            }
         }
      }
   }
   return restricoes;
}

int SolverMIP::cria_restricao_sobreposicao(void)
{
   int restricoes = 0;
   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(it_unidade,problemData->unidades,Unidade) 
   {
      for(int dia=0;dia<7;dia++) 
      {
         ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) 
         {
            ITERA_GGROUP(it_turma,it_disc->turmas,Turma) 
            {

               c.reset();
               c.setType(Constraint::C_EVITA_SOBREPOSICAO);

                  c.setUnidade(*it_unidade);
                  //c.setSala(*it_sala);
                  c.setDia(dia);
                  c.setDisciplina(*it_disc);
                  c.setTurma(*it_turma);

               sprintf( name, "%s", c.toString().c_str() ); 

               if (cHash.find(c) != cHash.end()) continue;

               cHash[ c ] = lp->getNumRows();

               nnz = it_unidade->salas.size();

               int creds = it_disc->cred_praticos + 
                  it_disc->cred_teoricos;
                  
               OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );
              
               ITERA_GGROUP(it_sala,it_unidade->salas,Sala) 
               {

                  v.reset();
                  v.setType(Variable::V_OFERECIMENTO);

                  v.setTurma(*it_turma);
                  v.setDisciplina(*it_disc);
                  v.setUnidade(*it_unidade);
                  v.setSala(*it_sala);
                  v.setDia(dia);

                  it_v = vHash.find(v);
                  if( it_v != vHash.end() )
                  {
                     row.insert(it_v->second, creds);
                  }
               }
               lp->addRow(row);
               restricoes++;
            }
         }
      }
   }
   return restricoes;
}

int SolverMIP::cria_restricao_mesma_unidade(void)
{
   int restricoes = 0;
   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;
   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
      ITERA_GGROUP(it_turma,it_disc->turmas,Turma) {

         c.reset();
         c.setType(Constraint::C_MESMA_UNIDADE);

//         c.setUnidade(*it_unidade);
//         c.setSala(*it_sala);
         //         c.setDia(dia);
         c.setDisciplina(*it_disc);
         c.setTurma(*it_turma);

         sprintf( name, "%s", c.toString().c_str() ); 

         if (cHash.find(c) != cHash.end()) continue;

         cHash[ c ] = lp->getNumRows();

         nnz = problemData->unidades.size();

         OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );
            
         ITERA_GGROUP(it_unidade,problemData->unidades,Unidade) {

            v.reset();
            v.setType(Variable::V_ABERTURA);

            v.setTurma(*it_turma);
            v.setDisciplina(*it_disc);
            v.setUnidade(*it_unidade);

            it_v = vHash.find(v);
            if( it_v != vHash.end() )
            {
               row.insert(it_v->second, 1.0);
            }
         }
         lp->addRow(row);
         restricoes++;
      }
   }
   return restricoes;
}

int SolverMIP::cria_restricao_max_creditos(void)
{
   int restricoes = 0;
   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(it_unidade,problemData->unidades,Unidade) 
   {
      ITERA_GGROUP(it_sala,it_unidade->salas,Sala) 
      {
         for(int dia=0;dia<7;dia++) 
         {
            ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) 
            {
               ITERA_GGROUP(it_turma,it_disc->turmas,Turma) 
               {

                  c.reset();
                  c.setType(Constraint::C_MAX_CREDITOS);

                  c.setUnidade(*it_unidade);
                  c.setSala(*it_sala);
                  c.setDia(dia);
                  c.setDisciplina(*it_disc);
                  c.setTurma(*it_turma);

                  sprintf( name, "%s", c.toString().c_str() ); 

                  if (cHash.find(c) != cHash.end()) continue;

                  cHash[ c ] = lp->getNumRows();

                  nnz = 1;

                  int max_creds = 0; //TODO, achar min_creds
                  
                  OPT_ROW row( nnz, OPT_ROW::LESS , max_creds, name );
              
                  v.reset();
                  v.setType(Variable::V_CREDITOS);

                  v.setTurma(*it_turma);
                  v.setDisciplina(*it_disc);
                  v.setUnidade(*it_unidade);
                  v.setSala(*it_sala);
                  v.setDia(dia);

                  it_v = vHash.find(v);
                  if( it_v != vHash.end() )
                  {
                     row.insert(it_v->second, -1.0);
                  }

                  lp->addRow(row);
                  restricoes++;
               }
            }
         }
      }
   }
   return restricoes;
}

int SolverMIP::cria_variavel_turma_bloco(void)
{
   int num_vars = 0;
   ITERA_GGROUP(it_unidades,problemData->unidades,Unidade)
   {
      ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular)
      {
         for(int dia=0;dia<7;dia++)
         {
            Variable v;
            v.reset();
            v.setType(Variable::V_TURMA_BLOCO);
            v.setBloco(*it_bloco);
            v.setDia(dia);
            v.setUnidade(*it_unidades);

            if (vHash.find(v) == vHash.end())
            {
               vHash[v] = lp->getNumCols();

               /* PERGUNTAR PRO MARCELO OU PRO ANDRE */
               OPT_COL col(OPT_COL::VAR_BINARY,beta/*coef*/,0.0,1.0,
                  (char*)v.toString().c_str());

               lp->newCol(col);

               num_vars += 1;
            }
         }
      }
   }
   return num_vars;
}

int SolverMIP::cria_variavel_min_creds(void)
{
   int num_vars = 0;
   ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular)
   {
      ITERA_GGROUP(it_disc,it_bloco->disciplinas,Disciplina)
      {
         ITERA_GGROUP(it_turma,it_disc->turmas,Turma)
         {
            Variable v;
            v.reset();
            v.setType(Variable::V_MIN_CRED_SEMANA);
            v.setTurma(*it_turma);
            v.setBloco(*it_bloco);

            if (vHash.find(v) == vHash.end())
            {
               vHash[v] = lp->getNumCols();

               OPT_COL col(OPT_COL::VAR_INTEGRAL,-lambda,0.0,1000.0,
                  (char*)v.toString().c_str());

               lp->newCol(col);

               num_vars += 1;
            }
         }
      }
   }
   return num_vars;
}

int SolverMIP::cria_variavel_max_creds(void)
{
   int num_vars = 0;
   ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular)
   {
      ITERA_GGROUP(it_disc,it_bloco->disciplinas,Disciplina)
      {
         ITERA_GGROUP(it_turma,it_disc->turmas,Turma)
         {
            Variable v;
            v.reset();
            v.setType(Variable::V_MAX_CRED_SEMANA);
            v.setTurma(*it_turma);
            v.setBloco(*it_bloco);

            if (vHash.find(v) == vHash.end())
            {
               vHash[v] = lp->getNumCols();

               OPT_COL col(OPT_COL::VAR_INTEGRAL,lambda,0.0,1000.0,
                  (char*)v.toString().c_str());

               lp->newCol(col);

               num_vars += 1;
            }
         }
      }
   }
   return num_vars;
}

int SolverMIP::cria_restricao_turmas_bloco(void)
{
   int restricoes = 0;
   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(it_unidade,problemData->unidades,Unidade) 
   {
      for(int dia=0;dia<7;dia++) 
      {
         ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular) 
         {


            c.reset();
            c.setType(Constraint::C_TURMAS_BLOCO);

            c.setUnidade(*it_unidade);
            c.setBloco(*it_bloco);
            c.setDia(dia);

            sprintf( name, "%s", c.toString().c_str() ); 

            if (cHash.find(c) != cHash.end()) continue;

            cHash[ c ] = lp->getNumRows();
            nnz = 0;

            ITERA_GGROUP(it_disc,it_bloco->disciplinas,Disciplina) 
               nnz += it_disc->turmas.size();

            nnz *= it_unidade->salas.size();
            nnz += 1;

            OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

            v.reset();
            v.setType(Variable::V_TURMA_BLOCO);

            v.setBloco(*it_bloco);
            v.setUnidade(*it_unidade);
            v.setDia(dia);

            it_v = vHash.find(v);
            if( it_v != vHash.end() )
            {
               row.insert(it_v->second, -M);
            }

            ITERA_GGROUP(it_sala,it_unidade->salas,Sala) 
            {
               ITERA_GGROUP(it_disc,it_bloco->disciplinas,Disciplina) 
               {
                  ITERA_GGROUP(it_turma,it_disc->turmas,Turma) 
                  {
                     v.reset();
                     v.setType(Variable::V_OFERECIMENTO);

                     v.setTurma(*it_turma);
                     v.setDisciplina(*it_disc);
                     v.setUnidade(*it_unidade);
                     v.setSala(*it_sala);
                     v.setDia(dia);

                     it_v = vHash.find(v);
                     if( it_v != vHash.end() )
                     {
                        row.insert(it_v->second, 1.0);
                     }
                  }
               }
            }
            lp->addRow(row);
            restricoes++;
         }
      }
   }
   return restricoes;
}

int SolverMIP::cria_restricao_cap_demanda(void)
{
   int restricoes = 0;
   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) 
   {
      c.reset();
      c.setType(Constraint::C_CAP_DEMANDA);
      c.setDisciplina(*it_disc);

      sprintf( name, "%s", c.toString().c_str() ); 

      if (cHash.find(c) != cHash.end()) continue;

      cHash[ c ] = lp->getNumRows();

      nnz = problemData->unidades.size() * it_disc->turmas.size();

      int total_demanda = 0;
      ITERA_GGROUP(it_unidade,problemData->unidades,Unidade)
         ITERA_GGROUP(it_dem,it_disc->demandas,Demanda) 
            total_demanda += it_dem->quantidade;

      OPT_ROW row( nnz, OPT_ROW::GREATER , total_demanda, name );

      ITERA_GGROUP(it_unidade,problemData->unidades,Unidade)
      {
         ITERA_GGROUP(it_turma,it_disc->turmas,Turma)
         {
            v.reset();
            v.setType(Variable::V_ALUNOS);

            v.setUnidade(*it_unidade);
            v.setDisciplina(*it_disc);
            v.setTurma(*it_turma);

            it_v = vHash.find(v);
            if( it_v != vHash.end() )
            {
               row.insert(it_v->second, 1.0);
            }
         }
      }
      lp->addRow(row);
      restricoes++;
   }
   return restricoes;
}

int SolverMIP::cria_restricao_cap_sala(void)
{
   int restricoes = 0;
   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;
   ITERA_GGROUP(it_unidade,problemData->unidades,Unidade) {
      ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
         ITERA_GGROUP(it_turma,it_disc->turmas,Turma) {

            c.reset();
            c.setType(Constraint::C_CAP_SALA);
            c.setUnidade(*it_unidade);
            c.setDisciplina(*it_disc);
            c.setTurma(*it_turma);

            sprintf( name, "%s", c.toString().c_str() ); 

            if (cHash.find(c) != cHash.end()) continue;

            cHash[ c ] = lp->getNumRows();

            nnz = it_unidade->salas.size() * 7;

            OPT_ROW row( nnz + 1, OPT_ROW::GREATER , 0.0, name );
            
            v.reset();
            v.setType(Variable::V_ALUNOS);

            v.setTurma(*it_turma);
            v.setDisciplina(*it_disc);
            v.setUnidade(*it_unidade);

            it_v = vHash.find(v);
            if( it_v != vHash.end() )
            {
               row.insert(it_v->second, -1.0);
            }

            ITERA_GGROUP(it_sala,it_unidade->salas,Sala) {
               for(int dia=0;dia<7;dia++) {

                  v.reset();
                  v.setType(Variable::V_OFERECIMENTO);

                  v.setTurma(*it_turma);
                  v.setDisciplina(*it_disc);
                  v.setUnidade(*it_unidade);
                  v.setSala(*it_sala);
                  v.setDia(dia);

                  it_v = vHash.find(v);
                  if( it_v != vHash.end() )
                  {
                     row.insert(it_v->second, it_sala->capacidade);
                  }
               }
            }
            lp->addRow(row);
            restricoes++;
         }
      }
   }

   return restricoes;
}

int SolverMIP::cria_restricao_cap_sala_unidade(void)
{
   int restricoes = 0;
   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(it_unidade,problemData->unidades,Unidade) 
   {
      ITERA_GGROUP(it_sala,it_unidade->salas,Sala) 
      {
         for(int dia=0;dia<7;dia++) 
         {
            ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) 
            {
               ITERA_GGROUP(it_turma,it_disc->turmas,Turma) 
               {

                  c.reset();
                  c.setType(Constraint::C_CAP_SALA_U);
                  c.setUnidade(*it_unidade);
                  c.setSala(*it_sala);
                  c.setDia(dia);
                  c.setDisciplina(*it_disc);
                  c.setTurma(*it_turma);

                  sprintf( name, "%s", c.toString().c_str() ); 

                  if (cHash.find(c) != cHash.end()) continue;

                  cHash[ c ] = lp->getNumRows();

                  nnz = 2;

                  double rhs = it_sala->capacidade + M;
                  OPT_ROW row( nnz, OPT_ROW::LESS , rhs, name );
              
                  v.reset();
                  v.setType(Variable::V_OFERECIMENTO);

                  v.setTurma(*it_turma);
                  v.setDisciplina(*it_disc);
                  v.setUnidade(*it_unidade);
                  v.setSala(*it_sala);
                  v.setDia(dia);

                  it_v = vHash.find(v);
                  if( it_v != vHash.end() )
                  {
                     row.insert(it_v->second, M);
                  }

                  v.reset();
                  v.setType(Variable::V_ALUNOS);

                  v.setTurma(*it_turma);
                  v.setDisciplina(*it_disc);
                  v.setUnidade(*it_unidade);

                  it_v = vHash.find(v);
                  if( it_v != vHash.end() )
                  {
                     row.insert(it_v->second, 1.0);
                  }

                  lp->addRow(row);
                  restricoes++;
               }
            }
         }
      }
   }
   return restricoes;
}

int SolverMIP::cria_restricao_dias_consecutivos(void)
{
   int restricoes = 0;
   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;
   for(int dia=1;dia<7;dia++) {
      ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
         ITERA_GGROUP(it_turma,it_disc->turmas,Turma) {

            c.reset();
            c.setType(Constraint::C_CAP_SALA);
            c.setTurma(*it_turma);
            c.setDisciplina(*it_disc);
            c.setDia(dia);

            sprintf( name, "%s", c.toString().c_str() ); 

            if (cHash.find(c) != cHash.end()) continue;

            cHash[ c ] = lp->getNumRows();

            nnz = 0;
            ITERA_GGROUP(it_unidade,problemData->unidades,Unidade)
               nnz += it_unidade->salas.size();

            OPT_ROW row( 2*nnz + 1, OPT_ROW::LESS , 1.0, name );
            
            v.reset();
            v.setType(Variable::V_DIAS_CONSECUTIVOS);

            v.setTurma(*it_turma);
            v.setDisciplina(*it_disc);
            v.setDia(dia);

            it_v = vHash.find(v);
            if( it_v != vHash.end() )
            {
               row.insert(it_v->second, -1.0);
            }

            ITERA_GGROUP(it_unidade,problemData->unidades,Unidade) 
            {
               ITERA_GGROUP(it_sala,it_unidade->salas,Sala) {

                  v.reset();
                  v.setType(Variable::V_OFERECIMENTO);

                  v.setTurma(*it_turma);
                  v.setDisciplina(*it_disc);
                  v.setUnidade(*it_unidade);
                  v.setSala(*it_sala);
                  v.setDia(dia-1);

                  it_v = vHash.find(v);
                  if( it_v != vHash.end() )
                  {
                     row.insert(it_v->second, -1.0);
                  }
                  v.setDia(dia);

                  it_v = vHash.find(v);
                  if( it_v != vHash.end() )
                  {
                     row.insert(it_v->second, 1.0);
                  }

               }
            }
            lp->addRow(row);
            restricoes++;
         }
      }
   }
   return restricoes;
}
