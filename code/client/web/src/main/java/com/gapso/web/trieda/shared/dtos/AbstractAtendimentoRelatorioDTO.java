package com.gapso.web.trieda.shared.dtos;

import com.gapso.web.trieda.shared.util.TriedaUtil;

public abstract class AbstractAtendimentoRelatorioDTO<NKType> extends AbstractDTO<NKType> implements AtendimentoRelatorioDTO {

	private static final long serialVersionUID = 5260734304888470231L;
	
	public String getContentVisaoSala(ReportType reportType) {
		if (reportType.equals(ReportType.WEB)) {
			final String BR = TriedaUtil.newLine(reportType);
			return getDisciplinaString() + BR
			+ TriedaUtil.truncate( getDisciplinaNome(), 12 ) + BR
			+ "Turma " + getTurma() + BR
			+ getQuantidadeAlunosString() + " aluno(s)";
		} else {
			return getDisciplinaString() + " / " + getTurma();
		}
	}
	
	public String getContentVisaoCurso(ReportType reportType) {
		if (reportType.equals(ReportType.WEB)) {
			final String BR = TriedaUtil.newLine(reportType);
			return getDisciplinaString() + BR
			+ TriedaUtil.truncate( getDisciplinaNome(), 12 ) + BR
			+ "Turma " + getTurma() + BR
			+ getUnidadeString() + BR
			+ getSalaString() + BR;
		} else {
			return getDisciplinaString() + " / " + getTurma();
		}
	}
	
	public void concatenateVisaoSala( AtendimentoRelatorioDTO other ) {
		setCursoNome( getCursoNome() + " / " + other.getCursoNome() );
		setCurricularString( getCurriculoString() + " / " + other.getCurriculoString() );
		setPeriodoString( getPeriodoString() + " / " + other.getPeriodoString() );
		setQuantidadeAlunosString( getQuantidadeAlunosString() + " / " + other.getQuantidadeAlunosString() );
		setQuantidadeAlunos( getQuantidadeAlunos() + other.getQuantidadeAlunos() );
	}
	
	public void concatenateVisaoCurso( AtendimentoRelatorioDTO other ) {}
}