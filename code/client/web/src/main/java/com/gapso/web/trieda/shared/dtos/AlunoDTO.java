package com.gapso.web.trieda.shared.dtos;

public class AlunoDTO
	extends AbstractDTO< String >
	implements Comparable< AlunoDTO >
{
	private static final long serialVersionUID = -5134820110949139907L;

	// Propriedades
	public static final String PROPERTY_ALUNO_ID = "alunoId";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_ALUNO_NOME = "alunoNome";
	public static final String PROPERTY_ALUNO_MATRICULA = "alunoMatricula";
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

	@Override
	public String getNaturalKey()
	{
		return getInstituicaoEnsinoId() + "-" + getMatricula();
	}

	@Override
	public int compareTo( AlunoDTO o )
	{
		int result = this.getInstituicaoEnsinoId().compareTo(
			o.getInstituicaoEnsinoId() );

		if ( result == 0 )
		{
			result = this.getNome().compareTo( o.getNome() );
		}

		return result;
	}
}
