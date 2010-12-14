#include "InitialSolution.h"

bool ordena_dec_demanda(Demanda const * left, Demanda const * right)
{
   return (left->quantidade > right->quantidade);
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


   // Copiando as demandas para poder ordena-las
   ITERA_GGROUP(it_demanda,problem_Data->demandas,Demanda)
   { vt_Demandas.push_back(new Demanda(**it_demanda)); }

   // Ordenando as demandas em ordem decrescente
   sort(vt_Demandas.begin(),vt_Demandas.end(),ordena_dec_demanda);

   ITERA_GGROUP(it_Campus,problem_Data->campi,Campus)
   {
      IS_Campus * is_campus = new IS_Campus();
      is_campus->campus = *it_Campus;

      ITERA_GGROUP(it_Unidade,it_Campus->unidades,Unidade)
      {
         IS_Unidade * is_unidade = new IS_Unidade();
         is_unidade->unidade = *it_Unidade;

         ITERA_GGROUP(it_Sala,it_Unidade->salas,Sala)
         {
            IS_Sala * is_sala = new IS_Sala();
            is_sala->sala = *it_Sala;

            ITERA_GGROUP(it_Creds_Disp,it_Sala->creditos_disponiveis,CreditoDisponivel)
            {
               int dia = it_Creds_Disp->dia_semana;

               int creds_Livres = it_Sala->credsLivres.at(dia);

               is_sala->atendimento_Tatico[dia] = 
                  make_pair<int/*credsLivres*/,vector<Disciplina*> >
                  (creds_Livres, vector<Disciplina*>(creds_Livres));
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
}

InitialSolution::~InitialSolution(void)
{
}

ProblemData & InitialSolution::get_Problem_Data() const
{
   return *problem_Data;
}

void InitialSolution::generate_Initial_Solution()
{
   vector<Demanda*>::iterator it_vt_Demandas =
      vt_Demandas.begin();

   for(; it_vt_Demandas != vt_Demandas.end(); it_vt_Demandas++)
   {
      Disciplina * disc = (*it_vt_Demandas)->disciplina;

      GGroup<Sala*>::iterator it_disc_salas = 
         problem_Data->discSalas[disc->getId()].begin();

      // usar o id da disc para acessar as salas compativeis com ela;
      // USAR a estrutura <discSalas> do problemData para poder encontrar as salas compativeis com a disc em questao.
   }
}