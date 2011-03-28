package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.domain.TipoSala;
import com.gapso.trieda.domain.Unidade;

public class SalasImportExcelBean extends AbstractImportExcelBean implements Comparable<SalasImportExcelBean> {
	
	private String codigoStr;
	private String codigoUnidadeStr;
	private String tipoStr;
	private String numeroStr;
	private String andarStr;
	private String capacidadeStr;
	
	private TipoSala tipo;
	private Unidade unidade;
	private Integer capacidade;
	
	public SalasImportExcelBean(int row) {
		super(row);
	}
	
	public List<ImportExcelError> checkSyntacticErrors() {
		List<ImportExcelError> erros = new ArrayList<ImportExcelError>();
		if (!tudoVazio()) {
			checkMandatoryField(codigoStr,ImportExcelError.SALA_CODIGO_VAZIO,erros);
			checkMandatoryField(codigoUnidadeStr,ImportExcelError.SALA_UNIDADE_VAZIO,erros);
			checkMandatoryField(tipoStr,ImportExcelError.SALA_TIPO_VAZIO,erros);
			checkMandatoryField(numeroStr,ImportExcelError.SALA_NUMERO_VAZIO,erros);
			checkMandatoryField(andarStr,ImportExcelError.SALA_ANDAR_VAZIO,erros);
			checkMandatoryField(capacidadeStr,ImportExcelError.SALA_CAPACIDADE_VAZIO,erros);
			capacidade = checkNonNegativeIntegerField(capacidadeStr,ImportExcelError.SALA_CAPACIDADE_FORMATO_INVALIDO,ImportExcelError.SALA_CAPACIDADE_VALOR_NEGATIVO,erros);
		} else {
			erros.add(ImportExcelError.TUDO_VAZIO);
		}
		return erros;
	}
	
	private boolean tudoVazio() {
		return isEmptyField(codigoStr) && isEmptyField(codigoUnidadeStr) && isEmptyField(tipoStr) && isEmptyField(numeroStr) && isEmptyField(andarStr)&& isEmptyField(capacidadeStr);
	}

	public String getCodigoStr() {
		return codigoStr;
	}

	public void setCodigoStr(String codigoStr) {
		this.codigoStr = codigoStr;
	}

	public String getCodigoUnidadeStr() {
		return codigoUnidadeStr;
	}

	public void setCodigoUnidadeStr(String codigoUnidadeStr) {
		this.codigoUnidadeStr = codigoUnidadeStr;
	}

	public String getTipoStr() {
		return tipoStr;
	}

	public void setTipoStr(String tipoStr) {
		this.tipoStr = tipoStr;
	}

	public String getNumeroStr() {
		return numeroStr;
	}

	public void setNumeroStr(String numeroStr) {
		this.numeroStr = numeroStr;
	}

	public String getAndarStr() {
		return andarStr;
	}

	public void setAndarStr(String andarStr) {
		this.andarStr = andarStr;
	}

	public String getCapacidadeStr() {
		return capacidadeStr;
	}

	public void setCapacidadeStr(String capacidadeStr) {
		this.capacidadeStr = capacidadeStr;
	}

	public Unidade getUnidade() {
		return unidade;
	}

	public void setUnidade(Unidade unidade) {
		this.unidade = unidade;
	}

	public TipoSala getTipo() {
		return tipo;
	}

	public void setTipo(TipoSala tipo) {
		this.tipo = tipo;
	}

	public Integer getCapacidade() {
		return capacidade;
	}

	@Override
	public int compareTo(SalasImportExcelBean o) {
		return getCodigoStr().compareTo(o.getCodigoStr());
	}
}