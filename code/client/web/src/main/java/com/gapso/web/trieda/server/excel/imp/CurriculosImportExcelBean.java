package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Disciplina;

public class CurriculosImportExcelBean extends AbstractImportExcelBean implements Comparable<CurriculosImportExcelBean> {
	
	private String codigoStr;
	private String descricaoStr;
	private String cursoCodigoStr;
	private String periodoStr;
	private String disciplinaCodigoStr;
	
	private Curso curso;
	private Integer periodo;
	private Disciplina disciplina;
	
	public CurriculosImportExcelBean(int row) {
		super(row);
	}
	
	public List<ImportExcelError> checkSyntacticErrors() {
		List<ImportExcelError> erros = new ArrayList<ImportExcelError>();
		if (!tudoVazio()) {
			checkMandatoryField(codigoStr,ImportExcelError.CURRICULO_CODIGO_VAZIO,erros);
			checkMandatoryField(descricaoStr,ImportExcelError.CURRICULO_DESCRICAO_VAZIO,erros);
			checkMandatoryField(cursoCodigoStr,ImportExcelError.CURRICULO_CURSO_VAZIO,erros);
			checkMandatoryField(periodoStr,ImportExcelError.CURRICULO_PERIODO_VAZIO,erros);
			checkMandatoryField(disciplinaCodigoStr,ImportExcelError.CURRICULO_DISCIPLINA_VAZIO,erros);
			periodo = checkNonNegativeIntegerField(periodoStr,ImportExcelError.CURRICULO_PERIODO_FORMATO_INVALIDO,ImportExcelError.CURRICULO_PERIODO_VALOR_NEGATIVO,erros);
		} else {
			erros.add(ImportExcelError.TUDO_VAZIO);
		}
		return erros;
	}
	
	public boolean tudoVazio() {
		return isEmptyField(codigoStr) && 
			   isEmptyField(descricaoStr) && 
			   isEmptyField(cursoCodigoStr) && 
			   isEmptyField(periodoStr) && 
			   isEmptyField(disciplinaCodigoStr);
	}
	
	public String getNaturalKeyString() {
		return getCursoCodigoStr() + "-" + getCodigoStr() + "-" + getPeriodo().toString() + "-" + getDisciplinaCodigoStr();
	}

	public String getCodigoStr() {
		return codigoStr;
	}

	public void setCodigoStr(String codigoStr) {
		this.codigoStr = codigoStr;
	}

	public String getDescricaoStr() {
		return descricaoStr;
	}

	public void setDescricaoStr(String descricaoStr) {
		this.descricaoStr = descricaoStr;
	}

	public String getCursoCodigoStr() {
		return cursoCodigoStr;
	}

	public void setCursoCodigoStr(String cursoCodigoStr) {
		this.cursoCodigoStr = cursoCodigoStr;
	}

	public String getPeriodoStr() {
		return periodoStr;
	}

	public void setPeriodoStr(String periodoStr) {
		this.periodoStr = periodoStr;
	}

	public String getDisciplinaCodigoStr() {
		return disciplinaCodigoStr;
	}

	public void setDisciplinaCodigoStr(String disciplinaCodigoStr) {
		this.disciplinaCodigoStr = disciplinaCodigoStr;
	}

	public Curso getCurso() {
		return curso;
	}

	public void setCurso(Curso curso) {
		this.curso = curso;
	}

	public Disciplina getDisciplina() {
		return disciplina;
	}

	public void setDisciplina(Disciplina disciplina) {
		this.disciplina = disciplina;
	}

	public Integer getPeriodo() {
		return periodo;
	}

	@Override
	public int compareTo(CurriculosImportExcelBean o) {
		return getCodigoStr().compareTo(o.getCodigoStr());
	}
	
	public static Map<String,CurriculosImportExcelBean> buildCurriculoCodigoToImportExcelBeanMap(List<CurriculosImportExcelBean> beans) {
		Map<String,CurriculosImportExcelBean> beansMap = new HashMap<String,CurriculosImportExcelBean>();
		for (CurriculosImportExcelBean bean : beans) {
			beansMap.put(bean.getCodigoStr(),bean);
		}
		return beansMap;
	}
}