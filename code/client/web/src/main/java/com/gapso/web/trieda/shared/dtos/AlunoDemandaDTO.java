package com.gapso.web.trieda.shared.dtos;

public class AlunoDemandaDTO extends AbstractDTO< String >
	implements Comparable< AlunoDemandaDTO >
{
	private static final long serialVersionUID = -5134820110949139907L;

	// Propriedades
	public static final String PROPERTY_CAMPUS_STRING = "campusString";
	public static final String PROPERTY_TURNO_STRING = "turnoString";
	public static final String PROPERTY_CURSO_STRING = "cursoString";
	public static final String PROPERTY_CURRICULO_STRING = "curriculoString";
	public static final String PROPERTY_ALUNO_DEMANDA_ID = "alunoDemandaId";
	public static final String PROPERTY_ALUNO_ID = "alunoId";
	public static final String PROPERTY_ALUNO_STRING = "alunoString";
	public static final String PROPERTY_ALUNO_MATRICULA = "alunoMatricula";
	public static final String PROPERTY_ALUNO_ATENDIDO = "alunoAtendido";
	public static final String PROPERTY_ALUNO_ATENDIDO_STRING = "alunoAtendidoString";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_DEMANDA_ID = "demandaId";
	public static final String PROPERTY_DEMANDA_STRING = "demandaString";
	public static final String PROPERTY_DISCIPLINA_ID = "disciplinaId";
	public static final String PROPERTY_DISCIPLINA_STRING = "disciplinaString";
	public static final String PROPERTY_PERIODO_STRING = "periodoString";
	public static final String PROPERTY_ALUNO_PRIORIDADE = "alunoPrioridade";
	public static final String PROPERTY_MOTIVO_NAO_ATENDIMENTO = "motivoNaoAtendimento";

	public AlunoDemandaDTO()
	{
		super();
	}

	public void setId( Long value )
	{
		set( PROPERTY_ALUNO_DEMANDA_ID, value );
	}

	public Long getId()
	{
		return get( PROPERTY_ALUNO_DEMANDA_ID );
	}

	public void setIdAluno( Long value )
	{
		set( PROPERTY_ALUNO_ID, value );
	}

	public Long getIdAluno()
	{
		return get( PROPERTY_ALUNO_ID );
	}

	public void setVersion( Integer value )
	{
		set( PROPERTY_VERSION, value );
	}

	public Integer getVersion()
	{
		return get( PROPERTY_VERSION );
	}

	public void setAlunoString( String value )
	{
		set( PROPERTY_ALUNO_STRING, value );
	}

	public String getAlunoString()
	{
		return get( PROPERTY_ALUNO_STRING );
	}
	
	public void setAlunoMatricula( String value )
	{
		set( PROPERTY_ALUNO_MATRICULA, value );
	}

	public String getAlunoMatricula()
	{
		return get( PROPERTY_ALUNO_MATRICULA );
	}

	public void setAlunoAtendido( Boolean value )
	{
		set( PROPERTY_ALUNO_ATENDIDO, value );
	}

	public Boolean getAlunoAtendido()
	{
		return get( PROPERTY_ALUNO_ATENDIDO );
	}

	public void setAlunoAtendidoString( String value )
	{
		set( PROPERTY_ALUNO_ATENDIDO_STRING, value );
	}

	public String getAlunoAtendidoString()
	{
		return get( PROPERTY_ALUNO_ATENDIDO_STRING );
	}

	public void setAlunoPrioridade(Integer value){
		set(PROPERTY_ALUNO_PRIORIDADE, value);
	}

	public Integer getAlunoPrioridade(){
		return get(PROPERTY_ALUNO_PRIORIDADE);
	}
	
	public Long getDisciplinaId()
	{
		return get( PROPERTY_DISCIPLINA_ID );
	}

	public void setDisciplinaId( Long value )
	{
		set( PROPERTY_DISCIPLINA_ID, value );
	}
	
	public String getDisciplinaString()
	{
		return get( PROPERTY_DISCIPLINA_STRING );
	}

	public void setDisciplinaString( String value )
	{
		set( PROPERTY_DISCIPLINA_STRING, value );
	}

	public Integer getPeriodo()
	{
		return get( PROPERTY_PERIODO_STRING );
	}

	public void setPeriodo( Integer value )
	{
		set( PROPERTY_PERIODO_STRING, value );
	}

	public Long getDemandaId()
	{
		return get( PROPERTY_DEMANDA_ID );
	}

	public void setDemandaId( Long value )
	{
		set( PROPERTY_DEMANDA_ID, value );
	}
	
	public String getDemandaString()
	{
		return get( PROPERTY_DEMANDA_STRING );
	}

	public void setDemandaString( String value )
	{
		set( PROPERTY_DEMANDA_STRING, value );
	}
	
	public String getCampusString()
	{
		return get( PROPERTY_CAMPUS_STRING );
	}

	public void setCampusString( String value )
	{
		set( PROPERTY_CAMPUS_STRING, value );
	}
	
	public String getTurnoString()
	{
		return get( PROPERTY_TURNO_STRING );
	}

	public void setTurnoString( String value )
	{
		set( PROPERTY_TURNO_STRING, value );
	}
	
	public String getCursoString()
	{
		return get( PROPERTY_CURSO_STRING );
	}

	public void setCursoString( String value )
	{
		set( PROPERTY_CURSO_STRING, value );
	}
	
	public String getCurriculoString()
	{
		return get( PROPERTY_CURRICULO_STRING );
	}

	public void setCurriculoString( String value )
	{
		set( PROPERTY_CURRICULO_STRING, value );
	}
	
	public String getMotivoNaoAtendimento()
	{
		return get( PROPERTY_MOTIVO_NAO_ATENDIMENTO );
	}

	public void setMotivoNaoAtendimento( String value )
	{
		set( PROPERTY_MOTIVO_NAO_ATENDIMENTO, value );
	}

	@Override
	public String getNaturalKey()
	{
		return getIdAluno() + "-" + getDemandaId();
	}

	@Override
	public int compareTo( AlunoDemandaDTO o )
	{
		int result = getDemandaId().compareTo( o.getDemandaId() );

		if ( result == 0 )
		{
			result = getIdAluno().compareTo( o.getIdAluno() );
		}

		return result;
	}
}
