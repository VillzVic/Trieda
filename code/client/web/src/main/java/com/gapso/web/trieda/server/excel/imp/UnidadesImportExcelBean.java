package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.domain.Campus;

public class UnidadesImportExcelBean extends AbstractImportExcelBean implements Comparable<UnidadesImportExcelBean> {
	
	private String codigoStr;
	private String nomeStr;
	private String codigoCampusStr;
	
	private Campus campus;
	
	public UnidadesImportExcelBean(int row) {
		super(row);
	}
	
	public List<ImportExcelError> checkSyntacticErrors() {
		List<ImportExcelError> erros = new ArrayList<ImportExcelError>();
		if (!tudoVazio()) {
			checkMandatoryField(codigoStr,ImportExcelError.UNIDADE_CODIGO_VAZIO,erros);
			checkMandatoryField(nomeStr,ImportExcelError.UNIDADE_NOME_VAZIO,erros);
			checkMandatoryField(codigoCampusStr,ImportExcelError.UNIDADE_CAMPUS_VAZIO,erros);
		} else {
			erros.add(ImportExcelError.TUDO_VAZIO);
		}
		return erros;
	}
	
	private boolean tudoVazio() {
		return isEmptyField(codigoStr) && isEmptyField(nomeStr) && isEmptyField(codigoCampusStr);
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

	public String getCodigoCampusStr() {
		return codigoCampusStr;
	}

	public void setCodigoCampusStr(String codigoCampusStr) {
		this.codigoCampusStr = codigoCampusStr;
	}

	public Campus getCampus() {
		return campus;
	}

	public void setCampus(Campus campus) {
		this.campus = campus;
	}

	@Override
	public int compareTo(UnidadesImportExcelBean o) {
		return getCodigoStr().compareTo(o.getCodigoStr());
	}
}
