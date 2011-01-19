#pragma once

#include <iostream>
#include <set>
#include <vector>
#include <map>

#include "IS_Campus.h"

#include "ProblemData.h"

#include "Variable.h"

#include "opt_lp.h"

#include "IS_Bloco_Curricular.h"

#include "IS_Curso.h"

using namespace std;

//#define FIX_LB_AND_UB_TO_THE_SOL_INI_VALUE

class Solucao
{
public:

   // ===============================

   Solucao(ProblemData & problem_Data);

   // ===============================

   // Construtor de cópia
   Solucao(Solucao const & init_sol);

   // ===============================

   virtual ~Solucao(void);

   // ===============================

   int demanda_Nao_Atendida;

   // ===============================

   ProblemData & get_Problem_Data() const;

   // ===============================

   GGroup<IS_Campus*> const & getSolucao() const;

   // ===============================

   int getNumDemandasAtendidas() const;

   // ===============================

   int getNumDemandas_NAO_Atendidas() const;
   
   // ===============================

   //void geraSolucao();

   // ===============================

   void geraSolucaoSubBlocos();
   
   // ===============================

   pair<vector<int>*,vector<double>*> repSolIniToVariaveis(VariableHash & v_Hash, int lp_Cols, OPT_LP & lp);

   // ===============================

   bool operator == (Solucao const & right);
   
   // ===============================

   bool operator != (Solucao const & right);

   // ===============================

   bool operator < (Solucao const & right);

   // ===============================

   bool operator > (Solucao const & right);

   // ===============================

   /* Imprime informações sobre a solução gerada. */
   void imprimeInfo();

private:

   // ===============================

   ProblemData * problem_Data;

   // ===============================

   GGroup<IS_Campus*> solucao;

   // ===============================

   //GGroup<IS_Bloco_Curricular*> is_Blocos_Curriculares;
   vector<IS_Bloco_Curricular*> is_Blocos_Curriculares;

   // ===============================

   //GGroup<IS_Curso*> is_Cursos;
   vector<IS_Curso*> is_Cursos;

   // ===============================

   // Etrutura para armazenar referências entre as <Sala> e <IS_Sala>.
   map<Sala*,IS_Sala*> map_Sala__IS_Sala;

   // ===============================

   /*
   Estrutura responsável por referenciar as demandas de cada disciplina. Utilizada internamente para auxiliar
   na construção da solução inicial.
   */  
   map<Disciplina*,
      pair<vector<pair<vector<pair<Demanda*,int/*Demanda específica ainda não atendida*/> >,
      int/*Somatório das demandas ainda não atendidas para um determinado conjunto*/> >,
      int/*Somatório das demandas ainda não atendidas de todos os conjuntos para uma dada disciplina*/ > >

      disc_Cjts_Demandas;

   // ===============================

   int total_Cjt_Demandas;

   // ===============================

   int num_Demandas_Atendidas;
   
   // ===============================

   int num_Demandas_NAO_Atendidas;

   // ===============================

   /*
   Utilizado para armazenar os indices da estrutura <cjts_Dem> que já foram utilizados
   (ou seja, tentou-se alocá-los)
   */

   GGroup<int/*indice da estrutura <cjts_Dem>*/> indices_Proibidos;

   // ===============================

   // METODOS
   
   // ===============================

   /* Agrupa demandas de uma mesma disciplina oferecidas para cursos diferentes, mas que 
   são compatíveis. */
   
   void agrupaDemandas();

   // ===============================

   /* Cria os objetos is_Bloco_Curriculares conforme necessário. */

   void criaIS_Blocos_Curriculares();

   // ===============================

   /* Dado um conjunto de IS_Blocos_Curriculares, essa função seleciona um IS_Bloco_Curricular que ainda não
   tenha sido atendido. */

   //IS_Bloco_Curricular * escolheIS_Bloco_Curricular();

   // Substituido por

   /*
      TRIEDA-559
      organiza os IS_Blocos_Curriculares conforme desejado.
   */

   void organizaIS_Blocos_Curriculares(vector<IS_Bloco_Curricular*> & is_Blocos_Curriculares);

   // ===============================

   /*
      Seleciona um conjunto de demandas para adicionar a nova demanda.

      Método utilizado na construção dos conjuntos de demanda.

      Por equanto, qdo uma nova demanda é compatível com mais de um conjunto de demandas
      associa-se essa demanda ao primeiro conjunto encontrado.

      Pode-se, por exemplo, dividir a demanda em partes e associar cada parte a um conjunto
      de demandas diferente.
   */

   int escolheConjuntoDeDemandasParaAddNovaDemanda(
      vector<vector<pair<Demanda*,int/*Demanda específica ainda não atendida*/> > > & cjts_Demanda,
      vector<int> & indices_Demandas);

   // ===============================

   /*
      Escolhe um conjunto de demandas entre os conjuntos que ainda não foram totalmente alocados.
   */

   pair<Disciplina*,int/*indice do conjunto*/> escolheConjuntoDeDemadasParaAlocarASala(
      vector<pair<pair<Disciplina*,int/*indice do conjunto*/>, int * /*Valor da demanda*/ > > & cjts_Dem);

   // ===============================

   /*
   Referencia todos os conjuntos de demanda.
   */

   void carregaEstruturaCjtsDem(
      vector<pair<pair<Disciplina*,int/*indice do conjunto*/>, int * /*Valor da demanda*/ > > & cjts_Dem);

   // ===============================

   /*
      Referencia as variações de uma regra de credito, caso a disciplina possua alguma regra de credito.
      Caso contrário, cria-se uma nova regra de credito e suas variações.
   */

   vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > * getRegradeCreditoEVariacoes(Disciplina * disciplina);

   // ===============================

   /* 
      Verifica se alguma das variações da regra de crédito informada é valida para a sala em questão.
      Retorna um iterator para a variação da regra de crédito caso encontre alguma valida, e
      um iterador para o final caso contrário.
   */

   vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >::iterator
      salaCompativelRegraCredito(
      IS_Sala* is_Sala_Compativel, 
      vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > & variacoes_Regra_de_Cred_Esc);

   // ===============================

   /* 
      Uma vez escolhido um conjunto de demandas específico, tenta-se alocá-lo em alguma
      das IS_Salas disponíveis.
   */

   //void aloca(pair<Disciplina*,int/*indice do conjunto*/> & cjt_Dem_Esc, vector<IS_Sala*> & is_Salas_Comaptiveis);

   // ===============================

   /*
      TRIEDA-559

      organiza as IS_Salas conforme desejado.
   */

   void organizaIS_Salas(vector<IS_Sala*> & is_Salas_Comaptiveis);
   
   // ===============================

   /* Dada uma demanda a ser atendida, o método abaixo seleciona uma fração de
   cada demanda do conjunto de demandas para a disciplina em questão de modo que
   o valor dado por <demanda_Atendida> seja atendido */

   vector<pair<Demanda*,int/*Demanda atendida*/> > elaboraTurma(
      pair<Disciplina*,int> & cjt_Dem, int demanda_Atendida);

      // ===============================

   /*
      organiza as Variações de uma regra de crédito conforme desejado.
   */

   void organizaVariacoesRegraCredito(
      vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > & variacoes_Regra_Credito);

};
