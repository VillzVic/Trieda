package com.gapso.web.trieda.shared.dtos;

public class ProfessorStatusDTO
	extends AbstractDTO< String >
	implements Comparable< ProfessorStatusDTO >
{

	private static final long serialVersionUID = -7811033580392980080L;

	public static final String PROPERTY_PROFESSOR_ID = "prfessorId";
	public static final String PROPERTY_PROFESSOR_VIRTUAL_ID = "prfessorVirtualId";
	public static final String PROPERTY_MARCADO = "marcado";
	public static final String PROPERTY_STATUS = "status";
	public static final String PROPERTY_CPF = "cpf";
	public static final String PROPERTY_TITULACAO = "titulacao";
	public static final String PROPERTY_CUSTO = "custo";
	public static final String PROPERTY_NOTA = "nota";
	public static final String PROPERTY_PREFERENCIA = "preferencia";
	public static final String PROPERTY_NOME = "nome";
	
	public ProfessorStatusDTO()
	{
		super();
	}
	
	public void setProfessorId( Long value )
	{
		set( PROPERTY_PROFESSOR_ID, value );
	}

	public Long getProfessorId()
	{
		return get( PROPERTY_PROFESSOR_ID );
	}
	
	public void setProfessorVirtualId( Long value )
	{
		set( PROPERTY_PROFESSOR_VIRTUAL_ID, value );
	}

	public Long getProfessorVirtualId()
	{
		return get( PROPERTY_PROFESSOR_VIRTUAL_ID );
	}
	
	public void setMarcado( Boolean value )
	{
		set( PROPERTY_MARCADO, value );
	}

	public Boolean getMarcado()
	{
		return get( PROPERTY_MARCADO );
	}
	
	public void setCpf( String value )
	{
		set( PROPERTY_CPF, value );
	}

	public String getCpf()
	{
		return get( PROPERTY_CPF );
	}
	
	public void setStatus( String value )
	{
		set( PROPERTY_STATUS, value );
	}

	public String getStatus()
	{
		return get( PROPERTY_STATUS );
	}
	
	public void setNome( String value )
	{
		set( PROPERTY_NOME, value );
	}

	public String getNome()
	{
		return get( PROPERTY_NOME );
	}
	
	public void setTitulacao( String value )
	{
		set( PROPERTY_TITULACAO, value );
	}

	public String getTitulacao()
	{
		return get( PROPERTY_TITULACAO );
	}
	
	public void setCusto( Integer value )
	{
		set( PROPERTY_CUSTO, value );
	}

	public Integer getCusto()
	{
		return get( PROPERTY_CUSTO );
	}
	
	public void setNota( Integer value )
	{
		set( PROPERTY_NOTA, value );
	}

	public Integer getNota()
	{
		return get( PROPERTY_NOTA );
	}
	
	public void setPreferencia( Integer value )
	{
		set( PROPERTY_PREFERENCIA, value );
	}

	public Integer getPreferencia()
	{
		return get( PROPERTY_PREFERENCIA );
	}

	@Override
	public int compareTo(ProfessorStatusDTO o) {
		return this.getProfessorId().compareTo(o.getProfessorId());
	}

	@Override
	public String getNaturalKey() {
		return getProfessorId().toString();
	}
}
