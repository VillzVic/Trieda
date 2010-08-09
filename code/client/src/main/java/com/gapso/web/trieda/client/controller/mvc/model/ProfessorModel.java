package com.gapso.web.trieda.client.controller.mvc.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class ProfessorModel extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public ProfessorModel() {
		super();
	}

	public ProfessorModel(String cpf, String nome, String tipo, Integer cargaHorMin, Integer cargaHorMax, String titulacao, String areaDeTitulacao, String nota, Integer credAnterior, Integer valorCredito) {
		setCpf(cpf);
		setNome(nome);
		setTipo(tipo);
		setCargaHorariaMinima(cargaHorMin);
		setCargaHorariaMaxima(cargaHorMax);
		setTitulacao(titulacao);
		setAreaDeTitulacao(areaDeTitulacao);
		setNota(nota);
		setCreditosAnteriores(credAnterior);
		setValorCredito(valorCredito);
	}
	
	public String getCpf() {
		return get("cpf");
	}
	public void setCpf(String value) {
		set("cpf", value);
	}

	public String getNome() {
		return get("nome");
	}
	public void setNome(String value) {
		set("nome", value);
	}
	
	public String getTipo() {
		return get("tipo");
	}
	public void setTipo(String value) {
		set("tipo", value);
	}
	
	public Integer getCargaHorariaMinima() {
		return get("hor_min");
	}
	public void setCargaHorariaMinima(Integer value) {
		set("hor_min", value);
	}
	
	public Integer getCargaHorariaMaxima() {
		return get("hor_max");
	}
	public void setCargaHorariaMaxima(Integer value) {
		set("hor_max", value);
	}
	
	public String getTitulacao() {
		return get("titulacao");
	}
	public void setTitulacao(String value) {
		set("titulacao", value);
	}
	
	public String getAreaDeTitulacao() {
		return get("area_titulacao");
	}
	public void setAreaDeTitulacao(String value) {
		set("area_titulacao", value);
	}
	
	public String getNota() {
		return get("nota");
	}
	public void setNota(String value) {
		set("nota", value);
	}
	
	public Integer getCreditosAnteriores() {
		return get("cred_anterior");
	}
	public void setCreditosAnteriores(Integer value) {
		set("cred_anterior", value);
	}
	
	public Integer getValorCredito() {
		return get("valor_credito");
	}
	public void setValorCredito(Integer value) {
		set("valor_credito", value);
	}
	
}
