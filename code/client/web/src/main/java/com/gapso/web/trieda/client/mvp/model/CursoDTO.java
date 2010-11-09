package com.gapso.web.trieda.client.mvp.model;


public class CursoDTO extends FileModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public CursoDTO() {
		super();
	}

	public CursoDTO(Long id, String codigo, String nome, Long tipoId, String tipoString, Integer numMinDoutores, Integer numMinMestres, Integer maxDisciplinasPeloProfessor, Boolean admMaisDeUmDisciplina, Integer version) {
		setId(id);
		setCodigo(codigo);
		setNome(nome);
		setTipoId(tipoId);
		setTipoString(tipoString);
		setNumMinDoutores(numMinDoutores);
		setNumMinMestres(numMinMestres);
		setMaxDisciplinasPeloProfessor(maxDisciplinasPeloProfessor);
		setAdmMaisDeUmDisciplina(admMaisDeUmDisciplina);
		setVersion(version);
	}
	
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
	
	public String getCodigo() {
		return get("codigo");
	}
	public void setCodigo(String value) {
		set("codigo", value);
	}
	
	public String getNome() {
		return get("nome");
	}
	public void setNome(String value) {
		set("nome", value);
	}
	
	public Long getTipoId() {
		return get("tipoId");
	}
	public void setTipoId(Long value) {
		set("tipoId", value);
	}
	
	public String getTipoString() {
		return get("tipoString");
	}
	public void setTipoString(String value) {
		set("tipoString", value);
	}
	
	public Integer getNumMinDoutores() {
		return get("numMinDoutores");
	}
	public void setNumMinDoutores(Integer value) {
		set("numMinDoutores", value);
	}
	
	public Integer getNumMinMestres() {
		return get("numMinMestres");
	}
	public void setNumMinMestres(Integer value) {
		set("numMinMestres", value);
	}
	
	public Integer getMaxDisciplinasPeloProfessor() {
		return get("maxDisciplinasPeloProfessor");
	}
	public void setMaxDisciplinasPeloProfessor(Integer value) {
		set("maxDisciplinasPeloProfessor", value);
	}
	
	public Boolean getAdmMaisDeUmDisciplina() {
		return get("admMaisDeUmDisciplina");
	}
	public void setAdmMaisDeUmDisciplina(Boolean value) {
		set("admMaisDeUmDisciplina", value);
	}

}
