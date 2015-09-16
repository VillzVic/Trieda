#ifndef _IMPROVE_METHODS_H_
#define _IMPROVE_METHODS_H_

#include <unordered_map>
#include <unordered_set>
#include <set>

#include "AlunoHeur.h"
#include "OfertaDisciplina.h"

using namespace std;

class SolucaoHeur;
class TurmaHeur;
class AulaHeur;

class ImproveMethods
{
public:
	ImproveMethods(void);
	~ImproveMethods(void);

	virtual void foo() = 0;	// virtual function to make this an abstract class

	// [REALOC]

	// tentar realocar alunos nas aulas principais, quando só há um tipo de aula para maximizar demanda atendida
	static void tryRealocAlunosPrinc(SolucaoHeur* const solucao, OfertaDisciplina* const ofertaDisc, bool mudarSala, bool equiv, 
									int priorAluno, double relaxMin = 1.0);
	// tentar realocar alunos nas aulas práticas, quando estas são componente secundária
	static void tryRealocAlunosCompSec(OfertaDisciplina* const ofertaDisc, bool mudarSala, int priorAluno, double relaxMin = 1.0);
	
	// [ALOCAR NAO ATENDIDOS]

	// tentar alocar alunos não atendidos
	static void tryAlocNaoAtendidos(SolucaoHeur* const solucao, OfertaDisciplina* const ofertaDisc, bool mudarSala, bool equiv,
									bool realoc, int priorAluno, double relaxMin = 1.0);
	
	static void tryAlocNaoAtendSemRealoc(SolucaoHeur* const solucao, OfertaDisciplina* const ofertaDisc, bool mudarSala, bool equiv,  
										int priorAluno, double relaxMin = 1.0);
	static void tryAlocNaoAtendSemRealoc(OfertaDisciplina* const ofertaDisc, unordered_set<AlunoHeur*> const &alunos, bool mudarSala,
										double relaxMin = 1.0);

	// alocar alunos incompletos sem realocar
	static void tryAlocCompSecSemRealoc(OfertaDisciplina* const ofertaDisc, bool mudarSala);

	// [CHECK! não está a funcionar mt bem!]
	// tentar alocar nao atendidos olhando para os alunos e tentar realoca-los a outras disciplinas para alocar nesta
	static void tryAlocNaoAtendidosRealocOld(SolucaoHeur* const solucao, OfertaDisciplina* const ofertaDisc, bool mudarSala, bool equiv, 
											int priorAluno, bool relaxMin = 1.0);

	// tentar mudar de sala para a menor sala maior ou menor que a actual, que esteja disponível que maximize a utilização.  
	//Todos os alunos têm que estar disponíveis
	static bool tryMudarSala(TurmaHeur* const turma, bool maior, bool igual = false);
	static bool tryReduzirSalas(OfertaDisciplina* const oferta);
	// verificar se alunos estão disponiveis para o realoc
	static bool alunosDispRealoc(TurmaHeur* const turma);

private:

	// verifica para cada turma quantos e quais alunos estão disponiveis
	static bool checkAlunosDispTurmas(unordered_set<AlunoHeur*> const &alunos, unordered_set<TurmaHeur*> const &turmas,
										unordered_map<TurmaHeur*, int> &dispTurma,
										unordered_map<AlunoHeur*, unordered_set<TurmaHeur*>> &alunosTurma);
	
	// tentar realocar alunos nas turmas dando prioridade às que têm menos disponibilidade
	static void tryAlocAlunosComp(unordered_map<TurmaHeur*, int> &dispTurma, 
									unordered_map<AlunoHeur*, unordered_set<TurmaHeur*>> &alunosTurma, 
									bool mudarSala);

	// escolhe turma
	static TurmaHeur* escolheTurma(AlunoHeur* const aluno, unordered_map<AlunoHeur*, unordered_set<TurmaHeur*>> &alunosTurma, 
							unordered_map<TurmaHeur*, int> &dispTurma, unordered_set<TurmaHeur*> &turmasLotadas, 
							bool mudarSala);

	// update disponibilidade
	static void updateDisponibilidade(unordered_set<TurmaHeur*> const &turmasLotadas, unordered_map<TurmaHeur*, int> &dispTurma,
										unordered_map<AlunoHeur*, unordered_set<TurmaHeur*>> &alunosTurma);

	// verifica o pos realocação. retorna true se fica, false se solucao é reposta
	static bool checkPosRealoc(OfertaDisciplina* const oferta, unordered_map<TurmaHeur*, pair<unordered_set<AlunoHeur*>, SalaHeur*>> &oldAlocOft,
								int nrAlocCompIni, char* metodo, double relaxMin);

	// reset nrDisp
	static void resetNrDisp(unordered_set<TurmaHeur*> const &turmas);
	static void resetDispTurma(unordered_map<TurmaHeur*, int> &dispTurma);

	// ordenar as turmas
	static void getOrdTurmas(unordered_map<TurmaHeur*, int> &dispTurma, set<TurmaHeur*> &turmasOrd);
	// ordenar turmas pelas que têm mais disponiveis
	static void getOrdTurmasInv(unordered_map<TurmaHeur*, int> &dispTurma, set<TurmaHeur*> &turmasOrd);

	// get mapa alunos ordenado por ordem crescente por nr turmas que ta disponivel
	static void getAlunosOrd(unordered_map<AlunoHeur*, unordered_set<TurmaHeur*>> const &alunosTurma, map<int, unordered_set<AlunoHeur*>> &ordAlunos);

	// get turmasPorAluno
	static void getTurmasPorAluno(unordered_map<TurmaHeur*, unordered_set<AlunoHeur*>> const &alunosTurma, 
									unordered_map<AlunoHeur*, unordered_set<TurmaHeur*>> &turmasPorAluno);

	// repor alocacao oferta (caso realocação piore resultado)
	static void reporAlocacao(OfertaDisciplina* const oferta, unordered_map<TurmaHeur*, pair<unordered_set<AlunoHeur*>, SalaHeur*>> const &oldAlocOft);

	// init disp turmas
	static void initDispTurmas(unordered_set<TurmaHeur*> const &turmas, unordered_map<TurmaHeur*, int> &dispTurma);

	// [CHECK! não está a funcionar mt bem!]
	// algoritmo recursivo para testar varias combinações de turmas para ver se é possivel a realocação em ofertas já alocadas
	static bool tryRealocTurmasRec(AlunoHeur* const &aluno, unordered_set<OfertaDisciplina*> const &ofertasAluno, bool mudarSala);
			
	// iteração
	static bool tryRealocTurmasRecIter(AlunoHeur* const &aluno, unordered_set<TurmaHeur*> const &turmasComb,
										unordered_set<OfertaDisciplina*>::const_iterator const &itOft,
										unordered_set<OfertaDisciplina*>::const_iterator const &itEnd,
										vector<pair<unordered_set<OfertaDisciplina*>::const_iterator, unordered_set<TurmaHeur*>>*> &setCombs, 
										bool mudarSala);

	// check implement if end
	static bool checkImplementEnd(AlunoHeur* const &aluno, unordered_set<TurmaHeur*> const &turmasComb,
										unordered_set<OfertaDisciplina*>::const_iterator const &itOft,
										unordered_set<OfertaDisciplina*>::const_iterator const &itEnd,
										vector<pair<unordered_set<OfertaDisciplina*>::const_iterator, unordered_set<TurmaHeur*>>*> &setCombs);

	// Filtrar alunos que excedem creditos
	static void filtrarAlunosExcCreds(int nrCreds, unordered_set<AlunoHeur*> &alunos);
};


#endif

