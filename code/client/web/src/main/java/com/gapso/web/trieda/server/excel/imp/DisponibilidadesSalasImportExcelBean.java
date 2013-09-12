package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;

import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.misc.Semanas;

public class DisponibilidadesSalasImportExcelBean extends AbstractImportExcelBean implements Comparable<DisponibilidadesSalasImportExcelBean> {
	private String salaCodigoStr;
	private String diaStr;
	private String horarioInicialStr;
	private String horarioFinalStr;

	private Sala sala;
	private Semanas diaSemana;
	private Calendar horarioInicial;
	private Calendar horarioFinal;

	public DisponibilidadesSalasImportExcelBean(int row) {
		super(row);
	}

	public List<ImportExcelError> checkSyntacticErrors() {
		List<ImportExcelError> erros = new ArrayList<ImportExcelError>();

		if (!tudoVazio()) {
			checkMandatoryField(this.salaCodigoStr,ImportExcelError.SALA_CODIGO_VAZIO,erros);
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
		return (isEmptyField(this.salaCodigoStr)
				&& isEmptyField(this.diaStr)
				&& isEmptyField(this.horarioInicialStr)
				&& isEmptyField(this.horarioFinalStr));
	}

	public String getSalaCodigoStr() {
		return this.salaCodigoStr;
	}

	public void setSalaCodigoStr(String salaCodigoStr) {
		this.salaCodigoStr = salaCodigoStr;
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

	public Sala getSala() {
		return sala;
	}

	public void setSala(Sala sala) {
		this.sala = sala;
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
	public int compareTo(DisponibilidadesSalasImportExcelBean o) {
		int res = this.getSalaCodigoStr().compareTo(o.getSalaCodigoStr());
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
