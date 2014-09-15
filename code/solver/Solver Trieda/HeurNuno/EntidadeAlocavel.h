#ifndef _ENTIDADE_ALOCAVEL_H_
#define _ENTIDADE_ALOCAVEL_H_

//#include "UtilHeur.h"
#include <unordered_map>
#include <unordered_set>
#include "GGroup.h"
#include "AulaHeur.h"

using namespace std;

class HorarioAula;
class AulaHeur;
class Campus;
class OfertaDisciplina;

class EntidadeAlocavel
{
public:
	EntidadeAlocavel(bool deslocavel, std::string const &tipo, bool sempreDisp = false);
	~EntidadeAlocavel(void);

	// add/remove turmas a que está alocado
	void addTurma(TurmaHeur* const turma);
	void removeTurma(TurmaHeur* const turma);

	// add/remove aulas dias da turma
	void addAulasTurma(TurmaHeur* const turma);
	void removeAulasTurma(TurmaHeur* const turma);

	// get turmas a que tá alocado
	int nrTurmas (void) const { return turmas_.size(); }
	void getTurmas(unordered_set<TurmaHeur*> &turmas);
	bool temTurmas(void) const;
	bool temTurma(TurmaHeur* turma) const;
	void getOfertasDisciplina(unordered_set<OfertaDisciplina*> &ofertas) const;
	void getOfertasDisciplina(unordered_set<OfertaDisciplina*>* const ofertas) const;

	// disponibilidade só olhando a conflitos de horário (e deslocamento se for o caso)
	bool estaDisponivel(int dia, AulaHeur* const aula) const;
	bool estaDisponivel(int dia, HorarioAula* const horario) const;
	// verificar se está disponivel naquela aula, naquele dia, tendo em conta a turma já alocada
	bool estaDisponivel(TurmaHeur* const turma, int dia, AulaHeur* const aula) const;
	bool estaDisponivel(unordered_map<int, AulaHeur*> const &aulas) const;

	// diz se tem nr max de horários sequenciais disponiveis num dia para um conjunto de horarios. 
	// OBS: não considerar sequencial intervalo grande! (PH: maxIntervalo!)
	bool temHorsSeqDisponivel(int nrHors, int dia, GGroup<HorarioAula*, LessPtr<HorarioAula>> const &horarios) const;

	// verifica a quantos créditos está alocado nesse dia
	int credsAlocDia(int dia) const;
	// última/primeira aula alocado no dia
	AulaHeur* ultAulaAlocDia(int dia) const;
	AulaHeur* primAulaAlocDia(int dia) const;

	// retorna os dias da semana nos quais a entidade tem alocação
	void getDiasSemanaAloc(unordered_set<int> &diasSemana) const;

	// verifica se dois horários/aulas são compatíveis tendo em conta a deslocação
	static bool violaDeslocacao(HorarioAula* const horario1, int campusId1, int unidadeId1, HorarioAula* const horario2, int campusId2, int unidadeId2);
	static bool violaDeslocacao(AulaHeur* const aula1, AulaHeur* const aula2);
	// verifica se este conjunto de aulas viola alguma deslocacao para o aluno
	bool violaAlgumaDeslocacao(unordered_map<int, AulaHeur*> const &aulas);
	
	// verifica se duas aulas são compatíveis tendo em conta a deslocação
	static bool aulasCompativeis(AulaHeur* const aulaUm, AulaHeur* const aulaDois, bool deslocavel = true);
	static bool aulasCompativeis(int aulaUmId, int aulaDoisId, bool deslocavel = true);

	// horarios incompativeis
	static bool horariosCompativeis(int campusIdUm, int unidIdUm, HorarioAula* const horUm, int campusIdDoi, int unidIdDois, HorarioAula* const horDois, bool deslocavel);

	// nr de créditos alocados
	int getNrCreditosAlocados(void) const { return credsAloc_; }
	// nr de dias alocados
	int getNrDiasAlocados(void) const { return diasAulas_.size(); }
	// verifica se está alocado no dia
	bool estaAlocadoDia(int dia) const { return (diasAulas_.find(dia) != diasAulas_.end()); }

	// verificar se há conflitos entre alocações
	bool temConflitos(void) const;

protected:
	unordered_set<TurmaHeur*> turmas_;	 // Turmas a que está alocado
	unordered_map<int, unordered_set<AulaHeur*> > diasAulas_;

	const bool deslocavel_; // aluno e prof = true / sala = false
	int credsAloc_;
	const std::string tipo_;
	const bool sempreDisp_;
};

#endif

