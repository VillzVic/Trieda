package com.gapso.web.trieda.shared.dtos;



public class SalaDTO extends AbstractDTO< String >
	implements Comparable< SalaDTO >
{
	private static final long serialVersionUID = -5134820110949139907L;

	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CODIGO = "codigo";
	public static final String PROPERTY_CAMPUS_ID = "campusId";
	public static final String PROPERTY_UNIDADE_ID = "unidadeId";
	public static final String PROPERTY_UNIDADE_STRING = "unidadeString";
	public static final String PROPERTY_NUMERO = "numero";
	public static final String PROPERTY_DESCRICAO = "descricao";
	public static final String PROPERTY_ANDAR = "andar";
	public static final String PROPERTY_TIPO_ID = "tipoSalaId";
	public static final String PROPERTY_TIPO_STRING = "tipoSalaString";
	public static final String PROPERTY_CAPACIDADE_INSTALADA = "capacidadeInstalada";
	public static final String PROPERTY_CAPACIDADE_MAX = "capacidadeMax";
	public static final String PROPERTY_CUSTO_OPERACAO_CRED = "custoOperacaoCred";
	public static final String PROPERTY_EXTERNA = "externa";
	public static final String PROPERTY_CONTAINS_CURRICULO_DISCIPLINA = "containsCurriculoDisciplina";
	public static final String PROPERTY_INSTITUICAO_ENSINO_ID = "instituicaoEnsinoId";
	public static final String PROPERTY_INSTITUICAO_ENSINO_STRING = "instituicaoEnsinoString";

	public SalaDTO()
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

	public Long getCampusId() {
		return get(PROPERTY_CAMPUS_ID);
	}
	public void setCampusId(Long value) {
		set(PROPERTY_CAMPUS_ID, value);
	}
	
	public Long getUnidadeId() {
		return get(PROPERTY_UNIDADE_ID);
	}
	public void setUnidadeId(Long value) {
		set(PROPERTY_UNIDADE_ID, value);
	}

	public String getUnidadeString() {
		return get(PROPERTY_UNIDADE_STRING);
	}
	public void setUnidadeString(String value) {
		set(PROPERTY_UNIDADE_STRING, value);
	}
	
	public String getNumero() {
		return get(PROPERTY_NUMERO);
	}
	public void setNumero(String value) {
		set(PROPERTY_NUMERO, value);
	}
	
	public String getDescricao() {
		return get(PROPERTY_DESCRICAO);
	}
	public void setDescricao(String value) {
		set(PROPERTY_DESCRICAO, value);
	}
	
	public String getAndar() {
		return get(PROPERTY_ANDAR);
	}
	public void setAndar(String value) {
		set(PROPERTY_ANDAR, value);
	}
	
	public Long getTipoId() {
		return get(PROPERTY_TIPO_ID);
	}
	public void setTipoId(Long value) {
		set(PROPERTY_TIPO_ID, value);
	}
	
	public String getTipoString() {
		return get(PROPERTY_TIPO_STRING);
	}
	public void setTipoString(String value) {
		set(PROPERTY_TIPO_STRING, value);
	}

	public Integer getCapacidadeInstalada() {
		return get(PROPERTY_CAPACIDADE_INSTALADA);
	}
	public void setCapacidadeInstalada(Integer value) {
		set(PROPERTY_CAPACIDADE_INSTALADA, value);
	}
	
	public Integer getCapacidadeMax() {
		return get(PROPERTY_CAPACIDADE_MAX);
	}
	public void setCapacidadeMax(Integer value) {
		set(PROPERTY_CAPACIDADE_MAX, value);
	}
	
	public Double getCustoOperacaoCred() {
		return get(PROPERTY_CUSTO_OPERACAO_CRED);
	}
	public void setCustoOperacaoCred(Double value) {
		set(PROPERTY_CUSTO_OPERACAO_CRED, value);
	}
	
	public Boolean getExterna() {
		return get(PROPERTY_EXTERNA);
	}
	public void setExterna(Boolean value) {
		set(PROPERTY_EXTERNA, value);
	}
	
	public Boolean getContainsCurriculoDisciplina() {
		return get(PROPERTY_CONTAINS_CURRICULO_DISCIPLINA);
	}
	public void setContainsCurriculoDisciplina(Boolean value) {
		set(PROPERTY_CONTAINS_CURRICULO_DISCIPLINA, value);
	}
	
	public boolean isLaboratorio()
	{
		return getTipoId() == 2;
	}

	@Override
	public String getNaturalKey()
	{
		return ( getCampusId() + "-" + getUnidadeId() + "-" + getId() );
	}

	@Override
	public int compareTo(SalaDTO o)
	{
		return getCodigo().compareTo( o.getCodigo() );
	}
}
