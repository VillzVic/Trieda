package com.gapso.web.trieda.shared.dtos;

import java.util.List;

public class ParametroDTO extends AbstractDTO<Long>
	implements Comparable<ParametroDTO>
{
	private static final long serialVersionUID = -5134820110949139907L;

	public static final String TATICO = "TATICO";
	public static final String OPERACIONAL = "OPERACIONAL";

	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CENARIOID = "cenarioid";
	public static final String PROPERTY_MODOOTIMIZACAO = "modootimizacao";
	public static final String PROPERTY_CAMPUS_ID = "campusId";
	public static final String PROPERTY_CAMPUS_DISPLAY = "campusDisplay";
	public static final String PROPERTY_TURNO_ID = "turnoId";
	public static final String PROPERTY_TURNO_DISPLAY = "turnoDisplay";
	public static final String PROPERTY_CARGAHORARIAALUNO = "cargahorariaaluno";
	public static final String PROPERTY_CARGAHORARIAALUNOSEL = "cargahorariaalunosel";
	public static final String PROPERTY_ALUNODEPERIODOMESMASALA = "alunodeperiodomesmasala";
	public static final String PROPERTY_ALUNOEMMUITOSCAMPI = "alunoemmuitoscampi";
	public static final String PROPERTY_MINIMIZARDESLOCAMENTOALUNO = "minimizardeslocamentoaluno";
	public static final String PROPERTY_CARGAHORARIAPROFESSOR = "cargahorariaprofessor";
	public static final String PROPERTY_CARGAHORARIAPROFESSORSEL = "cargahorariaprofessorsel";
	public static final String PROPERTY_PROFESSOREMMUITOSCAMPI = "professoremmuitoscampi";
	public static final String PROPERTY_MINIMIZARDESLOCAMENTOPROFESSOR = "minimizardeslocamentoprofessor";
	public static final String PROPERTY_MINIMIZARDESLOCAMENTOPROFESSORVALUE = "minimizardeslocamentoprofessorvalue";
	public static final String PROPERTY_MINIMIZARGAPPROFESSOR = "minimizargapprofessor";
	public static final String PROPERTY_EVITARREDUCAOCARGAHORARIAPROFESSOR = "evitarreducaocargahorariaprofessor";
	public static final String PROPERTY_EVITARREDUCAOCARGAHORARIAPROFESSORVALUE = "evitarreducaocargahorariaprofessorvalue";
	public static final String PROPERTY_EVITARULTIMOEPRIMEIROHORARIOPROFESSOR = "evitarultimoeprimeirohorarioprofessor";
	public static final String PROPERTY_PREFERENCIADEPROFESSORES = "preferenciadeprofessores";
	public static final String PROPERTY_AVALIACAODESEMPENHOPROFESSOR = "avaliacaodesempenhoprofessor";
	public static final String PROPERTY_NIVELDIFICULDADEDISCIPLINA = "niveldificuldadedisciplina";
	public static final String PROPERTY_COMPATIBILIDADEDISCIPLINASMESMODIA = "compatibilidadedisciplinasmesmodia";
	public static final String PROPERTY_REGRASGENERICASDIVISAOCREDITO = "regrasgenericasdivisaocredito";
	public static final String PROPERTY_REGRASESPECIFICASDIVISAOCREDITO = "regrasespecificasdivisaocredito";
	public static final String PROPERTY_MAXIMIZARNOTAAVALIACAOCORPODOCENTE = "maximizarnotaavaliacaocorpodocente";
	public static final String PROPERTY_MINIMIZARCUSTODOCENTECURSOS = "minimizarcustodocentecursos";
	public static final String PROPERTY_CONSIDERAREQUIVALENCIA = "considerarEquivalencias";
	public static final String PROPERTY_MINALUNOSPARAABRIRTURMA = "minalunosparaabrirturma";
	public static final String PROPERTY_MINALUNOSPARAABRIRTURMAVALUE = "minalunosparaabrirturmavalue";
	public static final String PROPERTY_COMPARTILHARDISCIPLINASCAMPI = "compartilhardisciplinascampi";
	public static final String PROPERTY_PERCENTUAISMINIMOSMESTRES = "percentuaisminimosmestres";
	public static final String PROPERTY_PERCENTUAISMINIMOSDOUTORES = "percentuaisminimosdoutores";
	public static final String PROPERTY_AREATITULACAOPROFESSORESECURSOS = "areatitulacaoprofessoresecursos";
	public static final String PROPERTY_LIMITARMAXIMODISCIPLINAPROFESSOR = "limitarmaximodisciplinaprofessor";
	public static final String PROPERTY_FUNCAOOBJETIVO = "funcaoObjetivo";
	public static final String PROPERTY_INSTITUICAO_ENSINO_ID = "instituicaoEnsinoId";
	public static final String PROPERTY_INSTITUICAO_ENSINO_STRING = "instituicaoEnsinoString";

	public ParametroDTO() {
		super();
	}
	
	public boolean isOperacional() {
		if(getModoOtimizacao() == null) {
			return false;
		}
		return getModoOtimizacao().equals(OPERACIONAL);
	}
	public boolean isTatico() {
		if(getModoOtimizacao() == null) {
			return false;
		}
		return getModoOtimizacao().equals(TATICO);
	}

	public void setId(Long value) {
		set(PROPERTY_ID, value);
	}
	public Long getId() {
		return get(PROPERTY_ID);
	}
	
	public void setVersion(Integer value) {
		set(PROPERTY_VERSION, value);
	}
	public Integer getVersion() {
		return get(PROPERTY_VERSION);
	}

	public Long getCenarioId() {
        return get(PROPERTY_CENARIOID);
    }
	public void setCenarioId(Long value) {
		set(PROPERTY_CENARIOID, value);
    }
	
	public String getModoOtimizacao() {
		return get(PROPERTY_MODOOTIMIZACAO);
	}
	public void setModoOtimizacao(String value) {
		set(PROPERTY_MODOOTIMIZACAO, value);
	}
	
	public Long getCampusId() {
		return get(PROPERTY_CAMPUS_ID);
	}
	public void setCampusId(Long value) {
		set(PROPERTY_CAMPUS_ID, value);
	}
	
	public String getCampusDisplay() {
		return get(PROPERTY_CAMPUS_DISPLAY);
	}
	public void setCampusDisplay(String value) {
		set(PROPERTY_CAMPUS_DISPLAY, value);
	}
	
	public Long getTurnoId() {
		return get(PROPERTY_TURNO_ID);
	}
	public void setTurnoId(Long value) {
		set(PROPERTY_TURNO_ID, value);
	}
	
	public String getTurnoDisplay() {
		return get(PROPERTY_TURNO_DISPLAY);
	}
	public void setTurnoDisplay(String value) {
		set(PROPERTY_TURNO_DISPLAY, value);
	}
	
	public Boolean getCargaHorariaAluno() {
		return get(PROPERTY_CARGAHORARIAALUNO);
	}
	public void setCargaHorariaAluno(Boolean value) {
		set(PROPERTY_CARGAHORARIAALUNO, value);
	}

	public Boolean getAlunoDePeriodoMesmaSala() {
		return get(PROPERTY_ALUNODEPERIODOMESMASALA);
	}
	public void setAlunoDePeriodoMesmaSala(Boolean value) {
		set(PROPERTY_ALUNODEPERIODOMESMASALA, value);
	}

	public Boolean getAlunoEmMuitosCampi() {
		return get(PROPERTY_ALUNOEMMUITOSCAMPI);
	}
	public void setAlunoEmMuitosCampi(Boolean value) {
		set(PROPERTY_ALUNOEMMUITOSCAMPI, value);
	}

	public Boolean getMinimizarDeslocamentoAluno() {
		return get(PROPERTY_MINIMIZARDESLOCAMENTOALUNO);
	}
	public void setMinimizarDeslocamentoAluno(Boolean value) {
		set(PROPERTY_MINIMIZARDESLOCAMENTOALUNO, value);
	}

	public Boolean getCargaHorariaProfessor() {
		return get(PROPERTY_CARGAHORARIAPROFESSOR);
	}
	public void setCargaHorariaProfessor(Boolean value) {
		set(PROPERTY_CARGAHORARIAPROFESSOR, value);
	}

	public Boolean getProfessorEmMuitosCampi() {
		return get(PROPERTY_PROFESSOREMMUITOSCAMPI);
	}
	public void setProfessorEmMuitosCampi(Boolean value) {
		set(PROPERTY_PROFESSOREMMUITOSCAMPI, value);
	}

	public Boolean getMinimizarDeslocamentoProfessor() {
		return get(PROPERTY_MINIMIZARDESLOCAMENTOPROFESSOR);
	}
	public void setMinimizarDeslocamentoProfessor(Boolean value) {
		set(PROPERTY_MINIMIZARDESLOCAMENTOPROFESSOR, value);
	}

	public Boolean getMinimizarGapProfessor() {
		return get(PROPERTY_MINIMIZARGAPPROFESSOR);
	}
	public void setMinimizarGapProfessor(Boolean value) {
		set(PROPERTY_MINIMIZARGAPPROFESSOR, value);
	}

	public Boolean getEvitarReducaoCargaHorariaProfessor() {
		return get(PROPERTY_EVITARREDUCAOCARGAHORARIAPROFESSOR);
	}
	public void setEvitarReducaoCargaHorariaProfessor(Boolean value) {
		set(PROPERTY_EVITARREDUCAOCARGAHORARIAPROFESSOR, value);
	}

	public Boolean getEvitarUltimoEPrimeiroHorarioProfessor() {
		return get(PROPERTY_EVITARULTIMOEPRIMEIROHORARIOPROFESSOR);
	}
	public void setEvitarUltimoEPrimeiroHorarioProfessor(Boolean value) {
		set(PROPERTY_EVITARULTIMOEPRIMEIROHORARIOPROFESSOR, value);
	}

	public Boolean getPreferenciaDeProfessores() {
		return get(PROPERTY_PREFERENCIADEPROFESSORES);
	}
	public void setPreferenciaDeProfessores(Boolean value) {
		set(PROPERTY_PREFERENCIADEPROFESSORES, value);
	}

	public Boolean getAvaliacaoDesempenhoProfessor() {
		return get(PROPERTY_AVALIACAODESEMPENHOPROFESSOR);
	}
	public void setAvaliacaoDesempenhoProfessor(Boolean value) {
		set(PROPERTY_AVALIACAODESEMPENHOPROFESSOR, value);
	}
	
	public Boolean getMinAlunosParaAbrirTurma() {
		return get(PROPERTY_MINALUNOSPARAABRIRTURMA);
	}
	public void setMinAlunosParaAbrirTurma(Boolean value) {
		set(PROPERTY_MINALUNOSPARAABRIRTURMA, value);
	}
	
	public Boolean getConsiderarEquivalencia() {
		return get(PROPERTY_CONSIDERAREQUIVALENCIA);
	}
	public void setConsiderarEquivalencia(Boolean value) {
		set(PROPERTY_CONSIDERAREQUIVALENCIA, value);
	}

	public Integer getMinAlunosParaAbrirTurmaValue() {
		return get(PROPERTY_MINALUNOSPARAABRIRTURMAVALUE);
	}
	public void setMinAlunosParaAbrirTurmaValue(Integer value) {
		set(PROPERTY_MINALUNOSPARAABRIRTURMAVALUE, value);
	}

	public Boolean getNivelDificuldadeDisciplina() {
		return get(PROPERTY_NIVELDIFICULDADEDISCIPLINA);
	}
	public void setNivelDificuldadeDisciplina(Boolean value) {
		set(PROPERTY_NIVELDIFICULDADEDISCIPLINA, value);
	}

	public Boolean getCompatibilidadeDisciplinasMesmoDia() {
		return get(PROPERTY_COMPATIBILIDADEDISCIPLINASMESMODIA);
	}
	public void setCompatibilidadeDisciplinasMesmoDia(Boolean value) {
		set(PROPERTY_COMPATIBILIDADEDISCIPLINASMESMODIA, value);
	}

	public Boolean getRegrasGenericasDivisaoCredito() {
		return get(PROPERTY_REGRASGENERICASDIVISAOCREDITO);
	}
	public void setRegrasGenericasDivisaoCredito(Boolean value) {
		set(PROPERTY_REGRASGENERICASDIVISAOCREDITO, value);
	}

	public Boolean getRegrasEspecificasDivisaoCredito() {
		return get(PROPERTY_REGRASESPECIFICASDIVISAOCREDITO);
	}
	public void setRegrasEspecificasDivisaoCredito(Boolean value) {
		set(PROPERTY_REGRASESPECIFICASDIVISAOCREDITO, value);
	}

	public Boolean getMaximizarNotaAvaliacaoCorpoDocente() {
		return get(PROPERTY_MAXIMIZARNOTAAVALIACAOCORPODOCENTE);
	}
	public void setMaximizarNotaAvaliacaoCorpoDocente(Boolean value) {
		set(PROPERTY_MAXIMIZARNOTAAVALIACAOCORPODOCENTE, value);
	}

	public Boolean getMinimizarCustoDocenteCursos() {
		return get(PROPERTY_MINIMIZARCUSTODOCENTECURSOS);
	}
	public void setMinimizarCustoDocenteCursos(Boolean value) {
		set(PROPERTY_MINIMIZARCUSTODOCENTECURSOS, value);
	}

	public Boolean getCompartilharDisciplinasCampi() {
		return get(PROPERTY_COMPARTILHARDISCIPLINASCAMPI);
	}
	public void setCompartilharDisciplinasCampi(Boolean value) {
		set(PROPERTY_COMPARTILHARDISCIPLINASCAMPI, value);
	}

	public Boolean getPercentuaisMinimosMestres() {
		return get(PROPERTY_PERCENTUAISMINIMOSMESTRES);
	}
	public void setPercentuaisMinimosMestres(Boolean value) {
		set(PROPERTY_PERCENTUAISMINIMOSMESTRES, value);
	}

	public Boolean getPercentuaisMinimosDoutores() {
		return get(PROPERTY_PERCENTUAISMINIMOSDOUTORES);
	}
	public void setPercentuaisMinimosDoutores(Boolean value) {
		set(PROPERTY_PERCENTUAISMINIMOSDOUTORES, value);
	}

	public Boolean getAreaTitulacaoProfessoresECursos() {
		return get(PROPERTY_AREATITULACAOPROFESSORESECURSOS);
	}
	public void setAreaTitulacaoProfessoresECursos(Boolean value) {
		set(PROPERTY_AREATITULACAOPROFESSORESECURSOS, value);
	}

	public Boolean getLimitarMaximoDisciplinaProfessor() {
		return get(PROPERTY_LIMITARMAXIMODISCIPLINAPROFESSOR);
	}
	public void setLimitarMaximoDisciplinaProfessor(Boolean value) {
		set(PROPERTY_LIMITARMAXIMODISCIPLINAPROFESSOR, value);
	}

	public String getCargaHorariaAlunoSel() {
		return get(PROPERTY_CARGAHORARIAALUNOSEL);
	}

	public void setCargaHorariaAlunoSel(String value) {
		set(PROPERTY_CARGAHORARIAALUNOSEL, value);
	}

	public String getCargaHorariaProfessorSel() {
		return get(PROPERTY_CARGAHORARIAPROFESSORSEL);
	}
	public void setCargaHorariaProfessorSel(String value) {
		set(PROPERTY_CARGAHORARIAPROFESSORSEL, value);
	}

	public Integer getMinimizarDeslocamentoProfessorValue() {
		return get(PROPERTY_MINIMIZARDESLOCAMENTOPROFESSORVALUE);
	}
	public void setMinimizarDeslocamentoProfessorValue(Integer value) {
		set(PROPERTY_MINIMIZARDESLOCAMENTOPROFESSORVALUE, value);
	}

	public Integer getEvitarReducaoCargaHorariaProfessorValue() {
		return get(PROPERTY_EVITARREDUCAOCARGAHORARIAPROFESSORVALUE);
	}
	public void setEvitarReducaoCargaHorariaProfessorValue(Integer value) {
		set(PROPERTY_EVITARREDUCAOCARGAHORARIAPROFESSORVALUE, value);
	}
	
	public Integer getFuncaoObjetivo() {
		return get(PROPERTY_FUNCAOOBJETIVO);
	}
	public void setFuncaoObjetivo(Integer value) {
		set(PROPERTY_FUNCAOOBJETIVO, value);
	}
	
	public List<CursoDTO> limitarmaximodisciplinaprofessor_list;
	public List<CursoDTO> getMaximizarNotaAvaliacaoCorpoDocenteList() {
		return limitarmaximodisciplinaprofessor_list;
	}
	public void setMaximizarNotaAvaliacaoCorpoDocenteList(List<CursoDTO> value) {
		limitarmaximodisciplinaprofessor_list = value;
	}
	
	public List<CursoDTO> minimizarcursodocentecursos_list;
	public List<CursoDTO> getMinimizarCustoDocenteCursosList() {
		return minimizarcursodocentecursos_list;
	}
	public void setMinimizarCustoDocenteCursosList(List<CursoDTO> value) {
		minimizarcursodocentecursos_list = value;
	}
	
	@Override
	public Long getNaturalKey() {
		return getId();
	}
	
	@Override
	public int compareTo(ParametroDTO o) {
		return getId().compareTo(o.getId());
	}
}
