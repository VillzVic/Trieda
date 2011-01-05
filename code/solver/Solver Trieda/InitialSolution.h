#pragma once

#include <iostream>
#include <set>
#include <vector>
#include <map>

#include "IS_Campus.h"

#include "ProblemData.h"

#include "Variable.h"

#include "opt_lp.h"

using namespace std;

//#include "CjtDemandas.h"

//#define FIX_LB_AND_UB_TO_THE_SOL_INI_VALUE

class InitialSolution
{
public:
   InitialSolution(ProblemData & problem_Data);

   // Copy Constructor
   InitialSolution(InitialSolution const & init_sol);

   virtual ~InitialSolution(void);

   ProblemData & get_Problem_Data() const;

   GGroup<IS_Campus*> const & getInitialSolution() const;

   int getNumDemandasAtendidas() const;

   int getNumDemandas_NAO_Atendidas() const;

   void generate_Initial_Solution();

   pair<vector<int>*,vector<double>*> repSolIniToVariaveis(VariableHash & v_Hash, int lp_Cols, OPT_LP & lp);

private:

   ProblemData * problem_Data;

   GGroup<IS_Campus*> solucao;

   /*
      Estrutura responsavel por armazenar, para cada disciplina, conjuntos de demanda. Cada conjunto é
   formado por demandas de cursos compatíveis.

      Se sobrar algum valor no inteiro (segundo elemento do par) significa que esse valor não foi alocado
      em lugar algum. Pode-se deixar para o solver resolver ou associar esse valor a uma variável de folga.
   
   */  
   map<Disciplina*,
      pair<vector<pair<vector<pair<Demanda*,int/*Demanda específica ainda não atendida*/> >,
      int/*Somatório das demandas ainda não atendidas para um determinado conjunto*/> >,
      int/*Somatório das demandas ainda não atendidas de todos os conjuntos para uma dada disciplina*/ > >

      disc_Cjts_Demandas;

   int total_Cjt_Demandas;

   int num_Demandas_Atendidas;
   
   int num_Demandas_NAO_Atendidas;

   // METODOS
   
   /* DESCREVER ESSE METODO DEPOIS */
   void agrupaDemandas();

   /*
      Por equanto, qdo uma nova demanda é compatível com mais de um conjunto de demandas
      associa-se essa demanda ao primeiro conjunto encontrado.

      Pode-se, por exemplo, dividir a demanda em partes e associar cada parte a um conjunto
      de demandas diferente.
   */
   int escolheConjuntoDeDemandasParaAddNovaDemanda(
      //vector<pair<GGroup<Demanda*>,int> > & cjts_Demanda, vector<int> & indices_Demandas);
      vector<vector<pair<Demanda*,int/*Demanda específica ainda não atendida*/> > > & cjts_Demanda,
      vector<int> & indices_Demandas);

   /*
      Escolhe um conjunto de demandas entre os conjuntos ainda não totalmente alocados.
   */
   //pair<GGroup<Demanda*>,int> * escolheConjuntoDeDemadasParaAlocarASala(vector<pair<GGroup<Demanda*>,int> *> & cjts_Demanda); 
   //pair<Disciplina*,int* /*Valor da demanda*/> escolheConjuntoDeDemadasParaAlocarASala(
   pair<Disciplina*,int/*indice do conjunto*/> escolheConjuntoDeDemadasParaAlocarASala(
      vector<pair<pair<Disciplina*,int/*indice do conjunto*/>, int * /*Valor da demanda*/ > > & cjts_Dem);

   /*
      Referencia todos os conjuntos de demanda
   */
   void carregaEstruturaCjtsDem(
      vector<pair<pair<Disciplina*,int/*indice do conjunto*/>, int * /*Valor da demanda*/ > > & cjts_Dem);

   /*
      Referencia as variações de uma regra de credito, caso a disciplina possua alguma regra de credito.
      Caso contrario, cria-se uma nova regra de credito e suas variacoes.
   */
   vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > * getRegradeCreditoEVariacoes(Disciplina * disciplina);

   /* 
      Verifica se alguma das variacoes da regra de credito informada é valida para a sala em questao.
      Retorna o indice da variacao da regra de credito caso encontre alguma valida, -1 caso contrario.
   */
   vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >::iterator
      //int salaCompativelRegraCredito(
      salaCompativelRegraCredito(
      IS_Sala* is_Sala_Compativel, 
      vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > & variacoes_Regra_de_Cred_Esc);

   /* 
      Uma vez escolhido um conjunto de demandas específico, tenta-se alocá-lo em alguma
      das IS_Salas disponíveis.
   */
   //void aloca(pair<GGroup<Demanda*>,int> & cjt_Dem_Esc, vector<IS_Sala*> & is_Salas_Comaptiveis);
   void aloca(pair<Disciplina*,int/*indice do conjunto*/> & cjt_Dem_Esc, vector<IS_Sala*> & is_Salas_Comaptiveis);

   /*
      Escolhe uma IS_Sala entre as IS_Salas compatívieis com a disciplina em questão.
   */
   IS_Sala * escolheIS_Sala(vector<IS_Sala*> & is_Salas_Comaptiveis);

   /*
      Verifica a disponibilidade de uma IS_Sala para uma dada disciplina.
      
      Se a disciplina tiver regra de crédito associada, verificam-se todas as
      variações da regra de crédito em questão, retornando os índices das regras
      válidas no momento.

      EXEMPLO:

      Considere uma disciplina com a seguinte regra de creditos
      Dias     1 - 2 - 3 - 4 - 5 - 6
      Créditos 0 - 4 - 0 - 0 - 0 - 0

      Admitindo que os dias letivos para a disciplina sejam 2, 3, 4, 5 e 6 temos as
      seguintes regreas de créditos válidas:

      I
      Dias     1 - 2 - 3 - 4 - 5 - 6 - 7
      Créditos 0 - 4 - 0 - 0 - 0 - 0 - 0

      II
      Dias     1 - 2 - 3 - 4 - 5 - 6 - 7
      Créditos 0 - 0 - 4 - 0 - 0 - 0 - 0

      III
      Dias     1 - 2 - 3 - 4 - 5 - 6 - 7
      Créditos 0 - 0 - 0 - 4 - 0 - 0 - 0

      IV
      Dias     1 - 2 - 3 - 4 - 5 - 6 - 7
      Créditos 0 - 0 - 0 - 0 - 4 - 0 - 0

      V
      Dias     1 - 2 - 3 - 4 - 5 - 6 - 7
      Créditos 0 - 0 - 0 - 0 - 0 - 4 - 0

      Admita tb que uma sala tenha como dias letivos os dias 2, 3, 4, 5 e 6 com 4 créditos disponíveis
      para cada dia, exceto no dia 3 onde temos apenas 2 créditos disponíveis. Assim, a combinação II não
      pode ser utilizada.


      O valor NULL tb pode ser retornado. Nesse caso, nenhuma regra de crédito compatível com
      a disciplina se aplicou à IS_Sala em questão.

      Quando uma disciplina não possui regra de crédito associada, a função sugere uma alocação
      como base na IS_Sala em questão.

      EXEMPLO

      Disiciplina com 5 creditos que não possui regra de crédito associada. Admita que os dias
      letivos são 2, 3, 4, 5 e 6.

      Regra sugerida: 0 2 2 1 0 0 0
  
   */
   //vector<int> * verificaDisponibilidadeIS_Sala(IS_Sala & is_sala, Disciplina & disciplina);


   vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > * verificaDisponibilidadeIS_Sala
      //(IS_Sala & is_sala, pair<GGroup<Demanda*>,int> & cjt_Dem);
      (IS_Sala & is_sala, pair<Disciplina*,int> & cjt_Dem_Esc);

   /*
      Dado um vetor de regras de crédito válidas, quero saber se ainda existe
      alguma regra de crédito que possa ser aplicada. Se ainda existir, retorno
      a primeira que encontrar.
   */
   int getRegraDeCreditoValida(vector<bool> & regras_de_Credito);

   /* Dada uma demanda a ser atendida, o método abaixo seleciona uma fração de
   cada demanda do conjunto de demandas para a disciplina em questão de modo que
   o valor dado por <demanda_Atendida> seja atendido */
   vector<pair<Demanda*,int/*Demanda atendida*/> > elaboraTurma(
      pair<Disciplina*,int> & cjt_Dem, int demanda_Atendida);

};
