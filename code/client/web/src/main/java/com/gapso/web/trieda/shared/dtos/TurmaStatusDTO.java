package com.gapso.web.trieda.shared.dtos;

public class TurmaStatusDTO
	extends AbstractDTO< String >
	implements Comparable< TurmaStatusDTO >
{

	private static final long serialVersionUID = 5213151854670364431L;

	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CENARIO_ID = "cenarioId";
	public static final String PROPERTY_NOME = "nome";
	public static final String PROPERTY_QTDE_DISC_SELECIONADA = "qtdeDiscSelecionada";
	public static final String PROPERTY_QTDE_TOTAL = "qtdeTotal";
	public static final String PROPERTY_STATUS = "status";
	public static final String PROPERTY_TURMA = "turma";
	public static final String PROPERTY_DISCIPLINA_ID = "disciplinaId";
	

	public TurmaStatusDTO()
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
	
	@Override
	public String getNaturalKey()
	{
		return this.getDisplayText();
	}
	
	public void setQtdeDiscSelecionada( Integer value )
	{
		set( PROPERTY_QTDE_DISC_SELECIONADA, value );
	}

	public Integer getQtdeDiscSelecionada()
	{
		return get( PROPERTY_QTDE_DISC_SELECIONADA );
	}
	
	public void setQtdeTotal( Integer value )
	{
		set( PROPERTY_QTDE_TOTAL, value );
	}

	public Integer getQtdeTotal()
	{
		return get( PROPERTY_QTDE_TOTAL );
	}
	
	public void setStatus( String value )
	{
		set( PROPERTY_STATUS, value );
	}

	public String getStatus()
	{
		return get( PROPERTY_STATUS );
	}
	
	public void setTurma( String value )
	{
		set( PROPERTY_TURMA, value );
	}

	public String getTurma()
	{
		return get( PROPERTY_TURMA );
	}
	
	public void setDisciplinaId( Long value )
	{
		set( PROPERTY_DISCIPLINA_ID, value );
	}

	public Long getDisciplinaId()
	{
		return get( PROPERTY_DISCIPLINA_ID );
	}

	@Override
	public int compareTo( TurmaStatusDTO o )
	{
		return this.getDisplayText().compareTo( o.getDisplayText() );
	}
}
