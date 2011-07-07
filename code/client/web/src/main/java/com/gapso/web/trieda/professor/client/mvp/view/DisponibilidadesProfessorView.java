package com.gapso.web.trieda.professor.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.gapso.web.trieda.professor.client.mvp.presenter.DisponibilidadesProfessorPresenter;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class DisponibilidadesProfessorView extends MyComposite implements DisponibilidadesProfessorPresenter.Display {

	private SimpleToolBar toolBar;
	private SimpleGrid<SemanaLetivaDTO> gridPanel;
	private ContentPanel panel;
	private GTabItem tabItem;
	private Button diasDeAulaButton;
	@SuppressWarnings("unused")
	private UsuarioDTO usuario;
	
	public DisponibilidadesProfessorView(UsuarioDTO usuario) {
		this.usuario = usuario;
		initUI();
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading("Professor » Disponibilidades do Professor");
		createToolBar();
		createGrid();
		createTabItem();
		initComponent(tabItem);
	}
	
	private void createTabItem() {
		tabItem = new GTabItem("Disponibilidades do Professor", Resources.DEFAULTS.semanaLetiva16());
		tabItem.setContent(panel);
	}
	
	private void createToolBar() {
		toolBar = new SimpleToolBar(false, false, false, false, false, this);
		diasDeAulaButton = toolBar.createButton("Dias e Horários de Aula", Resources.DEFAULTS.diaHorarioAula16());
		toolBar.add(diasDeAulaButton);
		panel.setTopComponent(toolBar);
	}
	
	private void createGrid() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    
	    gridPanel = new SimpleGrid<SemanaLetivaDTO>(getColumnList(), this);
	    panel.add(gridPanel, bld);
	}

	public List<ColumnConfig> getColumnList() {
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		list.add(new ColumnConfig(SemanaLetivaDTO.PROPERTY_CODIGO, "Codigo", 100));
		list.add(new ColumnConfig(SemanaLetivaDTO.PROPERTY_DESCRICAO, "Descrição", 100));
		return list;
	}
	
	@Override
	public SimpleGrid<SemanaLetivaDTO> getGrid() {
		return gridPanel;
	}

	@Override
	public Button getDiasDeAulaButton() {
		return diasDeAulaButton;
	}

	@Override
	public void setProxy(RpcProxy<PagingLoadResult<SemanaLetivaDTO>> proxy) {
		gridPanel.setProxy(proxy);
	}
	
}
