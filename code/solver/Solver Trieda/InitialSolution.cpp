#include "InitialSolution.h"

//bool ordena_dec_demanda(Demanda const * left, Demanda const * right)
bool ordena_dec_demanda(pair<Demanda const *, int> const & left, pair<Demanda const *, int> const & right)
{
   //return (left->quantidade > right->quantidade);
   return (left.second > right.second);
}

bool ordena_dec_sala(Sala const * left, Sala const * right)
{
   return (left->capacidade > right->capacidade);
}

InitialSolution::InitialSolution(ProblemData & _problem_Data) :
problem_Data(&_problem_Data)
{
   cout << "Gerando uma solucao inicial" << endl;

   // Copiando as demandas para poder ordena-las
   ITERA_GGROUP(it_Demanda,problem_Data->demandas,Demanda)
   //{ vt_Demandas.push_back(new Demanda(**it_Demanda)); }
   { vt_Demandas.push_back(make_pair(*it_Demanda,it_Demanda->quantidade)); }

   // Ordenando as demandas em ordem decrescente
   sort(vt_Demandas.begin(),vt_Demandas.end(),ordena_dec_demanda);

   num_Demandas_Atendidas = 0;
   num_Demandas_NAO_Atendidas = 0;

   // Criando as estruturas IS_*
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
               int dia = it_Creds_Disp->dia_semana;

               int creds_Livres = it_Sala->credsLivres.at(dia);

               pair<Demanda*,pair<int,int> > horario_a_ser_Alocado (NULL,
                  make_pair(0,0));

               vector<pair<Demanda*,pair<int,int> > > horarios_do_Dia_Letivo 
                  (creds_Livres,horario_a_ser_Alocado);

               is_sala->atendimento_Tatico[dia] = 
                  make_pair<int/*credsLivres*/,vector<pair<Demanda*,pair<int,int> > > > 
                  (creds_Livres, horarios_do_Dia_Letivo);
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
   num_Demandas_Atendidas = init_sol.getNumDemandasAtendidas();
   num_Demandas_NAO_Atendidas = init_sol.getNumDemandas_NAO_Atendidas();
}

InitialSolution::~InitialSolution(void)
{
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

bool InitialSolution::todasDemandasAtendidas() const
{
   //if(num_Demandas_Atendidas == vt_Demandas.size()) Dá tb, mas prefiro usar assim:
   if(num_Demandas_NAO_Atendidas == 0)
      return true;
   return false;
}

bool InitialSolution::tentouAtenderTodasDemandas() const
{
   int total_Atendimento_Demandas = 
      num_Demandas_Atendidas + num_Demandas_NAO_Atendidas;

   if(total_Atendimento_Demandas == vt_Demandas.size())
      return true;
   return false;
}

void InitialSolution::generate_Initial_Solution()
{
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

   // ---

   // Criando uma estrutura que copie as salas e ordene-as de acordo com a respectiva capacidade
   std::map<int/*Id Disc*/,vector<Sala*> > disc_Salas;

   std::map<int/*Id Disc*/,GGroup<Sala*> >::iterator it_Disc_Salas =
      problem_Data->discSalas.begin();

   for(; it_Disc_Salas != problem_Data->discSalas.end(); it_Disc_Salas++)
   { 
      ITERA_GGROUP(it_Sala,it_Disc_Salas->second,Sala)
      {
         disc_Salas[it_Disc_Salas->first].push_back(*it_Sala);
      }
   }

   // Ordenando as salas por capacidade, para cada disciplina
   std::map<int/*Id Disc*/,vector<Sala*> >::iterator it_Disc_VT_Salas =
      disc_Salas.begin();

   for(; it_Disc_VT_Salas != disc_Salas.end(); it_Disc_VT_Salas++)
   { sort(it_Disc_VT_Salas->second.begin(),it_Disc_VT_Salas->second.end(),ordena_dec_sala); }

   // ---

   // Criando uma estrutura que conta o número de turmas geradas para cada disciplina
   std::map<Disciplina*,int/*num turmas abertas*/> disc_Turmas;

   // Inicializando
   ITERA_GGROUP(it_Disc,problem_Data->disciplinas,Disciplina)
   { disc_Turmas[*it_Disc] = 0; }

   // ---

   // Tentando alocar todas as demandas. Pode ser que parte de alguma (ou toda a) demanda não seja alocada
   //while(!todasDemandasAtendidas())
   while(!tentouAtenderTodasDemandas())
   {
      vector<pair<Demanda*,int> >::iterator it_vt_Demandas =
         vt_Demandas.begin();

      for(; it_vt_Demandas != vt_Demandas.end(); it_vt_Demandas++)
      {
         // Se a demanda ainda não foi atendida
         if(it_vt_Demandas->second > 0)
         {
            Disciplina * pt_Disc = it_vt_Demandas->first->disciplina;

            // um dos 2 vai ser sempre 0.
            int total_Creditos = pt_Disc->cred_praticos + pt_Disc->cred_teoricos;

            /* Utilizada para indicar se uma demanda foi atendida, pelo menos em parte, em uma iteração
            das salas compatíveis. Se, em uma iteração dessas a demanda não for atendida (em pelo menos,
            o tamanho da menor sala) significa que não há salas com horários vagos suficientes para podermos
            alocar a demanda. 
                        
            -------

            Dá pra dividir a demanda e alocar separado por sala. Fica pra um TRIEDA FUTURO !!!
            
            */
            bool alocou_Demanda = false;

            // p tds salas que possuem a disc associada
            ITERA_VECTOR(it_Disc_Salas,disc_Salas[pt_Disc->getId()],Sala)
            {
               // Selecionando a IS_Sala correspondente à Sala em questão.
               IS_Sala * is_Sala = map_Sala__IS_Sala[*it_Disc_Salas];

               pair<int/*idDisc*/,int/*idSala*/> ids_Disc_Sala 
                  (pt_Disc->getId(),is_Sala->getId());

               // p tds os dias letivos comuns entre a disc e a sala selecionadas
               ITERA_GGROUP_N_PT(it_Dia_Let_Disc_Sala,problem_Data->disc_Salas_Dias[ids_Disc_Sala],int)
               {
                  /* Se a sala em questao possui, no mínimo, tantos horarios (creditos) vagos quanto o total
                  de créditos da disciplina em questão. */
                  if(is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].first >= total_Creditos
                     && (disc_Turmas[pt_Disc] < pt_Disc->num_turmas)
                     )
                  {
                     alocou_Demanda = true;

                     // Adicionando o indice da turma
                     //++disc_Turmas[pt_Disc];

                     // === ===

                     //int demanda_Atendida = 
                     //   //(*it_vt_Demandas)->quantidade / is_Sala->sala->capacidade;
                     //   it_vt_Demandas->second / is_Sala->sala->capacidade;

                     //// Caso a demanda seja inferior à cap. da sala em questão
                     //demanda_Atendida = 
                     //   //(demanda_Atendida > 0 ? demanda_Atendida : (*it_vt_Demandas)->quantidade);
                     //   (demanda_Atendida > 0 ? demanda_Atendida : it_vt_Demandas->second);

                     int demanda_Atendida =
                        (it_vt_Demandas->second <= is_Sala->sala->capacidade ? // Ou seja, cabe na sala?
                        it_vt_Demandas->second : -1);

                     demanda_Atendida = (demanda_Atendida == -1 ? 
                        (it_vt_Demandas->second - (it_vt_Demandas->second % is_Sala->sala->capacidade))
                        : demanda_Atendida );

                     // === ===

                     // Para cada credito a ser atendido
                     for(int cred = 0; cred < total_Creditos; cred++)
                     {
                        // Encontrando a posicao correta para inserir
                        int pos = is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].second.size()
                           - is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].first;

                        // Sutraindo o credito alocado dos creditos livres.
                        is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].first--;

                        // Referenciando a disciplina que está sendo alocada
                        //is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].second.at(pos).first =
                           //pt_Disc;

                        // Referenciando a demanda que está sendo parcialmente, ou totalmente atendida.
                        is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].second.at(pos).first = 
                           it_vt_Demandas->first;

                        // Armazenando a demanda (parcial ou total) atendida
                        //is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].second.at(pos).second = 
                        //dem_a_ser_Atendida;
                        is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].second.at(pos).second =
                           make_pair(disc_Turmas[pt_Disc],demanda_Atendida);
                     }              

                     /* Teste para o caso em que toda a demanda foi atendida */
                     //if(demanda_Atendida <= is_Sala->sala->capacidade)
                     if( (it_vt_Demandas->second - demanda_Atendida) == 0 )
                     {
                        // Se está aqui, é pq toda a demanda foi atendida
                        num_Demandas_Atendidas++;

                        // Para não atender denovo
                        //(*it_vt_Demandas)->quantidade = 0;
                        it_vt_Demandas->second = 0;
                     }
                     else
                     {
                        // Se está aqui, é pq NEM toda a demanda foi atendida

                        // Atualizando a demanda que ainda falta ser atendida
                        //(*it_vt_Demandas)->quantidade -= demanda_Atendida;
                        it_vt_Demandas->second -= demanda_Atendida;
                     }

                     // Reordenando as demandas em ordem decrescente
                     sort(vt_Demandas.begin(),vt_Demandas.end(),ordena_dec_demanda);

                     // Adicionando o indice da turma
                     ++disc_Turmas[pt_Disc];

                     break;
                  }

               }

               if(alocou_Demanda)
               { break; }
            }

            /* Caso a demanda não tenha sido atendida, devido ao fato de não existirem horários
            vagos suficientes em uma dada sala. */
            if(!alocou_Demanda)
            {
               //(*it_vt_Demandas)->quantidade = 0;
               it_vt_Demandas->second = 0;
               num_Demandas_NAO_Atendidas++;
            }
         }
      }
   }
}

pair<int*,double*> InitialSolution::repSolIniToVariaveis(VariableHash & v_Hash, int lp_Cols)
//void InitialSolution::repSolIniToVariaveis(VariableHash & v_Hash, int lp_Cols)
//pair<vector<int>*,vector<double>*> InitialSolution::repSolIniToVariaveis(VariableHash & v_Hash, int lp_Cols)
{
   //int * indices = new int (lp_Cols);
   int * indices = new int[lp_Cols];

   //vector<int> * indices = new vector<int>;

   //double * valores = new double (lp_Cols);
   double * valores = new double[lp_Cols];
   //vector<double> * valores = new vector<double>;

   /* Responsavel por armazenar os indices das vars do tipo V_CREDITOS que tiveram
   valor atribuido no processo de geração da solução inicial. */
   set<int> indices_V_CREDITOS;

   /* Responsavel por armazenar os indices das vars do tipo V_OFERECIMENTO que tiveram
   valor atribuido no processo de geração da solução inicial. */
   set<int> indices_V_OFERECIMENTO;

   /* Responsavel por armazenar os indices das vars do tipo V_ALUNOS que tiveram
   valor atribuido no processo de geração da solução inicial. */
   set<int> indices_V_ALUNOS;

   /* Responsavel por armazenar os indices das vars do tipo V_ALOC_ALUNO que tiveram
   valor atribuido no processo de geração da solução inicial. */
   set<int> indices_V_ALOC_ALUNO;

   int cnt = 0;

   // p/ cada campus
   ITERA_GGROUP(it_IS_Campus,solucao,IS_Campus)
   {
      map<vector<int/*indices de uma var do tipo V_ALUNOS*/>,
         pair<Variable,int/*valor var*/> > vars_Alunos;

      map<vector<int/*indices de uma var do tipo V_ALOC_ALUNO*/>,
         pair<Variable,int/*valor var*/> > vars_Aloc_Alunos;

      // p/ cada unidade
      ITERA_GGROUP(it_IS_Unidade,it_IS_Campus->unidades,IS_Unidade)
      {
         // p/ cada sala
         ITERA_GGROUP(it_IS_Sala,it_IS_Unidade->salas,IS_Sala)
         {
            map<int/*dia*/,pair<int/*credsLivres*/,
               vector<
               pair<Demanda*,
               pair<int/*Id Turma*/,int/*Demanda Atendida*/> > > > >::iterator

               it_Dia = it_IS_Sala->atendimento_Tatico.begin();

            // p/ cada dia do atendimento tatico em questao
            for(; it_Dia != it_IS_Sala->atendimento_Tatico.end(); it_Dia++)
            {
               map<vector<int/*indices de uma var do tipo V_CREDITOS ou V_OFERECIMENTO*/>,
                  pair<Variable,int/*valor var*/> > vars_Dia_Cred_OU_Oferc;

               // p/ cada credito do dia 
               for(unsigned cred = 0; cred < it_Dia->second.second.size(); cred++)
               {
                  // SOMENTE se o cred tiver sido alocado
                  if(it_Dia->second.second.at(cred).first) // != NULL
                  {
                     // ===

                     // Criando as variáveis e estruturas necessárias

                     Variable var_Cred_OU_Oferc;
                     vector<int> chave_Cred_OU_Oferc(5);

                     Variable var_Alunos;
                     vector<int> chave_Alunos(3);

                     Variable var_Aloc_Alunos;
                     vector<int> chave_Aloc_Alunos(4);

                     // Inicializando

                     var_Cred_OU_Oferc.reset();
                     var_Cred_OU_Oferc.setType(Variable::V_CREDITOS);

                     var_Alunos.reset();
                     var_Alunos.setType(Variable::V_ALUNOS);

                     var_Aloc_Alunos.reset();
                     var_Aloc_Alunos.setType(Variable::V_ALOC_ALUNO);

                     // Setando turma

                     var_Cred_OU_Oferc.setTurma( it_Dia->second.second.at(cred).second.first );
                     chave_Cred_OU_Oferc.at(0) = it_Dia->second.second.at(cred).second.first;

                     var_Alunos.setTurma( it_Dia->second.second.at(cred).second.first );
                     chave_Alunos.at(0) = it_Dia->second.second.at(cred).second.first;

                     var_Aloc_Alunos.setTurma( it_Dia->second.second.at(cred).second.first );
                     chave_Aloc_Alunos.at(0) = it_Dia->second.second.at(cred).second.first;

                     // Setando disciplina

                     var_Cred_OU_Oferc.setDisciplina( it_Dia->second.second.at(cred).first->disciplina );
                     chave_Cred_OU_Oferc.at(1) = it_Dia->second.second.at(cred).first->disciplina->getId();

                     var_Alunos.setDisciplina( it_Dia->second.second.at(cred).first->disciplina );
                     chave_Alunos.at(1) = it_Dia->second.second.at(cred).first->disciplina->getId();

                     var_Aloc_Alunos.setDisciplina( it_Dia->second.second.at(cred).first->disciplina );
                     chave_Aloc_Alunos.at(1) = it_Dia->second.second.at(cred).first->disciplina->getId();

                     // Setando campus

                     //var_Aloc_Alunos.setCampus( it_Dia->second.second.at


                     /*
                        CONTINUAR DAQUI

                        ASSIM QUE PEGA A OFERTA: it_Dia->second.second.at(cred).first->oferta

                        DAI PEGO O CAMPUS E O CURSO

                        PARA CONTINUAR, DESCOMENTAR AS LINHAS EM SOLVE() DO SOLVERMIP
                     
                     */
                        

                     // Setando unidade

                     var_Cred_OU_Oferc.setUnidade( it_IS_Unidade->unidade );
                     chave_Cred_OU_Oferc.at(2) = it_IS_Unidade->unidade->getId();

                     // Setando conjunto de sala (tps)

                     // ---

                     // Procurando o Conjunto de Salas correto.

                     int id = 
                        (it_IS_Sala->sala->tipo_sala->getId() == 1 ? 
                        it_IS_Sala->sala->capacidade : -it_IS_Sala->sala->capacidade);

                     ConjuntoSala * cjt_Sala = NULL;

                     bool found_CJT_Sala = false;

                     ITERA_GGROUP(it_Cjt_Sala,it_IS_Unidade->unidade->conjutoSalas,ConjuntoSala)
                     {
                        if(it_Cjt_Sala->getId() == id)
                        {
                           cjt_Sala = *it_Cjt_Sala;

                           found_CJT_Sala = true;

                           break;
                        }
                     }

                     if(!found_CJT_Sala)
                     { cout << "CJT SALA NAO ENCONTRADO" << endl; exit(1); }

                     chave_Cred_OU_Oferc.at(3) = id;

                     // ---

                     var_Cred_OU_Oferc.setSubCjtSala( cjt_Sala );

                     // Setando dia

                     var_Cred_OU_Oferc.setDia( it_Dia->first );
                     chave_Cred_OU_Oferc.at(4) = it_Dia->first;

                     // Setando oferta
                     var_Alunos.setOferta(problem_Data->refOfertas
                        [it_Dia->second.second.at(cred).first->oferta->getId()]);
                     chave_Alunos.at(2) = it_Dia->second.second.at(cred).first->oferta->getId();

                     // ===

                     // Logica para variaveis do tipo V_CREDITOS e V_OFERECIMENTO
                     if(vars_Dia_Cred_OU_Oferc.find(chave_Cred_OU_Oferc) == vars_Dia_Cred_OU_Oferc.end())
                     { vars_Dia_Cred_OU_Oferc[chave_Cred_OU_Oferc] = make_pair(var_Cred_OU_Oferc,1); }
                     else
                     {
                        if(var_Cred_OU_Oferc.getType() == Variable::V_CREDITOS)
                        { vars_Dia_Cred_OU_Oferc[chave_Cred_OU_Oferc].second += 1; }
                     }

                     // Logica para variavel do tipo V_ALUNOS
                     if(vars_Alunos.find(chave_Alunos) == vars_Alunos.end())
                     {
                        vars_Alunos[chave_Alunos] = make_pair
                           (var_Alunos,it_Dia->second.second.at(cred).second.second);
                     }
                  }
               }

               // --- --- ---

               map<vector<int/*indices de uma var do tipo CREDITOS*/>,
                  pair<Variable,int/*valor var*/> >::iterator 
                  it_Vars_Dia_Cred_OU_Oferc = vars_Dia_Cred_OU_Oferc.begin();

               /* P/ cada variavel registrada em <vars_Dia_Cred_OU_Oferc>, procuro em <v_Hash> e
               e se ela for encontrada, atribuo o valor indicado pela estrutura <vars_Dia_Cred_OU_Oferc> */
               for(; it_Vars_Dia_Cred_OU_Oferc != vars_Dia_Cred_OU_Oferc.end(); it_Vars_Dia_Cred_OU_Oferc++)
               {
                  // Procurando pela variavel no Hash de variaveis.
                  VariableHash::iterator it_v_Hash = v_Hash.find(it_Vars_Dia_Cred_OU_Oferc->second.first);

                  if(it_v_Hash != v_Hash.end())
                  {
                     //cout << "Found V_CREDITOS" << endl;

                     indices[cnt] = it_v_Hash->second;
                     //indices->push_back(it_v_Hash->second);

                     valores[cnt] = it_Vars_Dia_Cred_OU_Oferc->second.second;
                     //valores->push_back(it_Vars_Dia->second.second);

                     ++cnt;

                     indices_V_CREDITOS.insert(it_v_Hash->second);
                  }
                  else
                  {
                     cout << "Not Found" << endl;
                     exit(1);
                  }

                  // ===

                  /* Para cada variavel do tipo CREDITOS, tem que existir uma variavel do 
                  tipo OFERECIMENTO (Binaria) correspondente. */

                  // Alternando APENAS o tipo da variavel para OFERECIMENTO
                  it_Vars_Dia_Cred_OU_Oferc->second.first.setType(Variable::V_OFERECIMENTO);
                  
                  // Procurando pela variavel no Hash de variaveis.
                  it_v_Hash = v_Hash.find(it_Vars_Dia_Cred_OU_Oferc->second.first);

                  if(it_v_Hash != v_Hash.end())
                  {
                     //cout << "Found V_OFERECIMENTO" << endl;

                     indices[cnt] = it_v_Hash->second;
                     //indices->push_back(it_v_Hash->second);

                     valores[cnt] = 1;
                     //valores->push_back(1);

                     ++cnt;

                     indices_V_OFERECIMENTO.insert(it_v_Hash->second);
                  }
                  else
                  {
                     cout << "Not Found" << endl;
                     exit(1);
                  }

                  // ===

               }

               // --- --- ---
            
               map<vector<int/*indices de uma var do tipo V_ALUNOS*/>,
                  pair<Variable,int/*valor var*/> >::iterator 
                  it_Vars_Alunos = vars_Alunos.begin();

               /* P/ cada variavel registrada em <vars_Alunos>, procuro em <v_Hash> e
               e se ela for encontrada, atribuo o valor indicado pela estrutura <vars_Alunos> */
               for(; it_Vars_Alunos != vars_Alunos.end(); it_Vars_Alunos++)
               {
                  // Procurando pela variavel no Hash de variaveis.
                  VariableHash::iterator it_v_Hash = v_Hash.find(it_Vars_Alunos->second.first);

                  if(it_v_Hash != v_Hash.end())
                  {
                     //cout << "Found V_ALUNOS" << endl;

                     indices[cnt] = it_v_Hash->second;
                     //indices->push_back(it_v_Hash->second);

                     valores[cnt] = it_Vars_Alunos->second.second;
                     //valores->push_back(it_Vars_Dia->second.second);

                     ++cnt;

                     indices_V_ALUNOS.insert(it_v_Hash->second);
                  }
                  else
                  {
                     cout << "Not Found" << endl;
                     exit(1);
                  }
               }

               // --- --- ---
            }
         }
      }
   }

   /* Para todas as outras variaveis do tipo V_CREDITOS e V_OFERECIMENTO que não tiverem valor associado,
   deve-se inicializa-las com o valor 0. */

   /**/

   VariableHash::iterator it_v_Hash = v_Hash.begin();

   int cnt_V_CREDITOS_zeradas = 0;
   int cnt_V_OFERECIMENTO_zeradas = 0;
   int cnt_V_ALUNOS_zeradas = 0;

   for(; it_v_Hash != v_Hash.end() ; it_v_Hash++)
   {
      switch(it_v_Hash->first.getType())
      {
      case Variable::V_CREDITOS :
         if(indices_V_CREDITOS.find(it_v_Hash->second) == indices_V_CREDITOS.end())
         {
            indices[cnt] = it_v_Hash->second;
            //indices->push_back(it_v_Hash->second);
            
            valores[cnt] = 0;
            //valores->push_back(0);

            ++cnt;
            ++cnt_V_CREDITOS_zeradas;
         }
         break;
      case Variable::V_OFERECIMENTO :
         if(indices_V_OFERECIMENTO.find(it_v_Hash->second) == indices_V_OFERECIMENTO.end())
         { 
            indices[cnt] = it_v_Hash->second;
            //indices->push_back(it_v_Hash->second);

            valores[cnt] = 0;
            //valores->push_back(0);

            ++cnt;
            ++cnt_V_OFERECIMENTO_zeradas;
         }
         break;
      case Variable::V_ALUNOS:
         if(indices_V_ALUNOS.find(it_v_Hash->second) == indices_V_ALUNOS.end())
         {
            indices[cnt] = it_v_Hash->second;
            //indices->push_back(it_v_Hash->second);

            valores[cnt] = 0;
            //valores->push_back(0);

            ++cnt;
            ++cnt_V_ALUNOS_zeradas;
         }
      default:
         break;
      }
   }
   
   /**/

   cout << "cnt: " << cnt << endl;

   cout << "cnt_V_CREDITOS_zeradas: " << cnt_V_CREDITOS_zeradas << endl;
   cout << "cnt_V_OFERECIMENTO_zeradas: " << cnt_V_OFERECIMENTO_zeradas << endl;
   cout << "cnt_V_ALUNOS_zeradas: " << cnt_V_ALUNOS_zeradas << endl;
   
   cout << "Convertido" << endl;

/*
cout << "CONVERTI AS VARS DO TIPO X, FALTA INDICAR COM O VALOR 0 TODAS AS VARIAVEIS X QUE NAO FORAM UTILIZADAS " <<
"NA HEURISTICA. FALTA TB, CONVERTER AS OUTRAS VARIAVEIS (O,A,B)" << endl;
*/

   //exit(0);

   return make_pair(indices,valores);
}

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