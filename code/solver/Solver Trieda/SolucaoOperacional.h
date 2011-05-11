#ifndef _SOLUCAO_OPERACIONAL_H_
#define _SOLUCAO_OPERACIONAL_H_

#include <map>
#include <vector>

#include "input.h"
#include "Aula.h"
#include "Horario.h"
#include "Professor.h"
#include "ProblemData.h"
#include "MoveValidator.h"

typedef std::vector< std::vector< Aula * > * > MatrizSolucao;

class SolucaoOperacional
{
public:
   SolucaoOperacional( ProblemData * );

   SolucaoOperacional( SolucaoOperacional const & s );

   virtual ~SolucaoOperacional( void );

   // Dado um �ndice que corresponde a uma linha
   // de um dado professor na matriz de solu��o,
   // o dia da semana e o hor�rio de aula desejado,
   // retorna o hor�rio correspondente � aula
   // alocada (ou NULL, caso n�o haja aula alocada)
   Horario * getHorario( int, int, int );

   // Dado um �ndice que corresponde a uma linha da
   // matriz de solu��o, retorna o professor correspondente
   Professor * getProfessorMatriz( int );

   ProblemData * getProblemData() const;

   void setMatrizAulas( MatrizSolucao * );
   MatrizSolucao * getMatrizAulas() const;

   void toString();
   void toString2();

   // Dado o ID do professor, retorna um
   // ponteiro para o objeto 'Professor'
   std::map< int, Professor * > mapProfessores;

   // Retorna um iterator para o horario
   // desejado de um dado dia de um professor.
   // Dias considerados: 
   // 1: domingo
   // 2: segunda
   // 3: ter�a
   // 4: quarta
   // 5: quinta
   // 6: sexta
   // 7: s�bado
   std::vector< Aula * >::iterator getItHorariosProf( Professor &, int = 2, int = 0 );

   // Adiciona um novo professor � solu��o. O inteiro retornado
   // � referente � posi��o (linha) que o novo  professor ocupa na solu��o.
   int addProfessor(Professor &, std::vector< Aula * > &);

   // FUTURAMENTE, VAI SER NECESSARIO CRIAR UM METODO PARA REMOVER UM PROFESSOR.

   int getTotalDias() const;
   int getTotalHorarios() const;
   int getTotalDeProfessores() const;

   // Verifica se as aula <aX> e <aY> podem ter trocados os seus respectivos hor�rios.
   //bool podeTrocarHorariosAulas( Aula &, Aula & ) const;

   // Fun��o auxiliar � fun��o podeTrocarHorariosAulas
   //bool checkConflitoBlocoCurricular( Aula &, std::vector< std::pair< Professor *, Horario * > > & ) const;

   /* ToDo : utilizar a estrutura abaixo para alocar o mesmo professor para cr�ditos te�ricos e pr�ticos de
   uma disciplina (qdo a disc foi dividida). Uma outra maneira seria dar uma prioridade maior para alocar
   essas disciplinas. O problema � como tratar isso nos movimentos. */
   /* Armazena o professor alocado para ministrar os cr�ditos te�ricos e pr�ticos de uma disciplina. */
   //std::map<std::pair<int/*turma*/,int/*modulo do id da disciplina*/>, Professor*> profTurmaDiscAula;

   /* Checa a disponibilidade dos hor�rios da(s) sala(s) demandados pelas aulas em quest�o. */
   //bool checkDisponibilidadeHorarioSalaAula(
   //Aula & aula, 
   //std::vector< std::pair< Professor *, Horario * > > & novosHorariosAula) const;

   int indice_horario_aula_ordenado( int );

private:
   
   ProblemData * problem_data;

   int total_dias;
   int total_horarios;
   int total_professores;

   MatrizSolucao * matriz_aulas;

   // Verifica se o professor tem dispon�vel
   // um determinado hor�rio em um dia da semana
   bool horarioDisponivelProfessor( Professor *, int, int );

   // Informa quantos hor�ros cada dia da semana ter� na
   // matriz da solu��o. Esse valor corresponde � quantidade
   // m�xima que os dias da semana tem de hor�rios de aula.
   // Os dias que tiverem menos hor�rios de aula dever�o ter
   // aulas virtuais alocadas para completar a matriz
   int max_horarios_dia();
};

#endif