package com.gapso.web.trieda.shared.dtos;



public class CursoDTO extends AbstractDTO<String> implements Comparable<CursoDTO> {

	private static final long serialVersionUID = -5134820110949139907L;
	
	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CENARIO_ID = "cenarioId";
	public static final String PROPERTY_CODIGO = "codigo";
	public static final String PROPERTY_NOME = "nome";
	public static final String PROPERTY_TIPO_ID = "tipoId";
	public static final String PROPERTY_TIPO_STRING = "tipoString";
	public static final String PROPERTY_NUM_MIN_DOUTORES = "numMinDoutores";
	public static final String PROPERTY_NUM_MIN_MESTRES = "numMinMestres";
	public static final String PROPERTY_MAX_DISCIPLINAS_PELO_PROFESSOR = "maxDisciplinasPeloProfessor";
	public static final String PROPERTY_ADM_MAIS_DE_UMA_DISCIPLINA = "admMaisDeUmDisciplina";

	public CursoDTO() {
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
	
	public Integer getNumMinDoutores() {
		return get(PROPERTY_NUM_MIN_DOUTORES);
	}
	public void setNumMinDoutores(Integer value) {
		set(PROPERTY_NUM_MIN_DOUTORES, value);
	}
	
	public Integer getNumMinMestres() {
		return get(PROPERTY_NUM_MIN_MESTRES);
	}
	public void setNumMinMestres(Integer value) {
		set(PROPERTY_NUM_MIN_MESTRES, value);
	}
	
	public Integer getMaxDisciplinasPeloProfessor() {
		return get(PROPERTY_MAX_DISCIPLINAS_PELO_PROFESSOR);
	}
	public void setMaxDisciplinasPeloProfessor(Integer value) {
		set(PROPERTY_MAX_DISCIPLINAS_PELO_PROFESSOR, value);
	}
	
	public Boolean getAdmMaisDeUmDisciplina() {
		return get(PROPERTY_ADM_MAIS_DE_UMA_DISCIPLINA);
	}
	public void setAdmMaisDeUmDisciplina(Boolean value) {
		set(PROPERTY_ADM_MAIS_DE_UMA_DISCIPLINA, value);
	}
	
	@Override
	public String getNaturalKey() {
		return getCodigo();
	}	

	@Override
	public int compareTo(CursoDTO o) {
		return getNome().compareTo(o.getNome());
	}	
}