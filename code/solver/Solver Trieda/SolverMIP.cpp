#include "opt_cplex.h"
#include "ProblemData.h"
#include "ProblemSolution.h"
#include "SolverMIP.h"

// >>>
//#define PRINT_CSV
// <<<

SolverMIP::SolverMIP(ProblemData *aProblemData)
:Solver(aProblemData)
{
   //rho = 1 faz o codigo ficar mais lento, pq?
   alpha = beta = gamma = delta = lambda = epsilon = M = 1.0;

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
   lp->writeSolution("Solver Trieda.sol");

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

#ifdef PRINT_CSV
   FILE *f_V_CREDITOS = fopen("./CSV/V_CREDITOS.csv","wt");
   bool printLegend_V_CREDITOS = true;

   bool printLegend_V_ALUNOS = true;
   FILE *f_V_ALUNOS = fopen("./CSV/V_ALUNOS.csv","wt");
#endif

   /**
   ToDo:
   Ser� preciso preencher as classes "Atendimento" da seguinte forma:
   Ao encontrar um oferecimento, h� de se verificar qual campus, 
   unidade, sala, e dia da semana ele corresponde. 

   As vari�veis mais importantes s�o:
   x_{i,d,u,s,t}: indica quantos cr�ditos s�o oferecidos naquele 
   dia naquela sala daquela disciplina
   a_{i,d,c,cp}: para quantos alunos a disciplina ser� oferecida

   Essas duas vari�veis cont�m toda a informa��o necess�ria para
   constru��o da solu��o;

   Sugiro guardar esses valores em alguns maps:
   De <disciplina,turma> para um vetor de <dia,sala> (unidade)
   De <disciplina,turma> para alunos.
   */

   typedef
      std::map<std::pair<int,int>,std::vector<std::pair<int,int> > >
      ParVetor;
   typedef 
      std::map<std::pair<int,int>,int>
      ParInteiro;

   // >>>
   // A chave dever� sempre ser um par de inteiros com os respectivos atributos <disciplina,turma>.
   typedef
      std::map<std::pair<int,int>,
      std::vector/*vetor de dias*/<std::pair<int/*valor da variavel*/,
      std::pair<int/*id_unidade*/,int/*id_sala*/> > > > X___i_d_u_s_t;
   // <<<

   // >>>
   // A chave dever� sempre ser um vetor de tamanho 4 com os respectivos atributos <turma,disciplina,curso,campus>.
   typedef
      std::map<std::vector<int>,int>
      A___i_d_c_cp;
   // <<<

   ParVetor creditos_por_dia;
   ParInteiro alunos;

   // >>>
   X___i_d_u_s_t x;
   // <<<

   // >>>
   A___i_d_c_cp a;
   // <<<

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

         std::pair<int,int> key;

         // >>>
         std::vector<int> key_alunos;
         int id_disc;
         // <<<

         switch(v->getType()) {
         case Variable::V_ERROR:
            std::cout << "Vari�vel inv�lida " << std::endl;
            break;
         case Variable::V_CREDITOS:

            id_disc = v->getDisciplina()->getId();

            /*
            if(id_disc < 0) {
            id_disc = -id_disc;
            }
            */

            //key = std::make_pair(id_disc,v->getTurma());
            //key = std::make_pair(v->getDisciplina()->getId(),v->getTurma());
            //key = std::make_pair(v->getTurma(),v->getDisciplina()->getId());
            key = std::make_pair(v->getTurma(),id_disc);

            cout << "Oferta de " << v->getValue() << 
               // >>> 07/10/2010
               " creditos da disciplina " << v->getDisciplina()->codigo
               //" creditos da disciplina " << id_disc->codigo
               // <<< 07/10/2010
               << " para a turma " << v->getTurma()
               << " no dia " << v->getDia() << " e na sala " <<
               v->getSala()->getId() << std::endl;
            /*
            if(creditos_por_dia[key].size() == 0) 
            creditos_por_dia[key] = 
            std::vector<std::pair<int,int> >(7);
            creditos_por_dia[key][v->getDia()] = 
            std::make_pair((int)v->getValue(),v->getSala()->getId());
            */
            // >>>
            if(x[key].size() == 0) {
               //x[key] = std::vector<std::pair<int,std::pair<int,int>> >(7);
               x[key] = std::vector<std::pair<int,std::pair<int,int>> >
                  (7,std::make_pair(-1,std::make_pair(-1,-1)));
            }
            /**/
            // >>> 07/10/2010
            //if(x[key][v->getDia()].first >= 0) {
            if(x[key][v->getDia()].first > 0) {
               x[key][v->getDia()] = std::make_pair(
                  (x[key][v->getDia()].first + (int)v->getValue()),
                  std::make_pair(v->getUnidade()->getId(),v->getSala()->getId()));
            }
            //else {
            if(x[key][v->getDia()].first == -1) {
               x[key][v->getDia()] = std::make_pair(
                  (int)v->getValue(),
                  std::make_pair(v->getUnidade()->getId(),v->getSala()->getId()));
            }
            // <<< 07/10/2010
            /**/
            /*
            x[key][v->getDia()] = std::make_pair((int)v->getValue(),
            std::make_pair(v->getUnidade()->getId(),v->getSala()->getId()));
            */
            // <<<

#ifdef PRINT_CSV
            if(printLegend_V_CREDITOS){
               fprintf(f_V_CREDITOS,"Var. x,\t\ti,\td,\tu,\ts,\t\tt,\n");
               printLegend_V_CREDITOS = false;
            }
            // >>> 07/10/2010
            /*
            fprintf(f_V_CREDITOS,"%f,\t%d,\t%d,\t%d,\t%d,\t%d,\n",v->getValue(),v->getTurma(),v->getDisciplina()->getId(),
            v->getUnidade()->getId(),v->getSala()->getId(),v->getDia());
            */
            fprintf(f_V_CREDITOS,"%f,\t%d,\t%d,\t%d,\t%d,\t%d,\n",v->getValue(),v->getTurma(),id_disc,
               v->getUnidade()->getId(),v->getSala()->getId(),v->getDia());
            // <<< 07/10/2010
#endif

            break;
         case Variable::V_OFERECIMENTO: break;
         case Variable::V_ABERTURA: break;
         case Variable::V_ALUNOS:

            id_disc = v->getDisciplina()->getId();

            /*
            if(id_disc < 0) {
            id_disc = -id_disc;
            }
            */

            //key = std::make_pair(id_disc,v->getTurma());
            //key = std::make_pair(v->getDisciplina()->getId(),v->getTurma());
            //key = std::make_pair(v->getTurma(),v->getDisciplina()->getId());
            key = std::make_pair(v->getTurma(),id_disc);

            cout << "Oferecimento de " << v->getValue() << 
               // >>> 07/10/2010
               " vagas da disciplina " << v->getDisciplina()->codigo
               //" vagas da disciplina " << v->getDisciplina()->codigo
               // <<< 07/10/2010
               << " para a turma " << v->getTurma()
               << " do curso " << v->getCurso()->codigo << std::endl;

            //alunos[key] = (int) v->getValue();

            // >>> 07/10/2010
            //key_alunos.push_back(v->getDisciplina()->getId());
            //key_alunos.push_back(id_disc);
            // <<< 07/10/2010
            key_alunos.push_back(v->getTurma());

            key_alunos.push_back(id_disc);

            key_alunos.push_back(v->getCurso()->getId());
            key_alunos.push_back(v->getCampus()->getId());


            if(a.find(key_alunos) == a.end()) {
               a[key_alunos] = (int) v->getValue();
            }

            /*
            if(a.find(key_alunos) != a.end()) {
            a[key_alunos] += (int) v->getValue();
            }
            else {
            a[key_alunos] = (int) v->getValue();
            }
            */
            // <<<

#ifdef PRINT_CSV
            if(printLegend_V_ALUNOS){
               fprintf(f_V_ALUNOS,"Var. a,\t\ti,\td,\tc,\tcp,\n");
               printLegend_V_ALUNOS = false;
            }

            // >>> 07/10/2010
            /*
            fprintf(f_V_ALUNOS,"%f,\t%d,\t%d,\t%d,\t%d,\n",v->getValue(),v->getTurma(),
            v->getDisciplina()->getId(),v->getCurso()->getId(),v->getCampus()->getId());
            */
            fprintf(f_V_ALUNOS,"%f,\t%d,\t%d,\t%d,\t%d,\n",v->getValue(),v->getTurma(),
               id_disc,v->getCurso()->getId(),v->getCampus()->getId());
            // <<< 07/10/2010
#endif

            break;
         case Variable::V_ALOC_ALUNO: break;
         case Variable::V_N_SUBBLOCOS: break;
         case Variable::V_DIAS_CONSECUTIVOS: break;
         case Variable::V_MIN_CRED_SEMANA: break;
         case Variable::V_MAX_CRED_SEMANA: break;
         case Variable::V_ALOC_DISCIPLINA: break;
         case Variable::V_N_ABERT_TURMA_BLOCO: break;
         case Variable::V_SLACK_DIST_CRED_DIA_SUPERIOR: break;
         case Variable::V_SLACK_DIST_CRED_DIA_INFERIOR: break;
         }
      }
      vit++;
   }

   /* Depois do map montado, � poss�vel gerar os dados de sa�da */
   /* Como sugest�o, vou escrever uma rotina de impress�o */
   /*
   std::cout << 
   std::endl << "       RESUMO DA SOLUCAO       " << std::endl;
   for(ParVetor::iterator it = creditos_por_dia.begin();
   it != creditos_por_dia.end(); ++it) {
   std::cout << "..............................." << std::endl;
   std::cout << "Disciplina " << it->first.first << 
   ", turma " << it->first.second << ": " << std::endl;
   std::cout << "  Oferta: " << alunos[it->first] << " vagas" 
   << std::endl;
   for(int i=0;i<7;i++) {
   if(it->second[i].first > 0)
   std::cout << "   Dia " << i << ": " << it->second[i].first
   << " creditos, sala " << it->second[i].second
   << std::endl;
   }
   }
   */

   std::cout << 
      std::endl << "       RESUMO DA SOLUCAO       " << std::endl;
   for(X___i_d_u_s_t::iterator it = x.begin(); it != x.end(); ++it) {
      std::cout << "..............................." << std::endl;
      std::cout << "Disciplina " << it->first.second << 
         ", turma " << it->first.first << ": " << std::endl;

      std::vector<int> vt_key(4,-1);
      vt_key.at(0) = it->first.first ;
      vt_key.at(1) = it->first.second;

      ITERA_GGROUP(it_cp,problemData->campi,Campus) {
         ITERA_GGROUP(it_c,problemData->cursos,Curso) {
            vt_key.at(2) = it_c->getId();
            vt_key.at(3) = it_cp->getId() ;

            if(a.find(vt_key) != a.end()) {
               std::cout << "  Oferta: " << a[vt_key] << " vagas" 
                  << std::endl;
            }
         }
      }

      for(int i=0;i<7;i++) {
         if(it->second[i].first > 0)
            std::cout << "   Dia " << i << ": " << it->second[i].first
            << " creditos, sala " << it->second[i].second.second
            << std::endl;
      }
   }

   /*
   std::cout << "\nx\t -\t i\t d\t u\t s\t t" << std::endl;
   for(X___i_d_u_s_t::iterator it_x = x.begin(); it_x != x.end(); it_x++) {
   for(int i=0;i<7;i++) {
   std::cout << it_x->second[i].first << "\t\t" <<
   it_x->first.first << "\t" <<
   it_x->first.second << "\t" <<
   it_x->second[i].second.first << "\t" <<
   it_x->second[i].second.second << "\t" <<
   i << std::endl;
   }
   }

   std::cout << "\na\t -\t i\t d\t c\t cp" << std::endl;
   for(A___i_d_c_cp::iterator it_a = a.begin(); it_a != a.end(); it_a++) {
   std::cout << it_a->second << "\t\t" <<
   it_a->first.at(0) << "\t" <<
   it_a->first.at(1) << "\t" <<
   it_a->first.at(2) << "\t" <<
   it_a->first.at(3) << "\t" << std::endl;
   }
   */

   // Fill the solution

   // Coletando os ids dos campus, das unidades e das salas existentes  na entrada e suas respectivas descricoes.
   std::map<int,std::string> input_cp_desc;
   std::map<int,std::pair<std::string,int> > input_und_desc;
   std::map<int,std::pair<std::string,int> > input_sala_desc;

   ITERA_GGROUP(it_cp,problemData->campi,Campus) {
      input_cp_desc[it_cp->getId()] = it_cp->codigo;

      ITERA_GGROUP(it_und,it_cp->unidades,Unidade) {
         input_und_desc[it_und->getId()] = std::make_pair(it_und->codigo,it_und->id_campus);

         ITERA_GGROUP(it_sala,it_und->salas,Sala) {
            input_sala_desc[it_sala->getId()] = std::make_pair(it_sala->codigo,it_sala->id_unidade);
         }
      }
   }

   // Coletando todos os campus considerados para a saida.
   GGroup<int> ids_cp;

   for(A___i_d_c_cp::iterator it_a = a.begin(); it_a != a.end(); it_a++) {
      ids_cp.add(it_a->first.at(3));
   }

   // Coletando todas as unidades e salas consideradas para a saida.
   GGroup<int> ids_und;
   GGroup<int> ids_sala;

   for(X___i_d_u_s_t::iterator it_x = x.begin(); it_x != x.end(); it_x++) {
      for(unsigned dia = 0; dia < it_x->second.size(); dia++) {

         int idU = it_x->second.at(dia).second.first;
         int idS = it_x->second.at(dia).second.second;

         if( idU != -1 ) {
            ids_und.add(idU);
         }

         if( idS != -1 ) {
            ids_sala.add(idS);
         }
      }
   }

   // Adicionando os campus as classes de output
   for(GGroup<int>::iterator it_ids_cp = ids_cp.begin(); it_ids_cp != ids_cp.end(); it_ids_cp++) {
      int id = *it_ids_cp;
      problemSolution->addCampus(id,input_cp_desc[id]);
   }

   GGroup<AtendimentoCampus*>::iterator at_campus = problemSolution->atendimento_campus.begin();

   // Adicionando as unidades as classes de output
   for(GGroup<int>::iterator it_ids_und = ids_und.begin(); it_ids_und != ids_und.end(); it_ids_und++) {
      int id = *it_ids_und;
      at_campus->addUnidade(id,input_und_desc[id].first,input_und_desc[id].second);
   }

   GGroup<AtendimentoUnidade*>::iterator at_unidade = at_campus->atendimentos_unidades.begin();

   // Adicionando as salas as classes de output
   for(GGroup<int>::iterator it_ids_sala = ids_sala.begin(); it_ids_sala != ids_sala.end(); it_ids_sala++) {
      int id = *it_ids_sala;
      //problemSolution->atendimento_campus.begin()->atendimentos_unidades.begin()->addSala(id,input_sala_desc[id].first,input_sala_desc[id].second);
      at_unidade->addSala(id,input_sala_desc[id].first,input_sala_desc[id].second);
   }

   // >>>

   // ToDo: Fazer o msm que fiz aqui em baixo pro cp e und acima. nao sei se cp vai precisar

   // Adicionando os dias da semana as classes de output
   for(X___i_d_u_s_t::iterator it_x = x.begin(); it_x != x.end(); it_x++) {
      for(unsigned dia = 0; dia < it_x->second.size(); dia++) {

         //GGroup<AtendimentoSala*>::iterator it_at_sala = at_unidade->atendimentos_salas.end();

         // Procurando pela sala certa para adicionar o dia da semana
         ITERA_GGROUP(it_at_cp,problemSolution->atendimento_campus,AtendimentoCampus){
            ITERA_GGROUP(it_at_und,it_at_cp->atendimentos_unidades,AtendimentoUnidade){
               ITERA_GGROUP(it_at_sala,it_at_und->atendimentos_salas,AtendimentoSala){
                  if(it_at_sala->getId() == it_x->second.at(dia).second.second ) {

                     // poderia ser outro elemento de x, tendo em vista que todos os elementos <.second> de uma variavel x tem o mesmo tamanho.
                     int var_value = it_x->second.at(dia).first;

                     if( var_value != -1) {
                        it_at_sala->addDiaSemana(dia,"",it_x->second.at(dia).second.second);
                     }
                  }
               }
            }
         }
      }
   }

   // <<<

   std::map<int,Campus*> und_cp;

   // Listando as unidades dos campus.
   ITERA_GGROUP(it_cp,problemData->campi,Campus) {
      ITERA_GGROUP(it_und,it_cp->unidades,Unidade) {
         und_cp[it_und->getId()] = *it_cp;
      }
   }

   std::map<std::vector<int/*<d,c,cp,u,s,t>*/>,
      std::pair<GGroup<int/*i*/>,
      // >>> 27/10/2010
      //std::pair<int/*valor var x*/,int/*valor var a*/> > > solucao;
      std::pair<std::pair<int/*valor var x - c. teorico*/,int/*valor var x - pratico*/>,
      int/*valor var a*/> > > solucao;
   // <<< 27/10/2010

   for(X___i_d_u_s_t::iterator it_x = x.begin(); it_x != x.end(); it_x++) {
      for(int dia = 0; dia < 7; dia++){

         int value_x = it_x->second.at(dia).first;

         if(value_x >= 0) {
            /*
            std::cout << it_x->second[dia].first << "\t\t" <<
            it_x->first.first << "\t" <<
            it_x->first.second << "\t" <<
            it_x->second[dia].second.first << "\t" <<
            it_x->second[dia].second.second << "\t" <<
            dia << std::endl;
            */

            std::vector<int/*<d,c,cp,u,s,t>*/> chave(6);

            // >>> 27/10/2010
            // chave.at(0) = it_x->first.second;

            bool disc_teo = true;
            if(it_x->first.second < 0) {
               chave.at(0) = -it_x->first.second;
               disc_teo = false;
            }
            else {
               chave.at(0) = it_x->first.second;
            }
            // <<< 27/10/2010

            chave.at(1) = -1;
            for(A___i_d_c_cp::iterator it_a = a.begin(); it_a != a.end(); it_a++){
               if(it_a->first.at(0) == it_x->first.first &&
                  it_a->first.at(1) == it_x->first.second) {
                     chave.at(1) = it_a->first.at(2);
               }
            }

            chave.at(2) = und_cp.find(it_x->second.at(dia).second.first)->second->getId();
            chave.at(3) = it_x->second.at(dia).second.first;
            chave.at(4) = it_x->second.at(dia).second.second;
            chave.at(5) = dia;

            std::map<std::vector<int/*<d,c,cp,u,s,t>*/>,
               std::pair<GGroup<int/*i*/>,

               // >>> 27/10/2010
               //std::pair<int/*valor var x*/,int/*valor var a*/> > >::iterator 

               std::pair<std::pair<int/*valor var x - c. teorico*/,int/*valor var x - pratico*/>,
               int/*valor var a*/> > >::iterator 
               // <<< 27/10/2010

               it_solucao = solucao.find(chave);

            // >>> 27/10/2010

            // Adicionando a turma
            solucao[chave].first.add(it_x->first.first);

            // <<< 27/10/2010

            if( it_solucao == solucao.end() ){
               // Adicionando a turma
               //solucao[chave].first.add(it_x->first.first);

               // >>> 27/10/2010

               //solucao[chave].second = 
               //std::pair<int/*valor var x*/,int/*valor var a*/>(it_x->second.at(dia).first,0);

               if(disc_teo) {
                  solucao[chave].second = 
                     std::pair<std::pair<int/*valor var x - c. teorico*/,int/*valor var x - pratico*/>,
                     int/*valor var a*/>
                     (std::make_pair<int,int>(value_x,0),0);
               }
               else {
                  solucao[chave].second = 
                     std::pair<std::pair<int/*valor var x - c. teorico*/,int/*valor var x - pratico*/>,
                     int/*valor var a*/>
                     (std::make_pair<int,int>(0,value_x),0);
               }

               // <<< 27/10/2010

            }
            else {
               // >>> 27/10/2010

               // Adicionando a turma
               //solucao[chave].first.add(it_x->first.first);

               // Adicionando o valor de x
               //solucao[chave].second.first += value_x;

               if(disc_teo) {
                  solucao[chave].second.first.first += value_x;
               }
               else {
                  solucao[chave].second.first.second += value_x;
               }

               // <<< 27/10/2010
            }
         }
      }
   }

   std::map<std::vector<int/*<d,c,cp,u,s,t>*/>,
      std::pair<GGroup<int/*i*/>,

      // >>> 27/10/2010
      //std::pair<int/*valor var x*/,int/*valor var a*/> > >::iterator 

      std::pair<std::pair<int/*valor var x - c. teorico*/,int/*valor var x - pratico*/>,
      int/*valor var a*/> > >::iterator 
      // <<< 27/10/2010

      it_solucao = solucao.begin();

   /* Para cada solucao registrada, procurar por todas vars. do tipo a que possuam 
   o(s) par(es) (i,d) ou (-i,d) formados a partir da solu��o em quest�o.*/
   for(; it_solucao != solucao.end(); it_solucao++) {
      GGroup<int/*i*/>::iterator it_turma = 
         it_solucao->second.first.begin();

      // >>> 27/10/2010

      //int disc = it_solucao->first.at(0);
      int disc = (it_solucao->second.second.first.first > 0 ? 
         it_solucao->first.at(0) : (-it_solucao->first.at(0)));

      // <<< 27/10/2010

      for(; it_turma != it_solucao->second.first.end(); it_turma++) {
         int turma = *it_turma;

         for(A___i_d_c_cp::iterator it_a = a.begin(); it_a != a.end(); it_a++){
            if(it_a->first.at(0) == turma && it_a->first.at(1) == disc) {
               it_solucao->second.second.second += it_a->second;
            }
         }
      }
   }

   //Listando as disciplinas de acordo com o id.
   std::map<int/*id_disc*/,Disciplina*> discs;
   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
      discs[it_disc->getId()] = *it_disc;
   }

   // Procurando a posi��o correta para inserir
   ITERA_GGROUP(it_at_cp,problemSolution->atendimento_campus,AtendimentoCampus) {
      int id_at_cp = it_at_cp->getId();

      ITERA_GGROUP(it_at_und,it_at_cp->atendimentos_unidades,AtendimentoUnidade) {
         int id_at_und = it_at_und->getId();

         ITERA_GGROUP(it_at_sala,it_at_und->atendimentos_salas,AtendimentoSala) {
            int id_at_sala = it_at_sala->getId();

            ITERA_GGROUP(it_at_dia_sem,it_at_sala->atendimentos_dias_semana,AtendimentoDiaSemana) {
               int id_at_dia_sem = it_at_dia_sem->key.second;

               it_solucao = solucao.begin();

               //Registrando a solu��o no lugar certo.
               for(; it_solucao != solucao.end(); it_solucao++) {

                  if( it_solucao->first.at(2) == id_at_cp &&
                     it_solucao->first.at(3) == id_at_und &&
                     it_solucao->first.at(4) == id_at_sala &&
                     it_solucao->first.at(5) == id_at_dia_sem ) {

                        AtendimentoOferta * at_oferta = new AtendimentoOferta();

                        // Procurando o id de <ofertaCursoCampus>
                        ITERA_GGROUP(it_oferta,problemData->ofertas,Oferta) {
                           if( (it_oferta->campus_id == id_at_cp) &&
                              (it_oferta->curso_id == it_solucao->first.at(1))) {

                                 std::stringstream aux;
                                 aux << it_oferta->getId();

                                 at_oferta->oferta_curso_campi_id = aux.str();

                                 break;
                           }
                        }

                        std::stringstream aux;

                        aux << it_solucao->first.at(0);
                        at_oferta->disciplina_id = aux.str();
                        at_oferta->quantidade = it_solucao->second.second.second;

                        AtendimentoTatico * at_tatico = new AtendimentoTatico();

                        at_tatico->atendimento_oferta = at_oferta;

                        at_tatico->qtde_creditos_teoricos = it_solucao->second.second.first.first;

                        at_tatico->qtde_creditos_praticos = it_solucao->second.second.first.second;

                        it_at_dia_sem->atendimentos_tatico.add(at_tatico);
                  }
               }
            }
         }
      }
   }

#ifdef DEBUG
   if ( fout )
      fclose(fout);
#endif

#ifdef PRINT_CSV
   if (f_V_CREDITOS)
      fclose(f_V_CREDITOS);

   if(f_V_ALUNOS)
      fclose(f_V_ALUNOS);
#endif

   if ( xSol )
      delete[] xSol;
}

int SolverMIP::cria_variaveis()
{
   int num_vars = 0;

#ifdef PRINT_cria_variaveis
   int numVarsAnterior = 0;
#endif

   num_vars += cria_variavel_oferecimentos(); // o

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"o\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_creditos(); // x

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"x\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_abertura(); // z

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"z\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_alunos(); // a 

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"a\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_consecutivos(); // c

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"c\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_max_creds(); // H

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"H\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_min_creds(); // h

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"h\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_aloc_disciplina(); // y

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"y\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_num_subblocos(); // w

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"w\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_num_abertura_turma_bloco(); // v

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"v\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_aloc_alunos(); // b

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"b\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_de_folga_dist_cred_dia_superior(); // fcp

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"fcp\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_de_folga_dist_cred_dia_inferior(); // fcm

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"fcm\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_abertura_subbloco_de_blc_dia_campus(); // r

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"r\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   return num_vars;
}

int SolverMIP::cria_variavel_creditos(void)
{
   int num_vars = 0;

   ITERA_GGROUP(itCampus,problemData->campi,Campus)
   {
      ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
      {
         ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
         {
            ITERA_GGROUP(itDisc,itCjtSala->getDiscsAssociadas(),Disciplina)
            {
               for(int turma=0;turma<itDisc->num_turmas;turma++)
               {
                  for(int dia=0;dia<7;dia++)
                  {
                     Variable v;
                     v.reset();
                     v.setType(Variable::V_CREDITOS);

                     v.setTurma(turma);            // i
                     v.setDisciplina(*itDisc);     // d
                     v.setUnidade(*itUnidade);     // u
                     v.setSubCjtSala(*itCjtSala);  // tps  
                     v.setDia(dia);                // t

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
   }

   // >>>

   // OLD !!

   //ITERA_GGROUP(it_campus,problemData->campi,Campus)
   //{
   //   ITERA_GGROUP(it_unidades,it_campus->unidades,Unidade)
   //   {
   //      ITERA_GGROUP(it_salas,it_unidades->salas,Sala)
   //      {
   //         ITERA_GGROUP(it_disc,it_salas->disciplinasAssociadas,Disciplina)
   //         {
   //            for(int turma=0;turma<it_disc->num_turmas;turma++)
   //            {
   //               for(int dia=0;dia<7;dia++)
   //               {
   //                  Variable v;
   //                  v.reset();
   //                  v.setType(Variable::V_CREDITOS);

   //                  v.setTurma(turma);            // i
   //                  v.setDisciplina(*it_disc);    // d
   //                  v.setUnidade(*it_unidades);   // u
   //                  v.setSala(*it_salas);         // s  
   //                  v.setDia(dia);                // t

   //                  if (vHash.find(v) == vHash.end())
   //                  {
   //                     vHash[v] = lp->getNumCols();

   //                     OPT_COL col(OPT_COL::VAR_INTEGRAL,0.0,0.0,24.0,
   //                        (char*)v.toString().c_str());

   //                     lp->newCol(col);

   //                     num_vars += 1;
   //                  }
   //               }
   //            }
   //         }
   //      }
   //   }
   //}

   // <<<

   return num_vars;
}

int SolverMIP::cria_variavel_oferecimentos(void)
{
   int num_vars = 0;

   ITERA_GGROUP(itCampus,problemData->campi,Campus)
   {
      ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
      {
         ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
         {
            ITERA_GGROUP(itDisc,itCjtSala->getDiscsAssociadas(),Disciplina)
            {
               for(int turma=0;turma<itDisc->num_turmas;turma++)
               {
                  for(int dia=0;dia<7;dia++)
                  {
                     Variable v;
                     v.reset();
                     v.setType(Variable::V_OFERECIMENTO);

                     v.setTurma(turma);            // i
                     v.setDisciplina(*itDisc);     // d
                     v.setUnidade(*itUnidade);     // u
                     v.setSubCjtSala(*itCjtSala);  // tps  
                     v.setDia(dia);                // t

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
   }

   // >>>

   // OLD !!

   //ITERA_GGROUP(it_campus,problemData->campi,Campus)
   //{
   //   ITERA_GGROUP(it_unidades,it_campus->unidades,Unidade)
   //   {
   //      ITERA_GGROUP(it_salas,it_unidades->salas,Sala) 
   //      {
   //         ITERA_GGROUP(it_disc,it_salas->disciplinasAssociadas,Disciplina)
   //         {
   //            for(int turma=0;turma<it_disc->num_turmas;turma++)
   //            {
   //               for(int dia=0;dia<7;dia++)
   //               {
   //                  Variable v;
   //                  v.reset();
   //                  v.setType(Variable::V_OFERECIMENTO);

   //                  v.setTurma(turma);            // i
   //                  v.setDisciplina(*it_disc);    // d
   //                  v.setUnidade(*it_unidades);   // u
   //                  v.setSala(*it_salas);         // s  
   //                  v.setDia(dia);                // t

   //                  if (vHash.find(v) == vHash.end())
   //                  {
   //                     vHash[v] = lp->getNumCols();

   //                     OPT_COL col(OPT_COL::VAR_BINARY,0.0,0.0,1.0,
   //                        (char*)v.toString().c_str());

   //                     lp->newCol(col);

   //                     num_vars += 1;
   //                  }
   //               }
   //            }
   //         }
   //      }
   //   }
   //}

   // <<<

   return num_vars;
}

int SolverMIP::cria_variavel_abertura(void)
{
   int num_vars = 0;

   /*
   Pode ser implementado de uma maneira melhor, listando apenas as disciplinas que podem
   ser abertas em um campus (atraves do OFERTACURSO) e criando as suas respectivas variaveis.
   Desse modo, variaveis desnecess�rias (relacionadas � disciplinas que n�o existem em outros campus)
   seriam evitadas.

   VER <demandas_campus> em <ProblemData>
   */

   ITERA_GGROUP(it_campus,problemData->campi,Campus)
   {
      ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
      {
         for(int turma=0;turma<it_disc->num_turmas;turma++)
         {
            Variable v;
            v.reset();
            v.setType(Variable::V_ABERTURA);

            v.setTurma(turma);            // i
            v.setDisciplina(*it_disc);    // d
            v.setCampus(*it_campus);	  // cp

            std::pair<int,int> dc = std::make_pair
               (it_disc->getId(),it_campus->getId());


            if(problemData->demandas_campus.find(dc) ==	problemData->demandas_campus.end())
            {
               problemData->demandas_campus[dc] = 0;
            }

            //double ratioDem = ( it_disc->demanda_total - 
            double ratioDem = ( it_disc->getDemandaTotal() - 
               problemData->demandas_campus[dc] ) 
               /// (1.0 * it_disc->demanda_total);
               / (1.0 * it_disc->getDemandaTotal());

            double coeff = alpha + gamma * ratioDem;

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

   ITERA_GGROUP(itOferta,problemData->ofertas,Oferta)
   {
      GGroup<DisciplinaPeriodo>::iterator itPrdDisc = 
         itOferta->curriculo->disciplinas_periodo.begin();

      for(; itPrdDisc != itOferta->curriculo->disciplinas_periodo.end(); itPrdDisc++)
      {
         Disciplina * ptDisc = problemData->refDisciplinas[(*itPrdDisc).second];

         for(int turma = 0; turma < ptDisc->num_turmas; turma++)
         {
            Variable v;
            v.reset();
            v.setType(Variable::V_ALUNOS);

            v.setTurma(turma);               // i
            v.setDisciplina(ptDisc);         // d
            v.setOferta(*itOferta);          // o

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

   //ITERA_GGROUP(it_campus,problemData->campi,Campus)
   //{
   //   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
   //   {
   //      for(int turma=0;turma<it_disc->num_turmas;turma++)
   //      {
   //         ITERA_GGROUP(it_cursos,problemData->cursos,Curso)
   //         {
   //            Variable v;
   //            v.reset();
   //            v.setType(Variable::V_ALUNOS);

   //            v.setTurma(turma);            // i
   //            v.setDisciplina(*it_disc);    // d
   //            v.setCampus(*it_campus);	  // cp
   //            v.setCurso(*it_cursos);       // c

   //            if (vHash.find(v) == vHash.end())
   //            {
   //               vHash[v] = lp->getNumCols();

   //               OPT_COL col(OPT_COL::VAR_INTEGRAL,0.0,0.0,1000.0,
   //                  (char*)v.toString().c_str());

   //               lp->newCol(col);

   //               num_vars += 1;
   //            }

   //         }
   //      }
   //   }
   //}

   return num_vars;
}

int SolverMIP::cria_variavel_aloc_alunos(void)
{
   int num_vars = 0;

      ITERA_GGROUP(itOferta,problemData->ofertas,Oferta)
   {
      GGroup<DisciplinaPeriodo>::iterator itPrdDisc = 
         itOferta->curriculo->disciplinas_periodo.begin();

      for(; itPrdDisc != itOferta->curriculo->disciplinas_periodo.end(); itPrdDisc++)
      {
         Disciplina * ptDisc = problemData->refDisciplinas[(*itPrdDisc).second];

         for(int turma = 0; turma < ptDisc->num_turmas; turma++)
         {
            Variable v;
            v.reset();
            v.setType(Variable::V_ALOC_ALUNO);

            v.setTurma(turma);               // i
            v.setDisciplina(ptDisc);         // d
            v.setOferta(*itOferta);          // o

            if (vHash.find(v) == vHash.end())
            {
               vHash[v] = lp->getNumCols();

               OPT_COL col(OPT_COL::VAR_BINARY,0,0.0,1.0,
                  (char*)v.toString().c_str());

               lp->newCol(col);

               num_vars += 1;
            }
         }
      }
   }

   //ITERA_GGROUP(it_campus,problemData->campi,Campus)
   //{
   //   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
   //   {
   //      for(int turma=0;turma<it_disc->num_turmas;turma++)
   //      {
   //         ITERA_GGROUP(it_cursos,problemData->cursos,Curso)
   //         {
   //            Variable v;
   //            v.reset();
   //            v.setType(Variable::V_ALOC_ALUNO);

   //            v.setTurma(turma);            // i
   //            v.setDisciplina(*it_disc);    // d
   //            v.setCampus(*it_campus);		 // cp
   //            v.setCurso(*it_cursos);       // c

   //            if (vHash.find(v) == vHash.end())
   //            {
   //               vHash[v] = lp->getNumCols();

   //               OPT_COL col(OPT_COL::VAR_BINARY,0,0.0,1.0,
   //                  (char*)v.toString().c_str());

   //               lp->newCol(col);

   //               num_vars += 1;
   //            }
   //         }

   //      }
   //   }
   //}

   return num_vars;

}

int SolverMIP::cria_variavel_consecutivos(void)
{
   int num_vars = 0;

   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
   {
      for(int turma=0;turma<it_disc->num_turmas;turma++)
      {
         for(int dia=1;dia<7;dia++) 
         {
            Variable v;
            v.reset();
            v.setType(Variable::V_DIAS_CONSECUTIVOS);

            v.setTurma(turma);            // i
            v.setDisciplina(*it_disc);    // d
            v.setDia(dia);                // t

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
int SolverMIP::cria_variavel_min_creds(void)
{
   int num_vars = 0;
   ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular)
   {
      ITERA_GGROUP(it_disc,it_bloco->disciplinas,Disciplina)
      {
         for(int i=0;i<it_disc->num_turmas;i++)
         {
            Variable v;
            v.reset();
            v.setType(Variable::V_MIN_CRED_SEMANA);
            v.setTurma(i);
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
         for(int i=0;i<it_disc->num_turmas;i++)
         {
            Variable v;
            v.reset();
            v.setType(Variable::V_MAX_CRED_SEMANA);
            v.setTurma(i);
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

int SolverMIP::cria_variavel_aloc_disciplina(void)
{
   int num_vars = 0;

   ITERA_GGROUP(itCampus,problemData->campi,Campus)
   {
      ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
      {
         ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala) 
         {
            ITERA_GGROUP(itDisc,itCjtSala->getDiscsAssociadas(),Disciplina)
            {
               for(int turma=0;turma<itDisc->num_turmas;turma++)
               {
                  Variable v;
                  v.reset();
                  v.setType(Variable::V_ALOC_DISCIPLINA);

                  v.setUnidade(*itUnidade);     // u
                  v.setSubCjtSala(*itCjtSala);  // tps
                  v.setTurma(turma);            // i
                  v.setDisciplina(*itDisc);     // d

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

   // >>>

   // OLD

   //ITERA_GGROUP(it_campus,problemData->campi,Campus)
   //{
   //   ITERA_GGROUP(it_unidades,it_campus->unidades,Unidade)
   //   {
   //      ITERA_GGROUP(it_salas,it_unidades->salas,Sala) 
   //      {
   //         ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
   //         {
   //            for(int turma=0;turma<it_disc->num_turmas;turma++)
   //            {
   //               Variable v;
   //               v.reset();
   //               v.setType(Variable::V_ALOC_DISCIPLINA);

   //               v.setUnidade(*it_unidades);   // u
   //               v.setSala(*it_salas);         // s  
   //               v.setTurma(turma);            // i
   //               v.setDisciplina(*it_disc);    // d

   //               if (vHash.find(v) == vHash.end())
   //               {
   //                  vHash[v] = lp->getNumCols();

   //                  OPT_COL col(OPT_COL::VAR_BINARY,0.0,0.0,1.0,
   //                     (char*)v.toString().c_str());

   //                  lp->newCol(col);

   //                  num_vars += 1;
   //               }

   //            }
   //         }
   //      }
   //   }
   //}

   // <<<

   return num_vars;
}

int SolverMIP::cria_variavel_num_subblocos(void)
{
   int num_vars = 0;

   ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular)
   {
      for(int dia=0;dia<7;dia++)
      {
         Variable v;
         v.reset();

         v.setType(Variable::V_N_SUBBLOCOS);
         v.setBloco(*it_bloco);
         v.setDia(dia);
         v.setCampus(it_bloco->campus);

         if (vHash.find(v) == vHash.end())
         {
            vHash[v] = lp->getNumCols();

            OPT_COL col(OPT_COL::VAR_INTEGRAL,rho,0.0,4.0,
               (char*)v.toString().c_str());

            lp->newCol(col);

            num_vars += 1;
         }
      }
   }

   return num_vars;
}


int SolverMIP::cria_variavel_num_abertura_turma_bloco(void)
{
   int num_vars = 0;

   ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular)
   {
      for(int dia=0;dia<7;dia++)
      {
         Variable v;
         v.reset();

         v.setType(Variable::V_N_ABERT_TURMA_BLOCO);
         v.setBloco(*it_bloco);
         v.setDia(dia);

         if (vHash.find(v) == vHash.end())
         {
            vHash[v] = lp->getNumCols();

            OPT_COL col(OPT_COL::VAR_BINARY,beta,0.0,1.0,
               (char*)v.toString().c_str());

            lp->newCol(col);

            num_vars += 1;
         }

      }
   }

   return num_vars;
}

int SolverMIP::cria_variavel_de_folga_dist_cred_dia_superior(void)
{
   int num_vars = 0;

   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
      for(int dia=0;dia<7;dia++) {
         Variable v;
         v.reset();

         v.setType(Variable::V_SLACK_DIST_CRED_DIA_SUPERIOR);
         v.setDisciplina(*it_disc);
         v.setDia(dia);

         if (vHash.find(v) == vHash.end())
         {
            vHash[v] = lp->getNumCols();

            int cred_disc_dia = it_disc->min_creds;

            if( it_disc->divisao_creditos != NULL )
            {
               cred_disc_dia = it_disc->divisao_creditos->dia[dia];
            }

            //OPT_COL col(OPT_COL::VAR_INTEGRAL,epsilon,0.0, cred_disc_dia, (char*)v.toString().c_str());
            OPT_COL col(OPT_COL::VAR_INTEGRAL,epsilon,0.0, OPT_INF, (char*)v.toString().c_str());

            lp->newCol(col);

            num_vars += 1;
         }

      }
   }

   return num_vars;
}

int SolverMIP::cria_variavel_de_folga_dist_cred_dia_inferior(void)
{
   int num_vars = 0;

   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
      for(int dia=0;dia<7;dia++) {
         Variable v;
         v.reset();

         v.setType(Variable::V_SLACK_DIST_CRED_DIA_INFERIOR);
         v.setDisciplina(*it_disc);
         v.setDia(dia);

         if (vHash.find(v) == vHash.end())
         {
            vHash[v] = lp->getNumCols();

            int cred_disc_dia = it_disc->max_creds;

            if( it_disc->divisao_creditos != NULL )
            {
               cred_disc_dia = it_disc->divisao_creditos->dia[dia];
            }

            //OPT_COL col(OPT_COL::VAR_INTEGRAL,epsilon,0.0, cred_disc_dia, (char*)v.toString().c_str());
            OPT_COL col(OPT_COL::VAR_INTEGRAL,epsilon,0.0, OPT_INF, (char*)v.toString().c_str());

            lp->newCol(col);

            num_vars += 1;
         }

      }
   }

   return num_vars;
}

int SolverMIP::cria_variavel_abertura_subbloco_de_blc_dia_campus()
{
   int num_vars = 0;

   ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular)
   {
      for(int dia=0;dia<7;dia++)
      {
         Variable v;
         v.reset();

         v.setType(Variable::V_ABERTURA_SUBBLOCO_DE_BLC_DIA_CAMPUS);
         v.setBloco(*it_bloco);
         v.setDia(dia);
         v.setCampus(it_bloco->campus);

         if (vHash.find(v) == vHash.end())
         {
            vHash[v] = lp->getNumCols();

            OPT_COL col(OPT_COL::VAR_BINARY,0,0.0,1.0,
               (char*)v.toString().c_str());

            lp->newCol(col);

            num_vars += 1;
         }
      }
   }

   return num_vars;
}

// ==============================================================
//							CONSTRAINTS
// ==============================================================

int SolverMIP::cria_restricoes(void)
{
   int restricoes = 0;

#ifdef PRINT_cria_restricoes
   int numRestAnterior = 0;
#endif

   restricoes += cria_restricao_carga_horaria();				// Restricao 1.2.3

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.3\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_max_cred_sd();					// Restricao 1.2.4

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.4\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_min_cred_dd();					// Restricao 1.2.5

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.5\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_ativacao_var_o();					// Restricao 1.2.6

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.6\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_evita_sobreposicao();			// Restricao 1.2.7

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.7\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_disciplina_sala();				// Restricao 1.2.8

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.8\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_turma_sala();					// Restricao 1.2.9

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.9\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_evita_turma_disc_camp_d();		// Restricao 1.2.10

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.10\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_turmas_bloco();				// Restricao 1.2.11

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.11\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_max_cred_disc_bloco();			// Restricao 1.2.12

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.12\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_num_tur_bloc_dia_difunid();	// Restricao 1.2.13

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.13\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_lim_cred_diar_disc();			// Restricao 1.2.14

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.14\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_cap_aloc_dem_disc();			// Restricao 1.2.15

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.15\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_cap_sala_compativel_turma();	// Restricao 1.2.16

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.16\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_cap_sala_unidade();			// Restricao 1.2.17

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.17\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_turma_disc_dias_consec();		// Restricao 1.2.18

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.18\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_min_creds_turm_bloco();		// Restricao 1.2.19

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.19\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_max_creds_turm_bloco();		// Restricao 1.2.20

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.20\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_aluno_curso_disc();			// Restricao 1.2.21

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.21\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_alunos_cursos_dif();			// Restricao 1.2.22

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.22\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_de_folga_dist_cred_dia();		// Restricao 1.2.23

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.23\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_ativacao_var_r();						// Restricao 1.2.24

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.24\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   std::cout << "SAINDO !!!\n";
   exit(1);

   return restricoes;
}

int SolverMIP::cria_restricao_carga_horaria(void)
{
   int restricoes = 0;

   char name[100];
   int nnz;

   Constraint c;

   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(itCampus,problemData->campi,Campus)
   {
      ITERA_GGROUP(itDisciplina,problemData->disciplinas,Disciplina)
      {
         for(int turma = 0; turma < itDisciplina->num_turmas; turma++)
         {
            c.reset();
            c.setType(Constraint::C_CARGA_HORARIA);

            c.setCampus(*itCampus);
            c.setTurma(turma);
            c.setDisciplina(*itDisciplina);

            sprintf( name, "%s", c.toString().c_str() );

            if (cHash.find(c) != cHash.end()) continue;

            cHash[ c ] = lp->getNumRows();

            nnz = itCampus->totalSalas * 7;

            OPT_ROW row( nnz + 1, OPT_ROW::EQUAL , 0 , name );

            v.reset();
            v.setType(Variable::V_CREDITOS);

            // ---

            ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
            {
               ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
               {
                  for(int dia = 0; dia < 7; dia++)
                  {
                     v.setTurma(turma);
                     v.setDisciplina(*itDisciplina);
                     v.setUnidade(*itUnidade);
                     v.setSubCjtSala(*itCjtSala);
                     v.setDia(dia);                     

                     it_v = vHash.find(v);
                     if( it_v != vHash.end() )
                     { row.insert(it_v->second, 1.0); }
                  }
               }
            }

            // ---

            v.reset();
            v.setType(Variable::V_ABERTURA);

            v.setCampus(*itCampus);
            v.setDisciplina(*itDisciplina);
            v.setTurma(turma);

            it_v = vHash.find(v);
            if( it_v != vHash.end() )
            {
               row.insert(it_v->second,
                  -(itDisciplina->cred_praticos + 
                  itDisciplina->cred_teoricos));
            }

            // ---

            lp->addRow(row);
            restricoes++;
         }
      }
   }


   // >>>

   // OLD

   //ITERA_GGROUP(it_campus,problemData->campi,Campus) {
   //   ITERA_GGROUP(it_disciplina,problemData->disciplinas,Disciplina) {
   //      for(int i=0;i<it_disciplina->num_turmas;i++) {

   //         c.reset();
   //         c.setType(Constraint::C_CARGA_HORARIA);

   //         c.setCampus(*it_campus);
   //         c.setTurma(i);
   //         c.setDisciplina(*it_disciplina);

   //         sprintf( name, "%s", c.toString().c_str() );

   //         if (cHash.find(c) != cHash.end()) continue;

   //         cHash[ c ] = lp->getNumRows();

   //         nnz = it_campus->totalSalas * 7;

   //         OPT_ROW row( nnz + 1, OPT_ROW::EQUAL , 0 , name );

   //         v.reset();
   //         v.setType(Variable::V_CREDITOS);

   //         // ---

   //         ITERA_GGROUP(it_unidade,it_campus->unidades,Unidade) {
   //            ITERA_GGROUP(it_sala,it_unidade->salas,Sala) {
   //               for(int dia=0;dia<7;dia++) {
   //                  v.setUnidade(*it_unidade);
   //                  v.setSala(*it_sala);

   //                  v.setTurma(i);
   //                  v.setDisciplina(*it_disciplina);

   //                  v.setDia(dia);

   //                  it_v = vHash.find(v);
   //                  if( it_v != vHash.end() ){
   //                     row.insert(it_v->second, 1.0);

   //                  }
   //               }
   //            }
   //         }

   //         // ---

   //         v.reset();
   //         v.setType(Variable::V_ABERTURA);

   //         v.setCampus(*it_campus);
   //         v.setTurma(i);
   //         v.setDisciplina(*it_disciplina);

   //         it_v = vHash.find(v);
   //         if( it_v != vHash.end() ) {
   //            row.insert(it_v->second, 
   //               -(it_disciplina->cred_praticos + 
   //               it_disciplina->cred_teoricos));
   //         }

   //         // ---

   //         lp->addRow(row);
   //         restricoes++;
   //      }
   //   }
   //}

   // <<<

   return restricoes;
}

int SolverMIP::cria_restricao_max_cred_sd(void)
{
   int restricoes = 0;

   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(itCampus,problemData->campi,Campus)
   {
      ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
      {
         ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
         {
            for(int dia = 0; dia < 7; dia++)
            {
               c.reset();
               c.setType(Constraint::C_MAX_CREDITOS_SD);

               c.setUnidade(*itUnidade);
               c.setSubCjtSala(*itCjtSala);
               c.setDia(dia);

               sprintf( name, "%s", c.toString().c_str() );

               if (cHash.find(c) != cHash.end()) continue;

               cHash[ c ] = lp->getNumRows();

               nnz = 0;
               ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
               { nnz += itDisc->num_turmas; }

               int maxCreds = itCjtSala->maxCreds(dia);

               OPT_ROW row( nnz, OPT_ROW::LESS , maxCreds, name );

               ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
               {
                  for(int turma = 0; turma < itDisc->num_turmas; turma++)
                  {
                     v.reset();
                     v.setType(Variable::V_CREDITOS);

                     v.setTurma(turma);
                     v.setUnidade(*itUnidade);
                     v.setDisciplina(*itDisc);
                     v.setSubCjtSala(*itCjtSala);
                     v.setDia(dia);

                     it_v = vHash.find(v);
                     if( it_v != vHash.end() )
                     { row.insert(it_v->second, 1.0); }
                  }
               }

               lp->addRow(row);
               restricoes++;
            }
         }
      }
   }

   // >>>

   // OLD

   //ITERA_GGROUP(it_campus,problemData->campi,Campus) {

   //   ITERA_GGROUP(it_unidade,it_campus->unidades,Unidade) {
   //      ITERA_GGROUP(it_sala,it_unidade->salas,Sala) {
   //         for(int dia=0;dia<7;dia++) {
   //            c.reset();
   //            c.setType(Constraint::C_MAX_CREDITOS_SD);

   //            c.setUnidade(*it_unidade);
   //            c.setSala(*it_sala);
   //            c.setDia(dia);

   //            sprintf( name, "%s", c.toString().c_str() );

   //            if (cHash.find(c) != cHash.end()) continue;

   //            cHash[ c ] = lp->getNumRows();

   //            nnz = 0;
   //            ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
   //               nnz += it_disc->num_turmas;
   //            }

   //            int max_creds = it_sala->max_creds(dia);

   //            OPT_ROW row( nnz, OPT_ROW::LESS , max_creds, name );

   //            ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
   //               for(int i=0;i<it_disc->num_turmas;i++) {

   //                  v.reset();
   //                  v.setType(Variable::V_CREDITOS);

   //                  v.setUnidade(*it_unidade);
   //                  v.setSala(*it_sala);

   //                  v.setDisciplina(*it_disc);
   //                  v.setTurma(i);

   //                  v.setDia(dia);

   //                  it_v = vHash.find(v);
   //                  if( it_v != vHash.end() )
   //                  {
   //                     row.insert(it_v->second, 1.0);
   //                  }
   //               }
   //            }

   //            lp->addRow(row);
   //            restricoes++;
   //         }
   //      }
   //   }
   //}

   // <<<

   return restricoes;
}

int SolverMIP::cria_restricao_min_cred_dd(void)
{
   int restricoes = 0;
   char name[100];
   int nnz;

   Constraint c;

   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(itCampus,problemData->campi,Campus)
   {
      ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
      {
         ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
         {
            for(int dia = 0; dia < 7; dia++)
            {
               ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
               {
                  for(int turma = 0; turma < itDisc->num_turmas; turma++)
                  {
                     c.reset();

                     //c.setType(Constraint::C_VAR_O);
                     c.setType(Constraint::C_MIN_CREDITOS_DD);

                     c.setUnidade(*itUnidade);
                     c.setSubCjtSala(*itCjtSala);
                     c.setDia(dia);
                     c.setDisciplina(*itDisc);
                     c.setTurma(turma);

                     sprintf( name, "%s", c.toString().c_str() ); 

                     if (cHash.find(c) != cHash.end()) continue;

                     cHash[ c ] = lp->getNumRows();

                     nnz = 2;

                     OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

                     v.reset();
                     v.setType(Variable::V_OFERECIMENTO);

                     v.setTurma(turma);
                     v.setDisciplina(*itDisc);
                     v.setUnidade(*itUnidade);
                     v.setSubCjtSala(*itCjtSala);
                     v.setDia(dia);

                     it_v = vHash.find(v);
                     if( it_v != vHash.end() )
                     {
                        row.insert(it_v->second, 1.0); 
                        // FIXME
                        /* Minimo de um cr�dito, se � oferecida, s� 
                        para for�ar o oferecimento */
                     }

                     v.reset();
                     v.setType(Variable::V_CREDITOS);

                     v.setTurma(turma);
                     v.setDisciplina(*itDisc);
                     v.setUnidade(*itUnidade);
                     v.setSubCjtSala(*itCjtSala);
                     v.setDia(dia);

                     it_v = vHash.find(v);
                     if( it_v != vHash.end() )
                     { row.insert(it_v->second, -1.0); }

                     lp->addRow(row);
                     restricoes++;
                  }
               }
            }
         }
      }
   }

   //ITERA_GGROUP(it_campus,problemData->campi,Campus) {

   //   ITERA_GGROUP(it_unidade,it_campus->unidades,Unidade) {
   //      ITERA_GGROUP(it_sala,it_unidade->salas,Sala) {
   //         for(int dia=0;dia<7;dia++) {
   //            ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
   //               for(int i=0;i<it_disc->num_turmas;i++) {

   //                  c.reset();
   //                  //c.setType(Constraint::C_VAR_O);
   //                  c.setType(Constraint::C_MIN_CREDITOS_DD);
   //                  c.setUnidade(*it_unidade);
   //                  c.setSala(*it_sala);

   //                  c.setDisciplina(*it_disc);
   //                  c.setTurma(i);

   //                  c.setDia(dia);

   //                  sprintf( name, "%s", c.toString().c_str() ); 

   //                  if (cHash.find(c) != cHash.end()) continue;

   //                  cHash[ c ] = lp->getNumRows();

   //                  nnz = 2;

   //                  OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

   //                  v.reset();
   //                  v.setType(Variable::V_OFERECIMENTO);

   //                  v.setUnidade(*it_unidade);
   //                  v.setSala(*it_sala);

   //                  v.setDisciplina(*it_disc);
   //                  v.setTurma(i);

   //                  v.setDia(dia);

   //                  it_v = vHash.find(v);
   //                  if( it_v != vHash.end() )
   //                  {
   //                     row.insert(it_v->second, 1.0); 
   //                     // FIXME
   //                     /* Minimo de um cr�dito, se � oferecida, s� 
   //                     para for�ar o oferecimento */
   //                  }

   //                  v.reset();
   //                  v.setType(Variable::V_CREDITOS);

   //                  v.setUnidade(*it_unidade);
   //                  v.setSala(*it_sala);

   //                  v.setDisciplina(*it_disc);
   //                  v.setTurma(i);

   //                  v.setDia(dia);

   //                  it_v = vHash.find(v);
   //                  if( it_v != vHash.end() )
   //                  {
   //                     row.insert(it_v->second, -1.0);
   //                  }

   //                  lp->addRow(row);
   //                  restricoes++;
   //               }
   //            }
   //         }
   //      }
   //   }
   //}


   return restricoes;
}

int SolverMIP::cria_restricao_ativacao_var_o(void)
{
   int restricoes = 0;
   char name[100];
   int nnz;

   Constraint c;

   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(itCampus,problemData->campi,Campus)
   {
      ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
      {
         ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
         {
            for(int dia = 0; dia < 7; dia++)
            {
               ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
               {
                  for(int turma = 0; turma < itDisc->num_turmas; turma++)
                  {
                     c.reset();
                     c.setType(Constraint::C_VAR_O);

                     c.setUnidade(*itUnidade);
                     c.setSubCjtSala(*itCjtSala);
                     c.setDia(dia);
                     c.setDisciplina(*itDisc);
                     c.setTurma(turma);

                     sprintf( name, "%s", c.toString().c_str() ); 

                     if (cHash.find(c) != cHash.end()) continue;

                     cHash[ c ] = lp->getNumRows();

                     nnz = 2;

                     OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0, name );

                     v.reset();
                     v.setType(Variable::V_OFERECIMENTO);

                     v.setTurma(turma);
                     v.setDisciplina(*itDisc);
                     v.setUnidade(*itUnidade);
                     v.setSubCjtSala(*itCjtSala);
                     v.setDia(dia);

                     it_v = vHash.find(v);
                     if( it_v != vHash.end() )
                     {
                        row.insert(it_v->second, (itDisc->cred_praticos + 
                           itDisc->cred_teoricos));
                     }

                     v.reset();
                     v.setType(Variable::V_CREDITOS);

                     v.setTurma(turma);
                     v.setDisciplina(*itDisc);
                     v.setUnidade(*itUnidade);
                     v.setSubCjtSala(*itCjtSala);
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
   }

   //ITERA_GGROUP(it_campus,problemData->campi,Campus) {

   //   ITERA_GGROUP(it_unidade,it_campus->unidades,Unidade) {
   //      ITERA_GGROUP(it_sala,it_unidade->salas,Sala) {
   //         for(int dia=0;dia<7;dia++) {
   //            ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
   //               for(int i=0;i<it_disc->num_turmas;i++) {

   //                  c.reset();
   //                  c.setType(Constraint::C_VAR_O);

   //                  c.setUnidade(*it_unidade);
   //                  c.setSala(*it_sala);

   //                  c.setDisciplina(*it_disc);
   //                  c.setTurma(i);

   //                  c.setDia(dia);

   //                  sprintf( name, "%s", c.toString().c_str() ); 

   //                  if (cHash.find(c) != cHash.end()) continue;

   //                  cHash[ c ] = lp->getNumRows();

   //                  nnz = 2;

   //                  OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0, name );

   //                  v.reset();
   //                  v.setType(Variable::V_OFERECIMENTO);

   //                  v.setUnidade(*it_unidade);
   //                  v.setSala(*it_sala);

   //                  v.setDisciplina(*it_disc);
   //                  v.setTurma(i);

   //                  v.setDia(dia);

   //                  it_v = vHash.find(v);
   //                  if( it_v != vHash.end() )
   //                  {
   //                     row.insert(it_v->second, (it_disc->cred_praticos + 
   //                        it_disc->cred_teoricos));
   //                  }

   //                  v.reset();
   //                  v.setType(Variable::V_CREDITOS);

   //                  v.setUnidade(*it_unidade);
   //                  v.setSala(*it_sala);

   //                  v.setDisciplina(*it_disc);
   //                  v.setTurma(i);

   //                  v.setDia(dia);

   //                  it_v = vHash.find(v);
   //                  if( it_v != vHash.end() )
   //                  {
   //                     row.insert(it_v->second, -1.0);
   //                  }

   //                  lp->addRow(row);
   //                  restricoes++;
   //               }
   //            }
   //         }
   //      }
   //   }
   //}

   return restricoes;
}

int SolverMIP::cria_restricao_evita_sobreposicao(void)
{
   int restricoes = 0;

   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(itCampus,problemData->campi,Campus)
   {
      ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
      {
         for(int dia = 0; dia < 7; dia++)
         {
            ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
            {
               for(int turma = 0; turma < itDisc->num_turmas; turma++)
               {
                  c.reset();
                  c.setType(Constraint::C_EVITA_SOBREPOSICAO_TD);

                  c.setUnidade(*itUnidade);
                  c.setDia(dia);
                  c.setDisciplina(*itDisc);
                  c.setTurma(turma);

                  sprintf( name, "%s", c.toString().c_str() ); 

                  if (cHash.find(c) != cHash.end()) continue;

                  cHash[ c ] = lp->getNumRows();

                  nnz = itUnidade->salas.size();

                  OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

                  ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala) 
                  {
                     v.reset();
                     v.setType(Variable::V_OFERECIMENTO);

                     v.setTurma(turma);
                     v.setDisciplina(*itDisc);
                     v.setUnidade(*itUnidade);
                     v.setSubCjtSala(*itCjtSala);
                     v.setDia(dia);

                     it_v = vHash.find(v);
                     if( it_v != vHash.end() )
                     { row.insert(it_v->second, 1.0); }
                  }

                  lp->addRow(row);
                  restricoes++;
               }
            }
         }
      }
   }

   //ITERA_GGROUP(it_campus,problemData->campi,Campus) {

   //   ITERA_GGROUP(it_unidade,it_campus->unidades,Unidade) {
   //      for(int dia=0;dia<7;dia++) {
   //         ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
   //            for(int i=0;i<it_disc->num_turmas;i++) {

   //               c.reset();
   //               c.setType(Constraint::C_EVITA_SOBREPOSICAO_TD);

   //               c.setUnidade(*it_unidade);
   //               c.setDia(dia);
   //               c.setDisciplina(*it_disc);
   //               c.setTurma(i);

   //               sprintf( name, "%s", c.toString().c_str() ); 

   //               if (cHash.find(c) != cHash.end()) continue;

   //               cHash[ c ] = lp->getNumRows();

   //               nnz = it_unidade->salas.size();

   //               /*
   //               int creds = it_disc->cred_praticos + 
   //               it_disc->cred_teoricos;
   //               */

   //               OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

   //               ITERA_GGROUP(it_sala,it_unidade->salas,Sala) 
   //               {

   //                  v.reset();
   //                  v.setType(Variable::V_OFERECIMENTO);

   //                  v.setUnidade(*it_unidade);
   //                  v.setSala(*it_sala);

   //                  v.setDisciplina(*it_disc);
   //                  v.setTurma(i);

   //                  v.setDia(dia);

   //                  it_v = vHash.find(v);
   //                  if( it_v != vHash.end() )
   //                  {
   //                     //row.insert(it_v->second, creds);
   //                     row.insert(it_v->second, 1.0);
   //                  }
   //               }

   //               lp->addRow(row);
   //               restricoes++;
   //            }
   //         }
   //      }
   //   }
   //}

   return restricoes;
}

int SolverMIP::cria_restricao_disciplina_sala(void)
{
   int restricoes = 0;
   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(itCampus,problemData->campi,Campus)
   {
      ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
      {
         ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
         {
            ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
            {
               for(int turma = 0; turma < itDisc->num_turmas; turma++)
               {
                  c.reset();
                  c.setType(Constraint::C_TURMA_DISCIPLINA_SALA);
                  
                  c.setUnidade(*itUnidade);
                  c.setSubCjtSala(*itCjtSala);
                  c.setDisciplina(*itDisc);
                  c.setTurma(turma);

                  sprintf( name, "%s", c.toString().c_str() ); 

                  if (cHash.find(c) != cHash.end()) continue;

                  cHash[ c ] = lp->getNumRows();

                  nnz = 8;
                  OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

                  for( int dia = 0; dia < 7; dia++)
                  {
                     v.reset();
                     v.setType(Variable::V_OFERECIMENTO);

                     v.setTurma(turma);
                     v.setDisciplina(*itDisc);
                     v.setUnidade(*itUnidade);
                     v.setSubCjtSala(*itCjtSala);
                     v.setDia(dia);

                     it_v = vHash.find(v);
                     if( it_v != vHash.end() )
                     { row.insert(it_v->second, 1.0); }
                  }

                  v.reset();
                  v.setType(Variable::V_ALOC_DISCIPLINA);

                  v.setTurma(turma);
                  v.setDisciplina(*itDisc);
                  v.setUnidade(*itUnidade);
                  v.setSubCjtSala(*itCjtSala);

                  it_v = vHash.find(v);
                  if( it_v != vHash.end() )
                  { row.insert(it_v->second, -7.0); }

                  lp->addRow(row);
                  restricoes++;
               }
            }
         }
      }
   }

   //ITERA_GGROUP(it_campus,problemData->campi,Campus) {
   //   ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
   //      ITERA_GGROUP(it_sala,it_u->salas,Sala) {
   //         ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
   //            for(int i=0;i<it_disc->num_turmas;++i) {
   //               c.reset();
   //               c.setType(Constraint::C_TURMA_DISCIPLINA_SALA);
   //               c.setUnidade(*it_u);
   //               c.setSala(*it_sala);
   //               c.setDisciplina(*it_disc);
   //               c.setTurma(i);

   //               sprintf( name, "%s", c.toString().c_str() ); 

   //               if (cHash.find(c) != cHash.end()) continue;

   //               cHash[ c ] = lp->getNumRows();

   //               nnz = 8;
   //               OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

   //               for(int t=0;t<7;t++) {
   //                  v.reset();
   //                  v.setDia(t);
   //                  v.setDisciplina(*it_disc);
   //                  v.setUnidade(*it_u);
   //                  v.setSala(*it_sala);
   //                  v.setTurma(i);
   //                  v.setType(Variable::V_OFERECIMENTO);

   //                  it_v = vHash.find(v);
   //                  if( it_v != vHash.end() )
   //                  {
   //                     row.insert(it_v->second, 1.0);
   //                  }
   //               }
   //               v.reset();
   //               v.setTurma(i);
   //               v.setDisciplina(*it_disc);
   //               v.setUnidade(*it_u);
   //               v.setSala(*it_sala);
   //               v.setType(Variable::V_ALOC_DISCIPLINA);

   //               it_v = vHash.find(v);
   //               if( it_v != vHash.end() )
   //               {
   //                  row.insert(it_v->second, -7.0);
   //               }
   //               lp->addRow(row);
   //               restricoes++;
   //            }
   //         }
   //      }
   //   }
   //}

   return restricoes;
}

int SolverMIP::cria_restricao_turma_sala(void)
{
   int restricoes = 0;
   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina) 
   {
      for(int turma = 0; turma < itDisc->num_turmas; turma++)
      {
         c.reset();
         c.setType(Constraint::C_TURMA_SALA);
         c.setDisciplina(*itDisc);
         c.setTurma(turma);

         sprintf( name, "%s", c.toString().c_str() ); 

         if (cHash.find(c) != cHash.end()) continue;

         cHash[ c ] = lp->getNumRows();

         nnz = problemData->totalSalas;
         OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

         ITERA_GGROUP(itCampus,problemData->campi,Campus)
         {
            ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade) 
            {
               ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
               {
                  v.reset();
                  v.setType(Variable::V_ALOC_DISCIPLINA);

                  v.setTurma(turma);
                  v.setDisciplina(*itDisc);
                  v.setUnidade(*itUnidade);
                  v.setSubCjtSala(*itCjtSala);

                  it_v = vHash.find(v);
                  if( it_v != vHash.end() )
                  { row.insert(it_v->second, 1.0); }
               }
            }
         }

         lp->addRow(row);
         restricoes++;
      }
   }

   //ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
   //   for(int i=0;i<it_disc->num_turmas;++i) {
   //      c.reset();
   //      c.setType(Constraint::C_TURMA_SALA);
   //      c.setDisciplina(*it_disc);
   //      c.setTurma(i);

   //      sprintf( name, "%s", c.toString().c_str() ); 

   //      if (cHash.find(c) != cHash.end()) continue;

   //      cHash[ c ] = lp->getNumRows();

   //      nnz = problemData->totalSalas;
   //      OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

   //      ITERA_GGROUP(it_campus,problemData->campi,Campus) {
   //         ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
   //            ITERA_GGROUP(it_sala,it_u->salas,Sala) {
   //               v.reset();
   //               v.setType(Variable::V_ALOC_DISCIPLINA);
   //               v.setTurma(i);
   //               v.setDisciplina(*it_disc);
   //               v.setUnidade(*it_u);
   //               v.setSala(*it_sala);

   //               it_v = vHash.find(v);
   //               if( it_v != vHash.end() )
   //               {
   //                  row.insert(it_v->second, 1.0);
   //               }
   //            }
   //         }
   //      }
   //      lp->addRow(row);
   //      restricoes++;
   //   }
   //}

   return restricoes;
}

int SolverMIP::cria_restricao_evita_turma_disc_camp_d(void)
{
   int restricoes = 0;
   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
      for(int i=0;i<it_disc->num_turmas;++i) {
         c.reset();
         c.setType(Constraint::C_EVITA_TURMA_DISC_CAMP_D);
         c.setDisciplina(*it_disc);
         c.setTurma(i);

         sprintf( name, "%s", c.toString().c_str() ); 

         if (cHash.find(c) != cHash.end()) continue;

         cHash[ c ] = lp->getNumRows();

         nnz = problemData->totalSalas;
         OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

         ITERA_GGROUP(it_campus,problemData->campi,Campus) {
            v.reset();
            v.setType(Variable::V_ABERTURA);
            v.setTurma(i);
            v.setDisciplina(*it_disc);
            v.setCampus(*it_campus);

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

int SolverMIP::cria_restricao_turmas_bloco(void)
{	
   int restricoes = 0;
   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(itCampus,problemData->campi,Campus)
   {
      for(int dia = 0; dia < 7; dia++)
      {
         ITERA_GGROUP(itBloco,problemData->blocos,BlocoCurricular)
         {
            c.reset();
            c.setType(Constraint::C_TURMAS_BLOCO);
            c.setBloco(*itBloco);
            c.setDia(dia);
            c.setCampus(*itCampus);

            sprintf( name, "%s", c.toString().c_str() ); 

            if (cHash.find(c) != cHash.end()) continue;

            cHash[ c ] = lp->getNumRows();

            nnz = itBloco->total_turmas * 
               problemData->totalSalas;

            OPT_ROW row( nnz + 1, OPT_ROW::LESS , 0.0, name );

            ITERA_GGROUP(itDisc,itBloco->disciplinas,Disciplina)
            {
               ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
               {
                  ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
                  {
                     for(int turma = 0; turma < itDisc->num_turmas; turma++)
                     {
                        v.reset();
                        v.setType(Variable::V_OFERECIMENTO);
                        v.setTurma(turma);
                        v.setDisciplina(*itDisc);
                        v.setUnidade(*itUnidade);
                        v.setSubCjtSala(*itCjtSala);
                        v.setDia(dia);

                        it_v = vHash.find(v);
                        if( it_v != vHash.end() )
                        { row.insert(it_v->second, 1.0); }
                     }
                  }
               }
            }

            v.reset();
            v.setType(Variable::V_N_SUBBLOCOS);
            v.setBloco(*itBloco);
            v.setDia(dia);
            v.setCampus(*itCampus);

            it_v = vHash.find(v);
            if( it_v != vHash.end() )
            { row.insert(it_v->second, -9999.0); }
            /* Provavelmente esta restri��o � in�til */

            lp->addRow(row);
            ++restricoes;
         }
      }
   }

   //ITERA_GGROUP(it_campus,problemData->campi,Campus) {
   //   for(int t=0;t<7;t++) {
   //      ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular) {
   //         c.reset();
   //         c.setType(Constraint::C_TURMAS_BLOCO);
   //         c.setBloco(*it_bloco);
   //         c.setDia(t);
   //         c.setCampus(*it_campus);

   //         sprintf( name, "%s", c.toString().c_str() ); 

   //         if (cHash.find(c) != cHash.end()) continue;

   //         cHash[ c ] = lp->getNumRows();

   //         nnz = it_bloco->total_turmas * 
   //            problemData->totalSalas;

   //         OPT_ROW row( nnz + 1, OPT_ROW::LESS , 0.0, name );

   //         ITERA_GGROUP(it_disc,it_bloco->disciplinas,Disciplina) {
   //            ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
   //               ITERA_GGROUP(it_sala,it_u->salas,Sala) {
   //                  for(int i=0;i<it_disc->num_turmas;++i) {
   //                     v.reset();
   //                     v.setType(Variable::V_OFERECIMENTO);
   //                     v.setTurma(i);
   //                     v.setDisciplina(*it_disc);
   //                     v.setUnidade(*it_u);
   //                     v.setSala(*it_sala);
   //                     v.setDia(t);

   //                     it_v = vHash.find(v);
   //                     if( it_v != vHash.end() )
   //                     {
   //                        row.insert(it_v->second, 1.0);
   //                     }
   //                  }
   //               }
   //            }
   //         }

   //         v.reset();
   //         v.setType(Variable::V_N_SUBBLOCOS);
   //         v.setBloco(*it_bloco);
   //         v.setDia(t);
   //         v.setCampus(*it_campus);

   //         it_v = vHash.find(v);
   //         if( it_v != vHash.end() )
   //         {
   //            row.insert(it_v->second, -9999.0);
   //         }
   //         /* Provavelmente esta restri��o � in�til */

   //         lp->addRow(row);
   //         ++restricoes;
   //      }
   //   }
   //}

   return restricoes;
}

int SolverMIP::cria_restricao_max_cred_disc_bloco(void)
{
   int restricoes = 0;
   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

      ITERA_GGROUP(itCampus,problemData->campi,Campus)
   {
      for(int dia = 0; dia < 7; dia++)
      {
         ITERA_GGROUP(itBloco,problemData->blocos,BlocoCurricular)
         {
            c.reset();
            c.setType(Constraint::C_MAX_CRED_DISC_BLOCO);
            c.setBloco(*itBloco);
            c.setDia(dia);
            c.setCampus(*itCampus);

            sprintf( name, "%s", c.toString().c_str() ); 

            if (cHash.find(c) != cHash.end()) continue;

            cHash[ c ] = lp->getNumRows();

            nnz = itBloco->total_turmas * 
               problemData->totalSalas;

            OPT_ROW row( nnz + 1, OPT_ROW::LESS , 0.0, name );

            ITERA_GGROUP(itDisc,itBloco->disciplinas,Disciplina)
            {
               ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
               {
                  ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
                  {
                     for(int turma = 0; turma < itDisc->num_turmas; turma++)
                     {
                        v.reset();
                        v.setType(Variable::V_CREDITOS);
                        v.setTurma(turma);
                        v.setDisciplina(*itDisc);
                        v.setUnidade(*itUnidade);
                        v.setSubCjtSala(*itCjtSala);
                        v.setDia(dia);

                        it_v = vHash.find(v);
                        if( it_v != vHash.end() )
                        { row.insert(it_v->second, 1.0); }
                     }
                  }
               }
            }

            v.reset();
            v.setType(Variable::V_N_SUBBLOCOS);
            v.setBloco(*itBloco);
            v.setDia(dia);
            v.setCampus(*itCampus);

            it_v = vHash.find(v);
            if( it_v != vHash.end() )
            { row.insert(it_v->second, -24.0); /* #Warning: FIXME */ }
            /* Descobrir valor de H_t */

            lp->addRow(row);
            ++restricoes;
         }
      }
   }

   //ITERA_GGROUP(it_campus,problemData->campi,Campus) {
   //   for(int t=0;t<7;t++) {
   //      ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular) {
   //         c.reset();
   //         c.setType(Constraint::C_MAX_CRED_DISC_BLOCO);
   //         c.setBloco(*it_bloco);
   //         c.setDia(t);
   //         c.setCampus(*it_campus);

   //         sprintf( name, "%s", c.toString().c_str() ); 

   //         if (cHash.find(c) != cHash.end()) continue;

   //         cHash[ c ] = lp->getNumRows();

   //         nnz = it_bloco->total_turmas * 
   //            problemData->totalSalas;

   //         OPT_ROW row( nnz + 1, OPT_ROW::LESS , 0.0, name );

   //         ITERA_GGROUP(it_disc,it_bloco->disciplinas,Disciplina) {
   //            ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
   //               ITERA_GGROUP(it_sala,it_u->salas,Sala) {
   //                  for(int i=0;i<it_disc->num_turmas;++i) {
   //                     v.reset();
   //                     v.setType(Variable::V_CREDITOS);
   //                     v.setTurma(i);
   //                     v.setDisciplina(*it_disc);
   //                     v.setUnidade(*it_u);
   //                     v.setSala(*it_sala);
   //                     v.setDia(t);

   //                     it_v = vHash.find(v);
   //                     if( it_v != vHash.end() )
   //                     {
   //                        row.insert(it_v->second, 1.0);
   //                     }
   //                  }
   //               }
   //            }
   //         }
   //         v.reset();
   //         v.setType(Variable::V_N_SUBBLOCOS);
   //         v.setBloco(*it_bloco);
   //         v.setDia(t);
   //         v.setCampus(*it_campus);

   //         it_v = vHash.find(v);
   //         if( it_v != vHash.end() )
   //         {
   //            row.insert(it_v->second, -24.0); // #Warning: FIXME
   //         }
   //         /* Descobrir valor de H_t */

   //         lp->addRow(row);
   //         ++restricoes;
   //      }
   //   }
   //}

   return restricoes;
}

int SolverMIP::cria_restricao_num_tur_bloc_dia_difunid(void)
{
   int restricoes = 0;
   char name[100];
   int nnz;

   Constraint c;

   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular) {
      for(int dia = 0; dia < 7; dia++) {

         c.reset();
         c.setType(Constraint::C_NUM_TUR_BLOC_DIA_DIFUNID);

         c.setBloco(*it_bloco);
         c.setDia(dia);

         sprintf( name, "%s", c.toString().c_str() ); 

         if (cHash.find(c) != cHash.end()) continue;

         cHash[ c ] = lp->getNumRows();

         nnz = problemData->campi.size();

         OPT_ROW row( nnz + 1, OPT_ROW::LESS , 1.0 , 
            name );

         ITERA_GGROUP(it_cp,problemData->campi,Campus) {
            v.reset();
            v.setType(Variable::V_ABERTURA_SUBBLOCO_DE_BLC_DIA_CAMPUS);

            v.setBloco(*it_bloco);
            v.setDia(dia);
            v.setCampus(*it_cp);

            it_v = vHash.find(v);
            if( it_v != vHash.end() )
            {
               row.insert(it_v->second, 1.0);
            }
         }

         v.reset();
         v.setType(Variable::V_N_ABERT_TURMA_BLOCO);

         v.setBloco(*it_bloco);
         v.setDia(dia);

         it_v = vHash.find(v);
         if( it_v != vHash.end() )
         {
            row.insert(it_v->second, -1.0);
         }

         lp->addRow(row);
         ++restricoes;
      }
   }

   return restricoes;
}

int SolverMIP::cria_restricao_lim_cred_diar_disc(void)
{
   int restricoes = 0;

   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(itCampus,problemData->campi,Campus)
   {
      ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
      {
         ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
         {
            for(int dia = 0; dia < 7; dia++)
            {
               ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
               {
                  for(int turma = 0; turma < itDisc->num_turmas; turma++)
                  {
                     c.reset();
                     c.setType(Constraint::C_LIM_CRED_DIAR_DISC);
                     c.setTurma(turma);
                     c.setDisciplina(*itDisc);
                     c.setUnidade(*itUnidade);
                     c.setSubCjtSala(*itCjtSala);
                     c.setDia(dia);

                     sprintf( name, "%s", c.toString().c_str() ); 

                     if (cHash.find(c) != cHash.end()) continue;

                     cHash[ c ] = lp->getNumRows();

                     nnz = 1;

                     OPT_ROW row( 1, OPT_ROW::LESS , itDisc->max_creds , 
                        name );

                     v.reset();
                     v.setType(Variable::V_CREDITOS);
                     v.setTurma(turma);
                     v.setDisciplina(*itDisc);
                     v.setUnidade(*itUnidade);
                     v.setSubCjtSala(*itCjtSala);
                     v.setDia(dia);

                     it_v = vHash.find(v);
                     if( it_v != vHash.end() )
                     { row.insert(it_v->second, 1.0); }

                     lp->addRow(row);
                     ++restricoes;
                  }
               }
            }
         }
      }
   }

   //ITERA_GGROUP(it_campus,problemData->campi,Campus) {
   //   ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
   //      ITERA_GGROUP(it_sala,it_u->salas,Sala) {
   //         for(int t=0;t<7;t++) {
   //            ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
   //               for(int i=0;i<it_disc->num_turmas;i++) {
   //                  c.reset();
   //                  c.setType(Constraint::C_LIM_CRED_DIAR_DISC);
   //                  c.setTurma(i);
   //                  c.setDisciplina(*it_disc);
   //                  c.setUnidade(*it_u);
   //                  c.setSala(*it_sala);
   //                  c.setDia(t);

   //                  sprintf( name, "%s", c.toString().c_str() ); 

   //                  if (cHash.find(c) != cHash.end()) continue;

   //                  cHash[ c ] = lp->getNumRows();

   //                  nnz = 1;

   //                  OPT_ROW row( 1, OPT_ROW::LESS , it_disc->max_creds , 
   //                     name );

   //                  v.reset();
   //                  v.setType(Variable::V_CREDITOS);
   //                  v.setTurma(i);
   //                  v.setDisciplina(*it_disc);
   //                  v.setUnidade(*it_u);
   //                  v.setSala(*it_sala);
   //                  v.setDia(t);

   //                  it_v = vHash.find(v);
   //                  if( it_v != vHash.end() )
   //                  {
   //                     row.insert(it_v->second, 1.0);

   //                     lp->addRow(row);
   //                     ++restricoes;
   //                  }

   //                  //lp->addRow(row);
   //                  //++restricoes;
   //               }
   //            }
   //         }
   //      }
   //   }
   //}

   return restricoes;
}

int SolverMIP::cria_restricao_cap_aloc_dem_disc(void)
{
   int restricoes = 0;
   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(itOferta,problemData->ofertas,Oferta)
   {
      GGroup<DisciplinaPeriodo>::iterator itPrdDisc = 
         itOferta->curriculo->disciplinas_periodo.begin();

      for(; itPrdDisc != itOferta->curriculo->disciplinas_periodo.end(); itPrdDisc++)
      { 
         Disciplina * ptDisc = problemData->refDisciplinas[(*itPrdDisc).second];

         c.reset();
         c.setType(Constraint::C_CAP_ALOC_DEM_DISC);
         c.setOferta(*itOferta);
         c.setDisciplina(ptDisc);

         sprintf( name, "%s", c.toString().c_str() ); 

         if (cHash.find(c) != cHash.end()) continue;

         cHash[ c ] = lp->getNumRows();

         nnz = ptDisc->num_turmas;

         int rhs = 0;

         // Calculando P_{d,o}
         ITERA_GGROUP(itDem,problemData->demandas,Demanda)
         {
            if (itDem->disciplina->getId() == ptDisc->getId() &&
               itDem->oferta_id == itOferta->getId())
            {
               rhs += itDem->quantidade;
            }
         }

         OPT_ROW row( nnz , OPT_ROW::EQUAL , rhs , name );
        
         for(int turma = 0; turma < ptDisc->num_turmas; turma++)
         {
            v.reset();
            v.setType(Variable::V_ALUNOS);
            v.setTurma(turma);
            v.setDisciplina(ptDisc);
            v.setOferta(*itOferta);

            it_v = vHash.find(v);
            if( it_v != vHash.end() )
            { row.insert(it_v->second, 1.0); }
         }

         lp->addRow(row);
         ++restricoes;

      }
   }

   //ITERA_GGROUP(it_campus,problemData->campi,Campus) {
   //   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
   //      ITERA_GGROUP(it_curso,problemData->cursos,Curso) {
   //         c.reset();
   //         c.setType(Constraint::C_CAP_ALOC_DEM_DISC);
   //         c.setCampus(*it_campus);
   //         c.setDisciplina(*it_disc);
   //         c.setCurso(*it_curso);

   //         sprintf( name, "%s", c.toString().c_str() ); 

   //         if (cHash.find(c) != cHash.end()) continue;

   //         cHash[ c ] = lp->getNumRows();

   //         nnz = it_disc->num_turmas;

   //         int rhs = 0;
   //         ITERA_GGROUP(it_dem,problemData->demandas,Demanda) {
   //            if (it_dem->disciplina->getId() == it_disc->getId() &&
   //               it_dem->oferta->curso->getId() == it_curso->getId() &&
   //               it_dem->oferta->campus->getId() == it_campus->getId())
   //            {
   //               rhs += it_dem->quantidade;
   //            }
   //         }

   //         OPT_ROW row( nnz , OPT_ROW::EQUAL , rhs , name );

   //         for(int i=0;i<it_disc->num_turmas;++i) {
   //            v.reset();
   //            v.setType(Variable::V_ALUNOS);
   //            v.setTurma(i);
   //            v.setDisciplina(*it_disc);
   //            v.setCampus(*it_campus);
   //            v.setCurso(*it_curso);

   //            it_v = vHash.find(v);
   //            if( it_v != vHash.end() )
   //            {
   //               row.insert(it_v->second, 1.0);
   //            }
   //         }

   //         lp->addRow(row);
   //         ++restricoes;
   //      }
   //   }
   //}

   return restricoes;
}

int SolverMIP::cria_restricao_cap_sala_compativel_turma(void)
{
   int restricoes = 0;
   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(it_campus,problemData->campi,Campus) {
      ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
         for(int i=0;i<it_disc->num_turmas;++i) {

            c.reset();
            c.setType(Constraint::C_CAP_SALA_COMPATIVEL_TURMA);
            c.setCampus(*it_campus);
            c.setDisciplina(*it_disc);
            c.setTurma(i);

            sprintf( name, "%s", c.toString().c_str() ); 

            if (cHash.find(c) != cHash.end()) continue;

            cHash[ c ] = lp->getNumRows();

            nnz = problemData->cursos.size() +
               it_campus->totalSalas * 7;

            OPT_ROW row( nnz , OPT_ROW::LESS , 0.0 , name );

            ITERA_GGROUP(it_curso,problemData->cursos,Curso) {
               v.reset();
               v.setType(Variable::V_ALUNOS);
               v.setTurma(i);
               v.setDisciplina(*it_disc);
               v.setCampus(*it_campus);
               v.setCurso(*it_curso);

               it_v = vHash.find(v);
               if( it_v != vHash.end() )
               {
                  row.insert(it_v->second, 1.0);
               }
            }

            ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
               ITERA_GGROUP(it_sala,it_u->salas,Sala) {
                  for(int t=0;t<7;t++) {
                     v.reset();
                     v.setType(Variable::V_OFERECIMENTO);
                     v.setTurma(i);
                     v.setDisciplina(*it_disc);
                     v.setUnidade(*it_u);
                     v.setSala(*it_sala);
                     v.setDia(t);

                     it_v = vHash.find(v);
                     if( it_v != vHash.end() )
                     {
                        row.insert(it_v->second, -it_sala->capacidade);
                     }
                  }
               }
            }
            lp->addRow(row);
            ++restricoes;
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

   ITERA_GGROUP(it_campus,problemData->campi,Campus) {
      ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
         ITERA_GGROUP(it_sala,it_u->salas,Sala) {
            ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
               for(int i=0;i<it_disc->num_turmas;++i) {
                  for(int t=0;t<7;t++) {
                     c.reset();
                     c.setType(Constraint::C_CAP_SALA_UNIDADE);
                     c.setUnidade(*it_u);
                     c.setSala(*it_sala);
                     c.setDisciplina(*it_disc);
                     c.setTurma(i);
                     c.setCampus(*it_campus);
                     c.setDia(t);

                     sprintf( name, "%s", c.toString().c_str() ); 

                     if (cHash.find(c) != cHash.end()) continue;

                     cHash[ c ] = lp->getNumRows();

                     nnz = problemData->cursos.size() + 1;
                     int rhs = it_sala->capacidade + it_u->maiorSala;

                     OPT_ROW row( nnz, OPT_ROW::LESS , rhs , name );

                     v.reset();
                     v.setDia(t);
                     v.setDisciplina(*it_disc);
                     v.setUnidade(*it_u);
                     v.setSala(*it_sala);
                     v.setTurma(i);
                     v.setType(Variable::V_OFERECIMENTO);

                     it_v = vHash.find(v);
                     if( it_v != vHash.end() )
                     {
                        row.insert(it_v->second, it_u->maiorSala);
                     }

                     ITERA_GGROUP(it_curso,problemData->cursos,Curso) {

                        v.reset();
                        v.setTurma(i);
                        v.setDisciplina(*it_disc);
                        v.setCampus(*it_campus);
                        v.setCurso(*it_curso);

                        v.setType(Variable::V_ALUNOS);

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
            }
         }
      }
   }
   return restricoes;
}

int SolverMIP::cria_restricao_turma_disc_dias_consec(void)
{
   int restricoes = 0;

   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
   {
      for(int turma = 0; turma < itDisc->num_turmas; turma++)
      {
         for(int dia = 1; dia < 7; dia++)
         {
            c.reset();
            c.setDisciplina(*itDisc);
            c.setType(Constraint::C_TURMA_DISC_DIAS_CONSEC);
            c.setTurma(turma);
            c.setDia(dia);

            sprintf( name, "%s", c.toString().c_str() ); 

            if (cHash.find(c) != cHash.end()) continue;

            cHash[ c ] = lp->getNumRows();

            nnz = problemData->totalSalas * 2 + 1;

            OPT_ROW row( nnz, OPT_ROW::GREATER , -1 , name );

            v.reset();
            v.setType(Variable::V_DIAS_CONSECUTIVOS);
            v.setTurma(turma);
            v.setDisciplina(*itDisc);
            v.setDia(dia);

            it_v = vHash.find(v);
            if( it_v != vHash.end() )
            { row.insert(it_v->second, 1.0); }

            ITERA_GGROUP(itCampus,problemData->campi,Campus)
            {
               ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
               {
                  ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
                  {
                     v.reset();
                     v.setType(Variable::V_OFERECIMENTO);
                     v.setTurma(turma);
                     v.setDisciplina(*itDisc);
                     v.setUnidade(*itUnidade);
                     v.setSubCjtSala(*itCjtSala);
                     v.setDia(dia);

                     it_v = vHash.find(v);
                     if( it_v != vHash.end() )
                     { row.insert(it_v->second, -1.0); }

                     v.setDia(dia-1);

                     it_v = vHash.find(v);
                     if( it_v != vHash.end() )
                     { row.insert(it_v->second, 1.0); }
                  }
               }
            }

            lp->addRow(row);
            ++restricoes;
         }
      }
   }

   //ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
   //   for(int i=0;i<it_disc->num_turmas;++i) {
   //      for(int t=1;t<7;t++) {
   //         c.reset();
   //         c.setDisciplina(*it_disc);
   //         c.setType(Constraint::C_TURMA_DISC_DIAS_CONSEC);
   //         c.setTurma(i);
   //         c.setDia(t);

   //         sprintf( name, "%s", c.toString().c_str() ); 

   //         if (cHash.find(c) != cHash.end()) continue;

   //         cHash[ c ] = lp->getNumRows();

   //         nnz = problemData->totalSalas * 2 + 1;

   //         OPT_ROW row( nnz, OPT_ROW::GREATER , -1 , name );

   //         v.reset();
   //         v.setType(Variable::V_DIAS_CONSECUTIVOS);
   //         v.setTurma(i);
   //         v.setDisciplina(*it_disc);
   //         v.setDia(t);

   //         it_v = vHash.find(v);
   //         if( it_v != vHash.end() )
   //         {
   //            row.insert(it_v->second, 1.0);
   //         }

   //         ITERA_GGROUP(it_campus,problemData->campi,Campus) {
   //            ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
   //               ITERA_GGROUP(it_sala,it_u->salas,Sala) {
   //                  v.reset();
   //                  v.setType(Variable::V_OFERECIMENTO);
   //                  v.setTurma(i);
   //                  v.setDisciplina(*it_disc);
   //                  v.setUnidade(*it_u);
   //                  v.setSala(*it_sala);
   //                  v.setDia(t);

   //                  it_v = vHash.find(v);
   //                  if( it_v != vHash.end() )
   //                  {
   //                     row.insert(it_v->second, -1.0);
   //                  }

   //                  v.setDia(t-1);

   //                  it_v = vHash.find(v);
   //                  if( it_v != vHash.end() )
   //                  {
   //                     row.insert(it_v->second, 1.0);
   //                  }
   //               }
   //            }
   //         }
   //         lp->addRow(row);
   //         ++restricoes;
   //      }
   //   }
   //}

   return restricoes;
}

int SolverMIP::cria_restricao_min_creds_turm_bloco(void)
{
   int restricoes = 0;

   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular) {
      ITERA_GGROUP(it_disc,it_bloco->disciplinas,Disciplina) {
         for(int i=0;i<it_disc->num_turmas;i++){
            for(int dia=0;dia<7;dia++) {
               c.reset();
               c.setType(Constraint::C_MIN_CREDS_TURM_BLOCO);

               c.setBloco(*it_bloco);
               c.setDisciplina(*it_disc);
               c.setDia(dia);

               sprintf( name, "%s", c.toString().c_str() ); 

               if (cHash.find(c) != cHash.end()) continue;

               cHash[ c ] = lp->getNumRows();

               // Conferir nnz depois.
               nnz = (it_bloco->disciplinas.size() * problemData->totalSalas) + 1;

               OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0 , name );

               ITERA_GGROUP(it_b,problemData->blocos,BlocoCurricular) {
                  ITERA_GGROUP(it_disc,it_b->disciplinas,Disciplina) {
                     ITERA_GGROUP(it_u,it_b->campus->unidades,Unidade) {
                        ITERA_GGROUP(it_sala,it_u->salas,Sala) {
                           v.reset();
                           v.setType(Variable::V_CREDITOS);

                           v.setTurma(i);
                           v.setDisciplina(*it_disc);
                           v.setUnidade(*it_u);
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
               }

               v.reset();
               v.setType(Variable::V_MIN_CRED_SEMANA);

               v.setTurma(i);
               v.setBloco(*it_bloco);

               it_v = vHash.find(v);
               if( it_v != vHash.end() )
               {
                  row.insert(it_v->second, -1.0);
               }

               lp->addRow(row);
               ++restricoes;
            }
         }
      }
   }

   return restricoes;
}

int SolverMIP::cria_restricao_max_creds_turm_bloco(void)
{
   int restricoes = 0;

   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular) {
      ITERA_GGROUP(it_disc,it_bloco->disciplinas,Disciplina) {
         for(int i=0;i<it_disc->num_turmas;i++){
            for(int dia=0;dia<7;dia++) {
               c.reset();
               c.setType(Constraint::C_MAX_CREDS_TURM_BLOCO);

               c.setBloco(*it_bloco);
               c.setDisciplina(*it_disc);
               c.setDia(dia);

               sprintf( name, "%s", c.toString().c_str() ); 

               if (cHash.find(c) != cHash.end()) continue;

               cHash[ c ] = lp->getNumRows();

               // Conferir nnz depois.
               nnz = (it_bloco->disciplinas.size() * problemData->totalSalas) + 1;

               OPT_ROW row( nnz, OPT_ROW::LESS , 0.0 , name );

               ITERA_GGROUP(it_b,problemData->blocos,BlocoCurricular) {
                  ITERA_GGROUP(it_disc,it_b->disciplinas,Disciplina) {
                     ITERA_GGROUP(it_u,it_b->campus->unidades,Unidade) {
                        ITERA_GGROUP(it_sala,it_u->salas,Sala) {
                           v.reset();
                           v.setType(Variable::V_CREDITOS);

                           v.setTurma(i);
                           v.setDisciplina(*it_disc);
                           v.setUnidade(*it_u);
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
               }

               v.reset();
               v.setType(Variable::V_MAX_CRED_SEMANA);

               v.setTurma(i);
               v.setBloco(*it_bloco);

               it_v = vHash.find(v);
               if( it_v != vHash.end() )
               {
                  row.insert(it_v->second, -1.0);
               }

               lp->addRow(row);
               ++restricoes;
            }
         }
      }
   }

   return restricoes;
}

int SolverMIP::cria_restricao_aluno_curso_disc(void)
{
   int restricoes = 0;

   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(it_campus,problemData->campi,Campus) {
      ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
         for(int i=0;i<it_disc->num_turmas;++i) {
            ITERA_GGROUP(it_curso,problemData->cursos,Curso) {

               c.reset();
               c.setType(Constraint::C_ALUNO_CURSO_DISC);
               c.setDisciplina(*it_disc);
               c.setTurma(i);
               c.setCurso(*it_curso);
               c.setCampus(*it_campus);

               sprintf( name, "%s", c.toString().c_str() ); 

               if (cHash.find(c) != cHash.end()) continue;

               cHash[ c ] = lp->getNumRows();

               nnz = 2;

               OPT_ROW row( nnz, OPT_ROW::LESS , 0.0 , name );

               v.reset();
               v.setType(Variable::V_ALUNOS);
               v.setTurma(i);
               v.setDisciplina(*it_disc);
               v.setCurso(*it_curso);               
               v.setCampus(*it_campus);

               it_v = vHash.find(v);
               if( it_v != vHash.end() )
               {
                  row.insert(it_v->second, 1.0);
               }

               v.setType(Variable::V_ALOC_ALUNO);
               it_v = vHash.find(v);
               if( it_v != vHash.end() )
               {
                  row.insert(it_v->second, -it_campus->maiorSala );
               }

               lp->addRow(row);
               ++restricoes;
            }
         }
      }
   }
   return restricoes;
}

int SolverMIP::cria_restricao_alunos_cursos_dif(void)
{
   int restricoes = 0;
   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(it_campus,problemData->campi,Campus) {
      ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
         for(int i=0;i<it_disc->num_turmas;++i) {

            std::map<std::pair<Curso*,Curso*>,bool>::iterator it_compat_cursos =
               problemData->compat_cursos.begin();

            for(; it_compat_cursos != problemData->compat_cursos.end(); ++it_compat_cursos) {

               if(!it_compat_cursos->second) {

                  c.reset();
                  c.setType(Constraint::C_ALUNOS_CURSOS_DIF);

                  c.setCampus(*it_campus);

                  c.setCurso(it_compat_cursos->first.first);
                  c.setCursoIncompat(it_compat_cursos->first.second);

                  c.setDisciplina(*it_disc);
                  c.setTurma(i);

                  sprintf( name, "%s", c.toString().c_str() ); 

                  if (cHash.find(c) != cHash.end()) continue;

                  // Testando com os cursos invertidos

                  c.setCurso(it_compat_cursos->first.second);
                  c.setCursoIncompat(it_compat_cursos->first.first);

                  if (cHash.find(c) != cHash.end()) continue;

                  cHash[ c ] = lp->getNumRows();

                  nnz = 2;

                  OPT_ROW row( nnz, OPT_ROW::LESS , 1.0 , name );

                  // ---

                  v.reset();
                  v.setType(Variable::V_ALOC_ALUNO);

                  v.setTurma(i);
                  v.setDisciplina(*it_disc);
                  v.setCampus(*it_campus);
                  v.setCurso(it_compat_cursos->first.first);

                  it_v = vHash.find(v);
                  if( it_v != vHash.end() )
                  {
                     row.insert(it_v->second, 1);
                  }

                  // ---

                  v.reset();
                  v.setType(Variable::V_ALOC_ALUNO);

                  v.setTurma(i);
                  v.setDisciplina(*it_disc);
                  v.setCampus(*it_campus);
                  v.setCurso(it_compat_cursos->first.second);

                  it_v = vHash.find(v);
                  if( it_v != vHash.end() )
                  {
                     row.insert(it_v->second, 1);
                  }

                  lp->addRow(row);
                  ++restricoes;

               }
            }

         }
      }
   }

   return restricoes;
}

int SolverMIP::cria_restricao_de_folga_dist_cred_dia(void)
{
   int restricoes = 0;

   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
      for(int dia=0;dia<7;dia++) {
         c.reset();
         c.setType(Constraint::C_SLACK_DIST_CRED_DIA);

         c.setDisciplina(*it_disc);
         c.setDia(dia);

         sprintf( name, "%s", c.toString().c_str() ); 

         if (cHash.find(c) != cHash.end()) continue;

         cHash[ c ] = lp->getNumRows();

         nnz = problemData->totalSalas * it_disc->num_turmas  + 1;

         int rhs = it_disc->max_creds;
         if( it_disc->divisao_creditos != NULL )
         {
            rhs = it_disc->divisao_creditos->dia[dia];
         }

         OPT_ROW row( nnz, OPT_ROW::EQUAL , rhs , name );

         ITERA_GGROUP(it_campus,problemData->campi,Campus) {

            ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
               ITERA_GGROUP(it_sala,it_u->salas,Sala) {

                  ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
                     for(int i=0;i<it_disc->num_turmas;i++) {

                        v.reset();
                        v.setType(Variable::V_CREDITOS);

                        v.setTurma(i);
                        v.setDisciplina(*it_disc);
                        v.setUnidade(*it_u);
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
            }
         }

         v.reset();
         v.setType(Variable::V_SLACK_DIST_CRED_DIA_SUPERIOR);

         v.setDisciplina(*it_disc);
         v.setDia(dia);

         it_v = vHash.find(v);
         if( it_v != vHash.end() )
         {
            row.insert(it_v->second, 1.0);
         }

         v.reset();
         v.setType(Variable::V_SLACK_DIST_CRED_DIA_INFERIOR);

         v.setDisciplina(*it_disc);
         v.setDia(dia);

         it_v = vHash.find(v);
         if( it_v != vHash.end() )
         {
            row.insert(it_v->second, -1.0);
         }

         lp->addRow(row);
         ++restricoes;
      }
   }

   return restricoes;
}

int SolverMIP::cria_restricao_ativacao_var_r()
{
   int restricoes = 0;
   char name[100];
   int nnz;

   Constraint c;

   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular) {
      for(int dia=0;dia<7;dia++) {

         c.reset();
         c.setType(Constraint::C_VAR_R);

         c.setBloco(*it_bloco);
         c.setCampus(it_bloco->campus);
         c.setDia(dia);

         sprintf( name, "%s", c.toString().c_str() ); 

         if (cHash.find(c) != cHash.end()) continue;

         cHash[ c ] = lp->getNumRows();

         nnz = 2;

         OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

         v.reset();
         v.setType(Variable::V_N_SUBBLOCOS);

         v.setBloco(*it_bloco);
         v.setDia(dia);
         v.setCampus(it_bloco->campus);

         it_v = vHash.find(v);
         if( it_v != vHash.end() )
         {
            row.insert(it_v->second, 1.0);
         }

         v.reset();
         v.setType(Variable::V_ABERTURA_SUBBLOCO_DE_BLC_DIA_CAMPUS);

         v.setBloco(*it_bloco);
         v.setDia(dia);
         v.setCampus(it_bloco->campus);

         it_v = vHash.find(v);
         if( it_v != vHash.end() )
         {
            row.insert(it_v->second, -9999.0);
         }

         lp->addRow(row);
         restricoes++;
      }
   }

   return restricoes;
}

/*
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

int max_creds = 99999999; //TODO, achar max_creds
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

int max_creds = 99999999; //TODO, achar min_creds

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
*/

/*
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
*/