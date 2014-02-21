package com.gapso.web.trieda.shared.dtos;

public class ParametroConfiguracaoDTO extends AbstractDTO< String >
	implements Comparable< ParametroConfiguracaoDTO >
{
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 5667615446526876214L;
	
	public static final String PROPERTY_DATA_SOURCE = "campusNome";
	public static final String PROPERTY_URL_OTIMIZACAO = "urlOtimizacao";
	public static final String PROPERTY_NOME_OTIMIZACAO = "nomeOtimizacao";
	
	public ParametroConfiguracaoDTO()
	{
		super();
	}
	
	public void setDataSource(String value) {
		set(PROPERTY_DATA_SOURCE, value);
	}

	public String getDataSource() {
		return get(PROPERTY_DATA_SOURCE);
	}
	
	public void setUrlOtimizacao(String value) {
		set(PROPERTY_URL_OTIMIZACAO, value);
	}

	public String getUrlOtimizacao() {
		return get(PROPERTY_URL_OTIMIZACAO);
	}
	
	public void setNomeOtimizacao(String value) {
		set(PROPERTY_NOME_OTIMIZACAO, value);
	}

	public String getNomeOtimizacao() {
		return get(PROPERTY_NOME_OTIMIZACAO);
	}

	@Override
	public int compareTo(ParametroConfiguracaoDTO o) {
		return getInstituicaoEnsinoId().compareTo(o.getInstituicaoEnsinoId());
	}

	@Override
	public String getNaturalKey() {
		return getInstituicaoEnsinoString();
	}
}
