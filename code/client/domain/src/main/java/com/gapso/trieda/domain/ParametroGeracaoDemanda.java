package com.gapso.trieda.domain;

import java.io.Serializable;

public class ParametroGeracaoDemanda 
	implements Serializable
{
	
	/**
	 * 
	 */
	private static final long serialVersionUID = -618856341674079286L;
	
	private Turno turno;
	
	private Campus campus;

	private boolean considerarPreRequisitos = true;
	
	private boolean considerarCoRequisitos = true;
	
	private boolean usarDemandasDePrioridade2 = true;
	
	private Integer distanciaMaxEmPeriodosParaPrioridade2 = 2;
	
	private boolean maxCreditosPeriodo = false;
	
	private Integer maxCreditosManual = 20;
	
	private boolean aumentaMaxCreditosParaAlunosComDisciplinasAtrasadas = true;
	
	private Integer fatorDeAumentoDeMaxCreditos = 10;

	public boolean getConsiderarPreRequisitos() {
		return considerarPreRequisitos;
	}

	public void setConsiderarPreRequisitos(boolean considerarPreRequisitos) {
		this.considerarPreRequisitos = considerarPreRequisitos;
	}

	public boolean getConsiderarCoRequisitos() {
		return considerarCoRequisitos;
	}

	public void setConsiderarCoRequisitos(boolean considerarCoRequisitos) {
		this.considerarCoRequisitos = considerarCoRequisitos;
	}

	public Integer getDistanciaMaxEmPeriodosParaPrioridade2() {
		return distanciaMaxEmPeriodosParaPrioridade2;
	}

	public void setDistanciaMaxEmPeriodosParaPrioridade2(
			Integer distanciaMaxEmPeriodosParaPrioridade2) {
		this.distanciaMaxEmPeriodosParaPrioridade2 = distanciaMaxEmPeriodosParaPrioridade2;
	}

	public boolean getMaxCreditosPeriodo() {
		return maxCreditosPeriodo;
	}

	public void setMaxCreditosPeriodo(boolean maxCreditosPeriodo) {
		this.maxCreditosPeriodo = maxCreditosPeriodo;
	}

	public Integer getMaxCreditosManual() {
		return maxCreditosManual;
	}

	public void setMaxCreditosManual(Integer maxCreditosManual) {
		this.maxCreditosManual = maxCreditosManual;
	}

	public boolean getAumentaMaxCreditosParaAlunosComDisciplinasAtrasadas() {
		return aumentaMaxCreditosParaAlunosComDisciplinasAtrasadas;
	}

	public void setAumentaMaxCreditosParaAlunosComDisciplinasAtrasadas(
			boolean aumentaMaxCreditosParaAlunosComDisciplinasAtrasadas) {
		this.aumentaMaxCreditosParaAlunosComDisciplinasAtrasadas = aumentaMaxCreditosParaAlunosComDisciplinasAtrasadas;
	}

	public Integer getFatorDeAumentoDeMaxCreditos() {
		return fatorDeAumentoDeMaxCreditos;
	}

	public void setFatorDeAumentoDeMaxCreditos(Integer fatorDeAumentoDeMaxCreditos) {
		this.fatorDeAumentoDeMaxCreditos = fatorDeAumentoDeMaxCreditos;
	}

	public boolean getUsarDemandasDePrioridade2() {
		return usarDemandasDePrioridade2;
	}

	public void setUsarDemandasDePrioridade2(boolean usarDemandasDePrioridade2) {
		this.usarDemandasDePrioridade2 = usarDemandasDePrioridade2;
	}

	public Turno getTurno() {
		return turno;
	}

	public void setTurno(Turno turno) {
		this.turno = turno;
	}

	public Campus getCampus() {
		return campus;
	}

	public void setCampus(Campus campus) {
		this.campus = campus;
	}
}