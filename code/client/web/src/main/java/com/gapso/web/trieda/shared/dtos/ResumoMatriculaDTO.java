package com.gapso.web.trieda.shared.dtos;

public class ResumoMatriculaDTO extends AbstractDTO< String >
	implements Comparable< ResumoMatriculaDTO >
{

	private static final long serialVersionUID = 5908128223594131759L;
	
	// Propriedades
	public static final String PROPERTY_CAMPUS_STRING = "campusString";
	public static final String PROPERTY_ALUNO_MATRICULA = "alunoMatricula";
	public static final String PROPERTY_CREDITO_DEMANDA_P1 = "credDemandaP1";
	public static final String PROPERTY_CREDITO_ATENDIDOS_P1 = "credAtendidosP1";
	public static final String PROPERTY_CREDITO_NAO_ATENDIDOS_P1 = "credNaoAtendidosP1";
	public static final String PROPERTY_CREDITO_ATENDIDOS_P2 = "credAtendidosP2";
	public static final String PROPERTY_DISCIPLINA_DEMANDA_P1 = "disDemandaP1";
	public static final String PROPERTY_DISCIPLINA_ATENDIDOS_P1 = "disAtendidosP1";
	public static final String PROPERTY_DISCIPLINA_NAO_ATENDIDOS_P1 = "disNaoAtendidosP1";
	public static final String PROPERTY_DISCIPLINA_ATENDIDOS_P2 = "disAtendidosP2";
	public static final String PROPERTY_CREDITO_EXCESSO_P2 = "credExcessoP2";

	public static final String PROPERTY_CODIGO_DISCIPLINA = "codDisciplina";
	public static final String PROPERTY_DISCIPLINA_ATENDIDOS_SOMA = "disAtendidosSoma";
	public static final String PROPERTY_DISCIPLINA_DEMANDA_NAO_ATENDIDA = "disDemandaNaoAtendida"; 
	
	public ResumoMatriculaDTO()
	{
		super();
	}
	
	public void setCampusString(String value) {
		set(PROPERTY_CAMPUS_STRING, value);
	}

	public String getCampusString() {
		return get(PROPERTY_CAMPUS_STRING);
	}
	
	public void setAlunoMatricula(String value)	{
		set( PROPERTY_ALUNO_MATRICULA, value );
	}

	public String getAlunoMatricula() {
		return get( PROPERTY_ALUNO_MATRICULA );
	}

	public void setCredDemandaP1(Integer value) {
		set(PROPERTY_CREDITO_DEMANDA_P1, value);
	}
	
	public Integer getCredDemandaP1(){
		return get(PROPERTY_CREDITO_DEMANDA_P1);
	}
	
	public void setCredAtendidosP1(Integer value) {
		set(PROPERTY_CREDITO_ATENDIDOS_P1, value);
	}
	
	public Integer getCredAtendidosP1(){
		return get(PROPERTY_CREDITO_ATENDIDOS_P1);
	}
	
	public void setCredNaoAtendidosP1(Integer value) {
		set(PROPERTY_CREDITO_NAO_ATENDIDOS_P1, value);
	}
	
	public Integer getCredNaoAtendidosP1(){
		return get(PROPERTY_CREDITO_NAO_ATENDIDOS_P1);
	}
	
	public void setCredAtendidosP2(Integer value) {
		set(PROPERTY_CREDITO_ATENDIDOS_P2, value);
	}
	
	public Integer getCredAtendidosP2(){
		return get(PROPERTY_CREDITO_ATENDIDOS_P2);
	}

	public void setDisDemandaP1(Integer value) {
		set(PROPERTY_DISCIPLINA_DEMANDA_P1, value);
	}
	
	public Integer getDisDemandaP1(){
		return get(PROPERTY_DISCIPLINA_DEMANDA_P1);
	}
	
	public void setDisAtendidosP1(Integer value) {
		set(PROPERTY_DISCIPLINA_ATENDIDOS_P1, value);
	}
	
	public Integer getDisAtendidosP1(){
		return get(PROPERTY_DISCIPLINA_ATENDIDOS_P1);
	}
	
	public void setDisNaoAtendidosP1(Integer value) {
		set(PROPERTY_DISCIPLINA_NAO_ATENDIDOS_P1, value);
	}
	
	public Integer getDisNaoAtendidosP1(){
		return get(PROPERTY_DISCIPLINA_NAO_ATENDIDOS_P1);
	}
	
	public void setDisAtendidosP2(Integer value) {
		set(PROPERTY_DISCIPLINA_ATENDIDOS_P2, value);
	}
	
	public Integer getDisAtendidosP2(){
		return get(PROPERTY_DISCIPLINA_ATENDIDOS_P2);
	}
	
	public void setCredExcessoP2(Integer value) {
		set(PROPERTY_CREDITO_EXCESSO_P2, value);
	}
	
	public Integer getCredExcessoP2(){
		return get(PROPERTY_CREDITO_EXCESSO_P2);
	}
	
	public void setCodDisciplina(String value) {
		set(PROPERTY_CODIGO_DISCIPLINA, value);
	}
	
	public String getCodDisciplina(){
		return get(PROPERTY_CODIGO_DISCIPLINA);
	}
	
	public void setDisDemandaNaoAtendida(Integer value) {
		set(PROPERTY_DISCIPLINA_DEMANDA_NAO_ATENDIDA, value);
	}
	
	public Integer getDisDemandaNaoAtendida(){
		return get(PROPERTY_DISCIPLINA_DEMANDA_NAO_ATENDIDA);
	}
	
	public void setDisAtendidosSoma(Integer value) {
		set(PROPERTY_DISCIPLINA_ATENDIDOS_SOMA, value);
	}
	
	public Integer getDisAtendidosSoma(){
		return get(PROPERTY_DISCIPLINA_ATENDIDOS_SOMA);
	}
	
	@Override
	public int compareTo(ResumoMatriculaDTO o) {
		return getAlunoMatricula().compareTo(o.getAlunoMatricula());
	}

	@Override
	public String getNaturalKey() {
		return getAlunoMatricula();
	}

}
