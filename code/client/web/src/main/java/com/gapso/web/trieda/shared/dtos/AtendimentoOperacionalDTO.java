package com.gapso.web.trieda.shared.dtos;

import java.util.HashSet;
import java.util.List;
import java.util.Set;

import com.gapso.web.trieda.shared.util.TriedaUtil;

public class AtendimentoOperacionalDTO extends AbstractAtendimentoRelatorioDTO< String >
	implements Comparable< AtendimentoOperacionalDTO >, AtendimentoRelatorioDTO
{
	private static final long serialVersionUID = -2870302894382757778L;

	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CENARIO_ID = "cenarioId";
	public static final String PROPERTY_CAMPUS_ID = "campusId";
	public static final String PROPERTY_CAMPUS_STRING = "campusString";
	public static final String PROPERTY_UNIDADE_ID = "unidadeId";
	public static final String PROPERTY_UNIDADE_STRING = "unidadeString";
	public static final String PROPERTY_SALA_ID = "salaId";
	public static final String PROPERTY_SALA_STRING = "salaString";
	public static final String PROPERTY_DIA_SEMANA = "semana";
	public static final String PROPERTY_TURNO_ID = "turnoId";
	public static final String PROPERTY_TURNO_STRING = "turnoString";
	public static final String PROPERTY_HORARIO_ID = "horarioID";
	public static final String PROPERTY_HORARIO_DISPONIVEL_CENARIO_ID = "horarioDisponivelCenarioID";
	public static final String PROPERTY_HORARIO_STRING = "horarioString";
	public static final String PROPERTY_PROFESSOR_ID = "professorId";
	public static final String PROPERTY_PROFESSOR_STRING = "professorString";
	public static final String PROPERTY_PROFESSOR_VIRTUAL_ID = "professorVirtualId";
	public static final String PROPERTY_PROFESSOR_VIRTUAL_STRING = "professorVirtualString";
	public static final String PROPERTY_CREDITO_TEORICO_BOOLEAN = "creditoTeoricoBoolean";
	public static final String PROPERTY_CURSO_STRING = "cursoString";
	public static final String PROPERTY_CURSO_NOME = "cursoNome";
	public static final String PROPERTY_CURSO_ID = "cursoId";
	public static final String PROPERTY_CURRICULO_STRING = "curriculoString";
	public static final String PROPERTY_CURRICULO_ID = "curriculoId";
	public static final String PROPERTY_PERIODO = "periodo";
	public static final String PROPERTY_PERIODO_STRING = "periodoString";
	public static final String PROPERTY_DISCIPLINA_ID = "disciplinaId";
	public static final String PROPERTY_DISCIPLINA_STRING = "disciplinaString";
	public static final String PROPERTY_DISCIPLINA_NOME = "disciplinaNome";
	public static final String PROPERTY_TOTAL_CRETIDOS= "totalCreditos";
	public static final String PROPERTY_TOTAL_CRETIDOS_DISCIPLINA = "totalCreditoDisciplina";
	public static final String PROPERTY_OFERTA_ID = "ofertaId";
	public static final String PROPERTY_TURMA = "turma";
	public static final String PROPERTY_QUANTIDADE_ALUNOS = "quantidadeAlunos";
	public static final String PROPERTY_QUANTIDADE_ALUNOS_STRING = "quantidadeAlunosString";
	public static final String PROPERTY_COMPARTILHAMENTO_CURSOS = "compartilhamentoCursos";
	public static final String PROPERTY_SEMANA_LETIVA_ID = "semanaLetivaId";
	public static final String PROPERTY_SEMANA_LETIVA_TEMPO_AULA = "semanaLetivaTempoAula";

	private Integer totalLinhas = 1;

	public AtendimentoOperacionalDTO() { 
		super();
	}

	public AtendimentoOperacionalDTO( AtendimentoOperacionalDTO other )
	{
		this.copy( other );
	}

	private void copy( AtendimentoOperacionalDTO other )
	{
		this.setId( other.getId() );
		this.setVersion( other.getVersion() );
		this.setCenarioId( other.getCenarioId() );
		this.setCampusId( other.getCampusId() );
		this.setCampusString( other.getCampusString() );
		this.setUnidadeId( other.getUnidadeId() );
		this.setUnidadeString( other.getUnidadeString() );
		this.setSalaId( other.getSalaId() );
		this.setSalaString( other.getSalaString() );
		this.setTurnoId( other.getTurnoId() );
		this.setTurnoString( other.getTurnoString() );
		this.setHorarioId( other.getHorarioId() );
		this.setHorarioString( other.getHorarioString() );
		this.setProfessorId( other.getProfessorId() );
		this.setProfessorString( other.getProfessorString() );
		this.setProfessorVirtualId( other.getProfessorVirtualId() );
		this.setProfessorVirtualString( other.getProfessorVirtualString() );
		this.setCreditoTeoricoBoolean( other.getCreditoTeoricoBoolean() );
		this.setSemana( other.getSemana() );
		this.setCursoString( other.getCursoString() );
		this.setCursoNome( other.getCursoNome() );
		this.setCursoId( other.getCursoId() );
		this.setCurricularId( other.getCurriculoId() );
		this.setCurricularString( other.getCurriculoString() );
		this.setPeriodo( other.getPeriodo() );
		this.setPeriodoString( other.getPeriodoString() );
		this.setDisciplinaId( other.getDisciplinaId() );
		this.setDisciplinaString( other.getDisciplinaString() );
		this.setDisciplinaNome( other.getDisciplinaNome() );
		this.setTotalCreditos( other.getTotalCreditos() );
		this.setTotalCreditoDisciplina( other.getTotalCreditoDisciplina() );
		this.setOfertaId( other.getOfertaId() );
		this.setTurma( other.getTurma() );
		this.setQuantidadeAlunos( other.getQuantidadeAlunos() );
		this.setQuantidadeAlunosString( other.getQuantidadeAlunosString() );
		this.setCompartilhamentoCursosString( other.getCompartilhamentoCursosString() );
		this.setTotalLinhas( other.getTotalLinhas() );
		this.setSemanaLetivaId( other.getSemanaLetivaId() );
	}

	public void setId( Long value )
	{
		set( PROPERTY_ID, value );
	}

	public Long getId()
	{
		return get( PROPERTY_ID );
	}

	public void setVersion( Integer value )
	{
		set( PROPERTY_VERSION, value );
	}

	public Integer getVersion()
	{
		return get( PROPERTY_VERSION );
	}

	public void setCenarioId( Long value )
	{
		set( PROPERTY_CENARIO_ID, value );
	}

	public Long getCenarioId()
	{
		return get( PROPERTY_CENARIO_ID );
	}

	public void setCampusId( Long value )
	{
		set( PROPERTY_CAMPUS_ID, value );
	}

	public Long getCampusId()
	{
		return get( PROPERTY_CAMPUS_ID );
	}

	public String getCampusString()
	{
		return get( PROPERTY_CAMPUS_STRING );
	}

	public void setCampusString( String value )
	{
		set( PROPERTY_CAMPUS_STRING, value );
	}

	public void setUnidadeId( Long value )
	{
		set( PROPERTY_UNIDADE_ID, value );
	}

	public Long getUnidadeId()
	{
		return get( PROPERTY_UNIDADE_ID );
	}

	public String getUnidadeString()
	{
		return get( PROPERTY_UNIDADE_STRING );
	}

	public void setUnidadeString( String value )
	{
		set( PROPERTY_UNIDADE_STRING, value );
	}

	public void setSalaId( Long value )
	{
		set( PROPERTY_SALA_ID, value );
	}

	public Long getSalaId()
	{
		return get( PROPERTY_SALA_ID );
	}

	public String getSalaString()
	{
		return get( PROPERTY_SALA_STRING );
	}

	public void setSalaString( String value )
	{
		set( PROPERTY_SALA_STRING, value );
	}

	public void setTurnoId( Long value )
	{
		set( PROPERTY_TURNO_ID, value );
	}

	public Long getTurnoId()
	{
		return get( PROPERTY_TURNO_ID );
	}

	public void setTurnoString( String value )
	{
		set( PROPERTY_TURNO_STRING, value );
	}

	public String getTurnoString()
	{
		return get( PROPERTY_TURNO_STRING );
	}

	public void setHorarioId( Long value )
	{
		set( PROPERTY_HORARIO_ID, value );
	}

	public Long getHorarioId()
	{
		return get( PROPERTY_HORARIO_ID );
	}

	public void setHorarioDisponivelCenarioId( Long value )
	{
		set( PROPERTY_HORARIO_DISPONIVEL_CENARIO_ID, value );
	}

	public Long getHorarioDisponivelCenarioId()
	{
		return get( PROPERTY_HORARIO_DISPONIVEL_CENARIO_ID );
	}

	public void setHorarioString( String value )
	{
		set( PROPERTY_HORARIO_STRING, value );
	}

	public String getHorarioString()
	{
		return get( PROPERTY_HORARIO_STRING );
	}

	public void setProfessorId( Long value )
	{
		set( PROPERTY_PROFESSOR_ID, value );
	}

	public Long getProfessorId()
	{
		return get( PROPERTY_PROFESSOR_ID );
	}

	public void setProfessorString( String value )
	{
		set( PROPERTY_PROFESSOR_STRING, value );
	}

	public String getProfessorString()
	{
		return get( PROPERTY_PROFESSOR_STRING );
	}

	public void setProfessorVirtualId( Long value )
	{
		set( PROPERTY_PROFESSOR_VIRTUAL_ID, value );
	}

	public Long getProfessorVirtualId()
	{
		return get( PROPERTY_PROFESSOR_VIRTUAL_ID );
	}

	public void setProfessorVirtualString( String value )
	{
		set( PROPERTY_PROFESSOR_VIRTUAL_STRING, value );
	}

	public String getProfessorVirtualString()
	{
		return get( PROPERTY_PROFESSOR_VIRTUAL_STRING );
	}

	public void setCreditoTeoricoBoolean( Boolean value )
	{
		set( PROPERTY_CREDITO_TEORICO_BOOLEAN, value );
	}

	public Boolean getCreditoTeoricoBoolean()
	{
		return get( PROPERTY_CREDITO_TEORICO_BOOLEAN );
	}

	public void setSemana( Integer value )
	{
		set( PROPERTY_DIA_SEMANA, value );
	}

	public Integer getSemana()
	{
		return get( PROPERTY_DIA_SEMANA );
	}

	public void setCursoString( String value )
	{
		set( PROPERTY_CURSO_STRING, value );
	}

	public String getCursoString()
	{
		return get( PROPERTY_CURSO_STRING );
	}

	public void setCursoNome( String value )
	{
		set( PROPERTY_CURSO_NOME, value );
	}

	public String getCursoNome()
	{
		return get( PROPERTY_CURSO_NOME );
	}

	public void setCursoId( Long value )
	{
		set( PROPERTY_CURSO_ID, value );
	}

	public Long getCursoId()
	{
		return get( PROPERTY_CURSO_ID );
	}

	public void setCurricularId( Long value )
	{
		set( PROPERTY_CURRICULO_ID, value );
	}

	public Long getCurriculoId()
	{
		return get( PROPERTY_CURRICULO_ID );
	}

	public void setCurricularString( String value )
	{
		set( PROPERTY_CURRICULO_STRING, value );
	}

	public String getCurriculoString()
	{
		return get( PROPERTY_CURRICULO_STRING );
	}

	public void setPeriodo( Integer value )
	{
		set( PROPERTY_PERIODO, value );
	}

	public Integer getPeriodo()
	{
		return get( PROPERTY_PERIODO );
	}

	public void setPeriodoString( String value )
	{
		set( PROPERTY_PERIODO_STRING, value );
	}

	public String getPeriodoString()
	{
		return get( PROPERTY_PERIODO_STRING );
	}

	public void setDisciplinaId( Long value )
	{
		set( PROPERTY_DISCIPLINA_ID, value );
	}

	public Long getDisciplinaId()
	{
		return get( PROPERTY_DISCIPLINA_ID );
	}

	public String getDisciplinaString()
	{
		return get( PROPERTY_DISCIPLINA_STRING );
	}

	public void setDisciplinaString( String value )
	{
		set( PROPERTY_DISCIPLINA_STRING, value );
	}

	public String getDisciplinaNome()
	{
		return get( PROPERTY_DISCIPLINA_NOME );
	}

	public void setDisciplinaNome( String value )
	{
		set( PROPERTY_DISCIPLINA_NOME, value );
	}

	public void setTotalCreditos( Integer value )
	{
		set( PROPERTY_TOTAL_CRETIDOS, value );
	}

	public Integer getTotalCreditos()
	{
		return get( PROPERTY_TOTAL_CRETIDOS );
	}

	public void setTotalCreditoDisciplina( Integer value )
	{
		set( PROPERTY_TOTAL_CRETIDOS_DISCIPLINA, value );
	}

	@Override
	public Integer getTotalCreditoDisciplina()
	{
		return get(PROPERTY_TOTAL_CRETIDOS_DISCIPLINA);
	}	

	public void setOfertaId( Long value )
	{
		set( PROPERTY_OFERTA_ID, value );
	}

	public Long getOfertaId()
	{
		return get( PROPERTY_OFERTA_ID );
	}

	public String getTurma()
	{
		return get( PROPERTY_TURMA );
	}

	public void setTurma( String value )
	{
		set( PROPERTY_TURMA, value );
	}

	public void setQuantidadeAlunos( Integer value )
	{
		set( PROPERTY_QUANTIDADE_ALUNOS, value );
	}

	public Integer getQuantidadeAlunos()
	{
		return get( PROPERTY_QUANTIDADE_ALUNOS );
	}

	public void setQuantidadeAlunosString( String value )
	{
		set( PROPERTY_QUANTIDADE_ALUNOS_STRING, value );
	}

	public String getQuantidadeAlunosString()
	{
		return get( PROPERTY_QUANTIDADE_ALUNOS_STRING );
	}

	@Override
	public String getCompartilhamentoCursosString()
	{
		return get( PROPERTY_COMPARTILHAMENTO_CURSOS );
	}

	@Override
	public void setCompartilhamentoCursosString( String s )
	{
		set( PROPERTY_COMPARTILHAMENTO_CURSOS, s );
	}

	public Integer getTotalLinhas()
	{
		return totalLinhas;
	}

	public void setTotalLinhas( Integer value )
	{
		this.totalLinhas = value;
	}

	private Set< Long > idsCursosAdicionadosVisaoProfessor = new HashSet< Long >();
	private Set< Long > idsCurriculosAdicionadosVisaoProfessor = new HashSet< Long >();
	private Set< Integer > idsPeriodosAdicionadosVisaoProfessor = new HashSet< Integer >();

	public void concatenateVisaoProfessor( AtendimentoOperacionalDTO other )
	{
		idsCursosAdicionadosVisaoProfessor.add( this.getCursoId() );
		idsCurriculosAdicionadosVisaoProfessor.add( this.getCurriculoId() );
		idsPeriodosAdicionadosVisaoProfessor.add( this.getPeriodo() );

		if ( !idsCursosAdicionadosVisaoProfessor.contains( other.getCursoId() ) )
		{
			setCursoNome( getCursoNome() + " / " + other.getCursoNome() );
			idsCursosAdicionadosVisaoProfessor.add( other.getCursoId() );
		}

		if ( !idsCurriculosAdicionadosVisaoProfessor.contains( other.getCurriculoId() ) )
		{
			setCurricularString( getCurriculoString() + " / " + other.getCurriculoString() );
			idsCurriculosAdicionadosVisaoProfessor.add( other.getCurriculoId() );
		}

		if ( !idsPeriodosAdicionadosVisaoProfessor.contains( other.getPeriodo() ) )
		{
			setPeriodoString( getPeriodoString() + " / " + other.getPeriodoString() );
			idsPeriodosAdicionadosVisaoProfessor.add( other.getPeriodo() );
		}

		setQuantidadeAlunos( getQuantidadeAlunos() + other.getQuantidadeAlunos() );
		setTotalLinhas( getTotalLinhas() + other.getTotalLinhas() );
	}

	public String getContentToolTipVisaoSala(ReportType reportType) {
		String BG = TriedaUtil.beginBold(reportType);
		String ED = TriedaUtil.endBold(reportType);
		String BR = TriedaUtil.newLine(reportType);
		String professor = isProfessorVirtual() ? getProfessorVirtualString() : getProfessorString();
		
		return BG + "Nome: " + ED + getDisciplinaNome() + BR
		     + BG + "Turma: " + ED + getTurma() + BR
		     + BG + "Professor: " + ED + professor + BR
			 + BG + "Crédito(s) " + ( ( isTeorico() ) ? "Teórico(s)" : "Prático(s)" ) + ": " + ED + getTotalCreditos() + " de " + getTotalCreditoDisciplina() + BR
			 + BG + "Curso(s): " + ED + getCursoNome() + BR
			 + BG + "Matriz(es) Curricular(es): " + ED + getCurriculoString() + BR
			 + BG + "Período(s): " + ED + getPeriodoString() + BR
			 + BG + "Sala: " + ED + getSalaString() + BR
			 + BG + getQuantidadeAlunosString() + " aluno(s)" + ED + BR;
	}

	public String getContentToolTipVisaoCurso(ReportType reportType) {
		String BG = TriedaUtil.beginBold(reportType);
		String ED = TriedaUtil.endBold(reportType);
		String BR = TriedaUtil.newLine(reportType);
		String professor = isProfessorVirtual() ? getProfessorVirtualString() : getProfessorString();
		
		return BG + "Nome: " + ED + getDisciplinaNome() + BR
		     + BG + "Turma: " + ED + getTurma() + BR
		     + BG + "Professor: " + ED + professor + BR
			 + BG + "Crédito(s) " + ( ( isTeorico() ) ? "Teórico(s)" : "Prático(s)" ) + ": " + ED + getTotalCreditos() + " de " + getTotalCreditoDisciplina() + BR
			 + BG + "Curso(s): " + ED + getCursoNome() + BR
			 + BG + "Matriz(es) Curricular(es): " + ED + getCurriculoString() + BR
			 + BG + "Período(s): " + ED + getPeriodoString() + BR
			 + BG + "Sala: " + ED + getSalaString() + BR
			 + BG + getQuantidadeAlunosString() + " aluno(s)" + ED + BR;
	}

	public String getContentVisaoProfessor(ReportType reportType) {
		if (reportType.equals(ReportType.WEB)) {
			final String BR = TriedaUtil.newLine(reportType);
			return getDisciplinaString() + BR
			+ TriedaUtil.truncate( getDisciplinaNome(), 12 ) + BR
			+ "Turma " + getTurma() + BR
			+ getSalaString();
		} else {
			return getDisciplinaString() + " / " + getTurma();
		}
	}

	public String getContentToolTipVisaoProfessor(ReportType reportType) {
		String BG = TriedaUtil.beginBold(reportType);
		String ED = TriedaUtil.endBold(reportType);
		String BR = TriedaUtil.newLine(reportType);
		String professor = isProfessorVirtual() ? getProfessorVirtualString() : getProfessorString();
		
		return BG + "Nome: " + ED + getDisciplinaNome() + BR
		     + BG + "Turma: " + ED + getTurma() + BR
		     + BG + "Professor: " + ED + professor + BR
			 + BG + "Crédito(s) " + ( ( isTeorico() ) ? "Teórico(s)" : "Prático(s)" ) + ": " + ED + getTotalCreditos() + " de " + getTotalCreditoDisciplina() + BR
			 + BG + "Curso(s): " + ED + getCursoNome() + BR
			 + BG + "Matriz(es) Curricular(es): " + ED + getCurriculoString() + BR
			 + BG + "Período(s): " + ED + getPeriodoString() + BR
			 + BG + "Sala: " + ED + getSalaString() + BR
			 + BG + getQuantidadeAlunosString() + " aluno(s)" + ED + BR;
	}

	@Override
	public String getNaturalKey()
	{
		return getCampusString() + "-" + getUnidadeString()
			+ "-" + getSalaString() + "-" + getSemana()
			+ "-" + getCursoString() + "-" + getCurriculoString()
			+ "-" + getPeriodo() + "-" + getDisciplinaString()
			+ "-" + getTurma() + "-" + getHorarioString();
	}

	@Override
	public int compareTo( AtendimentoOperacionalDTO o )
	{
		return this.getNaturalKey().compareTo( o.getNaturalKey() );
	}

	@Override
	public String toString()
	{
		return getDisciplinaString() + "@" + getTurma()
			+ "@" + getSalaString() + "@" + getSemana()
			+ "#" + getTotalCreditos() + "#" + getHorarioId();
	}

	static public boolean compatibleByApproach1(
		AtendimentoOperacionalDTO dto1, AtendimentoOperacionalDTO dto2 )
	{
		return dto1.getDisciplinaId().equals( dto2.getDisciplinaId() )
			&& !dto1.getSalaId().equals( dto2.getSalaId() )
			&& !dto1.getTurma().equals( dto2.getTurma() )
			&& dto1.getTotalLinhas().equals( dto2.getTotalLinhas() )
			&& dto1.getSemana().equals( dto2.getSemana() );
	}

	static public boolean compatibleByApproach2(
		AtendimentoOperacionalDTO dto1, AtendimentoOperacionalDTO dto2 )
	{
		return !dto1.getDisciplinaId().equals( dto2.getDisciplinaId() )
			&& !dto1.getSalaId().equals( dto2.getSalaId() )
			&& !dto1.getTurma().equals( dto2.getTurma() )
			&& dto1.getTotalLinhas().equals( dto2.getTotalLinhas() )
			&& dto1.getSemana().equals( dto2.getSemana() );
	}

	static public int countListDTOsCreditos(
		List< AtendimentoOperacionalDTO > listDTOs )
	{
		int count = 0;

		for ( AtendimentoOperacionalDTO dto : listDTOs )
		{
			count += dto.getTotalCreditos();
		}

		return count;
	}

	@Override
	public boolean isTeorico()
	{
		return this.getCreditoTeoricoBoolean();
	}

	@Override
	public boolean isTatico()
	{
		return false;
	}

	@Override
	public boolean isProfessorVirtual()
	{
		return !TriedaUtil.isBlank( getProfessorVirtualId() );
	}

	@Override
	public Long getSemanaLetivaId()
	{
		return get( PROPERTY_SEMANA_LETIVA_ID );
	}

	public void setSemanaLetivaId( Long value )
	{
		set( PROPERTY_SEMANA_LETIVA_ID, value );
	}
	
	@Override
	public Integer getSemanaLetivaTempoAula() {
		return get( PROPERTY_SEMANA_LETIVA_TEMPO_AULA );
	}

	public void setSemanaLetivaTempoAula( Integer value ) {
		set( PROPERTY_SEMANA_LETIVA_TEMPO_AULA, value );
	}
}
