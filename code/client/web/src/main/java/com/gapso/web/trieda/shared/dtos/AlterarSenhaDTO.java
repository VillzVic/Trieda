package com.gapso.web.trieda.shared.dtos;

public class AlterarSenhaDTO extends AbstractDTO< String >
	implements Comparable< AlterarSenhaDTO >
{

	private static final long serialVersionUID = 8490302535896353271L;

	public static final String PROPERTY_SENHA_ANTIGA = "senhaAntiga";
	public static final String PROPERTY_SENHA_NOVA = "senhaNova";
	public static final String PROPERTY_SENHA_NOVA_CONF = "senhaNovaConf";
	
	public AlterarSenhaDTO()
	{
		super();
	}

	public void setSenhaNova(String value) {
		set(PROPERTY_SENHA_NOVA, value);
	}
	
	public String getSenhaNova() {
		return get(PROPERTY_SENHA_NOVA);
	}
	
	public void setSenhaAntiga(String value) {
		set(PROPERTY_SENHA_ANTIGA, value);
	}
	
	public String getSenhaAntiga() {
		return get(PROPERTY_SENHA_ANTIGA);
	}
		
	public void setSenhaNovaConf(String value) {
		set(PROPERTY_SENHA_NOVA_CONF, value);
	}
	
	public String getSenhaNovaConf() {
		return get(PROPERTY_SENHA_NOVA_CONF);
	}
	
	@Override
	public int compareTo(AlterarSenhaDTO o) {
		return getSenhaAntiga().compareTo(o.getSenhaAntiga());
	}
	
	@Override
	public String getNaturalKey() {
		return getSenhaAntiga();
	}
}