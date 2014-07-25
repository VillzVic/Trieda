package com.gapso.web.trieda.shared.dtos;

public class SalaStatusDTO
	extends AbstractDTO< String >
	implements Comparable< SalaStatusDTO >
{
	
	private static final long serialVersionUID = 7417413556442180L;
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CODIGO = "codigo";
	public static final String PROPERTY_CAPACIDADE_INSTALADA = "capacidadeInstalada";
	public static final String PROPERTY_TIPO_STRING = "tipoSalaString";
	public static final String PROPERTY_STATUS = "status";
	
	public SalaStatusDTO()
	{
		super();
	}
	
	public void setId(Long value) {
		set(PROPERTY_ID, value);
	}
	public Long getId() {
		return get(PROPERTY_ID);
	}
	
	public void setVersion(Integer value) {
		set(PROPERTY_VERSION, value);
	}
	public Integer getVersion() {
		return get(PROPERTY_VERSION);
	}

	public String getCodigo() {
		return get(PROPERTY_CODIGO);
	}
	public void setCodigo(String value) {
		set(PROPERTY_CODIGO, value);
	}
	
	public Integer getCapacidadeInstalada() {
		return get(PROPERTY_CAPACIDADE_INSTALADA);
	}
	public void setCapacidadeInstalada(Integer value) {
		set(PROPERTY_CAPACIDADE_INSTALADA, value);
	}
	
	public String getTipoString() {
		return get(PROPERTY_TIPO_STRING);
	}
	public void setTipoString(String value) {
		set(PROPERTY_TIPO_STRING, value);
	}
	
	public void setStatus( String value )
	{
		set( PROPERTY_STATUS, value );
	}

	public String getStatus()
	{
		return get( PROPERTY_STATUS );
	}
	
	@Override
	public String getNaturalKey()
	{
		return this.getDisplayText();
	}

	@Override
	public int compareTo( SalaStatusDTO o )
	{
		return this.getDisplayText().compareTo( o.getDisplayText() );
	}

}
