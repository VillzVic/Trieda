#include "Solucao.h"

bool ordenaConjuntosDemandas(pair<pair<Disciplina*,int>, int*> const & left,
                             pair<pair<Disciplina*,int>, int*> const & right)
{
   return (*left.second > *right.second);
}

bool ordenaIS_Salas(IS_Sala const * left, IS_Sala const * right)
{
   return (left->sala->capacidade > right->sala->capacidade);
}

bool ordenaIS_Blocos(IS_Bloco_Curricular const * left, IS_Bloco_Curricular const * right)
{
   return (left->demanda_Total > right->demanda_Total);
}

Solucao::Solucao(ProblemData & _problem_Data) :
problem_Data(&_problem_Data), total_Cjt_Demandas(0)
{
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

   // ---------------------------------

   demanda_Nao_Atendida = 0;

   // ---------------------------------

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
}

Solucao::Solucao(Solucao const & init_sol) :
problem_Data(&init_sol.get_Problem_Data())
{
   cerr << "COPY CONSTRUCTOR OF <Solucao> NOT IMPLEMENTED YET" << endl;
   exit(1);
}

Solucao::~Solucao(void)
{
}

void Solucao::agrupaDemandas()
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

void Solucao::criaIS_Blocos_Curriculares()
{
   ITERA_GGROUP(it_Bloco,problem_Data->blocos,BlocoCurricular)
   {
      is_Blocos_Curriculares.push_back(new IS_Bloco_Curricular(*it_Bloco));
   }

   // -------------------------

   map<Disciplina*,
      pair<vector<pair<vector<pair<Demanda*,int/*Demanda específica ainda não atendida*/> >,
      int/*Somatório das demandas ainda não atendidas para um determinado conjunto*/> >,
      int/*Somatório das demandas ainda não atendidas de todos os conjuntos para uma dada disciplina*/ > >
      ::iterator

      it_Disc_Cjts_Demandas = disc_Cjts_Demandas.begin();

   // Para cada disciplina
   for(;it_Disc_Cjts_Demandas != disc_Cjts_Demandas.end();++it_Disc_Cjts_Demandas)
   {
      Disciplina * pt_Disc = it_Disc_Cjts_Demandas->first;

      int total_Cjts = it_Disc_Cjts_Demandas->second.first.size();

      // Para cada conjunto de demandas
      for(int cjt = 0; cjt < total_Cjts; ++cjt)
      {
         int total_Demandas = it_Disc_Cjts_Demandas->second.first.at(cjt).first.size();

         if(total_Demandas > 1)
         {
            for(int dem = 0; dem < total_Demandas; ++dem)
            {
               //disciplinas_Comuns_Entre_Demandas_Cursos_Compat[it_Disc_Cjts_Demandas->first].add(
                  //it_Disc_Cjts_Demandas->second.first.at(cjt).first.at(dem).first);
               //relacao_Alunos_Disciplinas[it_Disc_Cjts_Demandas->first].add(
                  //it_Disc_Cjts_Demandas->second.first.at(cjt).first.at(dem).first);
               relacao_Alunos_Disciplinas[it_Disc_Cjts_Demandas->first].push_back(
                  it_Disc_Cjts_Demandas->second.first.at(cjt).first.at(dem));
            }
         }
      }
   }

   // -------------------------

   // Para cada IS_Bloco_Curricular
   ITERA_VECTOR(it_IS_Bloco,is_Blocos_Curriculares,IS_Bloco_Curricular)
   {
      // -------------------------
      Curso * curso_IS_Blc = (*it_IS_Bloco)->bloco->curso;

      // -------------------------
      map<Disciplina*,int/*Demanda nao atendida*/> disc_Dem_Nao_Atd;

      // -------------------------
      map<Disciplina*,int/*Demanda nao atendida*/>::iterator 
         it_Disciplina_Dem_Nao_Atendida = (*it_IS_Bloco)->disciplina_Dem_Nao_Atendida.begin();

      // -------------------------
      // Para cada disciplina do IS_Bloco_Curricular em questão
      for(;it_Disciplina_Dem_Nao_Atendida !=
         (*it_IS_Bloco)->disciplina_Dem_Nao_Atendida.end();
         ++it_Disciplina_Dem_Nao_Atendida)
      {
         Disciplina * pt_Disc = it_Disciplina_Dem_Nao_Atendida->first;

         // Se a disciplina pertence ao conjunto de disciplinas comuns
         //if(disciplinas_Comuns_Entre_Demandas_Cursos_Compat.find(pt_Disc) !=
            //disciplinas_Comuns_Entre_Demandas_Cursos_Compat.end())
         if(relacao_Alunos_Disciplinas.find(pt_Disc) != relacao_Alunos_Disciplinas.end())
         {
            /* Testo agora se o Curso em quetão pertence a alguma das demandas da disciplina.
            Se pertencer tenho que remover a disciplina do map do bloco. */

            bool mantem = true;

            vector<pair<Demanda*,int> >::iterator 
               it_Demanda = relacao_Alunos_Disciplinas[pt_Disc].begin();

            //ITERA_GGROUP(it_Demanda,disciplinas_Comuns_Entre_Demandas_Cursos_Compat[pt_Disc],Demanda)
            //ITERA_GGROUP(it_Demanda,relacao_Alunos_Disciplinas[pt_Disc],Demanda)
            for(;it_Demanda != relacao_Alunos_Disciplinas[pt_Disc].end();++it_Demanda)
            {
               if(it_Demanda->first->oferta->curso == curso_IS_Blc)
               {
                  //(*it_IS_Bloco)->demanda_Total -= it_Disciplina_Dem_Nao_Atendida->second;

                  // DELETAR
                  //(*it_IS_Bloco)->disciplina_Dem_Nao_Atendida.erase(it_Disciplina_Dem_Nao_Atendida);
                  mantem = false;
                  break;
               }
               else
               {
                  //cout << "NAO DELETAR !!";
                  //Adicionar à nova .. .
                  //disc_Dem_Nao_Atd[pt_Disc] = it_Disciplina_Dem_Nao_Atendida->second;
               }
            }

            if(mantem)
            {
               disc_Dem_Nao_Atd[pt_Disc] = it_Disciplina_Dem_Nao_Atendida->second;
            }
         }
         else
         {
            disc_Dem_Nao_Atd[pt_Disc] = it_Disciplina_Dem_Nao_Atendida->second;
         }
      }

      //cout << "------------------------"<<endl;
      //cout << "Curso: " << (*it_IS_Bloco)->bloco->curso->codigo;
      //cout << "\tPeriodo: " << (*it_IS_Bloco)->bloco->periodo << endl;
      //cout << "Antes: " << (*it_IS_Bloco)->disciplina_Dem_Nao_Atendida.size() << endl;

      (*it_IS_Bloco)->disciplina_Dem_Nao_Atendida.clear();
      (*it_IS_Bloco)->demanda_Total = 0;

      // Atualizando
      map<Disciplina*,int/*Demanda nao atendida*/>::iterator
         it_Disc_Dem_Nao_Atd = disc_Dem_Nao_Atd.begin();

      for(;it_Disc_Dem_Nao_Atd != disc_Dem_Nao_Atd.end();++it_Disc_Dem_Nao_Atd)
      {
         (*it_IS_Bloco)->disciplina_Dem_Nao_Atendida[it_Disc_Dem_Nao_Atd->first] = 
            it_Disc_Dem_Nao_Atd->second;

         (*it_IS_Bloco)->demanda_Total += it_Disc_Dem_Nao_Atd->second;
      }

      //cout << "Depois: " << (*it_IS_Bloco)->disciplina_Dem_Nao_Atendida.size() << endl;

   }
}

void Solucao::organizaIS_Blocos_Curriculares(vector<IS_Bloco_Curricular*> & is_Blc_Curric)
{
   // GULOSO

   sort(is_Blc_Curric.begin(),is_Blc_Curric.end(),ordenaIS_Blocos);

   // ALEATORIO

   //random_shuffle(is_Blc_Curric.begin(),is_Blc_Curric.end());
}

//IS_Bloco_Curricular * Solucao::escolheIS_Bloco_Curricular()
//{
//   //ITERA_GGROUP(it_IS_Bloco,is_Blocos_Curriculares,IS_Bloco_Curricular)
//   ITERA_VECTOR(it_IS_Bloco,is_Blocos_Curriculares,IS_Bloco_Curricular)
//   {
//      //if(!it_IS_Bloco->atendido) return *it_IS_Bloco;
//      if(!(*it_IS_Bloco)->atendido)
//      {
//         // Marcando o bloco para que ele não tente alocar novamente
//         (*it_IS_Bloco)->atendido = true;
//
//         return *it_IS_Bloco;
//      }
//   }
//   return NULL;
//}

int Solucao::escolheConjuntoDeDemandasParaAddNovaDemanda(
   vector<vector<pair<Demanda*,int/*Demanda específica ainda não atendida*/> > > & cjts_Demanda,
   vector<int> & indices_Demandas)
{
   // Por enquanto, retornando o primeiro conjunto valido
   return indices_Demandas.front();
}

pair<Disciplina*,int/*indice do conjunto*/> Solucao::escolheConjuntoDeDemadasParaAlocarASala(
   vector<pair<pair<Disciplina*,int/*indice do conjunto*/>, int * /*Valor da demanda*/ > > & cjts_Dem)
{
   // GULOSO

   /*Ordenando todos os elementos de <cjts_Dem>, ou seja, ordenando todos os conjuntos de demandas*/
   //sort(cjts_Dem.begin(),cjts_Dem.end(),ordenaConjuntosDemandas);

   //return make_pair(cjts_Dem.front().first.first,cjts_Dem.front().second);
   //return cjts_Dem.front().first;

   // -------------------------

   /* PARCIALMENTE ALEATORIO 
   
   Ordeno em ordem decrescente e escolho aleatoriamente entre algumas soluções.
   */
   
   /* Ordenando todos os elementos de <cjts_Dem>, ou seja, ordenando todos os conjuntos de demandas */
   //sort(cjts_Dem.begin(),cjts_Dem.end(),ordenaConjuntosDemandas);

   // Limite superior para a posição a ser sorteada

   int limite = cjts_Dem.size();

   int pos = rand() % limite;

   while(indices_Proibidos.find(pos) != indices_Proibidos.end())
   { pos = rand() % limite; }

   indices_Proibidos.add(pos);

   return cjts_Dem.at(pos).first;
}

void Solucao::carregaEstruturaCjtsDem(
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
               << "Solucao::inicializaEstruturaCjtsDem(...)" << endl;

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
Solucao::getRegradeCreditoEVariacoes(Disciplina * disciplina)
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

vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >::iterator Solucao::salaCompativelRegraCredito(
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
}

//void Solucao::aloca(pair<Disciplina*,int/*indice do conjunto*/> & cjt_Dem_Esc,
//                            vector<IS_Sala*> & is_Salas_Comaptiveis)
//{
//   // Contabiliza o número de turmas abertas para a disciplina do conjunto de demandas em questão
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
//   while(!terminou_Alocacao)
//   {
//      bool alocou_Em_Alguma_Sala = false;
//
//      /* 
//      Antes de alocar, verificar:
//
//      Se o número máximo de turmas que podem ser abertas já foi alcançado, não aloca mais.
//      */
//      //if(num_Turmas_Abertas < pt_Disc->num_turmas)
//      {
//         // Escolhendo a sala para tentar alocar
//
//         /* Iterando sobre as IS_Salas compativeis (que, agora, estão ordenadas por capacidade)
//         até que alguma sala disponível seja encontrada. Isto é, estamos procurando por uma sala
//         que possua a quantidade de créditos livres demandados pela disciplina para cada dia especificado. 
//
//         Caso, não encontre, não aloco nada. IDEIA: Poderiamos sugerir outras regras de credito semelhantes
//         à regra de crédito em questão e tentar aplicá-las.
//         */
//
//         organizaIS_Salas(is_Salas_Comaptiveis);
//
//         ITERA_VECTOR(it_IS_Salas_Compativeis,is_Salas_Comaptiveis,IS_Sala)
//         {
//            /* Obtendo alguma regra de crédito para a disciplina em questão.
//
//            Pode ser que a disciplina possua uma regra de crédito especificada na entrada. Caso 
//            isso não aconteça, uma regra de crédito será proposta.
//
//            Lembrando que uma regra de crédito não é fixa em relação aos dias. Por isso
//            temos uma coleção de regras de crédito abaixo. Tratam-se da mesma regra de crédito, porém,
//            cada uma especifica os dias em que os créditos devem ser ministrados.
//            */
//            vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > * 
//               variacoes_Regra_de_Cred_Esc = getRegradeCreditoEVariacoes(pt_Disc);
//
//            // Checando se a sala em questão é compatível com alguma das variações da regra de crédito em questão.
//            //int regra_de_Credt_Esc = 
//            vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >::iterator
//               it_Regra_de_Credt_Esc =
//               salaCompativelRegraCredito(*it_IS_Salas_Compativeis,*variacoes_Regra_de_Cred_Esc);
//
//            if(it_Regra_de_Credt_Esc != variacoes_Regra_de_Cred_Esc->end())
//            {
//               // Se esta aqui é pq achamos uma sala para realizar a alocação
//
//               //Calculando a quantidade de alunos que será alocada para a turma em questão
//               int num_Alunos_Turma = (
//                  (*it_IS_Salas_Compativeis)->sala->capacidade >= 
//                  disc_Cjts_Demandas[pt_Disc].first.at(cjt_Dem_Esc.second).second ?
//                  disc_Cjts_Demandas[pt_Disc].first.at(cjt_Dem_Esc.second).second
//                  : (*it_IS_Salas_Compativeis)->sala->capacidade);
//
//               // Configurando uma turma para o conjunto de demandas em questão.
//               vector<pair<Demanda*,int> > demandas_A_Alocar = 
//                  elaboraTurma(cjt_Dem_Esc,num_Alunos_Turma);
//
//               (*it_IS_Salas_Compativeis)->aloca(
//                  num_Turmas_Abertas,
//                  cjt_Dem_Esc,
//                  demandas_A_Alocar,
//                  num_Alunos_Turma,
//                  *it_Regra_de_Credt_Esc);
//
//               // Atualizando a estrutura <disc_Cjts_Demandas>
//               map<Disciplina*,
//                  pair<vector<pair<vector<pair<Demanda*,int/*Demanda específica ainda não atendida*/> >,
//                  int/*Somatório das demandas ainda não atendidas para um determinado conjunto*/> >,
//                  int/*Somatório das demandas ainda não atendidas de todos os conjuntos para uma dada disciplina*/ > >
//                  ::iterator
//
//                  it_Disc_Cjts_Demandas = disc_Cjts_Demandas.find(pt_Disc);
//
//               if(it_Disc_Cjts_Demandas != disc_Cjts_Demandas.end())
//               {
//                  int demanda_Total_Alocada = 0;
//
//                  // Iterador das demandas alocadas
//                  vector<pair<Demanda*,int> >::iterator
//                     it_Demandas_A_Alocar = demandas_A_Alocar.begin();
//
//                  // P/ cada demanda que foi alocada
//                  for(; it_Demandas_A_Alocar != demandas_A_Alocar.end(); it_Demandas_A_Alocar++)
//                  {
//                     // Iterador das demandas de um conjunto específico para uma dada disciplina
//                     vector<pair<Demanda*,int/*Demanda específica ainda não atendida*/> >::iterator
//                        it_Dems_Cjt = it_Disc_Cjts_Demandas->second.first.at(cjt_Dem_Esc.second).first.begin();
//
//                     // Procurando em <disc_Cjts_Demandas> a demanda acima
//                     for(; it_Dems_Cjt != it_Disc_Cjts_Demandas->second.first.at(cjt_Dem_Esc.second).first.end(); it_Dems_Cjt++)
//                     {
//                        //if(it_Dems_Cjt->first->getId() == it_Demandas_A_Alocar->first->getId())
//                        if(it_Dems_Cjt->first == it_Demandas_A_Alocar->first)
//                        {
//                           it_Dems_Cjt->second -= it_Demandas_A_Alocar->second;
//
//                           demanda_Total_Alocada += it_Demandas_A_Alocar->second;
//
//                           break;
//                        }
//                     }
//                  }
//
//                  // Atualizando o somatório das demandas ainda não atendidas para um determinado conjunto
//                  it_Disc_Cjts_Demandas->second.first.at(cjt_Dem_Esc.second).second -= demanda_Total_Alocada;
//
//                  // Atualizando o somatório das demandas ainda não atendidas de todos os conjuntos para uma dada disciplina
//                  it_Disc_Cjts_Demandas->second.second -= demanda_Total_Alocada;
//
//               }
//               else
//               {
//                  cerr << "Erro em Solucao::aloca(...)" << endl;
//                  exit(1);
//               }
//
//               // Contabilizando o total de turmas abertas
//               ++num_Turmas_Abertas;
//
//               alocou_Em_Alguma_Sala = true;
//
//               break;
//            }
//         }
//      }
//      //else
//      //{
//      //   terminou_Alocacao = true;
//
//      //   //cout << "Numero maximo de turmas alcançado" << endl;
//      //   //cout << " aloquei a demanda toda" << endl;
//
//      //   demanda_Nao_Atendida += disc_Cjts_Demandas[pt_Disc].second;
//
//      //   //cout << "\t======= " << endl;
//      //   //cout << "\tDisciplina: " << pt_Disc->codigo << endl;
//      //   //cout << "\t  Demanda nao alocada: " << disc_Cjts_Demandas[pt_Disc].second << endl;
//
//      //   //cout << "\t  num_Turmas_Abertas: " << num_Turmas_Abertas << endl;
//      //   //cout << "\t  Max Turmas: " << pt_Disc->num_turmas << endl;
//      //}
//
//      /* Se não consigo mais alocar em nenhuma sala ou se 
//      toda a demanda já foi alocada, paro.
//      */
//      if(!alocou_Em_Alguma_Sala || disc_Cjts_Demandas[pt_Disc].first.at(cjt_Dem_Esc.second).second <= 0)
//      { 
//         terminou_Alocacao = true;
//      }
//   }
//
//   //demanda_Nao_Atendida += 1;
//   //demanda_Nao_Atendida += disc_Cjts_Demandas[pt_Disc].second;
//}

void Solucao::organizaIS_Salas(vector<IS_Sala*> & is_Salas_Comaptiveis)
{
   // GULOSO

   sort(is_Salas_Comaptiveis.begin(),is_Salas_Comaptiveis.end(),ordenaIS_Salas);

   // ALEATORIO

   //random_shuffle(is_Salas_Comaptiveis.begin(),is_Salas_Comaptiveis.end());
}

vector<pair<Demanda*,int/*Demanda atendida*/> > Solucao::elaboraTurma(
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
   { cerr << "Tentando acessar uma disciplina que nao existe !! Solucao::elaboraTurma(...)" << endl; exit(1); }

   return qtd_Dem_Atd_Cjt;
}

void Solucao::organizaVariacoesRegraCredito(vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > & variacoes_Regra_Credito)
{
   return;
}

void Solucao::imprimeInfo()
{
   int demanda_Atendida = 0;
   //int demanda_Nao_Atendida = 0;

   //map<Disciplina*,
   //   pair<vector<pair<vector<pair<Demanda*,int/*Demanda específica ainda não atendida*/> >,
   //   int/*Somatório das demandas ainda não atendidas para um determinado conjunto*/> >,
   //   int/*Somatório das demandas ainda não atendidas de todos os conjuntos para uma dada disciplina*/ > >
   //   ::iterator

   //   it_Disc_Cjts_Demandas = disc_Cjts_Demandas.begin();

   //for(; it_Disc_Cjts_Demandas != disc_Cjts_Demandas.end(); ++it_Disc_Cjts_Demandas)
   //{
   //   cout << " ==================== " << endl;
   //   cout << "Disciplina: " << it_Disc_Cjts_Demandas->first->codigo << endl;

   //   vector<pair<vector<pair<Demanda*,int/*Demanda específica ainda não atendida*/> >,
   //      int/*Somatório das demandas ainda não atendidas para um determinado conjunto*/> >
   //      ::iterator

   //      it_Cjts_Demanda_Disciplina = it_Disc_Cjts_Demandas->second.first.begin();

   //   int cnt_cjt_Dem = 0;

   //   int cjt_Dem_Nao_At = 0;

   //   for(; it_Cjts_Demanda_Disciplina != it_Disc_Cjts_Demandas->second.first.end(); ++it_Cjts_Demanda_Disciplina)
   //   {
   //      cout << "  Cjt Dem : " << cnt_cjt_Dem++ << " Dem. Nao Atendida: " << it_Cjts_Demanda_Disciplina->second << endl;

   //      if( it_Cjts_Demanda_Disciplina->second > 0)
   //      { cjt_Dem_Nao_At += it_Cjts_Demanda_Disciplina->second; }
   //   }

   //   // ---

   //   ITERA_GGROUP(it_IS_Campus,solucao,IS_Campus)
   //   {
   //      ITERA_GGROUP(it_IS_Unidade,it_IS_Campus->unidades,IS_Unidade)
   //      {
   //         ITERA_GGROUP(it_IS_Sala,it_IS_Unidade->salas,IS_Sala)
   //         {
   //            if( problem_Data->discSalas[it_Disc_Cjts_Demandas->first->getId()].find
   //               (it_IS_Sala->sala) != problem_Data->discSalas[it_Disc_Cjts_Demandas->first->getId()].end())
   //            {
   //               cout << " ==================== " << endl;
   //               cout << "Sala: " << it_IS_Sala->sala->codigo << endl;
   //               cout << "  cap: " << it_IS_Sala->sala->capacidade << endl;

   //               ITERA_GGROUP(it_IS_Dia,it_IS_Sala->is_Dia_Semana,IS_Dia_Semana)
   //               {
   //                  cout << "  Dia: " << it_IS_Dia->creditos_Disponiveis->dia_semana << endl;
   //                  cout << "  Cred Livres: " << it_IS_Dia->creditos_Livres << endl;

   //                  //ITERA_GGROUP(it_IS_At_Tat,it_IS_Dia->is_Atendimentos_Taticos,IS_Atendimento_Tatico)
   //                  {
   //                  }
   //               }
   //            }
   //         }
   //      }
   //   }

   //   // ---

   //   //cout << "\n\n";

   //   if(cjt_Dem_Nao_At > 0) getchar();

   //   //demanda_NAO_Atendida += it_Disc_Cjts_Demandas->second.second;
   //}

   //ITERA_GGROUP(it_IS_Campus,solucao,IS_Campus)
   //{
   //   ITERA_GGROUP(it_IS_Unidade,it_IS_Campus->unidades,IS_Unidade)
   //   {
   //      ITERA_GGROUP(it_IS_Sala,it_IS_Unidade->salas,IS_Sala)
   //      {
   //         cout << " ==================== " << endl;
   //         cout << "Sala: " << it_IS_Sala->sala->codigo << endl;
   //         cout << "  cap: " << it_IS_Sala->sala->capacidade << endl;

   //         ITERA_GGROUP(it_IS_Dia,it_IS_Sala->is_Dia_Semana,IS_Dia_Semana)
   //         {
   //            cout << "  Dia: " << it_IS_Dia->creditos_Disponiveis->dia_semana << endl;
   //            cout << "  Cred Livres: " << it_IS_Dia->creditos_Livres << endl;

   //            //ITERA_GGROUP(it_IS_At_Tat,it_IS_Dia->is_Atendimentos_Taticos,IS_Atendimento_Tatico)
   //            {
   //            }
   //         }
   //      }
   //   }
   //}

   cout << "Demanda NAO atendida: " << demanda_Nao_Atendida << endl;

   //getchar();
}

ProblemData & Solucao::get_Problem_Data() const
{
   return *problem_Data;
}

GGroup<IS_Campus*> const & Solucao::getSolucao() const
{
   return solucao;
}

int Solucao::getNumDemandasAtendidas() const
{
   return num_Demandas_Atendidas;
}

int Solucao::getNumDemandas_NAO_Atendidas() const
{
   return num_Demandas_NAO_Atendidas;
}

//void Solucao::geraSolucao()
//{
//   cout << "Gerando uma solucao" << endl;
//
//   // ---------------------------------
//
//   // Agrupando as demandas
//   agrupaDemandas();
//
//   // ---------------------------------
//
//   // Iterador
//   map<Disciplina*,vector<pair<GGroup<Demanda*>,int/*Demanda ainda não atendida*/> > >::iterator
//      it_Disc_Cjts_Demandas;
//
//   // ---------------------------------
//
//   // Alocando as demandas às salas
//
//   /* PASSO 1
//
//   Referencio todos os conjuntos de demandas de cada disciplina.
//   Agora, basta ordenar esse conjunto. Sempre que uma demanda seja atendida, seja parcialmente
//   ou totalmente, uma reordenação será realizada.
//
//   Obs: Caso um conjunto de demandas de uma disciplina já tenha sido totalmente atendido,
//   o ponteiro para o mesmo será inicializado com NULL. Ao ordenar a estrutura, os 
//   ponteiros NULL deverão ficar ao final da mesma, sendo desconsiderados.
//
//   EXEMPLO:
//   Considere o caso em que 3 disciplinas possuem os seguintes conjuntos de demandas (já ordenados para cada disc):
//
//   Disc 1 -> [dem:80] ; [dem:45]
//   Disc 2 -> [dem:50] ; [dem:47]
//   Disc 3 -> [dem:30]
//
//   A estrutura <cjts_Dem> inicialmente ficaria assim:
//
//   [ [dem:80], [dem:50], [dem:47], [dem:45], [dem:30] ]
//
//   Digamos que a primeira demanda seja totalmente atendida.
//
//   -> [ [dem:50], [dem:47], [dem:45], [dem:30], NULL ]
//
//   Agora, que a nova primeira demanda seja atendida parcialmente em 15
//
//   [dem:50] <- [dem:50] - 15 => [dem:35]
//
//   -> [ [dem:47], [dem:45], [dem:50], [dem:30], NULL ]
//
//   E assim por diante, até que todos os ponteiros sejam NULL. Seja pq todas as demandas foram atendidas,
//   ou pq não há mais como alocar.
//
//   */
//
//   // ---
//
//   /* 
//   Estrutura que armazena os indices necessários para acessar todos os conjuntos de 
//   demandas de cada disciplina. 
//   Obs: Posso descobrir a disciplina via a <Demanda*>.
//   */
//
//   vector<pair<pair<Disciplina*,int/*indice do conjunto*/>, int * /*Valor da demanda*/ > > cjts_Dem;
//
//   // Inicializando a estrutura
//   carregaEstruturaCjtsDem(cjts_Dem);
//
//   // ----
//
//   // Para cada conjunto de demandas a ser atendido
//   for(int cnt_Cjt_Dems = 0; cnt_Cjt_Dems < total_Cjt_Demandas; cnt_Cjt_Dems++)
//   {
//      // Escolhendo o conjunto de demandas a ser atendido
//      pair<Disciplina*,int/*indice do conjunto*/> cjt_Demanda_Escolhido =
//         escolheConjuntoDeDemadasParaAlocarASala(cjts_Dem);
//
//      /* PASSO 2
//
//      De posse da estrutura <cjts_Dem> ordenada, agora tentaremos alocar a maior
//      demanda à sala disponível com maior capacidade que seja compativel com a 
//      disciplina em questão.
//
//      ATENÇÃO: a sala tem que ser compatível com a disciplina do conjunto de demandas
//      em questão. Ou seja, possuir capacidade maior ou igual à demanda total e possuir os
//      mesmos dias letivos. Utilizar a estrutura <disc_IS_Salas> instanciada e inicializada acima.
//
//      OBS: Deve-se tentar respeitar as regras de crédito de cada disciplina.
//
//      Devo ordenar as salas por capacidade
//      Em seguida:
//      Até que todas as salas compatíveis tenha sido percorridas:
//      Seleciono uma sala e tento alocar
//      Se não consegui alocar em nenhuma sala é pq não há nenhuma sala
//      com os horarios disponíveis. Pode-se deixar sem alocar, ou fragmentar os
//      creditos da disciplina para tentar alocar.
//
//      */
//
//      // Criando um vetor de IS_Salas compatíveis com o conjunto de demandas em questão.
//      vector<IS_Sala*> is_Salas_Comaptiveis;
//
//      Disciplina * pt_Disc = cjt_Demanda_Escolhido.first;
//
//      // Inicializando o vetor <is_Salas_Comaptiveis>
//      ITERA_GGROUP(it_Sala,problem_Data->discSalas.find(pt_Disc->getId())->second,Sala)
//      { is_Salas_Comaptiveis.push_back(map_Sala__IS_Sala.find(*it_Sala)->second); }
//
//      aloca(cjt_Demanda_Escolhido,is_Salas_Comaptiveis);
//
//      demanda_Nao_Atendida += disc_Cjts_Demandas[cjt_Demanda_Escolhido.first].second;
//   }
//
//   //cout << "demanda_Nao_Atendida: " << demanda_Nao_Atendida << endl;
//
//   cout << "Solucao gerada com sucesso.\n";
//}

//TRIEDA-568
//TRIEDA-558
void Solucao::geraSolucaoSubBlocos()
{
   cout << "Gerando uma solucao que admite a presenca de subblocos" << endl;

   // ---------------------------------

   // Agrupando as demandas
   agrupaDemandas();

   //map<Disciplina*,
   //   pair<vector<pair<vector<pair<Demanda*,int/*Demanda específica ainda não atendida*/> >,
   //   int/*Somatório das demandas ainda não atendidas para um determinado conjunto*/> >,
   //   int/*Somatório das demandas ainda não atendidas de todos os conjuntos para uma dada disciplina*/ > >
   //   ::iterator

   //   it_Disc_Cjts_Demandas = disc_Cjts_Demandas.begin();

   //for(; it_Disc_Cjts_Demandas != disc_Cjts_Demandas.end(); ++it_Disc_Cjts_Demandas)
   //{
   //   if(it_Disc_Cjts_Demandas->second.first.size() > 1)
   //   {
   //      cout << "hello." << endl;
   //      getchar();
   //   }

   //   vector<pair<vector<pair<Demanda*,int/*Demanda específica ainda não atendida*/> >,
   //   int/*Somatório das demandas ainda não atendidas para um determinado conjunto*/> >
   //   ::iterator

   //   it_Demandas = it_Disc_Cjts_Demandas->second.first.begin();

   //   for(;it_Demandas != it_Disc_Cjts_Demandas->second.first.end(); ++it_Demandas)
   //   {
   //      if(it_Demandas->first.size() > 2)
   //      {
   //         //getchar();
   //      }
   //   }
   //}

   // ---------------------------------

   // OBS: Poderia ficar no construtor !!!

   // Criando os is_Blocos_Curriculares
   criaIS_Blocos_Curriculares();

   // ---------------------------------

   // Alocando primeiro as disciplinas comuns a cursos compatíveis

   //alocaDiscsComuns();

   // ---------------------------------

   // OBS: Poderia ficar no construtor !!!

   // Criando os IS_Cursos

   ITERA_GGROUP(it_Curso,problem_Data->cursos,Curso)
   { is_Cursos.push_back(new IS_Curso(*it_Curso,problem_Data)); }

   // ---------------------------------

   // Organizando os blocos curriculares segundo algum critério
   organizaIS_Blocos_Curriculares(is_Blocos_Curriculares);

   // ---------------------------------

   /* Para cada IS_Bloco a ser atendido, tenta-se atender o máximo de demanda possível. */
   //for(unsigned cnt_IS_Blc = 0; cnt_IS_Blc < is_Blocos_Curriculares.size(); ++cnt_IS_Blc)
   ITERA_VECTOR(it_IS_Bloco_Curricular,is_Blocos_Curriculares,IS_Bloco_Curricular)
   {
      //IS_Bloco_Curricular * pt_IS_Bloco = escolheIS_Bloco_Curricular();

      //if(!pt_IS_Bloco)
      //{ break; } // Não há mais IS_Bloco para alocar

      // ---------------------------------

      map<Disciplina*,int/*Demanda nao atendida*/>::iterator
         it_Disciplina_Dem_Nao_Atendida = (*it_IS_Bloco_Curricular)->disciplina_Dem_Nao_Atendida.begin();

      //cout << " =========================================== " << endl;
      //getchar();

      // Para cada disciplina de um bloco - POR ENQUANTO, SEQUENCIAL !!!
      for(;it_Disciplina_Dem_Nao_Atendida != (*it_IS_Bloco_Curricular)->disciplina_Dem_Nao_Atendida.end(); ++it_Disciplina_Dem_Nao_Atendida)
      {
         // ---------------------------------

         // Contabilizando o número de turmas abertas para a disciplina em questão.

         int num_Turmas_Abertas = 0;

         // ---------------------------------

         // Criando um vetor de IS_Salas compatíveis com o conjunto de demandas em questão.
         vector<IS_Sala*> is_Salas_Comaptiveis;

         Disciplina * pt_Disc = it_Disciplina_Dem_Nao_Atendida->first;

         // Inicializando o vetor <is_Salas_Comaptiveis>
         ITERA_GGROUP(it_Sala,problem_Data->discSalas.find(pt_Disc->getId())->second,Sala)
         { is_Salas_Comaptiveis.push_back(map_Sala__IS_Sala.find(*it_Sala)->second); }

         // ---------------------------------

         // Organizando as salas segundo algum critério
         organizaIS_Salas(is_Salas_Comaptiveis);

         // ---------------------------------

         /*
         Se a disciplina em questão possui regra de crédito, tenta-se alocar 
         com a regra de crédito informada. Caso a disciplina não possua regra de 
         crédito, uma regra de crédito é sugerida.
         */

         vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > * 
            variacoes_Regra_Credito = 
            getRegradeCreditoEVariacoes(it_Disciplina_Dem_Nao_Atendida->first);

         // ---------------------------------

         /*
         Pode acontecer que nem a regra de crédito informada e a regra de 
         crédito sugerida sejam compatíveis com a sala em questão. Além disso,
         a regra de crédito em questão pode não ser compatível com nenhuma sala.
         Quando isso ocorre, deve-se tentar alocar a disciplina do jeito que der,
         ou seja, sem que exista uma regra de crédito para guiar a alocação.

         Ainda assim, pode acontecer de alguma disciplina não ser alocada, seja
         parcialmente ou totalmente.
         */

         // FLAG que indica se foi realizada alguma alocação
         //bool alocou_Alguma_Sala = false;

         // FLAG que indica o término da alocação de uma disciplina
         //bool terminou_Alocacao_Disciplina = false;

         // Referenciando a demanda não atendida da disciplina em questão
         int * dem_Nao_Atd = &it_Disciplina_Dem_Nao_Atendida->second;

         int id_Disc = it_Disciplina_Dem_Nao_Atendida->first->getId();

         //while(!terminou_Alocacao_Disciplina)
         while((*dem_Nao_Atd) > 0)
         {
            // Verificando a grade horaria para a disciplina do curso em questão.

            // ---------------------------------

            IS_Curso * is_Curso = NULL;

            // Procurando pelo IS_Curso
            ITERA_VECTOR(it_IS_Curso,is_Cursos,IS_Curso)
            {
               if((*it_IS_Curso)->curso == (*it_IS_Bloco_Curricular)->bloco->curso)
               {
                  is_Curso = *it_IS_Curso;
                  break;
               }
            }

            if(!is_Curso) {cout << "Solucao::geraSolucaoSubBlocos() -> Curso Invalido" << endl; exit(1);}

            // ---------------------------------

            // Obtendo a chave para acessar a grade horaria correta do curso em questão.
            pair<Curriculo*,int/*periodo*/> chave (
               (*it_IS_Bloco_Curricular)->bloco->curriculo,(*it_IS_Bloco_Curricular)->bloco->periodo);

            if(is_Curso->grade_Horaria_Periodo.find(chave) == is_Curso->grade_Horaria_Periodo.end())
            {
               cout << "Solucao::geraSolucaoSubBlocos() -> Chave Invalida." << endl;
               exit(1);
            }

            // Referenciando a grade horária
            Matrix<vector<pair<Disciplina*,int/*turma*/> > > * pt_Grade_Horaria_Curso =
               is_Curso->grade_Horaria_Periodo[chave];

            // ---------------------------------

            // Organizando as varições da regra de crédito em questão

            // OBS: Por eqto não faz nada. Deixa do jeito que foram geradas.

            organizaVariacoesRegraCredito(*variacoes_Regra_Credito);

            // ---------------------------------

            vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >::iterator
               it_Variacoes_Regra_Credito = variacoes_Regra_Credito->begin();
            
            it_Variacoes_Regra_Credito =
               is_Curso->verificaDisponibilidadeSemSubBloco(
               *variacoes_Regra_Credito,
               it_Variacoes_Regra_Credito,
               chave,
               pt_Disc);

            // ---------------------------------

            /* Verificando se alguma das regras de crédito se aplica na grade horaria em questão
            sem que nenhum subbloco seja gerado. */
            while(it_Variacoes_Regra_Credito != variacoes_Regra_Credito->end())
            {
               // ---------------------------------

               ITERA_VECTOR(it_IS_Sala_Compat,is_Salas_Comaptiveis,IS_Sala)
               {
                  // Checando se a regra é válida para a sala em questão
                  if((*it_IS_Sala_Compat)->regraValida(*it_Variacoes_Regra_Credito))
                  {
                     // ALOCAR !!!

                     // ---------------------------------

                     // Calculando a demanda a ser atendida
                     int demanda_Atendida = (
                        (*it_IS_Sala_Compat)->sala->capacidade >= (*dem_Nao_Atd)
                        ?
                        (*dem_Nao_Atd)
                        : (*it_IS_Sala_Compat)->sala->capacidade);

                     // ---------------------------------

                     // Obtendo uma referência para a demanda a ser alocada.

                     Demanda * pt_Demanda = 
                        (*it_IS_Bloco_Curricular)->bloco->disciplina_Demanda[pt_Disc];

                     // ---------------------------------

                     // Alocando

                     (*it_IS_Sala_Compat)->aloca(
                        num_Turmas_Abertas,
                        pt_Demanda,
                        demanda_Atendida,
                        *it_Variacoes_Regra_Credito);

                     // ---------------------------------

                     // Fazendo algumas atualizações.

                     /* Atualizando a grade curricular do curso em questão. */
                     is_Curso->aloca(
                        num_Turmas_Abertas,
                        pt_Disc,
                        *it_Variacoes_Regra_Credito,
                        pt_Grade_Horaria_Curso
                        );

                     // Atualizando a demanda não atendida
                     (*dem_Nao_Atd) -= demanda_Atendida;

                     // Atualizando o número total de turmas abertas
                     ++num_Turmas_Abertas;

                     // ---------------------------------

                     break; // Aloquei em alguma sala, por isso paro.
                  }
               }

               // ---------------------------------

               /*
               Verificando se a disciplina da variação da regra de crédito em questão ainda pode ser 
               alocada na grade horária sem que se tenha a necessidade de criar um subbloco.

               Só faço isso se tiver alguma demanda para alocar. Se não, paro de procurar por regras
               de crédito válidas
               */

               if((*dem_Nao_Atd) <= 0)
               { break; }
               else
               {
                  it_Variacoes_Regra_Credito = 

                     is_Curso->verificaDisponibilidadeSemSubBloco(
                     *variacoes_Regra_Credito,
                     (++it_Variacoes_Regra_Credito),
                     chave,
                     pt_Disc);
               }
            }

            if((*dem_Nao_Atd) > 0)
            {
               /*
               Se está aqui, é pq todas as variações da regra de crédito já foram aplicadas (quando possível)
               e mesmo assim, ainda há demanda para ser alocada.
               */

               demanda_Nao_Atendida += *dem_Nao_Atd;

               //cout << "Demanda da disciplina <" << it_Disciplina_Dem_Nao_Atendida->first->codigo 
               //   << "> ainda nao atendida totalmente .. ." << endl;
               // Por eqto, deixo pra lá.
               break;
            }
         }
      }
   }

   // ---------------------------------
   //ITERA_VECTOR(it_IS_Curso,is_Cursos,IS_Curso)
   //{
      //(*it_IS_Curso)->imprimeGradesHorarias();
      //cout << endl << endl;
   //}
   // ---------------------------------

   cout << "Solucao gerada com sucesso.\n";
}

pair<vector<int>*,vector<double>*> Solucao::repSolIniToVariaveis(VariableHash & v_Hash, int lp_Cols, OPT_LP & lp)
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

//pair<int*,double*> Solucao::repSolIniToVariaveis(VariableHash & v_Hash, int lp_Cols)
//void Solucao::repSolIniToVariaveis(VariableHash & v_Hash, int lp_Cols)
//pair<vector<int>*,vector<double>*> Solucao::repSolIniToVariaveis(VariableHash & v_Hash, int lp_Cols)
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

//pair<int*,double*> Solucao::repSolIniParaVariaveis(VariableHash & v_Hash, int lp_Cols)
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


void Solucao::alocaDiscsComuns()
{
   // Estrutura que armazena ponteiros para cada chave existente na estrutura <realacao_Turmas>
   //vector<pair<Disciplina*,int/*Turma*/>*> vt_Chaves_Relacao_Turmas;

   map<Disciplina*,vector<pair<Demanda*,int/*Dem a ser atendida*/> > >::iterator
      it_Relacao_Alunos_Disciplinas = relacao_Alunos_Disciplinas.begin();

   // Para cada disciplina
   for(; it_Relacao_Alunos_Disciplinas != relacao_Alunos_Disciplinas.end(); ++it_Relacao_Alunos_Disciplinas)
   {
      // ---------------------------------

      // Contabilizando o número de turmas abertas para a disciplina em questão.

      int num_Turmas_Abertas = 0;

      // ---------------------------------

      // Criando um vetor de IS_Salas compatíveis com o conjunto de demandas em questão.
      vector<IS_Sala*> is_Salas_Comaptiveis;

      Disciplina * pt_Disc = it_Relacao_Alunos_Disciplinas->first;

      // Inicializando o vetor <is_Salas_Comaptiveis>
      ITERA_GGROUP(it_Sala,problem_Data->discSalas.find(pt_Disc->getId())->second,Sala)
      { is_Salas_Comaptiveis.push_back(map_Sala__IS_Sala.find(*it_Sala)->second); }

      // ---------------------------------

      // Organizando as salas segundo algum critério
      organizaIS_Salas(is_Salas_Comaptiveis);

      // ---------------------------------

      /*
      Se a disciplina em questão possui regra de crédito, tenta-se alocar 
      com a regra de crédito informada. Caso a disciplina não possua regra de 
      crédito, uma regra de crédito é sugerida.
      */

      vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > * 
         variacoes_Regra_Credito = 
         getRegradeCreditoEVariacoes(pt_Disc);

      // ---------------------------------

      //Contabilizando a demanda que deve ser atendida

      int demanda_Total = 0;

      vector<pair<Demanda*,int/*Dem a ser atendida*/> >::iterator 
         it_Demandas = it_Relacao_Alunos_Disciplinas->second.begin();

      for(; it_Demandas != it_Relacao_Alunos_Disciplinas->second.end(); ++it_Demandas)
      { demanda_Total += it_Demandas->second; }

      // ---------------------------------

      while(demanda_Total > 0)
      {


   // *****************


            //// ---------------------------------

            //IS_Curso * is_Curso = NULL;

            //// Procurando pelo IS_Curso
            //ITERA_VECTOR(it_IS_Curso,is_Cursos,IS_Curso)
            //{
            //   if((*it_IS_Curso)->curso == (*it_IS_Bloco_Curricular)->bloco->curso)
            //   {
            //      is_Curso = *it_IS_Curso;
            //      break;
            //   }
            //}

            //if(!is_Curso) {cout << "Solucao::geraSolucaoSubBlocos() -> Curso Invalido" << endl; exit(1);}

            //// ---------------------------------

            //// Obtendo a chave para acessar a grade horaria correta do curso em questão.
            //pair<Curriculo*,int/*periodo*/> chave (
            //   (*it_IS_Bloco_Curricular)->bloco->curriculo,(*it_IS_Bloco_Curricular)->bloco->periodo);

            //if(is_Curso->grade_Horaria_Periodo.find(chave) == is_Curso->grade_Horaria_Periodo.end())
            //{
            //   cout << "Solucao::geraSolucaoSubBlocos() -> Chave Invalida." << endl;
            //   exit(1);
            //}

            //// Referenciando a grade horária
            //Matrix<vector<pair<Disciplina*,int/*turma*/> > > * pt_Grade_Horaria_Curso =
            //   is_Curso->grade_Horaria_Periodo[chave];

            //// ---------------------------------

            //// Organizando as varições da regra de crédito em questão

            //// OBS: Por eqto não faz nada. Deixa do jeito que foram geradas.

            //organizaVariacoesRegraCredito(*variacoes_Regra_Credito);

            //// ---------------------------------

            //vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >::iterator
            //   it_Variacoes_Regra_Credito = variacoes_Regra_Credito->begin();
            //
            //it_Variacoes_Regra_Credito =
            //   is_Curso->verificaDisponibilidadeSemSubBloco(
            //   *variacoes_Regra_Credito,
            //   it_Variacoes_Regra_Credito,
            //   chave,
            //   pt_Disc);



         ///*****************************

         // monto a turma

         
      }
   }
   
   //cout << "TESTE: " << relacao_Alunos_Disciplinas.size() << endl;

   //getchar();

   //map<Disciplina*,GGroup<Demanda*> >::iterator 
   //   it_Disciplinas_Comuns_Entre_Demandas_Cursos_Compat = 
   //   disciplinas_Comuns_Entre_Demandas_Cursos_Compat.begin();

   //Gero uma lista de IS_Cursos a partir das demandas.
   //   Depois saio verificando as grades dos IS_Cursos por horários compatíveis
   //   Crio as turmas na maior sala possível
}

bool Solucao::operator == (Solucao const & right)
{
   return demanda_Nao_Atendida == right.demanda_Nao_Atendida;
}

bool Solucao::operator != (Solucao const & right)
{
   return !(*this == right);
}

bool Solucao::operator < (Solucao const & right)
{
   return demanda_Nao_Atendida < right.demanda_Nao_Atendida;
}

bool Solucao::operator > (Solucao const & right)
{
   return !(*this < right);
}