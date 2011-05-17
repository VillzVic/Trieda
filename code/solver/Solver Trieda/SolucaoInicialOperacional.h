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

   // Armazena todas as aulas que devem ser alocadas.
   GGroup<Aula*,LessPtr<Aula> > aulas;


   MoveValidator * moveValidator;

   // Estrutura que armazena o custo de alocar um professor a uma dada aula.
   std::map< std::pair< Professor *, Aula * >, CustoAlocacao * > custoProfTurma;

   // Armazena, em ordem decrescente, os Custos de Alocação.
   std::vector< CustoAlocacao * > custosAlocacaoAulaOrdenado;

   // Estrutura que armazena todas as aulas que não foram
   // associadas a nenhum professor. Baseando-se nessa
   // estrutura, pode-se dizer se será necessário criar
   // professores virtuais, ou não.
   // Est. VAZIA -> a princípio, não precisa de prof. virtual.
   // Est. com um, ou mais elementos -> com crtz é necessário
   // criar um, ou mais, professores virtuais.
   GGroup< Aula * > aulasNaoRelacionadasProf;

   // Cada professor deve, preferencialmente, ministrar apenas
   // uma disciplina de um bloco curricular. A estrutura abaixo
   // associa cada bloco com os professores selecionados. Ou seja,
   // para uma dada aula, identifica-se o bloco ao qual a aula em
   // questão pertence e em seguida adiciona-se o professor ao
   // grupo de professores do bloco em questão.
   std::map< BlocoCurricular *, GGroup< Professor *, LessPtr< Professor > > > blocosProfs;

   // Armazena para cada sala os horarios que já foram alocados.
   // Estrutura: para cada sala, retorna-se o conjuntos de pares
   // de horario aula / dia da semana que a sala já está ocupada
   std::map< Sala *, GGroup< std::pair< HorarioAula *, int > > > sala_horarios_alocados;


   // Métodos


   void executaFuncaoPrioridade();

   // Função auxilar à função de prioridade.
   void calculaCustoFixProf( Professor &, Aula &, unsigned, int = 0, int = 0 );
   
   // Função que utiliza a estrutura acima para
   // dizer se um dado professor está sendo alocado
   // mais de uma vez para um bloco curricular.
   bool professorRepetido( Professor &, Aula & );

   // novo nome pro metodo : possuiFixacaoProfDiaHorario
   bool possui_fixacao_professor_dia_horario( Professor &, int, int );

   bool aulaFixadaProfDiaHorario (Aula const & aula) const;

   bool alocaAulaSeq( SolucaoOperacional *, std::vector< Aula * >::iterator, int, Professor &, Aula &);
};

#endif // _SOLUCAO_INICIAL_OPERACIONAL_H_
