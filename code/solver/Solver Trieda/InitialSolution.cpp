#include "InitialSolution.h"

//bool ordena_dec_demanda(Demanda const * left, Demanda const * right)
bool ordena_dec_demanda(pair<Demanda const *, int> const & left, pair<Demanda const *, int> const & right)
{
   //return (left->quantidade > right->quantidade);
   return (left.second > right.second);
}

//bool ordenaConjuntosDemandas(pair<GGroup<Demanda*>,int> const & left, pair<GGroup<Demanda*>,int> const & right)
bool ordenaConjuntosDemandas(pair<pair<Disciplina*,int>, int*> const & left,
                             pair<pair<Disciplina*,int>, int*> const & right)
{
   return (*left.second > *right.second);
}

//bool ordenaConjuntosDemandasPorDisciplina(pair<GGroup<Demanda*>,int> const * left, pair<GGroup<Demanda*>,int> const * right)
//{
//   if(left && right) // <left> e <right> != de NULL
//   { return (left->second > right->second); }
//   else if(left && !right) // <left> != de NULL e <right> NULL
//   { return true; }
//   else if(!left && right) // <left> != de NULL e <right> NULL
//   { return false; }
//
//   return false; // <left> e <right> NULL
//}

bool ordenaIS_SalasDecrescente(IS_Sala const * left, IS_Sala const * right)
{
   return (left->sala->capacidade > right->sala->capacidade);
}

//bool ordena_dec_sala(Sala const * left, Sala const * right)
//{
//   return (left->capacidade > right->capacidade);
//}

InitialSolution::InitialSolution(ProblemData & _problem_Data) :
problem_Data(&_problem_Data), total_Cjt_Demandas(0)
{
   // ---------------------------------

   // Agrupando as demandas
   agrupaDemandas();

   // ---------------------------------

   // Criando as estruturas IS_* para o <atendimento_Tatico_AUX>
   ITERA_GGROUP(it_Campus,problem_Data->campi,Campus)
   {
      IS_Campus * is_campus = new IS_Campus(*it_Campus);

      ITERA_GGROUP(it_Unidade,it_Campus->unidades,Unidade)
      {
         IS_Unidade * is_unidade = new IS_Unidade(*it_Unidade);

         ITERA_GGROUP(it_Sala,it_Unidade->salas,Sala)
         {
            IS_Sala * is_sala = new IS_Sala(*it_Sala);

            ITERA_GGROUP(it_Creds_Disp,it_Sala->creditos_disponiveis,CreditoDisponivel)
            {
               // Criando um IS_Dia_Semana
               IS_Dia_Semana * dia_Sem = new IS_Dia_Semana(*it_Creds_Disp);

               // P/ cada credito a ser criado
               for(int cred = 0; cred < it_Creds_Disp->max_creditos; cred++)
               {
                  // Criando um IS_Atendimento_Tatico
                  IS_Atendimento_Tatico * is_At_Tat = new IS_Atendimento_Tatico(cred);

                  // Adicionando o IS_Atendimento_Tatico criado acima ao IS_Dia_Semana
                  dia_Sem->is_Atendimentos_Taticos.add(is_At_Tat);
               }

               is_sala->is_Dia_Semana.add(dia_Sem);
            }

            is_unidade->salas.add(is_sala);
         }

         is_campus->unidades.add(is_unidade);
      }

      solucao.add(is_campus);
   }
}

InitialSolution::InitialSolution(InitialSolution const & init_sol) :
problem_Data(&init_sol.get_Problem_Data())
{
   cerr << "COPY CONSTRUCTOR OF <InitialSolution> NOT IMPLEMENTED YET" << endl;
   exit(1);
}

InitialSolution::~InitialSolution(void)
{
}

void InitialSolution::agrupaDemandas()
{
   // P/ cada demanda do <problem_Data>
   ITERA_GGROUP(it_Demanda,problem_Data->demandas,Demanda)
   {
      map<Disciplina*,
         pair<vector<pair<vector<pair<Demanda*,int> >,int> >, int > >::iterator

         it_Disc_Cjts_Demandas = disc_Cjts_Demandas.find(it_Demanda->disciplina);

      // Se já existe algum conjuto de demandas para a disciplina em questão.
      if(it_Disc_Cjts_Demandas != disc_Cjts_Demandas.end())
      {
         /* CONTABILIZANDO OS CONJUNTOS QUE SÃO COMPATÍVEIS COM A DEMANDA EM QUESTÃO */

         /* Estrutura que armazena os indices dos conjuntos de demandas compativeis com a
         demanda em questão. */
         vector<int/*posição de cada conjunto de demandas compativeis com a demanda em questão*/>
            indices_Demandas;

         int indice = 0;

         vector<pair<vector<pair<Demanda*,int/*Demanda específica ainda não atendida*/> >,
            int/*Somatório das demandas ainda não atendidas para um determinado conjunto*/> >::iterator

            it_Cjts_Demanda = it_Disc_Cjts_Demandas->second.first.begin();

         // Iterando sobre todos os conjutos de demandas existentes para a disc em questao
         for(; it_Cjts_Demanda != it_Disc_Cjts_Demandas->second.first.end(); it_Cjts_Demanda++)
         {
            bool demanda_Compativel = true;

            vector<pair<Demanda*,int/*Demanda específica ainda não atendida*/> >::iterator
               it_Demanda_Cjt = it_Cjts_Demanda->first.begin();

            // Iterando sobre as demandas de um conjunto existente para a disciplina em questao
            for(; it_Demanda_Cjt != it_Cjts_Demanda->first.end(); it_Demanda_Cjt++)
            {
               pair<Curso*,Curso*> cursos
                  (it_Demanda->oferta->curso,
                  it_Demanda_Cjt->first->oferta->curso);

               if(problem_Data->compat_cursos.find(cursos) == problem_Data->compat_cursos.end())
               {
                  cerr << "Compatibilidade entre cursos indefinida." << endl;
                  exit(1);
               }

               if( !(problem_Data->compat_cursos.find(cursos))->second )
               { demanda_Compativel = false; }
            }

            // Qdo o cjt for compativel, adiciono o seu indice em <indices_Demandas>
            if(demanda_Compativel)
            { indices_Demandas.push_back(indice); }

            indice++;
         }

         // ---------------------------------

         // Se nenhum conjunto de demandas é compatível com a demanda em questao, cria-se um novo conjunto
         if(indices_Demandas.empty())
         {
            // Cria-se um novo conjunto
            vector<pair<Demanda*,int> > cjt_Demandas
               (1/*Tam. Vector*/,make_pair(*it_Demanda,it_Demanda->quantidade));

            // Par responsavel por armazenar o conjunto de demandas e o somatório das demandas ainda não atendidas
            pair<vector<pair<Demanda*,int> >,int> cjt_Demandas_E_Dem_NAO_Atendida
               (cjt_Demandas,it_Demanda->quantidade);

            // Adicionando o novo conjunto
            it_Disc_Cjts_Demandas->second.first.push_back(cjt_Demandas_E_Dem_NAO_Atendida);

            // Atualizando o somatorio das demanadas
            it_Disc_Cjts_Demandas->second.second += it_Demanda->quantidade;

            ++total_Cjt_Demandas;
         }
         else
         {
            /* Se esta aqui, é pq a demanda é compativel com, pelo menos, um conjunto.
            Agora temos que determinar oq será feito com a demanada.

            Pode ser dividida para todos os conjuntos compativeis.
            Pode ser inteiramente adicionada a um conjunto especifico, sendo este, escolhido
            via algum critério.
            */

            // Escolhendo
            int cjt_Dem_Escolhido = indices_Demandas.front();
            //int cjt_Dem_Escolhido = escolheConjuntoDeDemandasParaAddNovaDemanda(
            //it_Disc_Cjts_Demandas->second.first,indices_Demandas);

            // Adicionando a nova demanda ao conjunto
            it_Disc_Cjts_Demandas->second.first.at(cjt_Dem_Escolhido).first.push_back(
               make_pair(*it_Demanda,it_Demanda->quantidade));

            // Somando o valor da nova demanda ao total de demandas do conjunto em questão
            it_Disc_Cjts_Demandas->second.first.at(cjt_Dem_Escolhido).second += it_Demanda->quantidade;

            // Somando o valor da nova demanda ao total de todas as demandas (para uma dada disciplina)
            it_Disc_Cjts_Demandas->second.second += it_Demanda->quantidade;
         }
      }
      else // Caso não exista nenhum conjunto para demandas da disciplina em questão
      {
         // Cria-se um novo conjunto
         vector<pair<Demanda*,int> > cjt_Demandas
            (1/*Tam. Vector*/,make_pair(*it_Demanda,it_Demanda->quantidade));

         // Par responsavel por armazenar o conjunto de demandas e o somatório das demandas ainda não atendidas
         pair<vector<pair<Demanda*,int> >,int> cjt_Demandas_E_Dem_NAO_Atendida
            (cjt_Demandas,it_Demanda->quantidade);

         // Estrutura que armazena os dados de todos os conjuntos de demandas para uma dada disciplina
         vector<pair<vector<pair<Demanda*,int> >,int> > cjt_Cjt_Demandas
            (1,cjt_Demandas_E_Dem_NAO_Atendida);
         
         // Inicializando
         disc_Cjts_Demandas[it_Demanda->disciplina] = make_pair(cjt_Cjt_Demandas,it_Demanda->quantidade);

         ++total_Cjt_Demandas;
      }
   }
}

int InitialSolution::escolheConjuntoDeDemandasParaAddNovaDemanda(
   //vector<pair<GGroup<Demanda*>,int> > & cjts_Demanda, vector<int> & indices_Demandas)
   vector<vector<pair<Demanda*,int/*Demanda específica ainda não atendida*/> > > & cjts_Demanda,
   vector<int> & indices_Demandas)
{
   // Por enquanto, retornando o primeiro conjunto valido
   return indices_Demandas.front();
}

//pair<Disciplina*,int* /*indice do conjunto*/> InitialSolution::escolheConjuntoDeDemadasParaAlocarASala(
pair<Disciplina*,int/*indice do conjunto*/> InitialSolution::escolheConjuntoDeDemadasParaAlocarASala(
   vector<pair<pair<Disciplina*,int/*indice do conjunto*/>, int * /*Valor da demanda*/ > > & cjts_Dem)
{
   // GULOSO

   /*Ordenando todos os elementos de <cjts_Dem>, ou seja, ordenando todos os conjuntos de demandas*/
   sort(cjts_Dem.begin(),cjts_Dem.end(),ordenaConjuntosDemandas);

   //return make_pair(cjts_Dem.front().first.first,cjts_Dem.front().second);
   return cjts_Dem.front().first;
}

void InitialSolution::carregaEstruturaCjtsDem(
   vector<pair<pair<Disciplina*,int/*indice do conjunto*/>, int * /*Valor da demanda*/ > > & cjts_Dem)
{
   // Iterador
   map<Disciplina*,
      pair<vector<pair<vector<pair<Demanda*,int/*Demanda específica ainda não atendida*/> >,
      int/*Somatório das demandas ainda não atendidas para um determinado conjunto*/> >,
      int/*Somatório das demandas ainda não atendidas de todos os conjuntos para uma dada disciplina*/ > >::iterator

      it_Disc_Cjts_Demandas = disc_Cjts_Demandas.begin();

   // Para cada disciplina
   for(; it_Disc_Cjts_Demandas != disc_Cjts_Demandas.end(); it_Disc_Cjts_Demandas++)
   {
      Disciplina * pt_Disc = it_Disc_Cjts_Demandas->first;

      int indice = 0;

      // Para cada conjunto de demandas de uma disciplina
      vector<pair<vector<pair<Demanda*,int/*Demanda específica ainda não atendida*/> >,
         int/*Somatório das demandas ainda não atendidas para um determinado conjunto*/> >::iterator

         it_Cjts_Demanda = it_Disc_Cjts_Demandas->second.first.begin();

      // Iterando sobre todos os conjutos de demandas existentes para a disc em questao
      for(; it_Cjts_Demanda != it_Disc_Cjts_Demandas->second.first.end(); it_Cjts_Demanda++)
      {
         if(it_Disc_Cjts_Demandas->second.first.front().second > 0 )
         { 
            //cjts_Dem.push_back(&it_Disc_Cjts_Demandas->second.front());
            cjts_Dem.push_back(make_pair(make_pair(pt_Disc,indice++),&it_Cjts_Demanda->second));
         }
         else
         { 
            cerr << ">> cjts_Dem.push_back(NULL) << no metodo "
               << "InitialSolution::inicializaEstruturaCjtsDem(...)" << endl;

            exit(1);

            /* Não deveria acontecer !!!

            Pode ser que na entrada, alguma disciplina não tenha demanda associada. Ou pior, 
            o algoritmo de associação de demandas está errado.
            */
         }
      }
   }
}

vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > * 
InitialSolution::getRegradeCreditoEVariacoes(Disciplina * disciplina)
{
   vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > * variacoes_Regra_de_Cred = NULL;
   
   if(disciplina->divisao_creditos)
   { variacoes_Regra_de_Cred = &disciplina->combinacao_divisao_creditos; }
   else
   {
      // Gerando uma regra de crédito
      vector<pair<int/*dia*/, int/*numCreditos*/> > regra_de_Credito;
      
      int total_Grupos_de_dois_Creditos = (disciplina->cred_praticos + disciplina->cred_teoricos) / 2;

      int creditos_Restantes = (disciplina->cred_praticos + disciplina->cred_teoricos) % 2;

      ITERA_GGROUP_N_PT(it_Dia_Let_Disc,disciplina->diasLetivos,int)
      {
         if(total_Grupos_de_dois_Creditos > 0)
         {
            regra_de_Credito.push_back(make_pair(*it_Dia_Let_Disc,2));
            total_Grupos_de_dois_Creditos--;
         }
         else if(creditos_Restantes > 0)
         {
            regra_de_Credito.push_back(make_pair(*it_Dia_Let_Disc,creditos_Restantes));
            creditos_Restantes = 0;
         }
         else
         { regra_de_Credito.push_back(make_pair(*it_Dia_Let_Disc,0)); }
      }

      // ------------------------------------

      // Inserindo a regra de credito na lista de variacoes
      variacoes_Regra_de_Cred = new vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >;
      variacoes_Regra_de_Cred->push_back(regra_de_Credito);

      // Com base na regra de crédito, gerar as possíveis variações, com base nos dias letivos da disciplina.
      int total_Variacoes = disciplina->diasLetivos.size() - 1;
      
      // Para cada possivel variacao
      for(int var = 0; var < total_Variacoes; var++)
      {
         // Copiando a ultima variacao de regra de credito utilizada
         vector<pair<int/*dia*/, int/*numCreditos*/> > variacao_Regra_de_Credito
            (variacoes_Regra_de_Cred->back());

         // Gerando a proxima variacao da regra de credito
         int creds_Primeiro_Dia = variacao_Regra_de_Credito.front().second;

         for(unsigned pos = 0; pos < (variacao_Regra_de_Credito.size()-1); pos++)
         {
            variacao_Regra_de_Credito.at(pos).second = 
               variacao_Regra_de_Credito.at(pos+1).second;
         }
         variacao_Regra_de_Credito.back().second = creds_Primeiro_Dia;

         variacoes_Regra_de_Cred->push_back(variacao_Regra_de_Credito);
      }
   }

   return variacoes_Regra_de_Cred;
}

vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >::iterator InitialSolution::salaCompativelRegraCredito(
   IS_Sala* is_Sala_Compativel, 
   vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > & variacoes_Regra_de_Cred_Esc)
{
   vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >::iterator it_Var_Regra_de_Cred =
      variacoes_Regra_de_Cred_Esc.begin();

   // Para cada variacao da regra de credito
   for(;it_Var_Regra_de_Cred != variacoes_Regra_de_Cred_Esc.end(); it_Var_Regra_de_Cred++)
   {
      bool encontrou_Variacao_Valida = true;

      vector<pair<int/*dia*/, int/*numCreditos*/> >::iterator it_Dias_Var_Regra_Cred =
         it_Var_Regra_de_Cred->begin();

      // Para cada dia considerado na variacao da regra de credito
      for(;it_Dias_Var_Regra_Cred != it_Var_Regra_de_Cred->end(); it_Dias_Var_Regra_Cred++)
      {
         // Procurando pelo dia da semana desejado
         ITERA_GGROUP(it_IS_Dia_Sem,is_Sala_Compativel->is_Dia_Semana,IS_Dia_Semana)
         {
            if(it_Dias_Var_Regra_Cred->first == it_IS_Dia_Sem->creditos_Disponiveis->dia_semana)
            {
               if(it_Dias_Var_Regra_Cred->second > it_IS_Dia_Sem->creditos_Livres)
               { 
                  encontrou_Variacao_Valida = false; 
                  break;
               }
            }
         }

         if(encontrou_Variacao_Valida == false)
         { break; }
      }

      if(encontrou_Variacao_Valida)
      {
         return it_Var_Regra_de_Cred;
      }

   }

   return variacoes_Regra_de_Cred_Esc.end();
   //return -1;
}

//void InitialSolution::aloca(pair<GGroup<Demanda*>,int> & cjt_Dem, vector<IS_Sala*> & is_Salas_Comaptiveis)
//void InitialSolution::aloca(pair<Disciplina*,int> & cjt_Dem_Esc, vector<IS_Sala*> & is_Salas_Comaptiveis)
//{
//   // Contabiliza o número de turmas abertas para o conjunto de demandas em questão
//   int num_Turmas_Abertas = 0;
//
//   Disciplina * pt_Disc = cjt_Dem_Esc.first;
//
//   /* Indica o término da alocação de uma demanda.
//
//   Pode ser pq não há mais demanda para alocar, ou pq nenhuma regra de crédito sugerida pode ser utilizada.
//   */
//   bool terminou_Alocacao = false;
//
//   // Verificando se há demanda para ser alocada
//   if(cjt_Dem_Esc.second <= 0)
//   { terminou_Alocacao = true; }
//
//   if(!terminou_Alocacao)
//   {
//      // Ordenando as IS_Salas de acordo com as suas capacidades
//      sort(is_Salas_Comaptiveis.begin(),is_Salas_Comaptiveis.end(),ordenaIS_SalasDecrescente);
//
//      // Iterando sobre as IS_Salas compativeis que, agora, estão ordenadas por capacidade
//      ITERA_VECTOR(it_IS_Salas_Compativeis,is_Salas_Comaptiveis,IS_Sala)
//      {
//         // Verificando a disponibilidade da IS_Sala para a disciplina em questão.
//         vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > *
//            regras_de_Cred_Validas = verificaDisponibilidadeIS_Sala(**it_IS_Salas_Compativeis,cjt_Dem_Esc);
//
//// -----------
//
//// TESTE !!!
//
//         pair<int,int> seg (2,1);
//         pair<int,int> ter (3,2);
//         pair<int,int> qua (4,1);
//         pair<int,int> qui (5,0);
//         pair<int,int> sex (6,1);
//
//         vector<pair<int,int> > vt_Tmp;
//         vt_Tmp.push_back(seg);
//         vt_Tmp.push_back(ter);
//         vt_Tmp.push_back(qua);
//         vt_Tmp.push_back(qui);
//         vt_Tmp.push_back(sex);
//
//         regras_de_Cred_Validas = new vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >;
//
//         regras_de_Cred_Validas->push_back(vt_Tmp);
//
//// -----------
////
//         if(regras_de_Cred_Validas) //!= NULL
//         { 
//            /* Estrutura que armazena um boleano para cada regra de credito valida.
//            Qdo a regra de credito em questão (dada pela posição no vetor) não puder mais
//            ser aplicada, esta terá a sua posição marcada como FALSE. */
//            vector<bool> posso_Aplicar_Regra_de_Credito (regras_de_Cred_Validas->size(),true);
//
//            // Escolhendo uma das regras de crédito para realizar a alocação
//            int regra_de_Credt_Esc = getRegraDeCreditoValida(posso_Aplicar_Regra_de_Credito);
//
//            while(regra_de_Credt_Esc != -1)
//            {
//               // Regra de credito a ser utilizada
//               // regras_de_Cred_Validas->at(regra_de_Credt_Esc)
//
//               /* 
//               Antes de alocar, verificar:
//
//               Se o número máximo de turmas que podem ser abertas já foi alcançado, não aloca mais.
//               */
//               if(num_Turmas_Abertas < pt_Disc->num_turmas)
//               {
//                  // Calculando a quantidade de alunos que será alocada para a turma em questão
////                  int num_Alunos_Turma = (
////                     ((*it_IS_Salas_Compativeis)->sala->capacidade >= cjt_Dem.second) ?
////                     cjt_Dem.second : (*it_IS_Salas_Compativeis)->sala->capacidade);
////
////                  // Método que atualiza o <atendimento_Tatico_AUX> da IS_Sala em questão
////                  (*it_IS_Salas_Compativeis)->aloca(
////                     num_Turmas_Abertas,
////                     cjt_Dem.first,
////                     num_Alunos_Turma,
////                     regras_de_Cred_Validas->at(regra_de_Credt_Esc));
////
////                  // Atualizando o cjt_Dem (Subtraindo da demanda total a demanda que acabou de ser alocada)
////                  cjt_Dem.second -= num_Alunos_Turma;
////                  
////                  /* Verificando ainda existem alunos para serem alocados.
////
////                     SE NÃO EXISTIREM MAIS ALUNOS PARA SEREM ALOCADOS, ENTÃO A ALOCAÇÃO
////                     FOI CONCLUÍDA.
////                  */
////                  if(cjt_Dem.second - num_Alunos_Turma == 0)
////                  { 
////                     terminou_Alocacao = true;
////                     break;
////                  }
////                  else if(cjt_Dem.second - num_Alunos_Turma < 0)
////                  {
////                     cerr << "Alocando a mais em InitialSolution::aloca(...)" << endl;
////                     exit(1);
////                  }
////
////                  //PAREI AQUI.
////
////                  // CONTINUAR IMPLEMENTANDO A ALOCACAO.
////
////                  // TEM QUE IMPLEMENTAR O METODO <verificaDisponibilidadeIS_Sala>
////
////                  ++num_Turmas_Abertas;
//               }
//               else
//               {
////                  // Como não dá mais pra alocar, forço a saída
////                  // regra_de_Credt_Esc = -1;
////
////                  // Para poder parar de procurar nas salas
////                  terminou_Alocacao = true;
////
////                  // Como não dá mais pra alocar, forço a saída
////                  break;
//               }
//
//               // Escolhendo uma das regras de crédito para realizar a alocação
//               regra_de_Credt_Esc = getRegraDeCreditoValida(posso_Aplicar_Regra_de_Credito);
//            } 
//
//            // Verificando se a alocação já foi concluída
//            if(terminou_Alocacao)
//            { break; }
//
//            // Lembrar de atualizar a estrutura <disc_Cjts_Demandas> e tb a IS_Sala
//         }
//         else
//         {
//            cout << "NAO VOU ALOCAR PQ NAO CONSEGUI ACHAR NENHUMA REGRA DE CREDITO QUE FOSSE VIAVEL!!!" << endl;
//            getchar();
//         }
////
////
////         // Se aloquei toda a demanda, paro
////         //if(alocou_Toda_Demanda)
////         //{ break; }
////
////         //getchar();
//      }
//   }
//
//
//   getchar();
//
//
//   // Pode ser que não consiga alocar em nenhuma IS_Sala. Por eqto, estou deixando pra lá.
//   //if(!alocou)
//   //{ 
//   //   cout << "Nao consegui alocar a demanda em questão. IGNORANDO" << endl;
//  //    getchar();
//  // }
//}








void InitialSolution::aloca(pair<Disciplina*,int/*indice do conjunto*/> & cjt_Dem_Esc,
                            vector<IS_Sala*> & is_Salas_Comaptiveis)
{
   // Contabiliza o número de turmas abertas para a disciplina do conjunto de demandas em questão
   int num_Turmas_Abertas = 0;

   Disciplina * pt_Disc = cjt_Dem_Esc.first;

   /* Indica o término da alocação de uma demanda.

   Pode ser pq não há mais demanda para alocar, ou pq nenhuma regra de crédito sugerida pode ser utilizada.
   */
   bool terminou_Alocacao = false;

   // Verificando se há demanda para ser alocada
   if( disc_Cjts_Demandas[pt_Disc].second <= 0)
   { terminou_Alocacao = true; }

   // Ordenando as IS_Salas de acordo com as suas capacidades
   sort(is_Salas_Comaptiveis.begin(),is_Salas_Comaptiveis.end(),ordenaIS_SalasDecrescente);

   while(!terminou_Alocacao)
   {
      bool alocou_Em_Alguma_Sala = false;

      /* 
      Antes de alocar, verificar:

      Se o número máximo de turmas que podem ser abertas já foi alcançado, não aloca mais.
      */
      if(num_Turmas_Abertas < pt_Disc->num_turmas)
      {
         // Escolhendo a sala para tentar alocar

         /* Iterando sobre as IS_Salas compativeis (que, agora, estão ordenadas por capacidade)
         até que alguma sala disponível seja encontrada. Isto é, estamos procurando por uma sala
         que possua a quantidade de créditos livres demandados pela disciplina para cada dia especificado. 

         Caso, não encontre, não aloco nada. IDEIA: Poderiamos sugerir outras regras de credito semelhantes
         à regra de crédito em questão e tentar aplicá-las.
         */
         ITERA_VECTOR(it_IS_Salas_Compativeis,is_Salas_Comaptiveis,IS_Sala)
         {
            /* Obtendo alguma regra de crédito para a disciplina em questão.

            Pode ser que a disciplina possua uma regra de crédito especificada na entrada. Caso 
            isso não aconteça, uma regra de crédito será proposta.

            Lembrando que uma regra de crédito não é fixa em relação aos dias. Por isso
            temos uma coleção de regras de crédito abaixo. Tratam-se da mesma regra de crédito, porém,
            cada uma especifica os dias em que os créditos devem ser ministrados.
            */
            vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > * 
               variacoes_Regra_de_Cred_Esc = getRegradeCreditoEVariacoes(pt_Disc);

            // Checando se a sala em questão é compatível com alguma das variações da regra de crédito em questão.
            //int regra_de_Credt_Esc = 
            vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >::iterator
               it_Regra_de_Credt_Esc =
               salaCompativelRegraCredito(*it_IS_Salas_Compativeis,*variacoes_Regra_de_Cred_Esc);

            if(it_Regra_de_Credt_Esc != variacoes_Regra_de_Cred_Esc->end())
            {
               // Se esta aqui é pq achamos uma sala para realizar a alocação

               //Calculando a quantidade de alunos que será alocada para a turma em questão
               int num_Alunos_Turma = (
                  (*it_IS_Salas_Compativeis)->sala->capacidade >= disc_Cjts_Demandas[pt_Disc].second ?
                  disc_Cjts_Demandas[pt_Disc].second : (*it_IS_Salas_Compativeis)->sala->capacidade);

               // Configurando uma turma para o conjunto de demandas em questão.
               vector<pair<Demanda*,int> > demandas_A_Alocar = 
                  elaboraTurma(cjt_Dem_Esc,num_Alunos_Turma);

               (*it_IS_Salas_Compativeis)->aloca(
                  num_Turmas_Abertas,
                  cjt_Dem_Esc,
                  demandas_A_Alocar,
                  num_Alunos_Turma,
                  *it_Regra_de_Credt_Esc);
                  //variacoes_Regra_de_Cred_Esc->at(regra_de_Credt_Esc));

               // Atualizando a estrutura <disc_Cjts_Demandas>
               map<Disciplina*,
                  pair<vector<pair<vector<pair<Demanda*,int/*Demanda específica ainda não atendida*/> >,
                  int/*Somatório das demandas ainda não atendidas para um determinado conjunto*/> >,
                  int/*Somatório das demandas ainda não atendidas de todos os conjuntos para uma dada disciplina*/ > >
                  ::iterator

                  it_Disc_Cjts_Demandas = disc_Cjts_Demandas.find(pt_Disc);

               if(it_Disc_Cjts_Demandas != disc_Cjts_Demandas.end())
               {
                  int demanda_Total_Alocada = 0;

                  // Iterador das demandas alocadas
                  vector<pair<Demanda*,int> >::iterator
                     it_Demandas_A_Alocar = demandas_A_Alocar.begin();

                  // P/ cada demanda que foi alocada
                  for(; it_Demandas_A_Alocar != demandas_A_Alocar.end(); it_Demandas_A_Alocar++)
                  {
                     // Iterador das demandas de um conjunto específico para uma dada disciplina
                     vector<pair<Demanda*,int/*Demanda específica ainda não atendida*/> >::iterator
                        it_Dems_Cjt = it_Disc_Cjts_Demandas->second.first.at(cjt_Dem_Esc.second).first.begin();

                     // Procurando em <disc_Cjts_Demandas> a demanda acima
                     for(; it_Dems_Cjt != it_Disc_Cjts_Demandas->second.first.at(cjt_Dem_Esc.second).first.end(); it_Dems_Cjt++)
                     {
                        if(it_Dems_Cjt->first->getId() == it_Demandas_A_Alocar->first->getId())
                        {
                           it_Dems_Cjt->second -= it_Demandas_A_Alocar->second;

                           demanda_Total_Alocada += it_Demandas_A_Alocar->second;

                           break;
                        }
                     }
                  }

                  // Atualizando o somatório das demandas ainda não atendidas para um determinado conjunto
                  it_Disc_Cjts_Demandas->second.first.at(cjt_Dem_Esc.second).second -= demanda_Total_Alocada;

                  // Atualizando o somatório das demandas ainda não atendidas de todos os conjuntos para uma dada disciplina
                  it_Disc_Cjts_Demandas->second.second -= demanda_Total_Alocada;

               }
               else
               {
                  cerr << "Erro em InitialSolution::aloca(...)" << endl;
                  exit(1);
               }

               // Contabilizando o total de turmas abertas
               ++num_Turmas_Abertas;

               alocou_Em_Alguma_Sala = true;

               break;
            }
         }
      }
      else
      {
         terminou_Alocacao = true;

         cout << "Numero maximo de turmas alcançado" << endl;
         cout << "Nao aloquei a demanda toda" << endl;
      }

      /* Se não consigo mais alocar em nenhuma sala ou se 
      toda a demanda já foi alocada, paro.
      */
      if(!alocou_Em_Alguma_Sala || disc_Cjts_Demandas[pt_Disc].second <= 0)
      { terminou_Alocacao = true; }
   }
}








// 04/01/2011 - ABAIXO

//void InitialSolution::aloca(pair<Disciplina*,int/*indice do conjunto*/> & cjt_Dem_Esc,
//                            vector<IS_Sala*> & is_Salas_Comaptiveis)
//{
//   // Contabiliza o número de turmas abertas para o conjunto de demandas em questão
//   int num_Turmas_Abertas = 0;
//
//   Disciplina * pt_Disc = cjt_Dem_Esc.first;
//
//   /* Indica o término da alocação de uma demanda.
//
//   Pode ser pq não há mais demanda para alocar, ou pq nenhuma regra de crédito sugerida pode ser utilizada.
//   */
//   bool terminou_Alocacao = false;
//
//   // Verificando se há demanda para ser alocada
//   if( disc_Cjts_Demandas[pt_Disc].second <= 0)
//   { terminou_Alocacao = true; }
//
//   if(!terminou_Alocacao)
//   {
//      // Ordenando as IS_Salas de acordo com as suas capacidades
//      sort(is_Salas_Comaptiveis.begin(),is_Salas_Comaptiveis.end(),ordenaIS_SalasDecrescente);
//
//     // Iterando sobre as IS_Salas compativeis que, agora, estão ordenadas por capacidade
//     ITERA_VECTOR(it_IS_Salas_Compativeis,is_Salas_Comaptiveis,IS_Sala)
//     {
//        // Verificando a disponibilidade da IS_Sala para a disciplina em questão.
//        vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > *
//           regras_de_Cred_Validas = verificaDisponibilidadeIS_Sala(**it_IS_Salas_Compativeis,cjt_Dem_Esc);
//
//// -----------
//
//// TESTE !!!
//
//         pair<int,int> seg (2,1);
//         pair<int,int> ter (3,2);
//         pair<int,int> qua (4,1);
//         pair<int,int> qui (5,0);
//         pair<int,int> sex (6,1);
//
//         vector<pair<int,int> > vt_Tmp;
//         vt_Tmp.push_back(seg);
//         vt_Tmp.push_back(ter);
//         vt_Tmp.push_back(qua);
//         vt_Tmp.push_back(qui);
//         vt_Tmp.push_back(sex);
//
//         regras_de_Cred_Validas = new vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >;
//
//         regras_de_Cred_Validas->push_back(vt_Tmp);
//
//// -----------
//
//         if(regras_de_Cred_Validas) //!= NULL
//         { 
//            /* Estrutura que armazena um boleano para cada regra de credito valida.
//            Qdo a regra de credito em questão (dada pela posição no vetor) não puder mais
//            ser aplicada, esta terá a sua posição marcada como FALSE. */
//            vector<bool> posso_Aplicar_Regra_de_Credito (regras_de_Cred_Validas->size(),true);
//
//            // Escolhendo uma das regras de crédito para realizar a alocação
//            int regra_de_Credt_Esc = getRegraDeCreditoValida(posso_Aplicar_Regra_de_Credito);
//
//            // Eqto existir regra de credito aplicavel e demanda para alocar
//            while((regra_de_Credt_Esc != -1) && (disc_Cjts_Demandas[pt_Disc].second > 0) )
//            {
//               /* 
//               Antes de alocar, verificar:
//
//               Se o número máximo de turmas que podem ser abertas já foi alcançado, não aloca mais.
//               */
//               if(num_Turmas_Abertas < pt_Disc->num_turmas)
//               {
//
//                  //Calculando a quantidade de alunos que será alocada para a turma em questão
//                  int num_Alunos_Turma = (
//                     (*it_IS_Salas_Compativeis)->sala->capacidade >= disc_Cjts_Demandas[pt_Disc].second ?
//                     disc_Cjts_Demandas[pt_Disc].second : (*it_IS_Salas_Compativeis)->sala->capacidade);
//
//                  // Configurando um turma para conjunto de demandas em questão.
//                  vector<pair<Demanda*,int> > demandas_A_Alocar = 
//                     elaboraTurma(cjt_Dem_Esc,num_Alunos_Turma);
//
//                  (*it_IS_Salas_Compativeis)->aloca(
//                     num_Turmas_Abertas,
//                     cjt_Dem_Esc,
//                     demandas_A_Alocar,
//                     num_Alunos_Turma,
//                     regras_de_Cred_Validas->at(regra_de_Credt_Esc));
//
//                  // Atualizando a estrutura <disc_Cjts_Demandas>
//                  map<Disciplina*,
//                     pair<vector<pair<vector<pair<Demanda*,int/*Demanda específica ainda não atendida*/> >,
//                     int/*Somatório das demandas ainda não atendidas para um determinado conjunto*/> >,
//                     int/*Somatório das demandas ainda não atendidas de todos os conjuntos para uma dada disciplina*/ > >
//                     ::iterator
//
//                     it_Disc_Cjts_Demandas = disc_Cjts_Demandas.find(pt_Disc);
//
//                  if(it_Disc_Cjts_Demandas != disc_Cjts_Demandas.end())
//                  {
//                     int demanda_Total_Alocada = 0;
//                   
//                     // Iterador das demandas alocadas
//                     vector<pair<Demanda*,int> >::iterator
//                        it_Demandas_A_Alocar = demandas_A_Alocar.begin();
//
//                     // P/ cada demanda que foi alocada
//                     for(; it_Demandas_A_Alocar != demandas_A_Alocar.end(); it_Demandas_A_Alocar++)
//                     {
//                        // Iterador das demandas de um conjunto específico para uma dada disciplina
//                        vector<pair<Demanda*,int/*Demanda específica ainda não atendida*/> >::iterator
//                           it_Dems_Cjt = it_Disc_Cjts_Demandas->second.first.at(cjt_Dem_Esc.second).first.begin();
//
//                        // Procurando em <disc_Cjts_Demandas> a demanda acima
//                        for(; it_Dems_Cjt != it_Disc_Cjts_Demandas->second.first.at(cjt_Dem_Esc.second).first.end(); it_Dems_Cjt++)
//                        {
//                           if(it_Dems_Cjt->first->getId() == it_Demandas_A_Alocar->first->getId())
//                           {
//                              it_Dems_Cjt->second -= it_Demandas_A_Alocar->second;
//
//                              demanda_Total_Alocada += it_Demandas_A_Alocar->second;
//
//                              break;
//                           }
//                        }
//                     }
//
//                     // Atualizando o somatório das demandas ainda não atendidas para um determinado conjunto
//                     it_Disc_Cjts_Demandas->second.first.at(cjt_Dem_Esc.second).second -= demanda_Total_Alocada;
//
//                     // Atualizando o somatório das demandas ainda não atendidas de todos os conjuntos para uma dada disciplina
//                     it_Disc_Cjts_Demandas->second.second -= demanda_Total_Alocada;
//
//                  }
//                  else
//                  {
//                     cerr << "Erro em InitialSolution::aloca(...)" << endl;
//                     exit(1);
//                  }
//
//                  // Contabilizando o total de turmas abertas
//                  ++num_Turmas_Abertas;
//               }
//               else
//               {
//                  // Para poder parar de procurar nas salas
//                  terminou_Alocacao = true;
//
//                  // Como não dá mais pra alocar, forço a saída
//                  break;
//               }
//
//               // Escolhendo uma das regras de crédito para realizar a alocação
//               regra_de_Credt_Esc = getRegraDeCreditoValida(posso_Aplicar_Regra_de_Credito);
//            } 
//
//            // Verificando se a alocação já foi concluída
//            if(terminou_Alocacao)
//            { break; }
//
//         }
//         else
//         {
//            cout << "NAO VOU ALOCAR PQ NAO CONSEGUI ACHAR NENHUMA REGRA DE CREDITO QUE FOSSE VIAVEL!!!" << endl;
//            getchar();
//         }
//      }
//   }
//
//
//   getchar();
//
//
//   // Pode ser que não consiga alocar em nenhuma IS_Sala. Por eqto, estou deixando pra lá.
//   //if(!alocou)
//   //{ 
//   //   cout << "Nao consegui alocar a demanda em questão. IGNORANDO" << endl;
//  //    getchar();
//  // }
//}


IS_Sala * InitialSolution::escolheIS_Sala(vector<IS_Sala*> & is_Salas_Comaptiveis)
{
   sort(is_Salas_Comaptiveis.begin(),is_Salas_Comaptiveis.end(),ordenaIS_SalasDecrescente);

   IS_Sala * is_Sala_Esc = is_Salas_Comaptiveis.front();

   is_Salas_Comaptiveis.erase(is_Salas_Comaptiveis.begin());

   return is_Sala_Esc;
}

vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > * 
//InitialSolution::verificaDisponibilidadeIS_Sala(IS_Sala & is_sala, pair<GGroup<Demanda*>,int> & cjt_Dem)
//InitialSolution::verificaDisponibilidadeIS_Sala(IS_Sala & is_sala, pair<Disciplina*,int*> & cjt_Dem_Esc)
InitialSolution::verificaDisponibilidadeIS_Sala(IS_Sala & is_sala, 
                                                pair<Disciplina*,int/*indice do conjunto*/> & cjt_Dem_Esc)
{
   cout << "IMPLEMENTAR !!" << endl;

      //// Estrutura para armazenar os creditos livres da IS_Sala escolhida.
      //vector<pair<int/*dia*/,int/*numCredts*/> > creds_Livres_Por_Dia_IS_Sala;

      //map<int/*dia*/,pair<int/*credsLivres*/,
      //   vector<
      //   pair<Demanda*,
      //   pair<int/*Id Turma*/,int/*Demanda Atendida*/> > > > >::iterator

      //   it_At_Tatico = is_Sala_Esc->atendimento_Tatico.begin();


      //// Armazenando a quantidade de creditos livres da IS_Sala escolhida.
      //for(; it_At_Tatico != is_Sala_Esc->atendimento_Tatico.end(); it_At_Tatico++)
      //{
      //   creds_Livres_Por_Dia_IS_Sala.push_back(
      //      make_pair(it_At_Tatico->first,it_At_Tatico->second.first));
      //}

      //Disciplina * pt_Disc = cjt_Dem.first.begin()->disciplina;

      //if(pt_Disc->divisao_creditos) // != NULL
      //{

      //}
      //else // == NULL
      //{
      //   cout << "IMPLEMENTAR !!!" << endl;
      //   exit(1);
      //}

   return NULL;
}


int InitialSolution::getRegraDeCreditoValida(vector<bool> & regras_de_Credito)
//int InitialSolution::getRegraDeCreditoValida(
//   vector<bool> & regras_de_Credito,
//   vector<IS_Sala*> & is_Salas_Comaptiveis)
{
   for(unsigned i=0;i<regras_de_Credito.size();i++)
   {
      if(regras_de_Credito.at(i))
         return i;
   }
   return -1;
}

vector<pair<Demanda*,int/*Demanda atendida*/> > InitialSolution::elaboraTurma(
   pair<Disciplina*,int> & cjt_Dem, int demanda_a_Ser_Atendida)
{
   vector<pair<Demanda*,int/*Demanda atendida*/> > qtd_Dem_Atd_Cjt;

   if(disc_Cjts_Demandas.find(cjt_Dem.first) != disc_Cjts_Demandas.end())
   {
      pair<vector<pair<Demanda*,int> >,int> * pt_Cjt_Dem =
         &(disc_Cjts_Demandas[cjt_Dem.first].first.at(cjt_Dem.second));

      // Demanda total do conjunto de demandas
      int demanda_Total = pt_Cjt_Dem->second;

      if(demanda_Total == demanda_a_Ser_Atendida)
      {
         // Adiciono todas as demandas do conjunto à estrutura
         vector<pair<Demanda*,int> >::iterator it_Demandas =
            pt_Cjt_Dem->first.begin();

         for(; it_Demandas != pt_Cjt_Dem->first.end(); it_Demandas++)
         { qtd_Dem_Atd_Cjt.push_back(*it_Demandas); }
      }
      else
      {
         /* HEURISTICA GULOSA 

         Vou coletando todas as demandas até que o somatório delas seja suficiente.
         */

         // Adiciono todas as demandas do conjunto à estrutura
         vector<pair<Demanda*,int> >::iterator it_Demandas = pt_Cjt_Dem->first.begin();

         for(; it_Demandas != pt_Cjt_Dem->first.end(); it_Demandas++)
         { 
            int demanda_Corrente = it_Demandas->second;

            if(demanda_a_Ser_Atendida == demanda_Corrente)
            { 
               qtd_Dem_Atd_Cjt.push_back(*it_Demandas);
               break;
            }
            else if(demanda_a_Ser_Atendida > demanda_Corrente)
            {
               // Atualizando a demanda a ser atendida
               demanda_a_Ser_Atendida -= demanda_Corrente;

               qtd_Dem_Atd_Cjt.push_back(*it_Demandas);
            }
            else if(demanda_a_Ser_Atendida < demanda_Corrente)
            {
               qtd_Dem_Atd_Cjt.push_back(*it_Demandas);
               qtd_Dem_Atd_Cjt.back().second = demanda_a_Ser_Atendida;
               break;
            }
            else if(demanda_a_Ser_Atendida == 0)
            { break; }
         }
      }
   }
   else
   { cerr << "Tentando acessar uma disciplina que nao existe !! InitialSolution::elaboraTurma(...)" << endl; exit(1); }

   return qtd_Dem_Atd_Cjt;
}

ProblemData & InitialSolution::get_Problem_Data() const
{
   return *problem_Data;
}

GGroup<IS_Campus*> const & InitialSolution::getInitialSolution() const
{
   return solucao;
}

int InitialSolution::getNumDemandasAtendidas() const
{
   return num_Demandas_Atendidas;
}

int InitialSolution::getNumDemandas_NAO_Atendidas() const
{
   return num_Demandas_NAO_Atendidas;
}

void InitialSolution::generate_Initial_Solution()
{
   cout << "Gerando uma solucao inicial" << endl;

   // ---------------------------------

   // Iterador
   map<Disciplina*,vector<pair<GGroup<Demanda*>,int/*Demanda ainda não atendida*/> > >::iterator
      it_Disc_Cjts_Demandas;

   // ---------------------------------

   // Etrutura para armazenar referências entre as <Sala> e <IS_Sala>.
   map<Sala*,IS_Sala*> map_Sala__IS_Sala;

   // Gerando referências entre as <Sala> e <IS_Sala>.
   ITERA_GGROUP(it_IS_Campus,solucao,IS_Campus)
   {
      ITERA_GGROUP(it_IS_Unidade,it_IS_Campus->unidades,IS_Unidade)
      {
         ITERA_GGROUP(it_IS_Sala,it_IS_Unidade->salas,IS_Sala)
         {
            map_Sala__IS_Sala[it_IS_Sala->sala] = *it_IS_Sala;
         }
      }
   }

   // ---------------------------------

   // Alocando as demandas às salas

   /* PASSO 1

   Referencio todos os conjuntos de demandas de cada disciplina.
   Agora, basta ordenar esse conjunto. Sempre que uma demanda seja atendida, seja parcialmente
   ou totalmente, uma reordenação será realizada.

   Obs: Caso um conjunto de demandas de uma disciplina já tenha sido totalmente atendido,
   o ponteiro para o mesmo será inicializado com NULL. Ao ordenar a estrutura, os 
   ponteiros NULL deverão ficar ao final da mesma, sendo desconsiderados.

   EXEMPLO:
   Considere o caso em que 3 disciplinas possuem os seguintes conjuntos de demandas (já ordenados para cada disc):
   
   Disc 1 -> [dem:80] ; [dem:45]
   Disc 2 -> [dem:50] ; [dem:47]
   Disc 3 -> [dem:30]

   A estrutura <cjts_Dem> inicialmente ficaria assim:

   [ [dem:80], [dem:50], [dem:47], [dem:45], [dem:30] ]

   Digamos que a primeira demanda seja totalmente atendida.

   -> [ [dem:50], [dem:47], [dem:45], [dem:30], NULL ]

   Agora, que a nova primeira demanda seja atendida parcialmente em 15
   
   [dem:50] <- [dem:50] - 15 => [dem:35]

   -> [ [dem:47], [dem:45], [dem:50], [dem:30], NULL ]

   E assim por diante, até que todos os ponteiros sejam NULL. Seja pq todas as demandas foram atendidas,
   ou pq não há mais como alocar.

   */

   // ---

   /* 
      Estrutura que armazena ponteiros para todos os conjuntos de demandas de cada disciplina. 
      Obs: Posso descobrir a disciplina via a <Demanda*>.
   */
   //vector<pair<pair<Disciplina*,int/*indice do conjunto*/>, int/*Valor da demanda*/ > > cjts_Dem;
   vector<pair<pair<Disciplina*,int/*indice do conjunto*/>, int * /*Valor da demanda*/ > > cjts_Dem;

   // Inicializando a estrutura
   carregaEstruturaCjtsDem(cjts_Dem);

   // ----

   // Para cada conjunto de demandas a ser atendido
   for(int cnt_Cjt_Dems = 0; cnt_Cjt_Dems < total_Cjt_Demandas; cnt_Cjt_Dems++)
   {
      // Escolhendo o conjunto de demandas a ser atendido
      //pair<GGroup<Demanda*>,int> * cjt_Demanda_Escolhido =
      //pair<Disciplina*,int* /*Valor da demanda*/> cjt_Demanda_Escolhido =
      pair<Disciplina*,int/*indice do conjunto*/> cjt_Demanda_Escolhido =
         escolheConjuntoDeDemadasParaAlocarASala(cjts_Dem);

      /* PASSO 2

      De posse da estrutura <cjts_Dem> ordenada, agora tentaremos alocar a maior
      demanda à sala disponível com maior capacidade que seja compativel com a 
      disciplina em questão.

      ATENÇÃO: a sala tem que ser compatível com a disciplina do conjunto de demandas
      em questão. Ou seja, possuir capacidade maior ou igual à demanda total e possuir os
      mesmos dias letivos. Utilizar a estrutura <disc_IS_Salas> instanciada e inicializada acima.

      OBS: Deve-se tentar respeitar as regras de crédito de cada disciplina.

      Devo ordenar as salas por capacidade
      Em seguida:
      Até que todas as salas compatíveis tenha sido percorridas:
      Seleciono uma sala e tento alocar
      Se não consegui alocar em nenhuma sala é pq não há nenhuma sala
      com os horarios disponíveis. Pode-se deixar sem alocar, ou fragmentar os
      creditos da disciplina para tentar alocar.

      */
     
      // Criando um vetor de IS_Salas compatíveis com o conjunto de demandas em questão.
      vector<IS_Sala*> is_Salas_Comaptiveis;
      
      Disciplina * pt_Disc = cjt_Demanda_Escolhido.first;

      // Inicializando o vetor <is_Salas_Comaptiveis>
      ITERA_GGROUP(it_Sala,problem_Data->discSalas.find(pt_Disc->getId())->second,Sala)
      { is_Salas_Comaptiveis.push_back(map_Sala__IS_Sala.find(*it_Sala)->second); }

      aloca(cjt_Demanda_Escolhido,is_Salas_Comaptiveis);

      //getchar();
   }
}










//void InitialSolution::generate_Initial_Solution()
//{
//   // Etrutura para armazenar referências entre as <Sala> e <IS_Sala>.
//   map<Sala*,IS_Sala*> map_Sala__IS_Sala;
//
//   // Gerando referências entre as <Sala> e <IS_Sala>.
//   ITERA_GGROUP(it_IS_Campus,solucao,IS_Campus)
//   {
//      ITERA_GGROUP(it_IS_Unidade,it_IS_Campus->unidades,IS_Unidade)
//      {
//         ITERA_GGROUP(it_IS_Sala,it_IS_Unidade->salas,IS_Sala)
//         {
//            map_Sala__IS_Sala[it_IS_Sala->sala] = *it_IS_Sala;
//         }
//      }
//   }
//
//   // ---
//
//   // Criando uma estrutura que copie as salas e ordene-as de acordo com a respectiva capacidade
//   std::map<int/*Id Disc*/,vector<Sala*> > disc_Salas;
//
//   std::map<int/*Id Disc*/,GGroup<Sala*> >::iterator it_Disc_Salas =
//      problem_Data->discSalas.begin();
//
//   for(; it_Disc_Salas != problem_Data->discSalas.end(); it_Disc_Salas++)
//   { 
//      ITERA_GGROUP(it_Sala,it_Disc_Salas->second,Sala)
//      {
//         disc_Salas[it_Disc_Salas->first].push_back(*it_Sala);
//      }
//   }
//
//   // Ordenando as salas por capacidade, para cada disciplina
//   std::map<int/*Id Disc*/,vector<Sala*> >::iterator it_Disc_VT_Salas =
//      disc_Salas.begin();
//
//   for(; it_Disc_VT_Salas != disc_Salas.end(); it_Disc_VT_Salas++)
//   { sort(it_Disc_VT_Salas->second.begin(),it_Disc_VT_Salas->second.end(),ordena_dec_sala); }
//
//   // ---
//
//   // Criando uma estrutura que conta o número de turmas geradas para cada disciplina
//   std::map<Disciplina*,int/*num turmas abertas*/> disc_Turmas;
//
//   // Inicializando
//   ITERA_GGROUP(it_Disc,problem_Data->disciplinas,Disciplina)
//   { disc_Turmas[*it_Disc] = 0; }
//
//   // ---
//
//   // Tentando alocar todas as demandas. Pode ser que parte de alguma (ou toda a) demanda não seja alocada
//   while(!tentouAtenderTodasDemandas())
//   {
//      vector<pair<Demanda*,int> >::iterator it_vt_Demandas =
//         vt_Demandas.begin();
//
//      for(; it_vt_Demandas != vt_Demandas.end(); it_vt_Demandas++)
//      {
//         // Se a demanda ainda não foi atendida
//         if(it_vt_Demandas->second > 0)
//         {
//            Disciplina * pt_Disc = it_vt_Demandas->first->disciplina;
//
//            // um dos 2 vai ser sempre 0.
//            int total_Creditos = pt_Disc->cred_praticos + pt_Disc->cred_teoricos;
//
//            /* Utilizada para indicar se uma demanda foi atendida, pelo menos em parte, em uma iteração
//            das salas compatíveis. Se, em uma iteração dessas a demanda não for atendida (em pelo menos,
//            o tamanho da menor sala) significa que não há salas com horários vagos suficientes para podermos
//            alocar a demanda. 
//
//            -------
//
//            Dá pra dividir a demanda e alocar separado por sala. Fica pra um TRIEDA FUTURO !!!
//
//            */
//            bool alocou_Demanda = false;
//
//            // p tds salas que possuem a disc associada
//            ITERA_VECTOR(it_Disc_Salas,disc_Salas[pt_Disc->getId()],Sala)
//            {
//               // Selecionando a IS_Sala correspondente à Sala em questão.
//               IS_Sala * is_Sala = map_Sala__IS_Sala[*it_Disc_Salas];
//
//               pair<int/*idDisc*/,int/*idSala*/> ids_Disc_Sala 
//                  (pt_Disc->getId(),is_Sala->getId());
//
//               // p tds os dias letivos comuns entre a disc e a sala selecionadas
//               ITERA_GGROUP_N_PT(it_Dia_Let_Disc_Sala,problem_Data->disc_Salas_Dias[ids_Disc_Sala],int)
//               {
//                  /* Se a sala em questao possui, no mínimo, tantos horarios (creditos) vagos quanto o total
//                  de créditos da disciplina em questão. */
//                  if(is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].first >= total_Creditos
//                     && (disc_Turmas[pt_Disc] < pt_Disc->num_turmas)
//                     && (is_Sala->sala->capacidade >= it_vt_Demandas->first->quantidade)
//                     )
//                  {
//                     alocou_Demanda = true;
//
//                     int demanda_Atendida =
//                        (it_vt_Demandas->second <= is_Sala->sala->capacidade ? // Ou seja, cabe na sala?
//                        it_vt_Demandas->second : -1);
//
//                     demanda_Atendida = (demanda_Atendida == -1 ? 
//                        (it_vt_Demandas->second - (it_vt_Demandas->second % is_Sala->sala->capacidade))
//                        : demanda_Atendida );
//
//                     // === ===
//
//                     // Para cada credito a ser atendido
//                     for(int cred = 0; cred < total_Creditos; cred++)
//                     {
//                        // Encontrando a posicao correta para inserir
//                        int pos = is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].second.size()
//                           - is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].first;
//
//                        // Subtraindo o credito alocado dos creditos livres.
//                        is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].first--;
//
//                        // Referenciando a demanda que está sendo parcialmente, ou totalmente atendida.
//                        is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].second.at(pos).first = 
//                           it_vt_Demandas->first;
//
//                        // Armazenando a demanda (parcial ou total) atendida
//                        is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].second.at(pos).second =
//                           make_pair(disc_Turmas[pt_Disc],demanda_Atendida);
//                     }              
//
//                     /* Teste para o caso em que toda a demanda foi atendida */
//                     if( (it_vt_Demandas->second - demanda_Atendida) == 0 )
//                     {
//                        // Se está aqui, é pq toda a demanda foi atendida
//                        num_Demandas_Atendidas++;
//
//                        // Para não atender denovo
//                        it_vt_Demandas->second = 0;
//                     }
//                     else
//                     {
//                        // Se está aqui, é pq NEM toda a demanda foi atendida
//
//                        // Atualizando a demanda que ainda falta ser atendida
//                        it_vt_Demandas->second -= demanda_Atendida;
//                     }
//
//                     // Reordenando as demandas em ordem decrescente
//                     sort(vt_Demandas.begin(),vt_Demandas.end(),ordena_dec_demanda);
//
//                     // Adicionando o indice da turma
//                     ++disc_Turmas[pt_Disc];
//
//                     break;
//                  }
//
//               }
//
//               if(alocou_Demanda)
//               { break; }
//            }
//
//            /* Caso a demanda não tenha sido atendida, devido ao fato de não existirem horários
//            vagos suficientes em uma dada sala. */
//            if(!alocou_Demanda)
//            {
//               it_vt_Demandas->second = 0;
//               num_Demandas_NAO_Atendidas++;
//            }
//         }
//      }
//   }
//}


pair<vector<int>*,vector<double>*> InitialSolution::repSolIniToVariaveis(VariableHash & v_Hash, int lp_Cols, OPT_LP & lp)
{
   vector<int> * indices = new vector<int>;

   vector<double> * valores = new vector<double>;

//   /* Responsavel por armazenar os indices das vars do tipo V_CREDITOS que tiveram
//   valor atribuido no processo de geração da solução inicial. */
//   set<int> indices_V_CREDITOS;
//
//   /* Responsavel por armazenar os indices das vars do tipo V_OFERECIMENTO que tiveram
//   valor atribuido no processo de geração da solução inicial. */
//   set<int> indices_V_OFERECIMENTO;
//
//   /* Responsavel por armazenar os indices das vars do tipo V_ALUNOS que tiveram
//   valor atribuido no processo de geração da solução inicial. */
//   set<int> indices_V_ALUNOS;
//
//   /* Responsavel por armazenar os indices das vars do tipo V_ALOC_ALUNO que tiveram
//   valor atribuido no processo de geração da solução inicial. */
//   set<int> indices_V_ALOC_ALUNO;
//
//   int cnt = 0;
//
//   // p/ cada campus
//   ITERA_GGROUP(it_IS_Campus,solucao,IS_Campus)
//   {
//      // Armazena os indices e valores das variaveis do tipo V_ALUNOS
//      map<vector<int/*indices de uma var do tipo V_ALUNOS*/>,
//         pair<Variable,int/*valor var*/> > vars_Alunos;
//
//      // Armazena os indices e valores das variaveis do tipo V_ALOC_ALUNO
//      map<vector<int/*indices de uma var do tipo V_ALOC_ALUNO*/>,
//         pair<Variable,int/*valor var*/> > vars_Aloc_Alunos;
//
//      // p/ cada unidade
//      ITERA_GGROUP(it_IS_Unidade,it_IS_Campus->unidades,IS_Unidade)
//      {
//         // p/ cada sala
//         ITERA_GGROUP(it_IS_Sala,it_IS_Unidade->salas,IS_Sala)
//         {
//            map<int/*dia*/,pair<int/*credsLivres*/,
//               vector<
//               pair<Demanda*,
//               pair<int/*Id Turma*/,int/*Demanda Atendida*/> > > > >::iterator
//
//               it_Dia = it_IS_Sala->atendimento_Tatico.begin();
//
//            // p/ cada dia do atendimento tatico
//            for(; it_Dia != it_IS_Sala->atendimento_Tatico.end(); it_Dia++)
//            {
//               /* Armazena os indices e valores das variaveis do tipo V_CREDITOS 
//
//               Como existe uma variavel do tipo V_OFERECIMENTO para cada variavel
//               do tipo V_CREDITOS, usam-se os mesmos indices armazenados nessa estrutura
//               para inicializar as variaveis do tipo V_OFERECIMENTO
//               */
//               map<vector<int/*indices de uma var do tipo V_CREDITOS ou V_OFERECIMENTO*/>,
//                  pair<Variable,int/*valor var*/> > vars_Dia_Cred_OU_Oferc;
//
//               // p/ cada credito do dia 
//               for(unsigned cred = 0; cred < it_Dia->second.second.size(); cred++)
//               {
//                  // SOMENTE se o cred tiver sido alocado
//                  if(it_Dia->second.second.at(cred).first) // != NULL
//                  {
//                     // Coletando os dados das variaveis
//
//                     // ===
//
//                     // Criando as variáveis e estruturas necessárias
//
//                     Variable var_Cred_OU_Oferc;
//                     vector<int> chave_Cred_OU_Oferc(5);
//
//                     Variable var_Alunos;
//                     vector<int> chave_Alunos(3);
//
//                     Variable var_Aloc_Alunos;
//                     vector<int> chave_Aloc_Alunos(4);
//
//                     // Inicializando
//
//                     var_Cred_OU_Oferc.reset();
//                     var_Cred_OU_Oferc.setType(Variable::V_CREDITOS);
//
//                     var_Alunos.reset();
//                     var_Alunos.setType(Variable::V_ALUNOS);
//
//                     var_Aloc_Alunos.reset();
//                     var_Aloc_Alunos.setType(Variable::V_ALOC_ALUNO);
//
//                     // Setando turma
//
//                     var_Cred_OU_Oferc.setTurma( it_Dia->second.second.at(cred).second.first );
//                     chave_Cred_OU_Oferc.at(0) = it_Dia->second.second.at(cred).second.first;
//
//                     var_Alunos.setTurma( it_Dia->second.second.at(cred).second.first );
//                     chave_Alunos.at(0) = it_Dia->second.second.at(cred).second.first;
//
//                     var_Aloc_Alunos.setTurma( it_Dia->second.second.at(cred).second.first );
//                     chave_Aloc_Alunos.at(0) = it_Dia->second.second.at(cred).second.first;
//
//                     // Setando disciplina
//
//                     var_Cred_OU_Oferc.setDisciplina( it_Dia->second.second.at(cred).first->disciplina );
//                     chave_Cred_OU_Oferc.at(1) = it_Dia->second.second.at(cred).first->disciplina->getId();
//
//                     var_Alunos.setDisciplina( it_Dia->second.second.at(cred).first->disciplina );
//                     chave_Alunos.at(1) = it_Dia->second.second.at(cred).first->disciplina->getId();
//
//                     var_Aloc_Alunos.setDisciplina( it_Dia->second.second.at(cred).first->disciplina );
//                     chave_Aloc_Alunos.at(1) = it_Dia->second.second.at(cred).first->disciplina->getId();
//
//                     // Setando curso
//
//                     var_Aloc_Alunos.setCurso( it_Dia->second.second.at(cred).first->oferta->curso );
//                     chave_Aloc_Alunos.at(2) = it_Dia->second.second.at(cred).first->oferta->curso->getId();
//
//                     // Setando campus
//
//                     var_Aloc_Alunos.setCampus( it_Dia->second.second.at(cred).first->oferta->campus );
//                     chave_Aloc_Alunos.at(3) = it_Dia->second.second.at(cred).first->oferta->campus->getId();
//
//                     // Setando unidade
//
//                     var_Cred_OU_Oferc.setUnidade( it_IS_Unidade->unidade );
//                     chave_Cred_OU_Oferc.at(2) = it_IS_Unidade->unidade->getId();
//
//                     // Setando conjunto de sala (tps)
//
//                     // ---
//
//                     // Procurando o Conjunto de Salas correto.
//
//                     int id = 
//                        (it_IS_Sala->sala->tipo_sala->getId() == 1 ? 
//                        it_IS_Sala->sala->capacidade : -it_IS_Sala->sala->capacidade);
//
//                     ConjuntoSala * cjt_Sala = NULL;
//
//                     bool found_CJT_Sala = false;
//
//                     ITERA_GGROUP(it_Cjt_Sala,it_IS_Unidade->unidade->conjutoSalas,ConjuntoSala)
//                     {
//                        if(it_Cjt_Sala->getId() == id)
//                        {
//                           cjt_Sala = *it_Cjt_Sala;
//
//                           found_CJT_Sala = true;
//
//                           break;
//                        }
//                     }
//
//                     if(!found_CJT_Sala)
//                     { cout << "CJT SALA NAO ENCONTRADO" << endl; exit(1); }
//
//                     chave_Cred_OU_Oferc.at(3) = id;
//
//                     // ---
//
//                     var_Cred_OU_Oferc.setSubCjtSala( cjt_Sala );
//
//                     // Setando dia
//
//                     var_Cred_OU_Oferc.setDia( it_Dia->first );
//                     chave_Cred_OU_Oferc.at(4) = it_Dia->first;
//
//                     // Setando oferta
//                     var_Alunos.setOferta(problem_Data->refOfertas
//                        [it_Dia->second.second.at(cred).first->oferta->getId()]);
//                     chave_Alunos.at(2) = it_Dia->second.second.at(cred).first->oferta->getId();
//
//                     // ===
//
//                     /* Tratamento das variaveis do tipo V_CREDITOS e V_OFERECIMENTO colhidas acima. 
//
//                     Checando se já existe uma variavel em <vars_Dia_Cred_OU_Oferc> 
//                     com os indices ou se é nova.
//                     */
//                     if(vars_Dia_Cred_OU_Oferc.find(chave_Cred_OU_Oferc) == vars_Dia_Cred_OU_Oferc.end())
//                     { vars_Dia_Cred_OU_Oferc[chave_Cred_OU_Oferc] = make_pair(var_Cred_OU_Oferc,1); }
//                     else
//                     {
//                        if(var_Cred_OU_Oferc.getType() == Variable::V_CREDITOS)
//                        { vars_Dia_Cred_OU_Oferc[chave_Cred_OU_Oferc].second += 1; }
//                     }
//
//                     /* Tratamento da variavel do tipo V_ALUNOS colhida acima 
//
//                     Existe apenas uma variável do tipo V_ALUNOS com os indices <i>, <d> e <o>.
//                     Portanto, mesmo que ela apareça várias vezes na solução, isso não quer dizer
//                     que o valor tenha que ser somado. Ele apenas estará replicado.
//
//                     O mesmo acontece com a variávelo do tipo V_ALOC_ALUNOS. Houve a necessidade de
//                     criar uma estrutura própria para essa variavel devido a diferença entre os indices
//                     quando comparada com a variavel do tipo V_ALUNOS
//                     */
//                     if(vars_Alunos.find(chave_Alunos) == vars_Alunos.end())
//                     {
//                        vars_Alunos[chave_Alunos] = make_pair
//                           (var_Alunos,it_Dia->second.second.at(cred).second.second);
//                     }
//
//                     if(vars_Aloc_Alunos.find(chave_Aloc_Alunos) == vars_Aloc_Alunos.end())
//                     {
//                        vars_Aloc_Alunos[chave_Aloc_Alunos] = make_pair
//                           (var_Aloc_Alunos,1);
//                     }
//                  }
//               }
//
//               // --- --- ---
//
//               map<vector<int/*indices de uma var do tipo CREDITOS*/>,
//                  pair<Variable,int/*valor var*/> >::iterator 
//                  it_Vars_Dia_Cred_OU_Oferc = vars_Dia_Cred_OU_Oferc.begin();
//
//               /* P/ cada variavel registrada em <vars_Dia_Cred_OU_Oferc>, procuro em <v_Hash> e
//               e se ela for encontrada, atribuo o valor indicado pela estrutura <vars_Dia_Cred_OU_Oferc> */
//               for(; it_Vars_Dia_Cred_OU_Oferc != vars_Dia_Cred_OU_Oferc.end(); it_Vars_Dia_Cred_OU_Oferc++)
//               {
//                  // Procurando pela variavel no Hash de variaveis.
//                  VariableHash::iterator it_v_Hash = v_Hash.find(it_Vars_Dia_Cred_OU_Oferc->second.first);
//
//                  if(it_v_Hash != v_Hash.end())
//                  {
//                     //cout << "Found V_CREDITOS" << endl;
//
//                     indices->push_back(it_v_Hash->second);
//
//                     valores->push_back(it_Vars_Dia_Cred_OU_Oferc->second.second);
//
//                     ++cnt;
//#ifdef FIX_LB_AND_UB_TO_THE_SOL_INI_VALUE
//                     // Alterando o lb e o ub da var em questão.
//                     double bound = it_Vars_Dia_Cred_OU_Oferc->second.second;
//                     lp.chgLB(it_v_Hash->second,bound);
//                     lp.chgUB(it_v_Hash->second,bound);
//#endif
//                     indices_V_CREDITOS.insert(it_v_Hash->second);
//                  }
//                  else
//                  {
//                     cout << "Not Found" << endl;
//                     exit(1);
//                  }
//
//                  // ===
//
//                  /* Para cada variavel do tipo CREDITOS, tem que existir uma variavel do 
//                  tipo OFERECIMENTO (Binaria) correspondente. */
//
//                  // Alternando APENAS o tipo da variavel para OFERECIMENTO
//                  it_Vars_Dia_Cred_OU_Oferc->second.first.setType(Variable::V_OFERECIMENTO);
//
//                  // Procurando pela variavel no Hash de variaveis.
//                  it_v_Hash = v_Hash.find(it_Vars_Dia_Cred_OU_Oferc->second.first);
//
//                  if(it_v_Hash != v_Hash.end())
//                  {
//                     //cout << "Found V_OFERECIMENTO" << endl;
//
//                     //indices[cnt] = it_v_Hash->second;
//                     indices->push_back(it_v_Hash->second);
//
//                     //valores[cnt] = 1;
//                     valores->push_back(1);
//
//#ifdef FIX_LB_AND_UB_TO_THE_SOL_INI_VALUE
//                     // Alterando o lb e o ub da var em questão.
//                     double bound = 1;
//                     lp.chgLB(it_v_Hash->second,bound);
//                     lp.chgUB(it_v_Hash->second,bound);
//#endif
//
//                     ++cnt;
//
//                     indices_V_OFERECIMENTO.insert(it_v_Hash->second);
//                  }
//                  else
//                  {
//                     cout << "Not Found" << endl;
//                     exit(1);
//                  }
//
//                  // ===
//
//               }
//
//               // --- --- ---
//
//            }
//         }
//      }
//
//      // --- --- ---
//
//      map<vector<int/*indices de uma var do tipo V_ALUNOS*/>,
//         pair<Variable,int/*valor var*/> >::iterator 
//         it_Vars_Alunos = vars_Alunos.begin();
//
//      /* P/ cada variavel registrada em <vars_Alunos>, procuro em <v_Hash> e
//      e se ela for encontrada, atribuo o valor indicado pela estrutura <vars_Alunos> */
//      for(; it_Vars_Alunos != vars_Alunos.end(); it_Vars_Alunos++)
//      {
//         // Procurando pela variavel no Hash de variaveis.
//         VariableHash::iterator it_v_Hash = v_Hash.find(it_Vars_Alunos->second.first);
//
//         if(it_v_Hash != v_Hash.end())
//         {
//            //cout << "Found V_ALUNOS" << endl;
//
//            //indices[cnt] = it_v_Hash->second;
//            indices->push_back(it_v_Hash->second);
//
//            //valores[cnt] = it_Vars_Alunos->second.second;
//            valores->push_back(it_Vars_Alunos->second.second);
//
//#ifdef FIX_LB_AND_UB_TO_THE_SOL_INI_VALUE
//            // Alterando o lb e o ub da var em questão.
//            double bound = it_Vars_Alunos->second.second;
//            lp.chgLB(it_v_Hash->second,bound);
//            lp.chgUB(it_v_Hash->second,bound);
//#endif
//
//            ++cnt;
//
//            indices_V_ALUNOS.insert(it_v_Hash->second);
//         }
//         else
//         {
//            cout << "Not Found" << endl;
//            exit(1);
//         }
//      }
//
//      // --- --- ---
//
//      map<vector<int/*indices de uma var do tipo V_ALOC_ALUNOS*/>,
//         pair<Variable,int/*valor var*/> >::iterator 
//         it_Vars_Aloc_Alunos = vars_Aloc_Alunos.begin();
//
//      /* P/ cada variavel registrada em <vars_Aloc_Alunos>, procuro em <v_Hash> e
//      e se ela for encontrada, atribuo o valor indicado pela estrutura <vars_Aloc_Alunos> */
//      for(; it_Vars_Aloc_Alunos != vars_Aloc_Alunos.end(); it_Vars_Aloc_Alunos++)
//      {
//         // Procurando pela variavel no Hash de variaveis.
//         VariableHash::iterator it_v_Hash = v_Hash.find(it_Vars_Aloc_Alunos->second.first);
//
//         if(it_v_Hash != v_Hash.end())
//         {
//            //cout << "Found V_ALOC_ALUNO" << endl;
//
//            //indices[cnt] = it_v_Hash->second;
//            indices->push_back(it_v_Hash->second);
//
//            //valores[cnt] = 1;
//            valores->push_back(1);
//
//#ifdef FIX_LB_AND_UB_TO_THE_SOL_INI_VALUE
//            // Alterando o lb e o ub da var em questão.
//            double bound = 1;
//            lp.chgLB(it_v_Hash->second,bound);
//            lp.chgUB(it_v_Hash->second,bound);
//#endif
//
//            ++cnt;
//
//            indices_V_ALOC_ALUNO.insert(it_v_Hash->second);
//         }
//         else
//         {
//            cout << "Not Found" << endl;
//            exit(1);
//         }
//      }
//
//      // --- --- ---
//
//   }
//
//   cout << "cnt (Apenas Vars. com valor): " << cnt << endl;
//
//   /* Para todas as outras variaveis do tipo V_CREDITOS, V_OFERECIMENTO, V_ALUNOS que não tiverem valor associado,
//   deve-se inicializa-las com o valor 0. */
//
//   VariableHash::iterator it_v_Hash = v_Hash.begin();
//
//   int cnt_V_CREDITOS_zeradas = 0;
//   int cnt_V_OFERECIMENTO_zeradas = 0;
//   int cnt_V_ALUNOS_zeradas = 0;
//   int cnt_V_ALOC_ALUNO_zeradas = 0;
//
//   for(; it_v_Hash != v_Hash.end() ; it_v_Hash++)
//   {
//      switch(it_v_Hash->first.getType())
//      {
//      case Variable::V_CREDITOS:
//         if(indices_V_CREDITOS.find(it_v_Hash->second) == indices_V_CREDITOS.end())
//         {
//            //indices[cnt] = it_v_Hash->second;
//            indices->push_back(it_v_Hash->second);
//
//            //valores[cnt] = 0;
//            valores->push_back(0);
//
//#ifdef FIX_LB_AND_UB_TO_THE_SOL_INI_VALUE
//            // Alterando o lb e o ub da var em questão.
//            double bound = 0;
//            lp.chgLB(it_v_Hash->second,bound);
//            lp.chgUB(it_v_Hash->second,bound);
//#endif
//
//            ++cnt;
//            ++cnt_V_CREDITOS_zeradas;
//         }
//         break;
//      case Variable::V_OFERECIMENTO:
//         if(indices_V_OFERECIMENTO.find(it_v_Hash->second) == indices_V_OFERECIMENTO.end())
//         { 
//            //indices[cnt] = it_v_Hash->second;
//            indices->push_back(it_v_Hash->second);
//
//            //valores[cnt] = 0;
//            valores->push_back(0);
//
//#ifdef FIX_LB_AND_UB_TO_THE_SOL_INI_VALUE
//            // Alterando o lb e o ub da var em questão.
//            double bound = 0;
//            lp.chgLB(it_v_Hash->second,bound);
//            lp.chgUB(it_v_Hash->second,bound);
//#endif
//
//
//            ++cnt;
//            ++cnt_V_OFERECIMENTO_zeradas;
//         }
//         break;
//      case Variable::V_ALUNOS:
//         if(indices_V_ALUNOS.find(it_v_Hash->second) == indices_V_ALUNOS.end())
//         {
//            //indices[cnt] = it_v_Hash->second;
//            indices->push_back(it_v_Hash->second);
//
//            //valores[cnt] = 0;
//            valores->push_back(0);
//
//#ifdef FIX_LB_AND_UB_TO_THE_SOL_INI_VALUE
//            // Alterando o lb e o ub da var em questão.
//            double bound = 0;
//            lp.chgLB(it_v_Hash->second,bound);
//            lp.chgUB(it_v_Hash->second,bound);
//#endif
//
//            ++cnt;
//            ++cnt_V_ALUNOS_zeradas;
//         }
//         break;
//      case Variable::V_ALOC_ALUNO:
//         if(indices_V_ALOC_ALUNO.find(it_v_Hash->second) == indices_V_ALOC_ALUNO.end())
//         { 
//            //indices[cnt] = it_v_Hash->second;
//            indices->push_back(it_v_Hash->second);
//
//            //valores[cnt] = 0;
//            valores->push_back(0);
//
//#ifdef FIX_LB_AND_UB_TO_THE_SOL_INI_VALUE
//            // Alterando o lb e o ub da var em questão.
//            double bound = 0;
//            lp.chgLB(it_v_Hash->second,bound);
//            lp.chgUB(it_v_Hash->second,bound);
//#endif
//
//            ++cnt;
//            ++cnt_V_ALOC_ALUNO_zeradas;
//         }
//         break;
//      default:
//         break;
//      }
//   }
//
//   cout << "cnt_V_CREDITOS_zeradas: " << cnt_V_CREDITOS_zeradas << endl;
//   cout << "cnt_V_OFERECIMENTO_zeradas: " << cnt_V_OFERECIMENTO_zeradas << endl;
//   cout << "cnt_V_ALUNOS_zeradas: " << cnt_V_ALUNOS_zeradas << endl;
//   cout << "cnt_V_ALOC_ALUNO_zeradas: " << cnt_V_ALOC_ALUNO_zeradas << endl;
//
//   cout << "cnt final: " << cnt << endl;
//
//   cout << "ESCREVER UM ALGORITMO PARA CHECAR SE HA A NECESSIDADE DE INICIALIZAR AS VARIAVEIS DE FOLGA. BASTA PERCORRER A ESTRUTURA <disc_Cjts_Demandas> PROCURANDO POR VALORES MAIORES DO QUE ZERO." << endl;
//   exit(1);
//
//   cout << "Convertido" << endl;
//
//   //exit(0);

   return make_pair(indices,valores);
}

//pair<int*,double*> InitialSolution::repSolIniToVariaveis(VariableHash & v_Hash, int lp_Cols)
//void InitialSolution::repSolIniToVariaveis(VariableHash & v_Hash, int lp_Cols)
//pair<vector<int>*,vector<double>*> InitialSolution::repSolIniToVariaveis(VariableHash & v_Hash, int lp_Cols)
//{
//   //int * indices = new int (lp_Cols);
//   //int * indices = new int[lp_Cols];
//
//   vector<int> * indices = new vector<int>;
//
//   //double * valores = new double (lp_Cols);
//   //double * valores = new double[lp_Cols];
//   
//   vector<double> * valores = new vector<double>;
//
//   /* Responsavel por armazenar os indices das vars do tipo V_CREDITOS que tiveram
//   valor atribuido no processo de geração da solução inicial. */
//   set<int> indices_V_CREDITOS;
//
//   /* Responsavel por armazenar os indices das vars do tipo V_OFERECIMENTO que tiveram
//   valor atribuido no processo de geração da solução inicial. */
//   set<int> indices_V_OFERECIMENTO;
//
//   /* Responsavel por armazenar os indices das vars do tipo V_ALUNOS que tiveram
//   valor atribuido no processo de geração da solução inicial. */
//   set<int> indices_V_ALUNOS;
//
//   /* Responsavel por armazenar os indices das vars do tipo V_ALOC_ALUNO que tiveram
//   valor atribuido no processo de geração da solução inicial. */
//   set<int> indices_V_ALOC_ALUNO;
//
//   int cnt = 0;
//
//   // p/ cada campus
//   ITERA_GGROUP(it_IS_Campus,solucao,IS_Campus)
//   {
//      // Armazena os indices e valores das variaveis do tipo V_ALUNOS
//      map<vector<int/*indices de uma var do tipo V_ALUNOS*/>,
//         pair<Variable,int/*valor var*/> > vars_Alunos;
//
//      // Armazena os indices e valores das variaveis do tipo V_ALOC_ALUNO
//      map<vector<int/*indices de uma var do tipo V_ALOC_ALUNO*/>,
//         pair<Variable,int/*valor var*/> > vars_Aloc_Alunos;
//
//      // p/ cada unidade
//      ITERA_GGROUP(it_IS_Unidade,it_IS_Campus->unidades,IS_Unidade)
//      {
//         // p/ cada sala
//         ITERA_GGROUP(it_IS_Sala,it_IS_Unidade->salas,IS_Sala)
//         {
//            map<int/*dia*/,pair<int/*credsLivres*/,
//               vector<
//               pair<Demanda*,
//               pair<int/*Id Turma*/,int/*Demanda Atendida*/> > > > >::iterator
//
//               it_Dia = it_IS_Sala->atendimento_Tatico.begin();
//
//            // p/ cada dia do atendimento tatico
//            for(; it_Dia != it_IS_Sala->atendimento_Tatico.end(); it_Dia++)
//            {
//               /* Armazena os indices e valores das variaveis do tipo V_CREDITOS 
//               
//                  Como existe uma variavel do tipo V_OFERECIMENTO para cada variavel
//                  do tipo V_CREDITOS, usam-se os mesmos indices armazenados nessa estrutura
//                  para inicializar as variaveis do tipo V_OFERECIMENTO
//               */
//               map<vector<int/*indices de uma var do tipo V_CREDITOS ou V_OFERECIMENTO*/>,
//                  pair<Variable,int/*valor var*/> > vars_Dia_Cred_OU_Oferc;
//
//               // p/ cada credito do dia 
//               for(unsigned cred = 0; cred < it_Dia->second.second.size(); cred++)
//               {
//                  // SOMENTE se o cred tiver sido alocado
//                  if(it_Dia->second.second.at(cred).first) // != NULL
//                  {
//                     // Coletando os dados das variaveis
//
//                     // ===
//
//                     // Criando as variáveis e estruturas necessárias
//
//                     Variable var_Cred_OU_Oferc;
//                     vector<int> chave_Cred_OU_Oferc(5);
//
//                     Variable var_Alunos;
//                     vector<int> chave_Alunos(3);
//
//                     Variable var_Aloc_Alunos;
//                     vector<int> chave_Aloc_Alunos(4);
//
//                     // Inicializando
//
//                     var_Cred_OU_Oferc.reset();
//                     var_Cred_OU_Oferc.setType(Variable::V_CREDITOS);
//
//                     var_Alunos.reset();
//                     var_Alunos.setType(Variable::V_ALUNOS);
//
//                     var_Aloc_Alunos.reset();
//                     var_Aloc_Alunos.setType(Variable::V_ALOC_ALUNO);
//
//                     // Setando turma
//
//                     var_Cred_OU_Oferc.setTurma( it_Dia->second.second.at(cred).second.first );
//                     chave_Cred_OU_Oferc.at(0) = it_Dia->second.second.at(cred).second.first;
//
//                     var_Alunos.setTurma( it_Dia->second.second.at(cred).second.first );
//                     chave_Alunos.at(0) = it_Dia->second.second.at(cred).second.first;
//
//                     var_Aloc_Alunos.setTurma( it_Dia->second.second.at(cred).second.first );
//                     chave_Aloc_Alunos.at(0) = it_Dia->second.second.at(cred).second.first;
//
//                     // Setando disciplina
//
//                     var_Cred_OU_Oferc.setDisciplina( it_Dia->second.second.at(cred).first->disciplina );
//                     chave_Cred_OU_Oferc.at(1) = it_Dia->second.second.at(cred).first->disciplina->getId();
//
//                     var_Alunos.setDisciplina( it_Dia->second.second.at(cred).first->disciplina );
//                     chave_Alunos.at(1) = it_Dia->second.second.at(cred).first->disciplina->getId();
//
//                     var_Aloc_Alunos.setDisciplina( it_Dia->second.second.at(cred).first->disciplina );
//                     chave_Aloc_Alunos.at(1) = it_Dia->second.second.at(cred).first->disciplina->getId();
//
//                     // Setando curso
//
//                     var_Aloc_Alunos.setCurso( it_Dia->second.second.at(cred).first->oferta->curso );
//                     chave_Aloc_Alunos.at(2) = it_Dia->second.second.at(cred).first->oferta->curso->getId();
//
//                     // Setando campus
//
//                     var_Aloc_Alunos.setCampus( it_Dia->second.second.at(cred).first->oferta->campus );
//                     chave_Aloc_Alunos.at(3) = it_Dia->second.second.at(cred).first->oferta->campus->getId();
//
//                     // Setando unidade
//
//                     var_Cred_OU_Oferc.setUnidade( it_IS_Unidade->unidade );
//                     chave_Cred_OU_Oferc.at(2) = it_IS_Unidade->unidade->getId();
//
//                     // Setando conjunto de sala (tps)
//
//                     // ---
//
//                     // Procurando o Conjunto de Salas correto.
//
//                     int id = 
//                        (it_IS_Sala->sala->tipo_sala->getId() == 1 ? 
//                        it_IS_Sala->sala->capacidade : -it_IS_Sala->sala->capacidade);
//
//                     ConjuntoSala * cjt_Sala = NULL;
//
//                     bool found_CJT_Sala = false;
//
//                     ITERA_GGROUP(it_Cjt_Sala,it_IS_Unidade->unidade->conjutoSalas,ConjuntoSala)
//                     {
//                        if(it_Cjt_Sala->getId() == id)
//                        {
//                           cjt_Sala = *it_Cjt_Sala;
//
//                           found_CJT_Sala = true;
//
//                           break;
//                        }
//                     }
//
//                     if(!found_CJT_Sala)
//                     { cout << "CJT SALA NAO ENCONTRADO" << endl; exit(1); }
//
//                     chave_Cred_OU_Oferc.at(3) = id;
//
//                     // ---
//
//                     var_Cred_OU_Oferc.setSubCjtSala( cjt_Sala );
//
//                     // Setando dia
//
//                     var_Cred_OU_Oferc.setDia( it_Dia->first );
//                     chave_Cred_OU_Oferc.at(4) = it_Dia->first;
//
//                     // Setando oferta
//                     var_Alunos.setOferta(problem_Data->refOfertas
//                        [it_Dia->second.second.at(cred).first->oferta->getId()]);
//                     chave_Alunos.at(2) = it_Dia->second.second.at(cred).first->oferta->getId();
//
//                     // ===
//
//                     /* Tratamento das variaveis do tipo V_CREDITOS e V_OFERECIMENTO colhidas acima. 
//                        
//                        Checando se já existe uma variavel em <vars_Dia_Cred_OU_Oferc> 
//                     com os indices ou se é nova.
//                     */
//                     if(vars_Dia_Cred_OU_Oferc.find(chave_Cred_OU_Oferc) == vars_Dia_Cred_OU_Oferc.end())
//                     { vars_Dia_Cred_OU_Oferc[chave_Cred_OU_Oferc] = make_pair(var_Cred_OU_Oferc,1); }
//                     else
//                     {
//                        if(var_Cred_OU_Oferc.getType() == Variable::V_CREDITOS)
//                        { vars_Dia_Cred_OU_Oferc[chave_Cred_OU_Oferc].second += 1; }
//                     }
//
//                     /* Tratamento da variavel do tipo V_ALUNOS colhida acima 
//                     
//                        Existe apenas uma variável do tipo V_ALUNOS com os indices <i>, <d> e <o>.
//                        Portanto, mesmo que ela apareça várias vezes na solução, isso não quer dizer
//                        que o valor tenha que ser somado. Ele apenas estará replicado.
//
//                        O mesmo acontece com a variávelo do tipo V_ALOC_ALUNOS. Houve a necessidade de
//                        criar uma estrutura própria para essa variavel devido a diferença entre os indices
//                        quando comparada com a variavel do tipo V_ALUNOS
//                     */
//                     if(vars_Alunos.find(chave_Alunos) == vars_Alunos.end())
//                     {
//                        vars_Alunos[chave_Alunos] = make_pair
//                           (var_Alunos,it_Dia->second.second.at(cred).second.second);
//                     }
//                     
//                     if(vars_Aloc_Alunos.find(chave_Aloc_Alunos) == vars_Aloc_Alunos.end())
//                     {
//                        vars_Aloc_Alunos[chave_Aloc_Alunos] = make_pair
//                           (var_Aloc_Alunos,1);
//                     }
//                  }
//               }
//
//               // --- --- ---
//
//               map<vector<int/*indices de uma var do tipo CREDITOS*/>,
//                  pair<Variable,int/*valor var*/> >::iterator 
//                  it_Vars_Dia_Cred_OU_Oferc = vars_Dia_Cred_OU_Oferc.begin();
//
//               /* P/ cada variavel registrada em <vars_Dia_Cred_OU_Oferc>, procuro em <v_Hash> e
//               e se ela for encontrada, atribuo o valor indicado pela estrutura <vars_Dia_Cred_OU_Oferc> */
//               for(; it_Vars_Dia_Cred_OU_Oferc != vars_Dia_Cred_OU_Oferc.end(); it_Vars_Dia_Cred_OU_Oferc++)
//               {
//                  // Procurando pela variavel no Hash de variaveis.
//                  VariableHash::iterator it_v_Hash = v_Hash.find(it_Vars_Dia_Cred_OU_Oferc->second.first);
//
//                  if(it_v_Hash != v_Hash.end())
//                  {
//                     //cout << "Found V_CREDITOS" << endl;
//
//                     //indices[cnt] = it_v_Hash->second;
//                     indices->push_back(it_v_Hash->second);
//
//                     //valores[cnt] = it_Vars_Dia_Cred_OU_Oferc->second.second;
//                     valores->push_back(it_Vars_Dia_Cred_OU_Oferc->second.second);
//
//                     ++cnt;
//
//                     indices_V_CREDITOS.insert(it_v_Hash->second);
//                  }
//                  else
//                  {
//                     cout << "Not Found" << endl;
//                     exit(1);
//                  }
//
//                  // ===
//
//                  /* Para cada variavel do tipo CREDITOS, tem que existir uma variavel do 
//                  tipo OFERECIMENTO (Binaria) correspondente. */
//
//                  // Alternando APENAS o tipo da variavel para OFERECIMENTO
//                  it_Vars_Dia_Cred_OU_Oferc->second.first.setType(Variable::V_OFERECIMENTO);
//                  
//                  // Procurando pela variavel no Hash de variaveis.
//                  it_v_Hash = v_Hash.find(it_Vars_Dia_Cred_OU_Oferc->second.first);
//
//                  if(it_v_Hash != v_Hash.end())
//                  {
//                     //cout << "Found V_OFERECIMENTO" << endl;
//
//                     //indices[cnt] = it_v_Hash->second;
//                     indices->push_back(it_v_Hash->second);
//
//                     //valores[cnt] = 1;
//                     valores->push_back(1);
//
//                     ++cnt;
//
//                     indices_V_OFERECIMENTO.insert(it_v_Hash->second);
//                  }
//                  else
//                  {
//                     cout << "Not Found" << endl;
//                     exit(1);
//                  }
//
//                  // ===
//
//               }
//
//               // --- --- ---
//            
//            }
//         }
//      }
//
//      // --- --- ---
//
//      map<vector<int/*indices de uma var do tipo V_ALUNOS*/>,
//         pair<Variable,int/*valor var*/> >::iterator 
//         it_Vars_Alunos = vars_Alunos.begin();
//
//      /* P/ cada variavel registrada em <vars_Alunos>, procuro em <v_Hash> e
//      e se ela for encontrada, atribuo o valor indicado pela estrutura <vars_Alunos> */
//      for(; it_Vars_Alunos != vars_Alunos.end(); it_Vars_Alunos++)
//      {
//         // Procurando pela variavel no Hash de variaveis.
//         VariableHash::iterator it_v_Hash = v_Hash.find(it_Vars_Alunos->second.first);
//
//         if(it_v_Hash != v_Hash.end())
//         {
//            //cout << "Found V_ALUNOS" << endl;
//
//            //indices[cnt] = it_v_Hash->second;
//            indices->push_back(it_v_Hash->second);
//
//            //valores[cnt] = it_Vars_Alunos->second.second;
//            valores->push_back(it_Vars_Alunos->second.second);
//
//            ++cnt;
//
//            indices_V_ALUNOS.insert(it_v_Hash->second);
//         }
//         else
//         {
//            cout << "Not Found" << endl;
//            exit(1);
//         }
//      }
//
//      // --- --- ---
//
//      map<vector<int/*indices de uma var do tipo V_ALOC_ALUNOS*/>,
//         pair<Variable,int/*valor var*/> >::iterator 
//         it_Vars_Aloc_Alunos = vars_Aloc_Alunos.begin();
//
//      /* P/ cada variavel registrada em <vars_Aloc_Alunos>, procuro em <v_Hash> e
//      e se ela for encontrada, atribuo o valor indicado pela estrutura <vars_Aloc_Alunos> */
//      for(; it_Vars_Aloc_Alunos != vars_Aloc_Alunos.end(); it_Vars_Aloc_Alunos++)
//      {
//         // Procurando pela variavel no Hash de variaveis.
//         VariableHash::iterator it_v_Hash = v_Hash.find(it_Vars_Aloc_Alunos->second.first);
//
//         if(it_v_Hash != v_Hash.end())
//         {
//            //cout << "Found V_ALOC_ALUNO" << endl;
//
//            //indices[cnt] = it_v_Hash->second;
//            indices->push_back(it_v_Hash->second);
//
//            //valores[cnt] = 1;
//            valores->push_back(1);
//
//            ++cnt;
//
//            indices_V_ALOC_ALUNO.insert(it_v_Hash->second);
//         }
//         else
//         {
//            cout << "Not Found" << endl;
//            exit(1);
//         }
//      }
//
//      // --- --- ---
//
//   }
//
//   cout << "cnt (Apenas Vars. com valor): " << cnt << endl;
//
//   /* Para todas as outras variaveis do tipo V_CREDITOS, V_OFERECIMENTO, V_ALUNOS que não tiverem valor associado,
//   deve-se inicializa-las com o valor 0. */
//
//   VariableHash::iterator it_v_Hash = v_Hash.begin();
//
//   int cnt_V_CREDITOS_zeradas = 0;
//   int cnt_V_OFERECIMENTO_zeradas = 0;
//   int cnt_V_ALUNOS_zeradas = 0;
//   int cnt_V_ALOC_ALUNO_zeradas = 0;
//
//   for(; it_v_Hash != v_Hash.end() ; it_v_Hash++)
//   {
//      switch(it_v_Hash->first.getType())
//      {
//      case Variable::V_CREDITOS:
//         if(indices_V_CREDITOS.find(it_v_Hash->second) == indices_V_CREDITOS.end())
//         {
//            //indices[cnt] = it_v_Hash->second;
//            indices->push_back(it_v_Hash->second);
//            
//            //valores[cnt] = 0;
//            valores->push_back(0);
//
//            ++cnt;
//            ++cnt_V_CREDITOS_zeradas;
//         }
//         break;
//      case Variable::V_OFERECIMENTO:
//         if(indices_V_OFERECIMENTO.find(it_v_Hash->second) == indices_V_OFERECIMENTO.end())
//         { 
//            //indices[cnt] = it_v_Hash->second;
//            indices->push_back(it_v_Hash->second);
//
//            //valores[cnt] = 0;
//            valores->push_back(0);
//
//            ++cnt;
//            ++cnt_V_OFERECIMENTO_zeradas;
//         }
//         break;
//      case Variable::V_ALUNOS:
//         if(indices_V_ALUNOS.find(it_v_Hash->second) == indices_V_ALUNOS.end())
//         {
//            //indices[cnt] = it_v_Hash->second;
//            indices->push_back(it_v_Hash->second);
//
//            //valores[cnt] = 0;
//            valores->push_back(0);
//
//            ++cnt;
//            ++cnt_V_ALUNOS_zeradas;
//         }
//         break;
//      case Variable::V_ALOC_ALUNO:
//         if(indices_V_ALOC_ALUNO.find(it_v_Hash->second) == indices_V_ALOC_ALUNO.end())
//         { 
//            //indices[cnt] = it_v_Hash->second;
//            indices->push_back(it_v_Hash->second);
//
//            //valores[cnt] = 0;
//            valores->push_back(0);
//
//            ++cnt;
//            ++cnt_V_ALOC_ALUNO_zeradas;
//         }
//         break;
//      default:
//         break;
//      }
//   }
//   
//   cout << "cnt_V_CREDITOS_zeradas: " << cnt_V_CREDITOS_zeradas << endl;
//   cout << "cnt_V_OFERECIMENTO_zeradas: " << cnt_V_OFERECIMENTO_zeradas << endl;
//   cout << "cnt_V_ALUNOS_zeradas: " << cnt_V_ALUNOS_zeradas << endl;
//   cout << "cnt_V_ALOC_ALUNO_zeradas: " << cnt_V_ALOC_ALUNO_zeradas << endl;
//
//   cout << "cnt final: " << cnt << endl;
//   
//   cout << "Convertido" << endl;
//
///*
//cout << "CONVERTI AS VARS DO TIPO X, FALTA INDICAR COM O VALOR 0 TODAS AS VARIAVEIS X QUE NAO FORAM UTILIZADAS " <<
//"NA HEURISTICA. FALTA TB, CONVERTER AS OUTRAS VARIAVEIS (O,A,B)" << endl;
//*/
//
//   //exit(0);
//
//   return make_pair(indices,valores);
//}

//pair<int*,double*> InitialSolution::repSolIniParaVariaveis(VariableHash & v_Hash, int lp_Cols)
//{
//   int * indices = new int (lp_Cols);
//   double * valores = new double (lp_Cols);
//
//   int cnt = 0;
//
//   VariableHash::iterator it_v_Hash = v_Hash.begin();
//
//   for(; it_v_Hash != v_Hash.end(); it_v_Hash++)
//   {
//      switch(it_v_Hash->first.getType())
//      {
//      case Variable::V_CREDITOS:
//         cout << "CREDITOS !!!" << endl;
//         //getchar();
//         break;
//      default:
//         break;
//      }
//   }
//
//   return make_pair(indices,valores);
//}