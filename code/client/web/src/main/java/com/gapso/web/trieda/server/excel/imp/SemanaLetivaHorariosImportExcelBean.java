package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;

import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;

public class SemanaLetivaHorariosImportExcelBean
extends AbstractImportExcelBean implements Comparable<SemanaLetivaHorariosImportExcelBean> {
	private String SemanaLetivaCodigoStr;
	private String turnoNomeStr;
	private String horarioStr;
	private String segundaStr;
	private String tercaStr;
	private String quartaStr;
	private String quintaStr;
	private String sextaStr;
	private String sabadoStr;
	private String domingoStr;

	private SemanaLetiva semanaLetiva;
	private Turno turno;
	private Calendar horario;
	private Boolean segunda;
	private Boolean terca;
	private Boolean quarta;
	private Boolean quinta;
	private Boolean sexta;
	private Boolean sabado;
	private Boolean domingo;

	public SemanaLetivaHorariosImportExcelBean(int row) {
		super(row);
	}

	public List<ImportExcelError> checkSyntacticErrors() {
		List<ImportExcelError> erros = new ArrayList<ImportExcelError>();

		if (!tudoVazio()) {
			checkMandatoryField(this.getSemanaLetivaCodigoStr(),ImportExcelError.SEMANA_LETIVA_CODIGO_VAZIO,erros);
			checkMandatoryField(this.getTurnoNomeStr(),ImportExcelError.SEMANA_LETIVA_TURNO_NOME_VAZIO,erros);
			checkMandatoryField(this.getHorarioStr(),ImportExcelError.SEMANA_LETIVA_HORARIO_VAZIO,erros);
			checkMandatoryField(this.getSegundaStr(),ImportExcelError.SEMANA_LETIVA_SEGUNDA_VAZIO,erros);
			checkMandatoryField(this.getTercaStr(),ImportExcelError.SEMANA_LETIVA_TERCA_VAZIO,erros);
			checkMandatoryField(this.getQuartaStr(),ImportExcelError.SEMANA_LETIVA_QUARTA_VAZIO,erros);
			checkMandatoryField(this.getQuintaStr(),ImportExcelError.SEMANA_LETIVA_QUINTA_VAZIO,erros);
			checkMandatoryField(this.getSextaStr(),ImportExcelError.SEMANA_LETIVA_SEXTA_VAZIO,erros);
			checkMandatoryField(this.getSabadoStr(),ImportExcelError.SEMANA_LETIVA_SABADO_VAZIO,erros);
			checkMandatoryField(this.getDomingoStr(),ImportExcelError.SEMANA_LETIVA_DOMINGO_VAZIO,erros);

			this.setHorario(checkTimeField(this.getHorarioStr(),ImportExcelError.SEMANA_LETIVA_HORARIO_FORMATO_INVALIDO,erros));
			setSegunda(checkBooleanField( getSegundaStr(), ImportExcelError.SEMANA_LETIVA_SEGUNDA_FORMATO_INVALIDO, erros ));
			setTerca(checkBooleanField( getTercaStr(), ImportExcelError.SEMANA_LETIVA_TERCA_FORMATO_INVALIDO, erros ));
			setQuarta(checkBooleanField( getQuartaStr(), ImportExcelError.SEMANA_LETIVA_QUARTA_FORMATO_INVALIDO, erros ));
			setQuinta(checkBooleanField( getQuintaStr(), ImportExcelError.SEMANA_LETIVA_QUINTA_FORMATO_INVALIDO, erros ));
			setSexta(checkBooleanField( getSextaStr(), ImportExcelError.SEMANA_LETIVA_SEXTA_FORMATO_INVALIDO, erros ));
			setSabado(checkBooleanField( getSabadoStr(), ImportExcelError.SEMANA_LETIVA_SABADO_FORMATO_INVALIDO, erros ));
			setDomingo(checkBooleanField( getDomingoStr(), ImportExcelError.SEMANA_LETIVA_DOMINGO_FORMATO_INVALIDO, erros ));
		} else {
			erros.add(ImportExcelError.TUDO_VAZIO);
		}

		return erros;
	}

	private boolean tudoVazio() {
		return (isEmptyField(this.getSemanaLetivaCodigoStr())
				&& isEmptyField(this.getTurnoNomeStr())
				&& isEmptyField(this.getHorarioStr())
				&& isEmptyField(this.getSegundaStr())
				&& isEmptyField(this.getTercaStr())
				&& isEmptyField(this.getQuartaStr())
				&& isEmptyField(this.getQuintaStr())
				&& isEmptyField(this.getSextaStr())
				&& isEmptyField(this.getSabadoStr())
				&& isEmptyField(this.getDomingoStr()));
	}

	@Override
	public int compareTo(SemanaLetivaHorariosImportExcelBean o) {
		int res = this.getSemanaLetivaCodigoStr().compareTo(o.getSemanaLetivaCodigoStr());
		if (res == 0) {
			res = this.getTurnoNomeStr().compareTo(o.getTurnoNomeStr());
			if (res == 0) {
				res = this.getHorarioStr().compareTo(o.getHorarioStr());
			}
		}
		
		return res;
	}

	public String getSemanaLetivaCodigoStr() {
		return SemanaLetivaCodigoStr;
	}

	public void setSemanaLetivaCodigoStr(String semanaLetivaCodigoStr) {
		SemanaLetivaCodigoStr = semanaLetivaCodigoStr;
	}

	public String getTurnoNomeStr() {
		return turnoNomeStr;
	}

	public void setTurnoNomeStr(String turnoNomeStr) {
		this.turnoNomeStr = turnoNomeStr;
	}

	public String getHorarioStr() {
		return horarioStr;
	}

	public void setHorarioStr(String horarioStr) {
		this.horarioStr = horarioStr;
	}

	public String getSegundaStr() {
		return segundaStr;
	}

	public void setSegundaStr(String segundaStr) {
		this.segundaStr = segundaStr;
	}

	public String getTercaStr() {
		return tercaStr;
	}

	public void setTercaStr(String tercaStr) {
		this.tercaStr = tercaStr;
	}

	public String getQuartaStr() {
		return quartaStr;
	}

	public void setQuartaStr(String quartaStr) {
		this.quartaStr = quartaStr;
	}

	public String getQuintaStr() {
		return quintaStr;
	}

	public void setQuintaStr(String quintaStr) {
		this.quintaStr = quintaStr;
	}

	public String getSextaStr() {
		return sextaStr;
	}

	public void setSextaStr(String sextaStr) {
		this.sextaStr = sextaStr;
	}

	public String getSabadoStr() {
		return sabadoStr;
	}

	public void setSabadoStr(String sabadoStr) {
		this.sabadoStr = sabadoStr;
	}

	public String getDomingoStr() {
		return domingoStr;
	}

	public void setDomingoStr(String domingoStr) {
		this.domingoStr = domingoStr;
	}

	public SemanaLetiva getSemanaLetiva() {
		return semanaLetiva;
	}

	public void setSemanaLetiva(SemanaLetiva semanaLetiva) {
		this.semanaLetiva = semanaLetiva;
	}

	public Turno getTurno() {
		return turno;
	}

	public void setTurno(Turno turno) {
		this.turno = turno;
	}

	public Calendar getHorario() {
		return horario;
	}

	public void setHorario(Calendar horario) {
		this.horario = horario;
	}

	public Boolean getSegunda() {
		return segunda;
	}

	public void setSegunda(Boolean segunda) {
		this.segunda = segunda;
	}

	public Boolean getTerca() {
		return terca;
	}

	public void setTerca(Boolean terca) {
		this.terca = terca;
	}

	public Boolean getQuarta() {
		return quarta;
	}

	public void setQuarta(Boolean quarta) {
		this.quarta = quarta;
	}

	public Boolean getQuinta() {
		return quinta;
	}

	public void setQuinta(Boolean quinta) {
		this.quinta = quinta;
	}

	public Boolean getSexta() {
		return sexta;
	}

	public void setSexta(Boolean sexta) {
		this.sexta = sexta;
	}

	public Boolean getSabado() {
		return sabado;
	}

	public void setSabado(Boolean sabado) {
		this.sabado = sabado;
	}

	public Boolean getDomingo() {
		return domingo;
	}

	public void setDomingo(Boolean domingo) {
		this.domingo = domingo;
	}
}
