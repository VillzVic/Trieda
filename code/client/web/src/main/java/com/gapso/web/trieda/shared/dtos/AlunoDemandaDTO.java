package com.gapso.web.trieda.shared.dtos;

public class AlunoDemandaDTO extends AbstractDTO< String >
	implements Comparable< AlunoDemandaDTO >
{
	private static final long serialVersionUID = -5134820110949139907L;

	// Propriedades
	public static final String PROPERTY_ALUNO_DEMANDA_ID = "alunoDemandaId";
	public static final String PROPERTY_ALUNO_ID = "alunoId";
	public static final String PROPERTY_ALUNO_STRING = "alunoString";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_DEMANDA_ID = "demandaId";
	public static final String PROPERTY_DEMANDA_STRING = "demandaString";
	public static final String PROPERTY_DISCIPLINA_ID = "disciplinaId";
	public static final String PROPERTY_DISCIPLINA_STRING = "disciplinaString";

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
