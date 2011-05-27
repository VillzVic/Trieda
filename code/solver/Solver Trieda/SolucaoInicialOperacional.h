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

   /* Estrutura auxiliar que indica as aulas que ainda n�o foram alocadas no processo 
   de gera��o da solu��o inicial. */
   GGroup< Aula *, LessPtr< Aula > > aulasNaoAlocadas;

   MoveValidator * moveValidator;

   // Estrutura que armazena o custo de alocar um professor a uma dada aula.
   std::map< std::pair< Professor *, Aula * >, CustoAlocacao * > custoProfTurma;

   /* Estrutura que armazena, separadamente, os custos de aloca��o por nivel de prioridade. 
   
   CADA SUBCONJUNTO DE <CustoAlocacao> J� EST� ORDENADO.
   */
   std::map<unsigned /*Nivel de prioridade*/,
      GGroup< CustoAlocacao *, GreaterPtr< CustoAlocacao > > > custoAlocacaoNiveisPrioridade;

   // Armazena, em ordem decrescente, os Custos de Aloca��o.
   //std::vector< CustoAlocacao * > custosAlocacaoAulaOrdenado;
   GGroup< CustoAlocacao *, GreaterPtr< CustoAlocacao > > custosAlocacaoAulaOrdenado;

   // Cada professor deve, preferencialmente, ministrar apenas
   // uma disciplina de um bloco curricular. A estrutura abaixo
   // associa cada bloco com os professores selecionados. Ou seja,
   // para uma dada aula, identifica-se o bloco ao qual a aula em
   // quest�o pertence e em seguida adiciona-se o professor ao
   // grupo de professores do bloco em quest�o.
   std::map< BlocoCurricular *, GGroup< Professor *, LessPtr< Professor > > > blocosProfs;

   // Armazena os horarios que j� foram alocados para cada sala.
   // Estrutura: para cada sala, retorna-se o conjuntos de pares
   // dia da semana / de horario aula que a sala j� est� ocupada.
   //std::map< Sala *, GGroup< std::pair< int /*Dia Semana*/, HorarioAula * > >, LessPtr<Sala> > horariosAulaSala;
   std::map< Sala*,  std::map< int /*Dia*/, GGroup< HorarioAula *, LessPtr< HorarioAula > > >,
			 LessPtr< Sala > > horariosAulaSala;

   // M�todos
   void executaFuncaoPrioridade( Campus &, GGroup< Professor *, LessPtr< Professor > > & );

   // Fun��o auxilar � fun��o de prioridade.
   void calculaCustoFixProf( Professor &, Aula &, unsigned, int = 0, int = 0 );

   /* Fun��o recursiva de aloca��o de aulas. */
   void alocaAulasRec(bool primeiraTentativaAlocacao, bool tentarManterViabilidade);

   // Fun��o auxiliar.
   Professor & addProfessor();

   // Fun��o que utiliza a estrutura acima para
   // dizer se um dado professor est� sendo alocado
   // mais de uma vez para um bloco curricular.
   bool professorRepetido( Professor &, Aula & );

};

#endif // _SOLUCAO_INICIAL_OPERACIONAL_H_
