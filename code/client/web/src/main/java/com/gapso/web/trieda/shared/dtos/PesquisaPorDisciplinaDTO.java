package com.gapso.web.trieda.shared.dtos;


public class PesquisaPorDisciplinaDTO extends AbstractTreeDTO< String >
	implements Comparable< PesquisaPorDisciplinaDTO >
{
	private static final long serialVersionUID = -5134820110949139907L;

	// Propriedades
	public static final String PROPERTY_TURMA_STRING = "turmaString";
	public static final String PROPERTY_DIA_SEMANA_STRING = "diaSemanaString";
	public static final String PROPERTY_HORARIO_ID = "horarioId";
	public static final String PROPERTY_HORARIO_INICIAL = "horarioInicial";
	public static final String PROPERTY_HORARIO_FINAL = "horarioFinal";
	public static final String PROPERTY_QUANTIDADE_ALUNOS_INT = "quantidadeAlunosInt";
	public static final String PROPERTY_SALA_ID = "salaId";
	public static final String PROPERTY_SALA_STRING = "salaString";
	public static final String PROPERTY_PROFESSOR_STRING = "professorString";
	public static final String PROPERTY_CURSO_ID = "CursoId";

	public PesquisaPorDisciplinaDTO()
	{
		super();
	}


	public void setTurma( String value )
	{
		set( PROPERTY_TURMA_STRING, value );
	}

	public String getTurma()
	{
		return get( PROPERTY_TURMA_STRING );
	}



	public void setQuantidadeAlunos( Integer value )
	{
		set( PROPERTY_QUANTIDADE_ALUNOS_INT, value );
	}

	public Integer getQuantidadeAlunos()
	{
		return get( PROPERTY_QUANTIDADE_ALUNOS_INT );
	}
	
	public void setDiaSemanaString(String value) {
		set(PROPERTY_DIA_SEMANA_STRING, value);
	}

	public String getDiaSemanaString() {
		return get(PROPERTY_DIA_SEMANA_STRING);
	}
	
	public void setHorarioInicial(String value) {
		set(PROPERTY_HORARIO_INICIAL, value);
	}

	public String getHorarioInicial() {
		return get(PROPERTY_HORARIO_INICIAL);
	}
	
	
	public void setHorarioFinal(String value) {
		set(PROPERTY_HORARIO_FINAL, value);
	}

	public String getHorarioFinal() {
		return get(PROPERTY_HORARIO_FINAL);
	}
	
	
	public void setHorarioId(Long value) {
		set(PROPERTY_HORARIO_ID, value);
	}

	public Long getHorarioId() {
		return get(PROPERTY_HORARIO_ID);
	}
	
	public void setSalaId(Long value) {
		set(PROPERTY_SALA_ID, value);
	}

	public Long getSalaId() {
		return get(PROPERTY_SALA_ID);
	}
	
	public void setSalaString(String value) {
		set(PROPERTY_SALA_STRING, value);
	}

	public String getSalaString() {
		return get(PROPERTY_SALA_STRING);
	}
	
	public void setProfessorString(String value) {
		set(PROPERTY_PROFESSOR_STRING, value);
	}

	public String getProfessorString() {
		return get(PROPERTY_PROFESSOR_STRING);
	}
	
	public Long getCursoId()
	{
		return get( PROPERTY_CURSO_ID );
	}
	
	public void setCursoId( Long value )
	{
		set(PROPERTY_CURSO_ID, value);
	}
	

	@Override
	public String getNaturalKey()
	{
		return getSalaId() +
			"-" + getTurma() +
			"-" + getCursoId();
	}

	@Override
	public int compareTo( PesquisaPorDisciplinaDTO o )
	{
		return  getNaturalKey().compareTo(o.getNaturalKey());
	}
}
