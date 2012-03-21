package com.gapso.web.trieda.shared.dtos;

import com.gapso.web.trieda.shared.util.TriedaUtil;

public abstract class AbstractAtendimentoRelatorioDTO<NKType> extends AbstractDTO<NKType> implements AtendimentoRelatorioDTO {

	private static final long serialVersionUID = 5260734304888470231L;
	
	public String getContentVisaoSala(ReportType reportType) {
		if (reportType.equals(ReportType.WEB)) {
			final String BR = TriedaUtil.newLine(reportType);
			
			String disciplinaInfo = "";
			if (getDisciplinaSubstitutaId() != null) {
				disciplinaInfo = "*" + getDisciplinaSubstitutaString() + "*" + BR + TriedaUtil.truncate(getDisciplinaSubstitutaNome(),12);
			} else {
				String disciplinaCod = getDisciplinaString();
				if (getQuantidadeAlunosString().contains("*")) {
					disciplinaCod = "*" + disciplinaCod + "*";
				}
				disciplinaInfo = disciplinaCod + BR + TriedaUtil.truncate(getDisciplinaNome(),12);
			}
			
			String qtdAlunos = "";
			if (getQuantidadeAlunosString().contains("/")) {
				qtdAlunos = "*" + getQuantidadeAlunos() + "*"; 
			} else  {
				qtdAlunos = getQuantidadeAlunos().toString();
			}
			
			return disciplinaInfo + BR
			+ "Turma " + getTurma() + BR
			+ qtdAlunos + " aluno(s)";
		} else {
			String disciplinaInfo = "";
			if (getDisciplinaSubstitutaId() != null) {
				disciplinaInfo = "*" + getDisciplinaSubstitutaString() + "*";
			} else {
				disciplinaInfo = getDisciplinaString();
			}
			return disciplinaInfo + " / " + getTurma();
		}
	}
	
	public String getContentVisaoCurso(ReportType reportType) {
		if (reportType.equals(ReportType.WEB)) {
			final String BR = TriedaUtil.newLine(reportType);
			String disciplinaInfo = "";
			if (getDisciplinaSubstitutaId() != null) {
				disciplinaInfo = "*" + getDisciplinaSubstitutaString() + "*" + BR + TriedaUtil.truncate(getDisciplinaSubstitutaNome(),12);
			} else {
				disciplinaInfo = getDisciplinaString() + BR + TriedaUtil.truncate(getDisciplinaNome(),12);
			}
			return disciplinaInfo + BR
			+ "Turma " + getTurma() + BR
			+ getUnidadeString() + BR
			+ getSalaString() + BR;
		} else {
			String disciplinaInfo = "";
			if (getDisciplinaSubstitutaId() != null) {
				disciplinaInfo = "*" + getDisciplinaSubstitutaString() + "*";
			} else {
				disciplinaInfo = getDisciplinaString();
			}
			return disciplinaInfo + " / " + getTurma();
		}
	}
	
	public void concatenateVisaoSala( AtendimentoRelatorioDTO other ) {
		String T = (this.getDisciplinaSubstitutaId() != null && !getCursoNome().contains(" / ")) ? "*" : "";
		String O = (other.getDisciplinaSubstitutaId() != null) ? "*" : ""; 
		
		setCursoNome( T + getCursoNome() + T + " / " + O + other.getCursoNome() + O );
		setCurricularString( T + getCurriculoString() + T + " / " + O + other.getCurriculoString() + O );
		setPeriodoString( T + getPeriodoString() + T + " / " + O + other.getPeriodoString() + O );
		setQuantidadeAlunosString( T + getQuantidadeAlunosString() + T + " / " + O + other.getQuantidadeAlunosString() + O );
		setQuantidadeAlunos( getQuantidadeAlunos() + other.getQuantidadeAlunos() );
	}
	
	public void concatenateVisaoCurso( AtendimentoRelatorioDTO other ) {}
	
	public Integer getDuracaoDeUmaAulaEmMinutos() {
		if (getDisciplinaSubstitutaSemanaLetivaId() != null) {
			return getDisciplinaSubstitutaSemanaLetivaTempoAula();
		}
		return getSemanaLetivaTempoAula();
	}
}