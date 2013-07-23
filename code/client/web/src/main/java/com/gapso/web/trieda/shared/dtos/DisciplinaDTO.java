package com.gapso.web.trieda.shared.dtos;

public class DisciplinaDTO extends AbstractDTO<String>
	implements Comparable<DisciplinaDTO>
{
	private static final long serialVersionUID = -5134820110949139907L;
	
	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CENARIO_ID = "cenarioId";
	public static final String PROPERTY_CODIGO = "codigo";
	public static final String PROPERTY_NOME = "nome";
	public static final String PROPERTY_CREDITOS_TEORICO = "creditosTeorico";
	public static final String PROPERTY_CREDITOS_PRATICO = "creditosPratico";
	public static final String PROPERTY_LABORATORIO = "laboratorio";
	public static final String PROPERTY_TIPO_ID = "tipoDisciplinaId";
	public static final String PROPERTY_TIPO_STRING = "tipoDisciplinaString";
	public static final String PROPERTY_DIFICULDADE = "dificuldade";
	public static final String PROPERTY_MAX_ALUNOS_TEORICO = "maxAlunosTeorico";
	public static final String PROPERTY_MAX_ALUNOS_PRATICO = "maxAlunosPratico";
	public static final String PROPERTY_USA_SABADO = "usaSabado";
	public static final String PROPERTY_USA_DOMINGO = "usaDomingo";

	public DisciplinaDTO()
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

	public void setCenarioId(Long value) {
		set(PROPERTY_CENARIO_ID, value);
	}
	public Long getCenarioId() {
		return get(PROPERTY_CENARIO_ID);
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
	
	public void setCreditosTeorico(Integer value) {
		set(PROPERTY_CREDITOS_TEORICO, value);
	}
	public Integer getCreditosTeorico() {
		return get(PROPERTY_CREDITOS_TEORICO);
	}
	
	public void setCreditosPratico(Integer value) {
		set(PROPERTY_CREDITOS_PRATICO, value);
	}
	public Integer getCreditosPratico() {
		return get(PROPERTY_CREDITOS_PRATICO);
	}
	
	public void setLaboratorio(Boolean value) {
		set(PROPERTY_LABORATORIO, value);
	}
	public Boolean getLaboratorio() {
		return get(PROPERTY_LABORATORIO);
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
	
	public String getDificuldade() {
		return get(PROPERTY_DIFICULDADE);
	}
	public void setDificuldade(String value) {
		set(PROPERTY_DIFICULDADE, value);
	}
	
	public Integer getMaxAlunosTeorico() {
		return get(PROPERTY_MAX_ALUNOS_TEORICO);
	}
	public void setMaxAlunosTeorico(Integer value) {
		set(PROPERTY_MAX_ALUNOS_TEORICO, value);
	}
	
	public Integer getMaxAlunosPratico() {
		return get(PROPERTY_MAX_ALUNOS_PRATICO);
	}
	public void setMaxAlunosPratico(Integer value) {
		set(PROPERTY_MAX_ALUNOS_PRATICO, value);
	}
	
	public Boolean getUsaSabado() {
		return get(PROPERTY_USA_SABADO);
	}
	
	public void setUsaSabado(Boolean value) {
		set(PROPERTY_USA_SABADO, value);
	}
	
	public Boolean getUsaDomingo() {
		return get(PROPERTY_USA_DOMINGO);
	}
	
	public void setUsaDomingo(Boolean value) {
		set(PROPERTY_USA_DOMINGO, value);
	}
	
	public int getTotalCreditos() {
		return getCreditosTeorico() + getCreditosPratico();
	}

	@Override
	public String getNaturalKey() {
		return getCodigo();
	}

	@Override
	public int compareTo(DisciplinaDTO o) {
		return getNome().compareTo(o.getNome());
	}
}