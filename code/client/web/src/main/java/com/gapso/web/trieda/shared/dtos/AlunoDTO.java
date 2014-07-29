package com.gapso.web.trieda.shared.dtos;

public class AlunoDTO
	extends AbstractDTO< String >
	implements Comparable< AlunoDTO >
{
	private static final long serialVersionUID = -5134820110949139907L;

	// Propriedades
	public static final String PROPERTY_ALUNO_ID = "alunoId";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_ALUNO_NOME = "nome";
	public static final String PROPERTY_ALUNO_MATRICULA = "matricula";
	public static final String PROPERTY_ALUNO_FORMANDO = "formando";
	public static final String PROPERTY_ALUNO_PERIODO = "periodo";
	public static final String PROPERTY_ALUNO_PRIORIDADE = "prioridade";
	public static final String PROPERTY_ALUNO_VIRTUAL = "virtual";
	public static final String PROPERTY_ALUNO_CRIADO_TRIEDA = "criadoTrieda";
	public static final String PROPERTY_CENARIO_ID = "cenarioId";

	public AlunoDTO()
	{
		super();
	}

	public void setId( Long value )
	{
		set( PROPERTY_ALUNO_ID, value );
	}

	public Long getId()
	{
		return get( PROPERTY_ALUNO_ID );
	}

	public void setCenarioId( Long value )
	{
		set( PROPERTY_CENARIO_ID, value );
	}

	public Long getCenarioId()
	{
		return get( PROPERTY_CENARIO_ID );
	}

	public void setVersion( Integer value )
	{
		set( PROPERTY_VERSION, value );
	}

	public Integer getVersion()
	{
		return get( PROPERTY_VERSION );
	}

	public void setNome( String value )
	{
		set( PROPERTY_ALUNO_NOME, value );
	}

	public String getNome()
	{
		return get( PROPERTY_ALUNO_NOME );
	}

	public void setMatricula( String value )
	{
		set( PROPERTY_ALUNO_MATRICULA, value );
	}

	public String getMatricula()
	{
		return get( PROPERTY_ALUNO_MATRICULA );
	}
	
	public void setFormando( Boolean value )
	{
		set( PROPERTY_ALUNO_FORMANDO, value );
	}

	public Boolean getFormando()
	{
		return get( PROPERTY_ALUNO_FORMANDO );
	}
	
	public void setPeriodo( Integer value )
	{
		set( PROPERTY_ALUNO_PERIODO, value );
	}

	public Integer getPrioridade()
	{
		return get( PROPERTY_ALUNO_PRIORIDADE );
	}
	
	public void setPrioridade( Integer value )
	{
		set( PROPERTY_ALUNO_PRIORIDADE, value );
	}

	public Integer getPeriodo()
	{
		return get( PROPERTY_ALUNO_PERIODO );
	}

	@Override
	public String getNaturalKey()
	{
		return getInstituicaoEnsinoId() + "-" + getMatricula();
	}
	
	public void setVirtual( Boolean value )
	{
		set( PROPERTY_ALUNO_VIRTUAL, value );
	}

	public Boolean getVirtual()
	{
		return get( PROPERTY_ALUNO_VIRTUAL );
	}
	
	public void setCriadoTrieda( Boolean value )
	{
		set( PROPERTY_ALUNO_CRIADO_TRIEDA, value );
	}

	public Boolean getCriadoTrieda()
	{
		return get( PROPERTY_ALUNO_CRIADO_TRIEDA );
	}

	@Override
	public int compareTo( AlunoDTO o )
	{
		int result = this.getInstituicaoEnsinoId().compareTo(
			o.getInstituicaoEnsinoId() );

		if ( result == 0 )
		{
			result = this.getMatricula().compareTo( o.getMatricula() );
		}

		return result;
	}
}
