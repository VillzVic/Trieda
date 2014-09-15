#ifndef _MIP_ALOCAR_ALUNOS_H_
#define _MIP_ALOCAR_ALUNOS_H_

#include "ParametrosHeuristica.h"
#include "MIPAloc.h"
#include "SalaHeur.h"

class SolucaoHeur;
class OfertaDisciplina;
class TurmaHeur;
class AlunoDemanda;
class AlunoHeur;
class Curso;
class SaveSolucao;

using namespace std;

static int nrMipsAlocarAlunos = 0;

class MIPAlocarAlunos : MIPAloc
{
public:
	MIPAlocarAlunos(SolucaoHeur* const solucao, SaveSolucao* const &solucaoHeur, bool realocSalas, double minRecCred, int priorAluno, bool alocarP2);
	virtual ~MIPAlocarAlunos(void);

	// alocar alunos
	void alocar(void);

	// só trocar salas e fechar turmas caso seja necessario
	void realocarSalas(void);

	// usado também em TurmaHeur->getMIPCoef
	static double getCoefVarAlunoOft_(OfertaDisciplina* const oferta, AlunoHeur* const aluno, int prioridade);

private:
	// solucao actual (turma -> [set aluno.id / sala.id])
	SaveSolucao* const &solucaoHeur_;
	bool realocSalas_;			// realocar salas?
	const int priorAluno_;		// prioridade máxima de alunos que podemos atender
	const bool alocarP2_;
	const double minRecCred_;	// minimo de receita crédito imposto
	
	// modo só trocar salas?
	bool soTrocarSalas_; 

	#pragma region [MAIN METHODS]

	// set parametros LP
	void setParametrosLP_(void);
	// preparar fase II
	void setFaseII(void);
	// Carregar solução
	void carregarSolucao(void);

	#pragma endregion


	#pragma region [CRIAR VARIÁVEIS]

	//-------------------- ESTRUTURAS ----------------------------------
	// varsAlunoOferta
	unordered_map<int, unordered_map<OfertaDisciplina*, int>> varsAlunoOferta_;	// aluno.id -> oferta -> colNr
	unordered_map<OfertaDisciplina*, unordered_map<int, int>> varsOfertaAluno_; // oferta -> aluno.id -> colNr
	unordered_map<int, unordered_map<int, unordered_map<int, int>>> varsAlunoOfertaCampusDisc_;   // campus.Id -> disciplina.Id -> aluno.id -> colNr
	int nrVarsAlunoOferta_;
	
	// varsAlunoTurma
	unordered_map<int, unordered_map<TurmaHeur*, int>> varsAlunoTurma_;			// aluno.id -> turma -> colNr
	unordered_map<TurmaHeur*, unordered_map<int, int>> varsAlunoPorTurma_;			// turma -> aluno.id -> colNr
	int nrVarsAlunoTurma_;
	// varsAbrirTurma
	unordered_map<TurmaHeur*, int> varsAbrirTurma_;								// turma -> colNr
	int nrVarsAbrirTurma_;
	unordered_set<int> colNrsTurmasAbrir;											// nrs das colunas de abertura de turmas que devem ficar abertas
	
	// varsTurmaSala
	unordered_map<TurmaHeur*, unordered_map<SalaHeur*, int>> varsTurmaSala_;			// turma -> sala.id -> colNr
	unordered_map<SalaHeur*, unordered_map<TurmaHeur*, int>> varsSalaTurma_;			// sala.id -> turma -> colNr
	int nrVarsTurmaSala_;

	// varsAssocTP	(associam uma teórica a uma prática)
	unordered_map<TurmaHeur*, unordered_map<TurmaHeur*, int>> varsAssocTurmasTP;		// turma -> [turma, colNr]
	int nrVarsAssoc_;
	unordered_set<int> assocsFixadas;	// col nrs de associações fixadas!

	// só em modo improve solução
	unordered_map<TurmaHeur*, int> varsViolaMin;								// turma -> colNr
	int nrVarsViolaMin_;


	//-------------------- MÉTODOS ----------------------------------
	// criar variáveis do modelo
	void criarVariaveis_(void);
	// criar variável binária aluno oferta -> 1 se o aluno for alocado à oferta disciplina
	void criarVariavelAlunoOferta_(int campusId, int disciplinaId, OfertaDisciplina* const oferta, AlunoHeur* aluno, int prioridade);
	// criar variáveis binárias aluno turma -> 1 se o aluno for alocado à turma
	void criarVariaveisAlunoTurma_(OfertaDisciplina* const oferta, unordered_set<TurmaHeur*> &turmas, int alunoId, AlunoDemanda* demanda);
	// criar variáveis binárias abrir turma -> 1 se a turma for de facto aberta
	void criarVariaveisAbrirTurma_(unordered_set<TurmaHeur*> const &turmas);
	// criar variáveis binárias de associação entre turmas teóricas e práticas
	// obs: só se for 1x1 ou 1xN
	void criarVariaveisAssocTurmas_(OfertaDisciplina* const oferta);
	// criar variaveis turma sala
	void criarVariaveisTurmaSala_(OfertaDisciplina* const oferta, bool teorico);

	static double getCoefVarAbrirTurma_(TurmaHeur* const turma);

	#pragma endregion

	#pragma region [CRIAR RESTRIÇÕES]

	// criar restrições do modelo
	void criarRestricoes_(void);

	// criar restrições que obrigam o aluno a, quando é alocado a uma oferta disciplina, ter que ser alocado a uma turma para cada componente
	void criarRestricoesTurmasOferta_(int alunoId, OfertaDisciplina* const oferta, int colNrAlunoOft, 
										unordered_map<int, TurmaHeur*> const &turmasTeoricas, unordered_map<int, TurmaHeur*> const &turmasPraticas);
	// obriga o aluno a cursar pelo menos uma destas turmas se estiver alocado à disciplina
	bool criarRestricoesTurmasOfertaComp_(int alunoId, OfertaDisciplina* const oferta, int colNrAlunoOft, 
											unordered_map<int, TurmaHeur*> const &turmas, unordered_map<TurmaHeur*, int> const &varsAlunoTurma);

	// restrições que limitam a uma só o número de disciplinas alocadas a um aluno, de entre um grupo de disciplinas equivalentes
	void criarRestricoesMaxUmEquiv_(AlunoHeur* const aluno);
	// restrição que limita o numero de creditos que um aluno pode cursar
	void criarRestricoesMaxCredsAluno_(AlunoHeur* const aluno);
	// restições de co-requisitos do aluno
	void criarRestricoesCoReqsAluno_(int alunoId);

	// criar as restrições que impedem que um aluno curse duas turmas que são incompatíveis
	void criarRestricoesTurmasIncompat_(int alunoId);
	// criar restricoes assign turma a sala
	void criarRestricaoAssignTurmaSala_(TurmaHeur* const turma);
	// criar restrições que determinam o máximo e minimo de alunos numa turma
	void criarRestricoesMaxMinAlunosTurma_(TurmaHeur* const turma);
	// criar restrições de turmas incompativeis para salas
	void criarRestricoesTurmasIncompat_(unordered_map<SalaHeur*, unordered_map<TurmaHeur*, int>>::const_iterator const &itSala);
	// criar restrições que impedem o aluno de ser alocado a um par de turmas teorica e pratica se não estiverem associadas
	// obs: só se 1x1 ou 1xN
	void criarRestAssocTurmasAluno_(int alunoId, OfertaDisciplina* const oferta, int colNrAlunoOft,
									unordered_map<int, TurmaHeur*> const &turmasTeoricas);

	// obrigar cada turma de ofertasdisciplina que têm duas componentes a ter pelo menos uma associação
	// obs: só se 1x1 ou 1xN
	void criarRestAssocTurma_(TurmaHeur* const turma);

	// criar restrição para não deixar fechar turmas carregadas
	void criarRestricaoMustAbrirTurmas_(void);
	// restrições que obrigam a deixar associações de turmas fixadas
	void criarRestricaoAssocsFix_(void);
	// criar restrição que impõe um mínimo de produtividade por crédito
	void criarRestricaoMinProdutCred_(void);

	// obter a variável oferta aluno a partir do id do campus, disciplina e do aluno. retorna -1 se não for encontrada
	int getVarAlunoOferta_(int campusId, int discId, int alunoId);
	// obter as variáveis oferta aluno a partir da disciplina e do aluno.
	unordered_set<int> getVarsAlunoOferta_(int discId, int alunoId);
	// verifica se o aluno já tem uma var para aquela oferta
	bool jaTemVarAlunoOferta_(OfertaDisciplina* const oferta, int alunoId);

	// verifica se tem o mínimo de alunos
	bool temMinimo(unordered_map<int, int> const &alunosTurma, const int min);
	// intersectar sets. retorna nr de formandos. seta o numero de alunos fora da intersecção em cada turma
	int intersectAlunosTurma(unordered_map<int, int> const &alunosUm, unordered_map<int, int> const &alunosDois, unordered_set<int> &inter,
							int &outUm, int &outDois);

	#pragma endregion

	// pega na solução herdada da heurística e carrega-a para o MIP
	void definirMIPStart_(void);
	// adicionar associação de turmas ao mip start
	void definirTurmasAssocMIPStart_(TurmaHeur* const turma, double* &values_);
};

#endif