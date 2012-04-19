package com.gapso.web.trieda.shared.util.relatorioVisao;

import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO.ReportType;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.services.Services;

public class GradeHorariaProfessorGrid extends GradeHorariaVisao{
	private RelatorioVisaoProfessorFiltro filtro;
	private boolean isVisaoProfessor;

	public GradeHorariaProfessorGrid(boolean isVisaoProfessor){
		super();

		this.isVisaoProfessor = isVisaoProfessor; 
	}

	public boolean isVisaoProfessor(){
		return this.isVisaoProfessor;
	}

	public void setVisaoProfessor(boolean isVisaoProfessor){
		this.isVisaoProfessor = isVisaoProfessor;
	}

	public void requestAtendimentos(){
		if(getFiltro().getTurnoDTO() == null || (getFiltro().getProfessorDTO() == null && getFiltro().getProfessorVirtualDTO() == null)) return;

		this.grid.mask("Carregando os dados, aguarde alguns instantes", "loading");
		
		Services.atendimentos().getAtendimentosParaGradeHorariaVisaoProfessor(getFiltro(), this.isVisaoProfessor(), this.getCallback());
	}
	
	protected TrioDTO<String, String, String> getHTMLInfo(AtendimentoRelatorioDTO atendimentoDTO){
		return TrioDTO.create(
			atendimentoDTO.getDisciplinaNome(),
			((AtendimentoOperacionalDTO) atendimentoDTO).getContentToolTipVisaoProfessor(ReportType.WEB),
			((AtendimentoOperacionalDTO) atendimentoDTO).getContentVisaoProfessor(ReportType.WEB)
		);
	}

	@Override
	public RelatorioVisaoProfessorFiltro getFiltro(){
		return this.filtro;
	}
	
	@Override
	public void setFiltro(RelatorioVisaoFiltro filtro){
		this.filtro = (RelatorioVisaoProfessorFiltro) filtro;
	}
	
}
