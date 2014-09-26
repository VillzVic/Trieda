package com.gapso.web.trieda.shared.dtos;

public class DisponibilidadeDTO 
	extends AbstractDTO< String >
	implements Comparable< DisponibilidadeDTO >
{
	private static final long serialVersionUID = -7948107704551255189L;
	
	// Tipos de Entidade
	public static final String DISCIPLINA = "DisponibilidadeDisciplina";
	public static final String PROFESSOR = "DisponibilidadeProfessor";
	public static final String CAMPUS = "DisponibilidadeCampus";
	public static final String UNIDADE = "DisponibilidadeUnidade";
	public static final String SALA = "DisponibilidadeSala";
	public static final String FIXACAO = "DisponibilidadeFixacao";
	
	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_ENTIDADE_TIPO = "entidadeTipo";
	public static final String PROPERTY_ENTIDADE_ID = "entidadeId";
	public static final String PROPERTY_HOR_INICIO = "horInicio";
	public static final String PROPERTY_HOR_FIM = "horFim";
	public static final String PROPERTY_SEGUNDA = "segunda";
	public static final String PROPERTY_TERCA = "terca";
	public static final String PROPERTY_QUARTA = "quarta";
	public static final String PROPERTY_QUINTA = "quinta";
	public static final String PROPERTY_SEXTA = "sexta";
	public static final String PROPERTY_SABADO = "sabado";
	public static final String PROPERTY_DOMINGO = "domingo";
	
	public void setDisponibilidadeId( Long value )
	{
		set( PROPERTY_ID, value );
	}

	public Long getDisponibilidadeId()
	{
		return get( PROPERTY_ID );
	}
	
	public void setEntidadeTipo( String value )
	{
		set( PROPERTY_ENTIDADE_TIPO, value );
	}

	public String getEntidadeTipo()
	{
		return get( PROPERTY_ENTIDADE_TIPO );
	}
	
	public void setEntidadeId( Long value )
	{
		set( PROPERTY_ENTIDADE_ID, value );
	}

	public Long getEntidadeId()
	{
		return get( PROPERTY_ENTIDADE_ID );
	}
	
	public void setHorarioInicioString( String value )
	{
		set( PROPERTY_HOR_INICIO, value );
	}

	public String getHorarioInicioString()
	{
		return get( PROPERTY_HOR_INICIO );
	}
	
	public void setHorarioFimString( String value )
	{
		set( PROPERTY_HOR_FIM, value );
	}

	public String getHorarioFimString()
	{
		return get( PROPERTY_HOR_FIM );
	}
	
	public void setSegunda( Boolean value )
	{
		set( PROPERTY_SEGUNDA, value );
	}

	public Boolean getSegunda()
	{
		return get( PROPERTY_SEGUNDA );
	}
	
	public void setTerca( Boolean value )
	{
		set( PROPERTY_TERCA, value );
	}

	public Boolean getTerca()
	{
		return get( PROPERTY_TERCA );
	}
	
	public void setQuarta( Boolean value )
	{
		set( PROPERTY_QUARTA, value );
	}

	public Boolean getQuarta()
	{
		return get( PROPERTY_QUARTA );
	}
	
	public void setQuinta( Boolean value )
	{
		set( PROPERTY_QUINTA, value );
	}

	public Boolean getQuinta()
	{
		return get( PROPERTY_QUINTA );
	}
	public void setSexta( Boolean value )
	{
		set( PROPERTY_SEXTA, value );
	}

	public Boolean getSexta()
	{
		return get( PROPERTY_SEXTA );
	}
	
	public void setSabado( Boolean value )
	{
		set( PROPERTY_SABADO, value );
	}

	public Boolean getSabado()
	{
		return get( PROPERTY_SABADO );
	}
	
	public void setDomingo( Boolean value )
	{
		set( PROPERTY_DOMINGO, value );
	}

	public Boolean getDomingo()
	{
		return get( PROPERTY_DOMINGO );
	}

	@Override
	public int compareTo(DisponibilidadeDTO o) {
		return getDisponibilidadeId().compareTo(o.getDisponibilidadeId());
	}

	@Override
	public String getNaturalKey() {
		return getDisponibilidadeId().toString();
	}
}
