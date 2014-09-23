package com.gapso.web.trieda.shared.dtos;

public class EstatisticasInputSolverXMLDTO extends AbstractDTO< String > implements Comparable< EstatisticasInputSolverXMLDTO > {

	private static final long serialVersionUID = 1L;
	
	// Propriedades
	public static final String PROPERTY_REQ_OTM_ROUND = "reqOtmRound";
	public static final String PROPERTY_TOTAL_CAMPI = "totalCampi";
	public static final String PROPERTY_CAMPI_SELECIONADOS = "campiSelecionados";
	public static final String PROPERTY_TOTAL_TURNOS = "totalTurnos";
	public static final String PROPERTY_TURNOS_SELECIONADOS = "turnosSelecionados";
	public static final String PROPERTY_TOTAL_ALUNOS = "totalAlunos";
	public static final String PROPERTY_TOTAL_ALUNODEMANDA_P1 = "totalAlunosDemandasP1";
	public static final String PROPERTY_TOTAL_ALUNODEMANDA_P2 = "totalAlunosDemandasP2";
	public static final String PROPERTY_TOTAL_AMBIENTES = "totalAmbientes";
	public static final String PROPERTY_TOTAL_PROFESSORES = "totalProfessores";
	
	public EstatisticasInputSolverXMLDTO() {
		super();
	}
	
	public void setReqOtmRound(Long value) {
		set(PROPERTY_REQ_OTM_ROUND, value);
	}
	public Long getReqOtmRound() {
		return get(PROPERTY_REQ_OTM_ROUND);
	}
	
	public void setTotalCampi(Integer value) {
		set(PROPERTY_TOTAL_CAMPI, value);
	}
	public Integer getTotalCampi() {
		return get(PROPERTY_TOTAL_CAMPI);
	}
	
	public void setCampiSelecionados(String value) {
		set(PROPERTY_CAMPI_SELECIONADOS, value);
	}
	public String getCampiSelecionados() {
		return get(PROPERTY_CAMPI_SELECIONADOS);
	}
	
	public void setTotalTurnos(Integer value) {
		set(PROPERTY_TOTAL_TURNOS, value);
	}
	public Integer getTotalTurnos() {
		return get(PROPERTY_TOTAL_TURNOS);
	}
	
	public void setTurnosSelecionados(String value) {
		set(PROPERTY_TURNOS_SELECIONADOS, value);
	}
	public String getTurnosSelecionados() {
		return get(PROPERTY_TURNOS_SELECIONADOS);
	}
	
	public void setTotalAlunos(Integer value) {
		set(PROPERTY_TOTAL_ALUNOS, value);
	}
	public Integer getTotalAlunos() {
		return get(PROPERTY_TOTAL_ALUNOS);
	}
	
	public void setTotalAlunosDemandasP1(Integer value) {
		set(PROPERTY_TOTAL_ALUNODEMANDA_P1, value);
	}
	public Integer getTotalAlunosDemandasP1() {
		return get(PROPERTY_TOTAL_ALUNODEMANDA_P1);
	}
	
	public void setTotalAlunosDemandasP2(Integer value) {
		set(PROPERTY_TOTAL_ALUNODEMANDA_P2, value);
	}
	public Integer getTotalAlunosDemandasP2() {
		return get(PROPERTY_TOTAL_ALUNODEMANDA_P2);
	}
	
	public void setTotalAmbientes(Integer value) {
		set(PROPERTY_TOTAL_AMBIENTES, value);
	}
	public Integer getTotalAmbientes() {
		return get(PROPERTY_TOTAL_AMBIENTES);
	}
	
	public void setTotalProfessores(Integer value) {
		set(PROPERTY_TOTAL_PROFESSORES, value);
	}
	public Integer getTotalProfessores() {
		return get(PROPERTY_TOTAL_PROFESSORES);
	}

	@Override
	public int compareTo(EstatisticasInputSolverXMLDTO o) {
		return getReqOtmRound().compareTo(o.getReqOtmRound());
	}

	@Override
	public String getNaturalKey() {
		return getReqOtmRound().toString();
	}
}