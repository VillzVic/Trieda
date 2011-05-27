#ifndef _SOLUCAO_INICIAL_OPERACIONAL_H_
#define _SOLUCAO_INICIAL_OPERACIONAL_H_

#include <vector>
#include <map>
#include <algorithm>

#include "Professor.h"
#include "ProblemData.h"
#include "Aula.h"
#include "SolucaoOperacional.h"
#include "CustoAlocacao.h"

#include "MoveValidator.h"

class SolucaoInicialOperacional
{
public:

   SolucaoInicialOperacional( ProblemData & );

   virtual ~SolucaoInicialOperacional();

   SolucaoOperacional & geraSolucaoInicial();

private:

   ProblemData & problemData;

   SolucaoOperacional * solIni;

   /* Estrutura auxiliar que indica as aulas que ainda não foram alocadas no processo 
   de geração da solução inicial. */
   GGroup< Aula *, LessPtr< Aula > > aulasNaoAlocadas;

   MoveValidator * moveValidator;

   // Estrutura que armazena o custo de alocar um professor a uma dada aula.
   std::map< std::pair< Professor *, Aula * >, CustoAlocacao * > custoProfTurma;

   /* Estrutura que armazena, separadamente, os custos de alocação por nivel de prioridade. 
   
   CADA SUBCONJUNTO DE <CustoAlocacao> JÁ ESTÁ ORDENADO.
   */
   std::map<unsigned /*Nivel de prioridade*/,
      GGroup< CustoAlocacao *, GreaterPtr< CustoAlocacao > > > custoAlocacaoNiveisPrioridade;

   // Armazena, em ordem decrescente, os Custos de Alocação.
   //std::vector< CustoAlocacao * > custosAlocacaoAulaOrdenado;
   GGroup< CustoAlocacao *, GreaterPtr< CustoAlocacao > > custosAlocacaoAulaOrdenado;

   // Cada professor deve, preferencialmente, ministrar apenas
   // uma disciplina de um bloco curricular. A estrutura abaixo
   // associa cada bloco com os professores selecionados. Ou seja,
   // para uma dada aula, identifica-se o bloco ao qual a aula em
   // questão pertence e em seguida adiciona-se o professor ao
   // grupo de professores do bloco em questão.
   std::map< BlocoCurricular *, GGroup< Professor *, LessPtr< Professor > > > blocosProfs;

   // Armazena os horarios que já foram alocados para cada sala.
   // Estrutura: para cada sala, retorna-se o conjuntos de pares
   // dia da semana / de horario aula que a sala já está ocupada.
   //std::map< Sala *, GGroup< std::pair< int /*Dia Semana*/, HorarioAula * > >, LessPtr<Sala> > horariosAulaSala;
   std::map< Sala*,  std::map< int /*Dia*/, GGroup< HorarioAula *, LessPtr< HorarioAula > > >,
			 LessPtr< Sala > > horariosAulaSala;

   // Métodos
   void executaFuncaoPrioridade( Campus &, GGroup< Professor *, LessPtr< Professor > > & );

   // Função auxilar à função de prioridade.
   void calculaCustoFixProf( Professor &, Aula &, unsigned, int = 0, int = 0 );

   /* Função recursiva de alocação de aulas. */
   void alocaAulasRec(bool primeiraTentativaAlocacao, bool tentarManterViabilidade);

   // Função auxiliar.
   Professor & addProfessor();

   // Função que utiliza a estrutura acima para
   // dizer se um dado professor está sendo alocado
   // mais de uma vez para um bloco curricular.
   bool professorRepetido( Professor &, Aula & );

};

#endif // _SOLUCAO_INICIAL_OPERACIONAL_H_
