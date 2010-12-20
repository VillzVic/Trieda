#include "InitialSolution.h"

//bool ordena_dec_demanda(Demanda const * left, Demanda const * right)
bool ordena_dec_demanda(pair<Demanda const *, int> const & left, pair<Demanda const *, int> const & right)
{
   //return (left->quantidade > right->quantidade);
   return (left.second> right.second);
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

               pair<Disciplina*,pair<int,int> > horario_a_ser_Alocado (NULL,
                  make_pair(0,0));

               vector<pair<Disciplina*,pair<int,int> > > horarios_do_Dia_Letivo 
                  (creds_Livres,horario_a_ser_Alocado);

               is_sala->atendimento_Tatico[dia] = 
                  make_pair<int/*credsLivres*/,vector<pair<Disciplina*,pair<int,int> > > > 
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
      //vector<Demanda*>::iterator it_vt_Demandas =
      vector<pair<Demanda*,int> >::iterator it_vt_Demandas =
         vt_Demandas.begin();

      for(; it_vt_Demandas != vt_Demandas.end(); it_vt_Demandas++)
      {
         // Se a demanda ainda não foi atendida
         //if((*it_vt_Demandas)->quantidade > 0)
         if(it_vt_Demandas->second > 0)
         {
            //Disciplina * pt_Disc = (*it_vt_Demandas)->disciplina;
            Disciplina * pt_Disc = it_vt_Demandas->first->disciplina;

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
                  if(is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].first >= total_Creditos)
                  {
                     alocou_Demanda = true;

                     // Adicionando o indice da turma
                     ++disc_Turmas[pt_Disc];

                     int demanda_Atendida = 
                        //(*it_vt_Demandas)->quantidade / is_Sala->sala->capacidade;
                        it_vt_Demandas->second / is_Sala->sala->capacidade;

                     // Caso a demanda seja inferior à cap. da sala em questão
                     demanda_Atendida = 
                        //(demanda_Atendida > 0 ? demanda_Atendida : (*it_vt_Demandas)->quantidade);
                        (demanda_Atendida > 0 ? demanda_Atendida : it_vt_Demandas->second);

                     // Para cada credito a ser atendido
                     for(int cred = 0; cred < total_Creditos; cred++)
                     {
                        // Encontrando a posicao correta para inserir
                        int pos = is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].second.size()
                           - is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].first;

                        // Sutraindo o credito alocado dos creditos livres.
                        is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].first--;

                        // Referenciando a disciplina que está sendo alocada
                        is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].second.at(pos).first =
                           pt_Disc;

                        // Armazenando a demanda (parcial ou total) atendida
                        //is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].second.at(pos).second = 
                        //dem_a_ser_Atendida;
                        is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].second.at(pos).second =
                           make_pair(disc_Turmas[pt_Disc],demanda_Atendida);
                     }              

                     /* Teste para o caso em que toda a demanda foi atendida */
                     if(demanda_Atendida <= is_Sala->sala->capacidade)
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

                        // Reordenando as demandas em ordem decrescente
                        sort(vt_Demandas.begin(),vt_Demandas.end(),ordena_dec_demanda);
                     }

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

pair<int*,double*> InitialSolution::repSolIniParaVariaveis(VariableHash & v_Hash, int lp_Cols)
{
   int * indices = new int (lp_Cols);
   double * valores = new double (lp_Cols);

   int cnt = 0;

   // p/ cada campus
   ITERA_GGROUP(it_IS_Campus,solucao,IS_Campus)
   {
      // p/ cada unidade
      ITERA_GGROUP(it_IS_Unidade,it_IS_Campus->unidades,IS_Unidade)
      {
         // p/ cada sala
         ITERA_GGROUP(it_IS_Sala,it_IS_Unidade->salas,IS_Sala)
         {
            map<int/*dia*/,pair<int/*credsLivres*/,
               vector<
               pair<Disciplina*,
               pair<int/*Id Turma*/,int/*Demanda Atendida*/> > > > >::iterator

               it_Dia = it_IS_Sala->atendimento_Tatico.begin();

            // p/ cada dia do atendimento tatico em questao
            for(; it_Dia != it_IS_Sala->atendimento_Tatico.end(); it_Dia++)
            {
               // Estrutura que armazena as informações das vars, para um dado dia.
               //map<Variable,int/*Num creds alocados no dia p a turma da disc*/> vars_Dia;

               map<vector<int/*indices de uma var do tipo CREDITOS*/>,
                  pair<Variable,int/*valor var*/> > vars_Dia;

               // p/ cada credito do dia 
               for(unsigned cred = 0; cred < it_Dia->second.second.size(); cred++)
               {
                  // SOMENTE se o cred tiver sido alocado
                  if(it_Dia->second.second.at(cred).first) // != NULL
                  {
                     // ===
                     //map<vector<int/*indices de uma var do tipo CREDITOS*/>,
                     //   pair<Variable,int/*valor var*/> > v;

                     Variable var;

                     vector<int> chave(5);

                     var.reset();
                     var.setType(Variable::V_CREDITOS);

                     var.setTurma( it_Dia->second.second.at(cred).second.first );
                     chave.at(0) = it_Dia->second.second.at(cred).second.first;

                     var.setDisciplina( it_Dia->second.second.at(cred).first );
                     chave.at(1) = it_Dia->second.second.at(cred).first->getId();

                     var.setUnidade( it_IS_Unidade->unidade );
                     chave.at(2) = it_IS_Unidade->unidade->getId();

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

                     chave.at(3) = id;

                     // ---

                     var.setSubCjtSala( cjt_Sala );

                     var.setDia( it_Dia->first );
                     chave.at(4) = it_Dia->first;

                     // ===

                     if(vars_Dia.find(chave) == vars_Dia.end())
                     { vars_Dia[chave] = make_pair(var,1); }
                     else
                     { vars_Dia[chave].second += 1; }
                  }
               }

               //map<Variable,int/*Num creds alocados no dia p/ a turma da disc*/>::iterator
               //   it_Vars_Dia = vars_Dia.begin();

               map<vector<int/*indices de uma var do tipo CREDITOS*/>,
                  pair<Variable,int/*valor var*/> >::iterator 
                  it_Vars_Dia = vars_Dia.begin();

               for(; it_Vars_Dia != vars_Dia.end(); it_Vars_Dia++)
               {
                  //VariableHash::iterator it_v_Hash = v_Hash.find(it_Vars_Dia->first);
                  VariableHash::iterator it_v_Hash = v_Hash.find(it_Vars_Dia->second.first);

                  if(it_v_Hash != v_Hash.end())
                  {
                     //cout << "Found" << endl;
                     indices[cnt] = it_v_Hash->second;
                     //valores[cnt] = it_Vars_Dia->second;
                     valores[cnt] = it_Vars_Dia->second.second;
                     cnt++;
                  }
                  else
                  {
                     cout << "Not Found" << endl;
                     exit(1);
                  }
               }
            }
         }
      }
   }

   cout << "Convertido" << endl;


cout << "CONVERTI AS VARS DO TIPO X, FALTA INDICAR COM O VALOR 0 TODAS AS VARIAVEIS X QUE NAO FORAM UTILIZADAS " <<
"NA HEURISTICA. FALTA TB, CONVERTER AS OUTRAS VARIAVEIS (O,A,B)" << endl;

   exit(1);

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