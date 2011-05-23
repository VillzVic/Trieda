package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Professor;

public class CampiTrabalhoImportExcelBean extends AbstractImportExcelBean implements Comparable<CampiTrabalhoImportExcelBean> {
	
	private String campusStr;
	private String cpfStr;
	private String professorStr;
	
	private Campus campus;
	private Professor professor;
	
	public CampiTrabalhoImportExcelBean(int row) {
		super(row);
	}
	
	public List<ImportExcelError> checkSyntacticErrors() {
		List<ImportExcelError> erros = new ArrayList<ImportExcelError>();
		if (!tudoVazio()) {
			checkMandatoryField(campusStr, ImportExcelError.CAMPITRABALHO_CAMPUS_VAZIO,erros);
			checkMandatoryField(cpfStr, ImportExcelError.CAMPITRABALHO_CPF_VAZIO,erros);
			checkMandatoryField(professorStr, ImportExcelError.CAMPITRABALHO_PROFESSOR_VAZIO,erros);
		} else {
			erros.add(ImportExcelError.TUDO_VAZIO);
		}
		return erros;
	}
	
	private boolean tudoVazio() {
		return isEmptyField(campusStr) &&
			isEmptyField(cpfStr) &&
			isEmptyField(professorStr);
	}
	
	public String getCampusStr() {
		return campusStr;
	}
	public void setCampusStr(String campusStr) {
		this.campusStr = campusStr;
	}

	public String getCpfStr() {
		return cpfStr;
	}
	public void setCpfStr(String turnoStr) {
		this.cpfStr = turnoStr;
	}

	public String getProfessorStr() {
		return professorStr;
	}
	public void setProfessorStr(String professorStr) {
		this.professorStr = professorStr;
	}

	public Campus getCampus() {
		return campus;
	}
	public void setCampus(Campus campus) {
		this.campus = campus;
	}

	public Professor getProfessor() {
		return professor;
	}
	public void setProfessor(Professor professor) {
		this.professor = professor;
	}

	@Override
	public int compareTo(CampiTrabalhoImportExcelBean o) {
		int result = getCampusStr().compareTo(o.getCampusStr());
		if (result == 0) {
			result = getProfessorStr().compareTo(o.getProfessorStr());
		}
		return result;
	}
	
}
