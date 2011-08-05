package com.gapso.web.trieda.shared.dtos;


public class UnidadeDTO extends AbstractDTO< String >
	implements Comparable< UnidadeDTO >
{
	private static final long serialVersionUID = -5134820110949139907L;

	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CODIGO = "codigo";
	public static final String PROPERTY_NOME = "nome";
	public static final String PROPERTY_CAMPUS_ID = "campusId";
	public static final String PROPERTY_CAMPUS_STRING = "campusString";
	public static final String PROPERTY_CAPACIDADE_SALAS = "capSalas";

	public UnidadeDTO()
	{
		super();
	}

	public UnidadeDTO( Long id, String codigo, String nome, Long campusId,
		String campusString, Integer capSalas, Integer version )
	{
		setId( id );
		setCodigo( codigo );
		setNome( nome );
		setCampusId( campusId );
		setCampusString( campusString );
		setCapSalas( capSalas );
		setVersion( version );
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

	public String getNome() {
		return get(PROPERTY_NOME);
	}
	public void setNome(String value) {
		set(PROPERTY_NOME, value);
	}
	
	public Long getCampusId() {
		return get(PROPERTY_CAMPUS_ID);
	}
	public void setCampusId(Long value) {
		set(PROPERTY_CAMPUS_ID, value);
	}
	
	public String getCampusString() {
		return get(PROPERTY_CAMPUS_STRING);
	}
	public void setCampusString(String value) {
		set(PROPERTY_CAMPUS_STRING, value);
	}
	
	public Integer getCapSalas() {
		return get(PROPERTY_CAPACIDADE_SALAS);
	}
	public void setCapSalas(Integer value) {
		set(PROPERTY_CAPACIDADE_SALAS, value);
	}

	@Override
	public String getNaturalKey()
	{
		return getCodigo();
	}

	@Override
	public int compareTo( UnidadeDTO o )
	{
		return getNome().compareTo( o.getNome() );
	}
}