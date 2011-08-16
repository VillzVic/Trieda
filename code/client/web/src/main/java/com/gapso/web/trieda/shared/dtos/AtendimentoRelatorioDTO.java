package com.gapso.web.trieda.shared.dtos;

public interface AtendimentoRelatorioDTO
{
	public Long getOfertaId();
	public Long getCampusId();
	public Integer getSemana();
	public Integer getTotalCreditos();
	public Long getDisciplinaId();
	public String getDisciplinaNome();
	public String getContentToolTipVisaoSala();
	public String getContentToolTipVisaoCurso();
	public String getContentVisaoSala();
	public Integer getQuantidadeAlunos();
	public String getDisciplinaString();
	public String getTurma();
	public String getCursoNome();
	public Long getCursoId();
	public String getTurnoString();
	public Long getTurnoId();
	public String getCurriculoString();
	public Long getCurriculoId();
	public String getPeriodoString();
	public String getQuantidadeAlunosString();
	public void concatenateVisaoSala( AtendimentoRelatorioDTO other );
	public void concatenateVisaoCurso( AtendimentoRelatorioDTO other );
	public String getExcelContentVisaoSala();
	public String getExcelContentVisaoCurso();
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
}
