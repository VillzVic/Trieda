package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Professor;

public class HabilitacoesProfessoresImportExcelBean extends AbstractImportExcelBean implements Comparable<HabilitacoesProfessoresImportExcelBean> {
	
	private String cpfStr;
	private String nomeStr;
	private String disciplinaStr;
	private String preferenciaStr;
	private String notaStr;
	
	private Disciplina disciplina;
	private Professor professor;
	private Integer preferencia;
	private Integer nota;
	
	public HabilitacoesProfessoresImportExcelBean(int row) {
		super(row);
	}
	
	public List<ImportExcelError> checkSyntacticErrors() {
		List<ImportExcelError> erros = new ArrayList<ImportExcelError>();
		if (!tudoVazio()) {
			checkMandatoryField(cpfStr,ImportExcelError.HABILITACOESPROFESSORES_CPF_VAZIO,erros);
			checkMandatoryField(nomeStr,ImportExcelError.HABILITACOESPROFESSORES_NOME_VAZIO,erros);
			checkMandatoryField(disciplinaStr,ImportExcelError.HABILITACOESPROFESSORES_DISCIPLINA_VAZIO,erros);
			checkMandatoryField(preferenciaStr,ImportExcelError.HABILITACOESPROFESSORES_PREFERENCIA_VAZIO,erros);
			checkMandatoryField(notaStr,ImportExcelError.HABILITACOESPROFESSORES_NOTA_VAZIO,erros);
			preferencia = checkNonNegativeIntegerField(preferenciaStr,ImportExcelError.HABILITACOESPROFESSORES_PREFERENCIA_FORMATO_INVALIDO,ImportExcelError.HABILITACOESPROFESSORES_PREFERENCIA_VALOR_NEGATIVO,erros);
			nota = checkNonNegativeIntegerField(notaStr,ImportExcelError.HABILITACOESPROFESSORES_NOTA_FORMATO_INVALIDO,ImportExcelError.HABILITACOESPROFESSORES_NOTA_VALOR_NEGATIVO,erros);
		} else {
			erros.add(ImportExcelError.TUDO_VAZIO);
		}
		return erros;
	}
	
	private boolean tudoVazio() {
		return isEmptyField(cpfStr) &&
			isEmptyField(nomeStr) &&
			isEmptyField(disciplinaStr) &&
			isEmptyField(preferenciaStr) &&
			isEmptyField(notaStr);
	}

	

	public String getCpfStr() {
		return cpfStr;
	}
	public void setCpfStr(String cpfStr) {
		this.cpfStr = cpfStr;
	}

	public String getNomeStr() {
		return nomeStr;
	}
	public void setNomeStr(String nomeStr) {
		this.nomeStr = nomeStr;
	}

	public String getDisciplinaStr() {
		return disciplinaStr;
	}
	public void setDisciplinaStr(String disciplinaStr) {
		this.disciplinaStr = disciplinaStr;
	}

	public String getPreferenciaStr() {
		return preferenciaStr;
	}
	public void setPreferenciaStr(String preferenciaStr) {
		this.preferenciaStr = preferenciaStr;
	}

	public String getNotaStr() {
		return notaStr;
	}
	public void setNotaStr(String notaStr) {
		this.notaStr = notaStr;
	}
	
	public Disciplina getDisciplina() {
		return disciplina;
	}
	public void setDisciplina(Disciplina disciplina) {
		this.disciplina = disciplina;
	}

	public Integer getPreferencia() {
		return preferencia;
	}
	public void setPreferencia(Integer preferencia) {
		this.preferencia = preferencia;
	}

	public Integer getNota() {
		return nota;
	}
	public void setNota(Integer nota) {
		this.nota = nota;
	}
	
	public Professor getProfessor() {
		return professor;
	}
	public void setProfessor(Professor professor) {
		this.professor = professor;
	}

	@Override
	public int compareTo(HabilitacoesProfessoresImportExcelBean o) {
		int result = getDisciplinaStr().compareTo(o.getDisciplinaStr());
		if (result == 0) {
			result = getNomeStr().compareTo(o.getNomeStr());
		}
		return result;
	}
	
	public String getNaturalKeyString() {
		return getCpfStr() + "-" + getNomeStr() + "-" + getDisciplinaStr();
	}
}
