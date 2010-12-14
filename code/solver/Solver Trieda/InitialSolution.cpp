#include "InitialSolution.h"

bool ordena_dec_demanda(Demanda const * left, Demanda const * right)
//bool ordena_dec_demanda(pair<Demanda const *, bool> const & left, pair<Demanda const *, bool> const & right)
{
   //return (left.first->quantidade > right.first->quantidade);
   return (left->quantidade > right->quantidade);
}

bool ordena_dec_sala(Sala const * left, Sala const * right)
{
   return (left->capacidade > right->capacidade);
}

InitialSolution::InitialSolution(ProblemData & _problem_Data) :
problem_Data(&_problem_Data)
{
   // Construir uma solução inicial com base no ProblemData.

   /*
   A ideia por trás disso é criar o "cenário" que o problema carregado representa.

   Por ex. A instância trivial tem 1 campus com uma unidade com 5 salas e por aí vai.

   Daí, deve-se criar uma estrutura que represente tudo o que existe na instância trivial.

   Já tem o set<Campus*>.

   Deve-se primeiro, estrutura-lo (Isso deve ser feito aqui no construtor, via informações obtidas do problem_Data).

   Depois de estruturado, deve-se preencher a estrutura criada com os dados de uma heuristica (Isso deve ser feito no método generateInitialSolution()).

   FALTA CRIAR O MÉTODO DE GETSOLUTION
   FALTA CRIAR O MÉTODO QUE VAI CONVERTER UMA <InitialSolution> NAS VARIAVEIS.
   */

   cout << "Gerando uma solução inicial" << endl;

   // Copiando as demandas para poder ordena-las
   ITERA_GGROUP(it_demanda,problem_Data->demandas,Demanda)
   //{ vt_Demandas.push_back(make_pair(new Demanda(**it_demanda),false)); }
   { vt_Demandas.push_back(new Demanda(**it_demanda)); }

   // Ordenando as demandas em ordem decrescente
   sort(vt_Demandas.begin(),vt_Demandas.end(),ordena_dec_demanda);

   num_Demandas_Atendidas = 0;

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

               pair<Disciplina*,int> horario_a_ser_Alocado (NULL,0);

               vector<pair<Disciplina*,int> > horarios_do_Dia_Letivo 
                  (creds_Livres,horario_a_ser_Alocado);

               is_sala->atendimento_Tatico[dia] = 
                  make_pair<int/*credsLivres*/,vector<pair<Disciplina*,int> > > 
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
}

InitialSolution::~InitialSolution(void)
{
}

ProblemData & InitialSolution::get_Problem_Data() const
{
   return *problem_Data;
}

int InitialSolution::getNumDemandasAtendidas() const
{
   return num_Demandas_Atendidas;
}

bool InitialSolution::todasDemandasAtendidas()
{
   if(num_Demandas_Atendidas == vt_Demandas.size())
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

   // Tentando alocar todas as demandas. Pode ser que parte de alguma demanda não seja alocada
   while(!todasDemandasAtendidas())
   {
      //vector<pair<Demanda*,bool> >::iterator it_vt_Demandas =
      vector<Demanda*>::iterator it_vt_Demandas =
         vt_Demandas.begin();

      for(; it_vt_Demandas != vt_Demandas.end(); it_vt_Demandas++)
      {
         // Se a demanda ainda não foi atendida
         //if(it_vt_Demandas->first->quantidade > 0)
         if((*it_vt_Demandas)->quantidade > 0)
         {

            //Disciplina * pt_Disc = (*it_vt_Demandas).first->disciplina;
            Disciplina * pt_Disc = (*it_vt_Demandas)->disciplina;

            int total_Creditos = pt_Disc->cred_praticos + pt_Disc->cred_teoricos;

            /* Utilizada para indicar se uma demanda foi atendida, pelo menos em parte, em uma iteração
            das salas compatíveis. Se, em uma iteração dessas a demanda não for atendida (em pelo menos,
            o tamanho da menor sala) significa que não há salas com horário vagos suficientes para podermos
            alocar a demanda. 
            Dá pra dividir a demanda e alocar separado por sala. Fica pra um TRIEDA FUTURO !!!*/
            bool alocou_Demanda = false;

            // p tds salas que possuem a disc associada
            ITERA_VECTOR(it_Disc_Salas,disc_Salas[pt_Disc->getId()],Sala)
            {
               // Selecionando a IS_Sala correspondente à Sala em questão.
               IS_Sala * is_Sala = map_Sala__IS_Sala[*it_Disc_Salas];

               pair<int/*idDisc*/,int/*idSala*/> ids_Disc_Sala 
                  //(pt_Disc->getId(),(*it_Disc_Salas)->getId()); ou usando a IS_SALA
                  (pt_Disc->getId(),is_Sala->getId());

               // p tds os dias letivos comuns entre a disc e a sala selecionadas
               ITERA_GGROUP_N_PT(it_Dia_Let_Disc_Sala,problem_Data->disc_Salas_Dias[ids_Disc_Sala],int)
               {
                  break; // ESSE BREAK EH SO PRA TESTE - TIRAR !!!

                  /* Se a sala em questao possui, no mínimo, tantos horarios (creditos) vagos quanto o total
                  de créditos da disciplina em questão. */
                  if(is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].first >= total_Creditos)
                  {
                     alocou_Demanda = true;

                     int dem_a_ser_Atendida = 
                        //(*it_vt_Demandas).first->quantidade / is_Sala->sala->capacidade;
                        (*it_vt_Demandas)->quantidade / is_Sala->sala->capacidade;

                     // Caso a demanda seja inferior à cap. da sala em questão
                     //dem_a_ser_Atendida > 0 ? dem_a_ser_Atendida : (*it_vt_Demandas).first->quantidade;
                     dem_a_ser_Atendida =
                        (dem_a_ser_Atendida > 0 ? dem_a_ser_Atendida : (*it_vt_Demandas)->quantidade);

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
                        is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].second.at(pos).second = 
                           dem_a_ser_Atendida;

                        ///* Teste para o caso em que toda a demanda foi atendida */
                        //if(dem_a_ser_Atendida <= is_Sala->sala->capacidade)
                        //{
                        //   // Se está aqui, é pq toda a demanda foi atendida
                        //   num_Demandas_Atendidas++;

                        //   // Para não atender denovo
                        //   (*it_vt_Demandas)->quantidade = 0;
                        //}
                        //else
                        //{
                        //   // Se está aqui, é pq NEM toda a demanda foi atendida

                        //   // Atualizando a demanda que ainda falta ser atendida
                        //   (*it_vt_Demandas)->quantidade -= dem_a_ser_Atendida;
                        //}
                     }              

                     /* Teste para o caso em que toda a demanda foi atendida */
                     if(dem_a_ser_Atendida <= is_Sala->sala->capacidade)
                     {
                        // Se está aqui, é pq toda a demanda foi atendida
                        num_Demandas_Atendidas++;

                        // Para não atender denovo
                        (*it_vt_Demandas)->quantidade = 0;
                     }
                     else
                     {
                        // Se está aqui, é pq NEM toda a demanda foi atendida

                        // Atualizando a demanda que ainda falta ser atendida
                        (*it_vt_Demandas)->quantidade -= dem_a_ser_Atendida;
                     }
                  }

                  // O problema pode estar aqui !!!!!!!!!!!!
                  if(!alocou_Demanda)
                  {
                     // Zero a demanda e não tento mais alocá-la para a solução inicial
                     (*it_vt_Demandas)->quantidade = 0;
                     break;
                  }
                  else
                  {
                     break; // Pq eu já aloquei a demanda e não devo continuar procurando.
                  }
               }
            }
         }
      }
   }

//   // p/ tds demandas
//   ITERA_VECTOR(it_vt_Demandas,vt_Demandas,Demanda)
//   {
//      Disciplina * pt_Disc = (*it_vt_Demandas)->disciplina;
//      
//      int creds_Disc_a_Alocar = pt_Disc->cred_praticos + pt_Disc->cred_teoricos;
//
//      // p tds salas que possuem a disc associada
//      ITERA_VECTOR(it_Disc_Salas,disc_Salas[pt_Disc->getId()],Sala)
//      {
//         
//         // Selecionando a IS_Sala correspondente à Sala em questão.
////         IS_Sala * is_Sala = map_Sala__IS_Sala[*it_Disc_Salas];
//
//         pair<int/*idDisc*/,int/*idSala*/> ids_Disc_Sala 
//            (pt_Disc->getId(),(*it_Disc_Salas)->getId());
//
//         // p tds os dias letivos comuns entre a disc e a sala selecionadas
//         ITERA_GGROUP_N_PT(it_Dia_Let_Disc_Sala,problem_Data->disc_Salas_Dias[ids_Disc_Sala],int)
//         {
//            // Se a sala em questa possui pelo menos um horario vago
//            //if(is_Sala->atendimento_Tatico[*it_Dia_Let_Disc_Sala].first > 0)
//            //{
//            //
//            //}
//         }
//      }
//   }
}