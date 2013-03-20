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

	public void setCredDemandaP1(int value) {
		set(PROPERTY_CREDITO_DEMANDA_P1, value);
	}
	
	public int getCredDemandaP1(){
		return get(PROPERTY_CREDITO_DEMANDA_P1);
	}
	
	public void setCredAtendidosP1(int value) {
		set(PROPERTY_CREDITO_ATENDIDOS_P1, value);
	}
	
	public int getCredAtendidosP1(){
		return get(PROPERTY_CREDITO_ATENDIDOS_P1);
	}
	
	public void setCredNaoAtendidosP1(int value) {
		set(PROPERTY_CREDITO_NAO_ATENDIDOS_P1, value);
	}
	
	public int getCredNaoAtendidosP1(){
		return get(PROPERTY_CREDITO_NAO_ATENDIDOS_P1);
	}
	
	public void setCredAtendidosP2(int value) {
		set(PROPERTY_CREDITO_ATENDIDOS_P2, value);
	}
	
	public int getCredAtendidosP2(){
		return get(PROPERTY_CREDITO_ATENDIDOS_P2);
	}

	public void setDisDemandaP1(int value) {
		set(PROPERTY_DISCIPLINA_DEMANDA_P1, value);
	}
	
	public int getDisDemandaP1(){
		return get(PROPERTY_DISCIPLINA_DEMANDA_P1);
	}
	
	public void setDisAtendidosP1(int value) {
		set(PROPERTY_DISCIPLINA_ATENDIDOS_P1, value);
	}
	
	public int getDisAtendidosP1(){
		return get(PROPERTY_DISCIPLINA_ATENDIDOS_P1);
	}
	
	public void setDisNaoAtendidosP1(int value) {
		set(PROPERTY_DISCIPLINA_NAO_ATENDIDOS_P1, value);
	}
	
	public int getDisNaoAtendidosP1(){
		return get(PROPERTY_DISCIPLINA_NAO_ATENDIDOS_P1);
	}
	
	public void setDisAtendidosP2(int value) {
		set(PROPERTY_DISCIPLINA_ATENDIDOS_P2, value);
	}
	
	public int getDisAtendidosP2(){
		return get(PROPERTY_DISCIPLINA_ATENDIDOS_P2);
	}
	
	public void setCredExcessoP2(int value) {
		set(PROPERTY_CREDITO_EXCESSO_P2, value);
	}
	
	public int getCredExcessoP2(){
		return get(PROPERTY_CREDITO_EXCESSO_P2);
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
