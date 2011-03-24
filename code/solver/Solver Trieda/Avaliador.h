#ifndef _AVALIADOR_H_
#define _AVALIADOR_H_

#include <vector>
#include "ProblemData.h"
#include "SolucaoOperacional.h"

class Avaliador
{
public:
   Avaliador(void);
   virtual ~Avaliador(void);

   // Retorna o valor de uma solu��o operacional
   double avaliaSolucao(SolucaoOperacional &);

private:
	//------------------ M�TODOS DE AVALIA��O DA SOLU��O -----------------//
	// -----------------------------------------------------------
	// Avalia��o de fixa��es n�o atendidas na solu�ao
	// -----------------------------------------------------------
	void calculaViolacaoRestricaoFixacao(SolucaoOperacional &);
	double totalViolacaoRestricaoFixacao;

	// -----------------------------------------------------------
	// Avalia��o de viola��es de tempo de deslocamento
	// entre campus e/ou unidades ocorreram na solu��o,
	// armazenando o total de tempo ocorrido nessas viola��es
	// -----------------------------------------------------------
	void calculaViolacoesDescolamento(SolucaoOperacional &);
	double totalViolacoesDescolamento;
	double totalTempoViolacoesDescolamento;

	// -----------------------------------------------------------
	// Avalia��o de gap's nos hor�rios dos professores
	// --> Cada Professor possui um conjunto de gap's
	// -----------------------------------------------------------
	void calculaGapsHorariosProfessores(SolucaoOperacional &);
	vector<vector<int>> gapsProfessores;
	double totalGapsHorariosProfessores;

	// -----------------------------------------------------------
	// Avalia��o da NOTA e do CUSTO do corpo
	// docente da maneira que foi alocado na solu��o
	// -----------------------------------------------------------
	void avaliacaoCustoCorpoDocente(SolucaoOperacional &);
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
	void violacoesCargasHorarias(SolucaoOperacional &);
	int totalViolacoesCHMinimaSemestreAterior;
	int totalViolacoesCHMinimaProfessor;
	int totalViolacoesCHMaximaProfessor;
	vector<int> violacoesCHMinimaSemestreAterior;
	vector<int> violacoesCHMinimaProfessor;
	vector<int> violacoesCHMaximaProfessor;

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
	void avaliaDiasProfessorMinistraAula(SolucaoOperacional &);
	int totalDiasProfessorMinistraAula;
	vector<int> professorMinistraAula;

	// -----------------------------------------------------------
	// Avalia��o da aloca��o do professor na �ltima
	// aula do dia D e na primeira aula do dia D+1
	// -----------------------------------------------------------
	void violacaoUltimaPrimeiraAula(SolucaoOperacional &);
	int totalViolacoesUltimaPrimeiraAula;
	vector<int> violacoesUltimaPrimeiraAulaProfessor;

	// -----------------------------------------------------------
	// Avalia��o se percentual m�nimo de mestres e doutores
	// por curso foi atendido ou n�o na solu��o operacional
	// -----------------------------------------------------------
	void avaliaNumeroMestresDoutores(SolucaoOperacional &);
	int totalViolacoesMestres;
	int totalViolacoesDoutores;

	// -----------------------------------------------------------
	// Avalia��o do m�ximo de disciplinas ppor professor por curso
	// -----------------------------------------------------------
	void avaliaMaximoDisciplinasProfessorPorCurso(SolucaoOperacional &);
	int totalViolacoesDiscProfCurso;
	vector<int> violacoesDisciplinasProfessor;

	// -----------------------------------------------------------
	// Avalia��o das viola��es de prefer�ncias dos professores
	// -----------------------------------------------------------
	void avaliaViolacoesPreferenciasProfessor(SolucaoOperacional &);
	int totalViolacoesPreferencias;
	vector<int> violacoesPreferenciasProfessor;
	//--------------------------------------------------------------------//

	//--------------------------- UTILIT�RIOS ----------------------------//
	// -----------------------------------------------------------
	// Tempo, em minutos, de cada hor�rio de aula
	// -----------------------------------------------------------
	int MINUTOS_POR_HORARIO;

	// -----------------------------------------------------------
	// Calcula o tempo NECESS�RIO para
	// se deslocar entre uma aula e outra
	// -----------------------------------------------------------
	double calculaTempoEntreCampusUnidades(
		SolucaoOperacional& ,Campus*, Campus*, Unidade*, Unidade*);

	// -----------------------------------------------------------
	// Informa quantos hor�rios um determinado
	// professor tem dispon�veis em um intervalo de aulas
	// -----------------------------------------------------------
	int horariosDisponiveisIntervalo(Professor*, int, Horario*, Horario*);

	// -----------------------------------------------------------
	// M�todo que calcula quantos hor�rios por dia s�o
	// representados na matriz de solu��o operacional.
	// Esse resulatado � dado pelo maior n�mero de
	// cr�ditos dispon�veis dentre os professores da
	// matriz de solu��o operacional informada
	// -----------------------------------------------------------
	int calculaTamanhoBlocoAula(SolucaoOperacional &);

	// -----------------------------------------------------------
	// Retorna o curso com o id informado
	// -----------------------------------------------------------
	Curso* procuraCurso(int, GGroup<Curso*>);
	//--------------------------------------------------------------------//
};

#endif