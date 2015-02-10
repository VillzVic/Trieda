package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;

import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.misc.Semanas;

public class DisponibilidadesDisciplinasImportExcelBean extends AbstractImportExcelBean implements Comparable<DisponibilidadesDisciplinasImportExcelBean> {
	private String disciplinaCodigoStr;
	private String diaStr;
	private String horarioInicialStr;
	private String horarioFinalStr;

	private Disciplina disciplina;
	private Semanas diaSemana;
	private Calendar horarioInicial;
	private Calendar horarioFinal;

	public DisponibilidadesDisciplinasImportExcelBean(int row) {
		super(row);
	}

	public List<ImportExcelError> checkSyntacticErrors() {
		List<ImportExcelError> erros = new ArrayList<ImportExcelError>();

		if (!tudoVazio()) {
			checkMandatoryField(this.disciplinaCodigoStr,ImportExcelError.DISCIPLINA_CODIGO_VAZIO,erros);
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
		return (isEmptyField(this.disciplinaCodigoStr)
				&& isEmptyField(this.diaStr)
				&& isEmptyField(this.horarioInicialStr)
				&& isEmptyField(this.horarioFinalStr));
	}

	public String getDisciplinaCodigoStr() {
		return this.disciplinaCodigoStr;
	}

	public void setDisciplinaCodigoStr(String disciplinaCodigoStr) {
		this.disciplinaCodigoStr = disciplinaCodigoStr;
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

	public Disciplina getDisciplina() {
		return disciplina;
	}

	public void setDisciplina(Disciplina disciplina) {
		this.disciplina = disciplina;
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
	public int compareTo(DisponibilidadesDisciplinasImportExcelBean o) {
		int res = this.getDisciplinaCodigoStr().compareTo(o.getDisciplinaCodigoStr());
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