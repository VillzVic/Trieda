#include "ParametrosPlanejamento.h"

#include <iostream>

using namespace std;

ostream & operator<<(ostream& out, ParametrosPlanejamento& param)
{
	out << "Parametros on:" << endl;

	if (param.equilibrar_diversidade_disc_dia == true)
		out << "equilibrar_diversidade_disc_dia" << endl;
	if (param.minimizar_desloc_prof == true)
		out << "minimizar_desloc_prof" << endl;
	if (param.minimizar_desloc_aluno == true)
		out << "minimizar_desloc_aluno" << endl;
	if (param.minimizar_horarios_vazios_professor == true)
		out << "minimizar_horarios_vazios_professor" << endl;
	if (param.proibirProfGapMTN > 0)
		out << "proibirProfGapMTN = " << param.proibirProfGapMTN << endl;
	if (param.proibirProfVirtualGapMTN > 0)
		out << "proibirProfVirtualGapMTN = " << param.proibirProfVirtualGapMTN << endl;
	if (param.proibirAlunoGap > 0)
		out << "proibirAlunoGap = " << param.proibirAlunoGap << endl;
	if (param.custo_prof_disponibilidade == true)
		out << "custo_prof_disponibilidade" << endl;
	if (param.evitar_reducao_carga_horaria_prof == true)
		out << "evitar_reducao_carga_horaria_prof" << endl;
	if (param.evitar_prof_ultimo_primeiro_hr == true)
		out << "evitar_prof_ultimo_primeiro_hr" << endl;
	if (param.min_alunos_abertura_turmas == true)
		out << "min_alunos_abertura_turmas_value = " << param.min_alunos_abertura_turmas_value << endl;
	if (param.min_alunos_abertura_turmas_praticas == true)
		out << "min_alunos_abertura_turmas_value" << param.min_alunos_abertura_turmas_value << endl;
	if (param.min_alunos_abertura_turmas_praticas_value > 1)
		out << "min_alunos_abertura_turmas_praticas_value" << endl;
	if (param.violar_min_alunos_turmas_formandos == true)
		out << "violar_min_alunos_turmas_formandos" << endl;
	if (param.considerar_equivalencia == true)
		out << "considerar_equivalencia" << endl;
	if (param.permite_compartilhamento_turma_sel == true)
		out << "permite_compartilhamento_turma_sel" << endl;
	if (param.permitir_alunos_em_varios_campi == true)
		out << "permitir_alunos_em_varios_campi" << endl;
	if (param.min_mestres == true)
		out << "min_mestres" << endl;
	if (param.min_doutores == true)
		out << "min_doutores" << endl;
	if (param.considerar_preferencia_prof == true)
		out << "considerar_preferencia_prof";
	if (param.considerar_desempenho_prof == true)
		out << "considerar_desempenho_prof" << endl;
	if (param.utilizarDemandasP2 == true)
		out << "utilizarDemandasP2" << endl;
	if (param.considerar_equivalencia_por_aluno == true)
		out << "considerar_equivalencia_por_aluno" << endl;
	if (param.considerar_disponibilidade_prof_em_tatico == true)
		out << "considerar_disponibilidade_prof_em_tatico" << endl;
	if (param.considerar_disciplinas_incompativeis_no_dia == true)
		out << "considerar_disciplinas_incompativeis_no_dia" << endl;
	if (param.discPratTeor1x1 == true)
		out << "discPratTeor1x1" << endl;
	if (param.discPratTeor1xN == true)
		out << "discPratTeor1xN" << endl;
	if (param.discPratTeorMxN == true)
		out << "discPratTeorMxN" << endl;
	if (param.discPratTeor1xN_antigo == true)
		out << "discPratTeor1xN_antigo" << endl;
	if (param.considerarMaxDiasPorProf == true)
		out << "considerarMaxDiasPorProf" << endl;
	if (param.minCredsDiariosPorProf == true)
		out << "minCredsDiariosPorProf" << endl;
	if (param.considerarMinPercAtendAluno == true)
		out << "considerarMinPercAtendAluno: " << param.minAtendPercPorAluno << endl;
	if (param.min_folga_ocupacao_sala == true)
		out << "min_folga_ocupacao_sala" << endl;
	if (param.limitar_unidades_por_periodo == true)
		out << "limitar_unidades_por_periodo" << endl;
	if (param.limitar_salas_por_periodo == true)
		out << "limitar_salas_por_periodo" << endl;
	if (param.considerarDescansoMinProf == true)
		out << "considerarDescansoMinProf = " << param.descansoMinProfValue << endl;
	if (param.priorizarCalouros == true)
		out << "priorizarCalouros" << endl;
	if (param.priorizarFormandos == true)
		out << "priorizarFormandos" << endl;
	if (param.minContratoIntegral == true)
		out << "minContratoIntegral" << endl;
	if (param.minContratoParcial == true)
		out << "minContratoParcial" << endl;

	return out;
}

void ParametrosPlanejamento::le_arvore(ItemParametrosPlanejamento& elem)
{
	modo_otimizacao = elem.modoOtimizacao();

	otimizarPor = elem.otimizarPor();

	ITERA_SEQ(it_niveis_dificuldade_horario,
		elem.niveisDificuldadeHorario(),
		NivelDificuldadeHorario)
	{
		NivelDificuldadeHorario * nivel_dif_hor = new NivelDificuldadeHorario();
		nivel_dif_hor->le_arvore(*it_niveis_dificuldade_horario);
		niveis_dificuldade_horario.add(nivel_dif_hor);
	}

	equilibrar_diversidade_disc_dia = elem.equilibrarDiversidadeDiscDia();
	minimizar_desloc_prof = elem.minimizarDeslocProfessor();
	minimizar_desloc_aluno = elem.minimizarDeslocAluno();

	// Se a tag existir (mesmo que esteja em branco) no xml de entrada
	if (elem.maxDeslocProfessor().present())
	{
		maxDeslocProf = elem.maxDeslocProfessor().get();
	}

	// Se a tag existir (mesmo que esteja em branco) no xml de entrada
	if (elem.maximizarAvaliacaoCursos().present())
	{
		ITERA_NSEQ(it_maximizar_avaliacao_cursos,
			elem.maximizarAvaliacaoCursos().get(),
			id, Identificador)
		{
			maximizar_avaliacao_cursos.add(*it_maximizar_avaliacao_cursos);
		}
	}

	// Se a tag existir (mesmo que esteja em branco) no xml de entrada
	if (elem.minimizarCustoDocenteCursos().present())
	{
		ITERA_NSEQ(it_minimizar_custo_docente_cursos,
			elem.minimizarCustoDocenteCursos().get(),
			id, Identificador)
		{
			minimizar_custo_docente_cursos.add(*it_minimizar_custo_docente_cursos);
		}
	}

	// Se a tag existir (mesmo que esteja em branco) no xml de entrada
	if (elem.permiteCompartilhamentoTurma().present())
	{
		xsd::cxx::tree::sequence< GrupoIdentificador >::iterator it =
			elem.permiteCompartilhamentoTurma().get().GrupoIdentificador().begin();
		for (; it != elem.permiteCompartilhamentoTurma().get().GrupoIdentificador().end(); ++it)
		{
			GGroup< int > * g = new GGroup < int > ;
			ITERA_NSEQ(it_id, *it, id, Identificador)
			{
				g->add(*it_id);
			}

			nao_permite_compart_turma.add(g);
		}
	}

	// ALUNO
	if (elem.cargaHorariaSemanalAluno().equilibrar())
	{
		carga_horaria_semanal_aluno = EQUILIBRAR;
	}
	else if (elem.cargaHorariaSemanalAluno().minimizarDias())
	{
		carga_horaria_semanal_aluno = MINIMIZAR_DIAS;
	}
	else if (elem.cargaHorariaSemanalAluno().indiferente())
	{
		carga_horaria_semanal_aluno = INDIFERENTE;
	}

	// PROFESSOR
	if (elem.cargaHorariaSemanalProfessor().equilibrar())
	{
		carga_horaria_semanal_prof = EQUILIBRAR;
	}
	else if (elem.cargaHorariaSemanalProfessor().minimizarDias())
	{
		carga_horaria_semanal_prof = MINIMIZAR_DIAS;
	}
	else if (elem.cargaHorariaSemanalProfessor().indiferente())
	{
		carga_horaria_semanal_prof = INDIFERENTE;
	}

	// ------------------------------------------------------------------------
	// Gap nos horararios dos professores
	minimizar_horarios_vazios_professor = elem.minimizarHorariosVaziosProfessor();
	proibirProfGapMTN = (int)elem.proibirHorariosVaziosProfessor();

	if (!proibirProfGapMTN)
	{
		if (minimizar_horarios_vazios_professor) proibirProfGapMTN = ConstraintLevel::Weak;
		else proibirProfGapMTN = ConstraintLevel::Off;
	}
	else proibirProfGapMTN = ConstraintLevel::Strong;

	proibirProfVirtualGapMTN = ConstraintLevel::Off;
	//if ( proibirProfGapMTN ) proibirProfVirtualGapMTN = proibirProfGapMTN;

	// ------------------------------------------------------------------------
	// Gap nos horararios dos alunos
	proibirAlunoGap = ConstraintLevel::Off;	//elem.proibirHorariosVaziosAluno();

	// ------------------------------------------------------------------------


	custo_prof_disponibilidade = elem.custoProfDisponibilidade();
	evitar_reducao_carga_horaria_prof = elem.evitarReducaoCargaHorariaProf();
	evitar_prof_ultimo_primeiro_hr = false; //elem.evitarProfUltimoPrimeiroHor(); 	// N�O USADA MAIS. SUBSTITUTA: considerarDescansoMinProf

	funcao_objetivo = elem.funcaoObjetivo();
	considerar_equivalencia = elem.considerarEquivalencia();
	permite_compartilhamento_turma_sel = elem.permiteCompartilhamentoTurmaSel();
	permitir_alunos_em_varios_campi = elem.permitirAlunosEmVariosCampi();
	min_doutores = elem.percentuaisMinimoDoutores();
	min_mestres = elem.percentuaisMinimoMestres();
	considerar_preferencia_prof = elem.preferenciaProfessorDisciplina();
	considerar_desempenho_prof = elem.desempenhoProfDisponibilidade();
	utilizarDemandasP2 = elem.utilizarDemandasP2();
	maximoDisciplinasDeUmProfessorPorCurso = elem.maximoDisciplinasDeUmProfessorPorCurso();

	perc_max_reducao_CHP = elem.evitarReducaoCargaHorariaProfValor().get();

	// -----------------------------------------------------
	// Minimo de alunos por turma
	min_alunos_abertura_turmas = elem.minAlunosAberturaTurmas();
	min_alunos_abertura_turmas_value = elem.minAlunosAberturaTurmasValor().get();
	if (min_alunos_abertura_turmas_value <= 1) min_alunos_abertura_turmas = false;

	min_alunos_abertura_turmas_praticas = min_alunos_abertura_turmas; //elem.minAlunosAberturaTurmasPraticas();
	min_alunos_abertura_turmas_praticas_value = 5; //elem.minAlunosAberturaTurmasPraticasValor().get();

	if (min_alunos_abertura_turmas_praticas_value)
		cout << "\nHARD-CODED PARA MINIMO DE " << min_alunos_abertura_turmas_praticas_value << " ALUNOS EM LABORATORIO" << endl;

	violar_min_alunos_turmas_formandos = elem.violarMinAlunosTurmasFormandos();
	// -----------------------------------------------------


	// considerar_equivalencia_por_aluno = false;
	considerar_equivalencia_por_aluno = considerar_equivalencia;
	considerar_equivalencia = false;

	considerar_disponibilidade_prof_em_tatico = true;
	considerar_disciplinas_incompativeis_no_dia = false;

	considerarMinPercAtendAluno = false;
	minAtendPercPorAluno = 0.8;

	min_folga_ocupacao_sala = false;

	limitar_unidades_por_periodo = false;
	limitar_salas_por_periodo = false;

	considerarDescansoMinProf = elem.evitarProfUltimoPrimeiroHor();
	descansoMinProfValue = elem.evitarProfUltimoPrimeiroHorValor();

	priorizarCalouros = elem.priorizarCalouros();
	priorizarFormandos = min_alunos_abertura_turmas && violar_min_alunos_turmas_formandos;

	minContratoParcial = elem.percentuaisMinimoParcialIntegral();
	minContratoIntegral = elem.percentuaisMinimoIntegral();

	minDeslocAlunoEntreUnidadesNoDia = false;

	considerarCoRequisitos = elem.considerarCoRequisitos();
	considerarPrioridadePorAlunos = elem.considerarPrioridadePorAlunos();

	regrasGenericasDivisaoCredito = elem.regrasGenericasDivisaoCredito();
	regrasEspecificasDivisaoCredito = elem.regrasEspecificasDivisaoCredito();

#ifdef UNIRITTER
	discPratTeor1x1 = false;
	discPratTeor1xN = true;
	discPratTeorMxN = false;
	discPratTeor1xN_antigo = false;
	considerarMaxDiasPorProf = true;
	minCredsDiariosPorProf = true;
#elif defined UNIT
	discPratTeor1x1 = false;
	discPratTeor1xN = true;
	discPratTeorMxN = false;
	discPratTeor1xN_antigo = false;
	considerarMaxDiasPorProf = true;
	minCredsDiariosPorProf = true;
#else
	discPratTeor1x1 = false;
	discPratTeor1xN = true;
	discPratTeorMxN = false;
	discPratTeor1xN_antigo = false;
	considerarMaxDiasPorProf = true;
	minCredsDiariosPorProf = true;
#endif

	if (discPratTeor1x1 + discPratTeor1xN + discPratTeorMxN + discPratTeor1xN_antigo != 1)
	{
		cout << "Erro, configuracao errada em relacao entre turmas teorica e pratica!" << endl;
		exit(1);
	}

	cout << *this;
}
