package com.gapso.web.trieda.shared.util.relatorioVisao;

import java.util.List;

import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO.ReportType;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.services.Services;

public class GradeHorariaCursoGrid extends GradeHorariaVisao{
	private RelatorioVisaoCursoFiltro filtro;
	private List<Integer> qtdColunasPorDiaSemana;

	public void requestAtendimentos(){
		if(getFiltro().getCurriculoDTO() == null || getFiltro().getTurnoDTO() == null || getFiltro().getCampusDTO() == null || getFiltro().getPeriodo() <= 0) return;

		this.grid.mask("Carregando os dados, aguarde alguns instantes", "loading");

		Services.atendimentos().getAtendimentosParaGradeHorariaVisaoCurso(getFiltro(), this.getCallback());
	}

	public List<ColumnConfig> getColumnList(List<Integer> l){
		qtdColunasPorDiaSemana = l;
		List<ColumnConfig> list = super.getColumnList(null);
		int columnsCount = getColumnsCount();

		for(int i = 8; i < columnsCount; i++) addColumn(list, "extra" + i, "");

		return list;
	}

	private int getColumnsCount(){
		int count = 0;

		if(this.qtdColunasPorDiaSemana == null) return count;

		for(Integer c : this.qtdColunasPorDiaSemana) count += c;

		return count;
	}

	protected int getSemana(int colIndex){
		return colIndex;
	}
	
	protected TrioDTO<String, String, String> getHTMLInfo(AtendimentoRelatorioDTO atendimentoDTO){
		return TrioDTO.create(
			atendimentoDTO.getDisciplinaString(),
			atendimentoDTO.getContentToolTipVisaoCurso(ReportType.WEB),
			atendimentoDTO.getContentVisaoCurso(ReportType.WEB)
		);
	}
		
	protected int getWidth(String semana){
		int width = super.getWidth(semana);
		if(this.qtdColunasPorDiaSemana == null) return width;

		int i = 0;
		if(semana.equals("segunda")) i = 2;
		if(semana.equals("terca" )) i = 3;
		if(semana.equals("quarta")) i = 4;
		if(semana.equals("quinta")) i = 5;
		if(semana.equals("sexta")) i = 6;
		if(semana.equals("sabado")) i = 7;
		if(semana.equals("domingo")) i = 1;

		return (this.qtdColunasPorDiaSemana.get(i) * width);
	}

	@Override
	public RelatorioVisaoCursoFiltro getFiltro(){
		return this.filtro;
	}
	
	@Override
	public void setFiltro(RelatorioVisaoFiltro filtro){
		this.filtro = (RelatorioVisaoCursoFiltro) filtro;
	}
	
}
