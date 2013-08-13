package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Sala;

public class DisciplinasSalasImportExcelBean extends AbstractImportExcelBean implements Comparable<DisciplinasSalasImportExcelBean> {
	
	private String salaStr;
	private String disciplinaStr;
	
	private Sala sala;
	private Disciplina disciplina;
	
	public DisciplinasSalasImportExcelBean(int row) {
		super(row);
	}
	
	public List<ImportExcelError> checkSyntacticErrors() {
		List<ImportExcelError> erros = new ArrayList<ImportExcelError>();
		if (!tudoVazio()) {
			checkMandatoryField(salaStr,ImportExcelError.DISCIPLINASALA_SALA_VAZIO,erros);
			checkMandatoryField(disciplinaStr,ImportExcelError.DISCIPLINASALA_DISCIPLINA_VAZIO,erros);
			String[] values = {salaStr,disciplinaStr};
			ImportExcelError[] errorsTypes = {ImportExcelError.DISCIPLINASALA_CURSO_VAZIO,ImportExcelError.DISCIPLINASALA_MATRIZ_CURRIULAR_VAZIO,ImportExcelError.DISCIPLINASALA_PERIODO_VAZIO};
			checkMandatoryFields(values,errorsTypes,erros);
			
		} else {
			erros.add(ImportExcelError.TUDO_VAZIO);
		}
		return erros;
	}
	
	private boolean tudoVazio() {
		return isEmptyField(salaStr) &&
			isEmptyField(disciplinaStr);
	}
	
/*	public boolean isAssociacaoSemCurriculo() {
		return isEmptyField(cursoStr) && isEmptyField(matrizCurricularStr) && isEmptyField(periodoStr);
	}*/
	
	public String getSalaStr() {
		return salaStr;
	}
	public void setSalaStr(String salaStr) {
		this.salaStr = salaStr;
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
			result = getDisciplinaStr().compareTo(o.getDisciplinaStr());
		}
		return result;
	}
	
}
