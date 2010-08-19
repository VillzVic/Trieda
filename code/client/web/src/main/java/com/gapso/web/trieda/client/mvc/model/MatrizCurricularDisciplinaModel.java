package com.gapso.web.trieda.client.mvc.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class MatrizCurricularDisciplinaModel extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public MatrizCurricularDisciplinaModel() {
		super();
	}
	
	public MatrizCurricularDisciplinaModel(String matrizCurricular, String curso, Integer periodo, String disciplina, Integer credTeoricos, Integer credPraticos, Integer totalCreditos) {
		setMatrizCurricular(matrizCurricular);
		setCurso(curso);
		setPeriodo(periodo);
		setDisciplina(disciplina);
		setCredTeoricos(credTeoricos);
		setCredPraticos(credPraticos);
		setTotalCreditos(totalCreditos);
	}
	
	public String getMatrizCurricular() {
		return get("matriz_curricular");
	}
	public void setMatrizCurricular(String value) {
		set("matriz_curricular", value);
	}
	
	public String getCurso() {
		return get("curso");
	}
	public void setCurso(String value) {
		set("curso", value);
	}
	
	public Integer getPeriodo() {
		return get("periodo");
	}
	public void setPeriodo(Integer value) {
		set("periodo", value);
	}
	
	public String getDisciplina() {
		return get("disciplina_nome");
	}
	public void setDisciplina(String value) {
		set("disciplina_nome", value);
	}
	
	public Integer getCredTeoricos() {
		return get("cred_teoricos");
	}
	public void setCredTeoricos(Integer value) {
		set("cred_teoricos", value);
	}
	
	public Integer getCredPraticos() {
		return get("cred_pratico");
	}
	public void setCredPraticos(Integer value) {
		set("cred_pratico", value);
	}

	public Integer getTotalCreditos() {
		return get("total_creditos");
	}
	public void setTotalCreditos(Integer value) {
		set("total_creditos", value);
	}
	
}
