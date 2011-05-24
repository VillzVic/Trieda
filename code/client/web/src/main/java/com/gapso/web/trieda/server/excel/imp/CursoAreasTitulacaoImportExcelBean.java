package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.Curso;

public class CursoAreasTitulacaoImportExcelBean extends AbstractImportExcelBean implements Comparable<CursoAreasTitulacaoImportExcelBean> {
	
	private String cursoStr;
	private String areaTitulacaoStr;
	
	private Curso curso;
	private AreaTitulacao areaTitulacao;
	
	public CursoAreasTitulacaoImportExcelBean(int row) {
		super(row);
	}
	
	public List<ImportExcelError> checkSyntacticErrors() {
		List<ImportExcelError> erros = new ArrayList<ImportExcelError>();
		if (!tudoVazio()) {
			checkMandatoryField(cursoStr, ImportExcelError.CURSO_AREA_TITULACAO_CURSO_VAZIO,erros);
			checkMandatoryField(areaTitulacaoStr, ImportExcelError.CURSO_AREA_TITULACAO_AREA_VAZIO,erros);
		} else {
			erros.add(ImportExcelError.TUDO_VAZIO);
		}
		return erros;
	}
	
	private boolean tudoVazio() {
		return isEmptyField(cursoStr) && isEmptyField(areaTitulacaoStr);
	}
	
	public String getCursoStr() {
		return cursoStr;
	}
	public void setCursoStr(String cursoStr) {
		this.cursoStr = cursoStr;
	}

	public String getAreaTitulacaoStr() {
		return areaTitulacaoStr;
	}
	public void setAreaTitulacaoStr(String areaTitulacaoStr) {
		this.areaTitulacaoStr = areaTitulacaoStr;
	}

	public Curso getCurso() {
		return curso;
	}
	public void setCurso(Curso curso) {
		this.curso = curso;
	}

	public AreaTitulacao getAreaTitulacao() {
		return areaTitulacao;
	}
	public void setAreaTitulacao(AreaTitulacao areaTitulacao) {
		this.areaTitulacao = areaTitulacao;
	}

	@Override
	public int compareTo(CursoAreasTitulacaoImportExcelBean o) {
		int result = getCursoStr().compareTo(o.getCursoStr());
		if (result == 0) {
			result = getAreaTitulacaoStr().compareTo(o.getAreaTitulacaoStr());
		}
		return result;
	}
	
}
