package com.gapso.web.trieda.shared.dtos;

public class TurmaDTO
	extends AbstractDTO< String >
	implements Comparable< TurmaDTO >
{
	
	private static final long serialVersionUID = -4621914595888454527L;

	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CENARIO_ID = "cenarioId";
	public static final String PROPERTY_NOME = "nome";
	public static final String PROPERTY_DISCIPLINA_ID = "disciplinaId";
	public static final String PROPERTY_DISCIPLINA_STRING = "disciplinaString";
	public static final String PROPERTY_CAMPUS_ID = "campusId";
	public static final String PROPERTY_PARCIAL = "parcial";
	public static final String PROPERTY_NO_ALUNOS = "noAlunos";
	public static final String PROPERTY_CRED_ALOCADOS = "credAlocados";
	
	public TurmaDTO()
	{
		super();
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
	
	public void setNome( String value )
	{
		set( PROPERTY_NOME, value );
	}

	public String getNome()
	{
		return get( PROPERTY_NOME );
	}
	
	public void setDisciplinaId( Long value )
	{
		set( PROPERTY_DISCIPLINA_ID, value );
	}

	public Long getDisciplinaId()
	{
		return get( PROPERTY_DISCIPLINA_ID );
	}
	
	public void setDisciplinaString( String value )
	{
		set( PROPERTY_DISCIPLINA_STRING, value );
	}

	public String getDisciplinaString()
	{
		return get( PROPERTY_DISCIPLINA_STRING );
	}
	
	public void setCampusId( Long value )
	{
		set( PROPERTY_CAMPUS_ID, value );
	}

	public Long getCampusId()
	{
		return get( PROPERTY_CAMPUS_ID );
	}
	
	public void setParcial( Boolean value )
	{
		set( PROPERTY_PARCIAL, value );
	}

	public Boolean getParcial()
	{
		return get( PROPERTY_PARCIAL );
	}
	
	public void setNoAlunos( Integer value )
	{
		set( PROPERTY_NO_ALUNOS, value );
	}

	public Integer getNoAlunos()
	{
		return get( PROPERTY_NO_ALUNOS );
	}
	
	public void setCredAlocados( Integer value )
	{
		set( PROPERTY_CRED_ALOCADOS, value );
	}

	public Integer getCredAlocados()
	{
		return get( PROPERTY_CRED_ALOCADOS );
	}

	@Override
	public int compareTo(TurmaDTO o) {
		return this.getNaturalKey().compareTo( o.getNaturalKey() );
	}

	@Override
	public String getNaturalKey() {
		String key = this.getNome() + "-" + this.getCampusId() + "-" +  this.getDisciplinaId() + "-" + this.getCenarioId();
		return key;
	}

}
