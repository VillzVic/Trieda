#ifndef _AVALIADOR_H_
#define _AVALIADOR_H_

#include <vector>
#include <cmath>
#include <map>

#include "ofbase.h"
#include "Fixacao.h"
#include "DateTime.h"
#include "ProblemData.h"
#include "SolucaoOperacional.h"

class Avaliador
{
public:
   Avaliador( void );
   virtual ~Avaliador( void );

   // Retorna o valor de uma solu��o operacional
   // Obs.: o par�metro 'bool' informa se o
   // avaliador deve ou n�o imprimir os resultados
   // detalhados da avalia��o da solu��o operacional
   double avaliaSolucao( SolucaoOperacional &, bool = false );

   // Pesos atribu�dos a cada avalia��o da solu��o
   double PESO_FIXACAO;
   double PESO_DESLOCAMENTO;
   double PESO_TEMPO_DESLOCAMENTO;
   double PESO_VIOLACAO_DESLOCAMENTO_PROFESSOR;
   double PESO_DESLOCAMENTO_PROFESSOR;
   double PESO_GAPS_HORARIO;
   double PESO_NOTA_CORPO_DOCENTE;
   double PESO_CUSTO_CORPO_DOCENTE;
   double PESO_CH_MINIMA_ANTERIOR;
   double PESO_CH_MINIMA_PROFESSOR;
   double PESO_CH_MAXIMA_PROFESSOR;
   double PESO_TOTAL_DIAS_SEMANA;
   double PESO_ULTIMA_E_PRIMEIRA_AULA;
   double PESO_PERCENTUAL_MESTRES;
   double PESO_PERCENTUAL_DOUTORES;
   double PESO_DISCIPLINAS_PROFESSOR_CURSO;
   double PESO_PREFERENCIA_DISCIPLINA;
   double PESO_NUMERO_PROFESSORES_VIRTUAIS;
   double PESO_CREDITOS_PROFESSORES_VIRTUAIS;
   double PESO_VIOLACOES_PARCIAL_INTEGRAL;
   double PESO_VIOLACOES_INTEGRAL;
   double PESO_VIOLACOES_DISCIPLINAS_ASSOCIADAS;
   double PESO_VIOLACOES_PROFESSOR_MESMO_BLOCO;
   double PESO_VIOLACOES_CONFLITO_BLOCO_CURRICULAR;

private:
	//------------------ M�TODOS DE AVALIA��O DA SOLU��O -----------------//
	// -----------------------------------------------------------
	// Avalia��o de fixa��es n�o atendidas na solu�ao
	// -----------------------------------------------------------
	void calculaViolacaoRestricaoFixacao( SolucaoOperacional & );
	double totalViolacaoRestricaoFixacao;

	// -----------------------------------------------------------
	// Avalia��o do deslocamento do professor (entre
	// unidades e/ou campus) na solu��o operacional dada
	// -----------------------------------------------------------
	// Os coment�rio ao lado da vari�veis referem-se ao n�mero
	// do item do crit�rio de avalia��o no Product Backlog,
	// para evitar mal entendimento do significado das vari�veis
	// -----------------------------------------------------------
	void calculaDescolamentoProfessor( SolucaoOperacional & );
	int totalViolacoesDeslocamentoProfessor; // TRIEDA-776
	int totalDeslocamentosProfessor;         // TRIEDA-777

	// -----------------------------------------------------------
	// Avalia��o de viola��es de tempo de deslocamento
	// entre campus e/ou unidades ocorreram na solu��o,
	// em rela��o a cada bloco/sub-bloco curricular
	// -----------------------------------------------------------
	// Os coment�rio ao lado da vari�veis referem-se ao n�mero
	// do item do crit�rio de avalia��o no Product Backlog,
	// para evitar mal entendimento do significado das vari�veis
	// -----------------------------------------------------------
	void calculaDescolamentoBlocoCurricular(SolucaoOperacional &);
	int totalViolacoesDescolamento; // TRIEDA-739
	double totalTempoDescolamento;  // TRIEDA-740

	// -----------------------------------------------------------
	// Avalia��o de gap's nos hor�rios dos professores
	// --> Cada Professor possui um conjunto de gap's
	// -----------------------------------------------------------
	void calculaGapsHorariosProfessores( SolucaoOperacional & );
	std::vector< std::vector< int > > gapsProfessores;
	double totalGapsHorariosProfessores;

	// -----------------------------------------------------------
	// Avalia��o da NOTA e do CUSTO do corpo
	// docente da maneira que foi alocado na solu��o
	// -----------------------------------------------------------
	void avaliacaoCustoCorpoDocente( SolucaoOperacional & );
	double totalAvaliacaoCorpoDocente;
	double totalCustoCorpoDocente;

	// -----------------------------------------------------------
	// Avalia��o das viola��es de carga hor�ria
	// dos professores verificando os tr�s seguintes
	// crit�rios: carga hor�ria do SEMESTRE ANTERIOR,
	// carga hor�rias M�NIMA e M�XIMA de cada professor.
	// -----------------------------------------------------------
	// Obs.: Os vetores abaixo armazenam quantas foram
	// as viola��es de cada tipo para cada professor.
	// O professor que corresponde � linha 'i' da matriz
	// de solu��o ter� suas viola��es armazenadas nas
	// colunas de �ndice 'i' dos tr�s vetores abaixo.
	// -----------------------------------------------------------
	void violacoesCargasHorarias( SolucaoOperacional & );
	int totalViolacoesCHMinimaSemestreAterior;
	int totalViolacoesCHMinimaProfessor;
	int totalViolacoesCHMaximaProfessor;
	std::vector< int > violacoesCHMinimaSemestreAterior;
	std::vector< int > violacoesCHMinimaProfessor;
	std::vector< int > violacoesCHMaximaProfessor;

	// -----------------------------------------------------------
	// Avalia��o do n�mero de dias que cada
	// professor est� ministrando pelo menos uma aula
	// -----------------------------------------------------------
	// Obs.: Os vetor abaixo armazena quantos dias da
	// semana cada professor tem aulas alocadas.
	// O professor que corresponde � linha 'i' da matriz
	// de solu��o ter� seu total de dias armazenado na
	// coluna de �ndice 'i' do vetor abaixo.
	// -----------------------------------------------------------
	void avaliaDiasProfessorMinistraAula( SolucaoOperacional & );
	int totalDiasProfessorMinistraAula;
	std::vector< int > professorMinistraAula;

	// -----------------------------------------------------------
	// Avalia��o da aloca��o do professor na �ltima
	// aula do dia D e na primeira aula do dia D+1
	// -----------------------------------------------------------
	void violacaoUltimaPrimeiraAula( SolucaoOperacional & );
	int totalViolacoesUltimaPrimeiraAula;
	std::vector< int > violacoesUltimaPrimeiraAulaProfessor;

	// -----------------------------------------------------------
	// Avalia��o se percentual m�nimo de mestres e doutores
	// por curso foi atendido ou n�o na solu��o operacional
	// -----------------------------------------------------------
	void avaliaNumeroMestresDoutores( SolucaoOperacional & );
	int totalViolacoesMestres;
	int totalViolacoesDoutores;

	// -----------------------------------------------------------
	// Avalia��o do m�ximo de disciplinas por professor por curso
	// -----------------------------------------------------------
	void avaliaMaximoDisciplinasProfessorPorCurso( SolucaoOperacional & );
	int totalViolacoesDiscProfCurso;
	std::vector< int > violacoesMaximoDisciplinasProfessor;

	// -----------------------------------------------------------
	// Avalia��o das viola��es de prefer�ncias dos professores
	// -----------------------------------------------------------
	void avaliaPreferenciasProfessorDisciplina( SolucaoOperacional & );
   // Informa o total geral de viola��es de prefer�ncia de disciplinas
	int totalPreferenciasProfessorDisciplina;
   // Informa o total de viola��es de prefer�ncia de disciplinas para cada professor
   std::map< Professor *, int, LessPtr< Professor > > violacoesPreferenciasProfessor;

	// -----------------------------------------------------------
	// Avalia��o do custo de se utilizar um ou
	// mais professores virtuais na solu��o operacional
	// -----------------------------------------------------------
	void avaliaCustoProfessorVirtual( SolucaoOperacional & );
	int totalProfessoresVirtuais;
	int totalCreditosProfessoresVirtuais;

	// -----------------------------------------------------------
	// Avalia��o da porcentagem de professores tempo
	// parcial e tempo integral para cada curso
	// OBS.: s�o duas avalia��es: uma que considera
	// a soma de tempo parcial com tempo integral e outra
	// que considera apenas os professores de tempo integral
	// -----------------------------------------------------------
	void avaliaTempoParcialIntegral( SolucaoOperacional & );
	int totalViolacoesTempoParcialIntegral;
	int totalViolacoesTempoIntegral;

	// -----------------------------------------------------------
	// Avalia��o do n�mero de aulas alocadas a professores,
   // tal que as disciplinas dessas aulas n�o est�o associadas
   // a esse professor no campo 'disciplinasAssociadas'
	// -----------------------------------------------------------
	void avaliaDisciplinasAssociadasProfessor( SolucaoOperacional & );
   // Informa quantas disciplinas est�o alocadas a
   // professores que n�o possuem essas disciplinas associadas a eles
	int totalViolacoesDisciplinasAssociadas;
   // Informa quais disciplinas est�o alocadas a cada
   // professor, sem que as mesmas est�o associadas ao profesor
   std::map< Professor *, std::vector< Disciplina * >,
      LessPtr< Professor > > violacoesDisciplinasAssociadasProfessor;

	// -----------------------------------------------------------
	// Avalia��o do n�mero de professores que
   // ministram mais de uma aula para o mesmo bloco curricular
	// -----------------------------------------------------------
	void avaliaProfessorMesmoBlocoCurricular( SolucaoOperacional & );
	int totalViolacoesProfessorMesmoBlocoCurricular;

	// -----------------------------------------------------------
	// Avalia��o do n�mero de aulas de um mesmo
   // bloco curricular alocadas para o mesmo hor�rio
	// -----------------------------------------------------------
	void avaliaConflitosBlocoCurricular( SolucaoOperacional & );
	int totalViolacoesConflitosBlocoCurricular;
	//--------------------------------------------------------------------//

	//--------------------------- UTILIT�RIOS ----------------------------//
	// -----------------------------------------------------------
	// Tempo, em minutos, de cada hor�rio de aula
	// -----------------------------------------------------------
	int MINUTOS_POR_HORARIO;

	// -----------------------------------------------------------
	// Informa quantos hor�rios um determinado
	// professor tem dispon�veis em um intervalo de aulas
	// -----------------------------------------------------------
	int horariosDisponiveisIntervalo( Professor *, int, Horario *, Horario * );

	// -----------------------------------------------------------
	// M�todo que calcula quantos hor�rios por dia s�o
	// representados na matriz de solu��o operacional.
	// Esse resulatado � dado pelo maior n�mero de
	// cr�ditos dispon�veis dentre os professores da
	// matriz de solu��o operacional informada
	// -----------------------------------------------------------
	int calculaTamanhoBlocoAula( SolucaoOperacional & );

	// -----------------------------------------------------------
	// Retorna o curso com o id informado
	// -----------------------------------------------------------
	Curso * procuraCurso( int, GGroup< Curso *, LessPtr< Curso > > );

	// -----------------------------------------------------------
	// Ordena um GGroup de aulas
	// -----------------------------------------------------------
	std::vector< Aula * > retornaVectorAulasOrdenado( GGroup< std::pair<Aula *, Horario * > > );

	// -----------------------------------------------------------
	// Exibe os resultados da avalia��o, AP�S todo o
	// processamento de avalia��o da solu�ao operacioanal
	// -----------------------------------------------------------
	void imprimeResultados();
	//--------------------------------------------------------------------//
};

#endif