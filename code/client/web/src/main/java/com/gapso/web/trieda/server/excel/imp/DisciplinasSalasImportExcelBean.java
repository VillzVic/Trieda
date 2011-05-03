package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Sala;

public class DisciplinasSalasImportExcelBean extends AbstractImportExcelBean implements Comparable<DisciplinasSalasImportExcelBean> {
	
	private String salaStr;
	private String cursoStr;
	private String matrizCurricularStr;
	private String periodoStr;
	private String disciplinaStr;
	
	private Sala sala;
	private Curso curso;
	private Curriculo matrizCurricular;
	private Integer periodo;
	private Disciplina disciplina;
	
	public DisciplinasSalasImportExcelBean(int row) {
		super(row);
	}
	
	public List<ImportExcelError> checkSyntacticErrors() {
		List<ImportExcelError> erros = new ArrayList<ImportExcelError>();
		if (!tudoVazio()) {
			checkMandatoryField(salaStr,ImportExcelError.DISCIPLINASALA_SALA_VAZIO,erros);
			checkMandatoryField(cursoStr,ImportExcelError.DISCIPLINASALA_CURSO_VAZIO,erros);
			checkMandatoryField(matrizCurricularStr,ImportExcelError.DISCIPLINASALA_MATRIZ_CURRIULAR_VAZIO,erros);
			checkMandatoryField(periodoStr,ImportExcelError.DISCIPLINASALA_PERIODO_VAZIO,erros);
			checkMandatoryField(disciplinaStr,ImportExcelError.DISCIPLINASALA_DISCIPLINA_VAZIO,erros);
			
			periodo = checkNonNegativeIntegerField(periodoStr,ImportExcelError.DISCIPLINASALA_PERIODO_FORMATO_INVALIDO,ImportExcelError.DISCIPLINASALA_PERIODO_VALOR_NEGATIVO,erros);
		} else {
			erros.add(ImportExcelError.TUDO_VAZIO);
		}
		return erros;
	}
	
	private boolean tudoVazio() {
		return isEmptyField(salaStr) &&
			isEmptyField(cursoStr) &&
			isEmptyField(matrizCurricularStr) &&
			isEmptyField(periodoStr) &&
			isEmptyField(disciplinaStr);
	}
	
	
	public String getSalaStr() {
		return salaStr;
	}
	public void setSalaStr(String salaStr) {
		this.salaStr = salaStr;
	}

	public String getCursoStr() {
		return cursoStr;
	}
	public void setCursoStr(String cursoStr) {
		this.cursoStr = cursoStr;
	}

	public String getMatrizCurricularStr() {
		return matrizCurricularStr;
	}
	public void setMatrizCurricularStr(String matrizCurricularStr) {
		this.matrizCurricularStr = matrizCurricularStr;
	}

	public String getPeriodoStr() {
		return periodoStr;
	}
	public void setPeriodoStr(String periodoStr) {
		this.periodoStr = periodoStr;
	}

	public String getDisciplinaStr() {
		return disciplinaStr;
	}
	public void setDisciplinaStr(String disciplinaStr) {
		this.disciplinaStr = disciplinaStr;
	}

	public Sala getSala() {
		return sala;
	}
	public void setSala(Sala sala) {
		this.sala = sala;
	}

	public Curso getCurso() {
		return curso;
	}
	public void setCurso(Curso curso) {
		this.curso = curso;
	}

	public Curriculo getMatrizCurricular() {
		return matrizCurricular;
	}
	public void setMatrizCurricular(Curriculo matrizCurricular) {
		this.matrizCurricular = matrizCurricular;
	}

	public Integer getPeriodo() {
		return periodo;
	}
	public void setPeriodo(Integer periodo) {
		this.periodo = periodo;
	}

	public Disciplina getDisciplina() {
		return disciplina;
	}
	public void setDisciplina(Disciplina disciplina) {
		this.disciplina = disciplina;
	}

	@Override
	public int compareTo(DisciplinasSalasImportExcelBean o) {
		int result = getSalaStr().compareTo(o.getSalaStr());
		if (result == 0) {
			result = getCursoStr().compareTo(o.getCursoStr());
			if (result == 0) {
				result = getMatrizCurricularStr().compareTo(o.getMatrizCurricularStr());
				if (result == 0) {
					result = getDisciplinaStr().compareTo(o.getDisciplinaStr());
				}
			}
		}
		return result;
	}
	
}
