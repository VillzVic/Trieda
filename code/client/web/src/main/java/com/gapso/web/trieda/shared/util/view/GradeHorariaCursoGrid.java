package com.gapso.web.trieda.shared.util.view;

import java.util.List;

import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.GridCellRenderer;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO.ReportType;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.SextetoDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class GradeHorariaCursoGrid extends GradeHorariaVisao{
	private List<Integer> qtdColunasPorDiaSemana;
	private CurriculoDTO curriculoDTO;
	private int periodo;
	private CampusDTO campusDTO;
	private CursoDTO cursoDTO;

	@SuppressWarnings("unchecked")
	public void requestAtendimentos(){
		if(getCurriculoDTO() == null || getTurnoDTO() == null || getCampusDTO() == null || getPeriodo() <= 0) return;

		this.grid.mask("Carregando os dados, aguarde alguns instantes", "loading");

		AsyncCallback<SextetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>,List<Integer>,List<String>>> c = 
			(AsyncCallback<SextetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>,List<Integer>,List<String>>>) 
			getCallback((new SextetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>,List<Integer>,List<String>>()).getClass());
		
		Services.atendimentos().getAtendimentosParaGradeHorariaVisaoCurso(getCurriculoDTO(), getPeriodo(), getTurnoDTO(), getCampusDTO(), getCursoDTO(), c);
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

	protected void addColumn(List<ColumnConfig> list, String id, String name){
		GridCellRenderer<LinhaDeCredito> change = getGridCellRenderer();
		int width = (name.equals("")) ? 0 : getWidth(id);
		
		ColumnConfig column = new ColumnConfig(id, name, width);
		column.setRenderer(change);
		column.setResizable(false);
		column.setMenuDisabled(true);
		column.setSortable(false);
		
		list.add(column);
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

	public CurriculoDTO getCurriculoDTO(){
		return this.curriculoDTO;
	}

	public void setCurriculoDTO(CurriculoDTO curriculoDTO){
		this.curriculoDTO = curriculoDTO;
	}

	public int getPeriodo(){
		return this.periodo;
	}

	public void setPeriodo(int periodo){
		this.periodo = periodo;
	}

	public CampusDTO getCampusDTO(){
		return this.campusDTO;
	}

	public void setCampusDTO(CampusDTO campusDTO){
		this.campusDTO = campusDTO;
	}

	public CursoDTO getCursoDTO(){
		return this.cursoDTO;
	}

	public void setCursoDTO(CursoDTO cursoDTO){
		this.cursoDTO = cursoDTO;
	}

}
