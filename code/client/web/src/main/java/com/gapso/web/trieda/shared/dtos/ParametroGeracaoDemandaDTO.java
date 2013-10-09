package com.gapso.web.trieda.shared.dtos;

public class ParametroGeracaoDemandaDTO extends AbstractDTO<Long>
	implements Comparable<ParametroDTO>
{

	/**
	 * 
	 */
	private static final long serialVersionUID = -2340670299105189293L;

	public static final String PROPERTY_CAMPUS_ID = "campusId";
	public static final String PROPERTY_TURNO_ID = "turnoId";
	public static final String PROPERTY_USARDEMANDASDEPRIORIDADE2 = "usarDemandasDePrioridade2";
	public static final String PROPERTY_DISTANCIAMAXEMPERIODOSPARAPRIORIDADE2 = "distanciaMaxEmPeriodosParaPrioridade2";
	public static final String PROPERTY_CONSIDERARPREREQUISITOS = "considerarPreRequisitos";
	public static final String PROPERTY_CONSIDERARCOREQUISITOS = "considerarCoRequisitos";
	public static final String PROPERTY_MAXCREDITOSPERIODO = "maxCreditosPeriodo";
	public static final String PROPERTY_MAXCREDITOSMANUAL = "maxCreditosManual";
	public static final String PROPERTY_AUMENTAMAXCREDITOSPARAALUNOSCOMDISCIPLINASATRASADAS = "aumentaMaxCreditosParaAlunosComDisciplinasAtrasadas";
	public static final String PROPERTY_FATORDEAUMENTODEMAXCREDITOS = "fatorDeAumentoDeMaxCreditos";
	
	public ParametroGeracaoDemandaDTO() {
		super();
	}
	
	public Long getCampusId() {
		return get(PROPERTY_CAMPUS_ID);
	}
	public void setCampusId(Long value) {
		set(PROPERTY_CAMPUS_ID, value);
	}
	
	public Long getTurnoId() {
		return get(PROPERTY_TURNO_ID);
	}
	public void setTurnoId(Long value) {
		set(PROPERTY_TURNO_ID, value);
	}

	public Boolean getUsarDemandasDePrioridade2() {
		return get(PROPERTY_USARDEMANDASDEPRIORIDADE2);
	}
	public void setUsarDemandasDePrioridade2(Boolean value) {
		set(PROPERTY_USARDEMANDASDEPRIORIDADE2, value);
	}
	
	public Integer getDistanciaMaxEmPeriodosParaPrioridade2() {
		return get(PROPERTY_DISTANCIAMAXEMPERIODOSPARAPRIORIDADE2);
	}
	public void setDistanciaMaxEmPeriodosParaPrioridade2(Integer value) {
		set(PROPERTY_DISTANCIAMAXEMPERIODOSPARAPRIORIDADE2, value);
	}
	
	public Boolean getConsiderarPreRequisitos() {
		return get(PROPERTY_CONSIDERARPREREQUISITOS);
	}
	public void setConsiderarPreRequisitos(Boolean value) {
		set(PROPERTY_CONSIDERARPREREQUISITOS, value);
	}
	
	public Boolean getConsiderarCoRequisitos() {
		return get(PROPERTY_CONSIDERARCOREQUISITOS);
	}
	public void setConsiderarCoRequisitos(Boolean value) {
		set(PROPERTY_CONSIDERARCOREQUISITOS, value);
	}
	
	public Boolean getMaxCreditosPeriodo() {
		return get(PROPERTY_MAXCREDITOSPERIODO);
	}
	public void setMaxCreditosPeriodo(Boolean value) {
		set(PROPERTY_MAXCREDITOSPERIODO, value);
	}
	
	public Integer getMaxCreditosManual() {
		return get(PROPERTY_MAXCREDITOSMANUAL);
	}
	public void setMaxCreditosManual(Integer value) {
		set(PROPERTY_MAXCREDITOSMANUAL, value);
	}
	
	public Boolean getAumentaMaxCreditosParaAlunosComDisciplinasAtrasadas() {
		return get(PROPERTY_AUMENTAMAXCREDITOSPARAALUNOSCOMDISCIPLINASATRASADAS);
	}
	public void setAumentaMaxCreditosParaAlunosComDisciplinasAtrasadas(Boolean value) {
		set(PROPERTY_AUMENTAMAXCREDITOSPARAALUNOSCOMDISCIPLINASATRASADAS, value);
	}
	
	public Integer getFatorDeAumentoDeMaxCreditos() {
		return get(PROPERTY_FATORDEAUMENTODEMAXCREDITOS);
	}
	public void setFatorDeAumentoDeMaxCreditos(Integer value) {
		set(PROPERTY_FATORDEAUMENTODEMAXCREDITOS, value);
	}

	@Override
	public int compareTo(ParametroDTO arg0) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public Long getNaturalKey() {
		// TODO Auto-generated method stub
		return null;
	}
	
}
