package com.gapso.web.trieda.shared.dtos;

public class AlunoStatusDTO 
	extends AbstractDTO< String >
	implements Comparable< AlunoStatusDTO >
{
	
	private static final long serialVersionUID = 678001366865992766L;

	public static final String PROPERTY_ALUNO_ID = "alunoId";
	public static final String PROPERTY_ALUNO_DEMANDA_ID = "alunoDemandaId";
	public static final String PROPERTY_MARCADO = "marcado";
	public static final String PROPERTY_MATRICULA = "matricula";
	public static final String PROPERTY_NOME = "nome";
	public static final String PROPERTY_STATUS = "status";
	public static final String PROPERTY_EQUIVALENCIA_ID = "equivalenciaId";
	public static final String PROPERTY_EQUIVALENCIA_STRING = "equivalenciaString";
	public static final String PROPERTY_FORMANDO = "formando";
	
	public AlunoStatusDTO()
	{
		super();
	}
	
	public void setAlunoId( Long value )
	{
		set( PROPERTY_ALUNO_ID, value );
	}

	public Long getAlunoId()
	{
		return get( PROPERTY_ALUNO_ID );
	}
	
	public void setAlunoDemandaId( Long value )
	{
		set( PROPERTY_ALUNO_DEMANDA_ID, value );
	}

	public Long getAlunoDemandaId()
	{
		return get( PROPERTY_ALUNO_DEMANDA_ID );
	}
	
	public void setMarcado( Boolean value )
	{
		set( PROPERTY_MARCADO, value );
	}

	public Boolean getMarcado()
	{
		return get( PROPERTY_MARCADO );
	}
	
	public void setMatricula( String value )
	{
		set( PROPERTY_MATRICULA, value );
	}

	public String getMatricula()
	{
		return get( PROPERTY_MATRICULA );
	}
	
	public void setNome( String value )
	{
		set( PROPERTY_NOME, value );
	}

	public String getNome()
	{
		return get( PROPERTY_NOME );
	}
	
	public void setStatus( String value )
	{
		set( PROPERTY_STATUS, value );
	}

	public String getStatus()
	{
		return get( PROPERTY_STATUS );
	}
	
	public void setEquivalenciaId( Long value )
	{
		set( PROPERTY_EQUIVALENCIA_ID, value );
	}

	public Long getEquivalenciaId()
	{
		return get( PROPERTY_EQUIVALENCIA_ID );
	}
	
	public void setEquivalenciaSring( String value )
	{
		set( PROPERTY_EQUIVALENCIA_STRING, value );
	}

	public String getEquivalenciaString()
	{
		return get( PROPERTY_EQUIVALENCIA_STRING );
	}
	
	public void setFormando( Boolean value )
	{
		set( PROPERTY_FORMANDO, value );
	}

	public Boolean getFormando()
	{
		return get( PROPERTY_FORMANDO );
	}

	@Override
	public int compareTo(AlunoStatusDTO o) {
		return this.getAlunoDemandaId().compareTo(o.getAlunoDemandaId());
	}

	@Override
	public String getNaturalKey() {
		return getAlunoDemandaId().toString();
	}
}
