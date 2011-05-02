package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Turno;

public class DemandasImportExcelBean extends AbstractImportExcelBean implements Comparable<DemandasImportExcelBean> {
	
	private String campusStr;
	private String turnoStr;
	private String cursoStr;
	private String matrizCurricularStr;
	private String periodoStr;
	private String disciplinaStr;
	private String demandaStr;
	
	private Campus campus;
	private Turno turno;
	private Curso curso;
	private Curriculo matrizCurricular;
	private Integer periodo;
	private Disciplina disciplina;
	private Integer demanda;
	
	public DemandasImportExcelBean(int row) {
		super(row);
	}
	
	public List<ImportExcelError> checkSyntacticErrors() {
		List<ImportExcelError> erros = new ArrayList<ImportExcelError>();
		if (!tudoVazio()) {
			checkMandatoryField(campusStr,ImportExcelError.DEMANDA_CAMPUS_VAZIO,erros);
			checkMandatoryField(turnoStr,ImportExcelError.DEMANDA_TURNO_VAZIO,erros);
			checkMandatoryField(cursoStr,ImportExcelError.DEMANDA_CURSO_VAZIO,erros);
			checkMandatoryField(matrizCurricularStr,ImportExcelError.DEMANDA_MATRIZ_CURRIULAR_VAZIO,erros);
			checkMandatoryField(periodoStr,ImportExcelError.DEMANDA_PERIODO_VAZIO,erros);
			checkMandatoryField(disciplinaStr,ImportExcelError.DEMANDA_DISCIPLINA_VAZIO,erros);
			checkMandatoryField(demandaStr,ImportExcelError.DEMANDA_DEMANDA_VAZIO,erros);
			
			periodo = checkNonNegativeIntegerField(periodoStr,ImportExcelError.DEMANDA_PERIODO_FORMATO_INVALIDO,ImportExcelError.DEMANDA_PERIODO_VALOR_NEGATIVO,erros);
			demanda = checkNonNegativeIntegerField(demandaStr,ImportExcelError.DEMANDA_DEMANDA_FORMATO_INVALIDO,ImportExcelError.DEMANDA_DEMANDA_VALOR_NEGATIVO,erros);
		} else {
			erros.add(ImportExcelError.TUDO_VAZIO);
		}
		return erros;
	}
	
	private boolean tudoVazio() {
		return isEmptyField(campusStr) &&
			isEmptyField(turnoStr) &&
			isEmptyField(cursoStr) &&
			isEmptyField(matrizCurricularStr) &&
			isEmptyField(periodoStr) &&
			isEmptyField(disciplinaStr) &&
			isEmptyField(demandaStr);
	}
	
	
	public String getCampusStr() {
		return campusStr;
	}
	public void setCampusStr(String campusStr) {
		this.campusStr = campusStr;
	}

	public String getTurnoStr() {
		return turnoStr;
	}
	public void setTurnoStr(String turnoStr) {
		this.turnoStr = turnoStr;
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

	public String getDemandaStr() {
		return demandaStr;
	}
	public void setDemandaStr(String demandaStr) {
		this.demandaStr = demandaStr;
	}

	public Campus getCampus() {
		return campus;
	}
	public void setCampus(Campus campus) {
		this.campus = campus;
	}

	public Turno getTurno() {
		return turno;
	}
	public void setTurno(Turno turno) {
		this.turno = turno;
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

	public Integer getDemanda() {
		return demanda;
	}
	public void setDemanda(Integer demanda) {
		this.demanda = demanda;
	}

	@Override
	public int compareTo(DemandasImportExcelBean o) {
		int result = getCampusStr().compareTo(o.getCampusStr());
		if (result == 0) {
			result = getTurnoStr().compareTo(o.getTurnoStr());
			if (result == 0) {
				result = getCursoStr().compareTo(o.getCursoStr());
				if (result == 0) {
					result = getMatrizCurricularStr().compareTo(o.getMatrizCurricularStr());
					if (result == 0) {
						result = getDisciplinaStr().compareTo(o.getDisciplinaStr());
					}
				}
			}
		}
		return result;
	}
	
}
