package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

public class CampiImportExcelBean implements Comparable<CampiImportExcelBean> {
	
	private int row;
	
	private String codigoStr;
	private String nomeStr;
	private String estadoStr;
	private String municipioStr;
	private String bairroStr;
	private String custoMedioCreditoStr;
	
	public CampiImportExcelBean(int row) {
		this.row = row;
	}
	
	public List<ImportExcelError> checkSyntacticErrors() {
		List<ImportExcelError> erros = new ArrayList<ImportExcelError>();
		if (!tudoVazio()) {
//			verificaAno(erros);
		} else {
			erros.add(ImportExcelError.TUDO_VAZIO);
		}
		return erros;
	}
	
	private boolean tudoVazio() {
		return vazio(codigoStr) && vazio(nomeStr) && vazio(estadoStr) && vazio(municipioStr) && vazio(bairroStr) && vazio(custoMedioCreditoStr);
	}
	
	private boolean vazio(String value) {
		return value == null || value.equals("");
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

	public int getRow() {
		return row;
	}

	@Override
	public int compareTo(CampiImportExcelBean o) {
		return 0;
	}
}