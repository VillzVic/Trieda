package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;

import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.misc.Semanas;

public class DisponibilidadesProfessoresImportExcelBean extends AbstractImportExcelBean implements Comparable<DisponibilidadesProfessoresImportExcelBean> {
	private String cpfStr;
	private String diaStr;
	private String horarioInicialStr;
	private String horarioFinalStr;

	private Professor professor;
	private Semanas diaSemana;
	private Calendar horarioInicial;
	private Calendar horarioFinal;

	public DisponibilidadesProfessoresImportExcelBean(int row) {
		super(row);
	}

	public List<ImportExcelError> checkSyntacticErrors() {
		List<ImportExcelError> erros = new ArrayList<ImportExcelError>();

		if (!tudoVazio()) {
			checkMandatoryField(this.cpfStr,ImportExcelError.PROFESSOR_CPF_VAZIO,erros);
			checkMandatoryField(this.diaStr,ImportExcelError.DISPONIBILIDADE_DIA_SEMANA_VAZIO,erros);
			checkMandatoryField(this.horarioInicialStr,ImportExcelError.DISPONIBILIDADE_HORARIO_INICIAL_VAZIO,erros);
			checkMandatoryField(this.horarioFinalStr,ImportExcelError.DISPONIBILIDADE_HORARIO_FINAL_VAZIO,erros);

			this.diaSemana = checkEnumField(diaStr,Semanas.class,ImportExcelError.DISPONIBILIDADE_DIA_SEMANA_VALOR_INVALIDO,erros);
			this.horarioInicial = checkTimeField(this.horarioInicialStr,ImportExcelError.DISPONIBILIDADE_HORARIO_INICIAL_FORMATO_INVALIDO,erros);
			this.horarioFinal = checkTimeField(this.horarioFinalStr,ImportExcelError.DISPONIBILIDADE_HORARIO_FINAL_FORMATO_INVALIDO,erros);
		} else {
			erros.add(ImportExcelError.TUDO_VAZIO);
		}

		return erros;
	}

	private boolean tudoVazio() {
		return (isEmptyField(this.cpfStr)
				&& isEmptyField(this.diaStr)
				&& isEmptyField(this.horarioInicialStr)
				&& isEmptyField(this.horarioFinalStr));
	}

	public String getCpfStr() {
		return this.cpfStr;
	}

	public void setCpfStr(String cpfStr) {
		this.cpfStr = cpfStr;
	}

	public String getDiaStr() {
		return diaStr;
	}

	public void setDiaStr(String diaStr) {
		this.diaStr = diaStr;
	}

	public String getHorarioInicialStr() {
		return horarioInicialStr;
	}

	public void setHorarioInicialStr(String horarioInicialStr) {
		this.horarioInicialStr = horarioInicialStr;
	}

	public String getHorarioFinalStr() {
		return horarioFinalStr;
	}

	public void setHorarioFinalStr(String horarioFinalStr) {
		this.horarioFinalStr = horarioFinalStr;
	}

	public Professor getProfessor() {
		return professor;
	}

	public void setProfessor(Professor professor) {
		this.professor = professor;
	}

	public Calendar getHorarioInicial() {
		return horarioInicial;
	}

	public Calendar getHorarioFinal() {
		return horarioFinal;
	}

	public Semanas getDiaSemana() {
		return diaSemana;
	}

	@Override
	public int compareTo(DisponibilidadesProfessoresImportExcelBean o) {
		int res = this.getCpfStr().compareTo(o.getCpfStr());
		if (res == 0) {
			res = this.getDiaStr().compareTo(o.getDiaStr());
			if (res == 0) {
				res = this.getHorarioInicialStr().compareTo(o.getHorarioInicialStr());
				if (res == 0) {
					res = this.getHorarioFinalStr().compareTo(o.getHorarioFinalStr());
				}
			}
		}
		
		return res;
	}
}
