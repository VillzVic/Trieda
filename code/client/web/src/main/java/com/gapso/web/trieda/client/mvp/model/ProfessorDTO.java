package com.gapso.web.trieda.client.mvp.model;



public class ProfessorDTO extends FileModel {

	private static final long serialVersionUID = 5815525344760896272L;
	
	public ProfessorDTO() {
	}

	/* == PROPRIEDADES ==
	 * Long   : id
	 * Integer: version
	 * Long   : cenarioId
	 * String : nome
	 * String : cpf
	 * String : tipoContratoString
	 * Long   : tipoContratoId
	 * Integer: cargaHorariaMax
	 * Integer: cargaHorariaMin
	 * String : titulacaoString
	 * Long   : titulacaoId
	 * String : areaTitulacaoString
	 * Long   : areaTitulacaoId
	 * Integer: notaDesempenho
	 * Integer: creditoAnterior
	 * Double : valorCredito
	 */
	
	public void setId(Long value) {
		set("id", value);
	}
	public Long getId() {
		return get("id");
	}
	
	public void setVersion(Integer value) {
		set("version", value);
	}
	public Integer getVersion() {
		return get("version");
	}
	
	public void setCenarioId(Long value) {
		set("cenarioId", value);
	}
	public Long getCenarioId() {
		return get("cenarioId");
	}

	public void setNome(String value) {
		set("nome", value);
	}
	public String getNome() {
		return get("nome");
	}
	
	public void setCpf(String value) {
		set("cpf", value);
	}
	public String getCpf() {
		return get("cpf");
	}
	
	public void setTipoContratoString(String value) {
		set("tipoContratoString", value);
	}
	public String getTipoContratoString() {
		return get("tipoContratoString");
	}

	public void setTipoContratoId(Long value) {
		set("tipoContratoId", value);
	}
	public Long getTipoContratoId() {
		return get("tipoContratoId");
	}
	
	public void setCargaHorariaMax(Integer value) {
		set("cargaHorariaMax", value);
	}
	public Integer getCargaHorariaMax() {
		return get("cargaHorariaMax");
	}
	
	public void setCargaHorariaMin(Integer value) {
		set("cargaHorariaMin", value);
	}
	public Integer getCargaHorariaMin() {
		return get("cargaHorariaMin");
	}
	
	public void setTitulacaoString(String value) {
		set("titulacaoString", value);
	}
	public String getTitulacaoString() {
		return get("titulacaoString");
	}
	
	public void setTitulacaoId(Long value) {
		set("titulacaoId", value);
	}
	public Long getTitulacaoId() {
		return get("titulacaoId");
	}
	
	public void setAreaTitulacaoString(String value) {
		set("areaTitulacaoString", value);
	}
	public String getAreaTitulacaoString() {
		return get("areaTitulacaoString");
	}
	
	public void setAreaTitulacaoId(Long value) {
		set("areaTitulacaoId", value);
	}
	public Long getAreaTitulacaoId() {
		return get("areaTitulacaoId");
	}
	
	public void setNotaDesempenho(Integer value) {
		set("notaDesempenho", value);
	}
	public Integer getNotaDesempenho() {
		return get("notaDesempenho");
	}
	
	public void setCreditoAnterior(Integer value) {
		set("creditoAnterior", value);
	}
	public Integer getCreditoAnterior() {
		return get("creditoAnterior");
	}
	
	public void setValorCredito(Double value) {
		set("valorCredito", value);
	}
	public Double getValorCredito() {
		return get("valorCredito");
	}

}
