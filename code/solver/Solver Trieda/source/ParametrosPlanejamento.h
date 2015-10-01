#ifndef _NIVEL_DIFICULDADE_HORARIO_H_
#define _PARAMETROS_PLANEJAMENTO_H_

#include "OFBase.h"

#include "NivelDificuldadeHorario.h"

class ParametrosPlanejamento : public OFBase
{
public:
	enum ConstraintLevel
	{
		Off = 0,
		Weak = 1,
		Strong = 2
	};

	std::string modo_otimizacao;
	GGroup< NivelDificuldadeHorario * > niveis_dificuldade_horario;

	GGroup< int > maximizar_avaliacao_cursos;
	GGroup< int > minimizar_custo_docente_cursos;
	GGroup< GGroup< int > * > nao_permite_compart_turma;
	enum CHS { EQUILIBRAR, MINIMIZAR_DIAS, INDIFERENTE };
	CHS carga_horaria_semanal_aluno;
	CHS carga_horaria_semanal_prof;

	int maxDeslocProf;

	// funcao objetivo:
	// 0 = maximizar margem
	// 1 = minimizar custo docente
	int funcao_objetivo;

	double perc_max_reducao_CHP;
	double minAtendPercPorAluno;

	// ALUNO ou BLOCOCURRICULAR
	std::string otimizarPor;

	bool equilibrar_diversidade_disc_dia;
	bool minimizar_desloc_prof;
	bool minimizar_desloc_aluno;
	bool minimizar_horarios_vazios_professor;
	bool custo_prof_disponibilidade;
	bool evitar_reducao_carga_horaria_prof;
	bool evitar_prof_ultimo_primeiro_hr;
	bool considerar_equivalencia;
	bool permite_compartilhamento_turma_sel;
	bool permitir_alunos_em_varios_campi;
	bool min_mestres;
	bool min_doutores;
	bool minContratoIntegral;
	bool minContratoParcial;
	bool considerar_preferencia_prof;
	bool considerar_desempenho_prof;

	bool min_alunos_abertura_turmas;
	int min_alunos_abertura_turmas_value;
	bool min_alunos_abertura_turmas_praticas;
	int min_alunos_abertura_turmas_praticas_value;
	bool violar_min_alunos_turmas_formandos;

	bool utilizarDemandasP2;
	bool considerar_equivalencia_por_aluno;
	bool considerar_disponibilidade_prof_em_tatico;
	bool considerar_disciplinas_incompativeis_no_dia;
	bool maximoDisciplinasDeUmProfessorPorCurso;

	bool discPratTeor1x1;
	bool discPratTeor1xN;
	bool discPratTeorMxN;

	bool discPratTeor1xN_antigo;
	bool considerarMaxDiasPorProf;
	bool minCredsDiariosPorProf;
	bool considerarMinPercAtendAluno;
	bool min_folga_ocupacao_sala;
	bool limitar_unidades_por_periodo;
	bool limitar_salas_por_periodo;

	bool considerarDescansoMinProf;
	double descansoMinProfValue;

	int proibirProfGapMTN;
	int proibirProfVirtualGapMTN;

	int proibirAlunoGap;

	bool priorizarCalouros;
	bool priorizarFormandos;

	bool minDeslocAlunoEntreUnidadesNoDia;

	bool considerarCoRequisitos;
	bool considerarPrioridadePorAlunos;

	bool regrasGenericasDivisaoCredito;
	bool regrasEspecificasDivisaoCredito;

	virtual void le_arvore(ItemParametrosPlanejamento &);
};

std::ostream & operator<<(std::ostream &, ParametrosPlanejamento &);

#endif
