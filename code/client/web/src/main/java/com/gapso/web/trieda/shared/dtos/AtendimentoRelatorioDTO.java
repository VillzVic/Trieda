package com.gapso.web.trieda.shared.dtos;

public interface AtendimentoRelatorioDTO
{
	static public enum ReportType {EXCEL, WEB};
	
	public Long getOfertaId();
	public Long getCampusId();
	public Integer getSemana();
	public Integer getTotalCreditos();
	public Long getDisciplinaId();
	public String getDisciplinaNome();
	public String getContentToolTipVisaoSala(ReportType reportType);
	public String getContentToolTipVisaoCurso(ReportType reportType);
	public String getContentVisaoSala(ReportType reportType);
	public String getContentVisaoCurso(ReportType reportType);
	public Integer getQuantidadeAlunos();
	public String getDisciplinaString();
	public String getTurma();
	public String getCursoNome();
	public Long getCursoId();
	public String getTurnoString();
	public Long getTurnoId();
	public String getCurriculoString();
	public Long getCurriculoId();
	public Long getSemanaLetivaId();
	public Integer getPeriodo();
	public String getPeriodoString();
	public String getQuantidadeAlunosString();
	public void concatenateVisaoSala( AtendimentoRelatorioDTO other );
	public void concatenateVisaoCurso( AtendimentoRelatorioDTO other );
	public Long getSalaId();
	public String getSalaString();
	public boolean isTeorico();
	public Integer getTotalCreditoDisciplina();
	public String getUnidadeString();
	public boolean isTatico();
	public boolean isProfessorVirtual();
	public String getProfessorString();
	public String getCompartilhamentoCursosString();
	public void setCompartilhamentoCursosString( String s );
	public void setCursoNome( String value );
	public void setCurricularString( String value );
	public void setPeriodoString( String value );
	public void setQuantidadeAlunosString( String value );
	public void setQuantidadeAlunos( Integer value );
}
