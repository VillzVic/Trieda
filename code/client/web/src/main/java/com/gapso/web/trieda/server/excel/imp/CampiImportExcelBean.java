package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.misc.Estados;

public class CampiImportExcelBean extends AbstractImportExcelBean implements Comparable<CampiImportExcelBean> {
	
	private String codigoStr;
	private String nomeStr;
	private String estadoStr;
	private String municipioStr;
	private String bairroStr;
	private String custoMedioCreditoStr;
	
	private Double custoMedioCredito;
	private Estados estado;
	
	public CampiImportExcelBean(int row) {
		super(row);
		this.custoMedioCredito = null;
	}
	
	public List<ImportExcelError> checkSyntacticErrors() {
		List<ImportExcelError> erros = new ArrayList<ImportExcelError>();
		if (!tudoVazio()) {
			checkMandatoryField(codigoStr,ImportExcelError.CAMPUS_CODIGO_VAZIO,erros);
			checkMandatoryField(nomeStr,ImportExcelError.CAMPUS_NOME_VAZIO,erros);
			checkMandatoryField(custoMedioCreditoStr,ImportExcelError.CAMPUS_CUSTO_MEDIO_CREDITO_VAZIO,erros);
			custoMedioCredito = checkNonNegativeDoubleField(custoMedioCreditoStr,ImportExcelError.CAMPUS_CUSTO_MEDIO_CREDITO_FORMATO_INVALIDO,ImportExcelError.CAMPUS_CUSTO_MEDIO_CREDITO_VALOR_NEGATIVO,erros);
			verificaEstado(erros);
		} else {
			erros.add(ImportExcelError.TUDO_VAZIO);
		}
		return erros;
	}
	
	private boolean tudoVazio() {
		return isEmptyField(codigoStr) && isEmptyField(nomeStr) && isEmptyField(estadoStr) && isEmptyField(municipioStr) && isEmptyField(bairroStr) && isEmptyField(custoMedioCreditoStr);
	}
	
	private void verificaEstado(List<ImportExcelError> erros) {
		try {
			estado = Estados.valueOf(estadoStr);
		} catch (Exception e) {
			erros.add(ImportExcelError.CAMPUS_ESTADO_VALOR_INVALIDO);
		}
	}
	
	public String getCodigoStr() {
		return codigoStr;
	}

	public void setCodigoStr(String codigoStr) {
		this.codigoStr = codigoStr;
	}

	public String getNomeStr() {
		return nomeStr;
	}

	public void setNomeStr(String nomeStr) {
		this.nomeStr = nomeStr;
	}

	public String getEstadoStr() {
		return estadoStr;
	}

	public void setEstadoStr(String estadoStr) {
		this.estadoStr = estadoStr;
	}

	public String getMunicipioStr() {
		return municipioStr;
	}

	public void setMunicipioStr(String municipioStr) {
		this.municipioStr = municipioStr;
	}

	public String getBairroStr() {
		return bairroStr;
	}

	public void setBairroStr(String bairroStr) {
		this.bairroStr = bairroStr;
	}

	public String getCustoMedioCreditoStr() {
		return custoMedioCreditoStr;
	}

	public void setCustoMedioCreditoStr(String custoMedioCreditoStr) {
		this.custoMedioCreditoStr = custoMedioCreditoStr;
	}

	public Double getCustoMedioCredito() {
		return custoMedioCredito;
	}

	public Estados getEstado() {
		return estado;
	}

	@Override
	public int compareTo(CampiImportExcelBean o) {
		return 0;
	}
}