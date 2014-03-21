package com.gapso.web.trieda.shared.util.relatorioVisao;

import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO.ReportType;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.services.Services;

public class GradeHorariaAlunoGrid extends GradeHorariaVisao{
	public GradeHorariaAlunoGrid(CenarioDTO cenarioDTO) {
		super(cenarioDTO);
	}

	private RelatorioVisaoAlunoFiltro filtro;

	public void requestAtendimentos(){
		if(filtro.getAlunoNome() == null && filtro.getAlunoMatricula() == null) return;

		this.grid.mask("Carregando os dados, aguarde alguns instantes", "loading");
		
		Services.atendimentos().getAtendimentosParaGradeHorariaVisaoAluno(cenarioDTO, getFiltro(), this.getCallback());
	}
	
	protected TrioDTO<String, String, String> getHTMLInfo(AtendimentoRelatorioDTO atendimentoDTO){
		return TrioDTO.create(
			atendimentoDTO.getDisciplinaString(),
			atendimentoDTO.getContentToolTipVisaoAluno(ReportType.WEB),
			atendimentoDTO.getContentVisaoAluno(ReportType.WEB)
		);
	}
	
	@Override
	public RelatorioVisaoAlunoFiltro getFiltro(){
		return this.filtro;
	}
	
	@Override
	public void setFiltro(RelatorioVisaoFiltro filtro){
		this.filtro = (RelatorioVisaoAlunoFiltro) filtro;
	}
	
	@Override
	public void checkRegistered() {
		if(!cenarioDTO.hasAlunos()){
			this.grid.getView().setEmptyText("Não há alunos cadastrados");
		} else if(!cenarioDTO.isOptimized()){
			this.grid.getView().setEmptyText("Cenário não foi otimizado");
		}
	}

}
