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

   // Retorna o valor de uma solução operacional
   // Obs.: o parâmetro 'bool' informa se o
   // avaliador deve ou não imprimir os resultados
   // detalhados da avaliação da solução operacional
   double avaliaSolucao( SolucaoOperacional &, bool = false );

   // Pesos atribuídos a cada avaliação da solução
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
	//------------------ MÉTODOS DE AVALIAÇÃO DA SOLUÇÃO -----------------//
	// -----------------------------------------------------------
	// Avaliação de fixações não atendidas na soluçao
	// -----------------------------------------------------------
	void calculaViolacaoRestricaoFixacao( SolucaoOperacional & );
	double totalViolacaoRestricaoFixacao;

	// -----------------------------------------------------------
	// Avaliação do deslocamento do professor (entre
	// unidades e/ou campus) na solução operacional dada
	// -----------------------------------------------------------
	// Os comentário ao lado da variáveis referem-se ao número
	// do item do critério de avaliação no Product Backlog,
	// para evitar mal entendimento do significado das variáveis
	// -----------------------------------------------------------
	void calculaDescolamentoProfessor( SolucaoOperacional & );
	int totalViolacoesDeslocamentoProfessor; // TRIEDA-776
	int totalDeslocamentosProfessor;         // TRIEDA-777

	// -----------------------------------------------------------
	// Avaliação de violações de tempo de deslocamento
	// entre campus e/ou unidades ocorreram na solução,
	// em relação a cada bloco/sub-bloco curricular
	// -----------------------------------------------------------
	// Os comentário ao lado da variáveis referem-se ao número
	// do item do critério de avaliação no Product Backlog,
	// para evitar mal entendimento do significado das variáveis
	// -----------------------------------------------------------
	void calculaDescolamentoBlocoCurricular(SolucaoOperacional &);
	int totalViolacoesDescolamento; // TRIEDA-739
	double totalTempoDescolamento;  // TRIEDA-740

	// -----------------------------------------------------------
	// Avaliação de gap's nos horários dos professores
	// --> Cada Professor possui um conjunto de gap's
	// -----------------------------------------------------------
	void calculaGapsHorariosProfessores( SolucaoOperacional & );
	std::vector< std::vector< int > > gapsProfessores;
	double totalGapsHorariosProfessores;

	// -----------------------------------------------------------
	// Avaliação da NOTA e do CUSTO do corpo
	// docente da maneira que foi alocado na solução
	// -----------------------------------------------------------
	void avaliacaoCustoCorpoDocente( SolucaoOperacional & );
	double totalAvaliacaoCorpoDocente;
	double totalCustoCorpoDocente;

	// -----------------------------------------------------------
	// Avaliação das violações de carga horária
	// dos professores verificando os três seguintes
	// critérios: carga horária do SEMESTRE ANTERIOR,
	// carga horárias MÍNIMA e MÁXIMA de cada professor.
	// -----------------------------------------------------------
	// Obs.: Os vetores abaixo armazenam quantas foram
	// as violações de cada tipo para cada professor.
	// O professor que corresponde à linha 'i' da matriz
	// de solução terá suas violações armazenadas nas
	// colunas de índice 'i' dos três vetores abaixo.
	// -----------------------------------------------------------
	void violacoesCargasHorarias( SolucaoOperacional & );
	int totalViolacoesCHMinimaSemestreAterior;
	int totalViolacoesCHMinimaProfessor;
	int totalViolacoesCHMaximaProfessor;
	std::vector< int > violacoesCHMinimaSemestreAterior;
	std::vector< int > violacoesCHMinimaProfessor;
	std::vector< int > violacoesCHMaximaProfessor;

	// -----------------------------------------------------------
	// Avaliação do número de dias que cada
	// professor está ministrando pelo menos uma aula
	// -----------------------------------------------------------
	// Obs.: Os vetor abaixo armazena quantos dias da
	// semana cada professor tem aulas alocadas.
	// O professor que corresponde à linha 'i' da matriz
	// de solução terá seu total de dias armazenado na
	// coluna de índice 'i' do vetor abaixo.
	// -----------------------------------------------------------
	void avaliaDiasProfessorMinistraAula( SolucaoOperacional & );
	int totalDiasProfessorMinistraAula;
	std::vector< int > professorMinistraAula;

	// -----------------------------------------------------------
	// Avaliação da alocação do professor na última
	// aula do dia D e na primeira aula do dia D+1
	// -----------------------------------------------------------
	void violacaoUltimaPrimeiraAula( SolucaoOperacional & );
	int totalViolacoesUltimaPrimeiraAula;
	std::vector< int > violacoesUltimaPrimeiraAulaProfessor;

	// -----------------------------------------------------------
	// Avaliação se percentual mínimo de mestres e doutores
	// por curso foi atendido ou não na solução operacional
	// -----------------------------------------------------------
	void avaliaNumeroMestresDoutores( SolucaoOperacional & );
	int totalViolacoesMestres;
	int totalViolacoesDoutores;

	// -----------------------------------------------------------
	// Avaliação do máximo de disciplinas por professor por curso
	// -----------------------------------------------------------
	void avaliaMaximoDisciplinasProfessorPorCurso( SolucaoOperacional & );
	int totalViolacoesDiscProfCurso;
	std::vector< int > violacoesMaximoDisciplinasProfessor;

	// -----------------------------------------------------------
	// Avaliação das violações de preferências dos professores
	// -----------------------------------------------------------
	void avaliaPreferenciasProfessorDisciplina( SolucaoOperacional & );
   // Informa o total geral de violações de preferência de disciplinas
	int totalPreferenciasProfessorDisciplina;
   // Informa o total de violações de preferência de disciplinas para cada professor
   std::map< Professor *, int, LessPtr< Professor > > violacoesPreferenciasProfessor;

	// -----------------------------------------------------------
	// Avaliação do custo de se utilizar um ou
	// mais professores virtuais na solução operacional
	// -----------------------------------------------------------
	void avaliaCustoProfessorVirtual( SolucaoOperacional & );
	int totalProfessoresVirtuais;
	int totalCreditosProfessoresVirtuais;

	// -----------------------------------------------------------
	// Avaliação da porcentagem de professores tempo
	// parcial e tempo integral para cada curso
	// OBS.: são duas avaliações: uma que considera
	// a soma de tempo parcial com tempo integral e outra
	// que considera apenas os professores de tempo integral
	// -----------------------------------------------------------
	void avaliaTempoParcialIntegral( SolucaoOperacional & );
	int totalViolacoesTempoParcialIntegral;
	int totalViolacoesTempoIntegral;

	// -----------------------------------------------------------
	// Avaliação do número de aulas alocadas a professores,
   // tal que as disciplinas dessas aulas não estão associadas
   // a esse professor no campo 'disciplinasAssociadas'
	// -----------------------------------------------------------
	void avaliaDisciplinasAssociadasProfessor( SolucaoOperacional & );
   // Informa quantas disciplinas estão alocadas a
   // professores que não possuem essas disciplinas associadas a eles
	int totalViolacoesDisciplinasAssociadas;
   // Informa quais disciplinas estão alocadas a cada
   // professor, sem que as mesmas estão associadas ao profesor
   std::map< Professor *, std::vector< Disciplina * >,
      LessPtr< Professor > > violacoesDisciplinasAssociadasProfessor;

	// -----------------------------------------------------------
	// Avaliação do número de professores que
   // ministram mais de uma aula para o mesmo bloco curricular
	// -----------------------------------------------------------
	void avaliaProfessorMesmoBlocoCurricular( SolucaoOperacional & );
	int totalViolacoesProfessorMesmoBlocoCurricular;

	// -----------------------------------------------------------
	// Avaliação do número de aulas de um mesmo
   // bloco curricular alocadas para o mesmo horário
	// -----------------------------------------------------------
	void avaliaConflitosBlocoCurricular( SolucaoOperacional & );
	int totalViolacoesConflitosBlocoCurricular;
	//--------------------------------------------------------------------//

	//--------------------------- UTILITÁRIOS ----------------------------//
	// -----------------------------------------------------------
	// Tempo, em minutos, de cada horário de aula
	// -----------------------------------------------------------
	int MINUTOS_POR_HORARIO;

	// -----------------------------------------------------------
	// Informa quantos horários um determinado
	// professor tem disponíveis em um intervalo de aulas
	// -----------------------------------------------------------
	int horariosDisponiveisIntervalo( Professor *, int, Horario *, Horario * );

	// -----------------------------------------------------------
	// Método que calcula quantos horários por dia são
	// representados na matriz de solução operacional.
	// Esse resulatado é dado pelo maior número de
	// créditos disponíveis dentre os professores da
	// matriz de solução operacional informada
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
	// Exibe os resultados da avaliação, APÓS todo o
	// processamento de avaliação da soluçao operacioanal
	// -----------------------------------------------------------
	void imprimeResultados();
	//--------------------------------------------------------------------//
};

#endif