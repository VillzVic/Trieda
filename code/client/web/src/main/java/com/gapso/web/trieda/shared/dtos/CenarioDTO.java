package com.gapso.web.trieda.shared.dtos;

import java.util.Date;

public class CenarioDTO extends AbstractDTO<String> implements
		Comparable<CenarioDTO> {

	private static final long serialVersionUID = 4046119822523079085L;

	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_NOME = "nome";
	public static final String PROPERTY_MASTER_DATA = "masterData";
	public static final String PROPERTY_OFICIAL = "oficial";
	public static final String PROPERTY_ANO = "ano";
	public static final String PROPERTY_SEMESTRE = "semestre";
	public static final String PROPERTY_CRIADO_USUARIO_ID = "criadoUsuarioId";
	public static final String PROPERTY_CRIADO_USUARIO_STRING = "criadoUsuarioString";
	public static final String PROPERTY_CRIADO_USUARIO_DATE = "criadoUsuarioDate";
	public static final String PROPERTY_ATUALIZADO_USUARIO_ID = "atualizadoUsuarioId";
	public static final String PROPERTY_ATUALIZADO_USUARIO_STRING = "atualizadoUsuarioString";
	public static final String PROPERTY_ATUALIZADO_USUARIO_DATE = "atualizadoUsuarioDate";
	public static final String PROPERTY_COMENTARIO = "comentario";
	public static final String PROPERTY_ALUNOS = "alunos";
	public static final String PROPERTY_PROFESSORES = "professores";
	public static final String PROPERTY_SALAS = "salas";
	public static final String PROPERTY_OTIMIZADO= "otimizado";

	public CenarioDTO()
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

	public void setNome(String value) {
		set(PROPERTY_NOME, value);
	}

	public String getNome() {
		return get(PROPERTY_NOME);
	}

	public void setMasterData(Boolean value) {
		set(PROPERTY_MASTER_DATA, value);
	}

	public Boolean getMasterData() {
		return get(PROPERTY_MASTER_DATA);
	}

	public void setOficial(Boolean value) {
		set(PROPERTY_OFICIAL, value);
	}

	public Boolean getOficial() {
		return get(PROPERTY_OFICIAL);
	}

	public void setAno(Integer value) {
		set(PROPERTY_ANO, value);
	}

	public Integer getAno() {
		return get(PROPERTY_ANO);
	}

	public void setSemestre(Integer value) {
		set(PROPERTY_SEMESTRE, value);
	}

	public Integer getSemestre() {
		return get(PROPERTY_SEMESTRE);
	}

	public void setCriadoUsuarioId(String value) {
		set(PROPERTY_CRIADO_USUARIO_ID, value);
	}

	public String getCriadoUsuarioId() {
		return get(PROPERTY_CRIADO_USUARIO_ID);
	}

	public void setAtualizadoUsuarioId(Long value) {
		set(PROPERTY_ATUALIZADO_USUARIO_ID, value);
	}

	public Long getAtualizadoUsuarioId() {
		return get(PROPERTY_ATUALIZADO_USUARIO_ID);
	}

	public void setCriadoUsuarioString(String value) {
		set(PROPERTY_CRIADO_USUARIO_STRING, value);
	}

	public String getCriadoUsuarioString() {
		return get(PROPERTY_CRIADO_USUARIO_STRING);
	}

	public void setAtualizadoUsuarioString(String value) {
		set(PROPERTY_ATUALIZADO_USUARIO_STRING, value);
	}

	public String getAtualizadoUsuarioString() {
		return get(PROPERTY_ATUALIZADO_USUARIO_STRING);
	}

	public void setCriadoUsuarioDate(Date value) {
		set(PROPERTY_CRIADO_USUARIO_DATE, value);
	}

	public Date getCriadoUsuarioDate() {
		return get(PROPERTY_CRIADO_USUARIO_DATE);
	}

	public void setAtualizadoUsuarioDate(Date value) {
		set(PROPERTY_ATUALIZADO_USUARIO_DATE, value);
	}

	public Date getAtualizadoUsuarioDate() {
		return get(PROPERTY_ATUALIZADO_USUARIO_DATE);
	}

	public void setComentario(String value) {
		set(PROPERTY_COMENTARIO, value);
	}

	public String getComentario() {
		return get(PROPERTY_COMENTARIO);
	}
	
	public void setAlunos(boolean value) {
		set(PROPERTY_ALUNOS, value);
	}
	
	public boolean hasAlunos(){
		return get(PROPERTY_ALUNOS);
	}
	
	public void setProfessores(boolean value){
		set(PROPERTY_PROFESSORES, value);
	}
	
	public boolean hasProfessores(){
		return get(PROPERTY_PROFESSORES);
	}
	
	public void setSalas(boolean value){
		set(PROPERTY_SALAS, value);
	}
	
	public boolean hasSalas(){
		return get(PROPERTY_SALAS);
	}

	@Override
	public String getNaturalKey() {
		return getNome();
	}

	@Override
	public int compareTo(CenarioDTO o) {
		return getNome().compareTo(o.getNome());
	}
	
	public void setOptimized(boolean value){
		set(PROPERTY_OTIMIZADO, value);
	}

	public boolean isOptimized() {
		return get(PROPERTY_OTIMIZADO);
	}
	
	

	
}