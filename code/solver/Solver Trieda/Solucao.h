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

   // Construtor de c�pia
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

   /* Imprime informa��es sobre a solu��o gerada. */
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

   // Etrutura para armazenar refer�ncias entre as <Sala> e <IS_Sala>.
   map<Sala*,IS_Sala*> map_Sala__IS_Sala;

   // ===============================

   /*
   Estrutura respons�vel por referenciar as demandas de cada disciplina. Utilizada internamente para auxiliar
   na constru��o da solu��o inicial.
   */  
   map<Disciplina*,
      pair<vector<pair<vector<pair<Demanda*,int/*Demanda espec�fica ainda n�o atendida*/> >,
      int/*Somat�rio das demandas ainda n�o atendidas para um determinado conjunto*/> >,
      int/*Somat�rio das demandas ainda n�o atendidas de todos os conjuntos para uma dada disciplina*/ > >

      disc_Cjts_Demandas;

   // ===============================

   int total_Cjt_Demandas;

   // ===============================

   int num_Demandas_Atendidas;
   
   // ===============================

   int num_Demandas_NAO_Atendidas;

   // ===============================

   /*
   Utilizado para armazenar os indices da estrutura <cjts_Dem> que j� foram utilizados
   (ou seja, tentou-se aloc�-los)
   */

   GGroup<int/*indice da estrutura <cjts_Dem>*/> indices_Proibidos;

   // ===============================

   // METODOS
   
   // ===============================

   /* Agrupa demandas de uma mesma disciplina oferecidas para cursos diferentes, mas que 
   s�o compat�veis. */
   
   void agrupaDemandas();

   // ===============================

   /* Cria os objetos is_Bloco_Curriculares conforme necess�rio. */

   void criaIS_Blocos_Curriculares();

   // ===============================

   /* Dado um conjunto de IS_Blocos_Curriculares, essa fun��o seleciona um IS_Bloco_Curricular que ainda n�o
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

      M�todo utilizado na constru��o dos conjuntos de demanda.

      Por equanto, qdo uma nova demanda � compat�vel com mais de um conjunto de demandas
      associa-se essa demanda ao primeiro conjunto encontrado.

      Pode-se, por exemplo, dividir a demanda em partes e associar cada parte a um conjunto
      de demandas diferente.
   */

   int escolheConjuntoDeDemandasParaAddNovaDemanda(
      vector<vector<pair<Demanda*,int/*Demanda espec�fica ainda n�o atendida*/> > > & cjts_Demanda,
      vector<int> & indices_Demandas);

   // ===============================

   /*
      Escolhe um conjunto de demandas entre os conjuntos que ainda n�o foram totalmente alocados.
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
      Referencia as varia��es de uma regra de credito, caso a disciplina possua alguma regra de credito.
      Caso contr�rio, cria-se uma nova regra de credito e suas varia��es.
   */

   vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > * getRegradeCreditoEVariacoes(Disciplina * disciplina);

   // ===============================

   /* 
      Verifica se alguma das varia��es da regra de cr�dito informada � valida para a sala em quest�o.
      Retorna um iterator para a varia��o da regra de cr�dito caso encontre alguma valida, e
      um iterador para o final caso contr�rio.
   */

   vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >::iterator
      salaCompativelRegraCredito(
      IS_Sala* is_Sala_Compativel, 
      vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > & variacoes_Regra_de_Cred_Esc);

   // ===============================

   /* 
      Uma vez escolhido um conjunto de demandas espec�fico, tenta-se aloc�-lo em alguma
      das IS_Salas dispon�veis.
   */

   //void aloca(pair<Disciplina*,int/*indice do conjunto*/> & cjt_Dem_Esc, vector<IS_Sala*> & is_Salas_Comaptiveis);

   // ===============================

   /*
      TRIEDA-559

      organiza as IS_Salas conforme desejado.
   */

   void organizaIS_Salas(vector<IS_Sala*> & is_Salas_Comaptiveis);
   
   // ===============================

   /* Dada uma demanda a ser atendida, o m�todo abaixo seleciona uma fra��o de
   cada demanda do conjunto de demandas para a disciplina em quest�o de modo que
   o valor dado por <demanda_Atendida> seja atendido */

   vector<pair<Demanda*,int/*Demanda atendida*/> > elaboraTurma(
      pair<Disciplina*,int> & cjt_Dem, int demanda_Atendida);

      // ===============================

   /*
      organiza as Varia��es de uma regra de cr�dito conforme desejado.
   */

   void organizaVariacoesRegraCredito(
      vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > & variacoes_Regra_Credito);

};
