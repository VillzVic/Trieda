package com.gapso.web.trieda.shared.util.relatorioVisao;

import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO.ReportType;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.services.Services;

public class GradeHorariaSalaGrid extends GradeHorariaVisao{
	
	protected String emptyTextAfterSearch = "Não existem alocações para a sala em questão.";
	
	public GradeHorariaSalaGrid(CenarioDTO cenarioDTO) {
		super(cenarioDTO);
	}
	
	public GradeHorariaSalaGrid(CenarioDTO cenarioDTO, boolean gradeHorariaAlocacaoManual) {
		super(cenarioDTO, gradeHorariaAlocacaoManual);
	}

	private RelatorioVisaoSalaFiltro filtro;
	
	@Override
	protected String getEmptyTextAfterSearch()
	{
		return emptyTextAfterSearch;
	}
	
	public void requestAtendimentos() {
		if(getFiltro().getSalaCodigo() == null) return;

		this.grid.mask("Carregando os dados, aguarde alguns instantes", "loading");

		Services.atendimentos().getAtendimentosParaGradeHorariaVisaoSala(cenarioDTO, getFiltro(), gradeHorariaAlocacaoManual, this.getCallback());
	}
	
	protected TrioDTO<String, String, String> getHTMLInfo(AtendimentoRelatorioDTO atendimentoDTO){
		return TrioDTO.create(
			atendimentoDTO.getDisciplinaString(),
			atendimentoDTO.getContentToolTipVisaoSala(ReportType.WEB),
			atendimentoDTO.getContentVisaoSala(ReportType.WEB)
		);
	}
	
	@Override
	public RelatorioVisaoSalaFiltro getFiltro(){
		return this.filtro;
	}
	
	@Override
	public void setFiltro(RelatorioVisaoFiltro filtro){
		this.filtro = (RelatorioVisaoSalaFiltro) filtro;
	}

	@Override
	public void checkRegistered() {
		if(!cenarioDTO.hasSalas()){
			this.grid.getView().setEmptyText("Não há ambientes cadastrados");
		} else if(!cenarioDTO.isOptimized()){
			this.grid.getView().setEmptyText("Cenário não foi otimizado");
		}
		
	}

}
